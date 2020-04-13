#include "SemaPass.h"

#include "Basic/NestedNameSpecifier.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Query/QueryContext.h"
#include "Sema/OverloadResolver.h"
#include "Sema/Builtin.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

static bool addCandidates(const MultiLevelLookupResult &lookupRes,
                          CandidateSet &CandSet) {
   // add candidates found via normal lookup
   unsigned Distance = 0;
   for (auto &Lookup : lookupRes) {
      for (auto &D : Lookup) {
         if (auto CD = dyn_cast<CallableDecl>(D)) {
            if (auto M = dyn_cast<MethodDecl>(D)) {
               // only consider the 'deepest' override
               if (M->isOverride())
                  CandSet.CandidateFns.insert(M->getOverridenMethod());

               if (M->isProtocolDefaultImpl() && !M->isProtocolRequirement())
                  continue;
            }

            CandSet.addCandidate(CD, Distance);
         }
      }

      ++Distance;
   }

   return false;
}

static bool addCandidates(SemaPass &Sema,
                          DeclContext *Ctx,
                          DeclarationName name,
                          CandidateSet &CandSet,
                          bool IsLocalLookup) {
   LookupOpts Opts = DefaultLookupOpts;
   if (IsLocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }

   const MultiLevelLookupResult *LookupRes;
   if (Sema.QC.MultiLevelLookup(LookupRes, Ctx, name, Opts)) {
      return true;
   }

   return addCandidates(*LookupRes, CandSet);
}

std::pair<DeclContext*, bool> SemaPass::getAsContext(QualType Ty)
{
   if (auto RecTy = Ty->asRecordType()) {
      return { RecTy->getRecord(), false };
   }

   if (auto Meta = Ty->asMetaType()) {
      if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
         return { MetaRecTy->getRecord(), true };
      }
   }

   return { nullptr, false };
}

NamedDecl *SemaPass::getTypeDecl(QualType Ty)
{
   CanType T = Ty->getDesugaredType()->getCanonicalType();

   if (auto Meta = T->asMetaType()) {
      T = Meta->getUnderlyingType();
   }

   if (auto *RecTy = T->asRecordType()) {
      return RecTy->getRecord();
   }
   if (auto *Alias = T->asTypedefType()) {
      return Alias->getTypedef();
   }

   return nullptr;
}

CandidateSet
SemaPass::lookupFunction(DeclarationName name,
                         Expression *SelfArg,
                         ArrayRef<Expression*> args,
                         ArrayRef<Expression*> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   DeclContext *DC = nullptr;
   if (SelfArg) {
      CanType SelfTy = SelfArg->getExprType()->removeReference()
                              ->removeMetaType()->getDesugaredType();

      if (auto *RT = SelfTy->asRecordType()) {
         DC = RT->getRecord();
      }
   }

   if (!DC) {
      DC = DeclCtx;
   }

   return lookupFunction(DC, name, SelfArg, args,
                         templateArgs, labels, Caller, suppressDiags);
}

CandidateSet
SemaPass::lookupFunction(DeclContext *Ctx,
                         DeclarationName name,
                         Expression *SelfArg,
                         ArrayRef<Expression *> args,
                         ArrayRef<Expression *> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   CandidateSet CandSet;
   bool IsLocalLookup = Ctx == &getDeclContext();

   addCandidates(*this, Ctx, name, CandSet, IsLocalLookup);
   lookupFunction(CandSet, name, SelfArg, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

CandidateSet SemaPass::getCandidates(DeclarationName name,
                                     Expression *SelfExpr) {
   CandidateSet CandSet;
   if (addCandidates(*this, &getDeclContext(), name, CandSet, true)) {
      return CandSet;
   }

   return CandSet;
}

CandidateSet
SemaPass::lookupCase(DeclarationName name,
                     EnumDecl *E,
                     ArrayRef<Expression *> args,
                     ArrayRef<Expression *> templateArgs,
                     ArrayRef<IdentifierInfo*> labels,
                     Statement *Caller,
                     bool suppressDiags) {
   CandidateSet CandSet;
   if (auto Case = E->hasCase(name)) {
      CandSet.addCandidate(Case);
   }

   lookupFunction(CandSet, name, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

static void checkIfCalledFromTemplate(SemaPass &SP, CallableDecl *C)
{
   if (!isa<NamedDecl>(SP.getDeclContext()))
      return;

   // Check if the function was called from a template.
   if (cast<NamedDecl>(SP.getDeclContext()).isInUnboundedTemplate()) {
      C->setCalledFromTemplate(true);
      SP.getILGen().notifyFunctionCalledInTemplate(C);
   }
}

void SemaPass::lookupFunction(CandidateSet &CandSet,
                              DeclarationName name,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels,
                              Statement *Expr,
                              bool suppressDiags) {
   lookupFunction(CandSet, name, nullptr, args, templateArgs, labels, Expr,
                  suppressDiags);
}

void SemaPass::lookupFunction(CandidateSet &CandSet,
                              DeclarationName name,
                              Expression *SelfArg,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels,
                              Statement *Expr,
                              bool suppressDiags) {
   if (CandSet.Candidates.empty()) {
      if (!suppressDiags) {
         diagnose(Expr, err_func_not_found, Expr->getSourceRange(),
                  0, name);
      }

      return;
   }

   auto *BestCand = sema::resolveCandidateSet(*this, CandSet, SelfArg, args,
                                              labels, templateArgs,
                                              SourceType(), Expr,
                                              !suppressDiags);

   if (!BestCand) {
      if (CandSet.isDependent()) {
         for (auto &Cand : CandSet.Candidates) {
            checkIfCalledFromTemplate(*this, Cand.getFunc());
         }

         return;
      }

      if (suppressDiags) {
         return;
      }

      return CandSet.diagnose(*this, SelfArg, args, templateArgs, Expr);
   }
}

MethodDecl *SemaPass::getEquivalentMethod(MethodDecl *Orig,
                                          RecordDecl *Inst) {
   if (Orig->isMemberwiseInitializer())
      return cast<StructDecl>(Inst)->getMemberwiseInitializer();

   if (isa<DeinitDecl>(Orig))
      return Inst->getDeinitializer();

   DeclarationName Name = Orig->getDeclName();
   switch (Name.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      Name = Context.getDeclNameTable().getConstructorName(
         Context.getRecordType(Inst),
         Name.getKind() == DeclarationName::ConstructorName);

      break;
   case DeclarationName::DestructorName:
      Name = Context.getDeclNameTable().getDestructorName(
         Context.getRecordType(Inst));
      break;
   default:
      break;
   }

   const MultiLevelLookupResult *LookupRes;
   if (QC.MultiLevelLookup(LookupRes, Inst, Name)) {
      return Orig;
   }

   for (auto *D : LookupRes->allDecls()) {
      auto *M = dyn_cast<MethodDecl>(D);
      if (!M)
         continue;

      if (M->getMethodID() == Orig->getMethodID())
         return M;
   }

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                                      const TemplateArgList &templateArgs,
                                      Statement *Caller) {
   auto F = Cand.getFunc();
   Cand.OuterTemplateArgs = FinalTemplateArgumentList::Create(Context,
                                                              templateArgs);

   if (templateArgs.isStillDependent()) {
      return true;
   }

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return true;

      RecordDecl *Inst;
      if (QC.InstantiateRecord(Inst, Case->getRecord(), Cand.OuterTemplateArgs,
                               Caller->getSourceLoc())) {
         Inst = nullptr;
      }

      if (!Inst)
         return false;

      Cand.setCandDecl(maybeInstantiateTemplateMember(Inst, Case));
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return true;

   RecordDecl *Inst;
   if (QC.InstantiateRecord(Inst, Init->getRecord(), Cand.OuterTemplateArgs,
                            Caller->getSourceLoc())) {
      Inst = nullptr;
   }

   if (Inst) {
      if (Init->isSynthesized()) {
         if (QC.DeclareMemberwiseInit(cast<StructDecl>(Inst))) {
            return false;
         }

         Cand.setCandDecl(cast<StructDecl>(Inst)->getMemberwiseInitializer());
      }
      else {
         Cand.setCandDecl(maybeInstantiateTemplateMember(Inst, Init));
      }

      if (QC.PrepareDeclInterface(Cand.getFunc())) {
         return false;
      }

      return true;
   }

   return false;
}

void SemaPass::maybeInstantiate(CandidateSet &CandSet,
                                Statement *Caller) {
   if (!CandSet) {
      return;
   }

   maybeInstantiate(CandSet.getBestMatch(), Caller);
}

void SemaPass::maybeInstantiate(CandidateSet::Candidate &Cand,
                                Statement *Caller) {
   if (Cand.isAnonymousCandidate()
   || Cand.InnerTemplateArgs.isStillDependent()) {
      return;
   }

   auto F = Cand.getFunc();
   checkIfCalledFromTemplate(*this, F);

   // this function was deserialized from a module, so we need at least it's
   // IL definition
   if (auto Info = F->getLazyFnInfo()) {
      // if the function is a template, we also need it's body
      if (F->isUnboundedTemplate())
         Info->loadBody(F);
   }

   // FIXME runtime-generics
   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         Cand.setCandDecl(maybeInstantiateMemberFunction(M, Caller));
      }

      return;
   }

   auto &TAs = Cand.InnerTemplateArgs;
   auto *FinalList = FinalTemplateArgumentList::Create(Context, TAs);

   if (auto Fn = dyn_cast<FunctionDecl>(Cand.getFunc())) {
      FunctionDecl *Inst;
      if (QC.InstantiateFunction(Inst, Fn, FinalList,
                                 Caller->getSourceLoc())) {
         return;
      }
      if (QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.setCandDecl(Inst);
   }
   else {
      if (F->getRecord()->isProtocol()) {
         return;
      }

      MethodDecl *Inst;
      if (QC.InstantiateMethod(Inst, cast<MethodDecl>(Cand.getFunc()), FinalList,
                               Caller->getSourceLoc())) {
         return;
      }
      if (QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.setCandDecl(Inst);
   }

   if (!F->isUnboundedTemplate() && RuntimeGenerics) {
      Cand.getFunc()->setShouldBeSpecialized(true);
   }
   else {
      Cand.setCandDecl(maybeInstantiateMemberFunction(Cand.getFunc(), Caller));
   }
}

CallableDecl *SemaPass::maybeInstantiateMemberFunction(CallableDecl *M,
                                              StmtOrDecl Caller,
                                              bool NeedImmediateInstantiation) {
   if (M->getBody() || !M->getBodyTemplate()) {
      return M;
   }

   CallableDecl *Template = M->getBodyTemplate();
   if (Template->isInvalid()) {
      return M;
   }

   if (auto FnInfo = Template->getLazyFnInfo()) {
      FnInfo->loadBody(Template);
   }

   if (!Template->isInUnboundedTemplate()
         && !Template->isExternal()
         && RuntimeGenerics) {
      M->setShouldBeSpecialized(true);
      return M;
   }

   if (NeedImmediateInstantiation) {
      QC.InstantiateMethodBody(M);
      QueuedInstantiations.remove(M);
   }
   else {
      // Queue the instantiation for later.
      QueuedInstantiations.insert(M);
   }

   return M;
}

MethodDecl *SemaPass::InstantiateMethod(RecordDecl *R, StringRef Name,
                                        StmtOrDecl SOD) {
   llvm_unreachable("nah");
//   auto *II = &Context.getIdentifiers().get(Name);
//   auto *M = LookupSingle<MethodDecl>(*R, II);
//
//   assert(M && "method does not exist!");
//   maybeInstantiateMemberFunction(M, SOD);
//
//   if (inCTFE())
//      ILGen->prepareFunctionForCtfe(M, SOD);
//
//   return M;
}

MethodDecl *SemaPass::InstantiateProperty(RecordDecl *R,
                                          StringRef Name,
                                          bool FindGetter,
                                          StmtOrDecl SOD) {
   llvm_unreachable("nah");
//   auto *II = &Context.getIdentifiers().get(Name);
//
//   if (FindGetter) {
//      auto GetterName = Context.getDeclNameTable()
//                               .getAccessorName(*II,
//                                                DeclarationName::AccessorKind::Getter);
//
//      auto *Getter = LookupSingle<MethodDecl>(*R, GetterName);
//      assert(Getter && "method does not exist!");
//
//      maybeInstantiateMemberFunction(Getter, SOD);
//
//      if (inCTFE())
//         ILGen->prepareFunctionForCtfe(Getter, SOD);
//
//      return Getter;
//   }
//
//   auto SetterName = Context.getDeclNameTable()
//                            .getAccessorName(*II,
//                                         DeclarationName::AccessorKind::Setter);
//
//   auto *Setter = LookupSingle<MethodDecl>(*R, SetterName);
//   assert(Setter && "method does not exist!");
//
//   maybeInstantiateMemberFunction(Setter, SOD);
//
//   if (inCTFE())
//      ILGen->prepareFunctionForCtfe(Setter, SOD);
//
//   return Setter;
}

RecordDecl *SemaPass::InstantiateRecord(SourceLocation POI,
                                        RecordDecl *R,
                                        FinalTemplateArgumentList *TemplateArgs){
   RecordDecl *Inst;
   if (QC.InstantiateRecord(Inst, R, TemplateArgs, POI)) {
      return nullptr;
   }

   return Inst;
}

AliasDecl *SemaPass::InstantiateAlias(SourceLocation POI,
                                      AliasDecl *td,
                                      FinalTemplateArgumentList *TemplateArgs) {
   AliasDecl *Inst;
   if (QC.InstantiateAlias(Inst, td, TemplateArgs, POI)) {
      return nullptr;
   }

   return Inst;
}

RecordDecl *SemaPass::InstantiateRecord(SourceLocation POI,
                                        RecordDecl *R,
                                        const TemplateArgList &TemplateArgs) {
   return InstantiateRecord(
      POI, R, FinalTemplateArgumentList::Create(Context, TemplateArgs));
}

LLVM_ATTRIBUTE_UNUSED
static bool wouldSelfBeValid(SemaPass &SP, bool &Static)
{
   Static = false;

   auto *Ctx = &SP.getDeclContext();
   switch (Ctx->getDeclKind()) {
   case Decl::MethodDeclID:
      Static = cast<MethodDecl>(Ctx)->isStatic();
      LLVM_FALLTHROUGH;
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      return true;
   default:
      return false;
   }
}

template<class Expr>
static void visitContextDependentArgs(SemaPass &Sema, Expr *C)
{
   for (auto *Arg : C->getArgs()) {
      Sema.visitTypeDependentContextualExpr(Arg);
      C->copyStatusFlags(Arg);
   }
}

static ExprResult CreateAnonymousCall(SemaPass &Sema, CallExpr *Call,
                                      NamedDecl *ND) {
   // this is semantically an anonymous call that happens to be spelled
   // like a normal function call, i.e.
   //  `let fn = () => {}; fn()` is semantically equivalent to
   //  `(fn)()`
//   auto ID = new(Sema.Context) IdentifierRefExpr(Call->getSourceRange(),
//                                                 Call->getDeclName(), { },
//                                                 Call->getContext());
//
//   ID->setParentExpr(Call->getParentExpr());
//
//   auto Anon = AnonymousCallExpr::Create(Sema.Context, Call->getParenRange(),
//                                         ID, Call->getArgs(),
//                                         Call->getLabels());
//
//   return Sema.visitExpr(Call, Anon);
   llvm_unreachable("don't do it!");
}

Expression *SemaPass::convertCStyleVarargParam(Expression *Expr)
{
   Expr = castToRValue(Expr);
   QualType type = Expr->getExprType();

   // (unsigned) char / short are promoted to int for c-style vararg functions.
   if (type->isIntegerType() && type->getBitwidth() < 32) {
      QualType promotedType = type->isUnsigned()
                              ? Context.getUInt32Ty()
                              : Context.getInt32Ty();

      return forceCast(Expr, promotedType);
   }

   // float is promoted to double for c-style vararg functions.
   if (type->isFloatTy()) {
      return forceCast(Expr, Context.getDoubleTy());
   }

   return Expr;
}

ExprResult SemaPass::visitCallExpr(CallExpr *Call, TemplateArgListExpr *ArgExpr)
{
   CallableDecl *C = Call->getFunc();
   C = maybeInstantiateMemberFunction(C, Call);

   if (QC.PrepareDeclInterface(C)) {
      Call->setIsInvalid(true);
      return Call;
   }

   QualType ExprType;
   if (auto *F = dyn_cast<FunctionDecl>(C)) {
      ExprType = F->getReturnType();
   }
   else if (auto *Case = dyn_cast<EnumCaseDecl>(C)) {
      ExprType = Context.getRecordType(Case->getRecord());
   }
   else {
      auto M = cast<MethodDecl>(C);
      if (M->isCompleteInitializer()) {
         ExprType = Context.getRecordType(M->getRecord());
      }
      else if (M->isStatic()) {
         ExprType = M->getReturnType();
      }
      else {
         ExprType = M->getReturnType();
      }
   }

   if (C->isTemplate()) {
      if (QC.SubstTemplateParamTypes(ExprType, ExprType,
                                     *Call->getTemplateArgs(),
                                     Call->getSourceRange())) {
         Call->setIsInvalid(true);
      }
   }

   Call->setExprType(ExprType);

   unsigned i = 0;
   MutableArrayRef<FuncArgDecl*> params = C->getArgs();

   for (Expression *&argVal : Call->getArgs()) {
      QualType neededType;
      bool cstyleVararg = false;

      if (i < params.size()) {
         neededType = params[i]->getType();
      }
      else if (C->isCstyleVararg()) {
         cstyleVararg = true;
      }
      else {
         neededType = params.back()->getType();
      }

      auto typecheckResult = typecheckExpr(argVal, neededType, Call);
      if (!typecheckResult) {
         Call->setIsInvalid(true);
         ++i;

         continue;
      }

      if (cstyleVararg) {
         argVal = convertCStyleVarargParam(argVal);
      }
      else {
         argVal = implicitCastIfNecessary(argVal, neededType);
      }

      ++i;
   }

   if (isInBuiltinModule(C)) {
      HandleBuiltinCall(Call);
   }

   return Call;

   /*
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr)
      TemplateArgs = ArgExpr->getExprs();

   auto &args = Call->getArgs();
   for (auto &arg : args) {
      if (auto *Macro = dyn_cast<MacroExpansionExpr>(arg)) {
         auto Result = resolveMacroExpansionExpr(Macro);
         if (Result) {
            arg = Result;
         }
      }
   }

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown) {
      return Call;
   }

   // check if '.init' is called without a parent expression
   if (!Call->getParentExpr() && Call->isDotInit()) {
      diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
      Call->setExprType(Context.getVoidType());

      return Call;
   }

   bool IsLocalLookup = true;
   DeclContext *Ctx = Call->getContext();

   // Check if the call's parent expr refers to a namespace or module.
   auto *NameSpec = checkNamespaceRef(Call);
   if (Call->isInvalid()) {
      return ExprError();
   }

   if (NameSpec) {
      if (NameSpec->getNameSpec()->isAnyNameDependent()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      Ctx = Call->getContext();
      IsLocalLookup = false;
   }

   auto ParentExpr = Call->getParentExpr();
   QualType ParentType;
   QualType ParentTypeNoSugar;

   if (ParentExpr && !refersToNamespace(ParentExpr)) {
      ParentExpr->setAllowNamespaceRef(true);

      auto ParentResult = visitExpr(Call, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Call->setParentExpr(ParentExpr);

      ParentType = ParentExpr->getExprType()->getCanonicalType()
                             ->removeReference();

      if (QC.ApplyCapabilites(ParentType, ParentType, &getDeclContext())) {
         Call->setIsInvalid(true);
      }

      ParentTypeNoSugar = ParentType->getDesugaredType();
      if (ParentTypeNoSugar->isDependentType()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      if (Call->isPointerAccess()) {
         ParentExpr = checkDeref(ParentExpr, ParentTypeNoSugar);
         Call->setParentExpr(ParentExpr);

         ParentType = ParentExpr->getExprType()->getCanonicalType()
                                ->removeReference();
         ParentTypeNoSugar = ParentType->getDesugaredType();
      }

      if (Call->isDotInit()) {
         // '.init' must either be called on self, in which case it has to be
         // in an initializer, or statically on a type
         if (auto Meta = ParentTypeNoSugar->asMetaType()) {
            auto UnderlyingTy = Meta->getUnderlyingType();
            if (!UnderlyingTy->isRecordType()) {
               diagnose(Call, err_dot_init_bad_type, UnderlyingTy,
                        Call->getSourceRange());

               Call->setExprType(UnderlyingTy);
               return Call;
            }

            // treat this as an initializer call
            Call->setIdent(Context.getDeclNameTable()
                                  .getConstructorName(UnderlyingTy));

            Call->setIsDotInit(false);
         }
         else if (!isa<SelfExpr>(ParentExpr->ignoreParens())
               && !isa<SuperExpr>(ParentExpr->ignoreParens())) {
            diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
            Call->setExprType(Context.getVoidType());

            return Call;
         }
         else {
            // Don't use an existential type here.
            Call->setIdent(
               Context.getDeclNameTable().getConstructorName(
                  ParentExpr->getExprType()
                            ->getCanonicalType()
                            ->removeReference()
                            ->getDesugaredType()));
         }
      }

      IsLocalLookup = false;

      auto ContextAndIsStatic = getAsContext(ParentTypeNoSugar);
      Call->setDirectCall(isa<SuperExpr>(ParentExpr)
                            || ContextAndIsStatic.second);

      ParentType = ParentType->removeMetaType();
      ParentTypeNoSugar = ParentType->getDesugaredType();
   }
   else if (Call->isPointerAccess()) {
      diagnose(Call, err_member_access_non_pointer, "<namespace>",
               Call->getSourceLoc());
   }
   else if (Call->hasLeadingDot()) {
      auto CtxTy = Call->getContextualType();
      if (!CtxTy || !CtxTy->isRecordType()) {
         diagnose(Call, err_requires_contextual_type, 2,
                  Call->getSourceRange());

         return ExprError();
      }

      auto *R = CtxTy->getRecord();
      if (QC.PrepareDeclInterface(R)) {
         visitContextDependentArgs(*this, Call);
         Call->setExprType(Context.getRecordType(R));
         return Call;
      }

      Ctx = R;
   }

   // This can only happen in instantiations of AnonymousCallExpr's.
   if (!Call->getDeclName()) {
      assert(Call->getParentExpr());
      return HandleStaticTypeCall(Call, ArgExpr,
                                  Call->getParentExpr()
                                      ->getExprType()->asMetaType()
                                      ->getUnderlyingType());
   }

   if (!Ctx) {
      Ctx = &getDeclContext();
   }

   // The name to look for.
   DeclarationName Name = Call->getDeclName();

   // The set of function candidates.
   CandidateSet CandSet;

   // We need to track if the called function is a subscript setter to adjust
   // the return type later.
   bool IsSubscriptSetter = false;

   LookupOpts Opts = DefaultLookupOpts;
   if (IsLocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }

   // FIXME primitive-extensions
   bool DoTypeLookup = ParentTypeNoSugar
      && (ParentTypeNoSugar->isRecordType()
          || ParentTypeNoSugar->isExistentialType());

   const MultiLevelLookupResult *Result;
   if (DoTypeLookup) {
      if (QC.MultiLevelTypeLookup(Result, ParentTypeNoSugar, Name, Opts)) {
         return ExprError();
      }
   }
   else {
      if (QC.MultiLevelLookup(Result, Ctx, Name, Opts)) {
         return ExprError();
      }
   }

   if (!Result->empty()) {
      // Check if we found something that is not a function first.
      auto ND = Result->front().front();
      switch (ND->getKind()) {
      case Decl::EnumCaseDeclID: {
         auto enumCaseExpr =
            new(getContext()) EnumCaseExpr(Call->getSourceLoc(),
                                           cast<EnumCaseDecl>(ND),
                                           move(Call->getArgs()));

         enumCaseExpr->setContextualType(Call->getContextualType());
         return visitExpr(Call, enumCaseExpr);
      }
      case Decl::SubscriptDeclID: {
         bool FoundGetter = false;
         bool FoundSetter = false;

         for (auto &Decls : *Result) {
            for (auto *Decl : Decls) {
               if (auto *SD = dyn_cast<SubscriptDecl>(Decl)) {
                  FoundGetter |= SD->hasGetter();
                  FoundSetter |= SD->hasSetter();
               }
            }

            if (FoundGetter && FoundSetter)
               break;
         }

         if (Call->isLHSOfAssignment()) {
            if (!FoundSetter) {
               diagnose(Call, err_subscript_does_not_have, 1,
                        Call->getSourceLoc());
            }

            Call->setIdent(Context.getDeclNameTable().getSubscriptName(
               DeclarationName::SubscriptKind::Setter));

            IsSubscriptSetter = true;
         }
         else {
            if (!FoundGetter) {
               diagnose(Call, err_subscript_does_not_have, 0,
                        Call->getSourceLoc());
            }

            Call->setIdent(Context.getDeclNameTable().getSubscriptName(
               DeclarationName::SubscriptKind::Getter));
         }

         Name = Call->getDeclName();

         if (ParentType) {
            if (QC.MultiLevelTypeLookup(Result, ParentTypeNoSugar, Name, Opts)) {
               return ExprError();
            }
         }
         else {
            if (QC.MultiLevelLookup(Result, Ctx, Name, Opts)) {
               return ExprError();
            }
         }

         break;
      }
      case Decl::AliasDeclID: {
         auto *Alias = cast<AliasDecl>(ND);
         if (Alias->getType()->isMetaType()) {
            auto UnderlyingTy = Alias->getType()->asMetaType()
               ->getUnderlyingType();

            return HandleStaticTypeCall(Call, ArgExpr, UnderlyingTy, false);
         }

         return CreateAnonymousCall(*this, Call, ND);
      }
      case Decl::AssociatedTypeDeclID: {
         auto *AT = dyn_cast<AssociatedTypeDecl>(ND);
         QualType T = Context.getAssociatedType(AT);

         return HandleStaticTypeCall(Call, ArgExpr, T);
      }
      case Decl::TemplateParamDeclID: {
         auto *P = cast<TemplateParamDecl>(ND);
         if (P->isUnbounded()) {
            visitContextDependentArgs(*this, Call);
            Call->setIsTypeDependent(true);
            Call->setExprType(UnknownAnyTy);

            return Call;
         }

         QualType T = Context.getTemplateArgType(P);
         return HandleStaticTypeCall(Call, ArgExpr, T);
      }
      case Decl::FuncArgDeclID:
      case Decl::LocalVarDeclID:
      case Decl::GlobalVarDeclID:
      case Decl::FieldDeclID:
      case Decl::PropDeclID:
         return CreateAnonymousCall(*this, Call, ND);
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID: {
         return HandleConstructorCall(
            Call, ArgExpr, Context.getRecordType(cast<RecordDecl>(ND)),
            nullptr, nullptr, nullptr, false);
      }
      case Decl::InitDeclID: {
         return HandleConstructorCall(Call, ArgExpr,
                                      Call->getDeclName().getConstructorType(),
                                      Result);
      }
      case Decl::MethodDeclID: {
         // If we found an instance method through local lookup, build an
         // implicit self expression.
         if (IsLocalLookup) {
            bool Static;
            if (wouldSelfBeValid(*this, Static)) {
               Expression *Self = SelfExpr::Create(Context,
                                                   Call->getSourceLoc(),
                                                   Static);

               auto Res = visitExpr(Call, Self);
               if (!Res)
                  return ExprError();

               Call->setParentExpr(Self);
            }
         }

         auto *M = cast<MethodDecl>(ND);
         if (M->getDeclName().getKind() == DeclarationName::SubscriptName
             && M->getDeclName().getSubscriptKind()
                == DeclarationName::SubscriptKind::Setter) {
            IsSubscriptSetter = true;
         }

         break;
      }
      default:
         break;
      }

      // Add candidates found via normal lookup.
      if (addCandidates(*Result, CandSet)) {
         Call->setIsInvalid(true);
         return ExprError();
      }
   }

   if (CandSet.Candidates.empty()) {
      // If this expression is type dependent, new overloads might be visible
      // at instantiation time, so don't report an error for now.
      if (CandSet.isDependent() || inUnboundedTemplate()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      if (ParentType) {
         if (ParentType->isMetaType()) {
            diagnose(Call, err_method_not_found, Call->getDeclName(), false,
                     ParentType->removeMetaType(), Call->getSourceLoc());
         }
         else {
            diagnose(Call, err_method_not_found, Call->getDeclName(), true,
                     ParentType, Call->getSourceLoc());
         }
      }
      else {
         diagnose(Call, err_func_not_found, 0, Call->getDeclName(),
                  Call->getSourceLoc());
      }

      return ExprError();
   }

   Expression *SelfExpr = Call->getParentExpr();
   if (SelfExpr && refersToNamespace(SelfExpr)) {
      SelfExpr = nullptr;
   }

   lookupFunction(CandSet, Call->getDeclName(), SelfExpr, args,
                  TemplateArgs, Call->getLabels(), Call);

   if (CandSet.isDependent()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet) {
      return ExprError();
   }

   // Get the called function.
   auto &Cand = CandSet.getBestMatch();
   auto func = Cand.getFunc();
   checkAccessibility(func, Call);

   // Resolve call.
   Call->setFunc(func);
   Call->setExprType(func->getReturnType());
   Call->setKind(CallKind::NamedFunctionCall);

   if (func->isInUnboundedTemplate()) {
      visitContextDependentArgs(*this, Call);

      Call->setIsTypeDependent(true);
      Call->setExprType(func->getReturnType());

      return Call;
   }

   if (func->throws()) {
      if (TryScopeStack.empty()) {
         diagnose(Call, err_throwing_call_not_in_try, func->getDeclName(),
                  Call->getSourceRange());
      }
      else {
         TryScopeStack.back() = true;
      }
   }

   Expression *ResultExpr = Call;
   if (!Call->needsInstantiation()) {
      Call->getArgs().clear();
      Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                             CandSet.ResolvedArgs.end());

      if (!Cand.InnerTemplateArgs.empty() && func->isTemplate()) {
         auto *FinalList  = sema::FinalTemplateArgumentList::Create(
            Context, Cand.InnerTemplateArgs);

         if (ArgExpr) {
            ArgExpr->setTemplateArgs(FinalList);
         }
         else {
            ArgExpr = TemplateArgListExpr::Create(Context,
                                                  Call->getSourceRange(),
                                                  Call, FinalList);

            ResultExpr = ArgExpr;
         }
      }
   }

   if (IsSubscriptSetter) {
      // The actual return type will be void, this is just to allow
      // subscripts to appear on the left hand side of an assignment.
      Call->setExprType(Context.getMutableReferenceType(
         func->getArgs()[2]->getType()));
   }

   if (isInBuiltinModule(func)) {
      HandleBuiltinCall(Call);
   }

   if (auto *PE = Call->getParentExpr()) {
      CheckReturnedSelfType(PE->ignoreParensAndImplicitCasts()->getExprType(),
                            Call);
   }

   return ResultExpr;
    */
}

LLVM_ATTRIBUTE_UNUSED
static void addTemplateArgs(SmallVectorImpl<Expression*> &Exprs,
                            SmallVectorImpl<std::unique_ptr<BuiltinExpr>>
                               &PlaceholderExprs,
                            const sema::TemplateArgument &Arg) {
   if (Arg.isVariadic()) {
      for (auto &VA : Arg.getVariadicArgs()) {
         addTemplateArgs(Exprs, PlaceholderExprs, VA);
      }

      return;
   }

   if (Arg.isValue()) {
      Exprs.push_back(Arg.getValueExpr());
      return;
   }

   PlaceholderExprs.emplace_back(
      std::make_unique<BuiltinExpr>(BuiltinExpr::CreateTemp(Arg.getType())));
   Exprs.push_back(PlaceholderExprs.back().get());
}

ExprResult SemaPass::HandleStaticTypeCall(CallExpr *Call,
                                          TemplateArgListExpr *ArgExpr,
                                          CanType Ty,
                                          bool AllowProtocol) {
   if (auto *ATT = Ty->asAssociatedType()) {
      auto *AT = ATT->getDecl();
      Call->setContainsAssociatedType(true);

      QualType CapableTy = ATT;
      if (QC.ApplyCapabilites(CapableTy, CapableTy, &getDeclContext())) {
         Call->setIsInvalid(true);
      }

      QualType Covar;
      if (Ty != CapableTy) {
         Covar = CapableTy;
      }
      else {
         Covar = AT->getCovariance();
      }

      if (Covar->isRecordType() || Covar->isExistentialType()) {
         auto Result = HandleConstructorCall(Call, ArgExpr, Covar, nullptr,
                                             nullptr, AT);

         if (Result) {
            Result.get()->setExprType(Ty);
            return Result;
         }

         return ExprError();
      }

      if (Covar->isUnknownAnyType()) {
         visitContextDependentArgs(*this, Call);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      Ty = Covar;
   }
   else if (auto *PT = Ty->asTemplateParamType()) {
      auto *P = PT->getParam();
      Call->setContainsGenericParam(true);

      if (!P->isVariadic() && P->isTypeName()) {
         QualType CapableTy = Ty;
         if (QC.ApplyCapabilites(CapableTy, CapableTy, &getDeclContext())) {
            Call->setIsInvalid(true);
         }

         QualType Covar;
         if (Ty != CapableTy) {
            Covar = CapableTy;
         }
         else {
            Covar = P->getCovariance();
         }

         if (Covar->isRecordType() || Covar->isExistentialType()) {
            return HandleConstructorCall(Call, ArgExpr, Covar, nullptr, P);
         }
      }

      return CreateAnonymousCall(*this, Call, P);
   }

   if (Ty->isVoidType()) {
      auto& args = Call->getArgs();
      if (!args.empty()) {
         diagnose(Call, err_too_many_args_for_call, 0, args.size(),
                  Call->getSourceLoc());

         return Call;
      }

      Call->setKind(CallKind::PrimitiveInitializer);
      Call->setExprType(Ty);

      return Call;
   }

   if (Ty->isRecordType() || Ty->isExistentialType()) {
      return HandleConstructorCall(Call, ArgExpr, Ty, nullptr, nullptr,
                                   nullptr, AllowProtocol);
   }

   if (Call->getArgs().empty() && hasDefaultValue(Ty)) {
      Call->setKind(CallKind::PrimitiveInitializer);
      Call->setExprType(Ty);

      return Call;
   }

   bool isValid = Ty->isVoidType() || Ty->isIntegerType() || Ty->isFPType();
   if (isValid) {
      Call->setExprType(Ty);

      unsigned neededArgs = Ty->isVoidType() ? 0 : 1;
      auto& args = Call->getArgs();
      if (args.size() != neededArgs) {
         auto diag = args.size() < neededArgs
                     ? err_too_few_args_for_call
                     : err_too_many_args_for_call;

         diagnose(Call, diag, neededArgs, args.size(), Call->getSourceLoc());
         return Call;
      }

      if (Ty->isIntegerType()) {
         if (auto lit = dyn_cast<IntegerLiteral>(args.front())) {
            lit->setContextualType(Ty);
            lit->setType(Ty);

            visitIntegerLiteral(lit);
         }
         else if (auto B = dyn_cast<BoolLiteral>(args.front())) {
            B->setContextualType(Ty);
            B->setType(Ty);

            visitBoolLiteral(B);
         }
         else {
            diagnose(Call, err_must_be_integral_constant, 1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return Call;
         }
      }
      else if (!Ty->isVoidType()) {
         assert(Ty->isFPType());
         auto lit = dyn_cast<FPLiteral>(args.front());
         if (!lit) {
            diagnose(Call, err_must_be_floating_constant,1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return Call;
         }

         lit->setContextualType(Ty);
         lit->setType(Ty);

         visitFPLiteral(lit);
      }

      Call->setKind(CallKind::PrimitiveInitializer);
      return Call;
   }

   diagnose(Call, err_cannot_call_type, Ty, true, Call->getSourceLoc());
   return ExprError();
}

ExprResult SemaPass::HandleConstructorCall(CallExpr *Call,
                                           TemplateArgListExpr *ArgExpr,
                                           QualType T,
                                           const MultiLevelLookupResult*
                                             LookupRes,
                                           TemplateParamDecl *Param,
                                           AssociatedTypeDecl *AT,
                                           bool AllowProtocol ) {
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr) {
      TemplateArgs = ArgExpr->getExprs();
   }

   if ((!T->isRecordType() || !T->getRecord()->isTemplate())
         && !TemplateArgs.empty()) {
      if (Call->getDeclName()) {
         diagnose(Call, err_generic_type_count, 0,
                  TemplateArgs.size(),
                  Call->getDeclName(), Call->getSourceLoc());
      }
      else {
         diagnose(Call, err_generic_type_count, 0,
                  TemplateArgs.size(),
                  "(anonymous)", Call->getSourceLoc());
      }

      TemplateArgs = MutableArrayRef<Expression*>();
   }

   if (T->isProtocol() && !Call->isDotInit() && !AllowProtocol) {
      diagnose(Call, err_protocol_initializer, Call->getSourceRange());
      Call->setExprType(T);
      return Call;
   }

   CandidateSet CandSet;
   DeclarationName DeclName;
   auto& givenArgs = Call->getArgs();

   if (LookupRes) {
      // Add candidates found via normal lookup.
      if (addCandidates(*LookupRes, CandSet)) {
         Call->setIsInvalid(true);
         Call->setExprType(T);
         return ExprError();
      }
   }
   else if (auto *RT = T->asRecordType()) {
      auto *R = RT->getRecord();
      DeclName = Context.getDeclNameTable()
                        .getConstructorName(Context.getRecordType(R),
                                            !Call->isDotInit());

      if (addCandidates(*this, R, DeclName, CandSet, false)) {
         Call->setIsInvalid(true);
         Call->setExprType(T);
         return ExprError();
      }
   }
   else {
      auto *Ext = T->uncheckedAsExistentialType();
      for (QualType ET : Ext->getExistentials()) {
         DeclName = Context.getDeclNameTable()
                           .getConstructorName(ET, !Call->isDotInit());

         if (QC.MultiLevelTypeLookup(LookupRes, ET, DeclName)) {
            Call->setIsInvalid(true);
            Call->setExprType(T);
            return ExprError();
         }
         if (addCandidates(*LookupRes, CandSet)) {
            Call->setIsInvalid(true);
            Call->setExprType(T);
            return ExprError();
         }
      }
   }

   Expression *SelfExpr = Call->getParentExpr();
   if (SelfExpr && refersToNamespace(SelfExpr)) {
      SelfExpr = nullptr;
   }

   lookupFunction(CandSet, DeclName, SelfExpr, givenArgs, TemplateArgs,
                  Call->getLabels(), Call);

   if (CandSet.isDependent()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet) {
      Call->setIsInvalid(true);
      Call->setExprType(T);

      return ExprError();
   }

   auto &Cand = CandSet.getBestMatch();
   auto Init = cast<InitDecl>(Cand.getFunc());

   // Update record since it might have been instantiated.
   RecordDecl *R = Init->getRecord();
   checkAccessibility(R, Call);

   // Check if this initializer is accessible.
   QC.CheckAccessibility(DeclCtx, Init, Call->getSourceLoc());

   // Make sure the record we're constructing is valid.
   if (QC.PrepareDeclInterface(R)) {
      return ExprError();
   }

   if (Init->isInUnboundedTemplate()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      if (!R->isTemplate()) {
         Call->setReturnType(Context.getRecordType(R));
         return Call;
      }

      return Call;
   }

   if (R->isAbstract() && !Call->isDotInit()) {
      diagnose(Call, err_instantiate_abstract_class, R->getDeclName(),
               Call->getSourceRange());
   }

   if (Call->isDotInit()) {
      if (Init->isFallible()) {
         auto fn = getCurrentFun();
         if (!fn->isFallibleInit()) {
            diagnose(Call, err_fallible_init_in_non_fallible_init,
                     Call->getSourceRange());
         }
      }

      Call->setExprType(Context.getVoidType());
   }
   else if (Init->isFallibleInit()) {
      Call->setExprType(cast<InitDecl>(Init)->getOptionTy());
   }
   else if (Param) {
      Call->setExprType(Context.getTemplateArgType(Param));
   }
   else if (AT) {
      Call->setExprType(Context.getAssociatedType(AT));
   }
   else if (R->isTemplate()) {
      Call->setExprType(Context.getDependentRecordType(
         R, Cand.OuterTemplateArgs, getParentType(Call->getParentExpr())));
   }
   else {
      Call->setExprType(Context.getRecordType(R));
   }

   if (Call->needsInstantiation()) {
      return Call;
   }

   Call->getArgs().clear();
   Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                          CandSet.ResolvedArgs.end());

   Call->setFunc(Init);

   if (Param) {
      Call->setParentExpr(
         new(Context) IdentifierRefExpr(Call->getIdentLoc(),
                                        IdentifierKind::TemplateParam,
                                        Param, Param->getCovariance()));

      Call->setKind(CallKind::GenericInitializerCall);
   }
   else if (AT) {
      Call->setParentExpr(
         new(Context) IdentifierRefExpr(Call->getIdentLoc(),
                                        IdentifierKind::AssociatedType, AT,
                                        Param->getCovariance()));

      Call->setKind(CallKind::GenericInitializerCall);
   }
   else {
      Call->setKind(CallKind::InitializerCall);
   }

   if (!Cand.InnerTemplateArgs.empty() && Init->isTemplate()) {
      auto *FinalList  = sema::FinalTemplateArgumentList::Create(
         Context, Cand.InnerTemplateArgs);

      if (ArgExpr) {
         ArgExpr->setTemplateArgs(FinalList);
      }
      else {
         ArgExpr = TemplateArgListExpr::Create(Context,
                                               Call->getSourceRange(),
                                               Call, FinalList);

         return ArgExpr;
      }
   }

   return Call;
}

CallExpr *SemaPass::CreateCall(CallableDecl *C,
                               ArrayRef<Expression*> Args,
                               SourceLocation Loc) {
   ASTVector<Expression*> Vec(Context, Args);
   return CreateCall(C, move(Vec), Loc);
}

CallExpr *SemaPass::CreateCall(CallableDecl *C,
                               ASTVector<Expression*> &&Args,
                               SourceLocation Loc) {
   CallKind K;
   if (auto F = dyn_cast<FunctionDecl>(C)) {
      K = CallKind::NamedFunctionCall;
   }
   else if (isa<EnumCaseDecl>(C)) {
      K = CallKind::NamedFunctionCall;
   }
   else {
      auto M = cast<MethodDecl>(C);
      if (M->isCompleteInitializer()) {
         K = CallKind::InitializerCall;
      }
      else if (M->isStatic()) {
         K = CallKind::StaticMethodCall;
      }
      else {
         K = CallKind::MethodCall;
      }
   }

   auto *callExpr = CallExpr::Create(Context, Loc, SourceRange(Loc), move(Args),
                                     C, K, QualType());

   callExpr->setIsDotInit(C->isBaseInitializer());
   return callExpr;
}

ExprResult SemaPass::visitAnonymousCallExpr(AnonymousCallExpr *Call)
{
   return Call;
}

namespace {

enum : int {
   No = -1, Yes = 0, YesWithPenalty = 1,
};
   
} // anonymous namespace

int SemaPass::ExprCanReturnImpl(Expression *E, CanType Ty)
{
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID: {
      auto IL = cast<IntegerLiteral>(E);

      // an expression with an explicit suffix can only return that type
      switch (IL->getSuffix()) {
      case IntegerLiteral::Suffix::i1: case IntegerLiteral::Suffix::u1:
      case IntegerLiteral::Suffix::i8: case IntegerLiteral::Suffix::u8:
      case IntegerLiteral::Suffix::i16: case IntegerLiteral::Suffix::u16:
      case IntegerLiteral::Suffix::i32: case IntegerLiteral::Suffix::u32:
      case IntegerLiteral::Suffix::i64: case IntegerLiteral::Suffix::u64:
      case IntegerLiteral::Suffix::i128: case IntegerLiteral::Suffix::u128:
         return Ty == IL->getType() ? Yes : No;
      case IntegerLiteral::Suffix::u:
         if (*Ty == Context.getUIntTy())
            return Yes;

         return (Ty->isIntegerType() && Ty->isUnsigned()) ? YesWithPenalty : No;
      case IntegerLiteral::Suffix::i:
         if (*Ty == Context.getIntTy())
            return Yes;

         return (Ty->isIntegerType() && !Ty->isUnsigned()) ? YesWithPenalty
                                                           : No;
      case IntegerLiteral::Suffix::None:
         break;
      }

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleByInt = getInitializableByDecl(
            InitializableByKind::Integer);

         return Context.getConformanceTable().conformsTo(R, ExpressibleByInt)
                ? YesWithPenalty : No;
      }

      if (*Ty == Context.getIntTy())
         return Yes;

      return Ty->isIntegerType() ? YesWithPenalty : No;
   }
   case Expression::CharLiteralID: {
      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::GraphemeCluster);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      return (Ty->isIntegerType() && Ty->getBitwidth() == 8) ? Yes : No;
   }
   case Expression::BoolLiteralID: {
      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::Bool);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      return (Ty->isIntegerType() && Ty->getBitwidth() == 1) ? Yes : No;
   }
   case Expression::FPLiteralID: {
      auto FP = cast<FPLiteral>(E);

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::Float);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      switch (FP->getSuffix()) {
      case FPLiteral::Suffix::f:
      case FPLiteral::Suffix::d:
         return (Ty == FP->getType()) ? Yes : No;
      case FPLiteral::Suffix::None:
         break;
      }

      if (Ty->isDoubleTy())
         return Yes;

      return Ty->isFPType() ? YesWithPenalty : No;
   }
   case Expression::StringLiteralID: {
      if (Ty->isPointerType() && Ty->getPointeeType()->isInt8Ty()) {
         return YesWithPenalty;
      }

      if (Ty->isArrayType() && Ty->asArrayType()->getElementType()->isInt8Ty()){
         return YesWithPenalty;
      }

      if (Ty->isRecordType() && (Ty->getRecord() == getStringDecl()))
         return Yes;

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(
            InitializableByKind::String);

         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty : No;
      }

      return No;
   }
   case Expression::IdentifierRefExprID:
   case Expression::CallExprID: {
      if (!Ty->isRecordType())
         return No;

      DeclarationName Name;
      if (auto *Call = dyn_cast<CallExpr>(E)) {
         Name = Call->getDeclName();
      }
      else {
         Name = cast<IdentifierRefExpr>(E)->getDeclName();
      }

      auto *R = Ty->getRecord();

      const SingleLevelLookupResult *Result;
      if (QC.LookupFirst(Result, R, Name)) {
         return Yes;
      }

      return Result->empty() ? No : Yes;
   }
   case Expression::EnumCaseExprID: {
      if (!Ty->isRecordType())
         return No;

      auto Enum = dyn_cast<EnumDecl>(Ty->getRecord());
      if (!Enum)
         return No;

      return Enum->hasCase(cast<EnumCaseExpr>(E)->getDeclName()) != nullptr
             ? Yes : No;
   }
   case Expression::LambdaExprID: {
      if (!Ty->isLambdaType())
         return No;

      auto LE = cast<LambdaExpr>(E);
      auto Func = Ty->asLambdaType();
      auto NeededParams = Func->getParamTypes();

      if (NeededParams.size() < LE->getNumArgs())
         return No;

      // check only the types of explicitly specified arguments
      unsigned i = 0;
      for (auto &Arg : LE->getArgs()) {
         auto NeededTy = NeededParams[i];
         auto GivenTypeRes = visitSourceType(Arg, Arg->getType());

         if (!GivenTypeRes) {
            LE->setIsInvalid(true);
            return No;
         }

         if (GivenTypeRes.getValue()->isAutoType())
            continue;

         if (GivenTypeRes.getValue() != NeededTy)
            return No;
      }

      auto NeededRetRes = visitSourceType(LE, LE->getReturnType());
      if (!NeededRetRes)
         return No;

      if (NeededRetRes.getValue()->isAutoType())
         return No;

      return NeededRetRes.getValue() == Func->getReturnType() ? Yes : No;
   }
   case Expression::BuiltinIdentExprID: {
      auto *BE = cast<BuiltinIdentExpr>(E);
      switch (BE->getIdentifierKind()) {
      case BuiltinIdentifier ::NULLPTR:
         return Ty->isPointerType();
      default:
         return false;
      }
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

int SemaPass::ExprCanReturn(Expression *E, CanType Ty)
{
   E = E->ignoreParens();
   Ty = Ty->getDesugaredType();

   if (Ty->isUnknownAnyType()) {
      return Yes;
   }

   // Look for an implicit initializer.
   if (Ty->isRecordType()) {
      auto *R = Ty->getRecord();

      // Load all external declarations.
      auto DN = Context.getDeclNameTable().getConstructorName(Ty);

      const MultiLevelLookupResult *Initializers;
      if (QC.MultiLevelLookup(Initializers, R, DN)) {
         return Yes;
      }

      for (auto *D : Initializers->allDecls()) {
         auto *I = cast<InitDecl>(D);
         if (!I->hasAttribute<ImplicitAttr>())
            continue;

         assert(I->getArgs().size() == 1 && "invalid implicit attribute");

         auto Res = ExprCanReturnImpl(E, I->getArgs().front()->getType());
         if (Res != No) {
            return Res;
         }
      }
   }

   assert(E->isContextDependent() && "not a context dependent expression!");
   return ExprCanReturnImpl(E, Ty);
}

static bool ArgTypesAreCompatible(QualType Given, QualType Needed)
{
   if (Given == Needed || Needed->isUnknownAnyType())
      return true;

   if (!isa<TemplateParamType>(Needed))
      return false;

   return ArgTypesAreCompatible(Given,
                                cast<TemplateParamType>(Needed)->getCovariance());
}

QualType SemaPass::ResolveContextualLambdaExpr(LambdaExpr *LE, QualType Ty)
{
   if (!Ty->isLambdaType())
      return QualType();

   auto Func = Ty->asLambdaType();
   auto NeededParams = Func->getParamTypes();
   auto NeededInfo = Func->getParamInfo();

   if (NeededParams.size() < LE->getNumArgs())
      return QualType();

   bool AllArgsResolved = true;
   SmallVector<QualType, 4> ArgTys;

   // check only the types of explicitly specified arguments
   unsigned i = 0;
   for (auto &Arg : LE->getArgs()) {
      auto NeededTy = NeededParams[i];
      auto Conv = NeededInfo[i++].getConvention();
      auto &ArgTy = Arg->getType();

      auto GivenTypeRes = visitSourceType(Arg, ArgTy);
      LE->copyStatusFlags(Arg);

      if (!GivenTypeRes) {
         LE->setIsInvalid(true);
         return QualType();
      }

      if (ArgTy->isAutoType()) {
         Arg->setConvention(Conv);

         if (NeededTy->containsTemplateParamType()) {
            AllArgsResolved = false;
         }
         else {
            ArgTy = NeededTy;
         }

         ArgTys.push_back(NeededTy);
         continue;
      }

      if (!ArgTypesAreCompatible(ArgTy, NeededTy))
         return QualType();

      ArgTys.push_back(ArgTy);
   }

   auto &RetTy = LE->getReturnType();

   auto NeededRetRes = visitSourceType(LE, RetTy);
   if (!NeededRetRes)
      return QualType();

   if (!RetTy->isAutoType()) {
      if (RetTy != Func->getReturnType()) {
         return QualType();
      }
   }

   if (!AllArgsResolved) {
      if (!getCurrentDecl()->inDependentContext())
         return QualType();

      // return a new function type with all types that we could resolve to
      // aid inference
      return Context.getLambdaType(RetTy->isAutoType()
                                      ? Func->getReturnType()
                                      : RetTy.getResolvedType(),
                                   ArgTys);
   }

   // evaluate the lambda expression to help inference via the return type
   auto Res = visitLambdaExpr(LE);
   if (!Res)
      return QualType();

   return LE->getExprType();
}

QualType SemaPass::GetDefaultExprType(Expression *E)
{
   E = E->ignoreParens();

   assert(E->isContextDependent() && "not a context dependent expression!");
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID:
   case Expression::CharLiteralID:
   case Expression::BoolLiteralID:
   case Expression::FPLiteralID:
   case Expression::StringLiteralID: {
      auto Result = visitExpr(E);
      if (!Result) {
         return QualType();
      }

      return Result.get()->getExprType();
   }
   case Expression::LambdaExprID: {
      auto LE = cast<LambdaExpr>(E);

      std::vector<QualType> ParamTys;
      ParamTys.reserve(LE->getArgs().size());

      // check only the types of explicitly specified arguments
      for (auto &Arg : LE->getArgs()) {
         auto GivenTypeRes = visitSourceType(Arg, Arg->getType());
         if (!GivenTypeRes) {
            LE->setIsInvalid(true);
            return QualType();
         }

         // can't determine the type of this expression without context
         if (GivenTypeRes.getValue()->isAutoType())
            return QualType();

         ParamTys.push_back(GivenTypeRes.getValue());
      }

      auto RetResult = visitSourceType(LE, LE->getReturnType());
      if (!RetResult)
         return QualType();

      QualType RetTy;
      if (RetResult.getValue()->isAutoType())
         RetTy = Context.getVoidType();
      else
         RetTy = RetResult.getValue();

      return Context.getLambdaType(RetTy, ParamTys);
   }
   case Expression::IdentifierRefExprID:
   case Expression::CallExprID:
   case Expression::BuiltinIdentExprID: {
      return QualType();
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

ExprResult SemaPass::visitTypeDependentContextualExpr(Expression *E)
{
   E = E->ignoreParens();

   if (!E->isContextDependent()) {
      return visitExpr(E);
   }

   E->setIsTypeDependent(true);
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID:
   case Expression::CharLiteralID:
   case Expression::BoolLiteralID:
   case Expression::FPLiteralID:
   case Expression::StringLiteralID:
   case Expression::BuiltinIdentExprID:
   case Expression::IdentifierRefExprID:
      // These expressions do not have any substatements that need to be
      // checked.
      return ExprError();
   case Expression::LambdaExprID: {
      auto *LE = cast<LambdaExpr>(E);

      // Check return type.
      (void) visitSourceType(LE, LE->getReturnType());

      // Check argument types.
      for (auto *Arg : LE->getArgs()) {
         QC.TypecheckDecl(Arg);
      }

      // Check closure body.
      DeclScopeRAII DSR(*this, LE->getFunc());
      auto Result = visitStmt(LE, LE->getBody());
      if (Result) {
         LE->setBody(Result.get());
      }

      break;
   }
   case Expression::CallExprID: {
      auto *Call = cast<CallExpr>(E);

      // Check arguments.
      for (auto &Arg : Call->getArgs()) {
         auto Result = visitTypeDependentContextualExpr(Arg);
         if (Result) {
            Arg = Result.get();
         }
      }

      break;
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }

   return E;
}

} // namespace ast
} // namespace cdot