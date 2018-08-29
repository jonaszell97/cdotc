//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Type.h"
#include "Basic/NestedNameSpecifier.h"
#include "IL/Constants.h"
#include "IL/Function.h"
#include "ILGen/ILGenPass.h"
#include "Query/QueryContext.h"
#include "Serialization/IncrementalCompilation.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace ast {

NamedDecl* SemaPass::maybeInstantiateTemplateMemberImpl(DeclContext *LookupCtx,
                                                        NamedDecl *Member) {
   auto *TemplateCtx = Member->getRecord();
   if (!TemplateCtx || !TemplateCtx->isTemplate())
      return Member;

   LookupCtx = LookupCtx->lookThroughExtension();

   // Find the instantiation this member was found in.
   RecordDecl *Inst = nullptr;
   while (LookupCtx) {
      Inst = dyn_cast<RecordDecl>(LookupCtx);
      if (Inst
            && Inst->isInstantiation()
            && Inst->getSpecializedTemplate() == TemplateCtx) {
         break;
      }

      LookupCtx = LookupCtx->getParentCtx();
   }

   if (!Inst)
      return Member;

   NamedDecl *MemberInst;
   if (QC.InstantiateTemplateMember(MemberInst, Member, Inst)) {
      MemberInst = Member;
   }

   return MemberInst;
}

void SemaPass::diagnoseCircularlyDependentGlobalVariables(Expression *Expr,
                                                          NamedDecl *globalVar){
   Expr->setIsInvalid(true);

   diagnose(globalVar, err_circular_global_value, globalVar->getSourceLoc(),
            globalVar->getDeclName(), EvaluatingGlobalVar->getDeclName());
   diagnose(note_dependent_global_here, EvaluatingGlobalVar->getSourceLoc());
}

static bool checkImplicitSelf(SemaPass &SP,
                              NamedDecl *Decl,
                              IdentifierRefExpr *Ident) {
   if (Ident->getParentExpr() || Decl->isStatic() || Ident->isStaticLookup())
      return true;

   auto Self = SelfExpr::Create(SP.getContext(), Ident->getSourceLoc(), false);

   SP.updateParent(Ident, Self);
   Ident->setParentExpr(Self);

   return SP.visitExpr(Ident, Self);
}

static void checkCapture(SemaPass &SP, Expression *Expr)
{
   if (!SP.getBoxDecl()) {
      SP.diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                  /*Box*/ 13);
   }
}

QualType SemaPass::getStaticForValue(IdentifierInfo *name) const
{
   for (auto S = currentScope; S; S = S->getEnclosingScope()) {
      if (auto SF = dyn_cast<StaticForScope>(S)) {
         if (SF->getElementName() == name)
            return SF->getElementTy();
      }
   }

   return QualType();
}

Expression *SemaPass::UnwrapExistential(QualType ParentType, Expression *Expr)
{
   if (!ParentType->isDependentRecordType())
      return Expr;

   if (!Expr->getExprType()->containsGenericType())
      return Expr;

   QualType Ty = resolveDependencies(Expr->getExprType(),
                                     ParentType->getTemplateArgs(),
                                     Expr);

   auto *Seq = ConversionSequence::Create(Context, CastStrength::Implicit,
                                          ConversionStep(
                                             CastKind::ExistentialRef,
                                             Ty));

   Expr->setSemanticallyChecked(true);
   auto *Conv = ImplicitCastExpr::Create(Context, Expr, Seq);

   auto Res = visitExpr(Conv);
   (void)Res; assert(Res && "bad implicit cast sequence!");

   return Conv;
}

static IdentifierRefExpr* CheckBuiltinType(SemaPass &SP,
                                           IdentifierRefExpr *Ident,
                                           bool &IsBuiltin,
                                           bool &UsedTemplateArgs,
                                           ArrayRef<Expression*> TemplateArgs) {
   enum class Kind {
      MetaType = 0, UnsafePtr, UnsafeMutablePtr,
   };

   auto DeclName = Ident->getDeclName();
   IsBuiltin = true;
   UsedTemplateArgs = false;

   if (DeclName.isStr("MetaType")) {
      if (TemplateArgs.size() != 1) {
         SP.diagnose(Ident, err_builtin_type_expects, (int)Kind::MetaType,
                     Ident->getSourceRange());

         return nullptr;
      }

      auto ArgRes = SP.visitExpr(Ident, TemplateArgs.front());
      if (!ArgRes)
         return nullptr;

      UsedTemplateArgs = true;

      auto Ty = ArgRes.get()->getExprType();
      if (Ty->isUnknownAnyType()) {
         Ident->setIsTypeDependent(true);
         Ident->setExprType(SP.getContext().getUnknownAnyTy());

         return nullptr;
      }

      if (!Ty->isMetaType()) {
         SP.diagnose(Ident, err_builtin_type_expects, (int)Kind::MetaType,
                     Ident->getSourceRange());

         return nullptr;
      }

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(Ty));

      return Ident;
   }

   if (DeclName.isStr("UnsafePtr")) {
      if (TemplateArgs.size() != 1) {
         SP.diagnose(Ident, err_builtin_type_expects, (int)Kind::UnsafePtr,
                     Ident->getSourceRange());

         return nullptr;
      }

      auto ArgRes = SP.visitExpr(Ident, TemplateArgs.front());
      if (!ArgRes)
         return nullptr;

      UsedTemplateArgs = true;

      auto Ty = ArgRes.get()->getExprType();
      if (Ty->isUnknownAnyType()) {
         Ident->setIsTypeDependent(true);
         Ident->setExprType(SP.getContext().getUnknownAnyTy());

         return nullptr;
      }
      if (!Ty->isMetaType()) {
         SP.diagnose(Ident, err_builtin_type_expects, (int)Kind::UnsafePtr,
                     Ident->getSourceRange());

         return nullptr;
      }

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(
         SP.getContext().getPointerType(
            Ty->asMetaType()->getUnderlyingType())));

      return Ident;
   }

   if (DeclName.isStr("UnsafeMutablePtr")) {
      if (TemplateArgs.size() != 1) {
         SP.diagnose(Ident, err_builtin_type_expects,
                     (int)Kind::UnsafeMutablePtr, Ident->getSourceRange());

         return nullptr;
      }

      auto ArgRes = SP.visitExpr(Ident, TemplateArgs.front());
      if (!ArgRes)
         return nullptr;

      auto Ty = ArgRes.get()->getExprType();
      if (Ty->isUnknownAnyType()) {
         Ident->setIsTypeDependent(true);
         Ident->setExprType(SP.getContext().getUnknownAnyTy());

         return nullptr;
      }
      if (!Ty->isMetaType()) {
         SP.diagnose(Ident, err_builtin_type_expects,
                     (int)Kind::UnsafeMutablePtr, Ident->getSourceRange());

         return nullptr;
      }

      UsedTemplateArgs = true;

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(
         SP.getContext().getMutablePointerType(
            Ty->asMetaType()->getUnderlyingType())));

      return Ident;
   }

   if (DeclName.isStr("UnsafeRawPtr")) {
      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(
         SP.getContext().getPointerType(SP.getContext().getVoidType())));

      return Ident;
   }

   if (DeclName.isStr("UnsafeMutableRawPtr")) {
      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(
         SP.getContext().getMutablePointerType(SP.getContext().getVoidType())));

      return Ident;
   }

   if (auto ty = SP.getBuiltinType(DeclName)) {
      IsBuiltin = true;
      Ident->setKind(IdentifierKind::MetaType);
      Ident->setExprType(SP.getContext().getMetaType(ty));

      return Ident;
   }

   IsBuiltin = false;
   return Ident;
}

template<class ExprTy>
void MakeDeref(SemaPass &SP, QualType ty, ExprTy *Expr)
{
   QualType RefTy;
   if (ty->isMutablePointerType()) {
      RefTy = SP.getContext().getMutableReferenceType(ty->getPointeeType());
   }
   else {
      RefTy = SP.getContext().getReferenceType(ty->getPointeeType());
   }

   auto ParentExpr = SP.castToRValue(Expr->getParentExpr());

   FunctionType *DerefFnTy = SP.getContext().getFunctionType(RefTy, { ty });
   auto Deref = UnaryOperator::Create(SP.getContext(), Expr->getSourceLoc(),
                                      op::Deref, DerefFnTy, ParentExpr,
                                      true);

   auto Res = SP.visitExpr(Expr, Deref);
   assert(Res && "invalid deref operator?");

   Expr->setParentExpr(Res.get());
   Expr->setIsPointerAccess(false);
}

static ExprResult makeNestedNameSpec(SemaPass &SP,
                                     ASTContext &Context,
                                     NestedNameSpecifier *NameSpec,
                                     IdentifierRefExpr *Ident) {
   NameSpec = NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                          Ident->getIdentInfo(),
                                          NameSpec);

   SmallVector<SourceRange, 4> Locs{ Ident->getSourceRange() };
   Expression *Curr = Ident->getParentExpr();

   while (Curr) {
      auto *IE = dyn_cast<IdentifierRefExpr>(Curr->ignoreTemplateArgs());
      if (!IE || !IE->getNameSpec())
         break;

      Locs.push_back(IE->getSourceRange());
      Curr = Curr->getParentExpr();
   }

   std::reverse(Locs.begin(), Locs.end());

   auto WithLoc = NestedNameSpecifierWithLoc::Create(Context.getDeclNameTable(),
                                                     NameSpec, Locs);

   auto Ty = Context.getDependentNameType(WithLoc);
   Ident->setNameSpec(NameSpec);
   Ident->setKind(IdentifierKind::MetaType);
   Ident->setExprType(Context.getMetaType(Ty));
   Ident->setIsTypeDependent(true);

   return Ident;
}

bool SemaPass::refersToNamespace(Expression *E)
{
   auto *Ident = dyn_cast<IdentifierRefExpr>(E);
   if (!Ident)
      return false;

   switch (Ident->getKind()) {
   case IdentifierKind::Namespace:
   case IdentifierKind::Import:
   case IdentifierKind::Module:
      return true;
   default:
      return false;
   }
}

QualType SemaPass::getParentType(Expression *ParentExpr)
{
   auto *Ident = dyn_cast_or_null<IdentifierRefExpr>(ParentExpr);
   if (!Ident)
      return QualType();

   if (Ident->getKind() != IdentifierKind::MetaType)
      return QualType();

   auto ParentType = Ident->getExprType()->asMetaType()->getUnderlyingType();
   if (auto *Dep = ParentType->asDependentRecordType()) {
      return Context.getDependentRecordType(
         Dep->getRecord(), &Dep->getTemplateArgs(),
         getParentType(Ident->getParentExpr()));
   }

   return ParentType;
}

ExprResult SemaPass::visitIdentifierRefExpr(IdentifierRefExpr *Ident,
                                            TemplateArgListExpr *ArgExpr) {
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr)
      TemplateArgs = ArgExpr->getExprs();

   // Check if this expressions parent expr refers to a namespace.
   auto *NameSpec = checkNamespaceRef(Ident);
   if (Ident->isInvalid())
      return ExprError();

   if (NameSpec && NameSpec->isAnyNameDependent()) {
      return makeNestedNameSpec(*this, Context, NameSpec, Ident);
   }

   // Check the parent expression.
   QualType ParentType;
   auto *PE = Ident->getParentExpr();
   if (PE && !refersToNamespace(PE)) {
      auto ParentRes = visitExpr(Ident, PE);
      if (!ParentRes)
         return ExprError();

      PE = ParentRes.get();
      Ident->setParentExpr(PE);

      ParentType = PE->getExprType()->stripReference();
      if (ParentType->isUnknownAnyType()) {
         return makeNestedNameSpec(*this, Context, NameSpec, Ident);
      }

      // If it's a meta type, it can either:
      //   - refer to a DeclContext if it's a record type
      //   - refer to a static type member otherwise
      if (ParentType->isMetaType()) {
         auto Underlying = ParentType->asMetaType()->getUnderlyingType();
         if (!Underlying->isRecordType()) {
            return HandleStaticTypeMember(Ident, Underlying);
         }

         ParentType = Underlying;
         Ident->setStaticLookup(true);
      }

      // Handle '->'
      if (ParentType->isPointerType() && Ident->isPointerAccess()) {
         if (!Ident->isTypeDependent()) {
            MakeDeref(*this, ParentType, Ident);
            Ident->setIsPointerAccess(false);

            PE = Ident->getParentExpr();
            ParentType = PE->getExprType()->stripReference();
         }
         else {
            ParentType = ParentType->getPointeeType();
         }
      }
      else if (Ident->isPointerAccess()) {
         diagnose(Ident, err_member_access_non_pointer, ParentType,
                  Ident->getSourceLoc());
         Ident->setIsPointerAccess(false);
      }

      // If it's not a record type at this point, try a builtin type member.
      if (!ParentType->stripReference()->isRecordType()) {
         return HandleBuiltinTypeMember(Ident, ParentType);
      }

      auto *R = ParentType->stripReference()->getRecord();
      if (QC.PrepareDeclInterface(R)) {
         return makeNestedNameSpec(*this, Context, NameSpec, Ident);
      }

      Ident->setDeclCtx(R);
   }
   else if (Ident->hasLeadingDot()) {
      auto CtxTy = Ident->getContextualType();
      if (!CtxTy || !CtxTy->isRecordType()) {
         diagnose(Ident, err_requires_contextual_type, 2,
                  Ident->getSourceRange());

         return ExprError();
      }

      auto *R = CtxTy->getRecord();
      if (QC.PrepareDeclInterface(R)) {
         return makeNestedNameSpec(*this, Context, NameSpec, Ident);
      }

      Ident->setDeclCtx(R);
   }

   bool UsedTemplateParams = false;
   MultiLevelLookupResult LookupResult;
   auto DeclName = Ident->getDeclName();
   bool LocalLookup = !Ident->getDeclCtx()
                      || Ident->getDeclCtx() == &getDeclContext();

   if (LocalLookup)
      Ident->setDeclCtx(&getDeclContext());

   DeclContext *LookupCtx = Ident->getDeclCtx();
   if (!LookupResult) {
      LookupResult = MultiLevelLookup(*LookupCtx, DeclName, true,
                                      LocalLookup);

      // nothing found through normal lookup, check builtin members or
      // declarations
      if (!LookupResult) {
         if (LocalLookup) {
            bool IsBuiltin;
            auto BuiltinRes = CheckBuiltinType(*this, Ident, IsBuiltin,
                                               UsedTemplateParams,TemplateArgs);

            if (IsBuiltin) {
               Ident->setFoundResult(true);

               if (!BuiltinRes) {
                  Ident->setExprType(ErrorTy);

                  if (Ident->isDependent())
                     return Ident;

                  return ExprError();
               }

               return BuiltinRes;
            }

            if (DeclName.isSimpleIdentifier()) {
               if (auto Ty = getStaticForValue(DeclName.getIdentifierInfo())) {
                  Ident->setIsValueDependent(true);
                  Ident->setIsTypeDependent(true);
                  Ident->setExprType(Ty);
                  Ident->setFoundResult(true);

                  return Ident;
               }
            }
         }

         if (auto R = dyn_cast<RecordDecl>(Ident->getDeclCtx())) {
            // builtin members
            if (DeclName.isStr("typeof")) {
               auto TI = getTypeInfoDecl();
               if (!TI) {
                  diagnose(Ident, err_no_builtin_decl, Ident->getSourceRange(),
                           10);

                  return ExprError();
               }

               Ident->setFoundResult(true);
               Ident->setKind(IdentifierKind::TypeOf);
               Ident->setExprType(Context.getReferenceType(
                  Context.getRecordType(TI)));
               Ident->setMetaType(Context.getMetaType(
                  Context.getRecordType(R)));

               return Ident;
            }
         }
      }
   }

   // nothing left to check, this identifier does not reference a declaration
   // in scope
   if (!LookupResult) {
      if (currentScope && currentScope->hasUnresolvedStaticCond()) {
         // this identifier might have been introduced in a dependent
         // static if / for
         Ident->setIsTypeDependent(true);
         Ident->setExprType(UnknownAnyTy);

         // For now, assume this identifier is valid.
         Ident->setFoundResult(true);

         return Ident;
      }
      if (!Ident->shouldIssueDiag()) {
         Ident->setIsInvalid(true);
         return ExprError();
      }

      // If this expression is type dependent, new declarations might be
      // visible at instantiation time, so don't report an error for now.
      if (Ident->isTypeDependent() || inUnboundedTemplate()) {
         Ident->setIsTypeDependent(true);
         Ident->setExprType(UnknownAnyTy);

         return Ident;
      }

      if (Ident->isSynthesized()) {
         unsigned DiagIdx;
         if (DeclName.isStr("Array")) {
            DiagIdx = 5;
         }
         else if (DeclName.isStr("Dictionary")) {
            DiagIdx = 6;
         }
         else if (DeclName.isStr("Option")) {
            DiagIdx = 7;
         }
         else {
            llvm_unreachable("bad synthesized identifier!");
         }

         diagnose(Ident, err_no_builtin_decl, Ident->getSourceRange(),
                  DiagIdx);

         return ExprError();
      }

      MessageKind diagId;
      bool WithContext = false;

      // provide appropriate diagnostic for type lookup, member lookup and
      // normal identifier lookup
      if (Ident->isInTypePosition()) {
         diagId = diag::err_type_not_found;
      }
      else if (Ident->getParentExpr()) {
         diagId = diag::err_member_not_found;
         WithContext = true;
      }
      else {
         diagId = diag::err_undeclared_identifer;
      }

      diagnoseMemberNotFound(WithContext ? Ident->getDeclCtx() : nullptr,
                             Ident, DeclName, diagId);

      return ExprError();
   }

   Ident->setFoundResult(true);

   // We found something. Get the relevant declaration.
   LambdaScope *lambdaScope = LookupResult.front().LS;
   NamedDecl *FoundDecl = LookupResult.front().front();
   QualType ResultType;

   if (FoundDecl->isInvalid()) {
      Ident->setIsInvalid(true);
      return ExprError();
   }

   if (auto Var = dyn_cast<LocalVarDecl>(FoundDecl)) {
      assert(LocalLookup && "found local variable through member lookup?");

      if (lambdaScope) {
         Var->setCaptured(true);

         Ident->setIsCapture(true);
         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()
              ->addCapture(Context, Var));

         checkCapture(*this, Ident);
      }

      Ident->setLocalVar(Var);
      Ident->setKind(IdentifierKind::LocalVar);
      Ident->setIsTypeDependent(Var->isTypeDependent());
      Ident->setIsValueDependent(Var->isValueDependent());

      ResultType = Var->getType();
      if (!ResultType->isReferenceType()) {
         if (Var->isConst() && Var->getValue()) {
            ResultType = Context.getReferenceType(ResultType);
         }
         else {
            ResultType = Context.getMutableReferenceType(ResultType);
         }
      }
   }
   else if (auto G = dyn_cast<GlobalVarDecl>(FoundDecl)) {
      if (QC.PrepareDeclInterface(G)) {
         return ExprError();
      }

      if (QC.CheckAccessibility(&getDeclContext(), G)) {
         Ident->setIsInvalid(true);
      }

      Ident->setGlobalVar(G);
      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setIsTypeDependent(G->isTypeDependent());
      Ident->setIsValueDependent(G->isValueDependent());

      ResultType = G->getType();
      if (!ResultType->isReferenceType()) {
         if (G->isConst()) {
            ResultType = Context.getReferenceType(ResultType);
         }
         else {
            ResultType = Context.getMutableReferenceType(ResultType);
         }
      }
   }
   else if (auto Arg = dyn_cast<FuncArgDecl>(FoundDecl)) {
      assert(LocalLookup && "found argument through member lookup?");

      if (lambdaScope) {
         Arg->setCaptured(true);

         Ident->setIsCapture(true);
         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()
              ->addCapture(Context, Arg));

         checkCapture(*this, Ident);
      }

      Ident->setFuncArg(Arg);
      Ident->setKind(IdentifierKind::FunctionArg);
      Ident->setIsTypeDependent(Arg->isTypeDependent());
      Ident->setIsValueDependent(Arg->isValueDependent());

      if (Arg->isVariadicArgPackExpansion()) {
         Ident->setContainsUnexpandedParameterPack(true);
      }

      if (Arg->getType()->isAutoType()) {
         assert(Arg->getDeclName().getKind()
            == DeclarationName::ClosureArgumentName
               && "type should not be ""auto!");

         Ident->setIsTypeDependent(true);
         ResultType = UnknownAnyTy;
      }
      else if (Arg->hasAttribute<AutoClosureAttr>()) {
         // auto closure argument is not an lvalue
         ResultType = Arg->getType();
      }
      else if (Arg->getConvention() == ArgumentConvention::MutableRef
            || Arg->getConvention() == ArgumentConvention::Owned) {
         ResultType = Context.getMutableReferenceType(Arg->getType());
      }
      else {
         ResultType = Context.getReferenceType(Arg->getType());
      }
   }
   else if (auto Case = dyn_cast<EnumCaseDecl>(FoundDecl)) {
      Case = maybeInstantiateTemplateMember(LookupCtx, Case);
      return HandleEnumCase(Ident, cast<EnumDecl>(Case->getRecord()));
   }
   else if (isa<CallableDecl>(FoundDecl)) {
      auto func = checkFunctionReference(Ident, DeclName,
                                         LookupResult,
                                         TemplateArgs);

      if (!func) {
         return ExprError();
      }

      if (QC.CheckAccessibility(&getDeclContext(), func)) {
         Ident->setIsInvalid(true);
      }

      UsedTemplateParams = func->isInstantiation();

      FunctionType *FTy = func->getFunctionType();
      if (auto M = dyn_cast<MethodDecl>(func)) {
         if (!Ident->isStaticLookup()) {
            auto *PE = Ident->getParentExpr();
            if (!PE->isLValue()) {
               diagnose(Ident, err_generic_error,
                  "cannot apply 'self' function reference to temporary value",
                  Ident->getSourceRange());
            }
            else if (auto *IE = dyn_cast<IdentifierRefExpr>(PE)) {
               IE->getVarDecl()->setCaptured(true);
            }
            else if (auto *SE = dyn_cast<SelfExpr>(PE)) {
               SE->setCaptureIndex(0);
               getCurrentFun()->getArgs().front()->setCaptured(true);
            }

            // Function does not take 'self' argument
            FTy = Context.getLambdaType(FTy->getReturnType(),
                                        FTy->getParamTypes().drop_front(1),
                                        FTy->getParamInfo().drop_front(1),
                                        FTy->getRawFlags());

            Ident->setPartiallyAppliedMethod(M);
         }
         else {
            Ident->setCallable(func);
         }
      }
      else {
         Ident->setCallable(func);
      }

      ResultType = FTy;
   }
   else if (auto Alias = dyn_cast<AliasDecl>(FoundDecl)) {
      Alias = maybeInstantiateTemplateMember(LookupCtx, Alias);

      if (Alias->isBeingEvaluated() && !Alias->isTemplate()) {
         diagnose(Ident, err_referenced_while_evaluating, 0 /*alias*/,
                  Alias->getDeclName(), Ident->getSourceRange());

         return ExprError();
      }

      if (QC.CheckAccessibility(&getDeclContext(), Alias)) {
         Ident->setIsInvalid(true);
      }

      if (TemplateArgs.empty()
            && Alias->isTemplate()
            && Ident->allowIncompleteTemplateArgs()) {
         QC.TypecheckDecl(Alias);

         Ident->setKind(IdentifierKind::Alias);
         Ident->setAlias(Alias);
         Ident->setExprType(Alias->getType());

         return Ident;
      }

      auto AliasRes = checkAlias(LookupResult, TemplateArgs, Ident);
      if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
         return makeNestedNameSpec(*this, Context, NameSpec, Ident);
      }

      if (!AliasRes) {
         AliasRes.getCandSet().diagnoseAlias(*this, Alias->getDeclName(),
                                             TemplateArgs, Ident);

         assert(Ident->isInvalid());
         return ExprError();
      }

      Alias = AliasRes.getAlias();

      if (Alias->isInvalid()) {
         Ident->setIsInvalid(true);
         return ExprError();
      }

      Ident->setKind(IdentifierKind::Alias);
      Ident->setAlias(Alias);

      if (Alias->getType()->isMetaType()) {
         Ident->setNameSpec(NestedNameSpecifier::Create(
            Context.getDeclNameTable(),
            Alias->getType()->asMetaType()->getUnderlyingType(),
            NameSpec));
      }

      if (isInReflectModule(Alias)) {
         return HandleReflectionAlias(Alias, Ident);
      }
      if (isInBuiltinModule(Alias)) {
         return HandleBuiltinAlias(Alias, Ident);
      }

      UsedTemplateParams = Alias->isInstantiation();
      ResultType = Alias->getType();
   }
   else if (auto NS = dyn_cast<NamespaceDecl>(FoundDecl)) {
      if (!Ident->allowNamespaceRef()) {
         diagnose(Ident, err_generic_error, "unexpected namespace reference",
                  Ident->getSourceRange());
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     NS,
                                                     NameSpec));

      Ident->setKind(IdentifierKind::Namespace);
      Ident->setNamespaceDecl(NS);

      ResultType = ErrorTy;
   }
   else if (auto I = dyn_cast<ImportDecl>(FoundDecl)) {
      if (!Ident->allowNamespaceRef()) {
         diagnose(Ident, err_generic_error, "unexpected import reference",
                  Ident->getSourceRange());
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     I->getImportedModule(),
                                                     NameSpec));

      Ident->setImport(I);
      ResultType = ErrorTy;
   }
   else if (auto Mod = dyn_cast<ModuleDecl>(FoundDecl)) {
      if (!Ident->allowNamespaceRef()) {
         diagnose(Ident, err_generic_error, "unexpected module reference",
                  Ident->getSourceRange());
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     Mod->getModule(),
                                                     NameSpec));

      Ident->setModule(Mod);
      ResultType = ErrorTy;
   }
   else if (auto R = dyn_cast<RecordDecl>(FoundDecl)) {
      R = maybeInstantiateTemplateMember(LookupCtx, R);

      if (QC.PrepareNameLookup(R)) {
         return ExprError();
      }

      if (R->isTemplate()) {
         TemplateArgList list(*this, R, TemplateArgs);
         UsedTemplateParams = true;

         auto Compat = list.checkCompatibility();
         if (!Compat) {
            diagnoseTemplateArgErrors(R, Ident, list, TemplateArgs, Compat);
            return ExprError();
         }

         auto FinalList = FinalTemplateArgumentList::Create(Context, list,
                                                            false);

         if (list.isStillDependent()) {
            ResultType = Context.getDependentRecordType(
               R, FinalList, getParentType(Ident->getParentExpr()));
         }
         else {
            RecordDecl *Inst;
            if (QC.InstantiateRecord(Inst, R, FinalList, Ident->getSourceLoc()))
               Inst = R;

            ResultType = Context.getRecordType(R);
         }
      }
      else {
         ResultType = Context.getRecordType(R);
      }

      if (QC.CheckAccessibility(&getDeclContext(), R)) {
         Ident->setIsInvalid(true);
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     ResultType,
                                                     NameSpec));

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setMetaType(Context.getMetaType(ResultType));

      ResultType = Ident->getMetaType();
   }
   else if (auto F = dyn_cast<FieldDecl>(FoundDecl)) {
      F = maybeInstantiateTemplateMember(LookupCtx, F);

      if (!checkImplicitSelf(*this, F, Ident))
         return ExprError();

      if (!F->isStatic() && Ident->isStaticLookup()) {
         auto Rec = F->getRecord();
         diagnose(Ident, err_non_static_member_accessed_statically,
                  Ident->getSourceLoc(), /*field*/ 0, F->getName(),
                  Rec->getSpecifierForDiagnostic(), Rec->getName());

         return ExprError();
      }

      if (!F->getType().isResolved() || F->getType()->isAutoType()) {
         if (!VisitedGlobalDecls.insert(F).second) {
            diagnoseCircularlyDependentGlobalVariables(Ident, F);
            return ExprError();
         }

         DeclScopeRAII declScopeRAII(*this, F->getDeclContext());
         visitFieldDecl(F);
      }

      auto Res = HandleFieldAccess(Ident, TemplateArgs, F);
      if (!Res)
         return Res;

      ResultType = Ident->getExprType();
   }
   else if (auto P = dyn_cast<PropDecl>(FoundDecl)) {
      P = maybeInstantiateTemplateMember(LookupCtx, P);

      if (!checkImplicitSelf(*this, P, Ident))
         return ExprError();

      if (!P->isStatic() && Ident->isStaticLookup()) {
         auto Rec = P->getRecord();
         diagnose(Ident, err_non_static_member_accessed_statically,
                  Ident->getSourceLoc(), /*property*/ 1, P->getName(),
                  Rec->getSpecifierForDiagnostic(), Rec->getName());

         return ExprError();
      }

      auto Res =  HandlePropAccess(Ident, P);
      if (!Res)
         return Res;

      ResultType = Ident->getExprType();
   }
   else if (auto AT = dyn_cast<AssociatedTypeDecl>(FoundDecl)) {
      AT = maybeInstantiateTemplateMember(LookupCtx, AT);

      Ident->setKind(IdentifierKind::AssociatedType);
      Ident->setNamedDecl(AT);

      if (QC.PrepareDeclInterface(AT)) {
         return ExprError();
      }

      if (AssociatedTypeSubst) {
         auto SubstAT = LookupSingle<AssociatedTypeDecl>(*AssociatedTypeSubst,
                                                         AT->getDeclName());

         if (SubstAT)
            AT = SubstAT;
      }

      if (ReferencedATs) {
         if (!Ident->getParentExpr() || isa<SelfExpr>(Ident->getParentExpr())) {
            ReferencedATs->push_back(AT);
         }
      }

      if (!AT->isImplementation()) {
         Ident->setContainsAssociatedType(true);
      }
      if (AT->isTemplateOrInTemplate()) {
         Ident->setContainsAssociatedType(true);
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     AT, NameSpec));

      ResultType = Context.getMetaType(Context.getAssociatedType(AT));
   }
   else if (auto Param = dyn_cast<TemplateParamDecl>(FoundDecl)) {
      if (Param->isTypeName()) {
         ResultType = Context.getMetaType(Context.getTemplateArgType(Param));
      }
      else {
         ResultType = Param->getValueType();
      }

      if (Param->isVariadic()) {
         Ident->setContainsUnexpandedParameterPack(true);
      }

      Ident->setNameSpec(NestedNameSpecifier::Create(Context.getDeclNameTable(),
                                                     Param, NameSpec));

      Ident->setTemplateParam(Param);
      Ident->setKind(IdentifierKind::TemplateParam);

      Ident->setContainsGenericParam(true);
      Ident->setIsTypeDependent(ResultType->isDependentType()
                                  || Param->isUnbounded());
   }
   else {
      llvm_unreachable("unhandled named decl kind");
   }

   Ident->setExprType(ResultType);

   if (ResultType->isDependentType())
      Ident->setIsTypeDependent(true);

   if (!UsedTemplateParams && ArgExpr) {
      diagnose(ArgExpr, err_generic_error, "unused template argument list",
               ArgExpr->getAngleRange());
   }

   if (ParentType) {
      return UnwrapExistential(ParentType, Ident);
   }

   return Ident;
}

ExprResult SemaPass::visitBuiltinIdentExpr(BuiltinIdentExpr *Ident)
{
   cdot::Type *builtinType;

   switch (Ident->getIdentifierKind()) {
   default: llvm_unreachable("bad builtin ident!");
   case BuiltinIdentifier::defaultValue:
      if (Ident->getContextualType()) {
         assert(hasDefaultValue(Ident->getContextualType()));
         Ident->setExprType(Ident->getContextualType());
      }

      assert(Ident->getExprType());
      return Ident;
   case BuiltinIdentifier::NULLPTR: {
      if (Ident->getContextualType().isNull()) {
         if (Ident->isDependent()) {
            Ident->setExprType(UnknownAnyTy);
            return Ident;
         }

         diagnose(Ident, err_requires_contextual_type, Ident->getSourceLoc(),
                  1 /*__nullptr*/);

         return ExprError();
      }

      if (Ident->getContextualType()->isRefcounted()
          || Ident->getContextualType()->isThinFunctionTy()
          || Ident->getContextualType()->isPointerType())
         builtinType = *Ident->getContextualType();
      else
         builtinType = Ident->getContextualType()->getPointerTo(Context);

      break;
   }
   case BuiltinIdentifier::FUNC:
   case BuiltinIdentifier::MANGLED_FUNC: {
      auto C = getFuncScope()->getCallableDecl();
      llvm::StringRef str = Ident->getIdentifierKind()
                               == BuiltinIdentifier ::FUNC
                            ? C->getName()
                            : ILGen->getFunc(C)->getName();

      return StringLiteral::Create(Context, Ident->getSourceRange(), str);
   }
   case BuiltinIdentifier::FLOAT_QNAN:
   case BuiltinIdentifier::FLOAT_SNAN:
      builtinType = Context.getFloatTy();
      break;
   case BuiltinIdentifier::DOUBLE_QNAN:
   case BuiltinIdentifier::DOUBLE_SNAN:
      builtinType = Context.getDoubleTy();
      break;
   case BuiltinIdentifier::__ctfe:
      builtinType = Context.getBoolTy();
      break;
   }

   Ident->setExprType(builtinType);
   return Ident;
}

template<class ExprTy>
static ExprResult checkIfSelfCapture(SemaPass &SP, ExprTy *E)
{
   LambdaExpr *LE = nullptr;
   for (auto S = SP.getCurrentScope(); S; S = S->getEnclosingScope()) {
      switch (S->getTypeID()) {
      case Scope::LambdaScopeID:
         LE = cast<LambdaScope>(S)->getLambdaExpr();
         break;
      case Scope::FunctionScopeID:
      case Scope::MethodScopeID: {
         auto Fn = cast<FunctionScope>(S)->getCallableDecl();
         if (!isa<MethodDecl>(Fn)
               || (cast<MethodDecl>(Fn)->isStatic() && !isa<InitDecl>(Fn))) {
            SP.diagnose(E, err_self_outside_method, E->getSourceLoc(),
                        isa<SuperExpr>(E));

            return ExprError();
         }

         auto R = Fn->getRecord();
         if (R->isInUnboundedTemplate()) {
            E->setIsTypeDependent(true);
         }

         QualType SelfTy = R->getType();
         if (Fn->hasMutableSelf() || R->isClass()) {
            SelfTy = SP.getContext().getMutableReferenceType(SelfTy);
         }
         else {
            SelfTy = SP.getContext().getReferenceType(SelfTy);
         }

         E->setExprType(SelfTy);

         if (!isa<InitDecl>(Fn) && !isa<DeinitDecl>(Fn)) {
            auto Arg = Fn->getArgs().front();
            E->setSelfArg(Arg);

            if (LE) {
               Arg->setCaptured(true);
               E->setCaptureIndex(LE->addCapture(SP.getContext(), Arg));
            }
         }

         return E;
      }
      default:
         break;
      }
   }

   SP.diagnose(E, err_self_outside_method, E->getSourceLoc(),
               isa<SuperExpr>(E));

   return ExprError();
}

ExprResult SemaPass::visitSuperExpr(SuperExpr *Expr)
{
   if (!checkIfSelfCapture(*this, Expr))
      return ExprError();

   auto R = getCurrentRecordCtx();
   auto currentCl = dyn_cast<ClassDecl>(R);
   if (!currentCl || currentCl->getParentClass() == nullptr) {
      diagnose(Expr, err_super_without_base, Expr->getSourceLoc(),
               getCurrentRecordCtx()->getName());

      return ExprError();
   }

   QualType SelfTy = Context.getRecordType(currentCl->getParentClass());
   if (Expr->getExprType()->isMutableReferenceType())
      SelfTy = Context.getMutableReferenceType(SelfTy);
   else
      SelfTy = Context.getReferenceType(SelfTy);

   Expr->setExprType(SelfTy);
   return Expr;
}

ExprResult SemaPass::visitSelfExpr(SelfExpr *Expr)
{
   if (Expr->isUppercase()) {
      auto *R = getCurrentRecordCtx();
      if (!R) {
         diagnose(Expr, err_self_outside_method, Expr->getSourceLoc(), 2);
         return ExprError();
      }

      if (QC.PrepareNameLookup(R)) {
         return ExprError();
      }

      AssociatedTypeDecl *SelfDecl;
      if (QC.GetAssociatedType(SelfDecl, R, getIdentifier("Self"),
                               R->getExtensions())) {
         return ExprError();
      }

      assert(SelfDecl && "no Self associated type!");
      Expr->setExprType(
         Context.getMetaType(Context.getAssociatedType(SelfDecl)));

      return Expr;
   }

   if (!checkIfSelfCapture(*this, Expr))
      return ExprError();

   return Expr;
}

namespace {

class PlaceholderExprBuilder {
   std::vector<std::unique_ptr<BuiltinExpr>> Exprs;

public:
   PlaceholderExprBuilder() = default;

   Expression *CreateExpr(QualType Ty)
   {
      auto NewExpr = std::make_unique<BuiltinExpr>(BuiltinExpr::CreateTemp(Ty));
      auto Ptr = NewExpr.get();

      Exprs.emplace_back(move(NewExpr));
      return Ptr;
   }
};

} // anonymous namespace

CallableDecl*
SemaPass::checkFunctionReference(Expression *E,
                                 DeclarationName funcName,
                                 MultiLevelLookupResult &MultiLevelResult,
                                 ArrayRef<Expression*> templateArgs) {
   CandidateSet CandSet;
   for (auto &Lookup : MultiLevelResult) {
      for (auto *ND : Lookup) {
         if (auto *C = dyn_cast<CallableDecl>(ND))
            CandSet.addCandidate(C);
      }
   }

   if (CandSet.Candidates.empty()) {
      diagnose(E, err_func_not_found, 0, funcName, E->getSourceRange());
      return nullptr;
   }

   if (CandSet.Candidates.size() == 1)
      return CandSet.Candidates.front().Func;

   auto CtxTy = E->getContextualType();
   if (!CtxTy || !CtxTy->isFunctionType()) {
      diagnose(E, err_ambiguous_call, 0, funcName, E->getSourceRange());
      return nullptr;
   }

   auto FnTy = CtxTy->asFunctionType();
   PlaceholderExprBuilder Builder;

   SmallVector<Expression*, 4> ArgExprs;
   for (auto &ParamTy : FnTy->getParamTypes()) {
      ArgExprs.push_back(Builder.CreateExpr(ParamTy));
   }

   lookupFunction(CandSet, funcName, ArgExprs, templateArgs, {}, E, true);
   if (!CandSet) {
      assert(CandSet.Status == CandidateSet::Ambiguous);
      CandSet.diagnoseAmbiguousCandidates(*this, funcName, ArgExprs,
                                          templateArgs, E);

      E->setIsInvalid(true);
      return nullptr;
   }

   return CandSet.getBestMatch().Func;
}

SemaPass::AliasResult
SemaPass::checkAlias(MultiLevelLookupResult &MultiLevelResult,
                     llvm::ArrayRef<Expression *> templateArgs,
                     Expression *E) {
   size_t bestMatch    = 0;
   unsigned BestMatchDistance = unsigned(-1);
   bool typeDependent  = false;
   bool valueDependent = false;
   AliasDecl *match    = nullptr;

   SmallPtrSet<AliasDecl*, 8> Candidates;
   CandidateSet CandSet;

   unsigned Distance = 0;
   for (auto &Lookup : MultiLevelResult) {
      for (auto *ND : Lookup) {
         AliasDecl *alias = dyn_cast<AliasDecl>(ND);
         if (!alias || alias->getConstraints().size() < bestMatch)
            continue;
         if (!Candidates.insert(alias).second)
            continue;

         auto *Cand = CandSet.addCandidate(alias);
         if (!Cand)
            continue;

         if (!templateArgs.empty() || alias->isTemplate()) {
            if (QC.TypecheckDecl(alias)) {
               continue;
            }

            Cand->InnerTemplateArgs = TemplateArgList(*this, alias, templateArgs);
            if (auto Ctx = E->getContextualType()) {
               Cand->InnerTemplateArgs.inferFromType(Ctx, alias->getType());
            }

            if (Cand->InnerTemplateArgs.isStillDependent()) {
               Cand->setIsInvalid();
               typeDependent = true;
               continue;
            }

            auto comp = Cand->InnerTemplateArgs.checkCompatibility();
            if (!comp) {
               Cand->setTemplateArgListFailure(comp);
               continue;
            }

            auto FinalList = FinalTemplateArgumentList::Create(
               Context, Cand->InnerTemplateArgs);

            auto Inst = Instantiator.InstantiateAlias(alias, E->getSourceLoc(),
                                                      FinalList);

            if (Inst.hasValue()) {
               alias = Inst.getValue();
               Cand->Alias = alias;
            }
            else {
               Cand->setIsInvalid();
               continue;
            }
         }

         if (checkAlias(alias, *Cand)) {
            if (match && BestMatchDistance == Distance) {
               CandSet.Status = CandidateSet::Ambiguous;
            }
            else if (Distance < BestMatchDistance) {
               CandSet.Status = CandidateSet::Success;
               match = alias;
               BestMatchDistance = Distance;
            }
         }
      }

      ++Distance;
   }

   if (!match) {
      if (typeDependent || valueDependent)
         return AliasResult(typeDependent, valueDependent);

      return AliasResult(move(CandSet));
   }

   if (CandSet.Status != CandidateSet::Success)
      return AliasResult(move(CandSet));

   visitScoped(match);

   if (match->isDependent())
      return AliasResult(match->isTypeDependent(), match->isValueDependent());

   return AliasResult(match);
}

bool SemaPass::checkAlias(AliasDecl *Alias, CandidateSet::Candidate &Cand)
{
   DeclScopeRAII declScopeRAII(*this, cast<DeclContext>(Alias));
   ScopeResetRAII scopeResetRAII(*this);

   for (auto &C : Alias->getConstraints()) {
      auto res = evaluateAsBool(Alias, C);
      if (res.hadError()) {
         Cand.setIsInvalid();
         Alias->setIsInvalid(true);
         return false;
      }

      if (!cast<il::ConstantInt>(res.getValue())->getBoolValue()) {
         Cand.setHasFailedConstraint(C);
         return false;
      }
   }

   return true;
}

IdentifierRefExpr *SemaPass::wouldBeValidIdentifier(SourceLocation Loc,
                                                    IdentifierInfo *maybeIdent,
                                                    bool LHSOfAssignment) {
   auto *expr = new(Context) IdentifierRefExpr(Loc, maybeIdent);
   expr->setIsLHSOfAssignment(LHSOfAssignment);
   expr->setIssueDiag(false);

   (void)visitExpr(expr);
   if (!expr->foundResult())
      return nullptr;

   return expr;
}

template<class T>
static NestedNameSpecifier *checkNamespaceRefCommon(SemaPass &SP, T *Expr)
{
   Expression *ParentExpr = Expr->getParentExpr();
   if (!ParentExpr)
      return nullptr;

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = SP.visitExpr(ParentExpr);
   if (!SemaRes) {
      Expr->copyStatusFlags(ParentExpr);
      return nullptr;
   }

   ParentExpr = SemaRes.get();
   Expr->setParentExpr(ParentExpr);

   DeclContext *Ctx = nullptr;
   NestedNameSpecifier *Res = nullptr;

   if (auto *TAExpr = dyn_cast<TemplateArgListExpr>(ParentExpr)) {
      ParentExpr = TAExpr->getParentExpr();
   }

   if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      if (Ident->getKind() == IdentifierKind::Namespace) {
         Ctx = Ident->getNamespaceDecl();
      }
      else if (Ident->getKind() == IdentifierKind::Import) {
         Ctx = Ident->getImport();
      }
      else if (Ident->getKind() == IdentifierKind::Module) {
         Ctx = Ident->getModule();
      }

      Res = Ident->getNameSpec();
   }

   if (!Ctx) {
      Expr->copyStatusFlags(ParentExpr);
      return Res;
   }

   if (auto Ident = dyn_cast<IdentifierRefExpr>(Expr)) {
      Ident->setDeclCtx(Ctx);
   }
   else if (auto Call = dyn_cast<CallExpr>(Expr)) {
      Call->setContext(Ctx);
   }
   else {
      Expr->copyStatusFlags(ParentExpr);
   }

   return Res;
}

NestedNameSpecifier *SemaPass::checkNamespaceRef(Expression *Expr)
{
   return checkNamespaceRefCommon(*this, Expr);
}

ExprResult SemaPass::checkNamespaceRef(MacroExpansionExpr *Expr)
{
   Expression *ParentExpr = Expr->getParentExpr();
   if (!ParentExpr)
      return Expr;

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      Expr->copyStatusFlags(ParentExpr);
      return ExprError();
   }

   ParentExpr = SemaRes.get();
   Expr->setParentExpr(ParentExpr);
   Expr->copyStatusFlags(ParentExpr);

   return Expr;
}

StmtResult SemaPass::checkNamespaceRef(MacroExpansionStmt *Stmt)
{
   Expression *ParentExpr = Stmt->getParentExpr();
   if (!ParentExpr)
      return Stmt;

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      Stmt->copyStatusFlags(ParentExpr);
      return StmtError();
   }

   ParentExpr = SemaRes.get();
   Stmt->setParentExpr(ParentExpr);
   Stmt->copyStatusFlags(ParentExpr);

   return Stmt;
}

DeclResult SemaPass::checkNamespaceRef(MacroExpansionDecl *D)
{
   Expression *ParentExpr = D->getParentExpr();
   if (!ParentExpr)
      return D;

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      D->copyStatusFlags(ParentExpr);
      return DeclError();
   }

   ParentExpr = SemaRes.get();
   D->setParentExpr(ParentExpr);
   D->copyStatusFlags(ParentExpr);

   return D;
}

ExprResult SemaPass::HandleStaticTypeMember(IdentifierRefExpr *Expr, QualType Ty)
{
   if (Expr->getIdentInfo()->isStr("typeof")) {
      auto TI = getTypeInfoDecl();
      if (!TI) {
         diagnose(Expr, err_no_builtin_decl, Expr->getSourceRange(), 9);
         return ExprError();
      }

      Expr->setExprType(Context.getReferenceType(Context.getRecordType(TI)));
      Expr->setKind(IdentifierKind::TypeOf);

      return Expr;
   }

   if (Ty->isDependentType()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   }

   diagnose(Expr, err_access_member_on_type, Expr->getSourceLoc(), Ty);
   return ExprError();
}

ExprResult SemaPass::HandleBuiltinTypeMember(IdentifierRefExpr *Expr,
                                             QualType Ty) {
   if (auto ArrTy = Ty->asArrayType()) {
      if (ArrTy->isDependentSizeArrayType()) {
         Expr->setIsValueDependent(true);
         return ExprError();
      }

      if (ArrTy->isDependentType()) {
         Expr->setIsTypeDependent(true);
         return ExprError();
      }

      if (Expr->getIdentInfo()->isStr("size")) {
         auto UIntTy = Context.getUIntTy();
         llvm::APInt API(UIntTy->getBitwidth(), ArrTy->getNumElements(), false);

         auto Lit = IntegerLiteral::Create(Context, Expr->getSourceRange(),
                                           Context.getUIntTy(),
                                           llvm::APSInt(std::move(API), true));

         (void)visitIntegerLiteral(Lit);

         return Lit;
      }
   }

   if (Ty->isDependentType()) {
      Expr->setExprType(UnknownAnyTy);
      Expr->setIsTypeDependent(true);
      Expr->setFoundResult(true);

      return Expr;
   }

   diagnose(Expr, err_access_member_on_type, Ty, Expr->getSourceRange());
   return ExprError();
}

ExprResult SemaPass::HandleEnumCase(IdentifierRefExpr *node, EnumDecl *E)
{
   if (E->hasCase(node->getIdentInfo())) {
      node->setSemanticallyChecked(true);

      auto *Case = LookupSingle<EnumCaseDecl>(*E, node->getDeclName());
      auto enumCaseExpr = new(getContext()) EnumCaseExpr(node->getSourceLoc(),
                                                         Case);

      enumCaseExpr->setContextualType(node->getContextualType());
      return visitEnumCaseExpr(enumCaseExpr);
   }

   diagnose(node, err_enum_case_not_found, node->getSourceLoc(),
            node->getIdent(), 0);

   return ExprError();
}

ExprResult SemaPass::HandleFieldAccess(IdentifierRefExpr *Ident,
                                       MutableArrayRef<Expression*> TemplateArgs,
                                       FieldDecl *F) {
   if (QC.PrepareDeclInterface(F))
      return ExprError();

   if (!isAccessible(F) && F->getAccessor()) {
      return HandlePropAccess(Ident, F->getAccessor());
   }

   if (F->isStatic()) {
      Ident->setKind(IdentifierKind::StaticField);
      Ident->setStaticFieldDecl(F);
   }
   else {
      Ident->setKind(IdentifierKind::Field);
      Ident->setFieldDecl(F);
   }

   if (F->isVariadic()) {
      if (TemplateArgs.empty()) {
         Ident->setContainsUnexpandedParameterPack(true);
         Ident->setIsTypeDependent(true);
         Ident->setExprType(F->getType());

         return Ident;
      }

      if (TemplateArgs.size() != 1) {
         diagnose(Ident, err_variadic_field_single_index,
                  Ident->getSourceRange());

         TemplateArgs = TemplateArgs.take_front(1);
      }

      auto ArgRes = visitExpr(Ident, TemplateArgs.front());
      if (!ArgRes)
         return ExprError();

      if (ArgRes.get()->isDependent()) {
         Ident->setExprType(UnknownAnyTy);
         return Ident;
      }

      auto StaticRes = evalStaticExpr(Ident, cast<StaticExpr>(ArgRes.get()));
      if (!StaticRes)
         return ExprError();

      auto V = StaticRes.getValue();
      if (!isa<il::ConstantInt>(V)) {
         diagnose(Ident, err_variadic_field_index_integral,
                  ArgRes.get()->getSourceRange());

         return ExprError();
      }

      TemplateArgs.front() = ArgRes.get();
   }

   QualType ty(F->getType());
   if (ty->isDependentType())
      Ident->setIsTypeDependent(true);

   if (QC.CheckAccessibility(&getDeclContext(), F)) {
      Ident->setIsInvalid(true);
   }

   if (F->isStatic()) {
      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setStaticFieldDecl(F);
   }
   else {
      Ident->setKind(IdentifierKind::Field);
      Ident->setFieldDecl(F);
   }

   bool ParentIsConst = false;
   bool BeingInitialized = false;

   if (!F->isStatic()) {
      if (Ident->getParentExpr()->getExprType()->isPointerType()) {
         ParentIsConst = Ident->getParentExpr()->getExprType()
                              ->isNonMutablePointerType();
      }
      else {
         ParentIsConst = !Ident->getParentExpr()->getExprType()
                               ->isMutableReferenceType();
      }

      // field access in non mutating methods of a class returns a mutable
      // reference
      if (auto Self = dyn_cast<SelfExpr>(Ident->getParentExpr())) {
         ParentIsConst &= !Self->getExprType()->stripReference()->getRecord()
                               ->isClass();
      }

      // if we're in this records initializer, return a mutable reference for
      // now. Later IL passes will ensure that each field is initialized
      // exactly once
      BeingInitialized = false;
      InitDecl *Init = dyn_cast_or_null<InitDecl>(getCurrentFun());
      if (Init) {
         BeingInitialized = Init->getRecord() == F->getRecord();
      }

      Ident->setParentExpr(castToRValue(Ident->getParentExpr()));
   }

   // if the expression that we're accessing the field on or the field itself
   // is constant, return a constant reference
   if ((F->isConst() && !BeingInitialized) || ParentIsConst) {
      Ident->setExprType(Context.getReferenceType(ty));
   }
   else {
      Ident->setExprType(Context.getMutableReferenceType(ty));
   }

   return Ident;
}

ExprResult SemaPass::HandlePropAccess(IdentifierRefExpr *Ident,
                                      PropDecl *P) {
   assert(P && "shouldn't be called otherwise");
   if (QC.PrepareDeclInterface(P))
      return ExprError();

   QualType ty;
   QualType SelfType;
   bool MutableSelf = false;

   if (Ident->isLHSOfAssignment()) {
      if (!P->hasSetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*setter*/ 1);

         Ident->setExprType(Context.getVoidType());
         return Ident;
      }

      if (QC.CheckAccessibility(&getDeclContext(), P->getSetterMethod())) {
         Ident->setIsInvalid(true);
      }

      MutableSelf = P->getSetterMethod()->hasMutableSelf();
      SelfType = P->getSetterMethod()->getSelfType();

      if (!P->isSynthesized())
         maybeInstantiateMemberFunction(P->getSetterMethod(), Ident);

      // this path should only be taken when resolving an expression sequence
      // so this type is only used to get the correct `=` overloads; the
      // actual type of the resolved expression will be `Void`
      ty = Context.getMutableReferenceType(P->getType());
   }
   else {
      if (!P->hasGetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*getter*/ 0);

         Ident->setExprType(P->getType());
         return Ident;
      }

      if (QC.CheckAccessibility(&getDeclContext(), P->getGetterMethod())) {
         Ident->setIsInvalid(true);
      }

      MutableSelf = P->getGetterMethod()->hasMutableSelf();
      SelfType = P->getGetterMethod()->getSelfType();

      if (!P->isSynthesized())
         maybeInstantiateMemberFunction(P->getGetterMethod(), Ident);

      ty = P->getType();
   }

   // if the expression that we're accessing the property on is constant,
   // error if we're trying to use a setter
   bool ParentIsConst = !Ident->getParentExpr()->getExprType()
                              ->isMutableReferenceType();

   // field access in non mutating methods of a class returns a mutable
   // reference
   if (auto Self = dyn_cast<SelfExpr>(Ident->getParentExpr())) {
      ParentIsConst &= !Self->getExprType()->stripReference()->getRecord()
                            ->isClass();
   }

   if (!SelfType->isDependentType() && !ty->isDependentType()) {
      if (MutableSelf && ParentIsConst) {
         diagnose(Ident, err_mutating_accessor_on_const,
                  Ident->isLHSOfAssignment(),
                  Ident->getParentExpr()->getSourceRange(),
                  Ident->getSourceLoc());
      }
      else {
         Ident->setParentExpr(implicitCastIfNecessary(Ident->getParentExpr(),
                                                      SelfType));
      }
   }
   else {
      Ident->setIsTypeDependent(true);
   }

   Ident->setAccessor(P);
   Ident->setExprType(ty);

   return Ident;
}

ExprResult SemaPass::visitTupleMemberExpr(TupleMemberExpr *Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = visitExpr(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   if (ParentExpr->getExprType()->stripReference()->isUnknownAnyType()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   }

   TupleType *tup = ParentExpr->getExprType()->stripReference()->asTupleType();
   if (!tup) {
      diagnose(Expr, err_not_tuple, Expr->getSourceLoc(),
               ParentExpr->getExprType());

      return ExprError();
   }

   if (tup->getArity() <= Expr->getIndex()) {
      diagnose(Expr, diag::err_tuple_arity, Expr->getSourceLoc(),
               Expr->getIndex(), tup->getArity());

      return ExprError();
   }

   // if the expression that we're accessing the property on is constant,
   // return an immutable reference
   bool ParentIsConst = !Expr->getParentExpr()->getExprType()
                             ->isMutableReferenceType();

   // field access in non mutating methods of a class returns a mutable
   // reference
   if (auto Self = dyn_cast<SelfExpr>(Expr->getParentExpr())) {
      ParentIsConst &= !Self->getExprType()->stripReference()->getRecord()
                            ->isClass();
   }

   QualType contained(tup->getContainedType(Expr->getIndex()));
   if (ParentIsConst) {
      Expr->setExprType(Context.getReferenceType(contained));
   }
   else {
      Expr->setExprType(Context.getMutableReferenceType(contained));
   }

   Expr->setParentExpr(castToRValue(Expr->getParentExpr()));
   return Expr;
}

ExprResult SemaPass::visitEnumCaseExpr(EnumCaseExpr *Expr)
{
   EnumDecl *E = Expr->getEnum();
   if (!E) {
      auto ty = Expr->getContextualType();
      if (!ty) {
         diagnose(Expr, err_requires_contextual_type, Expr->getSourceLoc(),
                  1 /*__nullptr*/);
         return ExprError();
      }

      ty = ty->stripReference();

      if (!ty->isRecordType()) {
         diagnose(Expr, err_value_is_not_enum, Expr->getSourceLoc(), ty);
         return ExprError();
      }

      auto rec = ty->getRecord();
      if (!isa<EnumDecl>(rec)) {
         diagnose(Expr, err_record_is_not_enum, Expr->getSourceLoc(),
                  rec->getSpecifierForDiagnostic(), rec->getName());
         return ExprError();
      }

      E = cast<EnumDecl>(rec);
   }

   if (QC.PrepareDeclInterface(E)) {
      return ExprError();
   }

   if (!E->hasCase(Expr->getIdentInfo())) {
      diagnose(Expr, err_enum_case_not_found, Expr->getSourceLoc(),
               E->getName(), Expr->getIdent(), false);

      return ExprError();
   }

   auto &args = Expr->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Expr, arg);
         if (!result)
            continue;

         arg = result.get();
      }
   }

   if (E->isUnboundedTemplate()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(Context.getRecordType(E));

      return Expr;
   }

   auto CandSet = lookupCase(Expr->getIdentInfo(), E, Expr->getArgs(),
                             {}, {}, Expr);

   if (CandSet.isDependent()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(Context.getRecordType(E));

      return Expr;
   }

   if (!CandSet)
      return ExprError();

   auto &Cand = CandSet.getBestMatch();
   auto Case = cast<EnumCaseDecl>(Cand.Func);

   if (QC.CheckAccessibility(&getDeclContext(), Case)) {
      Expr->setIsInvalid(true);
   }

   Expr->getArgs().clear();
   Expr->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                          CandSet.ResolvedArgs.end());

   E = cast<EnumDecl>(Case->getRecord());

   Expr->setCase(Case);
   Expr->setExprType(QualType(Context.getRecordType(E)));

   return Expr;
}

ExprResult SemaPass::visitSubscriptExpr(SubscriptExpr *Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = visitExpr(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   QualType SubscriptedTy = ParentExpr->getExprType()->stripReference();
   if (SubscriptedTy->isRecordType()) {
      auto DeclName = Context.getDeclNameTable().getSubscriptName(
         DeclarationName::SubscriptKind::General);

      ASTVector<Expression*> Args(Context, Expr->getIndices());
      auto call = CallExpr::Create(Context, SourceLocation(),
                                   Expr->getSourceRange(),
                                   ParentExpr, move(Args), {},
                                   DeclName);

      call->setIsLHSOfAssignment(Expr->isLHSOfAssignment());

      return visitExpr(Expr, call);
   }

   for (auto &Idx : Expr->getIndices()) {
      if (!SubscriptedTy->isRecordType())
         Idx->setContextualType(Context.getIntTy());

      auto indexResult = visitExpr(Expr, Idx);
      if (!indexResult)
         return ExprError();

      Idx = indexResult.get();
   }

   if (SubscriptedTy->isDependentType()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   }

   if (!Expr->hasSingleIndex())
      diagnose(Expr, err_subscript_too_many_indices, Expr->getSourceLoc());

   QualType resultType;
   if (PointerType *Ptr = SubscriptedTy->asPointerType()) {
      resultType = Ptr->getPointeeType();
   }
   else if (ArrayType *Arr = SubscriptedTy->asArrayType()) {
      resultType = Arr->getElementType();
   }
   // give more accurate diagnostic if this subscript was likely meant to be
   // a templare argument list
   else if (SubscriptedTy->isMetaType() && SubscriptedTy->asMetaType()
                                                        ->getUnderlyingType()
                                                        ->isRecordType()) {
      auto R = SubscriptedTy->asMetaType()->getUnderlyingType()->getRecord();
      diagnose(Expr, err_not_a_template, R->getSpecifierForDiagnostic(),
               R->getDeclName(), Expr->getSourceRange());

      diagnose(note_declared_here, R->getSourceLoc());

      return ExprError();
   }
   else {
      diagnose(Expr, err_illegal_subscript, Expr->getSourceLoc(),
               SubscriptedTy);

      return ExprError();
   }

   if (!Expr->getIndices().empty()) {
      auto &idx = Expr->getIndices().front();
      auto IdxTy = idx->getExprType()->stripReference();

      // allow signed and unsigned subscript indices
      if (IdxTy->isIntegerType()) {
         auto IntBits = Context.getTargetInfo().getPointerSizeInBytes() * 8;
         idx = implicitCastIfNecessary(
            idx, Context.getIntegerTy(IntBits, IdxTy->isUnsigned()));
      }
      else {
         idx = implicitCastIfNecessary(idx, Context.getUIntTy());
      }
   }

   bool IsMutablePtr = ParentExpr->getExprType()->stripReference()
                                 ->isMutablePointerType();

   bool ParentIsConst = !Expr->getParentExpr()->getExprType()
                             ->isMutableReferenceType();

   Expr->setParentExpr(castToRValue(Expr->getParentExpr()));

   // if the expression that we're subscripting is constant, return a
   // constant reference
   if (ParentIsConst && !IsMutablePtr) {
      Expr->setExprType(Context.getReferenceType(resultType));
   }
   else {
      Expr->setExprType(Context.getMutableReferenceType(resultType));
   }

   return Expr;
}

ExprResult SemaPass::visitTemplateArgListExpr(TemplateArgListExpr *Expr)
{
   bool Dependent = false;
   for (auto &TA : Expr->getExprs()) {
      auto res = visitExpr(Expr, TA);
      if (!res)
         return ExprError();

      TA = res.get();
      Dependent |= TA->isUnknownAny();
   }

   Expression *ParentExpr = Expr->getParentExpr();
   if (ParentExpr->isSemanticallyChecked()) {
      Expr->setExprType(ParentExpr->getExprType());
      return Expr;
   }

   ExprResult Res;
   switch (ParentExpr->getTypeID()) {
   case Expression::IdentifierRefExprID:
      Res = visitIdentifierRefExpr(cast<IdentifierRefExpr>(ParentExpr), Expr);
      break;
   case Expression::CallExprID:
      Res = visitCallExpr(cast<CallExpr>(ParentExpr), Expr);
      break;
   default:
      diagnose(Expr, err_generic_error, "template arguments not allowed here");
      break;
   }

   if (!Res) {
      Expr->copyStatusFlags(ParentExpr);
      ParentExpr->setSemanticallyChecked(true);

      return ExprError();
   }

   ParentExpr = Res.get();

   Expr->copyStatusFlags(ParentExpr);
   Expr->setParentExpr(ParentExpr);
   Expr->setExprType(ParentExpr->getExprType());

   ParentExpr->copyStatusFlags(Expr);
   ParentExpr->setSemanticallyChecked(true);

   return Expr;
}

void SemaPass::diagnoseMemberNotFound(ast::DeclContext *Ctx,
                                      StmtOrDecl Subject,
                                      DeclarationName memberName,
                                      diag::MessageKind msg,
                                      SourceRange SR) {
   if (!SR)
      SR = Subject.getSourceRange();

   if (!Ctx) {
      diagnose(Subject, msg, SR, memberName);
   }
   else {
      while (Ctx) {
         switch (Ctx->getDeclKind()) {
         case Decl::StructDeclID: case Decl::ClassDeclID:
         case Decl::EnumDeclID: case Decl::UnionDeclID:
         case Decl::ProtocolDeclID: case Decl::NamespaceDeclID: {
            auto ND = cast<NamedDecl>(Ctx);
            diagnose(Subject, err_member_not_found, SR,
                     ND->getSpecifierForDiagnostic(), ND->getDeclName(),
                     memberName);

            return;
         }
         case Decl::ModuleDeclID:
            diagnose(Subject, msg, SR, memberName);
            break;
         case Decl::ExtensionDeclID: {
            auto *Rec = Ctx->lookThroughExtension();
            if (Rec == Ctx) {
               diagnose(Subject, err_undeclared_identifer, memberName, SR);
               return;
            }

            Ctx = Rec;
            continue;
         }
         default:
            break;
         }

         Ctx = Ctx->getParentCtx();
      }
   }
}

void SemaPass::diagnoseTemplateArgErrors(NamedDecl *Template,
                                         Statement *ErrorStmt,
                                         TemplateArgList &list,
                                         llvm::ArrayRef<Expression*>
                                                                   OriginalArgs,
                                         TemplateArgListResult &Cand) {
   size_t selector;
   if (auto R = dyn_cast<RecordDecl>(Template)) {
      selector = R->getSpecifierForDiagnostic();
   }
   else {
      assert(isa<AliasDecl>(Template) && "invalid template decl");
      selector = 5;
   }

   diagnose(ErrorStmt, err_incompatible_template_args,
            ErrorStmt->getSourceRange(),
            selector, Template->getName());

   switch (Cand.ResultKind) {
   case sema::TemplateArgListResultKind::TLR_CouldNotInfer: {
      auto missingParam = reinterpret_cast<TemplateParamDecl*>(Cand.Data1);

      diagnose(note_could_not_infer_template_arg,
               missingParam->getDeclName(),
               missingParam->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_TooManyTemplateArgs: {
      auto neededSize = Template->getTemplateParams().size();
      auto givenSize  = OriginalArgs.size();

      diagnose(ErrorStmt, note_too_many_template_args,
               neededSize, givenSize, Template->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgKind: {
      unsigned diagSelect = unsigned(Cand.Data1);
      unsigned select1    = diagSelect & 0x3u;
      unsigned select2    = (diagSelect >> 2u) & 0x3u;

      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);
      diagnose(ErrorStmt, note_template_arg_kind_mismatch, select1,
               select2, 0,
               list.getArgForParam(Param)->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here,
               Param->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgVal: {
      auto givenTy = reinterpret_cast<Type*>(Cand.Data1);
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      diagnose(ErrorStmt, note_template_arg_type_mismatch, givenTy,
               0, Param->getCovariance());

      diagnose(ErrorStmt, note_template_parameter_here,
               Param->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_ConflictingInferredArg: {
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      if (Param->isTypeName()) {
         auto conflictingTy = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));

         auto templateArg = list.getArgForParam(Param);
         assert(templateArg && "bad diagnostic data");

         string name = Param->getName();
         if (templateArg->isVariadic()) {
            name += "[";
            name += std::to_string(templateArg->getVariadicArgs().size()
                                   - 1);
            name += "]";

            templateArg = &templateArg->getVariadicArgs().back();
         }

         diagnose(ErrorStmt, note_inferred_template_arg_conflict,
                  0 /*types*/, templateArg->getType(), conflictingTy,
                     name, templateArg->getLoc());

         diagnose(ErrorStmt, note_template_parameter_here,
                  Param->getSourceLoc());
      }
      else {
         llvm_unreachable("TODO");
      }

      break;
   }
   case sema::TemplateArgListResultKind::TLR_CovarianceError: {
      QualType Given = QualType::getFromOpaquePtr((void*)Cand.Data1);
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      auto templateArg = list.getArgForParam(Param);
      assert(templateArg && "bad diagnostic data");

      auto *CovarRec = Param->getCovariance()->getRecord();
      diagnose(ErrorStmt, note_template_arg_covariance,
               isa<ClassDecl>(CovarRec), CovarRec->getDeclName(),
               Param->getDeclName(), Given, templateArg->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here,
               Param->getSourceLoc());

      break;
   }
   default:
      break;
   }
}

static bool IsTestable(NamedDecl *ND)
{
   if (ND->hasAttribute<TestableAttr>())
      return true;

   auto *Ctx = ND->getNonTransparentDeclContext();
   if (auto *NamedCtx = dyn_cast<NamedDecl>(Ctx))
      return IsTestable(NamedCtx);

   return false;
}

bool SemaPass::isAccessible(NamedDecl *ND)
{
   if (Bits.InUnitTest && IsTestable(ND))
      return true;

   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return true;
   case AccessSpecifier::Private: {
      // only visible within the immediate context the symbol was defined in
      auto *Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == getDeclContext().getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && Curr->lookThroughExtension() == Ctx) {
            return true;
         }
         if (Curr == Ctx) {
            return true;
         }
      }

      return false;
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (Curr->lookThroughExtension() == Ctx->lookThroughExtension())
            return true;

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return true;
      }

      return false;
   }
   case AccessSpecifier::FilePrivate: {
      // visible within the file it was declared
      auto &FileMgr = compilationUnit->getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(cast<Decl>(getDeclContext())
                                                   .getSourceLoc());

      return DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID);
   }
   case AccessSpecifier::Internal: {
      return getDeclContext().getDeclModule()->getBaseModule()
         == ND->getModule()->getBaseModule();
   }
   }
}

void SemaPass::checkAccessibility(NamedDecl *ND, StmtOrDecl SOD)
{
   if (Bits.InUnitTest && IsTestable(ND))
      return;

   addDependency(ND);
   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return;
   case AccessSpecifier::Private: {
      // only visible within the immediate context the symbol was defined in
      auto *Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == getDeclContext().getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && Curr->lookThroughExtension() == Ctx) {
            return;
         }
         if (Curr == Ctx) {
            return;
         }
      }

      // declaration is not accessible here
      diagnose(SOD, err_private_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (Curr->lookThroughExtension() == Ctx->lookThroughExtension())
            return;

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return;
      }

      // declaration is not accessible here
      diagnose(SOD, err_protected_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), C->getDeclName(), SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::FilePrivate: {
      // visible within the file it was declared
      auto &FileMgr = compilationUnit->getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(SOD.getSourceLoc());

      if (DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID))
         return;

      // declaration is not accessible here
      diagnose(SOD, err_fileprivate_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), FileMgr.getFileName(DeclID),
               SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::Internal: {
      if (getDeclContext().getDeclModule()->getBaseModule()
             == ND->getModule()->getBaseModule()) {
         return;
      }

      diagnose(SOD, err_internal_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(),
               ND->getModule()->getBaseModule()->getDeclName(),
               SOD.getSourceRange());

      break;
   }
   }

   diagnose(note_access_spec_here, /*implicitly*/ !ND->getAccessRange(),
            (int)AccessSpec, ND->getAccessRange(), ND->getSourceLoc());
}

unsigned SemaPass::getSerializationFile(Decl *D)
{
   if (D->instantiatedFromProtocolDefaultImpl()) {
      D = D->getModule();
   }

   if (auto *ND = dyn_cast<NamedDecl>(D)) {
      while (auto *InstScope = getInstantiationScope(ND)) {
         ND = InstScope;
      }

      D = ND;
   }

   return getCompilationUnit().getFileMgr()
                              .getLexicalSourceId(D->getSourceLoc());
}

void SemaPass::addDependency(Decl *ReferencedDecl)
{
   addDependency(getCurrentDecl(), ReferencedDecl);
}

void SemaPass::addDependency(NamedDecl *D, Decl *ReferencedDecl)
{
   auto &CI = getCompilationUnit();
   auto *IncMgr = CI.getIncMgr();
   if (!IncMgr)
      return;

   IncMgr->addDependency(getSerializationFile(D),
                         getSerializationFile(ReferencedDecl));
}

} // namespace ast
} // namespace cdot