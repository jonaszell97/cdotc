//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "TypeCheckPass.h"
#include "../ASTIncludes.h"
#include "../../SymbolTable.h"
#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"
#include "../../../Message/Warning.h"
#include "../../../Variant/Type/BuiltinType.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../Operator/Conversion/LvalueToRvalue.h"
#include "../../../Util.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Message/Exceptions.h"
#include "../../../Variant/Type/AutoType.h"

#include "../../../Message/Diagnostics.h"

using namespace cdot::cl;
using namespace cdot::diag;

string self_str = "self";
std::vector<string> TypeCheckPass::currentNamespace = { "" };
std::vector<string> TypeCheckPass::importedNamespaces = { "" };
std::vector<std::vector<GenericConstraint>*> TypeCheckPass::GenericsStack = {};

TypeCheckPass::TypeCheckPass()
{
   pushScope();
   classScopeStack.push("");

   util::TCPass = this;
}

void TypeCheckPass::pushClassScope(cl::Class *cl, string& className)
{
   pushNamespace(className);
   latestScope->inProtocol = cl->isProtocol();

   classScopeStack.push(currentNamespace.back());
   GenericsStack.push_back(&cl->getGenerics());
}

void TypeCheckPass::popClassScope()
{
   popNamespace();
   latestScope->inProtocol = false;

   classScopeStack.pop();
   GenericsStack.pop_back();
}

void TypeCheckPass::doInitialPass(std::vector<std::shared_ptr<Statement>>& statements)
{
   for (const auto& stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto node = std::static_pointer_cast<ClassDecl>(stmt);
            auto cl = node->declaredClass;

            pushClassScope(cl, node->className);

            doInitialPass(node->innerDeclarations);
            for (const auto& field : node->fields) {
               DefineField(field.get(), cl);
            }

            popClassScope();

            break;
         }
         case NodeType::NAMESPACE_DECL: {
            auto ns = std::static_pointer_cast<NamespaceDecl>(stmt);
            pushNamespace(ns->nsName);
            doInitialPass(ns->contents->getStatements());
            popNamespace();
            break;
         }
         case NodeType::USING_STMT:
         case NodeType::EOF_STMT:
            stmt->accept(*this);
         default:
            break;
      }
   }

   importedNamespaces.empty();
   currentNamespace.empty();
}

namespace {
   void CopyScopeProps(
      Scope* src,
      Scope* dst)
   {
      if (src == nullptr) {
         return;
      }

      dst->currentSelf = src->currentSelf;
      dst->currentFunction = src->currentFunction;
      dst->enclosingScope = src;
      dst->unsafe = src->unsafe;
      dst->inLambda = src->inLambda;
      dst->declaredReturnType = src->declaredReturnType;
      dst->uninitializedFields = src->uninitializedFields;
      dst->captures = src->captures;
      dst->mutableSelf = src->mutableSelf;
   }

   bool warnCast(Type &lhs, Type &rhs)
   {
      return !lhs->isBoxedEquivOf(*rhs) && !rhs->isBoxedEquivOf(*lhs);
   }

   void raiseTypeError(Type &lhs, Type &rhs, AstNode* cause)
   {
      diag::err(err_type_mismatch) << lhs << rhs << cause << diag::term;
   }
}

void TypeCheckPass::pushScope() {
   Scope scope;
   scope.id = lastScopeID++;
   CopyScopeProps(latestScope, &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckPass::pushFunctionScope(
   Type returnType,
   string methodName,
   string mangledName,
   bool isLambda)
{
   Scope scope;
   CopyScopeProps(latestScope, &scope);
   scope.id = lastScopeID++;
   scope.isFunctionRoot = true;
   scope.isLambdaRoot = isLambda;
   scope.inLambda = isLambda;
   scope.returnable = true;
   scope.branches = 0; // FIXME

   if (!isLambda) {
      scope.currentFunction = std::make_pair(methodName, mangledName);
   }

   scope.declaredReturnType = returnType;

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckPass::pushMethodScope(
   Method *method)
{
   Scope scope;
   scope.id = lastScopeID++;
   scope.isFunctionRoot = true;
   scope.returnable = true;

   CopyScopeProps(latestScope, &scope);
   if (method->methodName != "init") {
      scope.declaredReturnType = method->returnType;
   }
   else {
      scope.declaredReturnType = Type(VoidType::get());
   }

   scope.currentFunction = std::make_pair(method->methodName, method->mangledName);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckPass::pushLoopScope(
   bool continuable,
   bool breakable)
{
   Scope scope;
   scope.id = lastScopeID++;
   scope.continuable = continuable;
   scope.breakable = breakable;

   CopyScopeProps(&Scopes.top(), &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckPass::popScope() {
   Scopes.pop();
   latestScope = &Scopes.top();
}

void TypeCheckPass::connectTree(
   AstNode *root)
{
   for (const auto& child : root->get_children()) {
      child->parent = root;
      connectTree(child.get());
   }
}

/**
 * Declares a variable in the current context
 * @param name
 * @param type
 * @param cause
 */
string TypeCheckPass::declareVariable(
   string &name,
   Type type,
   bool global,
   AstNode *cause)
{
   string var_name;
   if (global) {
      var_name = ns_prefix() + name;
   }
   else {
      var_name = name + std::to_string(latestScope->id);
   }

   if (SymbolTable::hasVariable(var_name, importedNamespaces)) {
      diag::err(err_var_redeclaration) << var_name << cause << diag::term;
   }

   SymbolTable::declareVariable(var_name, type, AccessModifier::PUBLIC, currentNamespace.back(), cause);

   return var_name;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckPass::pushTy(Type& type)
{
   typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
Type TypeCheckPass::popTy()
{
   auto top = typeStack.top();
   typeStack.pop();

   return top;
}

/**
 * Returns a variable if it exists, throws otherwise
 * @param ident
 * @param cause
 * @return
 */
pair<pair<Variable, string>, bool> TypeCheckPass::getVariable(
   string &ident,
   AstNode *cause)
{
   auto current = latestScope;
   string curr;
   bool escapesLambdaScope = false;

   while (current != nullptr) {
      curr = ident + std::to_string(current->id);
      if (SymbolTable::hasVariable(curr)) {
         break;
      }

      if (current->isLambdaRoot) {
         escapesLambdaScope = true;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      if (SymbolTable::hasVariable(ident, importedNamespaces)) {
         auto var = SymbolTable::getVariable(ident, importedNamespaces);
         if (var.first.access == AccessModifier::PRIVATE && std::find(currentNamespace.begin(),
            currentNamespace.end(), var.first.declaredNamespace) == currentNamespace.end())
         {
            diag::err(err_private_access) << 0 /*variable*/ << ident << cause
                                          << diag::term;
         }

         return { var, false };
      }

      diag::err(err_undeclared_identifer) << ident << cause
                                          << diag::term;
   }

   return { { SymbolTable::getVariable(curr), curr }, escapesLambdaScope };
}

bool TypeCheckPass::hasVariable(string ident)
{
   auto current = latestScope;
   auto& ns = currentNamespace.back();

   while (current != nullptr) {
      auto scopedName = ident + std::to_string(current->id);
      if (SymbolTable::hasVariable(scopedName)) {
         break;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      return SymbolTable::hasVariable(ident, importedNamespaces);
   }

   return true;
}

/**
 * Checks for a matching function overload, throws if none or more than one is found
 * @param fun
 * @param args
 * @param cause
 * @return
 */
CallCompatability TypeCheckPass::getFunction(
   string& funcName,
   std::vector<Argument>& args,
   std::vector<GenericType*> generics)
{
   string context;
   auto overloads = SymbolTable::getFunction(funcName, importedNamespaces);

   CallCompatability result;
   if (overloads.first == overloads.second) {
      return result;
   }

   bool firstIncompatible = true;
   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;

      auto res = util::findMatchingCall(args, overload->getArguments(), generics, overload->getGenerics());
      if (!res.isCompatible()) {
         if (firstIncompatible) {
            firstIncompatible = false;
            result = res;
            result.compatibility = CompatibilityType::NO_MATCHING_CALL;
         }

         CallCandidate cand;
         cand.func = overload.get();
         cand.incompatibleArg = result.incompatibleArg;
         cand.failedConstraint = result.failedConstraint;

         result.failedCandidates.push_back(cand);
      }
      else if (res.perfectMatch || (res.isCompatible()
         && (!result.isCompatible() || res.castPenalty <= result.castPenalty)))
      {
         result = res;
         result.func = overload.get();

         if (res.perfectMatch) {
            break;
         }
      }
   }

   return result;
}

void TypeCheckPass::ApplyCasts(
   std::vector<pair<string, std::shared_ptr<Expression>>> &args,
   std::vector<Argument> &givenArgs,
   std::vector<Argument> &declaredArgs,
   std::vector<size_t> &casts)
{
   size_t i = 0;
   for (auto& arg : givenArgs) {
      if (i >= declaredArgs.size()) {
         break;
      }

      assert(args.size() > i && "No arg to cast");
      if (arg.type != declaredArgs[i].type && !declaredArgs[i].type->isAutoTy()) {
         wrapImplicitCast(args[i].second, arg.type, declaredArgs[i].type);
      }

      ++i;
   }
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckPass::return_(
   Type& ret_type,
   AstNode *cause)
{
   auto current = latestScope;
   while (!current->returnable) {
      ++current->returned;
      if (current->declaredReturnType->isAutoTy()) {
         current->declaredReturnType = ret_type;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      diag::err(err_return_outside_func) << cause << diag::term;
   }

   if (!ret_type.implicitlyCastableTo(current->declaredReturnType) ||
      (current->declaredReturnType.isLvalue() && !ret_type.isLvalue()))
   {
      diag::err(err_return_type_mismatch) << ret_type << current->declaredReturnType
                                          << cause << diag::term;
   }

   if (current->declaredReturnType->isAutoTy()) {
      current->declaredReturnType = ret_type;
   }

   ++current->returned;
}

void TypeCheckPass::continue_(ContinueStmt* continueStmt)
{
   auto current = latestScope;
   while (current != nullptr && !current->continuable) {
      current = current->enclosingScope;
   }

   if (current == nullptr) {
      diag::err(err_loop_keyword_outside_loop) << 0 /*continue*/ << continueStmt
                                               << diag::term;
   }

   current->continued = true;
}

void TypeCheckPass::break_(BreakStmt *breakStmt)
{
   auto current = latestScope;
   while (current != nullptr && !current->breakable) {
      current = current->enclosingScope;
   }

   if (current == nullptr) {
      diag::err(err_loop_keyword_outside_loop) << 1 /*break*/ << breakStmt
                                               << diag::term;
   }

   current->broken = true;
}

void TypeCheckPass::resolve(Type* ty, AstNode* node)
{

}

void TypeCheckPass::pushNamespace(string &ns)
{
   auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");
}

void TypeCheckPass::popNamespace()
{
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

Type TypeCheckPass::ReturnMemberExpr(
   Expression *node,
   Type ty)
{
   if (node->memberExpr != nullptr) {
      if (ty.needsLvalueToRvalueConv()) {
         ty.isLvalue(false);
         node->lvalueCast = true;
      }

      pushTy(ty);
      ty = node->memberExpr->accept(*this);
   }
   else if (node->createsTemporary()
      && (ty->isRefcounted() || ty->isStruct()) && !node->isAssigned())
   {
      node->lvalueCast = ty.needsLvalueToRvalueConv();
      node->isTemporary(true);
      node->setTempType(ty->getRecord());
   }

   return ty;
}

void TypeCheckPass::wrapImplicitCast(
   Expression::SharedPtr& target,
   Type& originTy,
   Type destTy)
{
   auto cast = new ImplicitCastExpr(originTy, destTy, target);

   cast->loc = target->loc;
   target->setParent(cast);
   cast->children.push_back(&target);

   target.reset(cast);
}

void TypeCheckPass::lvalueToRvalue(
   std::shared_ptr<Expression> &target)
{
   auto copy = target;
   auto ltor = new LvalueToRvalue(copy);

   target.reset(ltor);
   CopyNodeProperties(copy.get(), target.get());
}

void TypeCheckPass::toRvalueIfNecessary(
   Type &ty,
   std::shared_ptr<Expression> &target,
   bool preCond)
{
   if (!preCond) {
      return;
   }

   if (ty.needsLvalueToRvalueConv()) {
      lvalueToRvalue(target);
   }

   ty.isLvalue(false);
}

bool TypeCheckPass::castGenericIfNecessary(
   Expression *node,
   std::vector<GenericType*> &concreteGenerics,
   Type &ty,
   bool preCond)
{
   if (!preCond || !ty->isGeneric()) {
      return false;
   }

   Type fromTy(ty);
   Type toTy(ty);

   resolveGenerics(toTy, concreteGenerics);
   toTy.isLvalue(fromTy.isLvalue());

   if (fromTy == toTy) {
      return false;
   }

   node->needs_cast = true;
   node->castFrom = fromTy;
   node->castTo = toTy;

   return true;
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type TypeCheckPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->nsName);
   if (node->isAnonymousNamespace_) {
      importedNamespaces.push_back(currentNamespace.back());
   }

   node->contents->accept(*this);
   if (!node->isAnonymousNamespace_) {
      popNamespace();
   }
   else {
      currentNamespace.pop_back();
   }

   return {};
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type TypeCheckPass::visit(CompoundStmt *node)
{
   if (!node->preserveScope) {
      pushScope();
   }

   latestScope->unsafe = node->isUnsafe_ || latestScope->unsafe;

   for (const auto& child : node->get_children()) {
      Type ty = child->accept(*this);
   }

   if (!node->preserveScope) {
      popScope();
   }

   return {};
}

namespace {

   bool isReservedIdentifier(string& ident)
   {
      return (
         ident == "_"   ||
         ident == "Self"
      );
   }

}

void TypeCheckPass::checkClassAccessibility(
   cdot::cl::Class *&cl,
   Expression *cause)
{
   if (cl->isPrivate() &&
      std::find(currentNamespace.begin(), currentNamespace.end(), cl->getDeclarationNamespace())
      == currentNamespace.end())
   {
      diag::err(err_class_not_accessible) << cl->getNameSelector()
                                          << cl->getName() << cause << diag::term;
   }
}

void TypeCheckPass::checkMemberAccessibility(
   cdot::cl::Class *&cl,
   string& memberName,
   AccessModifier &access,
   Expression *cause)
{
   if (access == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass())) {
      diag::err(err_protected_member_access) << memberName << cl->getNameSelector()
                                             << cl->getName() << cause << diag::term;
   }
   else if (access == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass())) {
      diag::err(err_private_member_access) << memberName << cl->getNameSelector()
                                           << cl->getName() << cause << diag::term;
   }
}

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
Type TypeCheckPass::visit(FunctionDecl *node)
{
   if (node->is_declaration) {
      for (const auto& arg : node->args) {
         if (arg->defaultVal != nullptr) {
            arg->defaultVal->setContextualType(arg->argType->type);
            arg->defaultVal->accept(*this);
         }
      }

      return {};
   }

   if (node->hasAttribute(Attr::Extern)) {
      auto& externKind = node->getAttribute(Attr::Extern).args.front();
      if (externKind == "C") {
         node->externKind = ExternKind::C;
      }
      else if (externKind == "CPP" || externKind == "C++") {
         node->externKind = ExternKind::CPP;
      }
      else {
         //err
      }
   }

   for (const auto& inner : node->getInnerDecls()) {
      inner->accept(*this);
   }

   pushNamespace(node->funcName);
   GenericsStack.push_back(&node->generics);

   auto return_type = node->returnType->getType();
   pushFunctionScope(return_type, node->declaredFunction->getName(),
      node->declaredFunction->getMangledName());

   for (const auto& arg : node->args) {
      arg->accept(*this);
      arg->binding = declareVariable(arg->argName, arg->argType->getType());
   }

   attributes = node->attributes;
   node->body->accept(*this);

   attributes.clear();

   if (latestScope->branches - latestScope->returned > 0 &&
      !return_type->isVoidTy() && node->funcName != "main")
   {
      diag::err(err_not_all_code_paths_return) << node << diag::term;
   }
   // implicit 0 return for main function
   else if (latestScope->branches - latestScope->returned > 0 && node->funcName == "main") {
      *return_type = IntegerType::get();
   }

   popScope();
   GenericsStack.pop_back();
   popNamespace();

   return {};
}

void TypeCheckPass::CopyNodeProperties(
   Expression *src,
   Expression *dst)
{
   dst->lhs_of_assignment = src->lhs_of_assignment;
   dst->function_argument = src->function_argument;
   dst->part_of_return_value = src->part_of_return_value;
   dst->loc = src->loc;
   dst->isTemporary(src->isTemporary());
   dst->setTempType(src->getTempType());

   if (src->sret_value) {
      dst->isHiddenReturnValue();
   }
   else if (src->returned_value) {
      dst->isReturnValue();
   }
}

BuiltinType* TypeCheckPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
{
   if (builtinIdentifiers.find(node->ident) == builtinIdentifiers.end()) {
      return {};
   }

   auto kind = builtinIdentifiers[node->ident];
   switch (kind) {
      case BuiltinIdentifier::FUNC:
         node->builtinValue = Variant(latestScope->currentFunction.first);
         node->builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::MANGLED_FUNC:
         node->builtinValue = Variant(latestScope->currentFunction.second);
         node->builtinType = ObjectType::get("String");
         break;
      case BuiltinIdentifier::FLOAT_QNAN:
      case BuiltinIdentifier::FLOAT_SNAN:
         node->builtinType = FPType::getFloatTy();
         break;
      case BuiltinIdentifier::DOUBLE_QNAN:
      case BuiltinIdentifier::DOUBLE_SNAN:
         node->builtinType = FPType::getDoubleTy();
         break;
   }

   node->builtinKind = kind;
   return node->builtinType;
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
Type TypeCheckPass::visit(IdentifierRefExpr *node)
{
   if (node->is_let_expr || node->is_var_expr) {
      diag::err(err_unexpected_let_expr) << node << diag::term;
   }

   auto builtin = HandleBuiltinIdentifier(node);
   if (builtin != nullptr) {
      return Type(builtin);
   }

   string ns_name = node->ident;
   for (const auto& ns : importedNamespaces) {
      auto curr = ns + ns_name;
      if (SymbolTable::isNamespace(curr)) {
         ns_name = curr;
         break;
      }
   }

   if (SymbolTable::isNamespace(ns_name) && node->memberExpr != nullptr)
   {
      auto current = node->memberExpr;

      while (current != nullptr && current->get_type() == NodeType::MEMBER_EXPR) {
         auto new_ns = ns_name + "." + current->ident;

         if (!SymbolTable::isNamespace(new_ns)) {
            break;
         }

         ns_name = new_ns;
         current = current->memberExpr;
      }

      node->is_namespace = true;
      node->memberExpr = current;

      if (SymbolTable::hasTypedef(ns_name, importedNamespaces)) {
         auto td = SymbolTable::getTypedef(ns_name, importedNamespaces);
         if (td.aliasedType->isObject()) {
            ns_name = td.aliasedType->getClassName();
         }
      }

      if (node->memberExpr->get_type() == NodeType::MEMBER_EXPR) {
         auto member_expr = std::static_pointer_cast<MemberRefExpr>(node->memberExpr);
         member_expr->isNsMember = true;
         member_expr->className = ns_name;
         member_expr->ident = ns_name + "." + member_expr->ident;
      }
      else if (node->memberExpr->get_type() == NodeType::CALL_EXPR) {
         auto member_expr = std::static_pointer_cast<CallExpr>(node->memberExpr);
         member_expr->isNsMember = true;
         member_expr->className = ns_name;
         member_expr->ident = member_expr->ident;
         member_expr->type = CallType::METHOD_CALL;
      }
      else if (node->memberExpr->get_type() == NodeType::ARRAY_ACCESS_EXPR) {
         diag::err(err_illegal_subscript) << "namespace" << node->memberExpr
                                          << diag::term;
      }
      else {
         assert(false && "Unknown operation");
      }

      node->memberExpr->contextualType = node->contextualType;

      return node->memberExpr->accept(*this);
   }

   if (node->ident == "super") {
      if (currentClass().empty()) {
         diag::err(err_self_outside_method) << 1 /*super*/ << node << diag::term;
      }

      auto currentCl = SymbolTable::getClass(currentClass());
      if (currentCl->getParent() == nullptr) {
         diag::err(err_super_without_base) << currentClass() << node << diag::term;
      }

      node->binding = latestScope->currentSelf;
      node->ident = "self";
      node->is_super = true;
      node->superClassName = currentCl->getParent()->getName();

      auto type = Type(ObjectType::get(node->superClassName));
      return ReturnMemberExpr(node, type);
   }

   // try a function instead
   if (!hasVariable(node->ident)) {
      std::vector<Argument> args;
      Type returnType;
      bool typeInferred = false;

      if (node->contextualType->isFunctionTy()) {
         auto asFunc = cast<FunctionType>(*node->contextualType);
         args = asFunc->getArgTypes();
         returnType = asFunc->getReturnType();
         typeInferred = true;
      }
      else {
         *returnType = VoidType::get();
      }

      auto overloads = SymbolTable::getFunction(node->ident, importedNamespaces);
      bool foundMatch = false;
      Function* result;

      // only one result
      if (!typeInferred && std::distance(overloads.first, overloads.second) == 1) {
         result = overloads.first->second.get();

         node->binding = result->getMangledName();
         node->is_function = true;

         std::vector<Argument> argTypes;
         for (const auto& arg : result->getArguments()) {
            argTypes.emplace_back("", arg.type);
         }

         auto funcTy = FunctionType::get(result->getReturnType(),
            argTypes, node->contextualType->isRawFunctionTy());
         return ReturnMemberExpr(node, Type(funcTy));
      }

      for (auto it = overloads.first; it != overloads.second; ++it) {
         auto& argTypes = it->second->getArguments();
         if (argTypes.size() != args.size()) {
            continue;
         }

         size_t i = 0;
         bool matches = true;
         for (auto& arg : argTypes) {
            if (!args.at(i).type.implicitlyCastableTo(arg.type)) {
               matches = false;
               break;
            }
         }

         if (!matches) {
            continue;
         }

         if (!returnType.implicitlyCastableTo(it->second->getReturnType())) {
            continue;
         }

         foundMatch = true;
         result = it->second.get();
         break;
      }

      if (foundMatch) {
         node->binding = result->getMangledName();
         node->is_function = true;

         std::vector<Argument> argTypes;
         for (const auto& arg : result->getArguments()) {
            argTypes.emplace_back("", arg.type);
         }

         auto funcTy = FunctionType::get(result->getReturnType(), argTypes,
            node->contextualType->isRawFunctionTy());
         return ReturnMemberExpr(node, Type(funcTy));
      }
   }

   bool implicit_this = node->ident != "self" && !hasVariable(node->ident) && !currentClass().empty();
   if (node->ident == "self" || implicit_this) {
      if (currentClass().empty()) {
         diag::err(err_self_outside_method) << 0 /*self*/ << node << diag::term;
      }

      if (implicit_this) {
         auto mem_ref = std::make_shared<MemberRefExpr>(node->ident);
         CopyNodeProperties(node, mem_ref.get());
         mem_ref->parent = node;

         mem_ref->setMemberExpr(node->memberExpr);
         node->memberExpr = mem_ref;
      }

      node->binding = latestScope->currentSelf;
      node->ident = "self";

      auto cl = SymbolTable::getClass(currentClass());

      // add generics
      std::vector<GenericType*> generics;
      for (const auto& gen : cl->getGenerics()) {
         auto covar = gen.covarName;
         if (covar.empty()) {
            covar = "Any";
         }

         generics.push_back(
            GenericType::get(gen.genericTypeName, ObjectType::get(covar))
         );
      }

      Type type(ObjectType::get(cl->getName(), generics));

      bool rawEnum = cl->isRawEnum();
      type.isLvalue(latestScope->mutableSelf || rawEnum);
      type.isSelf(!rawEnum);

      return ReturnMemberExpr(node, type);
   }

   auto res = getVariable(node->ident, node);

   // add capture for this variable
   if (latestScope->inLambda && res.second) {
      latestScope->captures->emplace_back(res.first.second, res.first.first.type);
      node->captured_var = true;
      node->capturedType = *res.first.first.type;
   }

   node->binding = res.first.second;
   node->declaration = declarations[node->binding];

   return ReturnMemberExpr(node, res.first.first.type);
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
Type TypeCheckPass::visit(DeclStmt *node)
{
   if (node->declared || node->is_declaration) {
      return {};
   }

   auto& ident = node->identifier;
   if (isReservedIdentifier(ident)) {
      diag::err(err_reserved_identifier) << ident << node << diag::term;
   }

   if (!node->type->resolved) {
      node->type->accept(*this);
   }

   auto declType = node->type->getType();
   auto& val = node->value;

   if (val != nullptr) {
      val->isAssigned(true);
      val->addUse();

      if (!isa<AutoType>(*declType)) {
         val->setContextualType(declType);
      }

      Type givenType = val->accept(*this);
      if (givenType->isVoidTy()) {
         diag::err(err_cannot_assign_void) << node << diag::term;
      }

      toRvalueIfNecessary(givenType, node->value, !declType.isLvalue());
      node->protocol_decl = givenType->isProtocol() && !declType->isProtocol();

      // in case it has changed
      declType = node->type->getType();

      if (declType->isAutoTy()) {
         declType = givenType;
         node->type->setType(declType);
      }
      else if (!givenType.implicitlyCastableTo(declType)) {
         diag::err(err_type_mismatch) << givenType << declType << val << diag::term;
      }

      if (declType != givenType) {
         wrapImplicitCast(node->value, givenType, declType);
      }
   }
   else if (!declType->hasDefaultValue()) {
      diag::err(err_not_initialized) << node << diag::term;
   }

   resolve(&declType);

   auto allocType = declType;
   node->struct_alloca = declType->isStruct();
   node->inc_refcount = declType->isObject() &&
      SymbolTable::getRecord(declType->getClassName())->isRefcounted();

   allocType.isLvalue(true);
   allocType.isConst(node->is_const);

   if (!node->is_global) {
      node->binding = declareVariable(ident, allocType, node->is_global, node);
   }
   else {
      SymbolTable::setVariable(node->binding, *allocType);
   }

   declarations.emplace(node->binding, node);

   node->declared = true;
   return {};
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
Type TypeCheckPass::visit(ForStmt *node)
{
   pushScope();

   if (node->initialization) {
      node->initialization->accept(*this);
   }

   if (node->increment) {
      node->increment->accept(*this);
   }

   if (node->termination) {
      node->termination->addUse();

      auto cond = node->termination->accept(*this);
      auto boolTy = Type(IntegerType::get(1));

      if (cond->isObject() && cond->getClassName() == "Bool") {
         wrapImplicitCast(node->termination, cond, boolTy);
      }
      else if (!cond.implicitlyCastableTo(boolTy)) {
         diag::err(err_cond_not_boolean) << 2 /*for*/ << node->termination
                                         << diag::term;
      }
   }

   if (node->body) {
      pushLoopScope();
      node->body->accept(*this);
      popScope();
   }

   popScope();
   return {};
}

Type TypeCheckPass::visit(ForInStmt *node)
{
   auto range = node->rangeExpr->accept(*this);
   node->rangeExpr->addUse();

   if (range.isSelf() && !GenericsStack.empty()) {
      resolveGenerics(range, *GenericsStack.back());
   }

   if (!range->isObject() || !SymbolTable::getClass(range->getClassName())->conformsTo("Iterable")) {
      diag::err(err_range_not_iterable) << node->rangeExpr << diag::term;
   }

   toRvalueIfNecessary(range, node->rangeExpr);

   auto cl = SymbolTable::getClass(range->getClassName());
   cl->addUse();

   auto& prot = cl->getConformedToProtocols();
   Type itType;
   for (const auto& p : prot) {
      if (p->getClassName() == "Iterable") {
         *itType = p->getConcreteGeneric("T")->getActualType();
         break;
      }
   }

   assert(!itType->isAutoTy() && "Iterable conformance shouldn't be possible otherwise!");
   resolveGenerics(itType, range);

   if (itType->isProtocol()) {
      node->protocolTy = *itType;
   }

   node->decl->type->accept(*this);
   if (!node->decl->type->getType().implicitlyCastableTo(itType)) {
      diag::err(err_type_mismatch) << node->decl->type->getType() << itType
                                   << node->decl << diag::term;
   }

   pushLoopScope();
   itType.isLvalue(true);
   itType.isConst(node->decl->is_const);

   node->binding = declareVariable(node->decl->identifier, itType, false, node);

   node->body->accept(*this);
   popScope();

   auto getIterator = cl->hasMethod("getIterator", {}, {}, *range);
   assert(getIterator.compatibility == CompatibilityType::COMPATIBLE
      && "Iterable not implemented correctly?");

   ++getIterator.method->uses;

   auto iteratorCl = SymbolTable::getClass(getIterator.method->returnType->getClassName());
   iteratorCl->addUse();

   auto nextFunc = iteratorCl->hasMethod("next", {}, {}, ObjectType::get(iteratorCl->getName()));
   assert(nextFunc.compatibility == CompatibilityType::COMPATIBLE
      && "Iterator<> not implemented correctly?");

   ++nextFunc.method->uses;

   node->iteratorGetter = getIterator.method->mangledName;
   node->iteratorClass = getIterator.method->returnType->getClassName();
   node->nextFunc = nextFunc.method->mangledName;
   node->iteratedType = itType;

   node->rangeIsRefcounted = range->isRefcounted();
   if (node->rangeIsRefcounted) {
      node->rangeClassName = range->getClassName();
   }

   return {};
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
Type TypeCheckPass::visit(
   WhileStmt *node)
{
   auto cond = node->condition->accept(*this);
   node->condition->addUse();

   auto boolTy = Type(IntegerType::get(1));
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      diag::err(err_cond_not_boolean) << 1 /*while*/ << node->condition
                                      << diag::term;
   }

   pushLoopScope();
   node->body->accept(*this);
   popScope();

   return {};
}

pair<Type, std::vector<Type>> TypeCheckPass::unify(
   std::vector<Expression::SharedPtr>& types)
{
   Type unified;
   std::vector<Type> evaledTypes;
   evaledTypes.reserve(types.size());

   bool anyCompatible = false;

   for (auto& expr : types) {
      auto type = expr->accept(*this);
      evaledTypes.push_back(type);

      if (unified->isAutoTy()) {
         unified = type;
         continue;
      }

      if (!unified.implicitlyCastableTo(type)) {
         if (!type.implicitlyCastableTo(unified)) {
            *unified = ObjectType::get("Any");
            anyCompatible = true;
         }
      }
      else if (!anyCompatible) {
         *unified = *type;
      }
   }

   size_t i = 0;
   for (auto& expr : types) {
      wrapImplicitCast(expr, evaledTypes.at(i), Type(unified));
      ++i;
   }

   return pair<Type, std::vector<Type>>{ unified, evaledTypes };
}

Type TypeCheckPass::HandleDictionaryLiteral(CollectionLiteral *node)
{
   node->type->accept(*this);

   auto dictTy = node->type->getType();
   auto HashableTy = ObjectType::get("Hashable");
   auto AnyTy = ObjectType::getAnyTy();

   auto& keys = node->keys;
   auto& values = node->values;

   assert(keys.size() == values.size() && "Should have raised a parse error");

   if (node->contextualType->isObject()) {
      auto dict = cast<ObjectType>(*node->contextualType);
      if (dict->getClassName() != "Dictionary") {
         raiseTypeError(node->contextualType, dictTy, node);
      }

      auto keyTy = Type(dict->getConcreteGeneric("K"));
      auto valTy = Type(dict->getConcreteGeneric("V"));

      for (auto& key : keys) {
         auto ty = key->accept(*this);
         key->addUse();
         toRvalueIfNecessary(ty, key);

         if (!ty.implicitlyCastableTo(keyTy)) {
            raiseTypeError(ty, keyTy, key.get());
         }
         else if (ty != keyTy) {
            wrapImplicitCast(key, ty, keyTy);
         }

         wrapImplicitCast(key, keyTy, Type(HashableTy));
      }

      for (auto& val : values) {
         auto ty = val->accept(*this);
         val->addUse();
         toRvalueIfNecessary(ty, val);

         if (!ty.implicitlyCastableTo(valTy)) {
            raiseTypeError(ty, valTy, val.get());
         }
         else if (ty != valTy) {
            wrapImplicitCast(val, ty, valTy);
         }

         wrapImplicitCast(val, valTy, Type(AnyTy));
      }

      return dictTy;
   }

   auto keyTy = unify(keys);
   auto valTy = unify(values);

   if (!keyTy.first->isObject() || !keyTy.first->getRecord()->conformsTo("Hashable")) {
      diag::err(err_dict_key_not_hashable) << node << diag::term;
   }

   size_t i = 0;
   for (auto& key : node->keys) {
      key->addUse();
      toRvalueIfNecessary(keyTy.second[i++], key);
      wrapImplicitCast(key, keyTy.first, Type(HashableTy));
   }

   i = 0;
   for (auto& val : node->values) {
      val->addUse();
      toRvalueIfNecessary(valTy.second[i++], val);
      wrapImplicitCast(val, valTy.first, Type(AnyTy));
   }

   std::vector<GenericType*> generics{ GenericType::get("K", *keyTy.first),
      GenericType::get("V", *valTy.first), };

   return Type(ObjectType::get("Dictionary", generics));
}

/**
 * Checks an array literal
 * @param node
 * @return
 */
Type TypeCheckPass::visit(CollectionLiteral *node)
{
   if (node->isDictionary) {
      return HandleDictionaryLiteral(node);
   }

   auto isInferred = node->contextualType->isAutoTy();
   if (isInferred) {
      Type elTy;
      if (node->values.empty()) {
         *elTy = ObjectType::get("Any")->getPointerTo();
      }
      else {
         elTy = unify(node->values).first;
         for (auto& el : node->values) {
            toRvalueIfNecessary(elTy, el);
            wrapImplicitCast(el, elTy, Type(ObjectType::getAnyTy()));
         }
      }

      if (node->hasAttribute(Attr::CArray)) {
         node->type->setType(elTy);
      }
      else {
         std::vector<GenericType*> generics{ GenericType::get("T", *elTy )};
         Type ArrayTy(ObjectType::get("Array", generics));
         node->type->setType(ArrayTy);
      }

      node->type->resolved = true;
      return ReturnMemberExpr(node, node->type->getType());
   }

   if (!node->contextualType->isPointerTy() && !node->contextualType->isObject()) {
      diag::err(err_type_mismatch) << node->contextualType << "Dictionary"
                                   << node << diag::term;
   }

   bool isCarray = false;
   Type elType;
   if (node->contextualType->isPointerTy()) {
      elType = node->contextualType->asPointerTy()->getPointeeType();
      isCarray = true;
   }
   else {
      *elType = node->contextualType->asObjTy()->getConcreteGeneric("T");
   }

   if (!node->values.empty()) {
      auto givenType = unify(node->values).first;
      for (auto& el : node->values) {
         el->addUse();
         toRvalueIfNecessary(givenType, el);
      }

      if (!givenType.implicitlyCastableTo(elType)) {
         diag::err(err_type_mismatch) << elType << givenType
                                      << node << diag::term;
      }
      else if (elType != givenType) {
         for (auto &el : node->values) {
            wrapImplicitCast(el, givenType, elType);
         }
      }

      if (!elType->isObject() || elType->getClassName() != "Any") {
         for (auto &el : node->values) {
            wrapImplicitCast(el, elType, Type(ObjectType::getAnyTy()));
         }
      }
   }

   Type ty(elType->getPointerTo());
   if (isCarray) {
      node->type->setType(ty);
   }
   else {
      node->type->setType(node->contextualType);
   }

   return ReturnMemberExpr(node, node->type->getType());
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
Type TypeCheckPass::visit(NumericLiteral *node)
{
   if ((node->memberExpr == nullptr && isa<PrimitiveType>(*node->contextualType)) ||
      node->hasAttribute(Attr::Primitive))
   {
      node->primitive = true;

      if (!node->type->implicitlyCastableTo(*node->contextualType)) {
         return Type(node->type);
      }

      if (isa<PrimitiveType>(*node->contextualType)) {
         node->type = *node->contextualType;
         node->value = node->value.castTo(node->type);
      }

      return Type(node->type);
   }

   if (node->contextualType->isBoxedPrimitive()) {
      node->type = node->contextualType->unbox();
   }

   switch (node->type->getTypeID()) {
      case TypeID::IntegerTypeID: {
         auto asInt = cast<IntegerType>(node->type);
         string className = "Int";
         if (asInt->isUnsigned()) {
            className = "U" + className;
         }

         auto bitWidth = asInt->getBitwidth();
         className += std::to_string(bitWidth);

         if (node->is_char) {
            className = "Char";
         }

         if (node->is_bool) {
            className = "Bool";
         }

         node->className = className;
         break;
      }
      case TypeID::FPTypeID: {
         auto asFloat = cast<FPType>(node->type);
         node->className = asFloat->getPrecision() == 64 ? "Double" : "Float";
         break;
      }
      case TypeID::ObjectTypeID:
         node->className = node->type->getClassName();
         break;
      default:
         llvm_unreachable("Unknown literal type");
   }

   assert(node->primitive || !node->className.empty() && "Unknown primitive type");

   auto ty = ObjectType::get(node->className);
   if (node->is_char) {
      BuiltinType* ty = IntegerType::get(8);
      node->value = node->value.castTo(ty);
   }
   else if (node->is_bool) {
      BuiltinType* ty = IntegerType::get(1);
      node->value = node->value.castTo(ty);
   }
   else {
      node->value = node->value.castTo(node->type);
   }

   return ReturnMemberExpr(node, Type(ty));
}

Type TypeCheckPass::visit(NoneLiteral *node)
{
   if (node->contextualType->isAutoTy()) {
      diag::err(err_requires_contextual_type) << "'none'" << node << diag::term;
   }
   if (!node->contextualType->isOptionTy()) {
      diag::err(err_type_mismatch) << node->contextualType << "Option"
                                   << node << diag::term;
   }

   return ReturnMemberExpr(node, node->contextualType);
}

Type TypeCheckPass::visit(StringLiteral *node)
{
   for (const auto& attr : node->attributes) {
      switch (attr.kind) {
         case Attr::CString:
            node->raw = true;
            break;
         default:
            diag::err(err_attr_not_applicable) << attr.name << node << diag::term;
      }
   }

   if (node->contextualType->isPointerTy()) {
      node->raw = true;
   }

   if (node->raw) {
      Type charTy(IntegerType::getCharTy());
      Type charPtr(PointerType::get(charTy));

      return ReturnMemberExpr(node, charPtr);
   }

   return ReturnMemberExpr(node, Type(ObjectType::get("String")));
}

Type TypeCheckPass::visit(StringInterpolation *node)
{
   bool first = true;
   size_t i = 0;
   for (auto& expr : node->strings) {
      auto val = expr->accept(*this);
      expr->addUse();
      toRvalueIfNecessary(val, expr);

      auto StringRepr = Type(ObjectType::get("StringRepresentable"));
      while (val->isPointerTy()) {
         auto pointee = val->asPointerTy()->getPointeeType();
         lvalueToRvalue(expr);
         val = pointee;
      }

      if (val->isObject()) {
         auto cl = SymbolTable::getClass(val->getClassName());
         if (cl->conformsTo("StringRepresentable")) {
            if (!first) {
               wrapImplicitCast(expr, val, StringRepr);
            }

            first = false;
            ++i;
            continue;
         }
      }
      else if (isa<PrimitiveType>(*val)) {
         assert(!first && "first element should always be string");

         auto boxed = Type(val->box());
         wrapImplicitCast(expr, val, boxed);
         wrapImplicitCast(expr, boxed, StringRepr);

         first = false;
         ++i;
         continue;
      }

      diag::err(err_not_string_representable) << expr << diag::term;
   }

   return ReturnMemberExpr(node, Type(ObjectType::get("String")));
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(SubscriptExpr *node)
{
   auto ts = popTy();
   if (!ts->isObject()) {
      Type int64Ty(IntegerType::get());
      node->_index->setContextualType(int64Ty);
   }

   Type index = node->_index->accept(*this);
   node->_index->addUse();

   if (ts->isObject())
   {
      auto& className = ts->getClassName();
      auto cl = SymbolTable::getClass(className, importedNamespaces);
      std::vector<Argument> args{ Argument("", index) };
      string op = "postfix []";

      auto methodResult = cl->hasMethod(op, args, {}, *ts);
      if (methodResult.isCompatible()) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->_index },
            op
         );

         call->loc = node->loc;
         call->parentExpr = node;
         call->parent = node;
         call->resolvedArgs.push_back(std::move(args.front()));

         auto expr = std::static_pointer_cast<Expression>(call);
         node->children.push_back(&expr);

         node->overridenCall = call;
         node->is_subscript_op = true;

         pushTy(ts);
         auto type = call->accept(*this);

         return ReturnMemberExpr(node, type);
      }
      else {
         diag::err(err_illegal_subscript) << ts << node << diag::term;
      }
   }

   if (!ts->isPointerTy()) {
      diag::err(err_illegal_subscript) << ts << node << diag::term;
   }

   toRvalueIfNecessary(index, node->_index);

   Type int64Ty(IntegerType::get(64));
   if (!index->isNumeric() && !index->isBoxedPrimitive()) {
      diag::err(err_subscript_index_not_integral) << node << diag::term;
   }
   else if (index != int64Ty) {
      wrapImplicitCast(node->_index, index, int64Ty);
   }

   ts = ts->asPointerTy()->getPointeeType();
   ts.isLvalue(true);

   return ReturnMemberExpr(node, ts);
}

void TypeCheckPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>>& args,
   std::vector<Argument> &givenArgs,
   CallCompatability &comp)
{
   if (comp.func == nullptr && comp.method == nullptr) {
      return;
   }

   size_t i = 0;
   auto& declaredArgs = comp.func != nullptr ? comp.func->getArguments()
                                             : comp.method->arguments;

   std::vector<pair<string, std::shared_ptr<Expression>>> realArgs;
   for (const auto &order : comp.argOrder) {
      if (order.second) {
         auto& arg = declaredArgs[order.first];
         realArgs.emplace_back(arg.label, arg.defaultVal);
      }
      else {
         realArgs.push_back(args[order.first]);
      }
   }

   args = realArgs;
   PrepareCallArgs(args, givenArgs, declaredArgs, comp.resolvedArgs);
}

void TypeCheckPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>> &args,
   std::vector<Argument> &givenArgs,
   std::vector<Argument> &declaredArgs,
   std::vector<Argument> &resolvedArgs)
{
   size_t i = 0;
   bool vararg = !declaredArgs.empty() && declaredArgs.back().cstyleVararg;
   for (auto& arg : args) {
      arg.second->addUse();

      if (i >= givenArgs.size()) {
         break;
      }

      auto &given = givenArgs[i].type;
      if (given->isAutoTy()) {
         given = resolvedArgs[i].type;
      }

      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)) {
         // assume that c style varargs do not require an lvalue
         toRvalueIfNecessary(given, arg.second);
         ++i;
         continue;
      }

      auto &declared = i >= declaredArgs.size() ? declaredArgs.back().type : declaredArgs[i].type;
      if (declared->isStruct() && given->isStruct() && !declared.isLvalue()) {
         arg.second->byval_pass = true;
      }

      toRvalueIfNecessary(given, arg.second, !declared.isLvalue());
      ++i;
   }
}

void TypeCheckPass::HandleFunctionCall(CallExpr *node)
{
   auto& resolvedArgs = node->resolvedArgs;
   auto result = getFunction(node->ident, resolvedArgs, node->resolvedGenerics);
   if (!result.isCompatible()) {
      // lambda or saved function call
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         auto fun = var.first.first.type;

         if (fun->isFunctionTy()) {
            pushTy(fun);
            node->type = CallType::ANON_CALL;
            node->binding = var.first.second;

            if (latestScope->inLambda && var.second) {
               latestScope->captures->emplace_back(var.first.second, var.first.first.type);
               node->isCapturedVar = true;
            }

            return HandleAnonCall(node);
         }
      }

      // possible implicit method call
      auto& currentCl = currentClass();
      if (!currentCl.empty() && SymbolTable::hasClass(currentCl)) {
         auto cl = SymbolTable::getClass(currentCl);

         auto compat = cl->hasMethod(node->ident, resolvedArgs, {}, ObjectType::get(currentCl));
         if (compat.compatibility == CompatibilityType::COMPATIBLE) {
            auto selfTy = Type(ObjectType::get(currentCl));
            pushTy(selfTy);

            node->type = CallType::METHOD_CALL;
            node->implicitSelfCall = true;
            node->selfBinding = latestScope->currentSelf;

            return HandleMethodCall(node);
         }
      }

      if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
         diag::err(err_no_matching_call) << 0 << node->ident << node << diag::cont;
         size_t i = result.failedCandidates.size();
         for (const auto& cand : result.failedCandidates) {
            auto& func = cand.func;
            if (cand.failedConstraint != nullptr) {
               diag::note(note_cand_failed_constraint) << cand.failedConstraint->reportFailure()
                                                       << func->getDecl()
                                                       << diag::cont;
            }
            else if (node->resolvedGenerics.size() < func->getGenerics().size()) {
               auto &missingGen = func->getGenerics()[node->resolvedGenerics.size()];
               diag::note(note_cand_generic_type_not_inferred) << missingGen.genericTypeName
                                                               << func->getDecl() << diag::cont;
            }
            else if (node->resolvedArgs.size() != func->getArguments().size()) {
               diag::note(note_cand_mismatched_arg_count) << func->getArguments().size()
                                                          << node->resolvedArgs.size()
                                                          << func->getDecl()
                                                          << diag::whole_line << diag::cont;
            }
            else {
               auto &givenArg = node->resolvedArgs[cand.incompatibleArg];
               auto &neededArg = func->getArguments()[cand.incompatibleArg];
               diag::note(note_cand_no_implicit_conv) << givenArg.type << neededArg.type
                                                      << cand.incompatibleArg + 1
                                                      << func->getDecl()->args[cand.incompatibleArg]
                                                      << diag::cont;
            }
         }

         exit(1);
      }
      if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
         diag::err(err_func_not_found) << 0 << node->ident << node << diag::term;
      }

      llvm_unreachable("No other options possible");
   }

   node->resolvedGenerics = result.generics;
   node->resolvedArgs = result.resolvedArgs;

   auto& func = result.func;
   func->addUse();

   auto& declaredArgs = func->getArguments();
   PrepareCallArgs(node->args, resolvedArgs, result);
   ApplyCasts(node->args, resolvedArgs, declaredArgs, result.neededCasts);

   node->binding = SymbolTable::mangleFunction(func->getName(), declaredArgs);
   node->declaredArgTypes = &func->getArguments();
   node->func = func;

   if (func->hasHiddenParam()) {
      node->hasStructReturn = true;
      node->structReturnType = *func->getReturnType();
   }

   auto& retTy = func->getReturnType();
   if (retTy->isGeneric() || (retTy->isObject() && retTy->getRecord()->isGeneric())) {
      auto genericTy = retTy;

      // the covariance of the generic type to cast from
      node->genericOriginTy = genericTy;

      // changes the generic type (i.e. T) with the concrete one
      resolveGenerics(retTy, node->resolvedGenerics);

      node->returnType = retTy;
      node->needsGenericCast = true;

      // concrete generic type of this particular instance
      node->genericDestTy = retTy;
   }
   else {
      node->returnType = func->getReturnType();
   }
}

void TypeCheckPass::HandleBuiltinCall(CallExpr *node)
{
   node->isBuiltin = true;

   auto& Builtin = util::builtinTypes[node->ident];
   if (!Builtin.second.empty()) {
      node->returnType = Type(Builtin.second[0]);
   }

   node->builtinFnKind = Builtin.first;
   switch (node->builtinFnKind) {
      case BuiltinFn::ALIGNOF:
      case BuiltinFn::SIZEOF:
      case BuiltinFn::TYPEOF:
      case BuiltinFn::NULLPTR: {
         if (node->args.size() != 1 || node->args.front().second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as argument to " + node->ident, node);
         }

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         if (node->builtinFnKind == BuiltinFn::SIZEOF || node->builtinFnKind == BuiltinFn::ALIGNOF) {
            bool needsCast = false;
            if (node->contextualType->isIntegerTy()) {
               node->returnType = node->contextualType;
               needsCast = true;
            }
            else if (node->contextualType->isBoxedPrimitive()) {
               auto unboxed = node->contextualType->unbox();
               if (unboxed->isIntegerTy()) {
                  node->returnType = node->contextualType;
                  needsCast = true;
               }
            }
            else if (!node->contextualType->isAutoTy()) {
               diag::err(err_type_mismatch) << node->returnType << node->contextualType
                                            << node << diag::term;
            }

            if (needsCast) {
               node->needs_cast = true;
               *node->castFrom = IntegerType::get()->box();
               node->castTo = node->returnType;
            }
         }

         if (node->builtinFnKind == BuiltinFn::NULLPTR) {
            auto ty = typeref->getType();
            bool refcounted = false;
            if (ty->isObject()) {
               refcounted = ty->getRecord()->isRefcounted();
            }

            if (!refcounted) {
               *node->returnType = ty->getPointerTo();
            }
            else {
               node->returnType = ty;
            }
         }

         break;
      }
      case BuiltinFn::BITCAST: {
         if (node->args.size() != 2 || node->args.at(1).second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as second argument to " + node->ident, node);
         }

         auto& arg = node->args.front();
         node->resolvedArgs.emplace_back(arg.first, arg.second->accept(*this), arg.second);

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(1).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         toRvalueIfNecessary(node->resolvedArgs.front().type, node->args.front().second);
         node->returnType = typeref->getType();

         break;
      }
      case BuiltinFn::ISNULL: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         auto& arg = node->args.front();
         auto ty = arg.second->accept(*this);
         node->resolvedArgs.emplace_back(arg.first, ty, arg.second);

         if (!ty->isPointerTy()) {
            diag::err(err_unexpected_value) << "pointer type"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::UNWRAP_PROTO: {
         if (node->args.size() != 1) {
            diag::err(err_incomp_arg_counts) << 1 << node->args.size()
                                             << node << diag::term;
         }

         auto& arg = node->args.front();
         auto ty = arg.second->accept(*this);
         node->resolvedArgs.emplace_back(arg.first, ty, arg.second);

         if (!ty->isProtocol()) {
            diag::err(err_unexpected_value) << "protocol"
                                            << arg.second << diag::term;
         }

         toRvalueIfNecessary(ty, arg.second);
         break;
      }
      case BuiltinFn::STACK_ALLOC: {
         if (node->args.size() != 2) {
            diag::err(err_incomp_arg_counts) << 2 << node->args.size()
                                             << node << diag::term;
         }

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         auto& arg = node->args[1].second;
         auto ty = typeref->getType();
         toRvalueIfNecessary(ty, arg);

         auto sizeTy = arg->accept(*this);
         if (!sizeTy->isIntegerTy()) {
            wrapImplicitCast(arg, sizeTy, Type(IntegerType::get()));
         }

         *node->returnType = ty->getPointerTo();

         break;
      }
      case BuiltinFn::MEMCPY:
      case BuiltinFn::MEMSET:
         return HandleFunctionCall(node);
   }
}

void TypeCheckPass::HandleEnumCase(CallExpr *node)
{
   auto cl = SymbolTable::getClass(node->className);
   if (!cl->isEnum()) {
      return;
   }

   auto en = static_cast<Enum *>(cl);
   auto res = en->hasCase(node->ident, node->resolvedArgs, node->resolvedGenerics);
   if (res.compatibility != CompatibilityType::COMPATIBLE) {
//      diag::err(err_enum_case_not_found) << node->className << node->ident << true
//                                         << node << diag::term;
   }

   auto &case_ = en->getCase(node->ident);
   std::vector<Argument> neededArgs;
   for (const auto &ty : case_.associatedValues) {
      neededArgs.emplace_back(ty.first, ty.second);
   }

   node->resolvedGenerics = res.generics;
   node->resolvedArgs = res.resolvedArgs;

   PrepareCallArgs(node->args, node->resolvedArgs, res);
   ApplyCasts(node->args, node->resolvedArgs, neededArgs, res.neededCasts);

   node->enum_case = true;
   node->caseVal = case_.rawValue;
   node->resolvedGenerics = res.generics;

   std::vector<Type > enumCaseTypes;
   for (const auto &arg : neededArgs) {
      enumCaseTypes.push_back(arg.type);
   }

   auto obj = ObjectType::get(node->className, node->resolvedGenerics);
   node->returnType = Type(obj);
}

void TypeCheckPass::HandleMethodCall(
   CallExpr *node)
{
   Class* cl;
   string className;
   string fullName;
   Type latest;

   if (node->parentExpr == nullptr && node->enum_case) {
      auto& inf = node->contextualType;
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise("Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      node->isNsMember = true;

      return HandleEnumCase(node);
   }

   if (node->isNsMember) {
      fullName = node->className + "." + node->ident;

      if (SymbolTable::hasClass(fullName)) {
         node->type = CallType::CONSTR_CALL;
         node->ident = fullName;

         return HandleConstructorCall(node);
      }

      if (SymbolTable::numFunctionsWithName(fullName) > 0) {
         node->type = CallType::FUNC_CALL;
         node->ident = fullName;

         return HandleFunctionCall(node);
      }

      if (!SymbolTable::hasClass(node->className)) {
         RuntimeError::raise("Function " + fullName + " does not exist", node);
      }

      HandleEnumCase(node);
      if (!node->returnType->isAutoTy()) {
         return;
      }

      cl = SymbolTable::getClass(node->className);
      className = node->className;
   }
   else {
      assert(!typeStack.empty() && "Nothing to call method on!");

      latest = popTy();
      if (node->isPointerAccess_) {
         if (!latest->isPointerTy()) {
            RuntimeError::raise("Value is not a pointer", node);
         }

         latest = latest->asPointerTy()->getPointeeType();
      }

      if (!latest->isObject()) {
         RuntimeError::raise("Cannot call method on value of type " + latest.toString(), node);
      }

      className = latest->getClassName();
      if (!SymbolTable::hasClass(className)) {
         RuntimeError::raise("Class " + latest.toString() + " does not exist", node);
      }

      cl = SymbolTable::getClass(className, importedNamespaces);
      node->className = className;

      if (cl->isProtocol()) {
         node->castFrom = latest;
         node->isProtocolCall = true;
      }
   }

   checkClassAccessibility(cl, node);

   BuiltinType* caller = node->isNsMember ? nullptr : *latest;
   auto &givenArgs = node->resolvedArgs;
   auto methodResult = cl->hasMethod(node->ident, givenArgs, node->resolvedGenerics, caller);

   node->resolvedGenerics = methodResult.generics;

   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      if (cl->hasField(node->ident)) {
         auto field = cl->getField(node->ident);
         Type fieldTy(field->fieldType);
         if (fieldTy->isFunctionTy()) {
            auto anon = std::make_shared<CallExpr>(
               CallType::ANON_CALL,
               std::move(node->args)
            );

            CopyNodeProperties(node, anon.get());

            anon->parentExpr = node;
            anon->resolvedArgs = std::move(node->resolvedArgs);
            node->memberExpr = anon;

            node->returnType = fieldTy;
            node->returnType.isLvalue(true);

            node->setIsAnonymousCall(true);
            node->setAnonymousFieldIndex(cl->getFieldOffset(field->fieldName));

            return;
         }
      }

      node->resolvedArgs = methodResult.resolvedArgs;
      throwMethodNotFound(methodResult, node, cl);
   }

   auto& method = methodResult.method;
   ++method->uses;

   // check if method is accessible from current context
   checkMemberAccessibility(cl, method->methodName, method->accessModifier, node);

   auto ty = method->returnType;
   bool isGeneric = false;
   if (method->isStatic) {
      isGeneric = castGenericIfNecessary(node, node->resolvedGenerics,
         ty, currentClass() != cl->getName());
   }
   else {
      isGeneric = castGenericIfNecessary(node, latest->getConcreteGenericTypes(),
         ty, currentClass() != cl->getName());
   }

   if (isGeneric) {
      node->returnType = node->castTo;
      node->returnType.isLvalue(method->returnType.isLvalue());
   }
   else {
      node->returnType = ty;
   }

   // returning an object with non-resolved generics
   if (method->returnType->isObject()) {
      if (method->isStatic) {
         resolveGenerics(node->returnType, node->resolvedGenerics);
      }
      else {
         resolveGenerics(node->returnType, latest->getConcreteGenericTypes());
      }
   }

   if (method->isProtocolDefaultImpl) {
      assert(node->parentExpr != nullptr && "No parent expression to cast!");
      node->parentExpr->needs_cast = true;
      node->parentExpr->castFrom = latest;
      node->parentExpr->castTo = Type(ObjectType::get(method->protocolName));
   }

   // methods with 'ref' return type
   node->returnType.isLvalue(method->returnType.isLvalue());
   node->binding = method->mangledName;
   node->method = method;
   node->is_virtual = cl->isAbstract() || cl->isVirtual(method);
   node->isStatic = method->isStatic;

   auto& declaredArgs = method->arguments;
   PrepareCallArgs(node->args, givenArgs, methodResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, methodResult.neededCasts);

   node->declaredArgTypes = &method->arguments;

   // check if this method has a hidden byval struct parameter
   if (method->hasStructReturn) {
      node->hasStructReturn = true;
      node->structReturnType = *method->returnType;
   }

   // if we call another constructor, we can assume that all fields
   // will be initialized there, otherwise that constructor will fail.
   // this will accept circular calls as valid, but those will cause
   // other problems anyway
   // FIXME calls in conditions
   auto& uninitializedFields = latestScope->uninitializedFields;
   if (method->methodName == "init" && currentClass() == cl->getName() &&
      uninitializedFields != nullptr)
   {
      uninitializedFields->clear();
   }
}

void TypeCheckPass::HandleConstructorCall(CallExpr *node)
{
   auto record = SymbolTable::getRecord(node->ident, importedNamespaces);
   if (record->isUnion()) {
      if (node->resolvedArgs.size() != 1) {
         diag::err(err_union_initializer_arg_count) << node << diag::term;
      }

      auto& ty = node->resolvedArgs.front().type;
      auto un = record->getAs<Union>();

      if (!un->initializableWith(*ty)) {
         diag::err(err_union_initializer_type) << node << diag::term;
      }

      toRvalueIfNecessary(ty, node->args.front().second);

      *node->returnType = ObjectType::get(un->getName());
      node->setUnionConstr(true);
      node->type = CallType::CONSTR_CALL;

      return;
   }

   Class* cl = record->getAs<Class>();
   checkClassAccessibility(cl, node);

   // resolve generics
   size_t i = 0;
   auto& classGenerics = cl->getGenerics();
   for (const auto& gen : node->generics) {
      if (i < classGenerics.size()) {
         gen->accept(*this);
         auto resolved = GenericType::get(classGenerics[i].genericTypeName, *gen->getType());

         node->resolvedGenerics.push_back(resolved);
         ++i;
      }
      else {
         break;
      }
   }

   string methodName = "init";
   auto& givenArgs = node->resolvedArgs;
   auto constrResult = cl->hasMethod(methodName, givenArgs, node->resolvedGenerics);

   node->resolvedGenerics = constrResult.generics;
   node->resolvedArgs = constrResult.resolvedArgs;

   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(constrResult, node, cl);
   }

   auto& method = constrResult.method;
   node->method = method;

   ++method->uses;
   cl->addUse();

   // check accessibility
   checkMemberAccessibility(cl, method->methodName, method->accessModifier, node);

   node->ident = cl->getName();
   if (cl->isAbstract()) {
      RuntimeError::raise("Cannot instantiate abstract class " + node->ident, node);
   }

   ObjectType* returnType = ObjectType::get(node->ident, node->resolvedGenerics);
   *node->returnType = returnType;

   node->type = CallType::CONSTR_CALL;
   node->binding = method->mangledName;

   auto& declaredArgs = method->arguments;
   PrepareCallArgs(node->args, givenArgs, constrResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, constrResult.neededCasts);

   node->declaredArgTypes = &method->arguments;
}

void TypeCheckPass::throwMethodNotFound(
   CallCompatability &result,
   CallExpr *node,
   Class *cl)
{
   diag::err(err_no_matching_call) << 1 << node->ident
                                   << node << diag::cont;

   size_t i = result.failedCandidates.size();
   for (const auto& cand : result.failedCandidates) {
      auto& method = cand.method;
      if (cand.failedConstraint != nullptr) {
         diag::note(note_cand_failed_constraint) << cand.failedConstraint->reportFailure()
                                                 << method->loc
                                                 << diag::cont;
      }
      else if (node->resolvedGenerics.size() < method->generics.size()) {
         auto &missingGen = method->generics[node->resolvedGenerics.size()];
         diag::note(note_cand_generic_type_not_inferred) << missingGen.genericTypeName
                                                         << method->declaration << diag::cont;
      }
      else if (node->args.size() != method->arguments.size()) {
         diag::note(note_cand_mismatched_arg_count) << method->arguments.size()
                                                    << node->args.size()
                                                    << method->loc
                                                    << diag::cont;
      }
      else {
         auto &givenArg = node->resolvedArgs[cand.incompatibleArg];
         auto &neededArg = method->arguments[cand.incompatibleArg];
         diag::note(note_cand_no_implicit_conv) << givenArg.type << neededArg.type
                                                << cand.incompatibleArg + 1
                                                << method->loc << diag::cont;
      }
   }

   exit(1);
}

void TypeCheckPass::HandleCallOperator(CallExpr *node)
{
   auto latest = popTy();
   if (latest->isObject() || !SymbolTable::hasClass(latest->getClassName())) {
      pushTy(latest);
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   string methodName = "postfix ()";
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   auto& givenArgs = node->resolvedArgs;
   auto callOpResult = cl->hasMethod(methodName, givenArgs, {}, *latest);

   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Call operator with signature " + util::args_to_string(givenArgs) + " does"
         " not exist on class " + className, node);
   }

   node->resolvedArgs = callOpResult.resolvedArgs;

   auto& method = callOpResult.method;
   node->type = CallType::METHOD_CALL;
   node->ident = "postfix ()";
   node->binding = method->mangledName;

   auto& declaredArgs = method->arguments;
   PrepareCallArgs(node->args, givenArgs, callOpResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, callOpResult.neededCasts);

   node->returnType = method->returnType;
   node->declaredArgTypes = &method->arguments;
}

void TypeCheckPass::HandleAnonCall(CallExpr *node)
{
   auto latest = popTy();
   auto& givenArgs = node->resolvedArgs;

   if (latest->isPointerTy()) {
      latest = latest->asPointerTy()->getPointeeType();
   }

   if (!latest->isFunctionTy()) {
      RuntimeError::raise("Given object of type " + latest.toString() + " is not callable", node);
   }

   auto func = cast<FunctionType>(*latest);

   std::vector<GenericType*> givenGenerics;
   std::vector<GenericConstraint> neededGenerics;
   std::vector<Argument> neededArgs(func->getArgTypes());

   auto result = util::findMatchingCall(givenArgs, neededArgs, givenGenerics, neededGenerics);
   if (!result.isCompatible()) {
      diag::err(err_no_matching_call) << 0 << node->ident << node << diag::term;
   }

   node->type = CallType::ANON_CALL;
   node->resolvedArgs = result.resolvedArgs;

   PrepareCallArgs(node->args, givenArgs, neededArgs, result.resolvedArgs);
   ApplyCasts(node->args, givenArgs, neededArgs, result.neededCasts);

   node->returnType = func->getReturnType();
   node->functionType = func;

   if (node->returnType->isStruct()) {
      node->hasStructReturn = true;
      node->structReturnType = *node->returnType;
   }
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Type TypeCheckPass::visit(
   CallExpr *node)
{
   //MAINCALL
   for (size_t i = node->resolvedArgs.size(); i < node->args.size(); ++i) {
      auto& arg = node->args.at(i);
      Type ty;
      if (!arg.second->needsContextualInformation()) {
         ty = arg.second->accept(*this);
      }

      node->resolvedArgs.emplace_back(arg.first, ty, arg.second);
   }

   if (node->type == CallType::FUNC_CALL) {
      if (util::in_vector(util::builtinFunctions, node->ident)) {
         HandleBuiltinCall(node);
      }
      else if (node->ident.empty()) {
         HandleCallOperator(node);
      }
      else if (SymbolTable::hasRecord(node->ident, importedNamespaces)) {
         HandleConstructorCall(node);
      }
      else {
         HandleFunctionCall(node);
      }
   }
   else if (node->type == CallType::METHOD_CALL) {
      HandleMethodCall(node);
   }
   else if (node->type == CallType::ANON_CALL) {
      HandleAnonCall(node);
   }

   return ReturnMemberExpr(node, node->returnType);
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(MemberRefExpr *node)
{
   Type latest;
   string className;
   Class* cl;

   if (node->parentExpr == nullptr && node->enum_case) {
      auto& inf = node->contextualType;
      if (inf->isAutoTy() || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise("Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      node->isNsMember = true;
   }

   if (node->isNsMember) {
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         node->binding = var.first.second;

         auto res = var.first.first.type;
         res.isLvalue(true);

         return ReturnMemberExpr(node, res);
      }

      cl = SymbolTable::getClass(node->className);
      checkClassAccessibility(cl, node);

      if (!hasVariable(node->ident) && SymbolTable::hasClass(node->className)) {
         node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);

         if (cl->isEnum()) {
            auto en = static_cast<Enum*>(cl);

            size_t genericParamsGiven = 0;
            if (!node->contextualType->isAutoTy() && node->contextualType->isObject()) {
               genericParamsGiven = node->contextualType->getConcreteGenericTypes().size();
            }

            if (en->isGeneric() && genericParamsGiven < en->getGenerics().size()) {
               RuntimeError::raise("Could not infer generic type " +
                     en->getGenerics()[genericParamsGiven].genericTypeName, node);
            }
            if (!en->hasCase(node->ident)) {
               diag::err(err_enum_case_not_found) << node->className << node->ident << false
                                                  << node << diag::term;
            }

            auto obj = ObjectType::get(node->className);
            if (en->isGeneric()) {
               for (const auto &gen : node->contextualType->getConcreteGenericTypes()) {
                  obj->getConcreteGenericTypes().push_back(gen);
               }
            }

            node->enum_case = true;
            node->caseVal = en->getCase(node->ident).rawValue;

            return ReturnMemberExpr(node, Type(obj));
         }
      }

      goto err;
   }

   latest = popTy();
   if (node->isPointerAccess) {
      if (!latest->isPointerTy()) {
         RuntimeError::raise("Value is not a pointer", node);
      }

      latest = latest->asPointerTy()->getPointeeType();
   }

   className = latest->getClassName();

   if (node->isTupleAccess) {
      if (!latest->isTupleTy()) {
         RuntimeError::raise("Cannot access indexed property on non-tuple value of type "
                     + latest.toString(), node);
      }

      auto asTuple = cast<TupleType>(*latest);
      if (asTuple->getArity() <= node->tupleIndex) {
         RuntimeError::raise("Cannot access index " + std::to_string(node->tupleIndex) + " on tuple "
                     "with arity " + std::to_string(asTuple->getArity()), node);
      }

      *node->fieldType = asTuple->getContainedType(node->tupleIndex);
      node->fieldType.isLvalue(true);
      node->fieldType.isConst(latest.isConst());

      return ReturnMemberExpr(node, node->fieldType);
   }

   if (className.empty()) {
      RuntimeError::raise("Cannot access property " + node->ident + " on value of type "
               + latest.toString(), node);
   }

   node->className = className;

   if (SymbolTable::hasUnion(className, importedNamespaces)) {
      CheckUnionAccess(node);
      return ReturnMemberExpr(node, node->fieldType);
   }

   cl = SymbolTable::getClass(className, importedNamespaces);
   checkClassAccessibility(cl, node);

   if (cl->isEnum() && node->ident == "rawValue") {
      *node->fieldType = IntegerType::get(64);
      node->isEnumRawValue = true;

      return ReturnMemberExpr(node, node->fieldType);
   }
   else if (cl->hasField(node->ident)) {
      Field *field = cl->getField(node->ident);

      // if this field needed initializing and we're in a constructor, erase it from the needed fields
      bool isUninitializedField = false;
      auto& uninitializedFields = latestScope->uninitializedFields;
      if (currentClass() == className && node->lhs_of_assignment && uninitializedFields != nullptr) {
         auto index = std::find(uninitializedFields->begin(), uninitializedFields->end(), node->ident);
         if (index != uninitializedFields->end()) {
            uninitializedFields->erase(index);
            isUninitializedField = true;
         }
      }

      // Check if we're trying to assign a const field
      if (field->isConst && node->lhs_of_assignment && !isUninitializedField) {
         RuntimeError::raise("Field " + field->fieldName + " of " + cl->getTypeName() + " is constant", node);
      }

      // Use a getter if available
      if (field->hasGetter && !node->lhs_of_assignment && currentClass() != className) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            field->getterName

         );

         CopyNodeProperties(node, call.get());
         call->parentExpr = node->parentExpr;
         node->getterOrSetterCall = call;

         pushTy(latest);
         return call->accept(*this);
      }

      // Use a setter if available (and we're on the left side of an assignment)
      if (field->hasSetter && node->lhs_of_assignment && currentClass() != className) {
         Expression* current = node;
         node->setter_call = true;

         while (current->parentExpr != nullptr) {
            current = current->parentExpr;
         }

         current->setter_call = true;
         current->setterName = field->setterName;

         return latest;
      }

      auto& field_type = field->fieldType;
      auto& concreteGenerics = latest->getConcreteGenericTypes();

      Type ty(field->fieldType);
      auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass() != cl->getName());
      if (currentClass() == cl->getName()) {
         resolveGenerics(ty, *GenericsStack.back());
      }

      node->fieldType = ty;
      node->binding = field->mangledName;

      checkMemberAccessibility(cl, field->fieldName, field->accessModifier, node);

      node->fieldType.isLvalue(true);
      node->fieldType.isConst(field->isConst && !isUninitializedField);

      return ReturnMemberExpr(node, node->fieldType);
   }

   err:
   diag::err(err_member_not_found) << cl->getNameSelector() << cl->getName()
                                   << node->ident << node << diag::term;
   llvm_unreachable(0);
}

void TypeCheckPass::CheckUnionAccess(MemberRefExpr *node)
{
   auto un = SymbolTable::getUnion(node->className, importedNamespaces);
   if (!un->hasField(node->ident)) {
      diag::err(err_member_not_found) << 3 /*union*/ << node->ident
                                      << node << diag::term;
   }

   Type ty(un->getFieldTy(node->ident));
   ty.isLvalue(true);
   ty.isConst(un->isConst());

   node->fieldType = ty;
   node->setUnionAccess(true);
}

Type TypeCheckPass::HandleCastOp(
   Type& lhs,
   Type& rhs,
   BinaryOperator *node)
{
   if (node->rhs->get_type() != NodeType::TYPE_REF) {
      RuntimeError::raise("Expected type name after 'as'", node->rhs.get());
   }

   toRvalueIfNecessary(lhs, node->lhs);
   node->operandType = *lhs;

   if (node->op != "as!" && !lhs->explicitlyCastableTo(*rhs)) {
      RuntimeError::raise("Cannot cast from " + lhs.toString() + " to " +
         rhs.toString(), node);
   }

   // check if cast returns an optional
   if (lhs->isObject()) {
      auto fromClass = SymbolTable::getClass(lhs->getClassName());
      if (fromClass->isBaseClassOf(rhs->getClassName())) {
         SymbolTable::getClass(rhs->getClassName())->needsTypeInfoGen(true);

         return Type(ObjectType::getOptionOf(*rhs));
      }
   }

   node->castTo = rhs;
   return rhs;
}

Type TypeCheckPass::HandleAssignmentOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   if (!fst->isPointerTy()) {
      if (!snd.implicitlyCastableTo(fst)) {
         RuntimeError::raise("Cannot assign value of type " + snd.toString() + " to variable of "
            "type " + fst.toString(), node->lhs.get());
      }
      else if (fst != snd) {
         if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to " + fst.toString(), node);
         }

         wrapImplicitCast(node->rhs, snd, fst);
         snd = fst;
      }
   }

   auto prevOp = node->op;
   auto op = util::isAssignmentOperator(prevOp);

   if (op != "=") {
      auto binOp = new BinaryOperator(op);
      CopyNodeProperties(node, binOp);

      binOp->setLhs(node->lhs);
      binOp->setRhs(node->rhs);

      binOp->lhsType = fst;
      binOp->rhsType = snd;
      binOp->lhsIsBoxed = node->lhsIsBoxed;
      binOp->rhsIsBoxed = node->rhsIsBoxed;

      node->preAssignmentOp = binOp;
      binOp->accept(*this);
   }
   else {
      node->rhs->isAssigned(true);
   }

   if (snd->isVoidTy()) {
      node->isNullAssignment = true;
   }

   if (fst->isProtocol()) {
      node->isProtocolAssignment = true;
   }

   node->updateRefCount = fst->isObject() &&
      SymbolTable::getClass(fst->getClassName())->isRefcounted();

   node->operandType = *fst;
   return Type(VoidType::get());
}

Type TypeCheckPass::HandleArithmeticOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto& op = node->op;
   if ((op == "+" || op == "-") && fst->isPointerTy() &&
         (snd->isIntegerTy() || snd->isBoxedPrimitive() || snd->isPointerTy())) {
      bool isUnsigned = false;
      if (snd->isIntegerTy()) {
         isUnsigned = snd->isUnsigned();
      }
      else if (snd->isPointerTy()) {
         isUnsigned = true;
      }
      else {
         isUnsigned = snd->unbox()->isUnsigned();
      }

      Type int64Ty(IntegerType::get(sizeof(int*) * 8, isUnsigned));
      if (snd != int64Ty) {
         wrapImplicitCast(node->rhs, snd, int64Ty);
      }

      node->pointerArithmeticType = *fst->asPointerTy()->getPointeeType();
      node->operandType = *int64Ty;

      return fst;
   }

   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if (!fst->isNumeric() || !snd->isNumeric()) {
         goto err;
      }

      Type retTy(cast<PrimitiveType>(*fst)->ArithmeticReturnType(op, *snd));
      if (retTy->isVoidTy()) {
         goto err;
      }

      if (fst != retTy) {
         wrapImplicitCast(node->lhs, fst, retTy);
      }
      if (snd != retTy) {
         wrapImplicitCast(node->rhs, snd, retTy);
      }

      retTy.isLvalue(false);
      node->operandType = *retTy;

      return retTy;
   }

   if (op == "**") {
      auto int64Ty = Type(IntegerType::get());
      if (!snd->isIntegerTy()) {
         diag::err(err_incompatible_binop_types) << 1 /*right*/ << "**" << 0 /*integral*/
                                                 << node->rhs << diag::term;
      }

      Type retType = fst->isIntegerTy() ? fst : Type(FPType::getDoubleTy());
      Type doubleTy(FPType::getDoubleTy());

      if (fst != doubleTy) {
         wrapImplicitCast(node->lhs, fst, doubleTy);
      }
      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, int64Ty);
      }

      retType.isLvalue(false);
      node->operandType = *retType;

      return retType;
   }

   err:
   RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst.toString()
      + " and " + snd.toString(), node);
   llvm_unreachable("");
}

Type TypeCheckPass::HandleBitwiseOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto& op = node->op;
   if (!fst->isIntegerTy() || !snd->isIntegerTy()) {
      RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " +
         fst.toString() + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = *fst;
   return fst;
}

Type TypeCheckPass::HandleLogicalOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   auto boolTy = Type(IntegerType::get(1));
   if (!fst.implicitlyCastableTo(boolTy) && !snd.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
               .toString() + " and " + snd.toString(), node);
   }

   if (!fst->isInt1Ty()) {
      wrapImplicitCast(node->lhs, fst, boolTy);
   }
   if (!snd->isInt1Ty()) {
      wrapImplicitCast(node->rhs, snd, boolTy);
   }

   node->operandType = *boolTy;
   return boolTy;
}

void TypeCheckPass::HandleEnumComp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
//   node->isEnumComp = true;
//
//   auto fstHasKnownCase = fst->hasKnownEnumCase();
//   auto sndHasKnownCase = snd->hasKnownEnumCase();
//   if (!fstHasKnownCase && !sndHasKnownCase) {
//      return;
//   }
//
//   auto& associatedValues = fstHasKnownCase ? fst->getAssociatedTypes() : snd->getAssociatedTypes();
//   node->knownCase = fstHasKnownCase ? fst->getKnownEnumCase() : snd->getKnownEnumCase();
//   node->caseTypes = fstHasKnownCase ? fst->getKnownEnumCaseTypes() : snd->getKnownEnumCaseTypes();
//
//   size_t i = 0;
//   for (const auto& val : associatedValues) {
//      if (val.second->isUnderscore()) {
//         node->caseValues.push_back(nullptr);
//      }
//      else {
//         node->caseValues.push_back(val.second);
//      }
//      ++i;
//   }
}

void TypeCheckPass::HandleTupleComp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   assert(fst->isTupleTy() && snd->isTupleTy() && "Can't compare tuple with non-tuple!");

   auto fstAsTuple = cast<TupleType>(*fst);

   node->isTupleComp = true;
   node->arity = fstAsTuple->getArity();
   node->llvmTupleType = fstAsTuple->getLlvmType();
}

Type TypeCheckPass::HandleEqualityOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   // pointer comparison operators
   if (node->op.length() == 3) {
      if ((fst->isPointerTy() + snd->isPointerTy() + fst->isRefcounted() + snd->isRefcounted()) == 0) {
         RuntimeError::raise("Expected at least one operand of " + node->op + " to be a pointer", node);
      }

      auto wordTy = Type(IntegerType::get());
      if (!fst.implicitlyCastableTo(wordTy) || !fst.implicitlyCastableTo(wordTy)) {
         RuntimeError::raise("Binary operator " + node->op + " is not applicable to types " +
                     fst.toString() + " and " + snd.toString(), node);
      }

      if (!fst->isPtrSizedInt()) {
         wrapImplicitCast(node->lhs, fst, wordTy);
      }

      if (!snd->isPtrSizedInt()) {
         wrapImplicitCast(node->rhs, snd, wordTy);
      }

      node->operandType = *wordTy;
      return Type(IntegerType::getBoolTy());
   }

   if ((!fst->isNumeric() && !fst->isEnum()) && !fst->isTupleTy() && !fst->isPointerTy()) {
      RuntimeError::raise("Cannot compare values of type " + fst
               .toString() + " and " + snd.toString() + " for equality", node);
   }

   if (!snd.implicitlyCastableTo(fst)) {
      RuntimeError::raise("Cannot compare values of type " + fst
               .toString() + " and " + snd.toString() + " for equality", node);
   }

   if (fst->isEnum()) {
      HandleEnumComp(fst, snd, node);
   }

   if (fst->isTupleTy()) {
      HandleTupleComp(fst, snd, node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = *fst;
   return Type(IntegerType::getBoolTy());
}

Type TypeCheckPass::HandleComparisonOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   if (fst->isPointerTy()) {
      fst = fst->asPointerTy()->getPointeeType();
   }

   auto castFrom = snd;
   if (snd->isPointerTy()) {
      snd = snd->asPointerTy()->getPointeeType();
   }

   if (!fst->isNumeric() || !snd->isNumeric()) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
               .toString() + " and " + snd.toString(), node);
   }

   if (snd != fst) {
      wrapImplicitCast(node->rhs, castFrom, fst);
   }

   node->operandType = *fst;

   if (node->boxedPrimitiveOp) {
      return Type(ObjectType::get("Bool"));
   }

   return Type(IntegerType::getBoolTy());
}

Type TypeCheckPass::HandleOtherOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node)
{
   RuntimeError::raise("Binary operator " + node->op + " is not defined for arguments of type " +
         fst.toString() + " and " + snd.toString(), node);
   llvm_unreachable(0);
}

Type TypeCheckPass::HandleBinaryOperator(
   Type& lhs,
   Type&rhs,
   BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (opTy != BinaryOperatorType::CAST && opTy != BinaryOperatorType::ASSIGNMENT) {
      if (lhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(lhs->getClassName()));
         if (en->isRawEnum()) {
            auto rawTy = Type(en->getRawType());
            wrapImplicitCast(node->lhs, lhs, rawTy);
            lhs = rawTy;
         }
      }
      if (rhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(rhs->getClassName()));
         if (en->isRawEnum()) {
            auto rawTy = Type(en->getRawType());
            wrapImplicitCast(node->rhs, rhs, rawTy);
            rhs = rawTy;
         }
      }
   }

   switch (opTy) {
      case BinaryOperatorType::ASSIGNMENT:
         return HandleAssignmentOp(lhs, rhs, node);
      case BinaryOperatorType::ARITHMETIC:
         return HandleArithmeticOp(lhs, rhs, node);
      case BinaryOperatorType::BITWISE:
         return HandleBitwiseOp(lhs, rhs, node);
      case BinaryOperatorType::LOGICAL:
         return HandleLogicalOp(lhs, rhs, node);
      case BinaryOperatorType::EQUALITY:
         return HandleEqualityOp(lhs, rhs, node);
      case BinaryOperatorType::COMPARISON:
         return HandleComparisonOp(lhs, rhs, node);
      case BinaryOperatorType::CAST:
         return HandleCastOp(lhs, rhs, node);
      case BinaryOperatorType::OTHER:
         return HandleOtherOp(lhs, rhs, node);
   }
}

Type TypeCheckPass::tryBinaryOperatorMethod(
   Type& fst,
   Type& snd,
   BinaryOperator *node,
   string &opName,
   bool wasLvalue)
{
   std::vector<Argument> args{ Argument { "", snd } };
   if (node->opType == BinaryOperatorType::CAST) {
      args.pop_back();
   }

   cdot::cl::Class* cl = SymbolTable::getClass(fst->getClassName(), importedNamespaces);
   auto binOpResult = cl->hasMethod(opName, args, {}, *fst);
   if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->lhs);
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs }, opName);

      if (node->opType == BinaryOperatorType::CAST) {
         call->args.pop_back();
      }
      else {
         call->resolvedArgs.push_back(std::move(args.back()));
      }

      // we already resolved the argument, don't want to visit it again
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *fst;

      auto res = call->accept(*this);
      node->isTemporary(call->isTemporary());
      node->setTempType(call->getTempType());

      return res;
   }

   return {};
}

Type TypeCheckPass::tryFreeStandingBinaryOp(
   Type& fst,
   Type& snd,
   BinaryOperator *node,
   string &opName,
   bool wasLvalue)
{
   std::vector<Argument> args{ Argument{ "", fst }, Argument{ "", snd } };

   auto freeOp = getFunction(opName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      if (wasLvalue) {
         lvalueToRvalue(node->lhs);
      }

      pushTy(fst);
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->lhs, node->rhs },
         freeOp.func->getName());

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *fst;

      return call->accept(*this);;
   }

   return {};
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
Type TypeCheckPass::visit(BinaryOperator *node)
{
   node->lhs->addUse();
   node->rhs->addUse();

   //NONBINARY
   auto opType = cdot::getBinaryOpType(node->op);
   node->opType = opType;

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      node->lhs->isLhsOfAssigment();
   }

   node->lhs->setContextualType(node->contextualType);

   bool isPreAssignmentOp = !node->lhsType->isAutoTy();
   Type fst = isPreAssignmentOp ? node->lhsType
                                : node->lhs->accept(*this);

   if (node->lhs->setter_call) {
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs },
         node->lhs->setterName
      );

      node->overridenCall = call;
      node->operandType = *fst;

      CopyNodeProperties(node, call.get());
      call->setMemberExpr(node->memberExpr);
      call->setParent(node->parent);

      pushTy(fst);
      return call->accept(*this);
   }

   // checked if this is an assignment operator method, so the lhs will be lvalue to rvalue converted
   bool wasLvalue = fst.needsLvalueToRvalueConv() && isAssignment;

   if (isAssignment) {
      if (fst.isConst()) {
         RuntimeError::raise("Trying to reassign constant", node->lhs.get());
      }
      else if (!fst.isLvalue()) {
         if (fst.isSelf()) {
            RuntimeError::raise("Cannot assign to 'self' in non-mutating function", node->lhs.get());
         }

         RuntimeError::raise("Cannot assign to rvalue of type " + fst.toString(), node->lhs.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      fst.isLvalue(false);
      node->isStructAssignment = fst->isStruct();
      node->isSelfAssignment = fst.isSelf();
   }
   else {
      toRvalueIfNecessary(fst, node->lhs, node->op != "===");
   }

   if (fst->isBoxedPrimitive()) {
      node->rhs->setContextualType(Type(fst->unbox()));
   }
   else {
      node->rhs->setContextualType(fst);
   }

   Type snd = isPreAssignmentOp ? node->rhsType
                                : node->rhs->accept(*this);

   if (opType == BinaryOperatorType::CAST && node->rhs->get_type() == NodeType::TYPE_REF) {
      auto typeref = std::static_pointer_cast<TypeRef>(node->rhs);
      if (typeref->getType()->isBoxedPrimitive()) {
         node->boxedResultType = typeref->getType()->getClassName();
      }
   }

   if (!isPreAssignmentOp && (opType == BinaryOperatorType::ARITHMETIC || opType == BinaryOperatorType::EQUALITY ||
      opType == BinaryOperatorType::COMPARISON || opType == BinaryOperatorType::CAST ||
      opType == BinaryOperatorType::BITWISE || opType == BinaryOperatorType::LOGICAL))
   {
      node->lhsIsBoxed = fst->isBoxedPrimitive();
      node->rhsIsBoxed = snd->isBoxedPrimitive();

      if (node->lhsIsBoxed && (node->rhsIsBoxed || snd->isNumeric())) {
         auto unboxed = Type(fst->unbox());
         node->boxedPrimitiveOp = true;
         wrapImplicitCast(node->lhs, fst, unboxed);
         fst = unboxed;
      }
      if (node->rhsIsBoxed && (node->lhsIsBoxed || fst->isNumeric())) {
         auto unboxed = Type(snd->unbox());
         node->boxedPrimitiveOp = true;

         if (opType != BinaryOperatorType::CAST) {
            wrapImplicitCast(node->rhs, snd, unboxed);
         }

         snd = unboxed;
      }
   }

   string opName = opType == BinaryOperatorType::CAST ? "infix as " + snd.toString()
                                                      : "infix " + node->op;

   if (fst->isObject() && SymbolTable::hasClass(fst->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(fst, snd, node, opName, wasLvalue);
      if (!methodRes->isAutoTy()) {
         return methodRes;
      }
   }

   auto freeOpRes = tryFreeStandingBinaryOp(fst, snd, node, opName, wasLvalue);
   if (!freeOpRes->isAutoTy()) {
      return freeOpRes;
   }

   if (util::is_reversible(node->op) && node->lhs->needsContextualInformation()
      && !node->rhs->needsContextualInformation())
   {
      auto _lhsNode = node->lhs;
      node->lhs = node->rhs;
      node->rhs = _lhsNode;

      auto _lhsType = fst;
      fst = snd;
      snd = _lhsType;
   }

   toRvalueIfNecessary(snd, node->rhs, node->op != "===");

   auto res = HandleBinaryOperator(fst, snd, opType, node);
   if (!isPreAssignmentOp && node->boxedPrimitiveOp) {
      string className;
      if (opType == BinaryOperatorType::ARITHMETIC || opType == BinaryOperatorType::BITWISE) {
         if (res->isFloatTy()) {
            className = "Float";
         }
         else if (res->isDoubleTy()) {
            className = "Double";
         }
         else {
            assert(res->isIntegerTy() && "Unknown arithmetic return type!");
            if (res->isUnsigned()) {
               className += "U";
            }

            className += "Int";

            auto bitwidth = res->getBitwidth();
            className += std::to_string(bitwidth);
         }
      }
      else if (opType == BinaryOperatorType::COMPARISON || opType == BinaryOperatorType::EQUALITY ||
         opType == BinaryOperatorType::LOGICAL)
      {
         className = "Bool";
      }
      else if (opType == BinaryOperatorType::CAST) {
         className = node->boxedResultType;
      }

      if (!className.empty()) {
         node->boxedResultType = className;
         *res = ObjectType::get(className);
      }
   }

   return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(TertiaryOperator *node)
{
   Type cond = node->condition->accept(*this);
   node->condition->addUse();

   Type boolTy = Type(IntegerType::getBoolTy());
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition of tertiary operator '?:' must be boolean or implicitly castable"
         " to bool", node);
   }

   toRvalueIfNecessary(cond, node->condition);

   if (!node->contextualType->isAutoTy()) {
      node->lhs->setContextualType(node->contextualType);
      node->rhs->setContextualType(node->contextualType);
   }

   Type fst = node->lhs->accept(*this);
   Type snd = node->rhs->accept(*this);

   node->lhs->addUse();
   node->rhs->addUse();

   if (!fst.implicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot apply tertiary operator '?:' to values of type " + fst.toString() +
               " and " + snd.toString(), node);
   }
   else if (fst != snd) {
      if (warnCast(fst, snd)) {
//         Warning::issue("Implicit cast from " + snd.toString() + " to " + fst.toString(), node->rhs.get());
      }

      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->resultType = *fst;
   return fst;
}

Type TypeCheckPass::tryFreeStandingUnaryOp(
   Type& lhs,
   UnaryOperator *node,
   string &opName)
{
   std::vector<Argument> args{ Argument{"", lhs} };
   string methodName = (node->prefix ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      pushTy(lhs);
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL,
         std::vector<Expression::SharedPtr>{ node->target }, freeOp.func->getName()
      );

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = *lhs;

      return call->accept(*this);;
   }

   return {};
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(UnaryOperator *node)
{
   string& op = node->op;
   Type target = node->target->accept(*this);
   node->target->addUse();

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (!freeStanding->isAutoTy()) {
      return freeStanding;
   }

   if (target->isObject()) {
      auto& className = target->getClassName();

      auto class_decl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = class_decl->hasMethod((node->prefix ? "prefix " : "postfix ") + op, {}, {}, *target);

      if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{},
            (node->prefix ? "prefix " : "postfix ") + op);

         node->overridenCall = call;
         node->operandType = *target;
         call->memberExpr = node->memberExpr;

         toRvalueIfNecessary(target, node->target);
         pushTy(target);

         return call->accept(*this);
      }
   }

   if (op == "++" || op == "--") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to rvalue of "
                     "type " + target.toString(), node->target.get());
      }
      if (target.isConst()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to 'let' constant", node->target.get());
      }
      if (target->isPointerTy()) {
         node->isPointerArithmetic = true;
      }
      else if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target.toString(),
            node->target.get());
      }

      node->operandType = *target;
      target.isLvalue(false);

      return ReturnMemberExpr(node, target);
   }

   if (op == "*") {
      bool wasLvalue = target.isLvalue();

      toRvalueIfNecessary(target, node->target, !node->lhs_of_assignment);
      if (!target->isPointerTy()) {
         RuntimeError::raise("Cannot dereference non-pointer type", node->target.get());
      }

      node->needsDereferenceLoad = (wasLvalue || !node->lhs_of_assignment)
         && !target->isStruct();

      target = target->asPointerTy()->getPointeeType();
      if (node->lhs_of_assignment) {
         target.isLvalue(true);
      }

      node->operandType = *target;

      return ReturnMemberExpr(node, target);
   }

   if (op == "&") {
      if (!target.isLvalue()) {
         RuntimeError::raise("Cannot apply unary operator '&' to non-reference value", node->target.get());
      }

      target.isLvalue(false);
      *target = target->getPointerTo();

      return ReturnMemberExpr(node, target);
   }

   Type result;
   toRvalueIfNecessary(target, node->target);

   node->operandType = *target;

   if (op == "+" || op == "-") {
      if (!target->isNumeric()) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target.toString(),
            node->target.get());
      }
      if (op == "-" && target->isUnsigned()) {
         RuntimeError::raise("Unary operator '-' cannot be applied to unsigned integer", node->target.get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!target->isIntegerTy()) {
         RuntimeError::raise("Unary operator '~' is only applicable to type Int", node->target.get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = Type(IntegerType::get(1));
      if (!target.implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Unary operator '!' is not applicable to type " + target
                     .toString(), node->target.get());
      }

      if (target != boolTy) {
         if (!target->isObject() && target->getClassName() == "Bool") {
//            Warning::issue("Implicit cast to boolean", node->target.get());
         }
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise("Unary operator " + node->op + " is not defined on class " +
               target.toString(), node);
   }

   return ReturnMemberExpr(node, result);
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Type TypeCheckPass::visit(BreakStmt *node)
{
   break_(node);
   return {};
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Type TypeCheckPass::visit(ContinueStmt *node)
{
   continue_(node);
   return {};
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Type TypeCheckPass::visit(IfStmt *node)
{
   Type cond = node->condition->accept(*this);
   node->condition->addUse();
   toRvalueIfNecessary(cond, node->condition);

   auto boolTy = Type(IntegerType::get(1));
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond.implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition must be boolean", node->condition.get());
   }

   // if there's no else, the remaining code path needs to return either way
   if (node->elseBranch) {
      bool ifReturns = false;
      bool elseReturns = false;

      pushScope();
      node->ifBranch->accept(*this);
      ifReturns = latestScope->branches - latestScope->returned <= 0;
      popScope();

      pushScope();
      node->elseBranch->accept(*this);
      elseReturns = latestScope->branches - latestScope->returned <= 0;;
      popScope();

      // all branches return
      if (ifReturns && elseReturns) {
         latestScope->returned++;
      }
   }
   else {
      pushScope();
      node->ifBranch->accept(*this);
      popScope();
   }

   return {};
}

bool TypeCheckPass::matchableAgainst(
   Type& matchTy,
   std::shared_ptr<CaseStmt> const& case_)
{
   if (case_->isDefault) {
      return true;
   }

   auto& caseVal = case_->caseVal;
   if ((matchTy->isNumeric() || matchTy->isTupleTy()) && !matchTy->isEnum()) {
      caseVal->setContextualType(matchTy);
      auto givenTy = caseVal->accept(*this);
      case_->caseType = givenTy;

      return givenTy.implicitlyCastableTo(matchTy);
   }

   if (matchTy->isEnum() && caseVal->get_type() == NodeType::MEMBER_EXPR) {
      auto memExpr = std::static_pointer_cast<MemberRefExpr>(caseVal);
      if (!memExpr->enum_case) {
         return false;
      }

      auto en = SymbolTable::getClass(matchTy->getClassName())->getAs<Enum>();
      if (!en->hasCase(memExpr->ident)) {
         return false;
      }

      case_->enumCaseVal = &en->getCase(memExpr->ident);
      return true;
   }

   if (matchTy->isEnum() && caseVal->get_type() == NodeType::CALL_EXPR) {
      auto callExpr = std::static_pointer_cast<CallExpr>(caseVal);
      if (!callExpr->enum_case) {
         return false;
      }

      auto en = SymbolTable::getClass(matchTy->getClassName())->getAs<Enum>();
      auto& givenArgs = callExpr->resolvedArgs;
      givenArgs.reserve(callExpr->args.size());
      std::vector<string> letIdents;

      bool var = false;
      bool isLetExpr = false;
      for (const auto& arg : callExpr->args) {
         if (arg.second->get_type() == NodeType::IDENTIFIER_EXPR) {
            auto ident = std::static_pointer_cast<IdentifierRefExpr>(arg.second);
            if (ident->is_let_expr || ident->is_var_expr) {
               isLetExpr = true;
               var = ident->is_var_expr;
               letIdents.push_back(ident->ident);
               continue;
            }
         }

         if (isLetExpr) {
            RuntimeError::raise("Expected all arguments to be 'let' expressions", arg.second.get());
         }
      }

      if (isLetExpr) {
         if (en->hasCase(callExpr->ident)) {
            auto& val = en->getCase(callExpr->ident);
            if (val.associatedValues.size() == callExpr->args.size()) {
               if (var) {
                  case_->isEnumVarCase = true;
               }
               else {
                  case_->isEnumLetCase = true;
               }

               case_->enumCaseVal = &val;
               for (size_t i = 0; i < val.associatedValues.size(); ++i) {
                  case_->letIdentifiers.emplace_back(letIdents[i], Type(val.associatedValues[i].second));
               }

               return true;
            }
         }

         return false;
      }

      for (size_t i = givenArgs.size(); i < callExpr->args.size(); ++i) {
         const auto& arg = callExpr->args.at(i);
         givenArgs.emplace_back(arg.first, arg.second->accept(*this));
      }

      auto compat = en->hasCase(callExpr->ident, givenArgs, callExpr->resolvedGenerics);
      if (compat.compatibility != CompatibilityType::COMPATIBLE) {
         return false;
      }

      case_->enumCaseVal = &en->getCase(callExpr->ident);
      return true;
   }

   if (matchTy->isObject()) {
      caseVal->setContextualType(matchTy);

      auto givenTy = caseVal->accept(*this);
      case_->caseType = givenTy;

      auto rec = matchTy->getRecord();
      if (rec->isUnion()) {
         return false;
      }

      auto cl = rec->getAs<Class>();
      auto operatorEquals = cl->hasMethod("infix ==", { Argument{ "", givenTy } });
      if (operatorEquals.compatibility == CompatibilityType::COMPATIBLE) {
         case_->operatorEquals = operatorEquals.method;
         return true;
      }

      // otherwise === will be used
      if (cl->isNonUnion()) {
         return true;
      }

      return false;
   }

   return false;
}

Type TypeCheckPass::visit(MatchStmt *node)
{
   Type switchType = node->switchValue->accept(*this);
   toRvalueIfNecessary(switchType, node->switchValue);

   node->switchValue->addUse();

   if (switchType->isObject() && !switchType->isEnum()) {
      auto cl = SymbolTable::getClass(switchType->getClassName());
      string protName = "Equatable";

      if (!cl->conformsTo(protName)) {
         RuntimeError::raise("Types used as a match value must conform to 'Equatable'", node);
      }
   }
   else if (!switchType->isNumeric() && !switchType->isTupleTy() && !switchType->isEnum())
   {
      RuntimeError::raise("Types used as a match value must conform to 'Equatable'", node);
   }

   std::vector<long> rawCaseValues;
   std::vector<size_t> checkIfContinuable;
   size_t numCases = node->cases.size();
   unsigned i = 0;
   bool allCasesReturn = true;

   for (const auto& case_ : node->cases) {
      if (!matchableAgainst(switchType, case_)) {
         RuntimeError::raise("Cannot match given value against value of type " +
                     switchType.toString(), case_->caseVal.get());
      }
      else if (!case_->isDefault && case_->caseType != switchType) {
         wrapImplicitCast(case_->caseVal, case_->caseType, switchType);
      }

      if (case_->isDefault) {
         node->hasDefault = true;
         node->defaultIndex = i;
      }
      else {
         case_->caseVal->addUse();
      }

//      ++latestScope->branches;

      pushLoopScope(true, true);

      if (switchType->isEnum() && !case_->isDefault) {
         if (std::find(rawCaseValues.begin(), rawCaseValues.end(), case_->enumCaseVal->rawValue)
            != rawCaseValues.end())
         {
            RuntimeError::raise("Duplicate case " + case_->enumCaseVal->name, case_.get());
         }

         rawCaseValues.push_back(case_->enumCaseVal->rawValue);

         if (case_->isEnumLetCase || case_->isEnumVarCase) {
            auto en = SymbolTable::getClass(switchType->getClassName())->getAs<Enum>();

            for (auto& val : case_->letIdentifiers) {
               auto ty = val.second;

               ty.isLvalue(true);
               ty.isConst(case_->isEnumLetCase);

               case_->letBindings.push_back(declareVariable(val.first, ty, false, case_.get()));
            }
         }
      }

      if (case_->body) {
         case_->body->accept(*this);

         if (i == numCases - 1 && latestScope->continued) {
            RuntimeError::raise("Cannot continue from last case statement", case_.get());
         }
         if (latestScope->continued && i < numCases - 1) {
            checkIfContinuable.push_back(i);
         }

         bool caseReturns = latestScope->branches - latestScope->returned <= 0;
         popScope();

         if (!caseReturns) {
            allCasesReturn = false;
         }
      }

      ++i;
   }

   for (const auto& ind : checkIfContinuable) {
      auto nextCase = node->cases[ind + 1];
      if (nextCase->isEnumLetCase) {
         RuntimeError::raise("Cannot continue to case with 'let' expression", nextCase.get());
      }
   }

   bool isExhaustive = false;
   if (node->hasDefault) {
      isExhaustive = true;
   }
   else if (switchType->isEnum()) {
      auto numCases = static_cast<Enum*>(SymbolTable::getClass(switchType->getClassName()))->getNumCases();
      isExhaustive = numCases == node->cases.size();
   }

   if (!isExhaustive) {
      RuntimeError::raise("Match statements must be exhaustive", node);
   }
   if (allCasesReturn) {
      ++latestScope->returned;
   }

   node->switchType = *switchType;
   node->allCasesReturn = allCasesReturn;

   return {};
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
Type TypeCheckPass::visit(CaseStmt *node)
{
   return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type TypeCheckPass::visit(LabelStmt *node)
{
   if (std::find(labels.begin(), labels.end(), node->labelName) != labels.end()) {
      RuntimeError::raise("Label '" + node->labelName + "' already exists in the same scope", node);
   }

   labels.push_back(node->labelName);
   return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type TypeCheckPass::visit(GotoStmt *node)
{
   if (!has_label(node->labelName)) {
      RuntimeError::raise("No label '" + node->labelName + "' to go to", node);
   }

   return {};
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(FuncArgDecl *node)
{
   if (!node->argType->resolved) {
      node->argType->accept(*this);
   }

   auto ts = node->argType->getType();
   node->isStruct = ts->isStruct();
   node->mut = ts.isLvalue();

   if (node->mut) {
      node->isStruct = false; // we don't want to memcpy a ref parameter
      node->argType->type = ts;
   }

   if (node->defaultVal) {
      node->defaultVal->setContextualType(ts);
      node->defaultVal->addUse();

      Type defaultType = node->defaultVal->accept(*this);
      if (!defaultType.implicitlyCastableTo(ts)) {
         RuntimeError::raise("Default value for parameter " + node->argName + " must be of type "
            + node->argType->getType().toString(), node->defaultVal.get());
      }
   }

   return ts;
}

/**
 * Returns the current context
 * @param node
 * @return
 */
Type TypeCheckPass::visit(ReturnStmt *node)
{
   if (node->returnValue) {
      node->returnValue->addUse();
      node->returnValue->isAssigned(true);
      node->returnValue->setContextualType(latestScope->declaredReturnType);

      Type retType = node->returnValue->accept(*this);
      toRvalueIfNecessary(retType, node->returnValue, !latestScope->declaredReturnType.isLvalue());

      node->returnType = latestScope->declaredReturnType;
      return_(retType, node->returnValue.get());

      if (latestScope->declaredReturnType->needsStructReturn()) {
         node->returnValue->isHiddenReturnValue();
         node->returnValue->isPartOfReturnValue(true);
         node->hiddenParamReturn = true;
      }
      else {
         node->returnValue->isReturnValue();
      }

      if (node->returnValue->declaration) {
         node->returnValue->declaration->isReturnValue();
         if (latestScope->declaredReturnType->isStruct()) {
            node->returnValue->declaration->isHiddenReturnValue();
         }
      }

      if (retType != latestScope->declaredReturnType) {
         wrapImplicitCast(node->returnValue, retType, latestScope->declaredReturnType);
      }
   }
   else {
      *node->returnType = VoidType::get();
      return_(node->returnType, node);
   }

   return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type TypeCheckPass::visit(Expression *node)
{
   return {};
}


/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Type TypeCheckPass::visit(LambdaExpr *node)
{
   Type returnType = node->returnType->accept(*this);
   if (node->contextualType->isFunctionTy()) {
      auto asFunc = cast<FunctionType>(*node->contextualType);
      auto& neededArgs = asFunc->getArgTypes();

      if (neededArgs.size() != node->args.size()) {
         RuntimeError::raise("Incompatible argument counts: Expected " +
            std::to_string(neededArgs.size()) + ", but found " + std::to_string(node->args.size()), node);
      }

      size_t i = 0;
      for (const auto& arg : node->args) {
         if (arg->defaultVal != nullptr) {
            RuntimeError::raise("Lambda expression arguments cannot have default values", arg->defaultVal.get());
         }

         arg->accept(*this);
         auto given = arg->argType->getType();
         auto& needed = neededArgs[i].type;

         if (given->isAutoTy()) {
            arg->argType->setType(needed);
         }
         else if (!given.implicitlyCastableTo(needed)) {
            RuntimeError::raise("Incompatible argument types: No implicit conversion from " +
               given.toString() + " to " + needed.toString(), arg.get());
         }

         ++i;
      }

      auto declaredRetTy = asFunc->getReturnType();
      if (node->returnType->getType()->isAutoTy()) {
         returnType = declaredRetTy;
         node->returnType->setType(returnType);
      }
      else if (!returnType.implicitlyCastableTo(declaredRetTy)) {
         RuntimeError::raise("Incompatible return types: No implicit conversion from " +
            returnType.toString() + " to " + asFunc->getReturnType().toString(), node->returnType.get());
      }
   }

   bool isSingleStmt = node->body->get_type() != NodeType::COMPOUND_STMT;
   if (isSingleStmt && returnType->isAutoTy()) {
      auto asExpr = std::dynamic_pointer_cast<Expression>(node->body);
      if (!asExpr) {
         RuntimeError::raise("Expected single-statement lambda to contain a valid expression", node->body.get());
      }

      node->body = std::make_shared<ReturnStmt>(asExpr);
   }

   std::vector<Argument> argTypes;
   string anon = "__anon";
   std::vector<pair<string, Type>> captures;

   pushFunctionScope(returnType, "", "", true);
   latestScope->captures = &captures;

   for (const auto& arg : node->args) {
      if (!arg->argType->resolved) {
         arg->accept(*this);
      }
      if (arg->argType->getType()->isAutoTy()) {
         RuntimeError::raise("Could not infer type of argument " + arg->argName, arg.get());
      }

      argTypes.emplace_back("", arg->argType->getType());
      if (arg->argName != "_") {
         arg->binding = declareVariable(arg->argName, arg->argType->getType());
      }
   }

   auto ret = node->body->accept(*this);
   if (latestScope->returned > 0 && returnType->isAutoTy()) {
      if (isSingleStmt) {
         returnType = ret;
         node->returnType->setType(ret);
      }
      else {
         returnType = latestScope->declaredReturnType;
         node->returnType->setType(latestScope->declaredReturnType);
      }
   }
   else if (!ret.implicitlyCastableTo(returnType)) {
      RuntimeError::raise("Returning value of type " + ret.toString() + " from function with "
         "declared return type " + returnType.toString(), node);
   }

   if (returnType->isAutoTy()) {
      *returnType = VoidType::get();
      node->returnType->setType(returnType);
   }

   auto funcTy = FunctionType::get(returnType, argTypes, false);

   node->lambdaType = funcTy;
   node->captures = captures;
   popScope();

   return ReturnMemberExpr(node,Type(funcTy));
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type TypeCheckPass::visit(UsingStmt *node)
{
   if (node->kind == UsingKind::NAMESPACE) {
      importedNamespaces.push_back(node->importNamespace + ".");
   }
   else {
      size_t i = 0;
      for (auto& fullName : node->fullNames) {
         auto& item = node->importedItems[i];
         bool declarationFound = node->kind == UsingKind::NAMESPACE;
         if (SymbolTable::hasClass(fullName)) {
            if (SymbolTable::getClass(fullName)->isPrivate()) {
               RuntimeError::raise("Class " + fullName + " is not accessible", node);
            }

            declarationFound = true;
            node->kind = UsingKind::CLASS;
         }
         else if (SymbolTable::hasVariable(fullName)) {
            declarationFound = true;
            node->kind = UsingKind::VARIABLE;
         }
         else if (SymbolTable::hasTypedef(fullName)) {
            declarationFound = true;
            node->kind = UsingKind::TYPEDEF;
         }
         else {
            auto functions = SymbolTable::numFunctionsWithName(fullName);
            if (functions > 0) {
               declarationFound = true;
               node->kind = UsingKind::FUNCTION;
            }
         }

         if (!declarationFound) {
            RuntimeError::raise("Namespace " + node->importNamespace +
               " does not have a member named " + item, node);
         }

         SymbolTable::declareTemporaryAlias(item, fullName);
         ++i;
      }
   }

   return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type TypeCheckPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();

   return {};
}

Type TypeCheckPass::visit(ImplicitCastExpr *node)
{
   resolve(&node->to);
   node->target->accept(*this);

   return node->to;
}

Type TypeCheckPass::visit(TypedefDecl *node)
{
   return {};
}

Type TypeCheckPass::visit(TypeRef *node)
{
   assert(!node->resolved && "Duplicate resolving");
   DeclPass::resolveType(node, GenericsStack, importedNamespaces, currentNamespace);
   return node->type;
}

Type TypeCheckPass::visit(DeclareStmt *node)
{
   for (const auto& decl : node->declarations) {
      decl->accept(*this);
   }

   return {};
}

Type TypeCheckPass::visit(LvalueToRvalue* node)
{
   llvm_unreachable("Should only be applied after evaluating the node");
}

Type TypeCheckPass::visit(DebugStmt* node)
{
   if (node->isUnreachable) {
      auto current = latestScope;
      while (!current->returnable) {
         ++current->returned;
         current = current->enclosingScope;
      }

      if (current) {
         ++current->returned;
      }
   }
   else {
      int i = 3;
   }

   return {};
}

Type TypeCheckPass::visit(Statement* node)
{
   return {};
}

Type TypeCheckPass::visit(TupleLiteral* node)
{
   std::vector<pair<string, BuiltinType*>> containedTypes;

   for (auto& el : node->elements) {
      el.second->addUse();

      auto ty = el.second->accept(*this);
      toRvalueIfNecessary(ty, el.second);

      containedTypes.emplace_back(el.first, *ty);
   }

   auto tupleTy = TupleType::get(containedTypes);
   if (!node->contextualType->isAutoTy()) {
      if (!tupleTy->implicitlyCastableTo(*node->contextualType)) {
         RuntimeError::raise("Incompatible types " + tupleTy->toString() + " and " +
               node->contextualType.toString(), node);
      }

      auto asTuple = cast<TupleType>(*node->contextualType);
      auto arity = tupleTy->getArity();
      for (size_t i = 0; i < arity; ++i) {
         if (tupleTy->getContainedType(i) != asTuple->getContainedType(i)) {
            auto cont =Type(tupleTy->getContainedType(i));
            wrapImplicitCast(node->elements.at(i).second, cont,
               Type(asTuple->getContainedType(i)));
         }
      }

      tupleTy = cast<TupleType>(asTuple);
   }

   node->tupleType = tupleTy;
   return Type(tupleTy);
}

Type TypeCheckPass::visit(TryStmt *node)
{
   node->body->accept(*this);

   for (auto& catchBlock : node->catchBlocks) {
      auto caughtType = catchBlock.caughtType->accept(*this);

      pushScope();

      Type ty(caughtType);
      ty.isConst(true);
      ty.isLvalue(true);

      catchBlock.identifier = declareVariable(catchBlock.identifier, ty);
      catchBlock.body->accept(*this);

      popScope();
   }

   if (node->finallyBlock != nullptr) {
      node->finallyBlock->accept(*this);
   }

   return {};
}

namespace {
   string desc = "description";
}

Type TypeCheckPass::visit(ThrowStmt *node)
{
   auto thrown = node->thrownVal->accept(*this);
   node->thrownVal->isAssigned(true);
   node->thrownVal->addUse();
   node->setThrownType(*thrown);

   if (thrown->isObject()) {
      auto rec = thrown->getRecord();
      if (rec->isNonUnion()) {
         auto cl = rec->getAs<Class>();
         if (cl->hasField(desc)) {
            auto field = cl->getField(desc);
            if (field->isProp && field->hasGetter) {
               node->descFn = cl->getMethod(cl->getName() + "." + field->getterName);
            }
         }
      }
   }

   return {};
}

