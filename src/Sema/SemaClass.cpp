#include "cdotc/Sema/SemaPass.h"

#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Basic/DependencyGraph.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Message/Diagnostics.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/Casting.h"

#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult PrepareRecordInterfaceQuery::run()
{
   auto* Rec = D;
   if (QC.PrepareNameLookup(Rec)) {
      return fail();
   }

   auto& Meta = QC.RecordMeta[Rec];
   Meta.Opaque = Rec->hasAttribute<OpaqueAttr>();

   // FIXME constraints

   auto* Ty = QC.Sema->Context.getRecordType(Rec);
   Ty->setDependent(Ty->isDependentType() || Rec->isInUnboundedTemplate());

   if (Rec->isInvalid()) {
      return finish(DoneWithError);
   }

//   if (QC.CheckConformances(Ty)) {
//      return finish(DoneWithError);
//   }

   return finish();
}

QueryResult TypecheckRecordQuery::run()
{
   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   return finish();
}

QueryResult DeclareImplicitInitializersQuery::run()
{
   finish();

   if (isa<ProtocolDecl>(R)) {
      return finish();
   }

   if (!R->getDeinitializer()) {
      if (QC.DeclareImplicitDefaultDeinit(R)) {
         return fail();
      }
   }

   auto* S = dyn_cast<StructDecl>(R);
   if (!S || QC.RecordMeta[S].Opaque) {
      return finish();
   }

   if (QC.DeclareImplicitDefaultInit(S)) {
      return fail();
   }

   bool HasMemberWiseInit;
   if (S->isInstantiation()) {
      HasMemberWiseInit = cast<StructDecl>(S->getSpecializedTemplate())
                              ->getMemberwiseInitializer()
                          != nullptr;
   }
   else {
      HasMemberWiseInit
          = S->hasExplicitMemberwiseInit()
            || (!S->isClass()
                && S->decl_begin<InitDecl>() == S->decl_end<InitDecl>());
   }

   if (HasMemberWiseInit) {
      if (QC.DeclareMemberwiseInit(S)) {
         return fail();
      }
   }

   return finish();
}

QueryResult DeclareImplicitDefaultInitQuery::run()
{
   auto Name = &QC.Context.getIdentifiers().get("__default_init");
   if (auto* MF = S->getModFile()) {
      MF->PerformExternalLookup(*S, Name);

      auto Result = S->lookup(Name);
      if (!Result.empty()) {
         S->setDefaultInitializer(cast<MethodDecl>(Result.front()));
         return finish();
      }
   }

   SourceType RetTy(QC.Context.getVoidType());
   auto Decl = MethodDecl::Create(
       QC.Context, AccessSpecifier::Private, S->getSourceLoc(), Name, RetTy,
       {QC.Sema->MakeSelfArg(S->getSourceLoc())}, {}, nullptr, false);

   QC.Sema->ActOnDecl(S, Decl);
   S->setDefaultInitializer(Decl);

   Decl->setSynthesized(true);
   Decl->setDefaultInitializer(true);
   Decl->setMutating(true);

   if (S->isInstantiation()) {
      Decl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
                            ->getDefaultInitializer()
                            ->getMethodID());
   }

   finish();
   return QC.PrepareDeclInterface(Decl);
}

QueryResult DeclareMemberwiseInitQuery::run()
{
   SmallVector<FuncArgDecl*, 4> args;
   for (auto F : S->getFields()) {
      if (!F->isStatic() && !F->getDefaultVal()) {
         SourceType T;
         if (QC.PrepareDeclInterface(F)) {
            T = QC.Context.getErrorTy();
         }
         else {
            T = F->getType();
         }

         auto* Lbl = F->getDeclName().getIdentifierInfo();
         auto arg = FuncArgDecl::Create(
             QC.Context, S->getSourceLoc(), S->getSourceLoc(), Lbl, Lbl,
             ArgumentConvention::Owned, T, nullptr, F->isVariadic());

         args.push_back(arg);
      }
   }

   auto MDecl = InitDecl::Create(QC.Context, AccessSpecifier::Public,
                                 S->getSourceLoc(), args, {}, nullptr);

   QC.Sema->ActOnDecl(S, MDecl);
   S->setMemberwiseInitializer(MDecl);

   MDecl->setSynthesized(true);
   MDecl->setMemberwiseInitializer(true);

   if (S->isInstantiation()) {
      MDecl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
                             ->getMemberwiseInitializer()
                             ->getMethodID());
   }

   finish();
   return QC.PrepareDeclInterface(MDecl);
}

QueryResult DeclareImplicitDefaultDeinitQuery::run()
{
   auto DDecl = DeinitDecl::Create(QC.Context, S->getSourceLoc(), nullptr, {});
   QC.Sema->ActOnDecl(S, DDecl);

   DDecl->setSynthesized(true);
   DDecl->setReturnType(SourceType(QC.Context.getVoidType()));

   if (S->isInstantiation()) {
      DDecl->setMethodID(
          S->getSpecializedTemplate()->getDeinitializer()->getMethodID());
   }

   finish();
   return QC.PrepareDeclInterface(DDecl);
}

QueryResult PrepareStructInterfaceQuery::run()
{
   if (QC.PrepareRecordInterface(D)) {
      return fail();
   }

   if (QC.DeclareImplicitInitializers(D)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckStructQuery::run()
{
   if (QC.TypecheckRecord(D)) {
      return fail();
   }

   return finish();
}

static bool resolveBaseClass(QueryContext& QC, ClassDecl* D)
{
   const SourceType& BaseTypeSource = D->getParentType();
   if (!BaseTypeSource) {
      return false;
   }

   TypeResult BaseResult = QC.Sema->visitSourceType(D, BaseTypeSource);
   if (!BaseResult) {
      return true;
   }

   QualType BaseTy = BaseResult.get();
   if (BaseTy->isDependentType()) {
      return false;
   }

   if (!BaseTy->isRecordType()) {
      QC.Sema->diagnose(BaseTypeSource.getTypeExpr(), err_generic_error,
                        "cannot extend non-class " + BaseTy.toDiagString());

      return true;
   }

   auto* BaseClass = dyn_cast<ClassDecl>(BaseTy->getRecord());
   if (!BaseClass) {
      QC.Sema->diagnose(BaseTypeSource.getTypeExpr(), err_generic_error,
                        "cannot extend non-class " + BaseTy.toDiagString());

      return true;
   }

   D->inherit(BaseClass);
   D->getStoredFields().insert(D->getStoredFields().begin(),
                               BaseClass->getStoredFields().begin(),
                               BaseClass->getStoredFields().end());

   return false;
}

bool SemaPass::canUseClass(SourceLocation Loc)
{
   if (canUseClassVal.hasValue()) {
      return canUseClassVal.getValue();
   }

   const char* missingFn = nullptr;
   if (!getAtomicReleaseDecl()) {
      missingFn = "_cdot_AtomicRelease";
   }
   else if (!getCopyClassDecl()) {
      missingFn = "_cdot_CopyClass";
   }
   else if (!getPureVirtualDecl()) {
      missingFn = "_cdot_PureVirtual";
   }

   canUseClassVal = missingFn == nullptr;

   if (missingFn != nullptr) {
      diagnose(
          err_generic_error,
          std::string(
              "'class' cannot be used because the builtin declaration for '")
              + missingFn + "' is missing",
          Loc);

      auto& options = getCompilationUnit().getOptions();
      if (options.noPrelude()) {
         StringRef commandLineArgs = options.getCommandLineArguments();
         size_t offset = commandLineArgs.find("-no-prelude");

         // Max length: 80
         if (commandLineArgs.size() > 80) {
            commandLineArgs = commandLineArgs.substr(offset - 35, 80);
            offset = commandLineArgs.find("-no-prelude");
         }

         SourceLocation optLoc(offset);
         diagnose(note_generic_note,
                  "try removing the command line argument '-no-prelude'",
                  FakeSourceLocation{commandLineArgs.str()},
                  SourceRange(optLoc, optLoc.offsetBy(11)));
      }
   }

   return canUseClassVal.getValue();
}

QueryResult PrepareClassInterfaceQuery::run()
{
   if (!QC.Sema->canUseClass(D->getSourceLoc())) {
      return fail();
   }

   if (resolveBaseClass(QC, D)) {
      return fail();
   }

   if (auto* Base = D->getParentClass()) {
      if (QC.PrepareClassInterface(Base)) {
         return fail();
      }
   }

   if (QC.PrepareStructInterface(D)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckClassQuery::run()
{
   if (QC.TypecheckRecord(D)) {
      return fail();
   }

   return finish();
}

QueryResult ResolveRawTypeQuery::run()
{
   QualType CaseValTy;
   if (auto& Ty = E->getRawType()) {
      auto res = QC.Sema->visitSourceType(E, Ty);
      if (!res) {
         return fail();
      }

      if (res.get()->isIntegerType()) {
         CaseValTy = res.get();
      }
      else if (res.get()->isRecordType()) {
         ProtocolDecl* Eq;
         if (QC.GetBuiltinProtocol(Eq, GetBuiltinProtocolQuery::Equatable))
            llvm_unreachable("no 'Equatable' declaration!");

         bool Conforms = QC.Sema->ConformsTo(res.get(), Eq);
         if (!Conforms) {
            QC.Sema->diagnose(err_generic_error,
                              "enum raw type must conform to 'Equatable', "
                                  + res.get()->toString() + " given",
                              E->getSourceLoc());
         }
         else {
            CaseValTy = res.get();
         }
      }
      else {
         QC.Sema->diagnose(err_generic_error,
                           "enum raw type must conform to 'Equatable'"
                               + res.get()->toString() + " given",
                           E->getSourceLoc());
      }
   }

   if (!CaseValTy) {
      if (QC.CI.getOptions().noPrelude()) {
         CaseValTy = QC.Context.getIntTy();
         E->getRawType().setResolvedType(CaseValTy);
      }
      else {
         RecordDecl* IntDecl;
         if (QC.GetBuiltinRecord(IntDecl, GetBuiltinRecordQuery::Int64)) {
            llvm_unreachable("no 'Int' declaration!");
         }

         CaseValTy = QC.Context.getRecordType(IntDecl);
         E->getRawType().setResolvedType(CaseValTy);
      }
   }

   return finish();
}

static bool mustProvideValue(QueryContext& QC, QualType RawType)
{
   if (RawType->isIntegerType()) {
      return false;
   }

   auto* R = RawType->asRecordType();
   if (!R) {
      return true;
   }

   return !QC.IsBuiltinIntegerType(R->getRecord());
}

static void synthesizeValue(SemaPass& Sema, EnumCaseDecl* Case,
                            QualType RawType, int64_t RawVal)
{
   if (auto* R = RawType->asRecordType()) {
      auto *F = cast<StructDecl>(R->getRecord())->getFields().front();
      if (!Sema.QC.PrepareDeclInterface(F)) {
         RawType = F->getType();
      }
      else {
         RawType = Sema.Context.getIntTy();
      }
   }

   Case->setILValue(Sema.getILGen().Builder.GetConstantInt(RawType, RawVal));
}

QueryResult PrepareEnumInterfaceQuery::run()
{
   auto& SP = *QC.Sema;
   if (auto Err = QC.ResolveRawType(D)) {
      return Query::finish(Err);
   }

   QualType CaseValTy = D->getRawType();
   int64_t NextCaseVal = 0ll;
   llvm::DenseMap<int64_t, EnumCaseDecl*> caseVals;

   for (const auto& Case : D->getCases()) {
      if (auto expr = Case->getRawValExpr()) {
         expr->setContextualType(CaseValTy);

         (void)SP.getAsOrCast(D, expr, CaseValTy);
         if (expr->isInvalid())
            continue;

         auto res = SP.evalStaticExpr(D, expr);
         if (!res)
            continue;

         Case->setRawValue(NextCaseVal);
      }
      else {
         if (mustProvideValue(QC, CaseValTy)) {
            SP.diagnose(D, err_generic_error,
                        "cannot synthesize for enum case with raw value "
                        "type "
                            + CaseValTy.toString(),
                        Case->getSourceLoc());
         }

         Case->setRawValue(NextCaseVal);
      }

      synthesizeValue(SP, Case, CaseValTy, NextCaseVal);

      auto it = caseVals.find(Case->getRawValue());
      if (it != caseVals.end()) {
         SP.diagnose(Case, err_generic_error,
                     "duplicate case value " + std::to_string(NextCaseVal),
                     Case->getSourceLoc());
         SP.diagnose(note_duplicate_case, 0, it->getSecond()->getSourceLoc());
      }

      caseVals.try_emplace(NextCaseVal++, Case);
   }

   if (QC.PrepareRecordInterface(D)) {
      return fail();
   }

   for (const auto& Case : D->getCases()) {
      if (QC.PrepareDeclInterface(Case)) {
         return fail();
      }
   }

   if (!D->getDeinitializer() && !D->isRawEnum()) {
      QC.DeclareImplicitDefaultDeinit(D);
   }

   return finish();
}

QueryResult TypecheckEnumQuery::run()
{
   if (QC.TypecheckRecord(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareEnumCaseInterfaceQuery::run()
{
   for (FuncArgDecl* ArgDecl : D->getArgs()) {
      if (auto Err = QC.TypecheckDecl(ArgDecl)) {
         if (!Err.isDependent()) {
            return Query::finish(Err);
         }
      }

      D->copyStatusFlags(ArgDecl);
   }

   D->setReturnType(SourceType(QC.Sema->Context.getRecordType(D->getRecord())));
   D->createFunctionType(sema());

   return finish();
}

QueryResult TypecheckEnumCaseQuery::run()
{
   llvm_unreachable("don't call this!");
}

QueryResult PrepareProtocolInterfaceQuery::run()
{
   if (QC.PrepareRecordInterface(D)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckProtocolQuery::run()
{
   if (QC.TypecheckRecord(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareExtensionInterfaceQuery::run()
{
   NamedDecl* ExtendedDecl = D->getExtendedRecord();
   if (!ExtendedDecl) {
      if (QC.GetExtendedDecl(ExtendedDecl, D->getExtendedType())) {
         return fail();
      }

      if (auto* R = dyn_cast<RecordDecl>(ExtendedDecl)) {
         D->setExtendedRecord(R);
         R->addExtension(D);
      }
   }

   if (auto* P = dyn_cast_or_null<ProtocolDecl>(ExtendedDecl)) {
      if (auto Err = QC.PrepareDeclInterface(ExtendedDecl)) {
         return Query::finish(Err);
      }

      finish();

      QC.Sema->checkProtocolExtension(D, P);
      if (D->isInvalid()) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckExtensionQuery::run()
{
   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareMethodInterfaceQuery::run()
{
   // `mutating` is not allowed on class methods.
   if (D->hasMutableSelf() && isa<ClassDecl>(D->getDeclContext())) {
      QC.Sema->diagnose(D, err_mutating_on_class, D->getSourceLoc());
      D->setMutating(false);
   }

   if (QC.PrepareCallableInterface(D)) {
      return fail();
   }

   // Check if this is the `copy` function of this type.
   if (D->getDeclName().isStr("copy") && !D->isStatic()
       && D->getArgs().size() == 1 && D->getReturnType()->isRecordType()
       && D->getReturnType()->getRecord() == D->getRecord()) {
      QC.RecordMeta[D->getRecord()].CopyFn = D;
   }

   // check virtual and override methods after all signatures (most
   // importantly those of base classes) are available.
   QC.Sema->checkVirtualOrOverrideMethod(D);

   return finish();
}

QueryResult TypecheckMethodQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D)) {
      return fail();
   }

   return finish();
}

namespace {

class ParamTypeVisitor : public RecursiveTypeVisitor<ParamTypeVisitor>,
                         public RecursiveASTVisitor<ParamTypeVisitor> {
   SmallPtrSetImpl<TemplateParamDecl*>& Params;

public:
   explicit ParamTypeVisitor(SmallPtrSetImpl<TemplateParamDecl*>& Params)
       : Params(Params)
   {
   }

   bool visitTemplateParamType(const TemplateParamType* T)
   {
      for (auto P : Params) {
         if (P->getName() == T->getTemplateParamTypeName()) {
            Params.erase(P);
         }
      }

      return false;
   }

   bool visitDependentSizeArrayType(const DependentSizeArrayType* T)
   {
      RecursiveASTVisitor::visit(T->getSizeExpr());
      return true;
   }

   bool visitIdentifierRefExpr(IdentifierRefExpr* E)
   {
      if (E->getParentExpr())
         return false;

      if (E->getKind() == IdentifierKind::TemplateParam)
         Params.erase(E->getTemplateParam());

      return false;
   }
};

} // anonymous namespace

QueryResult AssignInitNameQuery::run()
{
   auto& Context = QC.CI.getContext();

   QualType RecordTy = Context.getRecordType(Init->getRecord());
   if (!Init->getDeclName()) {
      auto DeclName = Context.getDeclNameTable().getConstructorName(RecordTy);

      Init->setName(DeclName);
      QC.Sema->makeDeclAvailable(*Init->getRecord(), DeclName, Init);
   }

   return finish();
}

QueryResult PrepareInitInterfaceQuery::run()
{
   auto R = D->getRecord();

   if (D->getArgs().empty() && isa<StructDecl>(R) && D->isCompleteInitializer()
       && !D->isMemberwise()) {
      cast<StructDecl>(R)->setParameterlessConstructor(D);
   }

   if (QC.RecordMeta[R].Opaque) {
      QC.Sema->diagnose(R, err_generic_error,
                        "opaque type may not contain an initializer",
                        R->getSourceLoc());

      return fail();
   }

   auto& Context = QC.CI.getContext();

   QualType RecordTy = Context.getRecordType(D->getRecord());
   D->setReturnType(SourceType(Context.getVoidType()));

   if (QC.AssignInitName(D)) {
      return fail();
   }

   if (D->isDefaultInitializer() && D->isCompleteInitializer()) {
      cast<StructDecl>(R)->setDefaultInitializer(D);
   }

   if (D->isMemberwiseInitializer() && D->isCompleteInitializer()) {
      cast<StructDecl>(R)->setMemberwiseInitializer(D);
   }

   if (QC.PrepareCallableInterface(D)) {
      return fail();
   }

   SmallPtrSet<TemplateParamDecl*, 4> Params;
   Params.insert(D->getTemplateParams().begin(), D->getTemplateParams().end());

   ParamTypeVisitor V(Params);
   for (auto& arg : D->getArgs()) {
      V.RecursiveTypeVisitor::visit(arg->getType().getResolvedType());
   }

   if (!Params.empty()) {
      QC.Sema->diagnose(D, err_initializer_templ_args_must_be_inferrable,
                        0 /*initializer*/, (*Params.begin())->getDeclName(),
                        (*Params.begin())->getSourceRange());
   }

   if (!isa<ClassDecl>(R)) {
      D->setMutating(true);
   }

   // Declare base initializer.
   if (D->isCompleteInitializer()) {
      QualType SelfTy = Context.getRecordType(R);
      D->setSelfType(SelfTy);

      QC.CreateBaseInit(D);
   }

   if (D->isFallible()) {
      D->setOptionTy(QC.Sema->getOptionOf(RecordTy, D));
   }

   return finish();
}

QueryResult CreateBaseInitQuery::run()
{
   if (D->isBaseInitializer()) {
      return finish();
   }

   QualType RecordTy = QC.Context.getRecordType(D->getRecord());
   auto BaseInitName
       = QC.Context.getDeclNameTable().getConstructorName(RecordTy, false);

   SmallVector<FuncArgDecl*, 4> Args{QC.Sema->MakeSelfArg(D->getSourceLoc())};
   Args.append(D->getArgs().begin(), D->getArgs().end());

   ASTVector<TemplateParamDecl*> Params(QC.Context, D->getTemplateParams());
   auto BaseD = InitDecl::Create(QC.Context, D->getAccess(), D->getSourceLoc(),
                                 Args, move(Params), D->getBody(), BaseInitName,
                                 D->isFallible());

   BaseD->setCompleteInit(D);
   BaseD->setSynthesized(true);
   BaseD->setIsInstantiation(D->isInstantiation());
   BaseD->setInstantiationInfo(D->getInstantiationInfo());
   BaseD->setMutating(true);

   if (auto* init = dyn_cast_or_null<InitDecl>(D->getBodyTemplate())) {
      BaseD->setBodyTemplate(init->getBaseInit());
   }

   QC.Sema->ActOnDecl(D->getDeclContext(), BaseD);
   QC.PrepareInitInterface(BaseD);

   D->setBaseInit(BaseD);

   return finish();
}

QueryResult TypecheckInitQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareDeinitInterfaceQuery::run()
{
   auto& Context = QC.CI.getContext();

   D->setReturnType(SourceType(Context.getVoidType()));
   D->setMutating(!isa<ClassDecl>(D->getRecord()));

   QualType SelfTy = Context.getRecordType(D->getRecord());
   D->setSelfType(SelfTy);

   if (!D->getDeclName()) {
      auto DeclName = Context.getDeclNameTable().getDestructorName(SelfTy);
      D->setName(DeclName);
      QC.Sema->makeDeclAvailable(*D->getRecord(), DeclName, D);
   }

   if (QC.PrepareCallableInterface(D)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckDeinitQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareFieldInterfaceQuery::run()
{
   FieldDecl* F = D;
   auto R = F->getRecord();

   if (!F->isStatic()) {
      if (isa<EnumDecl>(R)) {
         QC.Sema->diagnose(F, err_generic_error,
                           "enums may only contain static fields");
      }
      if (isa<ProtocolDecl>(R)) {
         QC.Sema->diagnose(F, err_generic_error,
                           "protocols may only contain static fields");
      }
      if (auto U = dyn_cast<UnionDecl>(R)) {
         if (U->isConst() && !F->isConst())
            QC.Sema->diagnose(F, err_generic_error,
                              "expected union field to be constant");

         U->isConst(F->isConst());
      }
   }

   auto& fieldType = F->getType();

   auto res = QC.Sema->visitSourceType(F, fieldType);
   if (!res) {
      F->setIsInvalid(true);
      R->setIsInvalid(true);
      return fail();
   }

   if (fieldType->isAutoType()) {
      // Mark this query as finished.
      finish();

      // We need to visit the default value expression now.
      if (QC.TypecheckDecl(D)) {
         F->setIsInvalid(true);
         R->setIsInvalid(true);
         return fail();
      }
   }

   if (F->isVariadic()) {
      if (!F->getType().getTypeExpr()) {
         QC.Sema->diagnose(F, err_variadic_field_type, F->getSourceRange());
         F->setVariadic(false);
      }
      else if (!F->getType().getTypeExpr()->isVariadicArgPackExpansion()) {
         QC.Sema->diagnose(F, err_variadic_field_type,
                           F->getType().getTypeExpr()->getSourceRange());
         F->setVariadic(false);
      }
   }

   if (auto Prop = F->getAccessor()) {
      if (F->isConst() && Prop->hasSetter()) {
         QC.Sema->diagnose(F, err_constant_field_setter);
      }

      Prop->setType(F->getType());
      if (QC.PrepareDeclInterface(Prop)) {
         F->setIsInvalid(true);
         R->setIsInvalid(true);
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckFieldQuery::run()
{
   auto* F = D;

   auto& fieldType = F->getType();
   if (auto defaultVal = F->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      SemaPass::DeclScopeRAII DSR(sema(), F->getRecord());

      ExprResult typeRes = QC.Sema->typecheckExpr(defaultVal, fieldType, F);
      if (typeRes) {
         F->setValue(typeRes.get());

         auto givenType = typeRes.get()->getExprType();
         if (fieldType->isAutoType()) {
            fieldType.setResolvedType(givenType);
         }
      }
   }

   if (auto Acc = F->getAccessor()) {
      return finish(QC.TypecheckDecl(Acc));
   }

   return finish();
}

QueryResult PreparePropInterfaceQuery::run()
{
   auto* Decl = D;
   auto R = Decl->getRecord();

   auto res = QC.Sema->visitSourceType(Decl, Decl->getType());
   if (!res) {
      return fail();
   }

   Decl->setType(res.get());

   if (Decl->isProtocolRequirement() && !Decl->isProtocolDefaultImpl()) {
      QualType Type = Decl->getType();
      if (QC.Sema->ContainsAssociatedTypeConstraint(Type)) {
         cast<ProtocolDecl>(R)->setHasAssociatedTypeConstraint(true);
      }
   }

   if (auto* Getter = Decl->getGetterMethod()) {
      Getter->setSynthesized(true);
      Getter->setProperty(true);
      Getter->setReturnType(Decl->getType());

      if (D->isReadWrite() && !Decl->getSetterMethod()) {
         auto* Ptr = QC.Sema->getUnsafeMutablePtrDecl();
         if (!Ptr) {
            QC.Sema->diagnose(err_no_builtin_decl,
                              BuiltinFeature::ReadWriteProperty,
                              Getter->getSourceLoc());

            return fail();
         }

         TemplateArgument TA(Ptr->getTemplateParams().front(), D->getType(),
                             Getter->getSourceLoc());

         auto* Args = FinalTemplateArgumentList::Create(QC.Context, TA);

         if (!TA.isStillDependent()) {
            RecordDecl* Inst = QC.Sema->getInstantiator().InstantiateRecord(
               Ptr, Args, Getter->getSourceLoc());

            if (!Inst) {
               return fail();
            }

            Getter->setReturnType(SourceType(QC.Context.getRecordType(Inst)));
         }
         else {
            Getter->setReturnType(
                SourceType(QC.Context.getDependentRecordType(Ptr, Args)));
         }
      }

      Getter->setLexicalContext(Decl->getLexicalContext());
      Getter->setLogicalContext(Decl->getDeclContext());

      if (auto Template = Decl->getPropTemplate()) {
         Getter->setBodyTemplate(Template->getGetterMethod());
      }

      Decl->setGetterMethod(Getter);

      if (QC.PrepareDeclInterface(Getter)) {
         return fail();
      }
   }

   if (auto* Setter = Decl->getSetterMethod()) {
      Setter->setSynthesized(true);
      Setter->setProperty(true);
      Setter->getArgs()[1]->setType(Decl->getType());

      Setter->setLexicalContext(Decl->getLexicalContext());
      Setter->setLogicalContext(Decl->getDeclContext());

      if (isa<ClassDecl>(R)) {
         Setter->setMutating(false);
      }

      if (auto Template = Decl->getPropTemplate()) {
         Setter->setBodyTemplate(Template->getSetterMethod());
      }

      Decl->setSetterMethod(Setter);

      if (QC.PrepareDeclInterface(Setter)) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckPropQuery::run()
{
   if (auto* Getter = D->getGetterMethod()) {
      if (QC.TypecheckDecl(Getter)) {
         return fail();
      }
   }

   if (auto* Setter = D->getSetterMethod()) {
      if (QC.TypecheckDecl(Setter)) {
         return fail();
      }
   }

   return finish();
}

QueryResult PrepareSubscriptInterfaceQuery::run()
{
   auto* Decl = D;
   auto R = Decl->getRecord();

   auto res = QC.Sema->visitSourceType(Decl, Decl->getType());
   if (!res)
      return fail();

   if (Decl->isProtocolRequirement() && !Decl->isProtocolDefaultImpl()) {
      QualType Type = Decl->getType();
      if (QC.Sema->ContainsAssociatedTypeConstraint(Type)) {
         cast<ProtocolDecl>(R)->setHasAssociatedTypeConstraint(true);
      }
   }

   if (auto* Getter = Decl->getGetterMethod()) {
      Getter->setSynthesized(true);
      Getter->setSubscript(true);

      Getter->setReturnType(res.get());
      Getter->setLexicalContext(Decl->getLexicalContext());
      Getter->setLogicalContext(Decl->getDeclContext());

      if (auto Template = Decl->getTemplate())
         Getter->setBodyTemplate(Template->getGetterMethod());

      Decl->setGetterMethod(Getter);

      if (QC.PrepareDeclInterface(Getter)) {
         return fail();
      }
   }

   if (auto* Setter = Decl->getSetterMethod()) {
      auto *newVal = Setter->getArgs().back();
      newVal->setType(res.get());
      cast<BuiltinExpr>(newVal->getDefaultVal())->setType(res.get());

      Setter->setSynthesized(true);
      Setter->setSubscript(true);

      Setter->setLexicalContext(Decl->getLexicalContext());
      Setter->setLogicalContext(Decl->getDeclContext());

      if (isa<ClassDecl>(R)) {
         Setter->setMutating(false);
      }

      if (auto Template = Decl->getTemplate())
         Setter->setBodyTemplate(Template->getSetterMethod());

      Decl->setSetterMethod(Setter);

      if (QC.PrepareDeclInterface(Setter)) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckSubscriptQuery::run()
{
   if (auto* Getter = D->getGetterMethod()) {
      if (QC.TypecheckDecl(Getter)) {
         return fail();
      }
   }

   if (auto* Setter = D->getSetterMethod()) {
      if (QC.TypecheckDecl(Setter)) {
         return fail();
      }
   }

   return finish();
}

void SemaPass::checkVirtualOrOverrideMethod(MethodDecl* M)
{
   if (M->isSynthesized() || isa<InitDecl>(M) || isa<DeinitDecl>(M)
       || M->isStatic() || M->inDependentContext())
      return;

   if (!isa<ClassDecl>(M->getRecord())) {
      // virtual methods may only appear in a class declaration
      if (M->isVirtualOrOverride()) {
         if (!isa<ClassDecl>(M->getRecord())) {
            diagnose(M, err_virt_method_outside_class, M->isOverride(),
                     M->getSourceLoc());
         }
      }

      return;
   }

   if (M->isVirtualOrOverride()) {
      // Virtual methods need to be instantiated for the VTable.
      maybeInstantiateMemberFunction(M, M);
   }

   auto Base = cast<ClassDecl>(M->getRecord())->getParentClass();
   MethodDecl* OverridenMethod = nullptr;
   MethodDecl* Candidate = nullptr;
   int EC = -1;

   while (Base && !OverridenMethod) {
      const SingleLevelLookupResult* Candidates;
      if (QC.LookupFirst(Candidates, Base, M->getDeclName())) {
         return;
      }

      for (auto& Cand : *Candidates) {
         auto BaseMethod = dyn_cast<MethodDecl>(Cand);
         if (!BaseMethod) {
            continue;
         }

         if (QC.PrepareDeclInterface(BaseMethod)) {
            continue;
         }

         auto Err = signaturesCompatible(M, BaseMethod);
         if (Err < 0) {
            OverridenMethod = BaseMethod;
            break;
         }
         else {
            Candidate = BaseMethod;
            EC = Err;
         }
      }

      Base = Base->getParentClass();
   }

   // all good.
   if (!OverridenMethod && !M->isOverride()) {
      return;
   }

   if (!OverridenMethod) {
      diagnose(M, err_override_no_matching_method, M->getSourceLoc(),
               Candidate == nullptr);

      if (Candidate) {
         assert(EC != -1 && "no error code?");
         diagnose(note_override_cand_not_viable, Candidate->getSourceLoc(), EC);
      }
   }
   else if (!M->isOverride()) {
      diagnose(M, err_overriding_decl_requires_override, M->getSourceLoc());
      diagnose(note_declared_here, OverridenMethod->getSourceLoc());
   }
   else if (!OverridenMethod->isVirtualOrOverride()) {
      diagnose(M, err_overriden_method_not_virtual, M->getDeclName(),
               M->getSourceLoc());

      diagnose(note_declared_here, OverridenMethod->getSourceLoc());
   }

   M->setOverridenMethod(OverridenMethod);
}

void SemaPass::checkIfAbstractMethodsOverridden(ClassDecl* R)
{
   if (R->isAbstract())
      return;

   auto* Base = R->getParentClass();
   while (Base) {
      for (auto* M : Base->getDecls<MethodDecl>()) {
         if (!M->isAbstract())
            continue;

         bool found = false;
         const SingleLevelLookupResult* Candidates;
         if (QC.LookupFirst(Candidates, R, M->getDeclName())) {
            return;
         }

         for (auto& Cand : *Candidates) {
            auto BaseMethod = dyn_cast<MethodDecl>(Cand);
            if (!BaseMethod || BaseMethod->isAbstract())
               continue;

            auto Err = signaturesCompatible(M, BaseMethod);
            if (Err < 0) {
               found = true;
               break;
            }
         }

         if (!found) {
            diagnose(R, err_abstract_must_be_overriden, R->getDeclName(),
                     M->getDeclName(), Base->getDeclName(), R->getSourceLoc());

            diagnose(note_declared_here, M->getSourceLoc());
         }
      }

      Base = Base->getParentClass();
   }
}

LLVM_ATTRIBUTE_UNUSED
static bool diagnoseCircularDependency(SemaPass& SP,
                                       DependencyGraph<NamedDecl*>& Dep)
{
   auto pair = Dep.getOffendingPair();

   // this pair should contain one RecordDecl and either a FieldDecl or an
   // EnumCaseDecl
   RecordDecl* R = nullptr;
   NamedDecl* FieldOrCase = nullptr;

   if (isa<RecordDecl>(pair.first)) {
      R = cast<RecordDecl>(pair.first);
   }
   else if (isa<EnumCaseDecl>(pair.first)) {
      FieldOrCase = cast<EnumCaseDecl>(pair.first);
   }
   else {
      FieldOrCase = cast<FieldDecl>(pair.first);
   }

   if (isa<RecordDecl>(pair.second)) {
      R = cast<RecordDecl>(pair.second);
   }
   else if (isa<EnumCaseDecl>(pair.second)) {
      FieldOrCase = cast<EnumCaseDecl>(pair.second);
   }
   else {
      FieldOrCase = cast<FieldDecl>(pair.second);
   }

   assert(R && FieldOrCase && "bad dependency pair!");

   SP.diagnose(R, err_circular_data_members, R->getDeclName(),
               FieldOrCase->getRecord()->getDeclName(), R->getSourceLoc());

   SP.diagnose(note_other_field_here, FieldOrCase->getSourceLoc());

   return true;
}

LLVM_ATTRIBUTE_UNUSED
static bool diagnoseCircularConformance(SemaPass& SP,
                                        DependencyGraph<ProtocolDecl*>& Dep)
{
   auto Pair = Dep.getOffendingPair();
   SP.diagnose(Pair.first, err_circular_conformance, Pair.first->getSourceLoc(),
               Pair.first->getDeclName(), Pair.second->getDeclName());

   return true;
}

static bool shouldAddEquatableConformance(RecordDecl* R)
{
   auto* Attr = R->getAttribute<NoDeriveAttr>();
   if (!Attr) {
      return true;
   }

   return Attr->getKind() != NoDeriveAttr::Equatable
          && Attr->getKind() != NoDeriveAttr::_All;
}

static bool shouldAddCopyableConformance(RecordDecl* R)
{
   auto* Attr = R->getAttribute<NoDeriveAttr>();
   if (!Attr) {
      return true;
   }

   return Attr->getKind() != NoDeriveAttr::Copyable
          && Attr->getKind() != NoDeriveAttr::_All;
}

static bool shouldAddRawRepresentableConformance(RecordDecl* R)
{
   auto* Attr = R->getAttribute<NoDeriveAttr>();
   if (!Attr) {
      return true;
   }

   return Attr->getKind() != NoDeriveAttr::RawRepresentable
          && Attr->getKind() != NoDeriveAttr::_All;
}

static void checkCopyableConformances(SemaPass& SP, RecordDecl* S,
                                      bool AllCopyable,
                                      bool AllImplicitlyCopyable)
{
   if (isa<ClassDecl>(S)) {
      return;
   }

   // Types that conform to MoveOnly do not get a synthesized copy function.
   auto& Context = SP.getContext();
   auto& ConfTable = Context.getConformanceTable();

   if (!shouldAddCopyableConformance(S)) {
      return;
   }

   auto* MoveOnly = SP.getMoveOnlyDecl();
   if (MoveOnly && ConfTable.conformsTo(S, MoveOnly)) {
      return;
   }

   // If not all types are copyable, we can't synthesize a copy function.
   // Instead synthesize a conformance to MoveOnly.
   if (!AllCopyable) {
      if (MoveOnly) {
         ConfTable.addExplicitConformance(Context, S, MoveOnly);
      }

      return;
   }
   if (auto* Copyable = SP.getCopyableDecl()) {
      auto NewConformance
          = ConfTable.addExplicitConformance(Context, S, Copyable);

      if (NewConformance) {
         SP.QC.CheckSingleConformance(Context.getRecordType(S), Copyable);
      }
   }

   // If all types are implicitly copyable, synthesize an ImplicitlyCopyable
   // conformance.
   if (!AllImplicitlyCopyable) {
      return;
   }
   if (auto* ImpCopyable = SP.getImplicitlyCopyableDecl()) {
      auto NewConformance
          = ConfTable.addExplicitConformance(Context, S, ImpCopyable);

      if (NewConformance) {
         SP.addDependency(S, ImpCopyable);
      }
   }
}

static void addRawRepresentableConformance(SemaPass& Sema, EnumDecl* E)
{
   SourceLocation Loc = E->getSourceLoc();

   // Make sure the raw type is resolved.
   if (auto Err = Sema.QC.ResolveRawType(E)) {
      return;
   }

   SourceType Ty(E->getRawType());
   SourceType MetaTy(Sema.Context.getMetaType(E->getRawType()));

   // alias RawType
   auto* typeExpr = new (Sema.Context)
       IdentifierRefExpr(Loc, IdentifierKind::MetaType, MetaTy);
   auto* rawTypeExpr = StaticExpr::Create(Sema.Context, typeExpr);
   auto* RawType = AliasDecl::Create(Sema.Context, Loc, AccessSpecifier::Public,
                                     Sema.getIdentifier("RawType"), MetaTy,
                                     rawTypeExpr, {});

   // init? (rawValue: RawType)
   auto* ArgName = Sema.getIdentifier("rawValue");
   bool UseFallible = !Sema.getCompilationUnit().getOptions().noPrelude();
   auto* Arg
       = FuncArgDecl::Create(Sema.Context, Loc, Loc, ArgName, ArgName,
                             ArgumentConvention::Owned, Ty, nullptr, false);

   auto* Init = InitDecl::Create(
       Sema.Context, AccessSpecifier::Public, Loc, Arg, {}, nullptr,
       Sema.Context.getDeclNameTable().getConstructorName(
           Sema.Context.getRecordType(E)),
       UseFallible);

   // prop rawValue: RawType { get }
   DeclarationName PropName = Sema.Context.getDeclNameTable().getAccessorName(
       *ArgName, DeclarationName::Getter);

   auto* Self = Sema.MakeSelfArg(Loc);
   auto* Getter = MethodDecl::Create(Sema.Context, AccessSpecifier::Public, Loc,
                                     PropName, Ty, Self, {}, nullptr, false);

   auto* Prop = PropDecl::Create(Sema.Context, AccessSpecifier::Public, Loc,
                                 ArgName, Ty, false, false, Getter, nullptr);

   Sema.ActOnDecl(E, RawType);
   Sema.ActOnDecl(E, Init);
   Sema.ActOnDecl(E, Prop);

   const RecordMetaInfo* meta;
   if (Sema.QC.GetRecordMeta(meta, E, false)) {
      return;
   }

   meta->IsImplicitlyRawRepresentable = true;
   meta->FromRawValueInit = Init;
   meta->GetRawValueFn = Getter;
}

QueryResult CheckBuiltinConformancesQuery::run()
{
   finish();

   QC.DeclareSelfAlias(R);

   bool TrivialLayout = true;
   bool AllCopyable = true;
   bool AllImplicitlyCopyable = true;
   bool NeedsRetainOrRelease = isa<ClassDecl>(R);

   auto& Context = QC.Sema->Context;
   auto& TI = Context.getTargetInfo();

   if (auto S = dyn_cast<StructDecl>(R)) {
      unsigned BaseClassFields = 0;
      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto Parent = C->getParentClass()) {
            BaseClassFields = Parent->getNumNonStaticFields();
         }

         TrivialLayout = false;
      }

      ArrayRef<FieldDecl*> Fields = S->getStoredFields();
      for (const auto& F : Fields.drop_front(BaseClassFields)) {
         if (QC.PrepareDeclInterface(F)) {
            continue;
         }

         auto& FieldTy = F->getType();
         if (FieldTy->isDependentType()) {
            continue;
         }

         // FIXME into-query
         TrivialLayout &= TI.isTriviallyCopyable(FieldTy);

         bool FieldNeedsRetain, FieldCopyable, FieldImplicitlyCopyable;
         if (QC.NeedsRetainOrRelease(FieldNeedsRetain, FieldTy)) {
            continue;
         }
         if (QC.IsCopyable(FieldCopyable, FieldTy)) {
            continue;
         }
         if (QC.IsImplicitlyCopyable(FieldImplicitlyCopyable, FieldTy)) {
            continue;
         }

         NeedsRetainOrRelease |= FieldNeedsRetain;
         AllCopyable &= FieldCopyable;
         AllImplicitlyCopyable &= FieldImplicitlyCopyable;
      }

      checkCopyableConformances(*QC.Sema, S, AllCopyable,
                                AllImplicitlyCopyable);
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      bool AllEquatable = true;
      bool AddRawRepresentable = true;

      for (auto C : E->getCases()) {
         if (QC.PrepareDeclInterface(C)) {
            continue;
         }

         if (C->isIndirect()) {
            TrivialLayout = false;
            AddRawRepresentable = false;
            continue;
         }

         for (const auto& Val : C->getArgs()) {
            auto& ArgTy = Val->getType();
            if (ArgTy->isDependentType()) {
               continue;
            }

            bool FieldNeedsRetain, FieldCopyable, FieldImplicitlyCopyable;
            if (QC.NeedsRetainOrRelease(FieldNeedsRetain, ArgTy)) {
               continue;
            }
            if (QC.IsCopyable(FieldCopyable, ArgTy)) {
               continue;
            }
            if (QC.IsImplicitlyCopyable(FieldImplicitlyCopyable, ArgTy)) {
               continue;
            }

            NeedsRetainOrRelease |= FieldNeedsRetain;
            AllCopyable &= FieldCopyable;
            AllImplicitlyCopyable &= FieldImplicitlyCopyable;

            // FIXME into-query
            TrivialLayout &= TI.isTriviallyCopyable(ArgTy);
         }
      }

      if (AllEquatable && shouldAddEquatableConformance(R)) {
         ProtocolDecl* Eq;
         if (QC.GetBuiltinProtocol(Eq, GetBuiltinProtocolQuery::Equatable)) {
            Eq = nullptr;
         }

         if (Eq) {
            QC.AddSingleConformance(R->getType(), Eq,
                                    ConformanceKind::Explicit);
            QC.CheckSingleConformance(R->getType(), Eq);
         }
      }

      if (AddRawRepresentable && shouldAddRawRepresentableConformance(R)) {
         ProtocolDecl* RawRep;
         if (QC.GetBuiltinProtocol(RawRep,
                                   GetBuiltinProtocolQuery::RawRepresentable)) {
            llvm_unreachable("no 'RawRepresentable' declaration!");
         }

         if (!QC.Context.getConformanceTable().conformsTo(E, RawRep)) {
            addRawRepresentableConformance(*QC.Sema, E);

            QC.AddSingleConformance(R->getType(), RawRep,
                                    ConformanceKind::Explicit);
            QC.CheckSingleConformance(R->getType(), RawRep);
         }
      }

      checkCopyableConformances(*QC.Sema, E, AllCopyable,
                                AllImplicitlyCopyable);
   }

   auto& Meta = QC.RecordMeta[R];
   Meta.IsTriviallyCopyable = TrivialLayout;
   Meta.NeedsRetainOrRelease = NeedsRetainOrRelease;

   return finish();
}

QueryResult CalculateRecordSizeQuery::run()
{
   if (QC.PrepareDeclInterface(R)) {
      return fail();
   }

   unsigned OccupiedBytes = 0;
   unsigned short Alignment = 1;

   auto& Context = QC.Sema->Context;
   auto& TI = Context.getTargetInfo();

   if (auto S = dyn_cast<StructDecl>(R)) {
      unsigned BaseClassFields = 0;
      if (auto C = dyn_cast<ClassDecl>(R)) {
         if (auto Base = C->getParentClass()) {
            unsigned BaseSize;
            if (auto Err = QC.CalculateRecordSize(BaseSize, Base)) {
               return Query::finish(Err);
            }

            auto& BaseMeta = QC.RecordMeta[Base];
            assert(BaseMeta.Size && "size not calculated!");

            OccupiedBytes += BaseMeta.Size;
            Alignment = BaseMeta.Alignment;
            BaseClassFields = Base->getNumNonStaticFields();
         }
         else {
            // strong & weak refcount, vtable
            OccupiedBytes += 3 * TI.getPointerSizeInBytes();
         }
      }

      ArrayRef<FieldDecl*> Fields = S->getStoredFields();
      for (const auto& F : Fields.drop_front(BaseClassFields)) {
         if (QC.PrepareDeclInterface(F)) {
            continue;
         }

         auto& FieldTy = F->getType();
         F->setOffset(OccupiedBytes);

         if (FieldTy->isDependentType()) {
            continue;
         }

         unsigned FieldSize;
         if (QC.GetTypeSize(FieldSize, FieldTy)) {
            continue;
         }

         OccupiedBytes += FieldSize;

         unsigned short FieldAlign;
         if (QC.GetTypeAlignment(FieldAlign, FieldTy)) {
            continue;
         }

         if (FieldAlign > Alignment)
            Alignment = FieldAlign;
      }

      if (!OccupiedBytes) {
         OccupiedBytes = TI.getPointerSizeInBytes();
         Alignment = TI.getPointerAlignInBytes();
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      unsigned maxSize = 0;
      unsigned short maxAlign = 1;

      for (auto C : E->getCases()) {
         if (QC.PrepareDeclInterface(C)) {
            continue;
         }

         unsigned CaseSize = 0;
         unsigned short CaseAlign = 1;

         if (C->isIndirect()) {
            CaseSize = TI.getPointerSizeInBytes();
            CaseAlign = TI.getPointerAlignInBytes();
         }
         else
            for (const auto& Val : C->getArgs()) {
               auto& ArgTy = Val->getType();

               unsigned FieldSize;
               if (QC.GetTypeSize(FieldSize, ArgTy)) {
                  continue;
               }

               CaseSize += FieldSize;

               unsigned short FieldAlign;
               if (QC.GetTypeAlignment(FieldAlign, ArgTy)) {
                  continue;
               }

               if (FieldAlign > CaseAlign)
                  CaseAlign = FieldAlign;
            }

         C->setSize(CaseSize);
         C->setAlignment(CaseAlign);

         if (CaseSize > maxSize)
            maxSize = CaseSize;

         if (CaseAlign > maxAlign)
            maxAlign = CaseAlign;
      }

      unsigned RawSize;
      if (QC.GetTypeSize(RawSize, E->getRawType()))
         RawSize = 0;

      unsigned short RawAlign;
      if (QC.GetTypeAlignment(RawAlign, E->getRawType()))
         RawAlign = 0;

      OccupiedBytes += RawSize;
      OccupiedBytes += maxSize;

      Alignment = std::max(RawAlign, maxAlign);
   }
   else if (isa<ProtocolDecl>(R)) {
      RecordDecl* ExistentialContainer;
      if (QC.GetBuiltinRecord(ExistentialContainer,
                              GetBuiltinRecordQuery::ExistentialContainer)) {
         ExistentialContainer = nullptr;
      }

      if (ExistentialContainer) {
         QualType T = QC.Sema->Context.getRecordType(ExistentialContainer);

         unsigned ExistentialSize;
         if (QC.GetTypeSize(ExistentialSize, T))
            ExistentialSize = 0;

         unsigned short ExistentialAlign;
         if (QC.GetTypeAlignment(ExistentialAlign, T))
            ExistentialAlign = 0;

         OccupiedBytes = ExistentialSize;
         Alignment = ExistentialAlign;
      }
   }

   auto& Meta = QC.RecordMeta[R];
   Meta.Size = OccupiedBytes;

   if (auto* Attr = R->getAttribute<AlignAttr>()) {
      llvm_unreachable("TODO!");
   }
   else {
      Meta.Alignment = Alignment;
   }

   return finish(OccupiedBytes);
}

QueryResult AddImplicitConformanceQuery::run()
{
   auto& Meta = QC.RecordMeta[R];
   if (Impl) {
      switch (K) {
      case ImplicitConformanceKind::StringRepresentable:
         Meta.ToStringFn = Impl;
         break;
      case ImplicitConformanceKind::Equatable:
         Meta.OperatorEquals = Impl;
         break;
      case ImplicitConformanceKind::Hashable:
         Meta.HashCodeFn = Impl;
         break;
      case ImplicitConformanceKind::Copyable:
         Meta.CopyFn = Impl;
         break;
      }

      return finish(Impl);
   }

   MethodDecl* M;
   switch (K) {
   case ImplicitConformanceKind::StringRepresentable: {
      if (auto* Fn = Meta.ToStringFn) {
         return finish(Fn);
      }

      auto String = QC.Sema->getStringDecl();
      assert(String && "StringRepresentable without String!");

      auto retTy = SourceType(QC.Context.getRecordType(String));
      DeclarationName DN = QC.Context.getIdentifiers().get("toString");

      M = MethodDecl::Create(
          QC.Context, AccessSpecifier::Public, R->getSourceLoc(), DN, retTy,
          QC.Sema->MakeSelfArg(R->getSourceLoc()), {}, nullptr, false);

      Meta.IsImplicitlyStringRepresentable = true;
      Meta.ToStringFn = M;

      break;
   }
   case ImplicitConformanceKind::Equatable: {
      if (auto* Fn = Meta.OperatorEquals) {
         return finish(Fn);
      }

      RecordDecl* BoolDecl;
      if (auto Err
          = QC.GetBuiltinRecord(BoolDecl, GetBuiltinRecordQuery::Bool)) {
         return Query::finish(Err);
      }

      auto retTy = SourceType(QC.Context.getRecordType(BoolDecl));
      auto argTy = SourceType(QC.Context.getRecordType(R));

      auto* Name = &QC.Context.getIdentifiers().get("that");
      auto Arg = FuncArgDecl::Create(
          QC.Context, R->getSourceLoc(), R->getSourceLoc(), Name, nullptr,
          ArgumentConvention::Borrowed, argTy, nullptr, false);

      OperatorInfo OpInfo;
      OpInfo.setFix(FixKind::Infix);
      OpInfo.setPrecedenceGroup(
          PrecedenceGroup(prec::Equality, Associativity::Right));

      auto& OpName = QC.Context.getIdentifiers().get("==");
      DeclarationName DN
          = QC.Context.getDeclNameTable().getInfixOperatorName(OpName);

      FuncArgDecl* Args[] = {QC.Sema->MakeSelfArg(R->getSourceLoc()), Arg};
      M = MethodDecl::CreateOperator(QC.Context, AccessSpecifier::Public,
                                     R->getSourceLoc(), DN, retTy, Args, {},
                                     nullptr, false);

      Meta.IsImplicitlyEquatable = true;
      Meta.OperatorEquals = M;

      break;
   }
   case ImplicitConformanceKind::Hashable: {
      if (auto* Fn = Meta.HashCodeFn) {
         return finish(Fn);
      }

      RecordDecl* UIntDecl;
      if (auto Err
          = QC.GetBuiltinRecord(UIntDecl, GetBuiltinRecordQuery::UInt64)) {
         return Query::finish(Err);
      }

      auto retTy = SourceType(QC.Context.getRecordType(UIntDecl));
      auto* Name = &QC.Context.getIdentifiers().get("hashCode");

      M = MethodDecl::Create(
          QC.Context, R->getAccess(), R->getSourceLoc(), Name, retTy,
          {QC.Sema->MakeSelfArg(R->getSourceLoc())}, {}, nullptr, false);

      Meta.IsImplicitlyHashable = true;
      Meta.HashCodeFn = M;

      break;
   }
   case ImplicitConformanceKind::Copyable: {
      if (auto* Fn = Meta.CopyFn) {
         return finish(Fn);
      }

      SourceType RetTy(QC.Context.getRecordType(R));
      auto* Name = &QC.Context.getIdentifiers().get("copy");

      M = MethodDecl::Create(
          QC.Context, R->getAccess(), R->getSourceLoc(), Name, RetTy,
          {QC.Sema->MakeSelfArg(R->getSourceLoc())}, {}, nullptr, false);

      Meta.IsImplicitlyCopyable = true;
      Meta.CopyFn = M;

      break;
   }
   }

   M->setSynthesized(true);
   QC.Sema->ActOnDecl(R, M);

   return finish(M);
}

QueryResult GetImplicitConformanceQuery::run()
{
   if (auto Err = QC.CheckConformances(QC.Context.getRecordType(R))) {
      return Query::finish(Err);
   }

   auto& Meta = QC.RecordMeta[R];
   switch (K) {
   case ImplicitConformanceKind::StringRepresentable:
      return finish(Meta.ToStringFn);
   case ImplicitConformanceKind::Equatable:
      return finish(Meta.OperatorEquals);
   case ImplicitConformanceKind::Hashable:
      return finish(Meta.HashCodeFn);
   case ImplicitConformanceKind::Copyable:
      return finish(Meta.CopyFn);
   }
}

QueryResult GetRecordMetaQuery::run()
{
   if (Initialize) {
      if (auto Err = QC.PrepareDeclInterface(R)) {
         return Query::finish(Err);
      }
   }

   return finish(&QC.RecordMeta[R]);
}

QueryResult IsBuiltinIntegerTypeQuery::run()
{
   const RecordMetaInfo* Info;
   if (QC.GetRecordMeta(Info, R, false)) {
      llvm_unreachable("should not fail");
   }

   return finish(Info->IsBuiltinIntegerType);
}

QueryResult IsBuiltinBoolTypeQuery::run()
{
   const RecordMetaInfo* Info;
   if (QC.GetRecordMeta(Info, R, false)) {
      llvm_unreachable("should not fail");
   }

   return finish(Info->IsBuiltinBoolType);
}

QueryResult IsBuiltinFloatingPointTypeQuery::run()
{
   const RecordMetaInfo* Info;
   if (QC.GetRecordMeta(Info, R, false)) {
      llvm_unreachable("should not fail");
   }

   return finish(Info->IsBuiltinFloatingPointType);
}