//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "TemplateInstantiator.h"
#include "AST/Transform.h"
#include "AST/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace ast {

void SemaPass::checkClassAccessibility(RecordDecl *cl,
                                       Expression *cause) {
//   if (!cl->isPrivate())
//      return;
//
//   return;
//   for (const auto &NS : declPass->getCurrentNamespaceVec()) {
//      if (getSymTab().getNamespace(NS) == cl->getDeclarationNamespace())
//         return;
//   }
//
//   diagnose(cause, err_class_not_accessible, cl->getNameSelector(),
//            cl->getName());
}

void SemaPass::checkMemberAccessibility(RecordDecl *record,
                                        const string& memberName,
                                        const AccessModifier &access,
                                        Expression *cause) {
//   if (access == AccessModifier::PROTECTED
//       && !record->protectedPropAccessibleFrom(currentClass())) {
//      diagnose(cause, err_protected_member_access, memberName,
//               record->getNameSelector(), record->getName());
//   }
//   else if (access == AccessModifier::PRIVATE
//            && !record->privatePropAccessibleFrom(currentClass())) {
//      diagnose(cause, err_private_member_access, memberName,
//               record->getNameSelector(), record->getName());
//   }
}

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

   auto Res = SP.visitExpr(Ident, Self);
   if (!Res)
      return false;

   return true;
}

QualType SemaPass::getStaticForValue(llvm::StringRef name) const
{
   for (auto S = currentScope; S; S = S->getEnclosingScope()) {
      if (auto SF = dyn_cast<StaticForScope>(S)) {
         if (SF->getElementName() == name)
            return SF->getElementTy();
      }
   }

   return QualType();
}

ExprResult SemaPass::visitIdentifierRefExpr(IdentifierRefExpr *Ident)
{
   assert(Ident->getIdentInfo() && "identifier expr without identifier!");

   bool currentDeclCtx = Ident->getDeclCtx() == nullptr;
   if (currentDeclCtx) {
      if (Ident->getIdent() == "Self") {
         auto R = getCurrentRecordCtx();
         if (!R) {
            diagnose(Ident, err_self_outside_method, Ident->getSourceLoc(),
                     2 /*Self*/);

            return ExprError();
         }

         Ident->setDeclCtx(R);
      }
      else if (auto Ty = getStaticForValue(Ident->getIdent())) {
         Ident->setIsValueDependent(true);
         Ident->setIsTypeDependent(true);
         Ident->setExprType(Ty);
         Ident->setFoundResult(true);

         return Ident;
      }
      else {
         Ident->setDeclCtx(&getDeclContext());
      }
   }

   LookupResult lookupResult;
   LambdaScope *lambdaScope = nullptr;

   if (currentDeclCtx) {
      llvm::SmallString<128> localVarName;
      localVarName += Ident->getIdent();

      auto initialSize = localVarName.size();
      for (auto scope = currentScope; scope;
           scope = scope->getEnclosingScope()) {

         IdentifierTable &Idents = Context.getIdentifiers();
         if (auto FS = dyn_cast<FunctionScope>(scope)) {
            if (auto LS = dyn_cast<LambdaScope>(scope); !lambdaScope)
               lambdaScope = LS;

            auto &ArgName = Idents.get(localVarName.str());
            lookupResult = FS->getCallableDecl()
                             ->lookupSingle<FuncArgDecl>(ArgName);

            if (lookupResult)
               break;
         }
         else if (auto BS = dyn_cast<BlockScope>(scope)) {
            localVarName += std::to_string(BS->getScopeID());

            auto &ValName = Idents.get(localVarName.str());
            lookupResult = Ident->getDeclCtx()->lookup(ValName);
            if (lookupResult)
               break;

            localVarName.resize(initialSize);
         }
      }
   }

   if (!lookupResult) {
      lookupResult = Ident->getDeclCtx()->lookup(Ident->getIdentInfo());

      if (!lookupResult && currentDeclCtx) {
         // implicit 'self'
         if (auto R = getCurrentRecordCtx()) {
            if (R->hasAnyDeclNamed(Ident->getIdentInfo())) {
               Ident->setDeclCtx(R);
               return visitIdentifierRefExpr(Ident);
            }
         }

         if (auto ty = getBuiltinType(Ident->getIdentInfo())) {
            Ident->setKind(IdentifierKind::MetaType);
            Ident->setExprType(Context.getMetaType(ty));

            return Ident;
         }
      }
   }

   if (!lookupResult) {
      if (currentScope && currentScope->hasUnresolvedStaticCond()) {
         // this identifier might have been introduced in a dependent
         // static if / for
         Ident->setIsTypeDependent(true);
      }
      else {
         auto diagId = Ident->isInTypePosition()
                       ? diag::err_type_not_found
                       : diag::err_undeclared_identifer;

         diagnoseMemberNotFound(currentDeclCtx ? nullptr : Ident->getDeclCtx(),
                                Ident, Ident->getIdent(), diagId);
      }

      return ExprError();
   }

   Ident->setFoundResult(true);

   bool UsedTemplateParams = false;
   NamedDecl *lookup = lookupResult.front();
   QualType lookupTy;

   if (auto Var = dyn_cast<LocalVarDecl>(lookup)) {
      assert(currentDeclCtx && "found local variable through member lookup?");

      Ident->setLocalVar(Var);
      if (lambdaScope) {
         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()->addCapture(Var));
         Ident->setKind(IdentifierKind::CapturedLocalVar);
      }
      else {
         Ident->setKind(IdentifierKind::LocalVar);
      }

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

      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setGlobalVar(G);

      lookupTy = G->getType();
      if (!lookupTy->isReferenceType() && !lookupTy->isUnknownAnyType())
         lookupTy = Context.getReferenceType(lookupTy);

      Ident->setIsTypeDependent(G->isTypeDependent());
   }
   else if (auto Arg = dyn_cast<FuncArgDecl>(lookup)) {
      assert(currentDeclCtx && "found argument through member lookup?");

      Ident->setFuncArg(Arg);
      if (lambdaScope) {
         Ident->setCaptureIndex(lambdaScope->getLambdaExpr()->addCapture(Arg));
         Ident->setKind(IdentifierKind::CapturedFunctionArg);
      }
      else {
         Ident->setKind(IdentifierKind::FunctionArg);
      }

      lookupTy = Arg->getType();
      if (!lookupTy->isReferenceType() && !lookupTy->isUnknownAnyType())
         lookupTy = Context.getReferenceType(lookupTy);

      Ident->setIsTypeDependent(Arg->isTypeDependent());
      Ident->setContainsUnexpandedParameterPack(
         Arg->isVariadicArgPackExpansion());
   }
   else if (isa<CallableDecl>(lookup)) {
      auto func = checkFunctionReference(Ident, Ident->getDeclCtx(),
                                         Ident->getIdentInfo(),
                                         Ident->getTemplateArgs());

      if (!func)
         return ExprError();

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
   else if (isa<AliasDecl>(lookup)) {
      auto AliasRes = checkAlias(lookupResult, Ident->getTemplateArgs(),
                                 Ident->getSourceLoc());

      if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
         Ident->setIsTypeDependent(AliasRes.isTypeDependent());
         Ident->setIsValueDependent(AliasRes.isValueDependent());

         return ExprError();
      }
      else if (!AliasRes) {
         Ident->setHadError(true);
         return ExprError();
      }

      Ident->setKind(IdentifierKind::Alias);
      Ident->setAliasVal(new(Context) Variant(AliasRes.getAlias()
                                                      ->getAliasExpr()
                                                      ->getEvaluatedExpr()));

      UsedTemplateParams = AliasRes.getAlias()->isInstantiation();
      lookupTy = AliasRes.getAlias()->getAliasExpr()->getExprType();
   }
   else if (auto NS = dyn_cast<NamespaceDecl>(lookup)) {
      Ident->setKind(IdentifierKind::Namespace);
      Ident->setNamespaceDecl(NS);

      lookupTy = Context.getNamespaceType(NS);
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
               Ident->setHadError(true);
               EncounteredError = true;
               diagnoseTemplateArgErrors(R, Ident, list, Compat);

               return ExprError();
            }
         }

         if (!lookupTy) {
            R = Instantiator.InstantiateRecord(Ident, R, move(list));
            lookupTy = Context.getRecordType(R);
         }
      }
      else {
         lookupTy = Context.getRecordType(R);
      }

      Ident->setKind(IdentifierKind::MetaType);
      Ident->setMetaType(Context.getMetaType(lookupTy));

      lookupTy = Ident->getMetaType();
   }
   else if (auto TD = dyn_cast<TypedefDecl>(lookup)) {
      if (!ensureDeclared(TD)) {
         return ExprError();
      }

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
      if (!AT->getActualType()) {
         Ident->setIsTypeDependent(true);
         return ExprError();
      }

      Ident->setKind(IdentifierKind::AssociatedType);
      lookupTy = Context.getMetaType(AT->getActualType());
   }
   else if (isa<EnumCaseDecl>(lookup)) {
      return HandleEnumCase(Ident, cast<EnumDecl>(R));
   }
   else if (auto Param = dyn_cast<TemplateParamDecl>(lookup)) {
      if (Param->isTypeName()) {
         lookupTy = Context.getMetaType(
            Context.getTemplateArgType(Param->getCovariance(),
                                       Param->getName()));

         Ident->setIsTypeDependent(true);
      }
      else {
         lookupTy = Param->getValueType();
         Ident->setIsValueDependent(true);
      }

      Ident->setContainsUnexpandedParameterPack(Param->isVariadic());
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
         Context, SourceRange(Ident->getSourceLoc()), Ident,
         Ident->getTemplateArgs());

      Ident->setSemanticallyChecked(true);

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
             || Ident->getContextualType()->isRawFunctionTy()
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
                               ? C->getName() : C->getLinkageName();

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

      return {};
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
   auto cl = getCurrentRecordCtx();
   if (!cl) {
      diagnose(Expr, err_self_outside_method, Expr->getSourceLoc(),
               0 /*self*/);

      return ExprError();
   }

   if (cl->isTemplate()) {
      Expr->setIsTypeDependent(true);
      return ExprError();
   }

   QualType type(Context.getRecordType(cl));

   auto fun = getCurrentFun();
   bool mutableSelf = false;

   if (auto m = dyn_cast_or_null<MethodDecl>(fun)) {
      mutableSelf = m->hasMutableSelf();
   }

   if (mutableSelf)
      type = Context.getReferenceType(type);

   Expr->setExprType(type);
   return Expr;
}

CallableDecl*
SemaPass::checkFunctionReference(Expression *E,
                                 DeclContext *Ctx,
                                 DeclarationName funcName,
                                 llvm::ArrayRef<Expression*> templateArgs) {
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

            Candidates.insert(Inst);
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

   std::vector<diag::DiagnosticBuilder> Diagnostics;

   for (auto ND : lookupResult) {
      auto alias = dyn_cast<AliasDecl>(ND);
      if (!alias || alias->getConstraints().size() < bestMatch)
         continue;

      if (!templateArgs.empty() || alias->isTemplate()) {
         TemplateArgList list(*this, alias, templateArgs);

         if (list.isStillDependent()) {
            typeDependent = true;
            continue;
         }
         else if (!list.checkCompatibility()) {
            continue;
         }

         alias = Instantiator.InstantiateAlias(alias, loc, move(list));
      }

      if (checkAlias(alias, Diagnostics)) {
         if (match) {
            diagnose(err_ambiguous_reference, loc, 2 /*alias*/, ND->getName());
            diagnose(note_candidate_here, match->getSourceLoc());
            diagnose(note_candidate_here, alias->getSourceLoc());

            return AliasResult();
         }

         match = alias;
      }
   }

   if (!match) {
      if (typeDependent || valueDependent)
         return AliasResult(typeDependent, valueDependent);

      return AliasResult();
   }

   visitScoped(match);
   if (match->isInvalid())
      return AliasResult();

   if (match->isDependent())
      return AliasResult(match->isTypeDependent(), match->isValueDependent());

   return AliasResult(match);
}

bool SemaPass::checkAlias(AliasDecl *Alias,
                          std::vector<diag::DiagnosticBuilder> &Diagnostics) {
   for (auto &C : Alias->getConstraints()) {
      auto res = evaluateAsBool(C);
      if (res.hadError())
         return false;

      if (!res.getValue().getZExtValue())
         return false;
   }

   return true;
}

IdentifierRefExpr *SemaPass::wouldBeValidIdentifier(SourceLocation Loc,
                                                    IdentifierInfo *maybeIdent) {
   DiagnosticScopeRAII diagnosticScopeRAII(*this);
   alignas(8) IdentifierRefExpr expr(Loc, maybeIdent);

   (void)visitExpr(&expr);
   if (!expr.foundResult())
      return nullptr;

   return new (getContext()) IdentifierRefExpr(move(expr));
}

GlobalVarDecl* SemaPass::getGlobalVariable(llvm::StringRef maybeGlobal)
{
   return getVariable(maybeGlobal);
}

ExprResult SemaPass::visitMemberRefExpr(MemberRefExpr *Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = getRValue(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   auto ty = ParentExpr->getExprType();
   if (ty->isUnknownAnyType())
      return ExprError();

   if (ty->isPointerType()) {
      if (!Expr->isPointerAccess())
         diagnose(Expr, err_access_member_on_pointer, Expr->getSourceLoc());

      ty = ty->getPointeeType();
   }
   else if (Expr->isPointerAccess()) {
      diagnose(Expr, err_member_access_non_pointer, ty, Expr->getSourceLoc());
   }

   QualType res;
   DeclContext *Ctx = nullptr;
   bool isStatic    = true;

   if (cdot::MetaType *Meta = ty->asMetaType()) {
      auto underlying = Meta->getUnderlyingType();

      if (underlying->isRecordType()) {
         Ctx = underlying->getRecord();
      }
      else {
         res = HandleStaticTypeMember(Expr, *underlying);
      }
   }
   else if (NamespaceType *NSTy = ty->asNamespaceType()) {
      Ctx = NSTy->getNamespace();
   }
   else if (ty->isRecordType()) {
      Ctx = ty->getRecord();
      isStatic = false;
   }
   else {
      return HandleBuiltinTypeMember(Expr, ty);
   }

   if (Ctx) {
      Expr->setSemanticallyChecked(true);

      auto *Ident = new(Context)
         IdentifierRefExpr(Expr->getSourceLoc(), Expr->getIdentInfo(),
                           move(Expr->getTemplateArgRef()), Ctx);

      Ident->setParentExpr(ParentExpr);
      Ident->copyStatusFlags(Expr);
      Ident->setStaticLookup(isStatic);
      Ident->setPointerAccess(Expr->isPointerAccess());
      Ident->setIsLHSOfAssignment(Expr->isLHSOfAssignment());

      return visitExpr(Expr, Ident);
   }
   else if (!res) {
      diagnose(Expr, err_access_member_on_type, Expr->getSourceLoc(), ty);
      return ExprError();
   }

   return Expr;
}

QualType SemaPass::HandleStaticTypeMember(MemberRefExpr *Expr, QualType Ty)
{
   if (Expr->getIdent() == "Type") {
      Expr->setMetaType(Ty);
      Expr->setKind(MemberKind::Type);

      return QualType(getObjectTy("cdot.TypeInfo"), true);
   }

   return UnknownAnyTy;
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

   QualType ty(F->getType());
   if (ty->isDependentType())
      return ExprError();

   // Use a getter if available
   if (F->hasGetter() && !Ident->isLHSOfAssignment() && !InOwningRecord) {
      Ident->setAccessorMethod(F->getGetterMethod());
      Ident->setExprType(F->getType());
      Ident->setKind(IdentifierKind::Accessor);

      return Ident;
   }

   // Use a setter if available (and we're on the left side of an assignment)
   if (F->hasSetter() && Ident->isLHSOfAssignment() && !InOwningRecord) {
      Ident->setAccessorMethod(F->getSetterMethod());
      Ident->setExprType(Context.getVoidType());
      Ident->setKind(IdentifierKind::Accessor);

      return Ident;
   }

   if (F->isStatic()) {
      Ident->setKind(IdentifierKind::GlobalVar);
      Ident->setStaticFieldDecl(F);
   }
   else {
      Ident->setKind(IdentifierKind::Field);
      Ident->setFieldDecl(F);
   }

   ty = QualType(Context.getReferenceType(ty));

   Ident->setExprType(ty);
   checkMemberAccessibility(F->getRecord(), F->getName(), F->getAccess(),
                            Ident);

   return Ident;
}

ExprResult SemaPass::HandlePropAccess(IdentifierRefExpr *Ident,
                                      PropDecl *P) {
   assert(P && "shouldn't be called otherwise");
   ensureDeclared(P);

   QualType ty;
   if (Ident->isLHSOfAssignment()) {
      if (!P->hasSetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*setter*/ 1);
      }
      else {
         Ident->setAccessorMethod(P->getSetterMethod());
      }

      ty = Context.getVoidType();
   }
   else {
      if (!P->hasGetter()) {
         diagnose(Ident, err_prop_does_not_have, Ident->getSourceLoc(),
                  P->getName(), /*getter*/ 0);
      }
      else {
         Ident->setAccessorMethod(P->getGetterMethod());
      }

      ty = P->getType();
   }

   if (ty->isDependentType())
      return ExprError();

   Ident->setKind(IdentifierKind::Accessor);
   Ident->setExprType(ty);

   maybeInstantiateMemberFunction(Ident->getAccessorMethod(), Ident);

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
   Expr->setExprType(QualType(Context.getReferenceType(contained)));

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
   for (const auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto res = visitExpr(Expr, arg);
         if (!res)
            return ExprError();
      }
   }

   auto CandSet = lookupCase(Expr->getIdentInfo(), E, Expr->getArgs(),
                             {}, Expr);

   if (!CandSet)
      return ExprError();

   auto Case = cast<EnumCaseDecl>(CandSet.getBestMatch().Func);

   PrepareCallArgs(Expr->getArgs(), Case->getFunctionType());
   ApplyCasts(Expr->getArgs(), CandSet);

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

   if (SubscriptedTy->isUnknownAnyType())
      return ExprError();

   if (!Expr->hasSingleIndex())
      diagnose(Expr, err_subscript_too_many_indices, Expr->getSourceLoc());

   if (!Expr->getIndices().empty()) {
      auto &idx = Expr->getIndices().front();
      auto IdxTy = idx->getExprType();

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

   QualType resultType;
   if (PointerType *Ptr = SubscriptedTy->asPointerType()) {
      resultType = Ptr->getPointeeType();
   }
   else if (ArrayType *Arr = SubscriptedTy->asArrayType()) {
      resultType = Arr->getElementType();
   }
   else {
      diagnose(Expr, err_illegal_subscript, Expr->getSourceLoc(),
               SubscriptedTy);

      return ExprError();
   }

   Expr->setExprType(Context.getReferenceType(resultType));
   return Expr;
}

void SemaPass::diagnoseMemberNotFound(ast::DeclContext *Ctx,
                                      Statement *Subject,
                                      llvm::StringRef memberName,
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

   diagnose(ErrorStmt, err_incompatible_template_args, selector,
            Template->getName());

   switch (Cand.ResultKind) {
      case sema::TemplateArgListResultKind::TLR_TooManyTemplateArgs: {
         auto neededSize = Template->getTemplateParams().size();
         auto givenSize  = list.getOriginalArgs().size();

         diagnose(ErrorStmt, note_too_many_template_args, neededSize,
                  givenSize, Template->getSourceLoc());

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

} // namespace ast
} // namespace cdot