//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Transform.h"
#include "AST/Type.h"
#include "ILGen/ILGenPass.h"
#include "IL/Function.h"
#include "TemplateInstantiator.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace ast {

void SemaPass::diagnoseCircularlyDependentGlobalVariables(Expression *Expr,
                                                          NamedDecl *globalVar){
   // find the global variable currently being evaluated
   NamedDecl *dependentDecl = nullptr;

//   for (auto curr = getParent(Expr); curr; curr = getParent(Expr)){
//      if (auto F = dyn_cast<FieldDecl>(curr)) {
//         dependentDecl = F;
//         break;
//      }
//      if (auto GV = dyn_cast<GlobalVarDecl>(curr)) {
//         dependentDecl = GV;
//         break;
//      }
//      if (auto GD = dyn_cast<GlobalDestructuringDecl>(curr)) {
//         dependentDecl = GD->getFirstDecl();
//         break;
//      }
//   }

   assert(dependentDecl && "dependent variable not found");

   diagnose(globalVar, err_circular_global_value, globalVar->getSourceLoc(),
            globalVar->getName(), dependentDecl->getName());
   diagnose(dependentDecl, note_dependent_global_here,
            dependentDecl->getSourceLoc());
}

static bool checkImplicitSelf(SemaPass &SP,
                              NamedDecl *Decl,
                              IdentifierRefExpr *Ident) {
   if (Ident->getParentExpr() || Decl->isStatic() || Ident->isStaticLookup())
      return true;

   auto Self = SelfExpr::Create(SP.getContext(), Ident->getSourceLoc());

   SP.updateParent(Ident, Self);
   Ident->setParentExpr(Self);

   return SP.visitExpr(Ident, Self);
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

static ExprResult checkTemplateArguments(SemaPass &SP, IdentifierRefExpr *Ident)
{
   for (auto &TA : Ident->getTemplateArgRef()) {
      auto res = SP.visitExpr(Ident, TA);
      if (!res)
         return ExprError();

      TA = res.get();
      if (TA->isUnknownAny()) {
         Ident->setIsTypeDependent(true);
         Ident->setExprType(SP.getContext().getUnknownAnyTy());

         return Ident;
      }
   }

   return Ident;
}

ExprResult SemaPass::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
{
   if (!checkTemplateArguments(*this, Ident))
      return ExprError();

   // can only happen in a template instantiation
   if (Ident->getParentExpr() && !Ident->getDeclCtx()) {
      auto ParentRes = getRValue(Ident, Ident->getParentExpr());
      if (!ParentRes)
         return ExprError();

      Ident->setParentExpr(ParentRes.get());

      auto CtxTy = Ident->getParentExpr()->getExprType();
      if (CtxTy->isMetaType())
         CtxTy = CtxTy->asMetaType()->getUnderlyingType();

      if (Ident->isPointerAccess())
         CtxTy = CtxTy->getPointeeType();

      assert(CtxTy->isRecordType() && "bad context type");
      Ident->setDeclCtx(CtxTy->getRecord());
   }

   auto DeclName = Ident->getDeclName();
   bool LocalLookup = !Ident->getDeclCtx()
                      || Ident->getDeclCtx() == &getDeclContext();

   if (LocalLookup) {
      if (DeclName.isStr("Self")) {
         auto R = getCurrentRecordCtx();
         if (!R) {
            diagnose(Ident, err_self_outside_method, Ident->getSourceLoc(),
                     2 /*Self*/);

            return ExprError();
         }

         if (isa<ProtocolDecl>(R)) {
            Ident->setKind(IdentifierKind::MetaType);
            Ident->setExprType(Context.getMetaType(Context.getSelfTy()));

            return Ident;
         }

         Ident->setIdent(R->getDeclName());
         DeclName = Ident->getDeclName();
      }

      if (DeclName.isStr("Meta")) {
         if (Ident->getTemplateArgs().size() != 1) {
            diagnose(Ident, err_meta_type_expects, Ident->getSourceRange());
            return ExprError();
         }

         auto ArgRes = visitExpr(Ident, Ident->getTemplateArgs().front());
         if (!ArgRes)
            return ExprError();

         auto Ty = ArgRes.get()->getExprType();
         if (!Ty->isMetaType()) {
            diagnose(Ident, err_meta_type_expects, Ident->getSourceRange());
            return ExprError();
         }

         Ident->setKind(IdentifierKind::MetaType);
         Ident->setExprType(Context.getMetaType(Ty));

         return Ident;
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

      Ident->setDeclCtx(&getDeclContext());
   }

   DeclContext *Ctx = Ident->getDeclCtx();
   LookupResult lookupResult;
   LambdaScope *lambdaScope = nullptr;

   if (LocalLookup) {
      DeclarationNameTable &NameTable = Context.getDeclNameTable();
      for (auto S = currentScope; S; S = S->getEnclosingScope()) {
         switch (S->getTypeID()) {
         case Scope::LambdaScopeID: {
            if (!lambdaScope)
               lambdaScope = cast<LambdaScope>(S);

            LLVM_FALLTHROUGH;
         }
         case Scope::FunctionScopeID: {
            lookupResult = cast<FunctionScope>(S)
               ->getCallableDecl()->lookupSingle<FuncArgDecl>(DeclName);

            if (lookupResult)
               break;

            LLVM_FALLTHROUGH;
         }
         case Scope::BlockScopeID: {
            DeclarationName DN = NameTable.getLocalVarName(DeclName,
                                                           cast<BlockScope>(S));

            lookupResult = Ctx->lookup(DN);
            break;
         }
         default:
            break;
         }

         if (lookupResult)
            break;
      }
   }

   if (!lookupResult) {
      lookupResult = Ident->getDeclCtx()->lookup(DeclName);

      if (!lookupResult) {
         if (LocalLookup) {
            // implicit 'self'
            if (auto R = getCurrentRecordCtx()) {
               if (R->hasAnyDeclNamed(DeclName)) {
                  Ident->setDeclCtx(R);
                  return visitIdentifierRefExpr(Ident);
               }
            }

            if (auto ty = getBuiltinType(DeclName)) {
               Ident->setKind(IdentifierKind::MetaType);
               Ident->setExprType(Context.getMetaType(ty));

               return Ident;
            }
         }
         else if (auto R = dyn_cast<RecordDecl>(Ident->getDeclCtx())) {
            // builtin members
            if (DeclName.isStr("typeof")) {
               auto TI = getTypeInfoDecl();
               if (!TI) {
                  diagnose(Ident, err_no_builtin_decl, Ident->getSourceRange(),
                           10);

                  return ExprError();
               }

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

   if (!lookupResult) {
      if (currentScope && currentScope->hasUnresolvedStaticCond()) {
         // this identifier might have been introduced in a dependent
         // static if / for
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

   bool UsedTemplateParams = false;
   NamedDecl *lookup = lookupResult.front();
   QualType lookupTy;

   if (auto Var = dyn_cast<LocalVarDecl>(lookup)) {
      assert(LocalLookup && "found local variable through member lookup?");
      Ident->setLocalVar(Var);

      if (lambdaScope) {
         Var->setCaptured(true);

         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()->addCapture(Var));
         Ident->setIsCapture(true);
      }

      Ident->setKind(IdentifierKind::LocalVar);

      lookupTy = Var->getType();
      if (!lookupTy->isReferenceType() && !lookupTy->isUnknownAnyType())
         lookupTy = Context.getReferenceType(lookupTy);

      Ident->setIsTypeDependent(Var->isTypeDependent());
   }
   else if (auto G = dyn_cast<GlobalVarDecl>(lookup)) {
      if (!G->getType()) {
         if (!VisitedGlobalDecls.insert(G).second) {
            diagnoseCircularlyDependentGlobalVariables(Ident, G);
            return ExprError();
         }

         DeclScopeRAII declScopeRAII(*this, G->getDeclContext());

         visitGlobalVarDecl(G);
         G->setGlobalOrdering(numGlobals++);
      }

      checkAccessibility(G, Ident);

      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setGlobalVar(G);

      lookupTy = G->getType();
      if (!lookupTy->isReferenceType() && !lookupTy->isUnknownAnyType())
         lookupTy = Context.getReferenceType(lookupTy);

      Ident->setIsTypeDependent(G->isTypeDependent());
   }
   else if (auto Arg = dyn_cast<FuncArgDecl>(lookup)) {
      assert(LocalLookup && "found argument through member lookup?");
      Ident->setFuncArg(Arg);

      if (lambdaScope) {
         Arg->setCaptured(true);

         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()->addCapture(Arg));
         Ident->setIsCapture(true);
      }

      Ident->setKind(IdentifierKind::FunctionArg);

      lookupTy = Arg->getType();
      if (!lookupTy->isReferenceType() && !lookupTy->isUnknownAnyType())
         lookupTy = Context.getReferenceType(lookupTy);

      Ident->setIsTypeDependent(Arg->isTypeDependent());

      if (Arg->isVariadicArgPackExpansion()) {
         Ident->setContainsUnexpandedParameterPack(true);
      }
   }
   else if (auto Case = dyn_cast<EnumCaseDecl>(lookup)) {
      checkAccessibility(Case, Ident);
      return HandleEnumCase(Ident, cast<EnumDecl>(Case->getRecord()));
   }
   else if (isa<CallableDecl>(lookup)) {
      auto func = checkFunctionReference(Ident, Ident->getDeclCtx(),
                                         DeclName,
                                         Ident->getTemplateArgs());

      if (!func)
         return ExprError();

      checkAccessibility(func, Ident);
      UsedTemplateParams = func->isInstantiation();

      FunctionType *FTy = func->getFunctionType();
      if (auto M = dyn_cast<MethodDecl>(func)) {
         auto R = cast<RecordDecl>(Ident->getDeclCtx());

         if (Ident->isStaticLookup()) {
            // function takes additional 'self' argument
            llvm::SmallVector<QualType, 8> argTypes;
            QualType selfTy(Context.getRecordType(R));

            argTypes.push_back(selfTy);
            argTypes.append(FTy->getParamTypes().begin(),
                            FTy->getParamTypes().end());

            FTy = Context.getLambdaType(FTy->getReturnType(), argTypes,
                                        FTy->getRawFlags());

            Ident->setCallable(func);
         }
         else {
            FTy = Context.getLambdaType(FTy);
            Ident->setPartiallyAppliedMethod(M);
         }
      }
      else {
         Ident->setCallable(func);
      }

      lookupTy = FTy;
   }
   else if (auto Alias = dyn_cast<AliasDecl>(lookup)) {
      auto AliasRes = checkAlias(lookupResult, Ident->getTemplateArgs(),
                                 Ident->getSourceLoc());

      if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
         Ident->setIsTypeDependent(AliasRes.isTypeDependent());
         Ident->setIsValueDependent(AliasRes.isValueDependent());
         Ident->setExprType(UnknownAnyTy);

         return Ident;
      }

      if (!AliasRes) {
         AliasRes.getCandSet().diagnoseAlias(*this, Alias->getDeclName(),
                                             Ident->getTemplateArgs(), Ident);

         assert(Ident->isInvalid());
         return ExprError();
      }

      Alias = AliasRes.getAlias();

      if (Alias->isInvalid()) {
         Ident->setIsInvalid(true);
         return ExprError();
      }

      checkAccessibility(Alias, Ident);

      if (Alias->isInCompilerNamespace()) {
         return HandleReflectionAlias(Alias, Ident);
      }

      Ident->setKind(IdentifierKind::Alias);
      Ident->setAliasVal(new(Context) Variant(Alias->getAliasExpr()
                                                   ->getEvaluatedExpr()));

      UsedTemplateParams = Alias->isInstantiation();
      lookupTy = Alias->getType();
   }
   else if (auto NS = dyn_cast<NamespaceDecl>(lookup)) {
      Ident->setKind(IdentifierKind::Namespace);
      Ident->setNamespaceDecl(NS);

      lookupTy = UnknownAnyTy;
   }
   else if (auto R = dyn_cast<RecordDecl>(lookup)) {
      if (R->isTemplate()) {
         TemplateArgList list(*this, R, Ident->getTemplateArgs());
         UsedTemplateParams = true;

         if (list.isStillDependent()) {
            lookupTy = Context.getDependentRecordType(R, move(list));
         }
         else {
            auto Compat = list.checkCompatibility();
            if (!Compat) {
               diagnoseTemplateArgErrors(R, Ident, list, Compat);
               return ExprError();
            }
         }

         if (!lookupTy) {
            auto MaybeInst = Instantiator.InstantiateRecord(Ident, R,
                                                            move(list));

            if (MaybeInst)
               R = MaybeInst.getValue();

            lookupTy = Context.getRecordType(R);
         }
      }
      else {
         lookupTy = Context.getRecordType(R);
      }

      checkAccessibility(R, Ident);

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setMetaType(Context.getMetaType(lookupTy));

      lookupTy = Ident->getMetaType();
   }
   else if (auto TD = dyn_cast<TypedefDecl>(lookup)) {
      if (!ensureDeclared(TD)) {
         Ident->setIsInvalid(true);
         return ExprError();
      }

      checkAccessibility(TD, Ident);

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setMetaType(Context.getMetaType(TD->getOriginTy()));

      lookupTy = Ident->getMetaType();
   }
   else if (auto F = dyn_cast<FieldDecl>(lookup)) {
      if (!checkImplicitSelf(*this, F, Ident))
         return ExprError();

      if (!F->isStatic() && Ident->isStaticLookup()) {
         auto Rec = F->getRecord();
         diagnose(Ident, err_non_static_member_accessed_statically,
                  Ident->getSourceLoc(), /*field*/ 0, F->getName(),
                  Rec->getSpecifierForDiagnostic(), Rec->getName());

         return ExprError();
      }

      if (!F->getType().isResolved()) {
         if (!VisitedGlobalDecls.insert(F).second) {
            diagnoseCircularlyDependentGlobalVariables(Ident, F);
            return ExprError();
         }

         DeclScopeRAII declScopeRAII(*this, F->getDeclContext());

         visitFieldDecl(F);
         F->setGlobalOrdering(numGlobals++);
      }

      auto Res = HandleFieldAccess(Ident, F);
      if (!Res)
         return Res;

      lookupTy = Ident->getExprType();
   }
   else if (auto P = dyn_cast<PropDecl>(lookup)) {
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

      lookupTy = Ident->getExprType();
   }
   else if (auto AT = dyn_cast<AssociatedTypeDecl>(lookup)) {
      Ident->setKind(IdentifierKind::AssociatedType);

      if (AssociatedTypeSubst) {
         auto SubstAT = AssociatedTypeSubst
            ->lookupSingle<AssociatedTypeDecl>(AT->getDeclName());

         if (SubstAT)
            AT = SubstAT;
      }

      lookupTy = Context.getMetaType(Context.getAssociatedType(AT));
   }
   else if (auto Param = dyn_cast<TemplateParamDecl>(lookup)) {
      if (Param->isTypeName()) {
         lookupTy = Context.getMetaType(Context.getTemplateArgType(Param));
         Ident->setIsTypeDependent(true);
      }
      else {
         lookupTy = Param->getValueType();
         Ident->setIsValueDependent(true);
      }

      if (Param->isVariadic()) {
         Ident->setContainsUnexpandedParameterPack(true);
      }

      Ident->setTemplateParam(Param);
      Ident->setKind(IdentifierKind::TemplateParam);
   }
   else {
      llvm_unreachable("unhandled named decl");
   }

   Ident->setExprType(lookupTy);

   if (lookupTy->isDependentType())
      Ident->setIsTypeDependent(true);

   // probably meant to be a subscript
   if (!UsedTemplateParams && !Ident->getTemplateArgs().empty()) {
      auto Subscript = SubscriptExpr::Create(
         Context, Ident->getSourceRange(), Ident,
         Ident->getTemplateArgs());

      // clear template arguments so they don't accidentally get instantiated
      Ident->getTemplateArgRef().clear();
      Ident->setSemanticallyChecked(true);

      Subscript->setIsLHSOfAssignment(Ident->isLHSOfAssignment());
      Ident->setIsLHSOfAssignment(false);

      updateParent(Subscript, Ident);
      return visitExpr(Ident, Subscript);
   }

   return Ident;
}

ExprResult SemaPass::visitBuiltinIdentExpr(BuiltinIdentExpr *Ident)
{
   cdot::Type *builtinType;

   switch (Ident->getIdentifierKind()) {
      default: llvm_unreachable("bad builtin ident!");
      case BuiltinIdentifier::NULLPTR: {
         if (Ident->getContextualType().isNull()) {
            diagnose(Ident, err_requires_contextual_type, Ident->getSourceLoc(),
                     "__nullptr");
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

ExprResult SemaPass::visitSuperExpr(SuperExpr *Expr)
{
   auto R = getCurrentRecordCtx();
   if (!R) {
      diagnose(Expr, err_self_outside_method, Expr->getSourceLoc(),
               1 /*super*/);

      return ExprError();
   }

   auto currentCl = dyn_cast<ClassDecl>(R);
   if (!currentCl || currentCl->getParentClass() == nullptr) {
      diagnose(Expr, err_super_without_base, Expr->getSourceLoc(),
               getCurrentRecordCtx()->getName());

      return ExprError();
   }

   Expr->setExprType(Context.getRecordType(currentCl));
   return Expr;
}

ExprResult SemaPass::visitSelfExpr(SelfExpr *Expr)
{
   auto R = getCurrentRecordCtx();
   if (!R) {
      diagnose(Expr, err_self_outside_method, Expr->getSourceLoc(),
               0 /*self*/);

      return ExprError();
   }

   QualType SelfTy;
   if (R->isTemplate()) {
      Expr->setIsTypeDependent(true);

      TemplateArgList ArgList(*this, R);
      SelfTy = Context.getDependentRecordType(R, move(ArgList));
   }
   else {
      SelfTy = Context.getRecordType(R);
   }

   auto fun = getCurrentFun();
   bool mutableSelf = false;

   if (auto M = dyn_cast_or_null<MethodDecl>(fun))
      mutableSelf = M->hasMutableSelf();

   if (mutableSelf)
      SelfTy = Context.getReferenceType(SelfTy);

   Expr->setExprType(SelfTy);
   return Expr;
}

CallableDecl*
SemaPass::checkFunctionReference(Expression *E,
                                 DeclContext *Ctx,
                                 DeclarationName funcName,
                                 llvm::ArrayRef<Expression*> templateArgs) {
   llvm_unreachable("FIX THIS SHIT!");
   CandidateSet CandSet;
   auto lookup = Ctx->lookup(funcName);

   for (auto ND : lookup)
      if (auto fn = dyn_cast<CallableDecl>(ND))
         CandSet.addCandidate(fn);

   if (CandSet.Candidates.empty())
      return nullptr;

   if (CandSet.Candidates.size() == 1)
      return CandSet.Candidates.front().Func;

   llvm::SmallPtrSet<CallableDecl *, 8> Candidates;
   for (auto &Cand : CandSet.Candidates) {
      if (Cand.Func->isTemplate()) {
         TemplateArgList list(*this, Cand.Func, templateArgs,
                              E->getSourceLoc());

         if (list.checkCompatibility()) {
            auto Inst = Instantiator.InstantiateFunction(
               E, cast<FunctionDecl>(Cand.Func), move(list));

            if (Inst) {
               Candidates.insert(Inst.getValue());
            }
         }
      }
      else if (templateArgs.empty()) {
         Candidates.insert(Cand.Func);
      }
   }

   auto CtxTy = E->getContextualType();
   if (CtxTy && CtxTy->isFunctionType()) {
      FunctionType *FnTy = CtxTy->asFunctionType();

      for (auto &Cand : CandSet.Candidates) {
         if (Cand.Func->getFunctionType() != FnTy) {
            Candidates.erase(Cand.Func);
         }
      }
   }

   if (Candidates.size() > 1) {
      auto fn = CandSet.Candidates.front().Func;
      diagnose(E, err_ambiguous_reference, E->getSourceLoc(),
               !isa<MethodDecl>(fn), fn->getName());

      for (auto C : Candidates)
         diagnose(note_candidate_here, C->getSourceLoc());

      return nullptr;
   }

   return *Candidates.begin();
}

SemaPass::AliasResult
SemaPass::checkAlias(LookupResult &lookupResult,
                     llvm::ArrayRef<Expression *> templateArgs,
                     SourceLocation loc) {
   size_t bestMatch    = 0;
   bool typeDependent  = false;
   bool valueDependent = false;
   AliasDecl *match    = nullptr;

   CandidateSet CandSet;

   for (auto ND : lookupResult) {
      auto alias = dyn_cast<AliasDecl>(ND);
      if (!alias || alias->getConstraints().size() < bestMatch)
         continue;

      auto &Cand = CandSet.addCandidate(alias);

      if (!templateArgs.empty() || alias->isTemplate()) {
         TemplateArgList list(*this, alias, templateArgs);

         if (list.isStillDependent()) {
            Cand.setIsInvalid();
            typeDependent = true;
            continue;
         }

         auto comp = list.checkCompatibility();
         if (!comp) {
            Cand.setTemplateArgListFailure(comp);
            continue;
         }

         auto Inst = Instantiator.InstantiateAlias(alias, loc, move(list));
         if (Inst.hasValue()) {
            alias = Inst.getValue();
            Cand.Alias = alias;
         }
         else {
            Cand.setIsInvalid();
            continue;
         }
      }

      if (checkAlias(alias, Cand)) {
         if (match) {
            CandSet.Status = CandidateSet::Ambiguous;
         }
         else {
            CandSet.Status = CandidateSet::Success;
            match = alias;
         }
      }
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
   for (auto &C : Alias->getConstraints()) {
      auto res = evaluateAsBool(C);
      if (res.hadError()) {
         Cand.setIsInvalid();
         Alias->setIsInvalid(true);
         return false;
      }

      if (!res.getValue().getZExtValue()) {
         Cand.setHasFailedConstraint(C);
         return false;
      }
   }

   return true;
}

IdentifierRefExpr *SemaPass::wouldBeValidIdentifier(SourceLocation Loc,
                                                    IdentifierInfo *maybeIdent,
                                                    bool LHSOfAssignment) {
   DiagnosticScopeRAII diagnosticScopeRAII(*this);

   alignas(8) IdentifierRefExpr expr(Loc, maybeIdent);
   expr.setIsLHSOfAssignment(LHSOfAssignment);

   (void)visitExpr(&expr);
   if (!expr.foundResult())
      return nullptr;

   return new (getContext()) IdentifierRefExpr(move(expr));
}

ExprResult SemaPass::checkNamespaceRef(Expression *Expr)
{
   Expression *ParentExpr = Expr->maybeGetParentExpr();
   if (!ParentExpr)
      return Expr;

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      Expr->copyStatusFlags(ParentExpr);
      return ExprError();
   }

   ParentExpr = SemaRes.get();
   Expr->setParentExpr(ParentExpr);

   DeclContext *Ctx = nullptr;
   if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      if (Ident->getKind() == IdentifierKind::Namespace) {
         Ctx = Ident->getNamespaceDecl();
      }
   }

   if (!Ctx) {
      Expr->copyStatusFlags(ParentExpr);
      return Expr;
   }

   if (auto MemExpr = dyn_cast<MemberRefExpr>(Expr)) {
      MemExpr->setParentExpr(nullptr);
      MemExpr->setContext(Ctx);
   }
   else if (auto Call = dyn_cast<CallExpr>(Expr)) {
      Call->setParentExpr(nullptr);
      Call->setContext(Ctx);
   }
   else {
      Expr->copyStatusFlags(ParentExpr);
   }

   return Expr;
}

ExprResult SemaPass::visitMemberRefExpr(MemberRefExpr *Expr)
{
   if (getStaticForValue(Expr->getIdentInfo())) {
      Expr->setIsValueDependent(true);
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   }

   if (!checkNamespaceRef(Expr))
      return ExprError();

   DeclContext *Ctx = Expr->getContext();
   bool isStatic = true;

   if (!Ctx) {
      auto ParentExpr = Expr->getParentExpr();
      auto ParentResult = getRValue(Expr, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Expr->setParentExpr(ParentExpr);

      auto ty = ParentExpr->getExprType()->stripReference();
      if (ty->isUnknownAnyType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(UnknownAnyTy);

         return Expr;
      }

      if (ty->isPointerType()) {
         if (!Expr->isPointerAccess())
            diagnose(Expr, err_access_member_on_pointer, Expr->getSourceLoc());

         ty = ty->getPointeeType();
      }
      else if (Expr->isPointerAccess()) {
         diagnose(Expr, err_member_access_non_pointer, ty, Expr->getSourceLoc());
      }

      if (cdot::MetaType *Meta = ty->asMetaType()) {
         auto underlying = Meta->getUnderlyingType();

         if (underlying->isRecordType()) {
            Ctx = underlying->getRecord();
         }
         else {
            return HandleStaticTypeMember(Expr, *underlying);
         }
      }
      else if (ty->isRecordType()) {
         Ctx = ty->getRecord();
         isStatic = false;
      }
      else {
         return HandleBuiltinTypeMember(Expr, ty);
      }
   }

   assert(Ctx && "should have returned earlier!");

   auto *Ident = new(Context)
      IdentifierRefExpr(Expr->getSourceLoc(), Expr->getIdentInfo(),
                        move(Expr->getTemplateArgRef()), Ctx);

   Ident->copyStatusFlags(Expr);
   Ident->setIsInvalid(false);

   Ident->setParentExpr(Expr->getParentExpr());
   Ident->setStaticLookup(isStatic);
   Ident->setPointerAccess(Expr->isPointerAccess());
   Ident->setIsLHSOfAssignment(Expr->isLHSOfAssignment());

   // do not move this to the top, otherwise the Ident expr will be marked
   // semantically evaluated
   Expr->setSemanticallyChecked(true);

   return visitExpr(Expr, Ident);
}

ExprResult SemaPass::HandleStaticTypeMember(MemberRefExpr *Expr, QualType Ty)
{
   if (Expr->getIdentInfo()->isStr("typeof")) {
      auto TI = getTypeInfoDecl();
      if (!TI) {
         diagnose(Expr, err_no_builtin_decl, Expr->getSourceRange(), 10);
         return ExprError();
      }

      Expr->setExprType(Context.getReferenceType(Context.getRecordType(TI)));
      Expr->setMetaType(Ty);
      Expr->setKind(MemberKind::TypeOf);

      return Expr;
   }

   diagnose(Expr, err_access_member_on_type, Expr->getSourceLoc(), Ty);
   return ExprError();
}

ExprResult SemaPass::HandleBuiltinTypeMember(MemberRefExpr *Expr,
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

   diagnose(Expr, err_access_member_on_type, Ty, Expr->getSourceRange());
   return ExprError();
}

ExprResult SemaPass::HandleEnumCase(IdentifierRefExpr *node, EnumDecl *E)
{
   if (E->hasCase(node->getIdentInfo())) {
      node->setSemanticallyChecked(true);

      auto enumCaseExpr = new(getContext()) EnumCaseExpr(node->getSourceLoc(),
                                                         node->getIdentInfo());

      enumCaseExpr->setEnum(E);
      enumCaseExpr->setContextualType(node->getContextualType());

      return visitEnumCaseExpr(enumCaseExpr);
   }

   diagnose(node, err_enum_case_not_found, node->getSourceLoc(),
            node->getIdent(), 0);

   return ExprError();
}

ExprResult SemaPass::HandleFieldAccess(IdentifierRefExpr *Ident,
                                       FieldDecl *F) {
   ensureDeclared(F);

   bool InOwningRecord = false;
   auto Ctx = &getDeclContext();
   while (Ctx) {
      if (auto CurrentRec = dyn_cast<RecordDecl>(Ctx)) {
         if (CurrentRec == F->getRecord()) {
            InOwningRecord = true;
            break;
         }
      }

      Ctx = Ctx->getParentCtx();
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
      if (Ident->getTemplateArgs().empty()) {
         Ident->setContainsUnexpandedParameterPack(true);
         Ident->setIsTypeDependent(true);
         Ident->setExprType(F->getType());

         return Ident;
      }

      if (Ident->getTemplateArgs().size() != 1) {
         diagnose(Ident, err_variadic_field_single_index,
                  Ident->getSourceRange());

         Ident->getTemplateArgRef().resize(1);
      }

      auto ArgRes = visitExpr(Ident, Ident->getTemplateArgs().front());
      if (!ArgRes)
         return ExprError();

      if (ArgRes.get()->isDependent()) {
         Ident->setExprType(UnknownAnyTy);
         return Ident;
      }

      auto StaticRes = evalStaticExpr(ArgRes.get());
      if (!StaticRes)
         return ExprError();

      auto &V = StaticRes.getValue();
      if (!V.isInt()) {
         diagnose(Ident, err_variadic_field_index_integral,
                  ArgRes.get()->getSourceRange());

         return ExprError();
      }

      Ident->getTemplateArgRef().front() = ArgRes.get();
   }

   QualType ty(F->getType());
   if (ty->isDependentType())
      Ident->setIsTypeDependent(true);

   // Use a getter if available
//   if (F->hasGetter() && !Ident->isLHSOfAssignment() && !InOwningRecord) {
//      Ident->setAccessorMethod(F->getGetterMethod());
//      Ident->setExprType(F->getType());
//      Ident->setKind(IdentifierKind::Accessor);
//
//      checkAccessibility(F->getGetterMethod(), Ident);
//
//      return Ident;
//   }
//
//   // Use a setter if available (and we're on the left side of an assignment)
//   if (F->hasSetter() && Ident->isLHSOfAssignment() && !InOwningRecord) {
//      Ident->setAccessorMethod(F->getSetterMethod());
//      Ident->setExprType(Context.getVoidType());
//      Ident->setKind(IdentifierKind::Accessor);
//
//      checkAccessibility(F->getSetterMethod(), Ident);
//
//      return Ident;
//   }

   checkAccessibility(F, Ident);

   if (F->isStatic()) {
      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setStaticFieldDecl(F);
   }
   else {
      Ident->setKind(IdentifierKind::Field);
      Ident->setFieldDecl(F);
   }

   Ident->setExprType(Context.getReferenceType(ty));
   return Ident;
}

ExprResult SemaPass::HandlePropAccess(IdentifierRefExpr *Ident,
                                      PropDecl *P) {
   assert(P && "shouldn't be called otherwise");
   ensureDeclared(P);

   // if there are still template arguments at this point, it means that the
   // user probably meant a subscript, this will be resolved after this
   // method. It also means, however, that this is not an accessor assignment
   bool NotActuallySetter = !Ident->getTemplateArgs().empty();

   QualType ty;
   if (Ident->isLHSOfAssignment() && !NotActuallySetter) {
      if (!P->hasSetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*setter*/ 1);
      }
      else {
         checkAccessibility(P->getSetterMethod(), Ident);
         maybeInstantiateMemberFunction(P->getSetterMethod(), Ident);
      }

      // this path should only be taken when resolving an expression sequence
      // so this type is only used to get the correct `=` overloads; the
      // actual type of the resolved expression will be `Void`
      ty = Context.getReferenceType(P->getType());
   }
   else {
      if (!P->hasGetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*getter*/ 0);
      }
      else {
         checkAccessibility(P->getGetterMethod(), Ident);
         maybeInstantiateMemberFunction(P->getGetterMethod(), Ident);
      }

      ty = P->getType();
   }

   if (ty->isDependentType())
      Ident->setIsTypeDependent(true);

   Ident->setAccessor(P);
   Ident->setExprType(ty);

   return Ident;
}

ExprResult SemaPass::visitTupleMemberExpr(TupleMemberExpr *Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = getRValue(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   TupleType *tup = ParentExpr->getExprType()->asTupleType();
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

   QualType contained(tup->getContainedType(Expr->getIndex()));
   Expr->setExprType(Context.getReferenceType(contained));

   return Expr;
}

ExprResult SemaPass::visitEnumCaseExpr(EnumCaseExpr *Expr)
{
   if (!Expr->getEnum()) {
      auto ty = Expr->getContextualType();
      if (!ty) {
         diagnose(Expr, err_requires_contextual_type, Expr->getSourceLoc(),
                  "unqualified enum case");
         return ExprError();
      }

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

      Expr->setEnum(cast<EnumDecl>(rec));
   }

   auto E = Expr->getEnum();
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
            return ExprError();

         arg = result.get();
         if (arg->isUnknownAny()) {
            Expr->setIsTypeDependent(true);
            Expr->setExprType(UnknownAnyTy);

            return Expr;
         }
      }
   }

   if (E->isTemplate()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(Context.getRecordType(E));

      return Expr;
   }

   auto CandSet = lookupCase(Expr->getIdentInfo(), E, Expr->getArgs(),
                             {}, Expr);

   if (CandSet.isDependent()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(Context.getRecordType(E));

      return Expr;
   }

   if (!CandSet)
      return ExprError();

   auto Case = cast<EnumCaseDecl>(CandSet.getBestMatch().Func);
   checkAccessibility(Case, Expr);

   PrepareCallArgs(Expr->getArgs(), Case);
   ApplyCasts(Expr->getArgs(), Expr, CandSet);

   E = cast<EnumDecl>(Case->getRecord());

   Expr->setEnum(E);
   Expr->setExprType(QualType(Context.getRecordType(E)));

   return Expr;
}

ExprResult SemaPass::visitSubscriptExpr(SubscriptExpr *Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = getRValue(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   QualType SubscriptedTy = ParentExpr->getExprType();
   for (auto &Idx : Expr->getIndices()) {
      if (!SubscriptedTy->isRecordType())
         Idx->setContextualType(Context.getIntTy());

      auto indexResult = visitExpr(Expr, Idx);
      if (!indexResult)
         return ExprError();

      Idx = indexResult.get();
   }

   if (SubscriptedTy->isRecordType()) {
      auto *II = &Context.getIdentifiers().get("[]");
      auto DeclName = Context.getDeclNameTable().getPostfixOperatorName(*II);
      auto call = new(getContext()) CallExpr(SourceLocation(),
                                             Expr->getSourceRange(),
                                             ParentExpr,
                                             Expr->getIndices().vec(),
                                             DeclName);

      return visitExpr(Expr, call);
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

   Expr->setExprType(Context.getReferenceType(resultType));
   return Expr;
}

void SemaPass::diagnoseMemberNotFound(ast::DeclContext *Ctx,
                                      Statement *Subject,
                                      DeclarationName memberName,
                                      diag::MessageKind msg) {
   if (!Ctx) {
      diagnose(Subject, msg, Subject->getSourceLoc(), memberName);
   }
   else {
      while (Ctx) {
         switch (Ctx->getDeclKind()) {
         case Decl::StructDeclID: case Decl::ClassDeclID:
         case Decl::EnumDeclID: case Decl::UnionDeclID:
         case Decl::ProtocolDeclID: case Decl::NamespaceDeclID: {
            auto ND = cast<NamedDecl>(Ctx);
            diagnose(Subject, err_member_not_found, Subject->getSourceLoc(),
                     ND->getSpecifierForDiagnostic(), ND->getDeclName(),
                     memberName);

            return;
         }
         case Decl::TranslationUnitID:
            diagnose(Subject, msg, Subject->getSourceLoc(), memberName);
            break;
         default:
            break;
         }

         Ctx = Ctx->getParentCtx();
      }
   }
}

void SemaPass::diagnoseMemberNotFound(ast::DeclContext *Ctx,
                                      Decl *Subject,
                                      DeclarationName memberName,
                                      diag::MessageKind msg) {
   if (!Ctx) {
      diagnose(Subject, msg, Subject->getSourceLoc(), memberName);
   }
   else {
      while (Ctx) {
         switch (Ctx->getDeclKind()) {
         case Decl::StructDeclID: case Decl::ClassDeclID:
         case Decl::EnumDeclID: case Decl::UnionDeclID:
         case Decl::ProtocolDeclID: case Decl::NamespaceDeclID: {
            auto ND = cast<NamedDecl>(Ctx);
            diagnose(Subject, err_member_not_found, Subject->getSourceLoc(),
                     ND->getSpecifierForDiagnostic(), ND->getName(),
                     memberName);

            return;
         }
         case Decl::TranslationUnitID:
            diagnose(Subject, msg, Subject->getSourceLoc(), memberName);
            break;
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
      auto givenSize  = list.getOriginalArgs().size();

      diagnose(ErrorStmt, note_too_many_template_args,
               neededSize, givenSize, Template->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgKind: {
      unsigned diagSelect = unsigned(Cand.Data1);
      unsigned select1    = diagSelect & 0x3u;
      unsigned select2    = (diagSelect >> 2u) & 0x3u;

      auto idx = Cand.Data2;
      auto Param = Template->getTemplateParams()[idx];

      diagnose(ErrorStmt, note_template_arg_kind_mismatch, select1,
               select2, idx + 1,
               list.getNamedArg(Param->getName())->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here,
               Param->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgVal: {
      auto givenTy = reinterpret_cast<Type*>(Cand.Data1);
      auto idx = Cand.Data2;

      auto Param = Template->getTemplateParams()[idx];
      auto neededTy = Param->getValueType();

      diagnose(ErrorStmt, note_template_arg_type_mismatch, givenTy,
               idx + 1, neededTy);

      diagnose(ErrorStmt, note_template_parameter_here,
               Param->getSourceLoc());

      break;
   }
   default:
      break;
   }
}

void SemaPass::checkAccessibility(NamedDecl *ND, Expression *Expr)
{
   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return;
   case AccessSpecifier::Private: {
      // only visible within the immediate context the symbol was defined in
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (Curr == Ctx)
            return;
      }

      // declaration is not accessible here
      diagnose(Expr, err_private_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), Expr->getSourceRange());

      break;
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (Curr == Ctx)
            return;

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return;
      }

      // declaration is not accessible here
      diagnose(Expr, err_protected_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), C->getDeclName(), Expr->getSourceRange());

      break;
   }
   case AccessSpecifier::FilePrivate:
   case AccessSpecifier::Internal: // FIXME actually implement internal
   {
      // visible within the file it was declared
      auto &FileMgr = compilationUnit->getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(Expr->getSourceLoc());

      if (DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID))
         return;

      // declaration is not accessible here
      diagnose(Expr, err_fileprivate_access, ND->getSpecifierForDiagnostic(),
               ND->getDeclName(), FileMgr.getFileName(DeclID),
               Expr->getSourceRange());

      break;
   }
   }

   diagnose(note_access_spec_here, /*implicitly*/ !ND->getAccessRange(),
            (int)AccessSpec, ND->getAccessRange(), ND->getSourceLoc());
}

} // namespace ast
} // namespace cdot