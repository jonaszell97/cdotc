#include "cdotc/Sema/SemaPass.h"

#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/OverloadResolver.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

static bool addCandidates(const MultiLevelLookupResult& lookupRes,
                          CandidateSet& CandSet)
{
   // add candidates found via normal lookup
   unsigned Distance = 0;
   for (auto& Lookup : lookupRes) {
      for (auto& D : Lookup) {
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

static bool addCandidates(SemaPass& Sema, DeclContext* Ctx,
                          DeclarationName name, CandidateSet& CandSet,
                          bool IsLocalLookup)
{
   LookupOpts Opts = DefaultLookupOpts;
   if (IsLocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }

   const MultiLevelLookupResult* LookupRes;
   if (Sema.QC.MultiLevelLookup(LookupRes, Ctx, name, Opts)) {
      return true;
   }

   return addCandidates(*LookupRes, CandSet);
}

std::pair<DeclContext*, bool> SemaPass::getAsContext(QualType Ty)
{
   if (auto RecTy = Ty->asRecordType()) {
      return {RecTy->getRecord(), false};
   }

   if (auto Meta = Ty->asMetaType()) {
      if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
         return {MetaRecTy->getRecord(), true};
      }
   }

   return {nullptr, false};
}

NamedDecl* SemaPass::getTypeDecl(QualType Ty)
{
   CanType T = Ty->getDesugaredType()->getCanonicalType();

   if (auto Meta = T->asMetaType()) {
      T = Meta->getUnderlyingType();
   }

   if (auto* RecTy = T->asRecordType()) {
      return RecTy->getRecord();
   }
   if (auto* Alias = T->asTypedefType()) {
      return Alias->getTypedef();
   }

   return nullptr;
}

CandidateSet SemaPass::lookupFunction(DeclarationName name, Expression* SelfArg,
                                      ArrayRef<Expression*> args,
                                      ArrayRef<Expression*> templateArgs,
                                      ArrayRef<IdentifierInfo*> labels,
                                      Statement* Caller, bool suppressDiags)
{
   DeclContext* DC = nullptr;
   if (SelfArg) {
      CanType SelfTy = SelfArg->getExprType()
                           ->removeReference()
                           ->removeMetaType()
                           ->getDesugaredType();

      if (auto* RT = SelfTy->asRecordType()) {
         DC = RT->getRecord();
      }
   }

   if (!DC) {
      DC = DeclCtx;
   }

   return lookupFunction(DC, name, SelfArg, args, templateArgs, labels, Caller,
                         suppressDiags);
}

CandidateSet SemaPass::lookupFunction(DeclContext* Ctx, DeclarationName name,
                                      Expression* SelfArg,
                                      ArrayRef<Expression*> args,
                                      ArrayRef<Expression*> templateArgs,
                                      ArrayRef<IdentifierInfo*> labels,
                                      Statement* Caller, bool suppressDiags)
{
   CandidateSet CandSet;
   bool IsLocalLookup = Ctx == &getDeclContext();

   addCandidates(*this, Ctx, name, CandSet, IsLocalLookup);
   lookupFunction(CandSet, name, SelfArg, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

CandidateSet SemaPass::getCandidates(DeclarationName name, Expression* SelfExpr)
{
   CandidateSet CandSet;
   if (addCandidates(*this, &getDeclContext(), name, CandSet, true)) {
      return CandSet;
   }

   return CandSet;
}

CandidateSet SemaPass::lookupCase(DeclarationName name, EnumDecl* E,
                                  ArrayRef<Expression*> args,
                                  ArrayRef<Expression*> templateArgs,
                                  ArrayRef<IdentifierInfo*> labels,
                                  Statement* Caller, bool suppressDiags)
{
   CandidateSet CandSet;
   if (auto Case = E->hasCase(name)) {
      CandSet.addCandidate(Case);
   }

   lookupFunction(CandSet, name, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

static void checkIfCalledFromTemplate(SemaPass& SP, CallableDecl* C)
{
   if (!isa<NamedDecl>(SP.getDeclContext()))
      return;

   // Check if the function was called from a template.
   if (cast<NamedDecl>(SP.getDeclContext()).isInUnboundedTemplate()) {
      C->setCalledFromTemplate(true);
      SP.getILGen().notifyFunctionCalledInTemplate(C);
   }
}

void SemaPass::lookupFunction(CandidateSet& CandSet, DeclarationName name,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels, Statement* Expr,
                              bool suppressDiags)
{
   lookupFunction(CandSet, name, nullptr, args, templateArgs, labels, Expr,
                  suppressDiags);
}

void SemaPass::lookupFunction(CandidateSet& CandSet, DeclarationName name,
                              Expression* SelfArg, ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels, Statement* Expr,
                              bool suppressDiags)
{
   if (CandSet.Candidates.empty()) {
      if (!suppressDiags) {
         diagnose(Expr, err_func_not_found, Expr->getSourceRange(), 0, name);
      }

      return;
   }

   auto* BestCand = sema::resolveCandidateSet(
       *this, CandSet, SelfArg, args, labels, templateArgs, SourceType(), Expr,
       !suppressDiags);

   if (!BestCand) {
      if (CandSet.isDependent()) {
         for (auto& Cand : CandSet.Candidates) {
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

MethodDecl* SemaPass::getEquivalentMethod(MethodDecl* Orig, RecordDecl* Inst)
{
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

   const MultiLevelLookupResult* LookupRes;
   if (QC.MultiLevelLookup(LookupRes, Inst, Name)) {
      return Orig;
   }

   for (auto* D : LookupRes->allDecls()) {
      auto* M = dyn_cast<MethodDecl>(D);
      if (!M)
         continue;

      if (M->getMethodID() == Orig->getMethodID())
         return M;
   }

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate& Cand,
                                      const TemplateArgList& templateArgs,
                                      Statement* Caller)
{
   auto F = Cand.getFunc();
   Cand.OuterTemplateArgs
       = FinalTemplateArgumentList::Create(Context, templateArgs);

   if (templateArgs.isStillDependent()) {
      return true;
   }

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return true;

      RecordDecl* Inst = Instantiator->InstantiateRecord(
         Case->getRecord(), Cand.OuterTemplateArgs, Caller->getSourceLoc());

      if (!Inst)
         return false;

      Cand.setCandDecl(maybeInstantiateTemplateMember(Inst, Case));
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return true;

   RecordDecl* Inst = Instantiator->InstantiateRecord(
      Init->getRecord(), Cand.OuterTemplateArgs, Caller->getSourceLoc());

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

void SemaPass::maybeInstantiate(CandidateSet& CandSet, Statement* Caller)
{
   if (!CandSet) {
      return;
   }

   maybeInstantiate(CandSet.getBestMatch(), Caller);
}

void SemaPass::maybeInstantiate(CandidateSet::Candidate& Cand,
                                Statement* Caller)
{
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
      Info->loadBody(F);
   }

   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         Cand.setCandDecl(maybeInstantiateMemberFunction(M, Caller));
      }

      return;
   }

   auto& TAs = Cand.InnerTemplateArgs;
   auto* FinalList = FinalTemplateArgumentList::Create(Context, TAs);

   if (auto *CS = Context.getExtConstraints(F)) {
      if (!CS->empty()) {
         DeclConstraint *FailedConstraint = nullptr;
         for (auto *C : *CS) {
            bool satisfied = true;
            if (QC.IsConstraintSatisfied(satisfied, C, F, F, FinalList)) {
               continue;
            }

            if (!satisfied) {
               FailedConstraint = C;
               break;
            }
         }

         if (FailedConstraint) {
            Cand.setHasFailedConstraint(FailedConstraint);
            return;
         }
      }
   }

   if (auto Fn = dyn_cast<FunctionDecl>(Cand.getFunc())) {
      FunctionDecl* Inst = Instantiator->InstantiateFunction(
         Fn, FinalList, Caller->getSourceLoc());

      if (!Inst || QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.setCandDecl(Inst);
   }
   else {
      if (F->getRecord()->isProtocol()) {
         return;
      }

      MethodDecl* Inst = Instantiator->InstantiateMethod(
         cast<MethodDecl>(Cand.getFunc()), FinalList, Caller->getSourceLoc());

      if (!Inst || QC.PrepareDeclInterface(Inst)) {
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

CallableDecl*
SemaPass::maybeInstantiateMemberFunction(CallableDecl* Fn, StmtOrDecl Caller,
                                         bool NeedImmediateInstantiation)
{
   if (Fn->getBody() || !Fn->getBodyTemplate()) {
      return Fn;
   }

   CallableDecl* Template = Fn->getBodyTemplate();
   if (Template->isInvalid()) {
      return Fn;
   }

   if (auto FnInfo = Template->getLazyFnInfo()) {
      FnInfo->loadBody(Template);
   }

   if (NeedImmediateInstantiation) {
      size_t prevSize = QueuedInstantiations.size();
      Instantiator->InstantiateFunctionBody(Fn);

      for (size_t i = prevSize; i < QueuedInstantiations.size(); ++i) {
         Instantiator->InstantiateFunctionBody(QueuedInstantiations[i]);
      }
   }
   else {
      QueuedInstantiations.insert(Fn);
   }

   return Fn;
}

MethodDecl* SemaPass::InstantiateMethod(RecordDecl* R, StringRef Name,
                                        StmtOrDecl SOD)
{
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

MethodDecl* SemaPass::InstantiateProperty(RecordDecl* R, StringRef Name,
                                          bool FindGetter, StmtOrDecl SOD)
{
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

RecordDecl* SemaPass::InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                        FinalTemplateArgumentList* TemplateArgs)
{
   return Instantiator->InstantiateRecord(R, TemplateArgs, POI);
}

AliasDecl* SemaPass::InstantiateAlias(SourceLocation POI, AliasDecl* td,
                                      FinalTemplateArgumentList* TemplateArgs)
{
   return Instantiator->InstantiateAlias(td, TemplateArgs, POI);
}

RecordDecl* SemaPass::InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                        const TemplateArgList& TemplateArgs)
{
   return InstantiateRecord(
       POI, R, FinalTemplateArgumentList::Create(Context, TemplateArgs));
}

LLVM_ATTRIBUTE_UNUSED
static bool wouldSelfBeValid(SemaPass& SP, bool& Static)
{
   Static = false;

   auto* Ctx = &SP.getDeclContext();
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
static void visitContextDependentArgs(SemaPass& Sema, Expr* C)
{
   for (auto* Arg : C->getArgs()) {
      Sema.visitTypeDependentContextualExpr(Arg);
      C->copyStatusFlags(Arg);
   }
}

Expression* SemaPass::convertCStyleVarargParam(Expression* Expr)
{
   Expr = castToRValue(Expr);
   QualType type = Expr->getExprType();

   // (unsigned) char / short are promoted to int for c-style vararg functions.
   if (type->isIntegerType() && type->getBitwidth() < 32) {
      QualType promotedType
          = type->isUnsigned() ? Context.getUInt32Ty() : Context.getInt32Ty();

      return forceCast(Expr, promotedType);
   }

   // float is promoted to double for c-style vararg functions.
   if (type->isFloatTy()) {
      return forceCast(Expr, Context.getDoubleTy());
   }

   return Expr;
}

ExprResult SemaPass::visitCallExpr(CallExpr* Call, TemplateArgListExpr* ArgExpr)
{
   CallableDecl* C = Call->getFunc();
   C = maybeInstantiateMemberFunction(C, Call);

   if (QC.PrepareDeclInterface(C)) {
      Call->setIsInvalid(true);
      return Call;
   }

   if (!Call->getExprType()) {
      QualType ExprType;
      if (auto* F = dyn_cast<FunctionDecl>(C)) {
         ExprType = F->getReturnType();
      }
      else if (auto* Case = dyn_cast<EnumCaseDecl>(C)) {
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
   }

   unsigned i = 0;
   MutableArrayRef<FuncArgDecl*> params = C->getArgs();
   bool isTemplate = C->isTemplateOrInTemplate();

   for (Expression*& argVal : Call->getArgs()) {
      QualType neededType;
      bool cstyleVararg = false;
      bool isVariadic = false;

      if (i < params.size() || !C->isCstyleVararg()) {
         auto *param = i < params.size() ? params[i] : params.back();
         neededType = param->getType();
         isVariadic = param->isVariadic();

         if (param->hasAttribute<AutoClosureAttr>()) {
            neededType = Context.getLambdaType(neededType, {}, {});
         }
      }
      else {
         cstyleVararg = true;
      }

      if (isTemplate && Call->getTemplateArgs() && !isVariadic) {
         if (QC.SubstTemplateParamTypes(neededType, neededType,
                                        *Call->getTemplateArgs(),
                                         Call->getSourceRange())) {
            ++i;
            Call->setIsInvalid(true);

            continue;
         }
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
}

LLVM_ATTRIBUTE_UNUSED
static void
addTemplateArgs(SmallVectorImpl<Expression*>& Exprs,
                SmallVectorImpl<std::unique_ptr<BuiltinExpr>>& PlaceholderExprs,
                const sema::TemplateArgument& Arg)
{
   if (Arg.isVariadic()) {
      for (auto& VA : Arg.getVariadicArgs()) {
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

CallExpr* SemaPass::CreateCall(CallableDecl* C, ArrayRef<Expression*> Args,
                               SourceLocation Loc)
{
   ASTVector<Expression*> Vec(Context, Args);
   return CreateCall(C, move(Vec), Loc);
}

CallExpr* SemaPass::CreateCall(CallableDecl* C, ASTVector<Expression*>&& Args,
                               SourceLocation Loc)
{
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

   auto* callExpr = CallExpr::Create(Context, Loc, SourceRange(Loc), move(Args),
                                     C, K, QualType());

   callExpr->setIsDotInit(C->isBaseInitializer());
   return callExpr;
}

ExprResult SemaPass::visitAnonymousCallExpr(AnonymousCallExpr* Call)
{
   return Call;
}

ExprResult SemaPass::visitTypeDependentContextualExpr(Expression* E)
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
      auto* LE = cast<LambdaExpr>(E);

      // Check return type.
      (void)visitSourceType(LE, LE->getReturnType());

      // Check argument types.
      for (auto* Arg : LE->getArgs()) {
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
      auto* Call = cast<CallExpr>(E);

      // Check arguments.
      for (auto& Arg : Call->getArgs()) {
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