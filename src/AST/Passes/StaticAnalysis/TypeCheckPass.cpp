//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "TypeCheckPass.h"
#include "../ASTIncludes.txt"
#include "../../SymbolTable.h"
#include "Class.h"
#include "Enum.h"
#include "../../../Message/Warning.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/CollectionType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../Operator/Conversion/LvalueToRvalue.h"
#include "../../../Util.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Message/Exceptions.h"
#include "../../../Variant/Type/AutoType.h"

using namespace cdot::cl;

string self_str = "self";
std::vector<string> TypeCheckPass::currentNamespace = { "" };
std::vector<string> TypeCheckPass::importedNamespaces = { "" };
std::stack<std::vector<ObjectType*>*> TypeCheckPass::GenericsStack = {};

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
   GenericsStack.push(&cl->getGenerics());
}

void TypeCheckPass::popClassScope()
{
   popNamespace();
   latestScope->inProtocol = false;

   classScopeStack.pop();
   GenericsStack.pop();
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

   bool warnCast(Type *&lhs, Type *&rhs) {
      return !lhs->isBoxedEquivOf(rhs) && !rhs->isBoxedEquivOf(lhs);
   }

   void raiseTypeError(Type* lhs, Type* rhs, AstNode* cause) {
      RuntimeError::raise("Incompatible types " + lhs->toString() + " and " + rhs->toString(), cause);
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
   Type* returnType,
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
      scope.declaredReturnType = new VoidType;
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
   Type *type,
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
      RuntimeError::raise("Redeclaration of variable " + var_name, cause);
   }

   SymbolTable::declareVariable(var_name, type, AccessModifier::PUBLIC, currentNamespace.back());

   return var_name;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckPass::pushTy(Type *type) {
   typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
Type* TypeCheckPass::popTy() {
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
pair<pair<Type*, string>, bool> TypeCheckPass::getVariable(
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
            RuntimeError::raise("Variable " + ident + " is inaccessible", cause);
         }

         return { { var.first.type, var.second }, false };
      }

      RuntimeError::raise("Reference to undeclared identifier " + ident, cause);
   }

   return { { SymbolTable::getVariable(curr).type, curr }, escapesLambdaScope };
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
   std::vector<Type*> generics)
{
   string context;
   auto overloads = SymbolTable::getFunction(funcName, importedNamespaces);

   CallCompatability result;
   if (overloads.first == overloads.second) {
      return result;
   }

   result.compatibility = CompatibilityType::NO_MATCHING_CALL;

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;

      auto res = util::findMatchingCall(args, overload->getArguments(), generics, overload->getGenerics());
      if (!res.isCompatible() && !result.isCompatible()) {
         result = res;
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
   for (const auto& cast : casts) {
      assert(givenArgs.size() > cast && args.size() > cast && "Invalid cast index");

      auto to = declaredArgs.size() > cast ? declaredArgs[cast].type : declaredArgs.back().type;
      wrapImplicitCast(args[cast].second, givenArgs[cast].type, to);
   }
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckPass::return_(
   Type* ret_type,
   AstNode *cause)
{
   auto current = latestScope;
   while (!current->returnable) {
      ++current->returned;
      if (current->declaredReturnType->isInferred()) {
         current->declaredReturnType = ret_type;
      }

      current = current->enclosingScope;
   }

   if (current == nullptr) {
      RuntimeError::raise("Keyword 'return' is only allowed in function bodies", cause);
   }

   if (!ret_type->implicitlyCastableTo(current->declaredReturnType) ||
      (current->declaredReturnType->isLvalue() && !ret_type->isLvalue()))
   {
      RuntimeError::raise("Returned value of type " + ret_type->toString() + " is not "
         "compatible with declared return type " + current->declaredReturnType->toString(), cause);
   }

   if (current->declaredReturnType->isInferred()) {
      current->declaredReturnType = ret_type;
   }

   ++current->returned;
}

void TypeCheckPass::continue_(ContinueStmt* continueStmt) {
   auto current = latestScope;
   while (!current->continuable) {
      current = current->enclosingScope;
   }

   if (current == nullptr) {
      RuntimeError::raise("'continue' is only valid in loops and switch statements", continueStmt);
   }

   current->continued = true;
}

void TypeCheckPass::break_(BreakStmt *breakStmt) {
   auto current = latestScope;
   while (!current->breakable) {
      current = current->enclosingScope;
   }

   if (current == nullptr) {
      RuntimeError::raise("'break' is only valid in loops and switch statements", breakStmt);
   }

   current->broken = true;
}

void TypeCheckPass::resolve(Type** ty, AstNode* node)
{
   (*ty)->visitContained(*this);

   auto generics = GenericsStack.empty() ? nullptr : GenericsStack.top();
   auto isValid = Type::resolve(ty, currentClass(), generics, importedNamespaces);
   if (node != nullptr && !isValid) {
      RuntimeError::raise("Type " + (*ty)->toString() + " does not exist", node);
   }

   if ((*ty)->isObject()) {
      auto cl = SymbolTable::getClass((*ty)->getClassName());
      cl->addUse();
   }
}

void TypeCheckPass::pushNamespace(string &ns) {
   auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");
}

void TypeCheckPass::popNamespace() {
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

Type* TypeCheckPass::ReturnMemberExpr(
   Expression *node,
   Type *ty)
{
   if (node->memberExpr != nullptr) {
      if (ty->needsLvalueToRvalueConv()) {
         ty->isLvalue(false);
         node->lvalueCast = true;
      }

      pushTy(ty);
      ty = node->memberExpr->accept(*this);
   }

   return ty;
}

void TypeCheckPass::wrapImplicitCast(
   Expression::SharedPtr& target,
   Type*& originTy,
   Type* destTy)
{
   auto cast = new ImplicitCastExpr(originTy->deepCopy(), destTy->deepCopy(), target);

   cast->setIndex(target->startIndex, target->endIndex, target->line, target->sourceFileId);
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
   Type *&ty,
   std::shared_ptr<Expression> &target,
   bool preCond)
{
   if (!preCond) {
      return;
   }

   if (ty->needsLvalueToRvalueConv()) {
      lvalueToRvalue(target);
   }

   ty->isLvalue(false);
}

bool TypeCheckPass::castGenericIfNecessary(
   Expression *node,
   unordered_map<string, Type *> &concreteGenerics,
   Type *&ty,
   bool preCond)
{
   bool isGeneric = false;
   if (!isGeneric) {
      for (const auto& cont : ty->getContainedTypes(true)) {
         if (cont->isGeneric()) {
            isGeneric = true;
            break;
         }
      }
   }

   if (!preCond || !isGeneric) {
      return false;
   }

   auto fromTy = ty->deepCopy();
   Type::resolveGeneric(&ty, concreteGenerics);

   auto toTy = ty->deepCopy();
   toTy->isLvalue(fromTy->isLvalue());

   if (*fromTy == toTy) {
      return false;
   }

   node->needsCast = true;
   node->castFrom = fromTy;
   node->castTo = toTy;

   return true;
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(NamespaceDecl *node)
{
   pushNamespace(node->nsName);
   if (node->isAnonymousNamespace) {
      importedNamespaces.push_back(currentNamespace.back());
   }

   node->contents->accept(*this);
   popNamespace();

   return nullptr;
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(CompoundStmt *node)
{
   if (!node->preserveScope) {
      pushScope();
   }

   size_t cleanupsSize = Cleanups.size();
   latestScope->unsafe = node->isUnsafe_ || latestScope->unsafe;

   for (const auto& child : node->get_children()) {
      child->accept(*this);
   }

   if (!node->preserveScope) {
      popScope();
   }

   size_t newSize = Cleanups.size();
   while (newSize > cleanupsSize) {
      auto top = Cleanups.top();
      node->valuesToClean.push_back(top);
      Cleanups.pop();
      --newSize;
   }


   return nullptr;
}

namespace {

   bool isReservedIdentifier(string& ident)
   {
      return (
         ident == "_"   ||
         ident == "Self"
      );
   }

   template <class T>
   std::vector<T*> copyTypeVector(std::vector<T*> vec)
   {
      std::vector<T*> copyVec;
      for (const auto& ty : vec) {
         copyVec.push_back(cast<T>(ty->deepCopy()));
      }

      return copyVec;
   }
}

void TypeCheckPass::checkClassAccessibility(
   cdot::cl::Class *&cl,
   Expression *cause)
{
   if (cl->isAbstract()) {
      RuntimeError::raise("Class " + cl->getName() + " is abstract and cannot be initialized", cause);
   }
   if (cl->isPrivate() && std::find(currentNamespace.begin(), currentNamespace.end(), cl->getDeclarationNamespace())
      == currentNamespace.end())
   {
      RuntimeError::raise("Class " + cl->getName() + " is not accessible", cause);
   }
}

void TypeCheckPass::checkMemberAccessibility(
   cdot::cl::Class *&cl,
   string& memberName,
   AccessModifier &access,
   Expression *cause)
{
   if (access == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass())) {
      RuntimeError::raise("Protected member " + memberName + " of class " +
               cl->getName() + " is not accessible", cause);
   }
   else if (access == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass())) {
      RuntimeError::raise("Private member " + memberName + " of class " +
               cl->getName() + " is not accessible", cause);
   }
}

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(FunctionDecl *node)
{
   if (node->is_declaration) {
      for (const auto& arg : node->args) {
         if (arg->defaultVal != nullptr) {
            arg->defaultVal->setContextualType(arg->argType->type);
            arg->defaultVal->accept(*this);
         }
      }

      return nullptr;
   }

   GenericsStack.push(&node->generics);

   auto& return_type = node->returnType->getType();
   pushFunctionScope(return_type, node->declaredFunction->getName(),
      node->declaredFunction->getMangledName());

   for (const auto& arg : node->args) {
      arg->accept(*this);
      arg->binding = declareVariable(arg->argName, arg->argType->getType());
   }

   attributes = node->attributes;
   node->body->accept(*this);

   attributes.clear();

   if (latestScope->branches - latestScope->returned > 0 && !return_type->isNullable() &&
      !isa<VoidType>(return_type) && node->funcName != "main")
   {
      RuntimeError::raise("Not all code paths return a value", node);
   }
   // implicit 0 return for main function
   else if (latestScope->branches - latestScope->returned > 0 && node->funcName == "main") {
      return_type = IntegerType::get();
      node->body->implicitZeroReturn = true;
   }

   popScope();
   GenericsStack.pop();
   return nullptr;
}

void TypeCheckPass::CopyNodeProperties(
   Expression *src,
   Expression *dst)
{
   dst->isLhsOfAssigment_ = src->isLhsOfAssigment_;
   dst->isFunctionArgument_ = src->isFunctionArgument_;
   dst->isPartOfReturnValue_ = src->isPartOfReturnValue_;
   dst->setIndex(src->startIndex, src->endIndex, src->line, src->sourceFileId);

   if (src->isHiddenReturnValue_) {
      dst->isHiddenReturnValue();
   }
   else if (src->isReturnValue_) {
      dst->isReturnValue();
   }
}

namespace {
   enum class BuiltinIdentifier {
      FUNC, MANGLED_FUNC
   };

   unordered_map<string, BuiltinIdentifier> builtinIdentifiers = {
      { "__func__", BuiltinIdentifier::FUNC },
      { "__mangled_func__", BuiltinIdentifier::MANGLED_FUNC }
   };
}

Type* TypeCheckPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
{
   if (builtinIdentifiers.find(node->ident) == builtinIdentifiers.end()) {
      return nullptr;
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
   }

   return node->builtinType->deepCopy();
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(IdentifierRefExpr *node)
{
   if (node->isLetExpr_ || node->isVarExpr_) {
      RuntimeError::raise("Unexpected 'let' / 'var' expression", node);
   }

   auto builtin = HandleBuiltinIdentifier(node);
   if (builtin != nullptr) {
      return builtin;
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

      node->isNamespace = true;
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
         RuntimeError::raise("Cannot access index on a namespace", node);
      }
      else {
         assert(false && "Unknown operation");
      }

      node->memberExpr->contextualType = node->contextualType;

      return node->memberExpr->accept(*this);
   }

   if (node->ident == "super") {
      if (currentClass().empty()) {
         RuntimeError::raise("'super' can only be used in instance methods", node);
      }

      auto currentCl = SymbolTable::getClass(currentClass());
      if (currentCl->getParent() == nullptr) {
         RuntimeError::raise("Class " + currentClass() + " does not have a base class", node);
      }

      node->binding = latestScope->currentSelf;
      node->ident = "self";
      node->isSuper = true;
      node->superClassName = currentCl->getParent()->getName();

      auto type = ObjectType::get(node->superClassName)->toRvalue();
      return ReturnMemberExpr(node, type);
   }

   // try a function instead
   if (!hasVariable(node->ident)) {
      std::vector<Type*> args;
      Type* returnType;
      bool typeInferred = false;

      if (node->contextualType != nullptr && isa<FunctionType>(node->contextualType)) {
         auto asFunc = cast<FunctionType>(node->contextualType);
         args = asFunc->getArgTypes();
         returnType = asFunc->getReturnType()->deepCopy();
         typeInferred = true;
      }
      else {
         returnType = new VoidType;
      }

      auto overloads = SymbolTable::getFunction(node->ident, importedNamespaces);
      bool foundMatch = false;
      Function* result;

      // only one result
      if (!typeInferred && std::distance(overloads.first, overloads.second) == 1) {
         result = overloads.first->second.get();

         node->binding = result->getMangledName();
         node->isFunction = true;

         std::vector<Type*> argTypes;
         for (const auto& arg : result->getArguments()) {
            argTypes.push_back(arg.type->deepCopy());
         }

         auto funcTy = new FunctionType(result->getReturnType()->deepCopy(), argTypes);
         funcTy->setFunction(result);

         return ReturnMemberExpr(node, funcTy);
      }

      for (auto it = overloads.first; it != overloads.second; ++it) {
         auto& argTypes = it->second->getArguments();
         if (argTypes.size() != args.size()) {
            continue;
         }

         size_t i = 0;
         bool matches = true;
         for (const auto& arg : argTypes) {
            if (!args.at(i)->implicitlyCastableTo(arg.type)) {
               matches = false;
               break;
            }
         }

         if (!matches) {
            continue;
         }

         if (!returnType->implicitlyCastableTo(it->second->getReturnType())) {
            continue;
         }

         foundMatch = true;
         result = it->second.get();
         break;
      }

      delete returnType;

      if (foundMatch) {
         node->binding = result->getMangledName();
         node->isFunction = true;

         std::vector<Type*> argTypes;
         for (const auto& arg : result->getArguments()) {
            argTypes.push_back(arg.type->deepCopy());
         }

         auto funcTy = new FunctionType(result->getReturnType()->deepCopy(), argTypes);
         funcTy->setFunction(result);

         return ReturnMemberExpr(node, funcTy);
      }
   }

   bool implicit_this = node->ident != "self" && !hasVariable(node->ident) && !currentClass().empty();
   if (node->ident == "self" || implicit_this) {
      if (currentClass().empty()) {
         RuntimeError::raise("'this' can only be used in instance methods", node);
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
      auto type = cl->getType();
      type->isLvalue(latestScope->mutableSelf);
      type->isSelf(true);

      return ReturnMemberExpr(node, type);
   }

   auto res = getVariable(node->ident, node);

   // add capture for this variable
   if (latestScope->inLambda && res.second) {
      latestScope->captures->emplace_back(res.first.second, res.first.first);
      node->isCapturedVar = true;
      node->capturedType = res.first.first->deepCopy();
   }

   node->binding = res.first.second;
   node->declaration = declarations[node->binding];

   return ReturnMemberExpr(node, res.first.first->deepCopy());
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(DeclStmt *node)
{
   if (node->declared || node->is_declaration) {
      return nullptr;
   }

   auto& ident = node->identifier;
   if (isReservedIdentifier(ident)) {
      RuntimeError::raise(ident + " is a reserved identifier", node);
   }

   if (!node->type->resolved) {
      node->type->accept(*this);
   }

   auto& declType = node->type->getType();
   auto& val = node->value;

   if (val != nullptr) {
      val->isGlobal(node->is_global);
      if (!isa<AutoType>(declType)) {
         val->setContextualType(declType);
      }

      Type* givenType = val->accept(*this);
      toRvalueIfNecessary(givenType, node->value, !declType->isLvalue());

      node->isProtocolDecl = givenType->isProtocol() && !declType->isProtocol();

      // in case it has changed
      declType = node->type->getType();

      if (declType->isInferred()) {
         if (givenType->isNull()) {
            RuntimeError::raise("Cannot assign value of type void", val.get());
         }

         delete declType;
         declType = givenType->deepCopy();
      }
      else if (isa<VoidType>(givenType) && !declType->isNullable()) {
         RuntimeError::raise("Cannot assign null to non-nullable variable of type " +
                     declType->toString(), val.get());
      }
      else if (!givenType->implicitlyCastableTo(declType)) {
         RuntimeError::raise("Incompatible types " + givenType->toString() + " and " +
                     declType->toString(), val.get());
      }

      if (*declType != givenType) {
         wrapImplicitCast(node->value, givenType, declType);
      }
   }
   else if (!declType->hasDefaultValue()) {
      RuntimeError::raise("Expected non-nullable variable " + ident + " to be defined", node);
   }

   resolve(&declType);

   auto allocType = declType;
   node->isStructAlloca = declType->isStruct();
   node->incRefCount = declType->isObject() &&
      SymbolTable::getClass(declType->getClassName())->isClass();

   allocType->isLvalue(true);
   allocType->isConst(node->is_const);

   if (!node->is_global) {
      node->binding = declareVariable(ident, allocType, node->is_global, node);
   }
   else {
      SymbolTable::setVariable(node->binding, allocType);
   }

   declarations.emplace(node->binding, node);

   if (node->incRefCount) {
      Cleanups.push(pair<string, string>{ node->binding, declType->getClassName() });
      node->className = declType->getClassName();
   }

   node->declared = true;
   return nullptr;
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(ForStmt *node)
{
   pushScope();

   if (node->initialization) {
      node->initialization->accept(*this);
   }

   if (node->increment) {
      node->increment->accept(*this);
   }

   if (node->termination) {
      auto cond = node->termination->accept(*this);

      auto boolTy = IntegerType::get(1);
      if (cond->isObject() && cond->getClassName() == "Bool") {
         wrapImplicitCast(node->termination, cond, boolTy);
      }
      else if (!cond->implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Condition must be boolean", node->termination.get());
      }
   }

   if (node->body) {
      pushLoopScope();
      node->body->accept(*this);
      popScope();
   }

   popScope();
   return nullptr;
}

Type* TypeCheckPass::visit(ForInStmt *node)
{
   auto range = node->rangeExpr->accept(*this);
   if (!range->isObject() || !SymbolTable::getClass(range->getClassName())->conformsTo("Iterable")) {
      RuntimeError::raise("Range expression in for-in statement must conform to "
         "'Iterable' protocol", node->rangeExpr.get());
   }

   toRvalueIfNecessary(range, node->rangeExpr);

   auto cl = SymbolTable::getClass(range->getClassName());
   cl->addUse();

   auto& prot = cl->getConformedToProtocols();
   Type* itType = nullptr;
   for (const auto& p : prot) {
      if (p->getClassName() == "Iterable") {
         itType = p->getConcreteGeneric("T")->deepCopy();
         break;
      }
   }

   assert(itType && "Iterable conformance shouldn't be possible otherwise!");

   // e.g. for let i in 1..5 { ... }
   // NOT  for let i in [1, 2, 3] { ... }
   if (itType->isProtocol()/* && !SymbolTable::getClass(itType->getClassName())->isEmptyProtocol()*/) {
      node->protocolTy = itType;
   }

   Type::resolveGeneric(&itType, range->getConcreteGenericTypes());
   node->decl->type->accept(*this);
   if (!node->decl->type->getType()->implicitlyCastableTo(itType)) {
      RuntimeError::raise("Iterated type " + itType->toString()
               + " is not compatible with expected type " + node->decl->type->getType()->toString(), node->decl.get());
   }

   pushLoopScope();
   itType->isLvalue(true);
   itType->isConst(node->decl->is_const);
   node->binding = declareVariable(node->decl->identifier, itType);

   node->body->accept(*this);
   popScope();

   auto getIterator = cl->hasMethod("getIterator");
   assert(getIterator.compatibility == CompatibilityType::COMPATIBLE
      && "Iterable not implemented correctly?");

   ++getIterator.method->uses;

   auto iteratorCl = SymbolTable::getClass(getIterator.method->returnType->getClassName());
   iteratorCl->addUse();

   auto nextFunc = iteratorCl->hasMethod("next");
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

   return nullptr;
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(
   WhileStmt *node)
{
   auto cond = node->condition->accept(*this);
   auto boolTy = IntegerType::get(1);
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond->implicitlyCastableTo(boolTy)) {
      RuntimeError::raise("Condition must be boolean", node->condition.get());
   }

   pushLoopScope();
   node->body->accept(*this);
   popScope();

   return nullptr;
}

pair<Type*, std::vector<Type*>> TypeCheckPass::unify(
   std::vector<Expression::SharedPtr>& types)
{
   Type* unified = nullptr;
   std::vector<Type*> evaledTypes;
   evaledTypes.reserve(types.size());

   bool anyCompatible = false;

   for (auto& expr : types) {
      auto type = expr->accept(*this);
      evaledTypes.push_back(type);

      if (unified == nullptr) {
         unified = type;
         continue;
      }

      if (!unified->implicitlyCastableTo(type)) {
         if (!type->implicitlyCastableTo(unified)) {
            unified = ObjectType::get("Any");
            anyCompatible = true;
         }
      }
      else if (!anyCompatible) {
         unified = type;
      }
   }

   size_t i = 0;
   for (auto& expr : types) {
      wrapImplicitCast(expr, evaledTypes.at(i), unified);
      ++i;
   }

   return { unified->deepCopy(), evaledTypes };
}

Type* TypeCheckPass::HandleDictionaryLiteral(CollectionLiteral *node)
{
   node->type->accept(*this);

   auto& dictTy = node->type->getType();
   auto HashableTy = ObjectType::get("Hashable");
   auto AnyTy = ObjectType::getAnyTy();

   auto& keys = node->keys;
   auto& values = node->values;

   assert(keys.size() == values.size() && "Should have raised a parse error");

   if (node->contextualType != nullptr && isa<CollectionType>(node->contextualType)) {
      auto dict = cast<CollectionType>(node->contextualType);
      if (dict->getKind() != CollectionKind::DICTIONARY) {
         raiseTypeError(node->contextualType, dictTy, node);
      }

      auto keyTy = dict->getConcreteGeneric("K");
      auto valTy = dict->getConcreteGeneric("V");

      for (auto& key : keys) {
         auto ty = key->accept(*this);
         toRvalueIfNecessary(ty, key);

         if (!ty->implicitlyCastableTo(keyTy)) {
            raiseTypeError(ty, keyTy, key.get());
         }
         else if (*ty != keyTy) {
            wrapImplicitCast(key, ty, keyTy);
         }

         wrapImplicitCast(key, keyTy, HashableTy);
      }

      for (auto& val : values) {
         auto ty = val->accept(*this);
         toRvalueIfNecessary(ty, val);

         if (!ty->implicitlyCastableTo(valTy)) {
            raiseTypeError(ty, valTy, val.get());
         }
         else if (*ty != valTy) {
            wrapImplicitCast(val, ty, valTy);
         }

         wrapImplicitCast(val, valTy, AnyTy);
      }

      return dictTy->deepCopy();
   }

   auto keyTy = unify(keys);
   auto valTy = unify(values);

   if (!keyTy.first->isObject() ||
      !SymbolTable::getClass(keyTy.first->getClassName())->conformsTo("Hashable"))
   {
      RuntimeError::raise("Dictionary keys must conform to 'Hashable'", node);
   }

   size_t i = 0;
   for (auto& key : node->keys) {
      toRvalueIfNecessary(keyTy.second[i++], key);
      wrapImplicitCast(key, keyTy.first, HashableTy);
   }

   i = 0;
   for (auto& val : node->values) {
      toRvalueIfNecessary(valTy.second[i++], val);
      wrapImplicitCast(val, valTy.first, AnyTy);
   }

   auto obj = cast<ObjectType>(dictTy);
   obj->specifyGenericType("K", keyTy.first);
   obj->specifyGenericType("V", valTy.first);

   return dictTy->deepCopy();
}

/**
 * Checks an array literal
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(CollectionLiteral *node)
{
   if (node->isDictionary) {
      return HandleDictionaryLiteral(node);
   }

   auto isInferred = node->contextualType != nullptr;
   if (!isInferred || node->contextualType->isInferred()) {
      Type* elTy;
      if (node->values.empty()) {
         elTy = ObjectType::get("Any")->getPointerTo();
      }
      else {
         elTy = unify(node->values).first;
         for (auto& el : node->values) {
            toRvalueIfNecessary(elTy, el);
            wrapImplicitCast(el, elTy, ObjectType::getAnyTy());
         }
      }

      if (node->hasAttribute(Attr::CArray)) {
         elTy->isCStyleArray(true);

         node->type = std::make_shared<TypeRef>(elTy);
         node->type->accept(*this);
      }
      else {
         node->type = std::make_shared<TypeRef>((new CollectionType(elTy->deepCopy())));
         node->type->accept(*this);
      }

      return ReturnMemberExpr(node, node->type->getType()->deepCopy());
   }

   if (!node->contextualType->isPointerTy() && !isa<CollectionType>(node->contextualType))
   {
      RuntimeError::raise("Collection literal cannot return value of type " +
               node->contextualType->toString(), node);
   }

   bool isCarray = false;
   Type* elType;
   if (isa<PointerType>(node->contextualType)) {
      elType = cast<PointerType>(node->contextualType)->getPointeeType();
      isCarray = true;
   }
   else {
      auto ptrType = cast<CollectionType>(node->contextualType);
      elType = ptrType->getConcreteGeneric("T")->deepCopy();
   }

   if (!node->values.empty()) {
      auto givenType = unify(node->values).first;
      for (auto& el : node->values) {
         toRvalueIfNecessary(givenType, el);
      }

      if (!givenType->implicitlyCastableTo(elType)) {
         RuntimeError::raise("Incompatible collection elements: Expected " + elType->toString() +
                     ", but got " + givenType->toString(), node);
      }
      else if (*elType != givenType) {
         for (auto &el : node->values) {
            wrapImplicitCast(el, givenType, elType);
         }
      }

      if (!elType->isObject() || elType->getClassName() != "Any") {
         for (auto &el : node->values) {
            wrapImplicitCast(el, elType, ObjectType::getAnyTy());
         }
      }
   }

   auto ty = elType->getPointerTo();
   ty->isCStyleArray(isa<PointerType>(node->contextualType));

   if (isCarray) {
      node->type = std::make_shared<TypeRef>(ty);
   }
   else {
      node->type = std::make_shared<TypeRef>(node->contextualType->deepCopy());
   }

   node->type->accept(*this);
   return ReturnMemberExpr(node, node->type->getType()->deepCopy());
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(NumericLiteral *node)
{
   if ((node->memberExpr == nullptr && node->contextualType != nullptr && isa<PrimitiveType>(node->contextualType)) ||
      node->hasAttribute(Attr::Primitive))
   {
      node->isPrimitive = true;

      if (node->contextualType != nullptr) {
         node->type = node->contextualType->deepCopy();
         node->value = node->value.castTo(node->type);

         return node->contextualType->deepCopy();
      }

      return node->type->deepCopy();
   }

   if (node->contextualType != nullptr) {
      if (node->contextualType->isBoxedPrimitive()) {
         node->type = node->contextualType->unbox();
      }
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

         if (node->isChar) {
            className = "Char";
         }

         if (node->isBool) {
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
      case TypeID::VoidTypeID: {
         node->isPrimitive = true;
         if (node->contextualType && node->contextualType->isPointerTy()) {
            node->type = node->contextualType;
            return node->contextualType;
         }

         cast<VoidType>(node->type)->setPointeeType(node->contextualType);
         return node->type->deepCopy();
      }
      case TypeID::ObjectTypeID:
         node->className = node->type->getClassName();
         break;
      default:
         llvm_unreachable("Unknown literal type");
   }

   assert(node->isPrimitive || !node->className.empty() && "Unknown primitive type");

   auto ty = ObjectType::get(node->className)->toRvalue();
   if (node->isChar) {
      Type* ty = IntegerType::ConstInt8;
      node->value = node->value.castTo(ty);
   }
   else if (node->isBool) {
      Type* ty = IntegerType::ConstInt1;
      node->value = node->value.castTo(ty);
   }
   else {
      node->value = node->value.castTo(node->type);
   }

   return ReturnMemberExpr(node, ty);
}

Type* TypeCheckPass::visit(NoneLiteral *node)
{
   if (node->contextualType == nullptr || isa<AutoType>(node->contextualType)) {
      RuntimeError::raise("'none' requires a contextual type", node);
   }
   if (!node->contextualType->isOptionTy()) {
      RuntimeError::raise("'none' can only be assigned to option types", node);
   }

   return node->contextualType->deepCopy();
}

Type* TypeCheckPass::visit(StringLiteral *node)
{
   for (const auto& attr : node->attributes) {
      switch (attr.kind) {
         case Attr::CString:
            node->raw = true;
            break;
         default:
            RuntimeError::raise("Attribute " + attr.name + " is not applicable"
               "to string literals", node);
      }
   }

   if (node->contextualType != nullptr && isa<PointerType>(node->contextualType)) {
      node->raw = true;
   }

   if (node->raw) {
      auto ty = new PointerType(IntegerType::get(8));
      ty->isCStyleArray(true);

      return ty;
   }

   return ReturnMemberExpr(node, ObjectType::get("String"));
}

Type* TypeCheckPass::visit(StringInterpolation *node)
{
   bool first = true;
   size_t i = 0;
   for (auto& expr : node->strings) {
      auto val = expr->accept(*this);
      auto StringRepr = ObjectType::get("StringRepresentable");
      StringRepr->isProtocol(true);

      toRvalueIfNecessary(val, expr);

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
      else if (isa<PrimitiveType>(val)) {
         assert(!first && "first element should always be string");

         auto boxed = val->box();
         wrapImplicitCast(expr, val, boxed);
         wrapImplicitCast(expr, boxed, StringRepr);

         first = false;
         ++i;
         continue;
      }

      RuntimeError::raise("Values used in string interpolation must conform to "
               "'StringRepresentable'", expr.get());
   }

   return ReturnMemberExpr(node, ObjectType::get("String"));
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(SubscriptExpr *node)
{
   auto ts = popTy()->deepCopy();
   resolve(&ts);

   if (!isa<ObjectType>(ts)) {
      node->_index->setContextualType(IntegerType::ConstInt64);
   }

   Type* index = node->_index->accept(*this);
   if (isa<ObjectType>(ts) && !ts->isCStyleArray())
   {
      auto& className = ts->getClassName();

      auto cl = SymbolTable::getClass(className, importedNamespaces);
      std::vector<Argument> args{ {"", index} };
      string op = "postfix []";

      auto methodResult = cl->hasMethod(op, args);
      if (methodResult.isCompatible()) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->_index },
            op
         );

         call->setIndex(node->startIndex, node->endIndex, node->line, node->sourceFileId);
         call->parentExpr = node;
         call->parent = node;
         call->resolvedArgs.push_back(std::move(args.front()));

         auto expr = std::static_pointer_cast<Expression>(call);
         node->children.push_back(&expr);

         node->overridenCall = call;
         node->isSubscriptOp = true;

         pushTy(ts);
         auto type = call->accept(*this);

         return ReturnMemberExpr(node, type);
      }
      else {
         RuntimeError::raise("Operator [](" + index->toString() + ") is not defined on class " +
                     ts->toString(), node);
      }
   }

   if (ts->isPointerTy() && !ts->isCStyleArray()) {
      node->isPointerShift = true;
      auto ptr = cast<PointerType>(ts)->getPointeeType()->deepCopy();
      delete ts;

      ts = ptr;
   }
   else if (!ts->isCStyleArray()) {
      RuntimeError::raise("Operator [](" + index->toString() + ") is not defined on type " +
               ts->toString(), node);
   }

   toRvalueIfNecessary(index, node->_index);

   Type* int64Ty = IntegerType::get(64);
   Type* uint64Ty = IntegerType::get(64, true);
   if (!index->isIntegerTy() && !index->implicitlyCastableTo(int64Ty) && !index->implicitlyCastableTo(uint64Ty)) {
      RuntimeError::raise("Array indices have to be integral, " + index->toString() + " given", node);
   }
   else if (*index != int64Ty) {
      wrapImplicitCast(node->_index, index, int64Ty);
   }
   else {
      delete int64Ty;
   }

   delete uint64Ty;

   ts->isLvalue(true);
   ts->isCStyleArray(false);

   return ReturnMemberExpr(node, ts);
}

void TypeCheckPass::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>>& args,
   std::vector<Argument> &givenArgs,
   CallCompatability &comp)
{
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

   i = 0;
   for (auto& arg : args) {
      if (i >= givenArgs.size()) {
         break;
      }

      auto &given = givenArgs[i].type;

      if (given == nullptr) {
         given = comp.resolvedArgs[i].type;
      }

      if (i >= declaredArgs.size()) {
         // assume that c style varargs do not require an lvalue
         toRvalueIfNecessary(given, arg.second);
         continue;
      }

      auto &declared = i >= declaredArgs.size() ? declaredArgs.back().type : declaredArgs[i].type;
      if (declared->isStruct() && given->isStruct() && !declared->isLvalue()) {
         arg.second->needsByValPass = true;
      }

      toRvalueIfNecessary(given, arg.second, !declared->isLvalue());
      ++i;
   }
}

void TypeCheckPass::HandleFunctionCall(CallExpr *node)
{
   GetCallArguments(
      GetFunctionOverloadArgs(node->ident),
      node
   );

   auto& resolvedArgs = node->resolvedArgs;
   auto result = getFunction(node->ident, resolvedArgs, node->generics);
   if (!result.isCompatible()) {
      // lambda or saved function call
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         auto fun = var.first.first->deepCopy();

         if (fun->isLvalue()) {
            fun = fun->toRvalue();
         }

         if (isa<FunctionType>(fun)) {
            pushTy(fun);
            node->type = CallType::ANON_CALL;
            node->binding = var.first.second;

            if (latestScope->inLambda && var.second) {
               latestScope->captures->emplace_back(var.first.second, var.first.first);
               node->isCapturedVar = true;
               node->capturedType = var.first.first;
            }

            return HandleAnonCall(node);
         }

         delete fun;
      }

      // possible implicit method call
      auto& currentCl = currentClass();
      if (!currentCl.empty() && SymbolTable::hasClass(currentCl)) {
         auto cl = SymbolTable::getClass(currentCl);
         GetCallArguments(
            GetMethodOverloadArgs(cl, node->ident),
            node
         );

         auto compat = cl->hasMethod(node->ident, resolvedArgs);
         if (compat.compatibility == CompatibilityType::COMPATIBLE) {
            pushTy(ObjectType::get(currentCl));

            node->type = CallType::METHOD_CALL;
            node->implicitSelfCall = true;
            node->selfBinding = latestScope->currentSelf;

            return HandleMethodCall(node);
         }
      }

      if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
         Expression* cause = node;
         if (node->args.size() > result.incompatibleArg) {
            cause = node->args[result.incompatibleArg].second.get();
         }

         if (!result.foundType.empty() && !result.expectedType.empty()) {
            RuntimeError::raise(
               "No matching call for function " + node->ident + " found: Candidate function not viable: No known "
                              "conversion from " + result.foundType + " to " + result.expectedType + " ", cause);
         }
         else {
            RuntimeError::raise("No matching call for function " + node->ident + " found", cause);
         }
      }
      if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
         RuntimeError::raise("Function " + node->ident + " does not exist", node);
      }

      llvm_unreachable("No other options possible");
   }

   node->generics = result.generics;

   auto& func = result.func;
   func->addUse();

   auto& declaredArgs = func->getArguments();
   PrepareCallArgs(node->args, resolvedArgs, result);
   ApplyCasts(node->args, resolvedArgs, declaredArgs, result.neededCasts);

   node->binding = SymbolTable::mangleFunction(func->getName(), declaredArgs);
   node->declaredArgTypes = &func->getArguments();

   if (func->hasHiddenParam()) {
      node->hasStructReturn = true;
      node->structReturnType = func->getReturnType();
   }

   for (auto& gen : node->generics) {
      resolve(&gen);
   }

   auto& retTy = func->getReturnType();
   bool isGeneric = false;
   if (!isGeneric) {
      for (const auto& cont : retTy->getContainedTypes(true)) {
         if (cont->isGeneric()) {
            isGeneric = true;
            break;
         }
      }
   }

   if (isGeneric) {
      auto returnTy = retTy->deepCopy();
      auto genericTy = retTy->deepCopy();

      // the covariance of the generic type to cast from
      node->genericOriginTy = genericTy;

      // changes the generic type (i.e. T) with the concrete one
      auto concreteGenerics = Type::resolveUnqualified(node->generics, func->getGenerics());
      Type::resolveGeneric(&returnTy, concreteGenerics);

      node->returnType = returnTy;
      node->needsGenericCast = true;

      // concrete generic type of this particular instance
      node->genericDestTy = returnTy->deepCopy();
   }
   else {
      node->returnType = func->getReturnType()->deepCopy();
   }
}

void TypeCheckPass::HandleBuiltinCall(CallExpr *node)
{
   node->isBuiltin = true;

   auto& Builtin = util::builtinTypes[node->ident];
   if (!Builtin.second.empty()) {
      node->returnType = Builtin.second[0]->deepCopy();
   }

   node->builtinType = Builtin.first;

   switch (node->builtinType) {
      case BuiltinFn::ALIGNOF:
      case BuiltinFn::SIZEOF:
      case BuiltinFn::NULLPTR: {
         if (node->args.size() != 1 || node->args.front().second->get_type() != NodeType::TYPE_REF) {
            RuntimeError::raise("Expected type as argument to " + node->ident, node);
         }

         auto typeref = std::static_pointer_cast<TypeRef>(node->args.at(0).second);
         if (!typeref->resolved) {
            typeref->accept(*this);
         }

         if (node->builtinType == BuiltinFn::NULLPTR) {
            auto ty = typeref->getType()->deepCopy();
            bool refcounted = false;
            if (ty->isObject()) {
               refcounted = SymbolTable::getClass(ty->getClassName())->isClass();
            }

            if (!refcounted) {
               node->returnType = ty->getPointerTo();
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
         typeref->accept(*this);

         toRvalueIfNecessary(node->resolvedArgs.front().type, node->args.front().second);
         node->returnType = typeref->getType()->deepCopy();
         break;
      }
      case BuiltinFn::MEMCPY:
      case BuiltinFn::MEMSET:
         return HandleFunctionCall(node);
   }
}

void TypeCheckPass::HandleEnumCase(CallExpr *node) {
   auto cl = SymbolTable::getClass(node->className);

   if (!cl->isEnum()) {
      return;
   }

   GetCallArguments(
      GetMethodOverloadArgs(cl, node->ident),
      node
   );

   auto en = static_cast<Enum *>(cl);
   auto res = en->hasCase(node->ident, node->resolvedArgs, node->generics);
   if (res.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Enum " + node->className + " does not define case " + node->ident +
         " with the given arguments", node);
   }

   auto &case_ = en->getCase(node->ident);
   std::vector<Argument> neededArgs;
   for (const auto &ty : case_.associatedValues) {
      neededArgs.emplace_back(ty.first, ty.second->deepCopy());
   }

   PrepareCallArgs(node->args, node->resolvedArgs, res);
   ApplyCasts(node->args, node->resolvedArgs, neededArgs, res.neededCasts);

   node->isEnumCase = true;
   node->caseVal = case_.rawValue;
   node->generics = res.generics;

   std::vector<Type *> enumCaseTypes;
   for (const auto &arg : neededArgs) {
      enumCaseTypes.push_back(arg.type);
   }

   auto obj = ObjectType::get(node->className);
   obj->isEnum(true);
   obj->setKnownEnumCase(&en->getCase(node->ident), node->args, enumCaseTypes);

   // specify supplied generics for this instance
   if (en->isGeneric()) {
      auto &cl_gen = en->getGenerics();

      size_t i = 0;
      for (const auto &gen : cl_gen) {
         obj->specifyGenericType(gen->getGenericClassName(), node->generics.at(i++)->deepCopy());
         if (gen->isGeneric()) {
            obj->isGeneric(true);
         }
      }
   }

   node->returnType = obj;

   return;
}

void TypeCheckPass::HandleMethodCall(
   CallExpr *node)
{
   Class* cl;
   string className;
   string fullName;
   Type* latest;

   if (node->parentExpr == nullptr && node->isEnumCase) {
      auto& inf = node->contextualType;

      if (node->contextualType == nullptr || !SymbolTable::hasClass(inf->getClassName())) {
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
      if (node->returnType != nullptr) {
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

         latest = latest->getPointeeType();
      }

      if (!latest->isObject()) {
         RuntimeError::raise("Cannot call method on value of type " + latest->toString(), node);
      }

      className = latest->getClassName();
      if (!SymbolTable::hasClass(className)) {
         RuntimeError::raise("Class " + latest->toString() + " does not exist", node);
      }

      cl = SymbolTable::getClass(className, importedNamespaces);
      node->className = className;

      if (cl->isProtocol()) {
         node->castFrom = latest;
         node->isProtocolCall = true;
      }
   }

   checkClassAccessibility(cl, node);
   GetCallArguments(
      GetMethodOverloadArgs(cl, node->ident),
      node
   );

   auto &givenArgs = node->resolvedArgs;
   auto methodResult = node->isNsMember
                       ? cl->hasMethod(node->ident, givenArgs, node->generics)
                       : cl->hasMethod(node->ident, givenArgs, node->generics, latest->getConcreteGenericTypes());

   node->generics = methodResult.generics;
   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(methodResult, node, cl);
   }

   auto& method = methodResult.method;
   ++method->uses;

   // check if method is accessible from current context
   checkMemberAccessibility(cl, method->methodName, method->accessModifier, node);

   unordered_map<string, Type*> concreteGenerics;
   if (!node->generics.empty()) {
      for (auto& gen : node->generics) {
         resolve(&gen);
      }

      concreteGenerics = Type::resolveUnqualified(node->generics, method->generics);
   }
   else if (!method->isStatic) {
      concreteGenerics = latest->getConcreteGenericTypes();
   }

   auto ty = method->returnType->deepCopy();
   auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass() != cl->getName());
   if (isGeneric) {
      delete ty;
      node->returnType = node->castTo->deepCopy();
      node->returnType->isLvalue(method->returnType->isLvalue());
   }
   else {
      node->returnType = ty;
   }

   // returning an object with non-resolved generics
   if (method->returnType->isObject()) {
      auto returnCl = SymbolTable::getClass(method->returnType->getClassName());
      if (returnCl->isGeneric()) {
         Type::resolveGeneric(&node->returnType, latest->getConcreteGenericTypes());
      }
   }

   if (method->isProtocolDefaultImpl) {
      assert(node->parentExpr != nullptr && "No parent expression to cast!");
      node->parentExpr->needsCast = true;
      node->parentExpr->castFrom = latest;
      node->parentExpr->castTo = ObjectType::get(method->protocolName);
   }

   // methods with 'ref' return type
   node->returnType->isLvalue(method->returnType->isLvalue());
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
      node->structReturnType = method->returnType;
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
   Class* cl = SymbolTable::getClass(node->ident, importedNamespaces);
   checkClassAccessibility(cl, node);

   string methodName = "init";
   GetCallArguments(
      GetMethodOverloadArgs(cl, methodName),
      node
   );

   auto& givenArgs = node->resolvedArgs;
   auto constrResult = cl->hasMethod(methodName, givenArgs, node->generics);

   node->generics = constrResult.generics;
   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      throwMethodNotFound(constrResult, node, cl);
   }

   auto& method = constrResult.method;
   ++method->uses;
   cl->addUse();

   // check accessibility
   checkMemberAccessibility(cl, method->methodName, method->accessModifier, node);

   node->ident = cl->getName();
   if (cl->isAbstract()) {
      RuntimeError::raise("Cannot instantiate abstract class " + node->ident, node);
   }

   ObjectType* returnType = ObjectType::get(node->ident);

   // specify supplied generics for this instance
   if (cl->isGeneric()) {
      auto& cl_gen = cl->getGenerics();

      size_t i = 0;
      for (const auto& gen : cl_gen) {
         returnType->specifyGenericType(gen->getGenericClassName(), node->generics.at(i++)->deepCopy());
         if (gen->isGeneric()) {
            returnType->isGeneric(true);
         }
      }
   }

   node->returnType = returnType;
   resolve(&node->returnType);

   node->type = CallType::CONSTR_CALL;
   node->binding = method->mangledName;

   auto& declaredArgs = method->arguments;

   PrepareCallArgs(node->args, givenArgs, constrResult);
   ApplyCasts(node->args, givenArgs, declaredArgs, constrResult.neededCasts);

   node->declaredArgTypes = &method->arguments;
}

void TypeCheckPass::throwMethodNotFound(
   CallCompatability &res,
   CallExpr *node,
   Class *cl)
{
   Expression* cause = node;
   if (node->args.size() > res.incompatibleArg) {
      cause = node->args[res.incompatibleArg].second.get();
   }

   if (node->generics.size() < cl->getGenerics().size()) {
      auto missingGeneric = cl->getGenerics().at(node->generics.size());

      RuntimeError::raise("Could not infer generic type " +
         missingGeneric->getGenericClassName() + " of class " + node->ident + " from context", node);
   }
   else if (node->generics.size() > cl->getGenerics().size()) {
      RuntimeError::raise("Mismatched generic type count: Expected " +
            std::to_string(cl->getGenerics().size()) + " but found " + std::to_string(node->generics.size()),
         node);
   }

   if (!res.foundType.empty() && !res.expectedType.empty()) {
      RuntimeError::raise(
         "No matching call for function " + node->ident + " found: Candidate function not viable: No known "
            "conversion from " + res.foundType + " to " + res.expectedType + " ", cause);
   }
   else {
      RuntimeError::raise("Class " + cl->getName() + " does not define function " + node->ident + " with "
         "given signature", cause);
   }
}

void TypeCheckPass::HandleCallOperator(CallExpr *node)
{
   auto latest = popTy();
   if (!isa<ObjectType>(latest) || !SymbolTable::hasClass(latest->getClassName())) {
      pushTy(latest);
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   string methodName = "postfix ()";
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   GetCallArguments(
      GetMethodOverloadArgs(cl, methodName),
      node
   );

   auto& givenArgs = node->resolvedArgs;
   auto callOpResult = cl->hasMethod(methodName, givenArgs);

   node->generics = callOpResult.generics;
   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise("Call operator with signature " + util::args_to_string(givenArgs) + " does"
               " not exist on class " + className, node);
   }

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

   if (isa<PointerType>(latest)) {
      latest = cast<PointerType>(latest)->getPointeeType();
   }

   if (!isa<FunctionType>(latest)) {
      RuntimeError::raise("Given object of type " + latest->toString() + " is not callable", node);
   }

   auto func = cast<FunctionType>(latest);

   std::vector<Type*> givenGenerics;
   std::vector<ObjectType*> neededGenerics;
   std::vector<Argument> neededArgs;
   for (const auto& arg : func->getArgTypes()) {
      neededArgs.emplace_back("", arg);
   }

   auto result = util::findMatchingCall(givenArgs, neededArgs, givenGenerics, neededGenerics);
   if (!result.isCompatible()) {
      Expression* cause = node;
      if (node->args.size() > result.incompatibleArg) {
         cause = node->args[result.incompatibleArg].second.get();
      }

      if (!result.foundType.empty() && !result.expectedType.empty()) {
         RuntimeError::raise("No matching call for function " + node->ident + " found: No known "
            "conversion from " + result.foundType + " to " + result.expectedType + " ", cause);
      }
      else {
         RuntimeError::raise("No matching call for function " + node->ident + " found", cause);
      }
   }

   result.func = func->getFunction();
   node->type = CallType::ANON_CALL;

   PrepareCallArgs(node->args, givenArgs, result);
   ApplyCasts(node->args, givenArgs, neededArgs, result.neededCasts);

   node->returnType = func->getReturnType()->deepCopy();
   node->functionType = func;

   if (node->returnType->isValueType()) {
      node->hasStructReturn = true;
      node->structReturnType = node->returnType;
   }
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(
   CallExpr *node)
{
   //MAINCALL
   for (auto& gen : node->generics) {
      resolve(&gen);
   }

   for (size_t i = node->resolvedArgs.size(); i < node->args.size(); ++i) {
      auto& arg = node->args.at(i);
      Type *ty = nullptr;
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
      else if (SymbolTable::hasClass(node->ident, importedNamespaces)) {
         HandleConstructorCall(node);
      }
      else {
         HandleFunctionCall(node);
      }
   }
   else if (node->type == CallType::METHOD_CALL) {
      HandleMethodCall(node);
   }

   return ReturnMemberExpr(node, node->returnType);
}

void TypeCheckPass::GetCallArguments(
   std::vector<std::vector<Argument> *> overloads,
   CallExpr *&node)
{
   return; // ;) 8=======D
   auto& callArgs = node->resolvedArgs;
   size_t i = 0;
   for (const auto& arg : node->args) {
      if (arg.second->needsContextualInformation()) {
         if (node->resolvedArgs.size() <= i) {

         }
      }
   }

   bool needsContextualInformation = false;
   for (size_t i = callArgs.size(); i < node->args.size(); ++i) {
      if (node->args.at(i).second->needsContextualInformation()) {
         needsContextualInformation = true;
         break;
      }
   }

   if (!needsContextualInformation) {
      for (size_t i = callArgs.size(); i < node->args.size(); ++i) {
         auto& arg = node->args.at(i);
         auto ty = arg.second->accept(*this);

         callArgs.emplace_back(arg.first, ty, arg.second);
      }

      return;
   }

   unordered_map<size_t, Type*> positionTypes;
   unordered_map<string, Type*> labeledTypes;

   for (const auto& overload : overloads) {
      size_t i = 0;
      for (const auto& arg : *overload) {
         auto posIndex = positionTypes.find(i);
         if (posIndex != positionTypes.end()) {
            if (*arg.type != positionTypes[i]) {
               positionTypes.erase(posIndex);
            }
         }
         else {
            positionTypes.emplace(i, arg.type);
         }

         auto labelIndex = labeledTypes.find(arg.label);
         if (labelIndex != labeledTypes.end()) {
            if (*arg.type != labeledTypes[arg.label]) {
               labeledTypes.erase(labelIndex);
            }
         }
         else {
            labeledTypes.emplace(arg.label, arg.type);
         }

         ++i;
      }
   }

   for (const auto& pos : positionTypes) {
      if (node->args.size() > pos.first) {
         node->args[pos.first].second->setContextualType(pos.second);
      }
   }

   for (const auto& label : labeledTypes) {
      auto index = std::find_if(
         node->args.begin(), node->args.end(),
         [label](const pair<string, Expression::SharedPtr>& p) {
            return p.first == label.first;
         }
      );

      if (index != node->args.end()) {
         index->second->setContextualType(label.second);
      }
   }

   for (size_t i = callArgs.size(); i < node->args.size(); ++i) {
      auto& arg = node->args.at(i);
      auto ty = arg.second->accept(*this);

      callArgs.emplace_back(arg.first, ty, arg.second);
   }
}

std::vector<std::vector<Argument>*> TypeCheckPass::GetMethodOverloadArgs(
   cl::Class *cl,
   string &methodName)
{
   std::vector<std::vector<Argument>*> overloadArgs;
   auto overloads = cl->getOverloads(methodName);
   for (auto it = overloads.first; it != overloads.second; ++it) {
      overloadArgs.push_back(&(it->second->arguments));
   }

   return overloadArgs;
}

std::vector<std::vector<Argument>*> TypeCheckPass::GetFunctionOverloadArgs(
   string &functionName)
{
   std::vector<std::vector<Argument>*> overloadArgs;
   auto overloads = SymbolTable::getFunction(functionName, importedNamespaces);
   for (auto it = overloads.first; it != overloads.second; ++it) {
      overloadArgs.push_back(&(it->second->getArguments()));
   }

   return overloadArgs;
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(MemberRefExpr *node)
{
   Type* latest;
   string className;
   Class* cl;

   if (node->parentExpr == nullptr && node->isEnumCase) {
      auto& inf = node->contextualType;

      if (node->contextualType == nullptr || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise("Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      node->isNsMember = true;
   }

   if (node->isNsMember) {
      if (hasVariable(node->ident)) {
         auto var = getVariable(node->ident, node);
         node->binding = var.first.second;

         auto res = var.first.first->deepCopy();
         res->isLvalue(true);

         return ReturnMemberExpr(node, res);
      }

      cl = SymbolTable::getClass(node->className);
      checkClassAccessibility(cl, node);

      if (!hasVariable(node->ident) && SymbolTable::hasClass(node->className)) {
         node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);

         if (cl->isEnum()) {
            auto en = static_cast<Enum*>(cl);

            size_t genericParamsGiven = 0;
            if (node->contextualType != nullptr && node->contextualType->isObject()) {
               genericParamsGiven = node->contextualType->getConcreteGenericTypes().size();
            }

            if (en->isGeneric() && genericParamsGiven < en->getGenerics().size()) {
               RuntimeError::raise("Could not infer generic type " +
                                 en->getGenerics()[genericParamsGiven]->getGenericClassName(), node);
            }
            if (!en->hasCase(node->ident)) {
               RuntimeError::raise("Enum " + node->className + " does not have a case '" +
                                 node->ident + "'", node);
            }

            auto obj = ObjectType::get(node->className);
            obj->isEnum(true);

            if (en->isGeneric()) {
               for (const auto &gen : node->contextualType->getConcreteGenericTypes()) {
                  obj->getConcreteGenericTypes().emplace(gen.first, gen.second->deepCopy());
               }
            }

            node->isEnumCase = true;
            node->caseVal = en->getCase(node->ident).rawValue;

            obj->setKnownEnumCase(&en->getCase(node->ident));

            return ReturnMemberExpr(node, obj);
         }
      }

      latest = cl->getType();
      node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);
      goto implicit_method_call;
   }

   latest = popTy();
   if (node->isPointerAccess) {
      if (!latest->isPointerTy()) {
         RuntimeError::raise("Value is not a pointer", node);
      }

      latest = latest->getPointeeType();
   }

   className = latest->getClassName();

   if (node->isTupleAccess) {
      if (!latest->isTupleTy()) {
         RuntimeError::raise("Cannot access indexed property on non-tuple value of type "
                     + latest->toString(), node);
      }

      auto asTuple = cast<TupleType>(latest);
      if (asTuple->getArity() <= node->tupleIndex) {
         RuntimeError::raise("Cannot access index " + std::to_string(node->tupleIndex) + " on tuple "
                     "with arity " + std::to_string(asTuple->getArity()), node);
      }

      node->fieldType = asTuple->getContainedType(node->tupleIndex)->deepCopy();
      node->fieldType->isLvalue(true);
      node->fieldType->isConst(latest->isConst());

      return ReturnMemberExpr(node, node->fieldType->deepCopy());
   }

   if (className.empty()) {
      RuntimeError::raise("Cannot access property " + node->ident + " on value of type "
               + latest->toString(), node);
   }

   cl = SymbolTable::getClass(className, importedNamespaces);
   node->className = className;

   checkClassAccessibility(cl, node);

   if (cl->isEnum()) {
      if (node->ident == "rawValue") {
         node->fieldType = IntegerType::get(64);
         node->isEnumRawValue = true;
         return ReturnMemberExpr(node, node->fieldType);
      }
      else {
         goto implicit_method_call;
      }
   }
   else if (cl->hasField(node->ident)) {
      Field *field = cl->getField(node->ident);

      // if this field needed initializing and we're in a constructor, erase it from the needed fields
      bool isUninitializedField = false;
      auto& uninitializedFields = latestScope->uninitializedFields;
      if (currentClass() == className && node->isLhsOfAssigment_ && uninitializedFields != nullptr) {
         auto index = std::find(uninitializedFields->begin(), uninitializedFields->end(), node->ident);
         if (index != uninitializedFields->end()) {
            uninitializedFields->erase(index);
            isUninitializedField = true;
         }
      }

      // Check if we're trying to assign a const field
      if (field->isConst && node->isLhsOfAssigment_ && !isUninitializedField) {
         RuntimeError::raise("Field " + field->fieldName + " of " + cl->getTypeName() + " is constant", node);
      }

      // Use a getter if available
      if (field->hasGetter && !node->isLhsOfAssigment_ && currentClass() != className) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            field->getterName

         );
         call->setMemberExpr(node->memberExpr);
         call->setIndex(node->startIndex, node->endIndex, node->line, node->sourceFileId);
         call->setParent(node->parent);

         node->getterOrSetterCall = call;

         pushTy(latest);
         return call->accept(*this);
      }

      // Use a setter if available (and we're on the left side of an assignment)
      if (field->hasSetter && node->isLhsOfAssigment_ && currentClass() != className) {
         Expression* current = node;
         node->isSetterCall = true;

         while (current->parentExpr != nullptr) {
            current = current->parentExpr;
         }

         current->isSetterCall = true;
         current->setterName = field->setterName;

         return latest->deepCopy();
      }

      auto& field_type = field->fieldType;
      auto& concreteGenerics = latest->getConcreteGenericTypes();

      Type* ty = field->fieldType->deepCopy();
      auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass() != cl->getName());

      node->fieldType = ty;
      node->lvalueCast = isGeneric;

      // returning an object with unresolved generics
      if (field->fieldType->isObject()) {
         auto returnCl = SymbolTable::getClass(field->fieldType->getClassName());
         if (returnCl->isGeneric()) {
            Type::resolveGeneric(&node->fieldType, latest->getConcreteGenericTypes());
         }
      }

      node->binding = field->mangledName;

      checkMemberAccessibility(cl, field->fieldName, field->accessModifier, node);

      node->fieldType->isLvalue(true);
      node->fieldType->isConst(field->isConst && !isUninitializedField);

      return ReturnMemberExpr(node, node->fieldType->deepCopy());
   }
   // method call with omitted parens
   else {
      implicit_method_call:
      auto possibleMethod = cl->hasMethod(node->ident, {});
      if (possibleMethod.isCompatible()) {
         CallExpr::SharedPtr call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{}, node->ident
         );

         call->generics = possibleMethod.generics;
         call->resolvedArgs = possibleMethod.resolvedArgs;

         CopyNodeProperties(node, call.get());
         pushTy(latest);

         node->parentExpr->memberExpr = call;
         return call->accept(*this);
      }
   }

   RuntimeError::raise("Field " + node->ident + " does not exist on class " +
         latest->toString(), node);
   llvm_unreachable("");
}

Type* TypeCheckPass::HandleCastOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   if (node->rhs->get_type() != NodeType::TYPE_REF) {
      RuntimeError::raise("Expected type name after 'as'", node->rhs.get());
   }

   toRvalueIfNecessary(fst, node->lhs);
   node->operandType = fst->deepCopy();

   if (node->op != "as!" && !fst->explicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot cast from " + fst->toString() + " to " +
               snd->toString(), node);
   }

   // check if cast returns an optional
   if (fst->isObject()) {
      auto fromClass = SymbolTable::getClass(fst->getClassName());
      if (fromClass->isBaseClassOf(snd->getClassName())) {
         SymbolTable::getClass(snd->getClassName())->needsTypeInfoGen(true);
         return ObjectType::getOptionOf(snd);
      }
   }

   node->castTo = snd;
   return snd;
}

Type* TypeCheckPass::HandleAssignmentOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   if (fst->isCStyleArray()) {
      //TODO check if sizes are comatible
      RuntimeError::raise("Cannot reassign c-style array", node->lhs.get());
   }
   else if (!snd->implicitlyCastableTo(fst)) {
      RuntimeError::raise("Cannot assign value of type " + snd->toString() + " to variable of "
               "type " + fst->toString(), node->lhs.get());
   }
   else if (*fst != snd) {
      if (warnCast(fst, snd)) {
         Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node);
      }

      wrapImplicitCast(node->rhs, snd, fst);
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

   if (isa<VoidType>(snd)) {
      node->isNullAssignment = true;
   }

   if (fst->isProtocol()) {
      node->isProtocolAssignment = true;
   }

   node->updateRefCount = fst->isObject() &&
      SymbolTable::getClass(fst->getClassName())->isClass();

   node->operandType = fst;
   return new VoidType;
}

Type* TypeCheckPass::HandleArithmeticOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   auto& op = node->op;

   if ((op == "+" || op == "-") && fst->isPointerTy() && snd->isPointerTy()) {
      if (!latestScope->unsafe) {
         RuntimeError::raise("Pointer arithmetic is only allowed in unsafe blocks", node);
      }

      Type* ptr;
      Type* intgr;

      if (fst->isLvalue() && snd->isLvalue()) {
         goto err;
      }

      if (fst->isLvalue()) {
         ptr = fst;
         intgr = snd;
      }
      else if (snd->isLvalue()) {
         ptr = snd;
         intgr = fst;
      }
      else {
         goto err;
      }

      if (!ptr->isInt64Ty()) {
         wrapImplicitCast(node->lhs, fst, IntegerType::ConstInt64);
      }
      if (!intgr->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt64);
      }

      node->pointerArithmeticType = ptr;
      node->operandType = IntegerType::ConstInt64;

      return ptr;
   }

   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%") {
      if ((!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd))) {
         goto err;
      }

      Type* ret_type = cast<PrimitiveType>(fst)->ArithmeticReturnType(op, snd);

      if (isa<VoidType>(ret_type)) {
         goto err;
      }

      if (*fst != ret_type) {
         wrapImplicitCast(node->lhs, fst, ret_type);
      }
      if (*snd != ret_type) {
         wrapImplicitCast(node->rhs, snd, ret_type);
      }

      ret_type->isLvalue(false);
      node->operandType = ret_type;

      return ret_type;
   }

   if (op == "**") {
      if (!snd->implicitlyCastableTo(IntegerType::ConstInt64)) {
         RuntimeError::raise("Right hand side of '**' must be of type integer", node->rhs.get());
      }

      Type* retType = fst->isIntegerTy() ? fst->deepCopy() : FPType::getDoubleTy();
      Type* doubleTy = FPType::ConstDoubleTy;

      if (*fst != doubleTy) {
         wrapImplicitCast(node->lhs, fst, FPType::ConstDoubleTy);
      }
      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt64);
      }

      retType->isLvalue(false);
      node->operandType = retType;

      return retType;
   }

   err:
   RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst->toString()
      + " and " + snd->toString(), node);
   llvm_unreachable("");
}

Type* TypeCheckPass::HandleBitwiseOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   auto& op = node->op;

   if (!fst->isIntegerTy() || !snd->isIntegerTy()) {
      RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst
               ->toString() + " and " + snd->toString(), node);
   }

   if (*snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = fst;
   return fst;
}

Type* TypeCheckPass::HandleLogicalOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   if (!fst->implicitlyCastableTo(IntegerType::ConstInt1) && !snd->implicitlyCastableTo(IntegerType::ConstInt1)) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
               ->toString() + " and " + snd->toString(), node);
   }

   if (!fst->isInt1Ty()) {
      wrapImplicitCast(node->lhs, fst, IntegerType::ConstInt1);
   }
   if (!snd->isInt1Ty()) {
      wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt1);
   }

   node->operandType = IntegerType::ConstInt1;
   return IntegerType::get(1);
}

void TypeCheckPass::HandleEnumComp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   node->isEnumComp = true;

   auto fstHasKnownCase = fst->hasKnownEnumCase();
   auto sndHasKnownCase = snd->hasKnownEnumCase();
   if (!fstHasKnownCase && !sndHasKnownCase) {
      return;
   }

   auto& associatedValues = fstHasKnownCase ? fst->getAssociatedTypes() : snd->getAssociatedTypes();
   node->knownCase = fstHasKnownCase ? fst->getKnownEnumCase() : snd->getKnownEnumCase();
   node->caseTypes = fstHasKnownCase ? fst->getKnownEnumCaseTypes() : snd->getKnownEnumCaseTypes();

   size_t i = 0;
   for (const auto& val : associatedValues) {
      if (val.second->isUnderscore()) {
         node->caseValues.push_back(nullptr);
      }
      else {
         node->caseValues.push_back(val.second);
      }
      ++i;
   }
}

void TypeCheckPass::HandleTupleComp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   assert(fst->isTupleTy() && snd->isTupleTy() && "Can't compare tuple with non-tuple!");

   auto fstAsTuple = cast<TupleType>(fst);

   node->isTupleComp = true;
   node->arity = fstAsTuple->getArity();
   node->tupleTypes = fstAsTuple->getContainedTypes(false);
   node->llvmTupleType = fstAsTuple->getLlvmType();
}

Type* TypeCheckPass::HandleEqualityOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   // pointer comparison operators
   if (node->op.length() == 3) {
      if (!isa<PointerType>(fst) && !isa<PointerType>(snd) && !isa<ObjectType>(fst) && !isa<ObjectType>(snd)) {
         RuntimeError::raise("Expected at least one operand of " + node->op + " to be a pointer", node);
      }

      if (!fst->explicitlyCastableTo(IntegerType::ConstInt64) ||
         !fst->explicitlyCastableTo(IntegerType::ConstInt64)
      ) {
         RuntimeError::raise("Binary operator " + node->op + " is not applicable to types " +
                     fst->toString() + " and " + snd->toString(), node);
      }

      if (!fst->isInt64Ty()) {
         wrapImplicitCast(node->lhs, fst, IntegerType::ConstInt64);
      }

      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt64);
      }

      node->operandType = IntegerType::ConstInt64;
      return IntegerType::get(1);
   }

   if ((!isa<PrimitiveType>(fst) && !fst->isEnum()) && !fst->isTupleTy() && !fst->isPointerTy()) {
      RuntimeError::raise("Cannot compare values of type " + fst
               ->toString() + " and " + snd->toString() + " for equality", node);
   }

   if (!snd->implicitlyCastableTo(fst)) {
      RuntimeError::raise("Cannot compare values of type " + fst
               ->toString() + " and " + snd->toString() + " for equality", node);
   }

   if (fst->isEnum()) {
      HandleEnumComp(fst, snd, node);
   }

   if (fst->isTupleTy()) {
      HandleTupleComp(fst, snd, node);
   }

   if (*snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = fst;
   return IntegerType::get(1);
}

Type* TypeCheckPass::HandleComparisonOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   if (!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd)) {
      RuntimeError::raise("Cannot apply binary operator '" + node->op + "' to values of type " + fst
               ->toString() + " and " + snd->toString(), node);
   }

   if (*snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = fst;

   if (node->boxedPrimitiveOp) {
      return ObjectType::get("Bool");
   }

   return IntegerType::get(1);
}

Type* TypeCheckPass::HandleOtherOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   auto& op = node->op;
   if (op == "..") {
      Type *intTy = IntegerType::get(64);
      if (!fst->implicitlyCastableTo(intTy)) {
         RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst
                     ->toString() + " and " + snd->toString(), node);
      }
      if (!snd->implicitlyCastableTo(intTy)) {
         RuntimeError::raise("Cannot apply binary operator '" + op + "' values of type " + fst
                     ->toString() + " and " + snd->toString(), node);
      }

      if (*fst != intTy) {
         wrapImplicitCast(node->lhs, fst, intTy);
      }
      if (*snd != intTy) {
         wrapImplicitCast(node->rhs, snd, intTy);
      }

      auto collTy = new CollectionType(intTy);
      collTy->isLvalue(false);
      node->operandType = intTy;

      return collTy;
   }

   RuntimeError::raise("Binary operator " + node->op + " is not defined for arguments of type " +
         fst->toString() + " and " + snd->toString(), node);

   return nullptr;
}

Type* TypeCheckPass::HandleBinaryOperator(
   Type *lhs,
   Type *rhs,
   BinaryOperatorType opTy,
   BinaryOperator *node)
{
   if (opTy != BinaryOperatorType::CAST) {
      if (lhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(lhs->getClassName()));
         if (!en->hasAssociatedValues()) {
            auto rawTy = en->getRawType()->deepCopy();
            wrapImplicitCast(node->lhs, lhs, rawTy);
            lhs = rawTy;
         }
      }
      if (rhs->isEnum()) {
         auto en = static_cast<cl::Enum *>(SymbolTable::getClass(rhs->getClassName()));
         if (!en->hasAssociatedValues()) {
            auto rawTy = en->getRawType()->deepCopy();
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

Type* TypeCheckPass::tryBinaryOperatorMethod(
   Type *fst,
   Type *snd,
   BinaryOperator *node,
   string &opName,
   bool wasLvalue)
{
   std::vector<Argument> args{ Argument { "", snd } };
   if (node->opType == BinaryOperatorType::CAST) {
      args.pop_back();
   }

   cdot::cl::Class* cl = SymbolTable::getClass(fst->getClassName(), importedNamespaces);

   auto binOpResult = cl->hasMethod(opName, args, {}, fst->getConcreteGenericTypes());
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
      node->operandType = fst;

      return call->accept(*this);
   }

   return nullptr;
}

Type* TypeCheckPass::tryFreeStandingBinaryOp(
   Type *fst,
   Type *snd,
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
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL, std::vector<Expression::SharedPtr>{ node->lhs,
         node->rhs }, freeOp.func->getName());

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = fst;

      return call->accept(*this);;
   }

   return nullptr;
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(BinaryOperator *node)
{
   //MAINBINARY
   auto opType = cdot::getBinaryOpType(node->op);
   node->opType = opType;

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      node->lhs->isLhsOfAssigment();
   }

   if (node->contextualType != nullptr) {
      node->lhs->setContextualType(node->contextualType);
   }

   bool isPreAssignmentOp = node->lhsType != nullptr;
   Type* fst = isPreAssignmentOp ? node->lhsType
                                 : node->lhs->accept(*this);

   if (node->lhs->isSetterCall) {
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs },
         node->lhs->setterName
      );

      node->overridenCall = call;
      node->operandType = fst;

      CopyNodeProperties(node, call.get());
      call->setMemberExpr(node->memberExpr);
      call->setParent(node->parent);

      pushTy(fst);
      return call->accept(*this);
   }

   bool wasLvalue = fst->needsLvalueToRvalueConv();
   if (isAssignment) {
      if (fst->isConst() && !fst->isCarrayElement()) {
         RuntimeError::raise("Trying to reassign constant", node->lhs.get());
      }
      else if (!fst->isLvalue()) {
         if (fst->isSelf()) {
            RuntimeError::raise("Cannot assign to 'self' in non-mutating function", node->lhs.get());
         }

         RuntimeError::raise("Cannot assign to rvalue of type " + fst->toString(), node->lhs.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      fst->isLvalue(false);
      node->isStructAssignment = fst->isStruct();
      node->isSelfAssignment = fst->isSelf();
   }
   else {
      toRvalueIfNecessary(fst, node->lhs, node->op != "===");
   }

   node->rhs->setContextualType(fst);
   Type* snd = isPreAssignmentOp ? node->rhsType
                                 : node->rhs->accept(*this);

   if (opType == BinaryOperatorType::CAST && node->rhs->get_type() == NodeType::TYPE_REF) {
      node->boxedResultType = std::static_pointer_cast<TypeRef>(node->rhs)
         ->getType()->getClassName();
   }

   if (!isPreAssignmentOp && (opType == BinaryOperatorType::ARITHMETIC || opType == BinaryOperatorType::EQUALITY ||
      opType == BinaryOperatorType::COMPARISON || opType == BinaryOperatorType::CAST ||
      opType == BinaryOperatorType::BITWISE || opType == BinaryOperatorType::LOGICAL))
   {
      node->lhsIsBoxed = fst->isBoxedPrimitive();
      node->rhsIsBoxed = snd->isBoxedPrimitive();

      if (node->lhsIsBoxed && (node->rhsIsBoxed || isa<PrimitiveType>(snd))) {
         auto unboxed = fst->unbox();
         node->boxedPrimitiveOp = true;
         wrapImplicitCast(node->lhs, fst, unboxed);
         fst = unboxed;
      }
      if (node->rhsIsBoxed && (node->lhsIsBoxed || isa<PrimitiveType>(fst))) {
         auto unboxed = snd->unbox();
         node->boxedPrimitiveOp = true;
         wrapImplicitCast(node->rhs, snd, unboxed);
         snd = unboxed;
      }
   }

   string opName = opType == BinaryOperatorType::CAST ? "infix as " + snd->toString()
                                                      : "infix " + node->op;

   if (fst->isObject() && SymbolTable::hasClass(fst->getClassName())) {
      auto methodRes = tryBinaryOperatorMethod(fst, snd, node, opName, wasLvalue);
      if (methodRes != nullptr) {
         return methodRes;
      }
   }

   auto freeOpRes = tryFreeStandingBinaryOp(fst, snd, node, opName, wasLvalue);
   if (freeOpRes != nullptr) {
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
            auto asInt = cast<IntegerType>(res);
            if (asInt->isUnsigned()) {
               className += "U";
            }

            className += "Int";
            auto bitwidth = asInt->getBitwidth();
            if (bitwidth != sizeof(int*) * 8) {
               className += std::to_string(bitwidth);
            }
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
         res = ObjectType::get(className);
         resolve(&res);
      }
   }

   return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(TertiaryOperator *node)
{
   Type* cond = node->condition->accept(*this);
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, IntegerType::ConstInt1);
   }
   else if (!cond->implicitlyCastableTo(IntegerType::ConstInt1)) {
      RuntimeError::raise("Condition of tertiary operator '?:' must be boolean or implicitly castable"
               " to bool", node);
   }

   toRvalueIfNecessary(cond, node->condition);

   Type* fst = node->lhs->accept(*this);
   Type* snd = node->rhs->accept(*this);

   if (!fst->implicitlyCastableTo(snd)) {
      RuntimeError::raise("Cannot apply tertiary operator '?:' to values of type " + fst->toString() +
               " and " + snd->toString(), node);
   }
   else if (*fst != snd) {
      if (warnCast(fst, snd)) {
         Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node->rhs.get());
      }
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->resultType = fst;
   return fst;
}

Type* TypeCheckPass::tryFreeStandingUnaryOp(
   Type *lhs,
   UnaryOperator *node,
   string &opName)
{
   std::vector<Argument> args{ Argument{"", lhs} };
   string methodName = (node->prefix ? "prefix " : "postfix ") + opName;

   auto freeOp = getFunction(methodName, args);
   if (freeOp.compatibility == CompatibilityType::COMPATIBLE) {
      pushTy(lhs);
      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL, std::vector<Expression::SharedPtr>{ node->target },
         freeOp.func->getName());

      // we already resolved the argument, don't want to visit it again
      call->resolvedArgs = args;
      call->memberExpr = node->memberExpr;
      call->parentExpr = node;
      CopyNodeProperties(node, call.get());

      node->overridenCall = call;
      node->operandType = lhs;

      return call->accept(*this);;
   }

   return nullptr;
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(UnaryOperator *node)
{
   string op = node->op;
   Type* target = node->target->accept(*this);

   auto freeStanding = tryFreeStandingUnaryOp(target, node, op);
   if (freeStanding != nullptr) {
      return freeStanding;
   }

   if (target->isObject()) {
      auto& className = target->getClassName();

      auto class_decl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = class_decl->hasMethod((node->prefix ? "prefix " : "postfix ") + op);

      if (unOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{},
            (node->prefix ? "prefix " : "postfix ") + op);

         node->overridenCall = call;
         node->operandType = target;
         call->memberExpr = node->memberExpr;

         toRvalueIfNecessary(target, node->target);
         pushTy(target);

         return call->accept(*this);
      }
   }

   if (op == "++" || op == "--") {
      if (!target->isLvalue()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to rvalue of "
                     "type " + target->toString(), node->target.get());
      }
      if (target->isConst()) {
         RuntimeError::raise("Unary operator " + op + " cannot be applied to 'let' constant", node->target.get());
      }

      auto pointee = target->toRvalue();
      if (!isa<PrimitiveType>(pointee)) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target->toString(),
            node->target.get());
      }

      node->operandType = pointee;
      return ReturnMemberExpr(node, pointee);
   }

   if (op == "*") {
      toRvalueIfNecessary(target, node->target, !node->isLhsOfAssigment_);

      if (!isa<PointerType>(target)) {
         RuntimeError::raise("Cannot dereference non-pointer type", node->target.get());
      }

      target = cast<PointerType>(target)->getPointeeType();
      if (node->isLhsOfAssigment_) {
         target->isLvalue(true);
      }

      node->operandType = target;
      return ReturnMemberExpr(node, target);
   }

   if (op == "&") {
      if (!target->isLvalue()) {
         RuntimeError::raise("Cannot apply unary operator '&' to non-reference value", node->target.get());
      }

      target->isLvalue(false);
      auto ptr = target->getPointerTo();

      return ReturnMemberExpr(node, ptr);
   }

   Type *result;
   if (target->needsLvalueToRvalueConv()) {
      lvalueToRvalue(node->target);
      target = target->toRvalue();
   }

   node->operandType = target;

   if (op == "+" || op == "-") {
      if (!isa<PrimitiveType>(target)) {
         RuntimeError::raise("Unary operator " + op + " is not applicable to type " + target->toString(),
            node->target.get());
      }
      if (op == "-" && isa<IntegerType>(target) && cast<IntegerType>(target)->isUnsigned()) {
         RuntimeError::raise("Unary operator '-' cannot be applied to unsigned integer", node->target.get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!isa<IntegerType>(target)) {
         RuntimeError::raise("Unary operator '~' is only applicable to type Int", node->target.get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = IntegerType::get(1);
      if (!target->implicitlyCastableTo(boolTy)) {
         RuntimeError::raise("Unary operator '!' is not applicable to type " + target
                     ->toString(), node->target.get());
      }

      if (target != boolTy) {
         if (!target->isObject() && target->getClassName() == "Bool") {
            Warning::issue("Implicit cast to boolean", node->target.get());
         }
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise("Unary operator " + node->op + " is not defined on class " +
               target->toString(), node);
   }

   return ReturnMemberExpr(node, result);
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(BreakStmt *node)
{
   break_(node);
   return nullptr;
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(ContinueStmt *node)
{
   continue_(node);
   return nullptr;
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(IfStmt *node)
{
   Type* cond = node->condition->accept(*this);
   toRvalueIfNecessary(cond, node->condition);

   auto boolTy = IntegerType::get(1);
   if (cond->isObject() && cond->getClassName() == "Bool") {
      wrapImplicitCast(node->condition, cond, boolTy);
   }
   else if (!cond->implicitlyCastableTo(boolTy)) {
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

   return nullptr;
}

bool TypeCheckPass::matchableAgainst(
   Type*& matchTy,
   std::shared_ptr<CaseStmt> const& case_)
{
   if (case_->isDefault) {
      return true;
   }

   auto& caseVal = case_->caseVal;

   if (isa<PrimitiveType>(matchTy) || matchTy->isTupleTy()) {
      caseVal->setContextualType(matchTy);
      auto givenTy = caseVal->accept(*this);
      case_->caseType = givenTy;

      return givenTy->implicitlyCastableTo(matchTy);
   }

   if (matchTy->isEnum() && caseVal->get_type() == NodeType::MEMBER_EXPR) {
      auto memExpr = std::static_pointer_cast<MemberRefExpr>(caseVal);
      if (!memExpr->isEnumCase) {
         return false;
      }

      auto en = static_cast<Enum*>(SymbolTable::getClass(matchTy->getClassName()));
      if (!en->hasCase(memExpr->ident)) {
         return false;
      }

      case_->enumCaseVal = &en->getCase(memExpr->ident);
      return true;
   }

   if (matchTy->isEnum() && caseVal->get_type() == NodeType::CALL_EXPR) {
      auto callExpr = std::static_pointer_cast<CallExpr>(caseVal);
      if (!callExpr->isEnumCase) {
         return false;
      }

      auto en = static_cast<Enum*>(SymbolTable::getClass(matchTy->getClassName()));
      auto& givenArgs = callExpr->resolvedArgs;
      givenArgs.reserve(callExpr->args.size());
      std::vector<string> letIdents;

      bool var = false;
      bool isLetExpr = false;
      for (const auto& arg : callExpr->args) {
         if (arg.second->get_type() == NodeType::IDENTIFIER_EXPR) {
            auto ident = std::static_pointer_cast<IdentifierRefExpr>(arg.second);
            if (ident->isLetExpr_ || ident->isVarExpr_) {
               isLetExpr = true;
               var = ident->isVarExpr_;
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
                  case_->letIdentifiers.emplace_back(letIdents[i], val.associatedValues[i].second);
               }

               return true;
            }
         }

         return false;
      }

      for (size_t i = givenArgs.size(); i < callExpr->args.size(); ++i) {
         const auto& arg = callExpr->args.at(i);
         if (arg.second->get_type() == NodeType::LAMBDA_EXPR) {
            givenArgs.emplace_back(arg.first, nullptr);
         }
         else {
            givenArgs.emplace_back(arg.first, arg.second->accept(*this));
         }
      }

      auto compat = en->hasCase(callExpr->ident, givenArgs, callExpr->generics);
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

      auto cl = matchTy->getClass();
      auto operatorEquals = cl->hasMethod("infix ==", { Argument{ "", givenTy } });
      if (operatorEquals.compatibility == CompatibilityType::COMPATIBLE) {
         case_->operatorEquals = operatorEquals.method;
         return true;
      }

      // otherwise === will be used
      if (cl->isClass()) {
         return true;
      }

      return false;
   }

   return false;
}

Type* TypeCheckPass::visit(MatchStmt *node)
{
   Type* switchType = node->switchValue->accept(*this);
   toRvalueIfNecessary(switchType, node->switchValue);

   if (switchType->isObject() && !switchType->isEnum()) {
      auto cl = SymbolTable::getClass(switchType->getClassName());
      string protName = "Equatable";

      if (!cl->conformsTo(protName)) {
         RuntimeError::raise("Types used as a match value must conform to 'Equatable'", node);
      }
   }
   else if (!isa<IntegerType>(switchType) && !isa<FPType>(switchType) &&
      !switchType->isTupleTy() && !switchType->isEnum())
   {
      RuntimeError::raise("Types used as a match value must conform to 'Equatable'", node);
   }

   std::vector<long> rawCaseValues;
   std::vector<size_t> checkIfContinuable;
   size_t numCases = node->cases.size();
   size_t i = 0;
   bool allCasesReturn = true;
   for (const auto& case_ : node->cases) {
      if (!matchableAgainst(switchType, case_)) {
         RuntimeError::raise("Cannot match given value against value of type " +
                     switchType->toString(), case_->caseVal.get());
      }
      else if (case_->caseType != nullptr && *case_->caseType != switchType) {
         wrapImplicitCast(case_->caseVal, case_->caseType, switchType);
      }

      if (case_->isDefault) {
         node->hasDefault = true;
         node->defaultIndex = i;
      }

      ++latestScope->branches;

      pushLoopScope(true, false);

      if (switchType->isEnum() && !case_->isDefault) {
         if (std::find(rawCaseValues.begin(), rawCaseValues.end(), case_->enumCaseVal->rawValue)
            != rawCaseValues.end())
         {
            RuntimeError::raise("Duplicate case " + case_->enumCaseVal->name, case_.get());
         }

         rawCaseValues.push_back(case_->enumCaseVal->rawValue);

         if (case_->isEnumLetCase || case_->isEnumVarCase) {
            auto en = static_cast<Enum*>(SymbolTable::getClass(switchType->getClassName()));

            for (auto& val : case_->letIdentifiers) {
               auto ty = val.second->deepCopy();
               if (ty->isGeneric() && currentClass() != en->getName()) {
                  case_->needsGenericCast = true;
                  case_->genericOriginTy = ty->deepCopy();

                  Type::resolveGeneric(&ty, switchType->getConcreteGenericTypes());

                  case_->genericDestTy = ty->deepCopy();
                  case_->genericDestTy->isLvalue(true);
               }

               ty->isLvalue(true);
               ty->isConst(case_->isEnumLetCase);

               case_->letBindings.push_back(declareVariable(val.first, ty));
            }
         }
      }

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

   node->isIntegralSwitch = switchType->isIntegerTy();
   node->switchType = switchType;
   node->allCasesReturn = allCasesReturn;

   return nullptr;
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(CaseStmt *node)
{
   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(LabelStmt *node)
{
   if (std::find(labels.begin(), labels.end(), node->labelName) != labels.end()) {
      RuntimeError::raise("Label '" + node->labelName + "' already exists in the same scope", node);
   }

   labels.push_back(node->labelName);
   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(GotoStmt *node)
{
   if (!has_label(node->labelName)) {
      RuntimeError::raise("No label '" + node->labelName + "' to go to", node);
   }

   return nullptr;
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(FuncArgDecl *node)
{
   if (!node->argType->resolved) {
      node->argType->accept(*this);
   }

   auto &ts = node->argType->getType();
   node->isStruct = ts->isStruct();
   node->mut = ts->isLvalue();

   if (node->mut) {
      node->isStruct = false; // we don't want to memcpy a ref parameter
      node->argType->type = ts;
   }

   if (node->defaultVal) {
      node->defaultVal->setContextualType(node->argType->getType(true));
      Type* def_type = node->defaultVal->accept(*this);

      if (!ts->implicitlyCastableTo(def_type)) {
         RuntimeError::raise("Default value for parameter " + node->argName + " must be of type "
                     "" + node->argType->toString(), node->defaultVal.get());
      }
   }

   return ts;
}

/**
 * Returns the current context
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(ReturnStmt *node)
{
   if (node->returnValue) {
      node->returnValue->setContextualType(latestScope->declaredReturnType);

      Type* retType = node->returnValue->accept(*this);
      toRvalueIfNecessary(retType, node->returnValue, !latestScope->declaredReturnType->isLvalue());

      node->returnType = latestScope->declaredReturnType->deepCopy();
      return_(retType, node->returnValue.get());

      if (latestScope->declaredReturnType->isStruct()) {
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

      if (*retType != latestScope->declaredReturnType) {
         wrapImplicitCast(node->returnValue, retType, latestScope->declaredReturnType);
      }
   }
   else {
      node->returnType = new VoidType();
      return_(node->returnType, node);
   }

   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(Expression *node)
{
   return nullptr;
}

void TypeCheckPass::DefineField(
   FieldDecl *node,
   cdot::cl::Class *cl)
{
   if (node->is_declaration) {
      return;
   }

   auto& field_type = node->type->getType();
   if (node->defaultVal != nullptr) {
      node->defaultVal->setContextualType(field_type);
      Type* def_type = node->defaultVal->accept(*this);
      resolve(&def_type);

      toRvalueIfNecessary(def_type, node->defaultVal);
      auto declaredField = cl->getField(node->fieldName);
      if (declaredField != nullptr) {
         declaredField->defaultVal = node->defaultVal;
      }

      if (field_type->isInferred()) {
         field_type = def_type->deepCopy();

         if (node->isStatic) {
            auto ty = field_type->deepCopy();
            ty->isLvalue(true);

            SymbolTable::declareVariable(node->binding, ty, node->am, currentNamespace.back());
         }
         else {
            auto ty = field_type->deepCopy();
            cl->getField(node->fieldName)->fieldType = ty;
            if (node->hasGetter) {
               node->getterMethod->returnType = ty;
            }
            if (node->hasSetter) {
               node->setterMethod->arguments.front().type = ty;
            }
         }
      }
      else if (!field_type->implicitlyCastableTo(def_type)) {
         RuntimeError::raise("Default value for field " + node->fieldName + " must be of type " +
                     node->type->toString(), node->defaultVal.get());
      }
      else if (*def_type != field_type) {
         if (warnCast(def_type, field_type)) {
            Warning::issue("Implicit cast from " + def_type->toString() + " to " + node->type->toString(),
               node->defaultVal.get());
         }
      }
   }

   if (node->isStatic) {
      node->binding = ns_prefix() + node->fieldName;
      return;
   }

   if (node->hasGetter && node->getterBody != nullptr) {
      pushMethodScope(node->getterMethod);
      latestScope->currentSelf = SymbolTable::mangleVariable(self_str, latestScope->id);

      node->getterBody->accept(*this);
      node->getterSelfBinding = latestScope->currentSelf;

      if (latestScope->returned == 0) {
         RuntimeError::raise("Returning Void from a method with declared return type " +
                     node->getterMethod->returnType->toString(), node);
      }

      if (latestScope->branches - latestScope->returned > 0) {
         RuntimeError::raise("Not all code paths return a value", node->getterBody.get());
      }

      popScope();
   }

   if (node->hasSetter && node->setterBody != nullptr) {
      pushMethodScope(node->setterMethod);
      string newValStr = "newVal";

      auto typeref = std::make_shared<TypeRef>(field_type->deepCopy());
      typeref->resolved = true;

      node->newVal = std::make_shared<FuncArgDecl>(newValStr, typeref);

      latestScope->currentSelf = SymbolTable::mangleVariable(self_str, latestScope->id);
      node->newVal->binding = declareVariable(newValStr, field_type->deepCopy());

      node->setterSelfBinding = latestScope->currentSelf;
      node->setterBody->accept(*this);

      popScope();
   }
}

void TypeCheckPass::DefineClass(
   ClassDecl *node,
   cdot::cl::Class *cl)
{
   pushClassScope(cl, node->className);

   for (const auto& decl : node->innerDeclarations) {
      decl->accept(*this);
   }

   for (const auto &method : node->methods) {
      DefineMethod(method.get(), cl);
   }

   for (const auto &constr : node->constructors) {
      DefineConstr(constr.get(), cl);
   }

   if (node->destructor != nullptr) {
      DefineDestr(node->destructor.get(), cl);
   }

   popClassScope();

   try {
      cl->finalize();
   } catch (string& err) {
      RuntimeError::raise(err, node);
   }
}

void TypeCheckPass::DefineMethod(
   MethodDecl *node,
   cdot::cl::Class *cl)
{
   if (node->isAlias || node->is_declaration) {
      return;
   }

   auto& return_type = node->returnType->getType();
   pushMethodScope(node->method);

   if (!node->isStatic) {
      latestScope->currentSelf = SymbolTable::mangleVariable(self_str, latestScope->id);
      node->selfBinding = latestScope->currentSelf;
   }

   attributes = node->attributes;

   if (node->body) {
      for (const auto &arg : node->args) {
         arg->visitDefault = true;
         arg->accept(*this);
         arg->binding = declareVariable(arg->argName, arg->argType->getType());
      }

      latestScope->mutableSelf = node->isMutating_;

      node->body->accept(*this);

      if (latestScope->returned == 0) {
         if (!isa<VoidType>(node->returnType->getType())) {
            RuntimeError::raise("Returning Void from a method with declared return type " +
                           node->returnType->getType()->toString(), node->returnType.get());
         }
      }
      else {
         return_type = latestScope->declaredReturnType;
      }

      node->method->returnType = return_type;

      if (latestScope->branches - latestScope->returned > 0 && !isa<VoidType>(return_type) &&
         !latestScope->declaredReturnType->isNullable())
      {
         RuntimeError::raise("Not all code paths return a value", node->body.get());
      }
   }
   else if (return_type->isInferred()) {
      return_type = new VoidType();
   }

   popScope();
   attributes.clear();
}

namespace {
   using cdot::cl::Field;

   bool fieldNeedsInitializing(const Field::SharedPtr& field)
   {
      return field->defaultVal == nullptr;
   }
}

void TypeCheckPass::DefineConstr(
   ConstrDecl *node,
   cdot::cl::Class *cl)
{
   if (node->memberwise || node->body == nullptr) {
      return;
   }

   auto voidTy = new VoidType;
   pushMethodScope(node->method);

   latestScope->currentSelf = SymbolTable::mangleVariable(self_str, latestScope->id);
   node->selfBinding = latestScope->currentSelf;

   std::vector<string> uninitialized;
   for (const auto& field : cl->getFields()) {
      if (fieldNeedsInitializing(field.second)) {
         uninitialized.push_back(field.second->fieldName);
      }
   }

   latestScope->uninitializedFields = &uninitialized;
   latestScope->mutableSelf = true;

   for (auto& arg : node->args) {
      arg->binding = declareVariable(arg->argName, arg->argType->getType());
   }

   node->body->accept(*this);

   if (!uninitialized.empty()) {
      RuntimeError::raise("Member " + uninitialized.front() + " of "
               "class " + cl->getName() + " needs to be initialized", node->body.get());
   }

   popScope();
   delete voidTy;
}

void TypeCheckPass::DefineDestr(DestrDecl *node, cdot::cl::Class *cl)
{
   auto voidTy = new VoidType;
   pushMethodScope(node->declaredMethod);
   latestScope->currentSelf = SymbolTable::mangleVariable(self_str, latestScope->id);
   node->selfBinding = latestScope->currentSelf;

   node->body->accept(*this);

   popScope();
   delete voidTy;
}

/**
 * Declares a new class
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(ClassDecl *node)
{
   DefineClass(node, node->declaredClass);
   return nullptr;
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(ConstrDecl *node)
{
   return nullptr;
}

Type* TypeCheckPass::visit(DestrDecl *node)
{
   return nullptr;
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(FieldDecl *node)
{
   return nullptr;
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(MethodDecl *node)
{
   return nullptr;
}

Type* TypeCheckPass::visit(EnumDecl *node)
{
   pushNamespace(node->className);

   for (const auto& inner : node->innerDeclarations) {
      inner->accept(*this);
   }

   popNamespace();

   auto en = node->declaredEnum;
   pushClassScope(en, node->className);

   for (const auto &method : node->methods) {
      DefineMethod(method.get(), en);
   }

   popClassScope();

   try {
      en->finalize();
   } catch (string err) {
      RuntimeError::raise(err, node);
   }

   return nullptr;
}

/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(LambdaExpr *node)
{
   node->returnType->accept(*this);
   Type* returnType = node->returnType->getType()->deepCopy();

   if (node->contextualType != nullptr && isa<FunctionType>(node->contextualType)) {
      auto asFunc = cast<FunctionType>(node->contextualType);
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
         auto& given = arg->argType->getType();
         auto& needed = neededArgs[i];

         if (given->isInferred()) {
            arg->argType->setType(needed->deepCopy());
         }
         else if (!given->implicitlyCastableTo(needed)) {
            RuntimeError::raise("Incompatible argument types: No implicit conversion from " +
               given->toString() + " to " + needed->toString(), arg.get());
         }

         ++i;
      }

      if (node->returnType->getType()->isInferred()) {
         returnType = asFunc->getReturnType()->deepCopy();
         node->returnType->setType(returnType);
      }
      else if (!returnType->implicitlyCastableTo(asFunc->getReturnType())) {
         RuntimeError::raise("Incompatible return types: No implicit conversion from " +
            returnType->toString() + " to " + asFunc->getReturnType()->toString(), node->returnType.get());
      }
   }

   bool isSingleStmt = node->body->get_type() != NodeType::COMPOUND_STMT;
   if (isSingleStmt && !isa<VoidType>(returnType) && !isa<AutoType>(returnType)) {
      auto asExpr = std::dynamic_pointer_cast<Expression>(node->body);
      if (!asExpr) {
         RuntimeError::raise("Expected single-statement lambda to contain a valid expression", node->body.get());
      }

      node->body = std::make_shared<ReturnStmt>(asExpr);
   }

   std::vector<Type*> argTypes;
   string anon = "__anon";
   Function* fun = new Function(anon, returnType);
   std::vector<pair<string, Type*>> captures;

   pushFunctionScope(returnType, "", "", true);
   latestScope->captures = &captures;

   for (const auto& arg : node->args) {
      if (arg->argType == nullptr || isa<AutoType>(arg->argType->getType(true))) {
         RuntimeError::raise("Could not infer type of argument " + arg->argName, arg.get());
      }
      if (!arg->argType->resolved) {
         arg->accept(*this);
      }

      argTypes.push_back(arg->argType->getType()->deepCopy());
      fun->addArgument(arg->argType->getType(), arg->defaultVal, arg->argName);

      if (arg->argName != "_") {
         arg->binding = declareVariable(arg->argName, arg->argType->getType()->deepCopy());
      }
   }

   auto ret = node->body->accept(*this);
   if (latestScope->returned > 0 && !isa<VoidType>(returnType)) {
      if (isa<AutoType>(returnType)) {
         if (isSingleStmt) {
            returnType = ret;
            node->returnType->setType(ret->deepCopy());
         }
         else {
            returnType = latestScope->declaredReturnType;
            node->returnType->setType(latestScope->declaredReturnType->deepCopy());
         }
      }
      else {
         RuntimeError::raise("Not all code paths return a value", node);
      }
   }
   else if (ret && !ret->implicitlyCastableTo(returnType)) {
      RuntimeError::raise("Returning value of type " + ret->toString() + " from function with "
               "declared return type " + returnType->toString(), node);
   }

   auto funcTy = new FunctionType(returnType->deepCopy(), argTypes);
   funcTy->setFunction(fun);

   node->lambdaType = funcTy;
   node->captures = captures;
   popScope();

   return ReturnMemberExpr(node, funcTy->deepCopy());
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(UsingStmt *node)
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

   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();

   return nullptr;
}

Type* TypeCheckPass::visit(ImplicitCastExpr *node)
{
   resolve(&node->to);
   node->target->accept(*this);

   return node->to;
}

Type* TypeCheckPass::visit(TypedefDecl *node)
{
   return nullptr;
}

Type* TypeCheckPass::visit(TypeRef *node)
{
   if (node->resolved) {
      return node->type->deepCopy();
   }

   if (!node->resolved) {
      if (isa<ObjectType>(node->type) && node->type->getClassName() == "Self") {
         if (!latestScope->inProtocol) {
            RuntimeError::raise("'Self' is only valid in protocol definitions", node);
         }

         SymbolTable::getClass(currentClass())->hasAssociatedTypes(true);
      }

      resolve(&node->type, node);
      node->resolved = true;

      if (node->type->isObject() && !SymbolTable::hasClass(node->type->getClassName())) {
         RuntimeError::raise("Unknown typename " + node->type->toString(), node);
      }
      if (node->type->isObject() && !node->type->isGeneric() &&
         SymbolTable::getClass(node->type->getClassName())->hasAssociatedTypes() &&
         !node->isGenericConstraint_ && !latestScope->inProtocol)
      {
         RuntimeError::raise("Protocols with associated types or self constraints can only be used as"
                     " generic constraints", node);
      }
   }

   return node->type->deepCopy();
}

Type* TypeCheckPass::visit(DeclareStmt *node)
{
   for (const auto& decl : node->declarations) {
      decl->accept(*this);
   }

   return nullptr;
}

Type* TypeCheckPass::visit(LvalueToRvalue* node)
{
   llvm_unreachable("Should only be applied after evaluating the node");
}

Type* TypeCheckPass::visit(DebugStmt* node)
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

   return nullptr;
}

Type* TypeCheckPass::visit(Statement* node)
{
   return nullptr;
}

Type* TypeCheckPass::visit(TupleLiteral* node)
{
   std::vector<pair<string, Type*>> containedTypes;

   for (const auto& el : node->elements) {
      auto ty = el.second->accept(*this);
      containedTypes.emplace_back(el.first, ty);
   }

   auto tupleTy = new TupleType(containedTypes);
   if (node->contextualType && !isa<AutoType>(node->contextualType)) {
      if (!tupleTy->implicitlyCastableTo(node->contextualType)) {
         RuntimeError::raise("Incompatible types " + tupleTy->toString() + " and " +
                     node->contextualType->toString(), node);
      }

      auto asTuple = cast<TupleType>(node->contextualType);
      auto arity = tupleTy->getArity();
      for (size_t i = 0; i < arity; ++i) {
         if (*tupleTy->getContainedType(i) != asTuple->getContainedType(i)) {
            wrapImplicitCast(node->elements.at(i).second, tupleTy->getContainedType(i), asTuple->getContainedType(i));
         }
      }

      delete tupleTy;
      tupleTy = cast<TupleType>(asTuple->deepCopy());
   }

   node->tupleType = tupleTy;
   return tupleTy->deepCopy();
}