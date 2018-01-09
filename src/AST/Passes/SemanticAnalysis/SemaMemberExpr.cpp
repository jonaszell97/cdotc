//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"
#include "TemplateInstantiator.h"

#include <llvm/Support/raw_ostream.h>

#include "../ASTIncludes.h"

#include "../../../Util.h"
#include "../../SymbolTable.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/MetaType.h"
#include "../../../Variant/Type/ArrayType.h"

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"
#include "../Declaration/DeclPass.h"
#include "../../../Variant/Type/TypeGroup.h"
#include "../StaticExpr/StaticExprEvaluator.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::cl;
using namespace cdot::sema;

namespace cdot {
namespace ast {

QualType SemaPass::VisitSubExpr(Expression *node, QualType ty)
{
   if (stopEvaluating(node))
      return {};

   if (auto MemExpr = node->getMemberExpr()) {
      if (auto M = dyn_cast<MemberRefExpr>(MemExpr.get())) {
         ty = visitMemberRefExpr(M, ty);
      }
      else if (auto C = dyn_cast<CallExpr>(MemExpr.get())) {
         ty = visitCallExpr(C, ty);
      }
      else {
         ty = visitSubscriptExpr(cast<SubscriptExpr>(MemExpr.get()), ty);
      }

      if (MemExpr->hadError()) {
         node->setHadError(true);
         return {};
      }

      if (MemExpr->isTypeDependant()) {
         node->setIsTypeDependent(true);
         return {};
      }

      if (MemExpr->isValueDependant()) {
         node->setIsValueDependent(true);
         return {};
      }
   }

   return ty;
}

void SemaPass::checkClassAccessibility(cdot::cl::Record *cl,
                                       Expression *cause) {
   if (!cl->isPrivate())
      return;

   return;
   for (const auto &NS : declPass->getCurrentNamespaceVec()) {
      if (SymbolTable::getNamespace(NS) == cl->getDeclarationNamespace())
         return;
   }

   diagnose(cause, err_class_not_accessible, cl->getNameSelector(),
            cl->getName());
}

void SemaPass::checkMemberAccessibility(cdot::cl::Record *record,
                                        const string& memberName,
                                        const AccessModifier &access,
                                        Expression *cause) {
   if (access == AccessModifier::PROTECTED
       && !record->protectedPropAccessibleFrom(currentClass())) {
      diagnose(cause, err_protected_member_access, memberName,
               record->getNameSelector(), record->getName());
   }
   else if (access == AccessModifier::PRIVATE
            && !record->privatePropAccessibleFrom(currentClass())) {
      diagnose(cause, err_private_member_access, memberName,
               record->getNameSelector(), record->getName());
   }
}

QualType SemaPass::visitIdentifierRefExpr(IdentifierRefExpr *node)
{
   if (node->isLetExpr() || node->isVarExpr())
      diag::err(err_unexpected_let_expr) << node << diag::term;

   if (node->getIdent() == "...") {
      node->setIsTypeDependent(true);
      return {};
   }

   if (node->getIdent() == "super")
      return trySuper(node);

   if (node->getIdent() == "self")
      return trySelf(node, true);

   if (auto Param = hasTemplateParam(node->getIdent())) {
      if (Param->isTypeName()) {
         node->setIsTypeDependent(true);
         return {};
      }

      node->setIsValueDependent(true);
      return VisitSubExpr(node, Param->valueType ? Param->valueType
                                                 : (Type*)IntegerType::get());
   }

   if (auto AT = hasAssociatedType(node->getIdent())) {
      if (!AT->getType()) {
         node->setIsTypeDependent(true);
         return {};
      }

      return VisitSubExpr(node, MetaType::get(*AT->getType()));
   }

   if (auto TA = getTemplateArg(node->getIdent())) {
      if (TA->isType()) {
         return VisitSubExpr(node, MetaType::get(TA->getType()));
      }

      return VisitSubExpr(node, QualType(TA->getValue().typeOf()));
   }

   if (hasVariable(node->getIdent())) {
      auto res = getVariable(node->getIdent(), node);

      // add capture for this variable
      if (res.escapesLambdaScope) {
         node->setIsCaptured(true);
         node->setCapturedValue(res.V->decl);
         currentScope()->captures->insert(res.V->decl);
      }

      node->setBinding(res.V->name);

      if (res.V->type->isDependantType()) {
         node->setIsTypeDependent(true);
         return { };
      }

      return VisitSubExpr(node, res.V->type);
   }

   auto MaybeArg = tryFunctionArg(node);
   if (MaybeArg || node->isTypeDependant() || node->hadError())
      return MaybeArg;

   auto MaybeBuiltin = HandleBuiltinIdentifier(node);
   if (MaybeBuiltin || node->isTypeDependant() || node->hadError())
      return MaybeBuiltin;

   auto MaybeFunc = tryFunctionReference(node);
   if (MaybeFunc || node->isTypeDependant() || node->hadError())
      return MaybeFunc;

   auto MaybeAlias = tryAlias(node->getIdent(), node->getTemplateArgs(), node);
   if (!MaybeAlias.isVoid() || node->isTypeDependant() || node->hadError()) {
      node->setAliasVal(std::move(MaybeAlias));
      return VisitSubExpr(node, QualType(node->getAliasVal().typeOf()));
   }

   if (auto ty = resolveNamespaceReference(node))
      return VisitSubExpr(node, QualType(ty));

   auto MaybeSelf = trySelf(node, false);
   if (MaybeSelf || node->isTypeDependant() || node->hadError())
      return MaybeSelf;

   diagnose(node, err_undeclared_identifer, node->getIdent());
   return {};
}

QualType SemaPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
{
   if (builtinIdentifiers.find(node->getIdent())
       == builtinIdentifiers.end()) {
      return {};
   }

   Variant builtinValue;
   cdot::Type *builtinType;

   auto kind = builtinIdentifiers[node->getIdent()];
   switch (kind) {
      case BuiltinIdentifier::NULLPTR: {
         if (node->getContextualType()->isAutoTy()) {
            diagnose(node, err_generic_error,
                     "__nullptr requires a contextual type");

            return {};
         }

         if (node->getContextualType()->isRefcounted()
             || node->getContextualType()->isRawFunctionTy()
             || node->getContextualType()->isPointerTy())
            builtinType = *node->getContextualType();
         else
            builtinType = node->getContextualType()->getPointerTo();

         break;
      }
      case BuiltinIdentifier::FUNC:
         builtinValue = Variant(string(currentScope()->function->getName()));
         builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::MANGLED_FUNC:
         builtinValue = Variant(string(currentScope()->function
                                                    ->getLinkageName()));
         builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::FLOAT_QNAN:
      case BuiltinIdentifier::FLOAT_SNAN:
         builtinType = FPType::getFloatTy();
         break;
      case BuiltinIdentifier::DOUBLE_QNAN:
      case BuiltinIdentifier::DOUBLE_SNAN:
         builtinType = FPType::getDoubleTy();
         break;
      case BuiltinIdentifier::__ctfe:
         builtinType = IntegerType::getBoolTy();
         break;
   }

   node->setBuiltinValue(builtinValue);
   node->setBuiltinType(builtinType);
   node->setBuiltinKind(kind);

   return VisitSubExpr(node, QualType(builtinType));
}

QualType SemaPass::trySuper(IdentifierRefExpr *node)
{
   if (node->getIdent() != "super")
      return {};

   if (!currentClass()) {
      diagnose(node, err_self_outside_method, 1 /*super*/);
      return {};
   }

   auto currentCl = dyn_cast<Class>(currentClass());
   if (!currentCl || currentCl->getParent() == nullptr) {
      diagnose(node, err_super_without_base, currentClass()->getName());
      return {};
   }

   node->isSuper(true);

   auto type = QualType(ObjectType::get(currentCl));
   return VisitSubExpr(node, type);
}

QualType SemaPass::trySelf(IdentifierRefExpr *node, bool guaranteedSelf)
{
   bool implicitSelf = false;
   if (!guaranteedSelf) {
      implicitSelf = !hasVariable(node->getIdent())
                     && currentClass() != nullptr;
   }

   if (!guaranteedSelf && !implicitSelf)
      return {};

   if (!currentClass()) {
      diagnose(node, err_self_outside_method, 0 /*self*/);
      return {};
   }

   auto cl = currentClass();
   if (cl->isTemplated()) {
      node->setIsTypeDependent(true);
      return {};
   }

   if (implicitSelf) {
      if (!cl->hasField(node->getIdent()) && !cl->hasProperty(node->getIdent()))
         return {};

      auto subExpr = std::make_shared<MemberRefExpr>(string(node->getIdent()));
      CopyNodeProperties(node, subExpr.get());

      subExpr->setMemberExpr(node->getMemberExpr());
      node->setMemberExpr(subExpr);
   }

   QualType type(ObjectType::get(cl->getName()));

   bool rawEnum = cl->isRawEnum();

   auto fun = getCurrentFunction();
   bool mutableSelf = false;

   if (fun && isa<cl::Method>(fun)) {
      auto m = cast<cl::Method>(fun);
      mutableSelf = m->hasMutableSelf() || m->isInitializer();
   }

   type.isLvalue(mutableSelf || rawEnum);
   type.isSelf(!rawEnum);

   auto res = VisitSubExpr(node, type);
   if (res) {
      node->setIdent("self");
      node->setIsSelf(true);
   }
   else if (implicitSelf) {
      node->setMemberExpr(node->getMemberExpr()->getMemberExpr());
   }

   return res;
}

QualType SemaPass::tryFunctionReference(IdentifierRefExpr *node)
{
   if (!getAnyFn(node->getIdent()))
      return {};

   auto overloads = getFunctionOverloads(node->getIdent());

   cdot::Function* result = nullptr;
   auto ctx = node->getContextualType();
   auto fnTy = dyn_cast<FunctionType>(*ctx);

   for (const auto &overload : overloads) {
      bool compatible = false;
      if (fnTy) {
         auto given = FunctionType::get(overload->getReturnType(),
                                        overload->getArguments(),
                                        fnTy->isRawFunctionTy());

         if (given != fnTy)
            continue;

         compatible = true;
      }
      else {
         compatible = ctx->isAutoTy();
      }

      if (compatible && result) {
         diagnose(node, err_generic_error,
                  "reference to function " + node->getIdent() + " is "
                     "ambiguous");

         break;
      }

      if (compatible)
         result = overload;
   }

   if (result) {
      node->setCallable(result);
      node->isFunction(true);

      return FunctionType::get(result->getReturnType(),
                               result->getArguments(),
                               ctx->isRawFunctionTy());
   }

   return {};
}

QualType SemaPass::tryFunctionArg(IdentifierRefExpr *node)
{
   auto curr = &Scopes.top();

   bool escapesLambdaScope = false;
   while (curr) {
      if (!curr->function) {
         curr = curr->enclosingScope;
         continue;
      }

      escapesLambdaScope |= curr->isLambdaRoot;
      size_t argNum = 0;

      for (const auto &arg : curr->function->getArguments()) {
         if (arg.label == node->getIdent()) {
            auto ty = arg.type;
            if (ty->isDependantType()) {
               for (auto &list : templateArgStack)
                  *ty = resolveDependencies(*ty, *list);
            }

            node->setIsTypeDependent(ty->isDependantType());
            node->setFunctionArg(true);
            node->setArgNo(argNum);

            QualType argTy(*ty, ty.isLvalue() || !ty.isConst());

            if (escapesLambdaScope) {
               auto decl = curr->function->getDeclaration()
                               ->getArgs()[argNum].get();

               currentScope()->captures->insert(decl);
               node->setCapturedValue(decl);
               node->setIsCaptured(true);
            }

            return VisitSubExpr(node, argTy);
         }

         ++argNum;
      }

      curr = curr->enclosingScope;
   }

   return {};
}

Type *SemaPass::resolveNamespaceReference(IdentifierRefExpr *node)
{
   string PossibleNamespace;
   if (node->getIdent() == "Self") {
      if (!currentClass()) {
         diagnose(node, err_self_outside_method, 2 /*Self*/);
         return AutoType::get();
      }

      PossibleNamespace = currentClass()->getName();
   }
   else {
      PossibleNamespace = node->getIdent();
   }

   if (auto td = getTypedef(PossibleNamespace)) {
      if (td->aliasedType->isObjectTy()) {
         PossibleNamespace = td->aliasedType->getClassName();
      }
   }

   if (auto R = currentClass()) {
      if (auto AT = R->getAssociatedType(PossibleNamespace))
         if (AT->getType()->isObjectTy())
            PossibleNamespace = AT->getType()->getClassName();
   }

   if (auto R = getRecord(PossibleNamespace)) {
      if (R->isTemplated()) {
         TemplateArgList list(*this, R, node->getTemplateArgs());
         if (!list.checkCompatibility()) {
            issueDiagnostics(list);
            node->setHadError(true);

            return nullptr;
         }

         R = TemplateInstantiator::InstantiateRecord(*this,
                                                     node->getSourceLoc(),
                                                     R, std::move(list));
      }

      node->isNamespace(true);

      return MetaType::get(ObjectType::get(R));
   }

   if (auto NS = declPass->isNamespace(PossibleNamespace)) {
      node->isNamespace(true);
      return NamespaceType::get(SymbolTable::getNamespace(NS));
   }

   return nullptr;
}

Variant SemaPass::tryAlias(llvm::StringRef ident,
                           llvm::ArrayRef<TemplateArg> templateArgs,
                           Expression *expr) {
   auto aliases = getAliases(ident);
   if (aliases.empty())
      return {};

   llvm::SmallVector<llvm::SmallVector<DiagnosticBuilder, 4>, 4>
      failedCandidates;

   size_t bestMatch = 0;
   bool typeDependant = false;
   cdot::Alias *match = nullptr;
   TemplateArgList bestMatchTemplateArgs;

   for (auto &alias : aliases) {
      if (alias->getConstraints().size() < bestMatch)
         continue;

      TemplateArgList list(*this, alias, templateArgs);
      if (list.isStillDependant()) {
         typeDependant = true;
         continue;
      }
      if (!list.checkCompatibility()) {
         failedCandidates.emplace_back(std::move(list.getDiagnostics()));
         continue;
      }

      bool satisfied = true;

      StaticExprEvaluator Eval(*this, &list);
      for (auto &C : alias->getConstraints()) {
         auto Inst =
            TemplateInstantiator::InstantiateAlias(*this, alias,
                                                   expr->getSourceLoc(),
                                                   C, list);

         auto res = Eval.evaluate(Inst.get());
         if (res.typeDependant)
            return { };

         if (res.hadError) {
            for (auto &diag : res.diagnostics)
               diagnostics.emplace_back(std::move(diag));

            encounteredError = true;
            expr->setHadError(true);

            return { };
         }

         if (!res.val.intVal) {
            satisfied = false;
            break;
         }
      }

      if (!satisfied)
         continue;

      bestMatch = alias->getConstraints().size();
      match = alias;
      bestMatchTemplateArgs = std::move(list);
   }

   if (!match) {
      if (typeDependant) {
         expr->setIsTypeDependent(true);
         return {};
      }

      diagnose(expr, err_generic_error,
               "no match for alias " + ident + " found");

      for (auto &diags : failedCandidates) {
         for (auto &diag : diags)
            diag << diag::cont;
      }

      expr->setHadError(true);
      return {};
   }

   StaticExprEvaluator Eval(*this, &bestMatchTemplateArgs);

   visitAliasDecl(match->getDecl());
   if (!match->isTypeDependant())
      return match->getVal();

   auto Inst =
      TemplateInstantiator::InstantiateAlias(*this, match, expr->getSourceLoc(),
                                             match->getAliasExpr(),
                                             bestMatchTemplateArgs);

   auto res = Eval.evaluate(Inst.get());
   if (res.typeDependant) {
      expr->setIsTypeDependent(true);
      return { };
   }

   if (res.hadError) {
      for (auto &diag : res.diagnostics)
         diagnostics.emplace_back(std::move(diag));

      encounteredError = true;
      expr->setHadError(true);

      return { };
   }

   return res.val;
}

bool SemaPass::wouldBeValidIdentifier(llvm::StringRef maybeIdent)
{
   if (hasVariable(maybeIdent))
      return true;

   if (auto P = hasTemplateParam(maybeIdent))
      return !P->isTypeName();

   if (auto AT = hasAssociatedType(maybeIdent))
      return !AT->getType()->isAutoTy();

   if (auto Func = currentScope()->function) {
      for (const auto &arg : Func->getArguments())
         if (maybeIdent.equals(arg.label))
            return true;
   }

   if (builtinIdentifiers.find(maybeIdent) != builtinIdentifiers.end())
      return true;

   if (getAnyFn(maybeIdent))
      return true;

   if (auto Rec = currentClass()) {
      if (Rec->hasProperty(maybeIdent))
         return true;

      if (auto Str = dyn_cast<Struct>(Rec))
         return Str->hasField(maybeIdent);
   }

   return false;
}

Variable* SemaPass::getGlobalVariable(llvm::StringRef maybeGlobal)
{
   return declPass->getVariable(maybeGlobal);
}

QualType SemaPass::visitMemberRefExpr(MemberRefExpr *node, QualType ty)
{
   if (node->getIdent().empty() && !node->isTupleAccess()) {
      llvm::outs() << "EMPTY IDENT\n";
       int i=3; //CBP
   }
   QualType res;

   if (ty->isPointerTy()) {
      if (!node->isPointerAccess()) {
         diagnose(node, err_generic_error,
                  "cannot access value on pointer, did you mean to use '->'?");

         return {};
      }

      ty = ty->getPointeeType();
   }
   else if (node->isPointerAccess()) {
      diagnose(node, err_generic_error, "value is not a pointer");
   }

   if (auto Meta = dyn_cast<MetaType>(*ty)) {
      auto underlying = Meta->getUnderlyingType();

      if (underlying->isObjectTy()) {
         res = HandleStaticMember(node, underlying->getRecord());
      }
      else {
         res = HandleStaticTypeMember(node, underlying);
      }
   }
   else if (auto NSTy = dyn_cast<NamespaceType>(*ty)) {
      res = HandleNamespaceMember(node, NSTy->getNamespace());
   }
   else if (auto Tup = dyn_cast<TupleType>(*ty)) {
      res = HandleTupleMember(node, Tup);
   }

   else if (!ty->isObjectTy()) {
      res = HandleTypeMember(node, *ty);

      if (!res && !node->isTypeDependant() && !node->hadError()) {
         diagnose(node, err_generic_error,
                  "cannot access member on value of type " + ty->toString());
      }
   }
   else {
      auto R = ty->getRecord();

      if (R->hasMethodWithName(node->getIdent()))
         res = HandleMethodReference(node, R);
      else if (auto U = dyn_cast<Union>(R)) {
         res = HandleUnionMember(node, U);
      }
      else if (auto E = dyn_cast<Enum>(R)) {
         res = HandleEnumMember(node, E);
      }
      else if (R->hasProperty(node->getIdent()))
         res = HandlePropAccess(node, R);
      else if (auto S = dyn_cast<Struct>(R))
         if (S->hasField(node->getIdent()))
            res = HandleFieldAccess(node, S);

      if (!res && !node->isTypeDependant() && !node->hadError()) {
         diagnose(node, err_member_not_found, R->getNameSelector(),
                  R->getName(), node->getIdent());
      }
   }

   return VisitSubExpr(node, res);
}

QualType SemaPass::HandleStaticMember(MemberRefExpr *node, cl::Record *R)
{
   if (auto F = R->getField(node->getIdent())) {
      if (!F->isIsStatic()) {
         diagnose(node, err_generic_error,
                  "non-static data member " + F->getFieldName() + " can only "
                     "be accessed on instances of " + R->getName());

         return {};
      }

      return HandleFieldAccess(node, R, F);
   }
   else if (auto P = R->getProperty(node->getIdent())) {
      if (!P->isStatic()) {
         diagnose(node, err_generic_error,
                  "non-static property " + P->getName() + " can only "
                     "be accessed on instances of " + R->getName());

         return {};
      }

      return HandlePropAccess(node, R, P);
   }
   else if (auto AT = R->getAssociatedType(node->getIdent())) {
      if (!AT->getType()) {
         node->setIsTypeDependent(true);
         return {};
      }

      node->setKind(MemberKind::AssociatedType);
      return QualType(MetaType::get(*AT->getType()));
   }
   else if (R->hasMethodWithName(node->getIdent())) {
      return HandleMethodReference(node, R);
   }
   else if (node->getIdent() == "Type") {
      node->setMetaType(ObjectType::get(R));
      node->setKind(MemberKind::Type);

      return QualType(ObjectType::get("cdot.TypeInfo"), true, true);
   }
   else if (auto E = dyn_cast<Enum>(R)) {
      return HandleEnumCase(node, E);
   }

   return {};
}

QualType SemaPass::HandleStaticTypeMember(MemberRefExpr *node, Type *Ty)
{
   if (node->getIdent() == "Type") {
      node->setMetaType(Ty);
      node->setKind(MemberKind::Type);

      return QualType(ObjectType::get("cdot.TypeInfo"), true, true);
   }

   return {};
}

QualType SemaPass::HandleEnumCase(MemberRefExpr *node, cl::Enum *E)
{
   if (E->hasCase(node->getIdent())) {
      auto enumCaseExpr =
         std::make_shared<EnumCaseExpr>(string(node->getIdent()));

      enumCaseExpr->setEnum(E);
      enumCaseExpr->setContextualType(node->getContextualType());
      enumCaseExpr->setSourceLoc(node->getSourceLoc());
      enumCaseExpr->setMemberExpr(node->getMemberExpr());

      node->getParentExpr()->setMemberExpr(enumCaseExpr);

      return visitEnumCaseExpr(enumCaseExpr.get());
   }

   diagnose(node, err_enum_case_not_found, node->getIdent(), 0);

   return {};
}

QualType SemaPass::HandleEnumMember(MemberRefExpr *node, cl::Enum *E)
{
   if (node->getIdent() == "rawValue") {
      node->setKind(MemberKind::EnumRawValue);

      return QualType(E->getRawType());
   }

   return HandleStaticMember(node, E);
}

QualType SemaPass::HandleUnionMember(MemberRefExpr *node, cl::Union *U)
{
   if (U->hasField(node->getIdent())) {
      QualType ty(U->getFieldTy(node->getIdent()));
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
   return {};
}

QualType SemaPass::HandleNamespaceMember(MemberRefExpr *node, Namespace *NS)
{
   llvm::SmallString<128> qualifiedName;
   qualifiedName += NS->getName();
   qualifiedName += ".";
   qualifiedName += node->getIdent();

   auto lookup = SymbolTable::lookup(qualifiedName.str(),
                                     declPass->importedNamespaces(),
                                     declPass->currentNamespace);

   using Kind = SymbolTable::LookupResult::LookupResultKind;

   switch (lookup.kind) {
      case Kind::LRK_Nothing:
         break;
      case Kind::LRK_Record:
      case Kind::LRK_Class:
      case Kind::LRK_Enum:
      case Kind::LRK_Struct:
      case Kind::LRK_Union:
      case Kind::LRK_Protocol: {
         auto R = lookup.Entry->getRecord();
         if (R->isTemplated()) {
            TemplateArgList list(*this, R, node->getTemplateArgs());
            if (!list.checkCompatibility()) {
               issueDiagnostics(list);
               node->setHadError(true);

               return nullptr;
            }

            R = TemplateInstantiator::InstantiateRecord(*this,
                                                        node->getSourceLoc(),
                                                        R, std::move(list));
         }

         node->setKind(MemberKind::Namespace);
         return { MetaType::get(ObjectType::get(R)) };
      }
      case Kind::LRK_Function: {
         llvm_unreachable("todo");
      }
      case Kind::LRK_GlobalVariable: {
         auto &G = *lookup.Entry->getVariable();
         node->setBinding(G.getName());

         auto res = G.getType();
         res.isLvalue(true);

         node->setKind(MemberKind::GlobalVariable);

         return VisitSubExpr(node, res);
      }
      case Kind::LRK_Typedef:
         node->setKind(MemberKind::Namespace);
         return { MetaType::get(lookup.Entry->getTypedef()->aliasedType) };
      case Kind::LRK_Alias: {
         auto res = tryAlias(node->getIdent(), node->getTemplateArgs(), node);

         node->setKind(MemberKind::Alias);
         node->setAliasVal(std::move(res));

         return QualType(node->getAliasVal().typeOf());
      }
      case Kind::LRK_Namespace:
         node->setKind(MemberKind::Namespace);
         return { NamespaceType::get(lookup.NS) };
   }

   return {};
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

         return {};
      }

      QualType ty(tup->getContainedType(node->getTupleIndex()));
      ty.isLvalue(true);

      node->setKind(MemberKind::TupleAccess);

      return ty;
   }

   return {};
}

QualType SemaPass::HandleFieldAccess(MemberRefExpr *node, Record* R, Field *F)
{
   if (!F)
      F = R->getField(node->getIdent());

   // Use a getter if available
   if (F->hasGetter() && !node->getIsLhsOfAssigment() && currentClass() != R) {
      node->isGetterCall(true);
      node->setAccessorMethod(F->getter);

      node->setKind(MemberKind::Accessor);
      node->setFieldType(QualType(F->fieldType));

      return node->getFieldType();
   }

   // Use a setter if available (and we're on the left side of an assignment)
   if (F->hasSetter() && node->getIsLhsOfAssigment() && currentClass() != R) {
      node->isSetterCall(true);
      node->setAccessorMethod(F->setter);
      node->setFieldType(QualType(F->fieldType));
      node->setKind(MemberKind::Accessor);

      setterMethod = node->getAccessorMethod();

      return node->getFieldType();
   }

   QualType ty(F->fieldType);
   ty.isLvalue(true);
   ty.isConst(F->isConst);

   // if we're in the classes initializer, all fields are non-const
   auto fun = getCurrentFunction();
   if (fun && isa<cl::Method>(fun)) {
      auto M = cast<cl::Method>(fun);
      if (M->isInitializer() && M->getOwningRecord() == R) {
         ty.isConst(false);
      }
   }

   if (F->isStatic)
      node->setKind(MemberKind::GlobalVariable);
   else
      node->setKind(MemberKind::Field);

   node->setFieldType(ty);
   node->setBinding(F->linkageName);

   checkMemberAccessibility(R, F->fieldName, F->accessModifier, node);

   return node->getFieldType();
}

QualType SemaPass::HandlePropAccess(MemberRefExpr *node, Record *rec,
                                    Property *P) {
   if (!P)
      P = rec->getProperty(node->getIdent());

   assert(P && "shouldn't be called otherwise");

   QualType ty;
   if (node->getIsLhsOfAssigment()) {
      if (!P->hasSetter()) {
         diagnose(node, err_generic_error,
                  "property " + node->getIdent() + "does not have a setter");
      }
      else {
         setterMethod = P->getSetter();
         node->isSetterCall(true);
         node->setAccessorMethod(P->getSetter());
      }

      *ty = VoidType::get();
   }
   else {
      if (!P->hasGetter()) {
         diagnose(node, err_generic_error,
                  "property " + node->getIdent() + "does not have a getter");
      }
      else {
         node->isGetterCall(true);
         node->setAccessorMethod(P->getGetter());
      }

      ty = P->getType();
   }

   node->setKind(MemberKind::Accessor);
   node->setFieldType(ty);

   return ty;
}

QualType SemaPass::HandleMethodReference(MemberRefExpr *node, cl::Record *R)
{
   auto overloads = R->getMethods().equal_range(node->getIdent());

   cl::Method* result = nullptr;
   auto ctx = node->getContextualType();
   auto fnTy = dyn_cast<FunctionType>(*ctx);

   while (overloads.first != overloads.second) {
      auto overload = &overloads.first->second;
      bool compatible = false;

      if (fnTy) {
         auto given = FunctionType::get(overload->getReturnType(),
                                        overload->getArguments(),
                                        fnTy->isRawFunctionTy());

         if (given != fnTy)
            continue;

         compatible = true;
      }
      else {
         compatible = ctx->isAutoTy();
      }

      if (compatible && result) {
         diagnose(node, err_generic_error,
                  "reference to method " + node->getIdent() + " is "
                     "ambiguous");

         break;
      }

      if (compatible)
         result = overload;

      ++overloads.first;
   }

   if (result) {
      node->setCallable(result);
      node->setKind(MemberKind::Function);

      auto args = result->getArguments();
      args.emplace(args.begin(), "", QualType(ObjectType::get(R)));

      return FunctionType::get(result->getReturnType(),
                               std::move(args), ctx->isRawFunctionTy());
   }

   return {};
}

QualType SemaPass::visitEnumCaseExpr(EnumCaseExpr *node)
{
   if (!node->getEnum()) {
      auto ty = node->getContextualType();
      if (!ty) {
         diagnose(node, err_generic_error,
                  "unqualified enum case requires a contextual type");

         return {};
      }

      if (!ty->isObjectTy()) {
         diagnose(node, err_generic_error,
                  ty->toString() + " is not a valid enumeration type");

         return {};
      }

      auto rec = ty->getRecord();
      if (!isa<Enum>(rec)) {
         diagnose(node, err_generic_error,
                  ty->toString() + " is not a valid enumeration type");

         return {};
      }

      node->setEnum(cast<Enum>(rec));
   }

   auto en = node->getEnum();
   if (!en->hasCase(node->getIdent())) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getIdent(), false);

      return {};
   }

   std::vector<Argument> resolvedArgs;

   auto &args = node->getArgs();
   for (const auto &arg : args) {
      QualType ty;

      if (auto Pl = getPlaceholderType(arg.get())) {
         *ty = Pl;
      }
      else {
         ty = visit(arg);

         if (arg->isTypeDependant()) {
            node->setIsTypeDependent(true);
            return {};
         }
         else if (arg->isValueDependant()) {
            node->setIsValueDependent(true);
         }
      }

      resolvedArgs.emplace_back("", ty, arg);
   }

   auto res = getCase(en, node->getIdent(), resolvedArgs);
   if (!res->isCompatible()) {
      diagnose(node, err_enum_case_not_found, en->getName(),
               node->getIdent(), !args.empty());

      return {};
   }

   PrepareCallArgs(node->getArgs(), resolvedArgs,
                   res->getCase()->associatedValues);
   ApplyCasts(node->getArgs(), res->getResolvedGivenArgs(),
              res->getCase()->associatedValues);

   return VisitSubExpr(node, QualType(ObjectType::get(en)));
}

QualType SemaPass::visitSubscriptExpr(SubscriptExpr *node, QualType ts)
{
   if (!ts->isObjectTy()) {
      QualType int64Ty(IntegerType::get());
      node->getIndex()->setContextualType(int64Ty);
   }

   auto indexResult = visitExpr(node, node->getIndex());
   if (indexResult.hadError())
      return {};

   QualType index = indexResult.getType();

   if (ts->isObjectTy()) {
      auto call = std::make_shared<CallExpr>(
         std::vector<Expression::SharedPtr>{ std::move(node->getIndex()) },
         string("postfix []")
      );

      call->setSourceLoc(node->getSourceLoc());
      call->setParentExpr(node);
      call->setParent(node);
      call->getResolvedArgs().emplace_back("", index);

      auto expr = std::static_pointer_cast<Expression>(call);

      node->setOverridenCall(call);

      auto result = visitCallExpr(call.get(), ts);
      return VisitSubExpr(node, result);
   }

   QualType resultType;
   if (auto Ptr = dyn_cast<PointerType>(*ts)) {
      resultType = Ptr->getPointeeType();
   }
   else if (auto Arr = dyn_cast<ArrayType>(*ts)) {
      *resultType = Arr->getElementType();
   }
   else {
      diagnose(node, err_illegal_subscript, ts);
      return {};
   }

   QualType int64Ty(IntegerType::get(64));
   forceCast(node->getIndex(), index, int64Ty);

   resultType.isLvalue(true);
   return VisitSubExpr(node, resultType);
}

} // namespace ast
} // namespace cdot