//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"

#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"

#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Expression/RefExpr/MemberRefExpr.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/RefExpr/SubscriptExpr.h"
#include "AST/Expression/StaticExpr.h"
#include "AST/Expression/TypeRef.h"

#include "AST/Statement/Block/CompoundStmt.h"

#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/PropDecl.h"

#include "AST/ASTContext.h"
#include "AST/Transform.h"

#include "Variant/Type/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/Twine.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::sema;

namespace cdot {
namespace ast {

QualType SemaPass::VisitSubExpr(Expression *node, QualType ty)
{
   if (stopEvaluating(node))
      return ty;

   if (auto MemExpr = node->getSubExpr()) {
      if (auto M = dyn_cast<MemberRefExpr>(MemExpr)) {
         ty = visitMemberRefExpr(M, ty);
      }
      else if (auto C = dyn_cast<CallExpr>(MemExpr)) {
         ty = visitCallExpr(C, ty);
      }
      else {
         ty = visitSubscriptExpr(cast<SubscriptExpr>(MemExpr), ty);
      }

      if (MemExpr->hadError()) {
         node->setHadError(true);
         return UnknownAnyTy;
      }

      if (MemExpr->isTypeDependent()) {
         node->setIsTypeDependent(true);
         return UnknownAnyTy;
      }

      if (MemExpr->isValueDependent()) {
         node->setIsValueDependent(true);
         return UnknownAnyTy;
      }
   }
//   else if (ty && ty->isMetaType() && !node->isMetaTypeAllowed()) {
//      diagnose(node, err_generic_error, "invalid appearance of meta type "
//         "expression");
//   }
   else if (ty && ty->isNamespaceType()) {
      diagnose(node, err_generic_error, "invalid appearance of namespace");
   }

   return ty;
}

bool SemaPass::checkPossibleTemplateArgList(
                                 Expression *Expr,
                                 std::vector<TemplateArgExpr*> &templateArgs) {
   auto S = dyn_cast_or_null<SubscriptExpr>(Expr->getSubExpr());
   if (!S)
      return false;

   for (auto &Idx : S->getIndices()) {
      auto res = visitExpr(Expr, Idx);
      if (res.hadError())
         return false;

      TemplateArgExpr *TA = nullptr;
      auto ty = res.getType();

      if (ty->isMetaType()) {
         auto typeSrc = new(Context) TypeRef(ty->asMetaType()
                                               ->getUnderlyingType());
         TA = new(Context) TemplateArgExpr(typeSrc);
      }
      else {
         auto staticExpr = new(Context) StaticExpr(Idx);
         TA = new(Context) TemplateArgExpr(staticExpr);
      }

      TA->setSourceLoc(Idx->getSourceLoc());
      updateParent(TA, getParent(Idx));

      templateArgs.push_back(TA);
   }

   Expr->setSubExpr(S->getSubExpr());

   return true;
}

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
   llvm::StringRef dependentName;
   SourceLocation dependentLoc;

   for (auto curr = getParent(Expr); curr; curr = getParent(Expr)){
      if (auto F = dyn_cast<FieldDecl>(curr)) {
         dependentName = F->getName();
         dependentLoc = F->getSourceLoc();

         break;
      }
      if (auto GV = dyn_cast<GlobalVarDecl>(curr)) {
         dependentName = GV->getName();
         dependentLoc = GV->getSourceLoc();

         break;
      }
      if (auto GD = dyn_cast<GlobalDestructuringDecl>(curr)) {
         dependentName = GD->getFirstDecl()->getName();
         dependentLoc = GD->getSourceLoc();

         break;
      }
   }

   assert(dependentLoc && "dependent variable not found");

   diagnose(globalVar, err_generic_error,
            "type inferred global variables " + globalVar->getName() + " and "
            + dependentName + " are mutually dependent on "
               "each others value");
}

static bool anyDeclIsTemplate(LookupResult &lookupResult)
{
   for (auto &decl : lookupResult)
      if (decl->isTemplate())
         return true;

   return false;
}

QualType SemaPass::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   using IK = IdentifierRefExpr::IdentifierKind;

   if (node->getIdent() == "...") {
      node->setIsTypeDependent(true);
      return UnknownAnyTy;
   }

   if (node->getIdent() == "super") {
      if (auto ty = checkSuper(node)) {
         return ty;
      }

      return UnknownAnyTy;
   }

   if (node->getIdent() == "self") {
      if (auto ty = checkSelf(node)) {
         return ty;
      }

      return UnknownAnyTy;
   }

   if (node->getIdent() == "Self") {
      if (!getCurrentRecord()) {
         diagnose(node, err_self_outside_method, 2 /*Self*/);
         return UnknownAnyTy;
      }

      node->setIdent(string(getCurrentRecord()->getName()));
   }

   if (auto Param = hasTemplateParam(node->getIdent())) {
      if (Param->isTypeName()) {
         node->setIsTypeDependent(true);
         return UnknownAnyTy;
      }

      node->setIsValueDependent(true);
      return Param->getValueType()
               ? *Param->getValueType()->getType()
               : Context.getIntTy();
   }

   if (auto AT = hasAssociatedType(node->getIdent())) {
      if (!AT->getActualType()->getType()) {
         node->setIsTypeDependent(true);
         return UnknownAnyTy;
      }

      return Context.getMetaType(*AT->getActualType()->getType());
   }

   if (auto TA = getTemplateArg(node->getIdent())) {
      if (TA->isType()) {
         return Context.getMetaType(TA->getType());
      }

      llvm_unreachable("hmm");
//      return VisitSubExpr(node, TA->et);
   }

   auto MaybeBuiltin = HandleBuiltinIdentifier(node);
   if (MaybeBuiltin || node->isTypeDependent() || node->hadError())
      return MaybeBuiltin;

   auto lookupResult = declPass->getDeclContext().lookup(node->getIdent());
   if (!lookupResult) {
      llvm::SmallString<128> localVarName;
      localVarName += node->getIdent();

      auto initialSize = localVarName.size();
      for (auto scope = currentScope; scope;
           scope = scope->getEnclosingScope()) {
         if (auto BS = dyn_cast<BlockScope>(scope)) {
            localVarName += std::to_string(BS->getScopeID());

            lookupResult = declPass->getDeclContext().lookup(
               localVarName.str());
            if (lookupResult)
               break;

            localVarName.resize(initialSize);
         }
      }

      // implicit self
      if (auto R = getCurrentRecord()) {
         lookupResult = R->lookup(node->getIdent());
         if (lookupResult) {
            node->setKind(IK::Self);
            auto MemberExpr = new (getContext())
               MemberRefExpr(string(node->getIdent()));

            auto SubExpr = node->getSubExpr();
            MemberExpr->setSubExpr(SubExpr);

            node->setSubExpr(MemberExpr);

            return Context.getRecordType(R);
         }
      }

      if (auto ty = getBuiltinType(node->getIdent()))
         return Context.getMetaType(ty);

      if (!lookupResult) {
         diagnose(node, err_undeclared_identifer, node->getIdent());
         return UnknownAnyTy;
      }
   }

   NamedDecl *lookup = lookupResult.front();
   QualType lookupTy;
   if (auto Var = dyn_cast<LocalVarDecl>(lookup)) {
      node->setKind(IK::LocalVar);
      node->setLocalVar(Var);

      lookupTy = Var->getTypeRef()->getType();
   }
   else if (auto G = dyn_cast<GlobalVarDecl>(lookup)) {
      if (!G->getTypeRef()->getType()) {
         if (!VisitedGlobalDecls.insert(G).second) {
            diagnoseCircularlyDependentGlobalVariables(node, G);
            return UnknownAnyTy;
         }

         DeclPass::DeclScopeRAII declScopeRAII(*declPass,
                                               getNearestDeclContext(G));

         visitGlobalVarDecl(G);
         G->setGlobalOrdering(numGlobals++);
      }

      node->setKind(IK::GlobalVar);
      node->setGlobalVar(G);

      lookupTy = G->getTypeRef()->getType();
   }
   else if (auto Arg = dyn_cast<FuncArgDecl>(lookup)) {
      node->setKind(IK::FunctionArg);
      node->setFuncArg(Arg);

      lookupTy = Arg->getArgType()->getType();
   }
   else if (auto C = dyn_cast<CallableDecl>(lookup)) {
      lookupTy = checkFunctionReference(node, C);
      if (!lookupTy)
         return UnknownAnyTy;
   }
   else if (auto alias = dyn_cast<AliasDecl>(lookup)) {
      if (anyDeclIsTemplate(lookupResult)) {
         (void)checkPossibleTemplateArgList(node, node->getTemplateArgRef());
      }

      auto AliasRes = checkAlias(lookupResult, node->getTemplateArgs(),
                                 node->getSourceLoc());

      if (AliasRes) {
         node->setKind(IK::Alias);
         node->setExprType(AliasRes.getAlias()->getAliasExpr()->getExprType());
         node->setAliasVal(new(Context) Variant(AliasRes.getAlias()
                                                        ->getAliasExpr()
                                                        ->getEvaluatedExpr()));

         return node->getExprType();
      }
      else if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
         node->setIsTypeDependent(AliasRes.isTypeDependent());
         node->setIsValueDependent(AliasRes.isValueDependent());
      }
      else {
         node->setHadError(true);
         encounteredError = true;
         diagnostics.append(std::make_move_iterator(AliasRes.getDiagnostics()
                                                            .begin()),
                            std::make_move_iterator(AliasRes.getDiagnostics()
                                                            .end()));
      }

      return UnknownAnyTy;
   }
   else if (auto NS = dyn_cast<NamespaceDecl>(lookup)) {
      node->setKind(IK::Namespace);
      node->setNamespaceDecl(NS);

      lookupTy = Context.getNamespaceType(NS);
   }
   else if (auto R = dyn_cast<RecordDecl>(lookup)) {
      if (anyDeclIsTemplate(lookupResult)) {
         if (checkPossibleTemplateArgList(node, node->getTemplateArgRef())) {
            TemplateArgList list(*this, R, node->getTemplateArgs());
            R = InstantiateRecord(R, move(list), node->getSourceLoc());
         }
      }

      node->setKind(IK::MetaType);
      node->setMetaType(Context.getMetaType(Context.getRecordType(R)));

      lookupTy = node->getMetaType();
   }
   else if (auto TD = dyn_cast<TypedefDecl>(lookup)) {
      node->setKind(IK::MetaType);
      node->setMetaType(Context.getMetaType(*TD->getOriginTy()->getType()));

      lookupTy = node->getMetaType();
   }
   else {
      llvm_unreachable("unhandled named decl");
   }

   return lookupTy;
}

QualType SemaPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
{
   if (builtinIdentifiers.find(node->getIdent())
       == builtinIdentifiers.end()) {
      return {};
   }

   Variant *builtinValue = nullptr;
   cdot::Type *builtinType;

   auto kind = builtinIdentifiers[node->getIdent()];
   switch (kind) {
      case BuiltinIdentifier::NULLPTR: {
         if (node->getContextualType().isNull()) {
            diagnose(node, err_generic_error,
                     "__nullptr requires a contextual type");

            return UnknownAnyTy;
         }

         if (node->getContextualType()->isRefcounted()
             || node->getContextualType()->isRawFunctionTy()
             || node->getContextualType()->isPointerType())
            builtinType = *node->getContextualType();
         else
            builtinType = node->getContextualType()->getPointerTo(Context);

         break;
      }
      case BuiltinIdentifier::FUNC:
         builtinValue = new (getContext())
            Variant(string(getFuncScope()->getCallableDecl()->getName()));

         builtinType = getObjectTy("String");
         break;
      case BuiltinIdentifier::MANGLED_FUNC:
         builtinValue = new (getContext())
            Variant(string(getFuncScope()->getCallableDecl()
                                         ->getLinkageName()));
         builtinType = getObjectTy("String");
         break;
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

   node->setBuiltinValue(builtinValue);
   node->setBuiltinType(builtinType);
   node->setBuiltinKind(kind);
   node->setKind(IdentifierRefExpr::IdentifierKind::BuiltinValue);

   return builtinType;
}

QualType SemaPass::checkSuper(IdentifierRefExpr *node)
{
   if (!getCurrentRecord()) {
      diagnose(node, err_self_outside_method, 1 /*super*/);
      return {};
   }

   auto currentCl = dyn_cast<ClassDecl>(getCurrentRecord());
   if (!currentCl || currentCl->getParentClass() == nullptr) {
      diagnose(node, err_super_without_base, getCurrentRecord()->getName());
      return UnknownAnyTy;
   }

   return QualType(Context.getRecordType(currentCl));
}

QualType SemaPass::checkSelf(IdentifierRefExpr *node)
{
   if (!getCurrentRecord()) {
      diagnose(node, err_self_outside_method, 0 /*self*/);
      return UnknownAnyTy;
   }

   auto cl = getCurrentRecord();
   if (cl->isTemplate()) {
      node->setIsTypeDependent(true);
      return UnknownAnyTy;
   }

   QualType type(Context.getRecordType(cl));

   bool rawEnum = cl->isRawEnum();

   auto fun = getCurrentFun();
   bool mutableSelf = false;

   if (auto m = dyn_cast_or_null<MethodDecl>(fun)) {
      mutableSelf = m->hasMutableSelf() || isa<InitDecl>(m);
   }

   type.isLvalue(mutableSelf || rawEnum);

   return type;
}

QualType SemaPass::checkFunctionReference(IdentifierRefExpr *node,
                                          CallableDecl *F) {
   auto overloads = llvm::ArrayRef<CallableDecl*>(); //F->getOverloads();
   FunctionDecl* result = nullptr;
   auto ctx = node->getContextualType();
   auto fnTy = ctx->asFunctionType();

   for (const auto &overload : overloads) {
      bool compatible = false;
      if (fnTy) {
         auto given = overload->getFunctionType();
         if (given != fnTy)
            continue;

         compatible = true;
      }
      else {
         compatible = ctx.isNull();
      }

      if (compatible && result) {
         diagnose(node, err_generic_error,
                  "reference to function " + node->getIdent() + " is "
                     "ambiguous");

         break;
      }

      if (compatible)
         result = cast<FunctionDecl>(overload);
   }

   if (result) {
      node->setCallable(result);
      node->setKind(IdentifierRefExpr::IdentifierKind::Function);

      if (auto M = dyn_cast<MethodDecl>(result)) {
         auto args = result->getFunctionType()->getArgTypes();

         llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
         copy.insert(copy.begin(),
                     Context.getRecordType(M->getRecord()));

         return Context.getFunctionType(result->getFunctionType()
                                              ->getReturnType(),
                                        args, FunctionType::FunctionPtr);
      }
      else {
         return result->getFunctionType();
      }
   }

   return {};
}

SemaPass::AliasResult
SemaPass::checkAlias(LookupResult &lookupResult,
                     llvm::ArrayRef<TemplateArgExpr *> templateArgs,
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
            auto &diags = list.getDiagnostics();
            Diagnostics.insert(Diagnostics.end(),
                               std::make_move_iterator(diags.begin()),
                               std::make_move_iterator(diags.end()));

            continue;
         }

         alias = TemplateInstantiator::InstantiateAlias(*this, alias, loc,
                                                        move(list));
      }

      if (checkAlias(alias, Diagnostics)) {
         if (match) {
            Diagnostics.clear();
            Diagnostics.push_back(diag::err(err_generic_error)
               << "reference to alias " + ND->getName() + " is ambiguous"
               << loc);

            Diagnostics.push_back(diag::err(err_generic_error)
                                     << "candidate here"
                                     << match->getSourceLoc());

            Diagnostics.push_back(diag::err(err_generic_error)
                                     << "candidate here"
                                     << alias->getSourceLoc());

            return AliasResult(move(Diagnostics));
         }

         match = alias;
      }
   }

   if (!match) {
      if (typeDependent || valueDependent)
         return AliasResult(typeDependent, valueDependent);

      return AliasResult(move(Diagnostics));
   }

   visitAliasDecl(match);

   if (match->hadError())
      return AliasResult(move(Diagnostics));

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

IdentifierRefExpr *SemaPass::wouldBeValidIdentifier(llvm::StringRef maybeIdent)
{
   DiagnosticScopeRAII diagnosticScopeRAII(*this);
   IdentifierRefExpr expr(maybeIdent);
   visitIdentifierRefExpr(&expr);

   if (encounteredError)
      return nullptr;

   return new (getContext()) IdentifierRefExpr(move(expr));
}

GlobalVarDecl* SemaPass::getGlobalVariable(llvm::StringRef maybeGlobal)
{
   return declPass->getVariable(maybeGlobal);
}

QualType SemaPass::visitMemberRefExpr(MemberRefExpr *node, QualType ty)
{
   QualType res;

   if (ty->isPointerType()) {
      if (!node->isPointerAccess()) {
         diagnose(node, err_generic_error,
                  "cannot access value on pointer, did you mean to use '->'?");

         return UnknownAnyTy;
      }

      ty = ty->getPointeeType();
   }
   else if (node->isPointerAccess()) {
      diagnose(node, err_generic_error, "value is not a pointer");
   }

   if (auto Meta = ty->asMetaType()) {
      auto underlying = Meta->getUnderlyingType();

      if (underlying->isObjectType()) {
         res = HandleStaticMember(node, underlying->getRecord());
      }
      else {
         res = HandleStaticTypeMember(node, *underlying);
      }
   }
   else if (auto NSTy = ty->asNamespaceType()) {
      res = HandleNamespaceMember(node, NSTy->getNamespace());
   }
   else if (auto Tup = ty->asTupleType()) {
      res = HandleTupleMember(node, Tup);
   }

   else if (!ty->isObjectType()) {
      res = HandleTypeMember(node, *ty);

      if (!res && !node->isTypeDependent() && !node->hadError()) {
         diagnose(node, err_generic_error,
                  "cannot access member on value of type " + ty->toString());
      }
   }
   else {
      auto R = ty->getRecord();

      if (R->hasMethodWithName(node->getIdent()))
         res = HandleMethodReference(node, R);
      else if (auto U = dyn_cast<UnionDecl>(R)) {
         res = HandleUnionMember(node, U);
      }
      else if (auto E = dyn_cast<EnumDecl>(R)) {
         res = HandleEnumMember(node, E);
      }
      else if (auto P = R->getProperty(node->getIdent()))
         res = HandlePropAccess(node, R, P);
      else if (auto S = dyn_cast<StructDecl>(R))
         if (auto F = S->getField(node->getIdent()))
            res = HandleFieldAccess(node, S, F);

      if (!res && !node->isTypeDependent() && !node->hadError()) {
         diagnose(node, err_member_not_found, R->getNameSelector(),
                  R->getName(), node->getIdent());
      }
   }

   return res;
}

QualType SemaPass::HandleStaticMember(MemberRefExpr *node, RecordDecl *R)
{
   if (auto lookupResult = R->lookup(node->getIdent())) {
      if (lookupResult.size() != 1)
         llvm_unreachable("not yet");

      auto decl = lookupResult.front();
      if (auto F = dyn_cast<FieldDecl>(decl)) {
         if (!F->isStatic()) {
            diagnose(node, err_generic_error,
                     "non-static data member " + F->getName() + " can only "
                        "be accessed on instances of " + R->getName());

            return UnknownAnyTy;
         }
         if (!F->getType()->isResolved()) {
            if (!VisitedGlobalDecls.insert(F).second) {
               diagnoseCircularlyDependentGlobalVariables(node, F);
               return UnknownAnyTy;
            }

            DeclPass::DeclScopeRAII declScopeRAII(*declPass,
                                                  getNearestDeclContext(F));

            visitFieldDecl(F);
            F->setGlobalOrdering(numGlobals++);
         }

         return HandleFieldAccess(node, R, F);
      }
      else if (auto P = dyn_cast<PropDecl>(decl)) {
         if (!P->isStatic()) {
            diagnose(node, err_generic_error,
                     "non-static property " + P->getName() + " can only "
                        "be accessed on instances of " + R->getName());

            return UnknownAnyTy;
         }

         return HandlePropAccess(node, R, P);
      }
      else if (auto M = dyn_cast<MethodDecl>(decl)) {
         return HandleMethodReference(node, R);
      }
      else if (auto AT = dyn_cast<AssociatedTypeDecl>(decl)) {
         if (!AT->getActualType()->getType()) {
            node->setIsTypeDependent(true);
            return UnknownAnyTy;
         }

         node->setKind(MemberKind::AssociatedType);
         return QualType(Context.getMetaType(*AT->getActualType()->getType()));
      }
      else if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
         return HandleEnumCase(node, cast<EnumDecl>(R));
      }
      else if (auto Inner = dyn_cast<RecordDecl>(decl)) {
         node->setKind(MemberKind::Namespace);
         return Context.getMetaType(Context.getRecordType(Inner));
      }
      else if (auto alias = dyn_cast<AliasDecl>(decl)) {
         if (anyDeclIsTemplate(lookupResult)) {
            (void)checkPossibleTemplateArgList(node, node->getTemplateArgRef());
         }

         auto AliasRes = checkAlias(lookupResult, node->getTemplateArgs(),
                                    node->getSourceLoc());

         if (AliasRes) {
            node->setKind(MemberKind ::Alias);
            node->setExprType(AliasRes.getAlias()->getAliasExpr()->getExprType());
            node->setAliasVal(
               new(Context) Variant(AliasRes.getAlias()->getAliasExpr()
                                            ->getEvaluatedExpr()));

            return node->getExprType();
         }
         else if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
            node->setIsTypeDependent(AliasRes.isTypeDependent());
            node->setIsValueDependent(AliasRes.isValueDependent());
         }
         else {
            node->setHadError(true);
            encounteredError = true;
            diagnostics.append(std::make_move_iterator(AliasRes.getDiagnostics()
                                                               .begin()),
                               std::make_move_iterator(AliasRes.getDiagnostics()
                                                               .end()));
         }

         return UnknownAnyTy;
      }
      else {
         llvm_unreachable("unhandled named decl kind!");
      }
   }
   else if (node->getIdent() == "Type") {
      node->setMetaType(Context.getRecordType(R));
      node->setKind(MemberKind::Type);

      return QualType(getObjectTy("cdot.TypeInfo"), true, true);
   }

   return UnknownAnyTy;
}

QualType SemaPass::HandleStaticTypeMember(MemberRefExpr *node, Type *Ty)
{
   if (node->getIdent() == "Type") {
      node->setMetaType(Ty);
      node->setKind(MemberKind::Type);

      return QualType(getObjectTy("cdot.TypeInfo"), true, true);
   }

   return UnknownAnyTy;
}

QualType SemaPass::HandleEnumCase(MemberRefExpr *node, EnumDecl *E)
{
   if (E->hasCase(node->getIdent())) {
      auto enumCaseExpr =
         new (getContext()) EnumCaseExpr(string(node->getIdent()));

      enumCaseExpr->setEnum(E);
      enumCaseExpr->setContextualType(node->getContextualType());
      enumCaseExpr->setSourceLoc(node->getSourceLoc());
      enumCaseExpr->setSubExpr(node->getSubExpr());

      replaceExpressionWith(*this, node, enumCaseExpr);

      return visitEnumCaseExpr(enumCaseExpr);
   }

   diagnose(node, err_enum_case_not_found, node->getIdent(), 0);

   return UnknownAnyTy;
}

QualType SemaPass::HandleEnumMember(MemberRefExpr *node, EnumDecl *E)
{
   if (node->getIdent() == "rawValue") {
      node->setKind(MemberKind::EnumRawValue);

      return E->getRawType()->getType();
   }

   return HandleStaticMember(node, E);
}

QualType SemaPass::HandleUnionMember(MemberRefExpr *node, UnionDecl *U)
{
   if (auto F = U->getField(node->getIdent())) {
      QualType ty(F->getType()->getType());
      ty.isLvalue(true);
      ty.isConst(U->isConst());

      node->setFieldType(ty);
      node->setRecord(U);
      node->setKind(MemberKind::UnionAccess);

      return ty;
   }

   return HandleStaticMember(node, U);
}

QualType SemaPass::HandleTypeMember(MemberRefExpr *node, Type *Ty)
{
   return UnknownAnyTy;
}

QualType SemaPass::HandleNamespaceMember(MemberRefExpr *node, NamespaceDecl *NS)
{
   auto lookupResult = NS->lookup(node->getIdent());
   if (!lookupResult) {
      diagnose(node, err_generic_error,
               "namespace " + NS->getName() + " does not have a member named "
               + node->getIdent());
      return UnknownAnyTy;
   }

   assert(lookupResult.size() == 1);

   auto lookup = lookupResult.front();
   if (auto R = dyn_cast<RecordDecl>(lookup)) {
      node->setKind(MemberKind::Namespace);
      return { Context.getMetaType(Context.getRecordType(R)) };
   }
   else if (auto F = dyn_cast<FunctionDecl>(lookup)) {
      node->setKind(MemberKind::Function);
      node->setCallable(F);

      return F->getFunctionType();
   }
   else if (auto GV = dyn_cast<GlobalVarDecl>(lookup)) {
      node->setGlobalVar(GV);

      auto res = GV->getTypeRef()->getType();
      res.isLvalue(true);

      return res;
   }
   else if (auto TD = dyn_cast<TypedefDecl>(lookup)) {
      node->setKind(MemberKind::Namespace);
      return {Context.getMetaType(*TD->getOriginTy()->getType())};
   }
   else if (auto alias = dyn_cast<AliasDecl>(lookup)) {
      if (anyDeclIsTemplate(lookupResult)) {
         (void)checkPossibleTemplateArgList(node, node->getTemplateArgRef());
      }

      auto AliasRes = checkAlias(lookupResult, node->getTemplateArgs(),
                                 node->getSourceLoc());

      if (AliasRes) {
         node->setKind(MemberKind ::Alias);
         node->setExprType(AliasRes.getAlias()->getAliasExpr()->getExprType());
         node->setAliasVal(new(Context) Variant(AliasRes.getAlias()
                                                        ->getAliasExpr()
                                                        ->getEvaluatedExpr()));

         return node->getExprType();
      }
      else if (AliasRes.isTypeDependent() || AliasRes.isValueDependent()) {
         node->setIsTypeDependent(AliasRes.isTypeDependent());
         node->setIsValueDependent(AliasRes.isValueDependent());
      }
      else {
         node->setHadError(true);
         encounteredError = true;
         diagnostics.append(std::make_move_iterator(AliasRes.getDiagnostics()
                                                            .begin()),
                            std::make_move_iterator(AliasRes.getDiagnostics()
                                                            .end()));
      }

      return UnknownAnyTy;
   }
   else if (auto SubNS = dyn_cast<NamespaceDecl>(lookup)) {
      node->setKind(MemberKind::Namespace);
      return { Context.getNamespaceType(SubNS) };
   }
   else {
      llvm_unreachable("bad lookup result");
   }
}

QualType SemaPass::HandleTupleMember(MemberRefExpr *node, TupleType *tup)
{
   if (node->isTupleAccess()) {
      if (tup->getArity() <= node->getTupleIndex()) {
         diagnose(node, err_generic_error,
                  "cannot access index "
                  + std::to_string(node->getTupleIndex())
                  + " on tuple with arity "
                  + std::to_string(tup->getArity()));

         return UnknownAnyTy;
      }

      QualType ty(tup->getContainedType(node->getTupleIndex()));
      ty.isLvalue(true);

      node->setKind(MemberKind::TupleAccess);

      return ty;
   }

   return UnknownAnyTy;
}

QualType SemaPass::HandleFieldAccess(MemberRefExpr *node,
                                     RecordDecl* R, FieldDecl *F)
{
   if (!F)
      F = R->getField(node->getIdent());

   // Use a getter if available
   if (F->hasGetter() && !node->isLhsOfAssignment() && getCurrentRecord() != R) {
      node->setAccessorMethod(F->getGetterMethod());
      node->setFieldType(QualType(F->getType()->getType()));
      node->setKind(MemberKind::Accessor);

      return node->getFieldType();
   }

   // Use a setter if available (and we're on the left side of an assignment)
   if (F->hasSetter() && node->isLhsOfAssignment() && getCurrentRecord() != R) {
      node->setAccessorMethod(F->getSetterMethod());
      node->setFieldType(Context.getVoidType());
      node->setKind(MemberKind::Accessor);

      return node->getFieldType();
   }

   QualType ty(F->getType()->getType());
   ty.isLvalue(true);
   ty.isConst(F->isConst());

   // if we're in the classes initializer, all fields are non-const
   auto fun = getCurrentFun();
   if (fun) {
      auto Init = dyn_cast<InitDecl>(fun);
      if (Init && Init->getRecord() == R) {
         ty.isConst(false);
      }
   }

   if (F->isStatic()) {
      node->setKind(MemberKind::GlobalVariable);
      node->setFieldDecl(F);
   }
   else {
      node->setKind(MemberKind::Field);
   }

   node->setFieldType(ty);
   checkMemberAccessibility(R, F->getName(), F->getAccess(), node);

   return node->getFieldType();
}

QualType SemaPass::HandlePropAccess(MemberRefExpr *node, RecordDecl *rec,
                                    PropDecl *P) {
   if (!P)
      P = rec->getProperty(node->getIdent());

   assert(P && "shouldn't be called otherwise");

   QualType ty;
   if (node->isLhsOfAssignment()) {
      if (!P->hasSetter()) {
         diagnose(node, err_generic_error,
                  "property " + node->getIdent() + "does not have a setter");
      }
      else {
         node->setAccessorMethod(P->getSetterMethod());
      }

      ty = Context.getVoidType();
   }
   else {
      if (!P->hasGetter()) {
         diagnose(node, err_generic_error,
                  "property " + node->getIdent() + "does not have a getter");
      }
      else {
         node->setAccessorMethod(P->getGetterMethod());
      }

      ty = P->getType()->getType();
   }

   node->setKind(MemberKind::Accessor);
   node->setFieldType(ty);

   return ty;
}

QualType SemaPass::HandleMethodReference(MemberRefExpr *node, RecordDecl *R)
{
   auto M = R->getMethod(node->getIdent());
   if (!M)
      return UnknownAnyTy;

   auto overloads = llvm::ArrayRef<MethodDecl*>(M);// M->getOverloads();

   MethodDecl* result = nullptr;
   auto ctx = node->getContextualType();
   auto fnTy = ctx->asFunctionType();

   for (auto overload : overloads) {
      bool compatible = false;

      if (fnTy) {
         auto given = overload->getFunctionType();
         if (given != fnTy)
            continue;

         compatible = true;
      }
      else {
         compatible = ctx.isNull();
      }

      if (compatible && result) {
         diagnose(node, err_generic_error,
                  "reference to method " + node->getIdent() + " is "
                     "ambiguous");

         break;
      }

      if (compatible)
         result = cast<MethodDecl>(overload);
   }

   if (result) {
      node->setCallable(result);
      node->setKind(MemberKind::Function);

      auto args = result->getFunctionType()->getArgTypes();
      llvm::SmallVector<QualType, 8> copy(args.begin(), args.end());
      copy.insert(copy.begin(), Context.getRecordType(R));

      return Context.getFunctionType(result->getReturnType()->getType(), args,
                                     ctx->asFunctionType()->getRawFlags());
   }

   return UnknownAnyTy;
}

QualType SemaPass::visitEnumCaseExpr(EnumCaseExpr *node)
{
   if (!node->getEnum()) {
      auto ty = node->getContextualType();
      if (!ty) {
         diagnose(node, err_generic_error,
                  "unqualified enum case requires a contextual type");

         return UnknownAnyTy;
      }

      if (!ty->isObjectType()) {
         diagnose(node, err_generic_error,
                  ty->toString() + " is not a valid enumeration type");

         return UnknownAnyTy;
      }

      auto rec = ty->getRecord();
      if (!isa<EnumDecl>(rec)) {
         diagnose(node, err_generic_error,
                  ty->toString() + " is not a valid enumeration type");

         return UnknownAnyTy;
      }

      node->setEnum(cast<EnumDecl>(rec));
   }

   auto en = node->getEnum();
   if (!en->hasCase(node->getIdent())) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getIdent(), false);

      return UnknownAnyTy;
   }

   auto &args = node->getArgs();
   for (const auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto res = visitExpr(node, arg);
         if (res.hadError())
            return UnknownAnyTy;
      }
   }

   auto res = getCase(en, node->getIdent(), node->getArgs(), node);
   if (!res->isCompatible()) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getIdent(), !args.empty());

      return UnknownAnyTy;
   }

   PrepareCallArgs(node->getArgs(), res->getCase()->getFunctionType());
   ApplyCasts(node->getArgs(), res->getCase()->getFunctionType());

   return QualType(Context.getRecordType(en));
}

QualType SemaPass::visitSubscriptExpr(SubscriptExpr *node, QualType ts)
{
   for (auto &Idx : node->getIndices()) {
      if (!ts->isObjectType())
         Idx->setContextualType(Context.getIntTy());

      auto indexResult = visitExpr(node, Idx);
      if (indexResult.hadError())
         return { };
   }


   if (ts->isObjectType()) {
      auto call = new (getContext()) CallExpr(std::move(node->getIndicesRef()),
                                              "postfix []");

      replaceExpressionWith(*this, node, call);
      return visitCallExpr(call, ts);
   }

   if (!node->hasSingleIndex()) {
      diagnose(node, err_generic_error, "expected one subscript index");
      if (node->getIndices().empty())
         return UnknownAnyTy;
   }

   QualType index = node->getIndices().front()->getExprType();

   QualType resultType;
   if (auto Ptr = ts->asPointerType()) {
      resultType = Ptr->getPointeeType();
   }
   else if (auto Arr = ts->asArrayType()) {
      resultType = Arr->getElementType();
   }
   else {
      diagnose(node, err_illegal_subscript, ts);
      return UnknownAnyTy;
   }

   forceCast(node->getIndices().front(), index, Context.getIntTy());

   resultType.isLvalue(true);
   return resultType;
}

void SemaPass::diagnoseMemberNotFound(ast::DeclContext *Ctx,
                                      Statement *Subject,
                                      llvm::StringRef memberName,
                                      llvm::StringRef hint) {
   using llvm::Twine;

   if (isa<TranslationUnit>(Ctx)) {
      if (hint.empty())
         diagnose(Subject, err_generic_error,
                  Twine("reference to undeclared identifier ") + memberName);
      else
         diagnose(Subject, err_generic_error,
                  Twine(hint) + " " + memberName + " not found");
   }
   else
      diagnose(Subject, err_generic_error,
               Ctx->getSpecifierForDiagnostic() + " does not have a "
                  "member named " + memberName);
}

} // namespace ast
} // namespace cdot