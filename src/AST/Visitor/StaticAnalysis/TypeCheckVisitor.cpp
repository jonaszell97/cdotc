//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "TypeCheckVisitor.h"
#include "../Visitor.cpp"
#include "../../SymbolTable.h"
#include "Class.h"
#include "Enum.h"
#include "../../../Message/Warning.h"
#include "../../Statement/Declaration/ExtendStmt.h"
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
std::vector<string> TypeCheckVisitor::currentNamespace = { "" };
std::vector<string> TypeCheckVisitor::importedNamespaces = {""};
std::vector<ObjectType*>* TypeCheckVisitor::currentClassGenerics = nullptr;
bool TypeCheckVisitor::inProtocolDefinition = false;

TypeCheckVisitor::TypeCheckVisitor() {

}

TypeCheckVisitor::TypeCheckVisitor(TypeCheckVisitor *parent) :
   parent(parent),
   continuable(parent->continuable),
   breakable(parent->breakable),
   scope(parent->scope + std::to_string(parent->children.size())),
   currentClass(parent->currentClass),
   currentSelf(parent->currentSelf),
   currentFunction(parent->currentFunction),
   declaredReturnType(parent->declaredReturnType),
   currentBlockUnsafe(parent->currentBlockUnsafe),
   uninitializedFields(parent->uninitializedFields),
   capturedVariables(parent->capturedVariables),
   isLambdaVisitor(parent->isLambdaVisitor)
{
   parent->addChild(this);
}

namespace {
   void CopyScopeProps(Scope* src, Scope* dst) {
      dst->currentClass = src->currentClass;
      dst->currentSelf = src->currentSelf;
      dst->currentFunction = src->currentFunction;
      dst->enclosingScope = src;
   }
}

void TypeCheckVisitor::pushFunctionScope(Type *returnType, bool isLambda) {
   Scope scope;
   scope.isFunctionRoot = true;
   scope.isLambdaRoot = isLambda;
   scope.returnable = true;
   scope.declaredReturnType = returnType;

   CopyScopeProps(&Scopes.top(), &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckVisitor::pushMethodScope(Type *returnType, string &className) {
   Scope scope;
   scope.isFunctionRoot = true;
   scope.returnable = true;
   scope.declaredReturnType = returnType;

   CopyScopeProps(&Scopes.top(), &scope);
   scope.currentClass = className;

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckVisitor::pushLoopScope(bool continuable, bool breakable) {
   Scope scope;
   scope.continuable = continuable;
   scope.breakable = breakable;

   CopyScopeProps(&Scopes.top(), &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void TypeCheckVisitor::popScope() {
   Scopes.pop();
   latestScope = &Scopes.top();
}

TypeCheckVisitor TypeCheckVisitor::makeFunctionVisitor(Type *declaredReturnTy) {
   TypeCheckVisitor funcVisitor(this);

   funcVisitor.declaredReturnType = declaredReturnTy;
   funcVisitor.returnable = true;

   return funcVisitor;
}

TypeCheckVisitor TypeCheckVisitor::makeMethodVisitor(Type *declaredReturnTy, string &className) {
   TypeCheckVisitor funcVisitor(this);

   funcVisitor.declaredReturnType = declaredReturnTy;
   funcVisitor.returnable = true;
   funcVisitor.currentClass = className;

   return funcVisitor;
}

void TypeCheckVisitor::connectTree(AstNode *root) {
   for (const auto& child : root->get_children()) {
      child->parent = root;
      connectTree(child.get());
   }
}

void TypeCheckVisitor::dump() {
   int tab = 0;
   auto current = parent;
   while (current != nullptr) {
      ++tab; current = current->parent;
   }
}

void TypeCheckVisitor::DeclareClasses(CompoundStmt::SharedPtr root) {

   for (const auto& stmt : root->getStatements()) {
      if (stmt->get_type() == NodeType::CLASS_DECL) {
         auto cl_dec = std::static_pointer_cast<ClassDecl>(stmt);
         DeclareClass(cl_dec.get());
      }
      else if (stmt->get_type() == NodeType::ENUM_DECL) {
         auto enumDecl = std::static_pointer_cast<EnumDecl>(stmt);
         DeclareEnum(enumDecl.get());
      }
      else if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
         auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
         pushNamespace(ns_dec->nsName);
         DeclareClasses(ns_dec->contents);
         popNamespace();
      }
      else if (stmt->get_type() == NodeType::USING_STMT) {
         stmt->accept(*this);
      }
      else if (stmt->get_type() == NodeType::TYPEDEF_DECL) {
         stmt->accept(*this);
      }
      else if (stmt->get_type() == NodeType::EOF_STMT) {
         stmt->accept(*this);
      }
   }

   for (const auto& stmt : root->getStatements()) {
      if (stmt->get_type() == NodeType::CLASS_DECL) {
         auto cl = std::static_pointer_cast<ClassDecl>(stmt);
         DeclareClassMethods(cl.get());
      }
      else if (stmt->get_type() == NodeType::ENUM_DECL) {
         auto enumDecl = std::static_pointer_cast<EnumDecl>(stmt);
         DeclareEnumMethods(enumDecl.get());
      }
      else if (stmt->get_type() == NodeType::NAMESPACE_DECL) {
         auto ns_dec = std::static_pointer_cast<NamespaceDecl>(stmt);
         pushNamespace(ns_dec->nsName);
         DeclareClasses(ns_dec->contents);
         popNamespace();
      }
      else if (stmt->get_type() == NodeType::FUNCTION_DECL) {
         DeclareFunction(std::static_pointer_cast<FunctionDecl>(stmt).get());
      }
      else if (stmt->get_type() == NodeType::USING_STMT) {
         stmt->accept(*this);
      }
      else if (stmt->get_type() == NodeType::EOF_STMT) {
         stmt->accept(*this);
      }
   }
}

/**
 * Declares a variable in the current context
 * @param name
 * @param type
 * @param cause
 */
string TypeCheckVisitor::declare_var(string& name, Type* type, bool global, AstNode *cause) {
   string var_name;
   if (global) {
      var_name = ns_prefix() + name;
   }
   else {
      var_name = name + scope;
   }

   if (SymbolTable::hasVariable(var_name, importedNamespaces)) {
      RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + name, cause);
   }

   if (global) {
      type = type->getPointerTo();
   }

   SymbolTable::declareVariable(var_name, type);

   return var_name;
}

/**
 * Declares a function (overload) in the current context
 * @param fun
 * @param args
 * @param ret
 * @param cause
 */
Type*& TypeCheckVisitor::declare_fun(
   Function::UniquePtr&& func,
   std::vector<ObjectType*>& generics,
   AstNode* decl
) {
   auto overloads = SymbolTable::getFunction(func->getName(), currentNamespace.back());
   auto& args = func->getArgTypes();
   auto score = util::func_score(args);

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;

//      auto res = util::func_call_compatible(func->getArgTypes(), overload->getArgTypes(), generics,
//         overload->getGenerics());
//
//      if (res.perfect_match) {
//         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare function " + func->getName() + " with the same "
//            "signature as a previous declaration", decl);
//      }
   }

   auto& name = func->getName();
   auto& ret = func->getReturnType();

   SymbolTable::declareFunction(name, std::move(func));

   return ret;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckVisitor::pushTy(Type *type) {
   typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
Type* TypeCheckVisitor::popTy() {
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
pair<pair<Type*, string>, bool> TypeCheckVisitor::get_var(
   string& ident, 
   AstNode* cause)
{
   auto current = this;
   auto& ns = currentNamespace.back();
   string curr;
   bool escapesLambdaScope = false;

   while (current != nullptr) {
      curr = ident + current->scope;
      if (SymbolTable::hasVariable(curr)) {
         break;
      }

      if (current->isLambdaRoot) {
         escapesLambdaScope = true;
      }

      current = current->parent;
   }

   if (current == nullptr) {
      if (SymbolTable::hasVariable(ident, importedNamespaces)) {
         return { SymbolTable::getVariable(ident, importedNamespaces), false };
      }

      RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident, cause);
   }

   return { SymbolTable::getVariable(curr, ns), escapesLambdaScope };
}

bool TypeCheckVisitor::has_var(string ident) {
   auto current = this;
   auto& ns = currentNamespace.back();

   while (current != nullptr) {
      if (SymbolTable::hasVariable(ident + current->scope, ns)) {
         break;
      }

      current = current->parent;
   }

   if (current == nullptr) {
      return false;
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
FunctionResult TypeCheckVisitor::get_fun(
   string& fun,
   std::vector<Type*>& args,
   std::vector<Type*>& generics,
   std::vector<string>& argLabels,
   std::vector<pair<string, std::shared_ptr<Expression>>>& argValues)
{
   string context;
   auto overloads = SymbolTable::getFunction(fun, importedNamespaces);

   FunctionResult result;
   result.compatibility = CompatibilityType::FUNC_NOT_FOUND;

   int bestMatch = 0;

   if (overloads.first == overloads.second) {
      return result;
   }

   result.compatibility = CompatibilityType::NO_MATCHING_CALL;

   for (auto it = overloads.first; it != overloads.second; ++it) {
      auto& overload = it->second;
      std::vector<Type*> givenArgs;
      std::vector<Type*>& neededArgs = overload->getArgTypes();

      givenArgs.reserve(args.size());

      size_t i = 0;
      for (const auto& arg : args) {
         // lambda expression not yet evaluated
         if (arg == nullptr) {
            auto& needed = neededArgs[i];
            auto lambda = std::static_pointer_cast<LambdaExpr>(argValues[i].second);
            if (!checkLambdaCompatibility(lambda.get(), needed)) {
               continue;
            }

            givenArgs.push_back(needed->deepCopy());
         }
         else if (neededArgs.size() <= i || (!neededArgs.at(i)->isLvalue() && arg->isLvalue())) {
            givenArgs.push_back(arg->deepCopy()->toRvalue());
         }
         else {
            givenArgs.push_back(arg->deepCopy());
         }
         ++i;
      }

      auto order = util::orderArgs(argLabels, givenArgs, overload->getArgNames(), argValues, overload->getArgDefaults());
      auto res = util::func_call_compatible(givenArgs, neededArgs, generics, overload->getGenerics());

      result.expectedType = res.expectedType;
      result.foundType = res.foundType;
      result.incompArg = res.incomp_arg;
      result.orderedArgs = order;

      for (const auto& arg : givenArgs) {
         delete arg;
      }

      if (res.is_compatible) {
         result.compatibility = CompatibilityType::COMPATIBLE;
      }

      if (res.perfect_match) {
         result.func = overload.get();
         result.neededCasts = res.needed_casts;

         return result;
      }

      if (res.is_compatible && res.compat_score >= bestMatch) {
         result.func = overload.get();
         result.neededCasts = res.needed_casts;
      }
   }

   return result;
}

void TypeCheckVisitor::ApplyCasts(
   std::vector<pair<string, std::shared_ptr<Expression>>>& args,
   std::vector<Type*>& argTypes,
   unordered_map<size_t, pair<Type*, Type*>>& casts)
{
   for (auto& _cast : casts) {
      assert(args.size() > _cast.first && "Invalid cast index!");
      wrapImplicitCast(args.at(_cast.first).second, _cast.second.first, _cast.second.second);
   }
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckVisitor::return_(
   Type* ret_type,
   AstNode *cause)
{
   if (declaredReturnType != nullptr) {
      if (!ret_type->implicitlyCastableTo(declaredReturnType)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Returned value of type " + ret_type->toString() + " is not "
            "compatible with declared return type " + declaredReturnType->toString(), cause);
      }

      ++returned;
   }
   else {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'return' is only allowed in function bodies", cause);
   }
}

void TypeCheckVisitor::continue_() {
   auto current = this;
   while (current != nullptr) {
      current->continued = true;
      if (current->isContinueRoot) {
         break;
      }

      current = current->parent;
   }
}

void TypeCheckVisitor::break_() {
   auto current = this;
   while (current != nullptr) {
      current->broken = true;
      if (current->isBreakRoot) {
         break;
      }

      current = current->parent;
   }
}

namespace {

   ObjectType* isGeneric(std::vector<ObjectType*>& generics, Type*& obj) {
      if (!obj->isObject() || generics.empty()) {
         return nullptr;
      }

      auto index = std::find_if(generics.begin(), generics.end(), [obj](ObjectType* gen) {
         return gen->getGenericClassName() == obj->getClassName();
      });

      if (index != generics.end()) {
         return *index;
      }

      return nullptr;
   }

}

void TypeCheckVisitor::resolve(Type** ty) {

   // resolve unqalified generic types, for example:
   //   let x: Array<Int> will be parsed as having one generic Type Int,
   //   but the parser doesn't know that this corresponds to the generic
   //   parameter "T" of class Array
   Type::resolveUnqualified(*ty);

   if (isa<ObjectType>(*ty) && (*ty)->getClassName() == "Self" && !currentClass.empty()) {
      *ty = ObjectType::get(currentClass);
      (*ty)->isGeneric(true);
      (*ty)->setGenericClassName("Self");
      (*ty)->setContravariance(ObjectType::get(currentClass));
   }

   if (currentClassGenerics != nullptr) {
      if (auto gen = isGeneric(*currentClassGenerics, *ty)) {
         auto backup = *ty;
         *ty = gen;
         Type::CopyProperties(backup, *ty);
      }
   }

   SymbolTable::resolveTypedef(*ty, importedNamespaces);

   for (const auto& cont : (*ty)->getTypeReferences()) {
      resolve(cont);
   }

   if ((*ty)->getLengthExpr() != nullptr) {
      auto &lengthExpr = (*ty)->getLengthExpr();
   }

   if (isa<ObjectType>(*ty)) {
      auto asObj = cast<ObjectType>(*ty);
      if (!SymbolTable::hasClass(asObj->getClassName())) {
         return;
      }

      auto cl = SymbolTable::getClass(asObj->getClassName());
      if (cl->isStruct()) {
         asObj->isStruct(true);
      }
      else if (cl->isEnum()) {
         asObj->isEnum(true);
      }
   }
}

void TypeCheckVisitor::checkExistance(ObjectType *objTy, AstNode* cause) {
   if (!SymbolTable::hasClass(objTy->getClassName())) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Class " + objTy->getClassName() + " does not exist", cause);
   }
}

void TypeCheckVisitor::pushNamespace(string &ns) {
   auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");

   SymbolTable::declareNamespace(newNs);
}

void TypeCheckVisitor::popNamespace() {
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

Type* TypeCheckVisitor::ReturnMemberExpr(Expression *node, Type *ty) {

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

void TypeCheckVisitor::wrapImplicitCast(Expression::SharedPtr& target, Type*& originTy, Type* destTy) {
   auto cast = new ImplicitCastExpr(originTy->deepCopy(), destTy->deepCopy(), target);

   cast->setIndex(target->startIndex, target->endIndex, target->sourceFileId);
   target->setParent(cast);
   cast->children.push_back(&target);

   target.reset(cast);
}

void TypeCheckVisitor::lvalueToRvalue(
   std::shared_ptr<Expression> &target)
{
   auto copy = target;
   auto ltor = new LvalueToRvalue(copy);

   target.reset(ltor);
   CopyNodeProperties(copy.get(), target.get());
}

void TypeCheckVisitor::toRvalueIfNecessary(
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

bool TypeCheckVisitor::castGenericIfNecessary(
   Expression *node,
   unordered_map<string, Type *> &concreteGenerics,
   Type *&ty,
   bool preCond)
{
   if (!preCond || !ty->isGeneric()) {
      return false;
   }

   auto fromTy = ty->deepCopy();
   Type::resolveGeneric(&ty, concreteGenerics);

   auto toTy = ty->deepCopy();
   toTy->isLvalue(fromTy->isLvalue());

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
Type* TypeCheckVisitor::visit(NamespaceDecl *node) {
   pushNamespace(node->nsName);
   node->contents->accept(*this);
   popNamespace();

   return nullptr;
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CompoundStmt *node) {
   auto prevUnsafe = currentBlockUnsafe;
   currentBlockUnsafe = node->isUnsafe_ || prevUnsafe;

   if (isNewlyCreated || node->preserveScope) {
      isNewlyCreated = false;
      for (const auto& child : node->get_children()) {
         child->accept(*this);
      }
   }
   else {
      TypeCheckVisitor t(this);
      for (const auto& child : node->get_children()) {
         child->accept(t);
      }
   }

   currentBlockUnsafe = prevUnsafe;
   return nullptr;
}

namespace {

   bool isReservedIdentifier(string& ident) {
      return (
         ident == "_"   ||
         ident == "Self"
      );
   }

   template <class T>
   std::vector<T*> copyTypeVector(std::vector<T*> vec) {
      std::vector<T*> copyVec;
      for (const auto& ty : vec) {
         copyVec.push_back(cast<T>(ty->deepCopy()));
      }

      return copyVec;
   }
}

void TypeCheckVisitor::DeclareFunction(FunctionDecl *node) {
   if (SymbolTable::hasClass(node->funcName)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare a function with the same name as a class declaration",
         node);
   }
   if (isReservedIdentifier(node->funcName)) {
      RuntimeError::raise(ERR_TYPE_ERROR, node->funcName + " is a reserved identifier", node);
   }

   node->returnType->accept(*this);
   auto& return_type = node->returnType->getType();

   auto qualified_name = ns_prefix() + node->funcName;
   if (qualified_name == "main") {
      if (!isa<IntegerType>(return_type) && !isa<VoidType>(return_type)) {
         Warning::issue("Declared return type of main function is always ignored", node);
      }

      return_type = IntegerType::get(64);
      node->returnType->setType(return_type->deepCopy());
   }

   Function::UniquePtr fun = std::make_unique<Function>(qualified_name, return_type, node->generics);
   node->declaredFunction = fun.get();

   if (return_type->isStruct()) {
      node->hasHiddenParam = true;
      fun->hasHiddenParam(true);
   }

   std::vector<Type*> argTypes;
   for (const auto &arg : node->args) {
      if (isReservedIdentifier(arg->argName)) {
         RuntimeError::raise(ERR_TYPE_ERROR, arg->argName + " is a reserved identifier", arg.get());
      }

      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();
      argTypes.push_back(resolvedArg);

      fun->addArgument(resolvedArg, arg->defaultVal, arg->argName);
   }

   declare_fun(std::move(fun), node->generics, node);

   if (qualified_name == "main") {
      node->binding = qualified_name;
   }
   else {
      node->binding = SymbolTable::mangleFunction(qualified_name, argTypes);
   }

   node->declaredFunction->setMangledName(node->binding);
}

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FunctionDecl *node) {

   auto& return_type = node->returnType->getType();
   auto funcVisitor = makeFunctionVisitor(return_type);

   for (const auto& arg : node->args) {
      arg->visitDefault = true;
      arg->accept(*this);

      arg->binding = funcVisitor.declare_var(arg->argName, arg->argType->getType());
   }

   attributes = node->attributes;
   node->body->accept(funcVisitor);

   attributes.clear();

   if (funcVisitor.branches - funcVisitor.returned > 0 && !return_type->isNullable() &&
      !isa<VoidType>(return_type) && node->funcName != "main")
   {
      RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
   }
   // implicit 0 return for main function
   else if (funcVisitor.branches - funcVisitor.returned > 0 && node->funcName == "main") {
      return_type = IntegerType::get();
      node->body->implicitZeroReturn = true;
   }

   return nullptr;
}

void TypeCheckVisitor::CopyNodeProperties(Expression *src, Expression *dst) {
   dst->isLhsOfAssigment_ = src->isLhsOfAssigment_;
   dst->setIndex(src->startIndex, src->endIndex, src->sourceFileId);

   if (src->isHiddenReturnValue_) {
      dst->isHiddenReturnValue();
   }
   else if (src->isReturnValue_) {
      dst->isReturnValue();
   }
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(IdentifierRefExpr *node) {

   if (node->isLetExpr_ || node->isVarExpr_) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Unexpected 'let' / 'var' expression", node);
   }

   string ns_name = node->ident;

   for (const auto& ns : importedNamespaces) {
      auto curr = ns + ns_name;
      if (SymbolTable::isNamespace(curr)) {
         ns_name = curr;
         break;
      }
   }

   if (SymbolTable::isNamespace(ns_name)) {
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

      if (node->memberExpr == nullptr) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot use a namespace as a value", node);
      }

      if (node->memberExpr->get_type() == NodeType::MEMBER_EXPR) {
         auto member_expr = std::static_pointer_cast<MemberRefExpr>(node->memberExpr);
         member_expr->isNsMember = true;
         member_expr->className = ns_name;
         member_expr->ident = member_expr->ident;
      }
      else if (node->memberExpr->get_type() == NodeType::CALL_EXPR) {
         auto member_expr = std::static_pointer_cast<CallExpr>(node->memberExpr);
         member_expr->isNsMember = true;
         member_expr->className = ns_name;
         member_expr->ident = member_expr->ident;
         member_expr->type = CallType::METHOD_CALL;
      }
      else if (node->memberExpr->get_type() == NodeType::ARRAY_ACCESS_EXPR) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index on a namespace", node);
      }
      else {
         assert(false && "Unknown operation");
      }

      node->memberExpr->contextualType = node->contextualType;

      return node->memberExpr->accept(*this);
   }

   if (node->ident == "super") {
      if (currentClass.empty()) {
         RuntimeError::raise(ERR_CONTEXT_ERROR, "'super' can only be used in instance methods", node);
      }

      auto currentCl = SymbolTable::getClass(currentClass);
      if (currentCl->getParent() == nullptr) {
         RuntimeError::raise(ERR_CONTEXT_ERROR, "Class " + currentClass + " does not have a base class", node);
      }

      node->binding = currentSelf;
      node->ident = "self";
      node->isSuper = true;
      node->superClassName = currentCl->getParent()->getName();

      auto type = ObjectType::get(node->superClassName)->toRvalue();
      return ReturnMemberExpr(node, type);
   }

   // try a function instead
   if (!has_var(node->ident)) {
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

         auto argTypes = copyTypeVector(result->getArgTypes());
         auto funcTy = new FunctionType(result->getReturnType()->deepCopy(), argTypes);
         funcTy->setFunction(result);

         return ReturnMemberExpr(node, funcTy);
      }

      for (auto it = overloads.first; it != overloads.second; ++it) {
         auto& argTypes = it->second->getArgTypes();
         if (argTypes.size() != args.size()) {
            continue;
         }

         size_t i = 0;
         bool matches = true;
         for (const auto& arg : argTypes) {
            if (!args.at(i)->implicitlyCastableTo(arg)) {
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

         auto argTypes = copyTypeVector(result->getArgTypes());
         auto funcTy = new FunctionType(result->getReturnType()->deepCopy(), argTypes);
         funcTy->setFunction(result);

         return ReturnMemberExpr(node, funcTy);
      }
   }

   bool implicit_this = node->ident != "self" && !has_var(node->ident) && !currentClass.empty();
   if (node->ident == "self" || implicit_this) {
      if (currentClass.empty()) {
         RuntimeError::raise(ERR_CONTEXT_ERROR, "'this' can only be used in instance methods", node);
      }

      if (implicit_this) {
         auto mem_ref = std::make_shared<MemberRefExpr>(node->ident);
         CopyNodeProperties(node, mem_ref.get());
         mem_ref->parent = node;

         mem_ref->setMemberExpr(node->memberExpr);
         node->memberExpr = mem_ref;
      }

      node->binding = currentSelf;
      node->ident = "self";

      //TODO check if 'self' is mutable
      auto type = SymbolTable::getClass(currentClass)->getType()->toRvalue();
      return ReturnMemberExpr(node, type);
   }

   auto res = get_var(node->ident, node);

   // add capture for this variable
   if (isLambdaVisitor && res.second) {
      capturedVariables->emplace_back(res.first.second, res.first.first);
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
Type* TypeCheckVisitor::visit(DeclStmt *node) {
   if (node->declared) {
      return nullptr;
   }

   auto& ident = node->identifier;

   if (isReservedIdentifier(ident)) {
      RuntimeError::raise(ERR_TYPE_ERROR, ident + " is a reserved identifier", node);
   }

   node->type->accept(*this);
   auto& declType = node->type->getType();
   auto& val = node->value;

   if (declType->isUnsafePointer() && !currentBlockUnsafe) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Raw Pointer types and C-Style arrays are only allowed in 'unsafe' "
         "blocks", node);
   }

   if (val != nullptr) {

      val->isGlobal(node->is_global);
      if (!isa<AutoType>(declType)) {
         val->setContextualType(declType);
      }

      pushTy(declType);

      Type* givenType = val->accept(*this);
      toRvalueIfNecessary(givenType, node->value, !declType->isLvalue());

      // in case it has changed
      declType = node->type->getType();

      if (declType->isInferred()) {
         if (givenType->isNull()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Null requires a contextual type", val.get());
         }

         delete declType;
         declType = givenType->deepCopy();
      }
      else if (isa<VoidType>(givenType) && !declType->isNullable()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign null to non-nullable variable of type " +
            declType->toString(), val.get());
      }
      else if (!givenType->implicitlyCastableTo(declType)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + givenType->toString() + " and " +
            declType->toString(), val.get());
      }

      if (*declType != givenType) {
         wrapImplicitCast(node->value, givenType, declType);
      }
   }
   else if (!declType->hasDefaultValue()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Expected non-nullable variable " + ident + " to be defined",
         node);
   }

   resolve(&declType);

   if (declType->getLengthExpr() != nullptr) {
      declType->getLengthExpr()->setContextualType(IntegerType::ConstInt64);

      Type* ty = declType->visitLengthExpr(this);
      toRvalueIfNecessary(ty, declType->getLengthExpr());

      if (!ty->implicitlyCastableTo(IntegerType::ConstInt64)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Array length has to be integral", declType->getLengthExpr().get());
      }
      else if (*IntegerType::ConstInt64 != ty) {
         wrapImplicitCast(declType->getLengthExpr(), ty, IntegerType::get(64));
      }
   }

   auto allocType = declType;

   if (declType->isStruct() || declType->isFunctionTy()) {
      node->isStructAlloca = true;
   }

   allocType->isLvalue(true);
   allocType->isConst(node->is_const);

   node->binding = declare_var(ident, allocType, node->is_global, node);
   declarations.emplace(node->binding, node);

   node->declared = true;
   return nullptr;
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ForStmt *node) {
   TypeCheckVisitor init_visitor(this);
   if (node->initialization) {
      node->initialization->accept(init_visitor);
   }

   if (node->increment) {
      node->increment->accept(init_visitor);
   }

   if (node->termination) {
      node->termination->accept(init_visitor);
   }

   if (node->body) {
      TypeCheckVisitor body_visitor(&init_visitor);
      body_visitor.continuable = true;
      body_visitor.breakable = true;

      node->body->accept(body_visitor);
   }

   return nullptr;
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(WhileStmt *node) {
   node->condition->accept(*this);
   TypeCheckVisitor body_visitor(this);
   body_visitor.continuable = true;
   body_visitor.breakable = true;

   node->body->accept(body_visitor);

   return nullptr;
}

pair<Type*, std::vector<Type*>> TypeCheckVisitor::unify(std::vector<Expression::SharedPtr>& types) {
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

/**
 * Checks an array literal
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(CollectionLiteral *node) {

   auto isInferred = node->contextualType != nullptr;
   if (!isInferred || node->contextualType->isInferred()) {
      Type* elTy;
      if (node->elements.empty()) {
         elTy = ObjectType::get("Any")->getPointerTo();
      }
      else {
         elTy = unify(node->elements).first;
         for (auto& el : node->elements) {
            toRvalueIfNecessary(elTy, el);
         }
      }

      if (node->hasAttribute(Attr::CArray)) {
         elTy->isCStyleArray(true);
         elTy->setLength(0);

         node->type = std::make_shared<TypeRef>(elTy);
         node->type->accept(*this);
      }
      else {
         node->type = std::make_shared<TypeRef>((new CollectionType(elTy)));
         node->type->accept(*this);
      }

      return ReturnMemberExpr(node, node->type->getType());
   }

   if (!node->contextualType->isPointerTy() && !isa<CollectionType>(node->contextualType))
   {
      RuntimeError::raise(ERR_TYPE_ERROR, "Collection literal cannot return value of type " +
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
      elType = ptrType->getConcreteGeneric("T");
   }

   if (!node->elements.empty()) {
      auto givenType = unify(node->elements).first;
      for (auto& el : node->elements) {
         toRvalueIfNecessary(givenType, el);
      }

      if (!givenType->implicitlyCastableTo(elType)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible collection elements: Expected " + elType->toString() +
            ", but got " + givenType->toString(), node);
      }
      else if (givenType->isBoxedEquivOf(elType)) {
         elType = givenType;
      }
      else if (*elType != givenType) {
         for (auto &el : node->elements) {
            wrapImplicitCast(el, givenType, elType);
         }
      }
   }

   auto ty = elType->getPointerTo();
   ty->isCStyleArray(isa<PointerType>(node->contextualType));
   ty->setLength(0);

   if (isCarray) {
      node->type = std::make_shared<TypeRef>(ty);
   }
   else {
      node->type = std::make_shared<TypeRef>(node->contextualType->deepCopy());
   }

   node->type->accept(*this);
   if (node->declaration != nullptr) {
      node->declaration->type = node->type;
   }

   return ReturnMemberExpr(node, node->type->getType()->deepCopy());
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(NumericLiteral *node) {

   if ((node->memberExpr == nullptr && node->contextualType != nullptr && isa<PrimitiveType>(node->contextualType)) ||
      node->hasAttribute(Attr::Primitive))
   {
      node->isPrimitive = true;
      return node->type->deepCopy();
   }

   switch (node->type->getTypeID()) {
      case TypeID::IntegerTypeID: {
         auto asInt = cast<IntegerType>(node->type);
         string className = "Int";
         if (asInt->isUnsigned()) {
            className = "U" + className;
         }

         auto bitWidth = asInt->getBitwidth();
         if (bitWidth != (sizeof(int *) * 8)) {
            className += std::to_string(bitWidth);
         }

         if (className == "Int8") {
            className = "Char";
         }

         if (className == "Int1") {
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
      default:
         llvm_unreachable("Unknown literal type");
   }

   // TODO remove this
   if (!SymbolTable::hasClass(node->className)) {
      node->isPrimitive = true;
      return node->type->deepCopy();
   }

   auto ty = ObjectType::get(node->className)->toRvalue();
   return ReturnMemberExpr(node, ty);
}

Type* TypeCheckVisitor::visit(NoneLiteral *node) {
   if (node->contextualType == nullptr || isa<AutoType>(node->contextualType)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "'none' requires a contextual type", node);
   }
   if (!node->contextualType->isOptionTy()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "'none' can only be assigned to option types", node);
   }

   return node->contextualType->deepCopy();
}

Type* TypeCheckVisitor::visit(StringLiteral *node) {

   for (const auto& attr : node->attributes) {
      switch (attr.kind) {
         case Attr::CString:
            node->raw = true;
            break;
         default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr.name + " is not applicable on string literals",
               node);
      }
   }

   if (node->contextualType != nullptr && isa<PointerType>(node->contextualType)) {
      node->raw = true;
   }

   if (node->raw) {
      auto ty = new PointerType(IntegerType::get(8));
      ty->setLength(node->value.length());
      ty->isCStyleArray(true);

      return ty;
   }

   auto str = ObjectType::get("String")->toRvalue();
   return ReturnMemberExpr(node, str);
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(SubscriptExpr *node) {

   auto ts = popTy()->deepCopy();
   resolve(&ts);

   if (!isa<ObjectType>(ts)) {
      node->_index->setContextualType(IntegerType::ConstInt64);
   }

   Type* index = node->_index->accept(*this);
   if (isa<ObjectType>(ts) && !ts->isCStyleArray()) {

      auto& className = ts->getClassName();
      
      auto cl = SymbolTable::getClass(className, importedNamespaces);
      std::vector<Type*> argTypes{ index };
      string op = "postfix []";
      cdot::cl::MethodResult methodResult = cl->hasMethod(op, argTypes);

      if (methodResult.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{ node->_index },
            op
         );

         ApplyCasts(call->args, argTypes, methodResult.neededCasts);

         call->setIndex(node->startIndex, node->endIndex, node->sourceFileId);
         call->parentExpr = node;
         call->parent = node;
         call->argTypes.push_back(index);

         auto expr = std::static_pointer_cast<Expression>(call);
         node->children.push_back(&expr);

         node->overridenCall = call;
         node->isSubscriptOp = true;

         pushTy(ts);
         auto type = call->accept(*this);

         return ReturnMemberExpr(node, type);
      }
      else {
         RuntimeError::raise(ERR_TYPE_ERROR, "Operator [](" + index->toString() + ") is not defined on class " +
            ts->toString(), node);
      }
   }

   if (ts->isPointerTy() && !ts->isCStyleArray()) {
      node->isPointerShift = true;
   }
   else if (!ts->isCStyleArray()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Operator [](" + index->toString() + ") is not defined on type " +
         ts->toString(), node);
   }

   toRvalueIfNecessary(index, node->_index);

   Type* int64Ty = IntegerType::get(64);
   if (!index->implicitlyCastableTo(int64Ty)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Array indices have to be integral, " + index->toString() + " given", node);
   }
   else if (*index != int64Ty) {
      wrapImplicitCast(node->_index, index, int64Ty);
   }
   else {
      delete int64Ty;
   }

   ts->isLvalue(true);
   ts->isCStyleArray(false);

   return ReturnMemberExpr(node, ts);
}

void TypeCheckVisitor::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>> &args,
   std::vector<Type*> &declaredArgTypes,
   std::vector<Type*> &argTypes
) {
   size_t i = 0;
   for (auto& arg : args) {
      if (i >= argTypes.size()) {
         break;
      }
      if (i >= declaredArgTypes.size()) {
         // assume that c style varargs do not require an lvalue
         toRvalueIfNecessary(argTypes.at(i), arg.second);

         continue;
      }

      if (argTypes.at(i) == nullptr) {
         continue;
      }

      if (args[i].second->get_type() == NodeType::LAMBDA_EXPR) {
         args[i].second->setContextualType(declaredArgTypes[i]);
         args[i].second->accept(*this);
      }

      if (declaredArgTypes.at(i)->isStruct() && argTypes.at(i)->isStruct() && !declaredArgTypes.at(i)->isLvalue()) {
         arg.second->needsByValPass = true;
      }

      if (!declaredArgTypes.at(i)->isLvalue() && argTypes.at(i)->isLvalue() && !argTypes.at(i)->isStruct()) {
         lvalueToRvalue(arg.second);
      }

      ++i;
   }
}

void TypeCheckVisitor::PrepareCallArgs(
   std::vector<pair<string, std::shared_ptr<Expression>>> &args,
   std::vector<Type*> &declaredArgTypes,
   std::vector<Type*> &argTypes,
   std::vector<Expression::SharedPtr> &defaultValues
) {
   size_t i = 0;
   while (args.size() < declaredArgTypes.size()) {
      auto defVal = defaultValues[args.size() + i];
      if (defVal == nullptr) {
         break;
      }

      args.emplace_back("", defVal);
      argTypes.push_back(declaredArgTypes[i]);
      ++i;
   }

   PrepareCallArgs(args, declaredArgTypes, argTypes);
}

void TypeCheckVisitor::HandleFunctionCall(CallExpr *node)
{
   std::vector<string> argLabels;
   for (const auto& arg : node->args) {
      argLabels.push_back(arg.first);
   }

   auto& argTypes = node->argTypes;
   FunctionResult result = get_fun(node->ident, argTypes, node->generics, argLabels, node->args);

   if (result.compatibility != CompatibilityType::COMPATIBLE) {

      // possible implicit method call
      if (!currentClass.empty()) {
         auto cl = SymbolTable::getClass(currentClass);
         auto compat = cl->hasMethod(node->ident, argTypes);
         if (compat.compatibility == CompatibilityType::COMPATIBLE) {
            pushTy(ObjectType::get(currentClass));

            node->type = CallType::METHOD_CALL;
            node->implicitSelfCall = true;
            node->selfBinding = currentSelf;

            return HandleMethodCall(node);
         }
      }

      // lambda or saved function call
      if (has_var(node->ident)) {
         auto var = get_var(node->ident);
         auto fun = var.first.first->deepCopy();

         if (fun->isLvalue()) {
            fun = fun->toRvalue();
         }

         if (isa<FunctionType>(fun)) {
            pushTy(fun);
            node->type = CallType::ANON_CALL;
            node->binding = var.first.second;

            if (isLambdaVisitor && var.second) {
               capturedVariables->emplace_back(var.first.second, var.first.first);
               node->isCapturedVar = true;
               node->capturedType = var.first.first;
            }

            return HandleAnonCall(node);
         }

         delete fun;
      }

      if (result.compatibility == CompatibilityType::NO_MATCHING_CALL) {
         Expression* cause = node;
         if (node->args.size() > result.incompArg) {
            cause = node->args[result.incompArg].second.get();
         }

         if (!result.foundType.empty() && !result.expectedType.empty()) {
            RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
               "No matching call for function " + node->ident + " found: Candidate function not viable: No known "
                  "conversion from " + result.foundType + " to " + result.expectedType + " ",
               cause
            );
         }
         else {
            RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
               "No matching call for function " + node->ident + " found",
               cause
            );
         }
      }
      if (result.compatibility == CompatibilityType::FUNC_NOT_FOUND) {
         RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
            "Function " + node->ident + " does not exist", node);
      }

      llvm_unreachable("No other options possible");
   }

   node->args = result.orderedArgs;

   auto& func = result.func;
   auto& declaredArgTypes = func->getArgTypes();
   auto& defaultValues = func->getArgDefaults();

   PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
   ApplyCasts(node->args, argTypes, result.neededCasts);

   node->binding = SymbolTable::mangleFunction(func->getName(), func->getArgTypes());

   if (func->hasHiddenParam()) {
      node->hasHiddenParamReturn = true;
      node->hiddenParamType = func->getReturnType();
   }

   auto& retTy = func->getReturnType();

   if (retTy->isGeneric()) {
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
      node->genericDestTy = returnTy;
      if (returnTy->isStruct() || (returnTy->isLvalue() && returnTy->isPointerToStruct())) {
         node->genericDestTy = node->genericDestTy->getPointerTo();
      }
   }
   else {
      node->returnType = func->getReturnType()->deepCopy();
   }
}

void TypeCheckVisitor::HandleMethodCall(
   CallExpr *node)
{
   Class* cl;
   string className;
   Type* latest;

   if (node->parentExpr == nullptr && node->isEnumCase) {
      auto& inf = node->contextualType;

      if (node->contextualType == nullptr || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      node->isNsMember = true;

      goto enum_case;
   }

   if (node->isNsMember) {
      if (!SymbolTable::hasClass(node->className)) {
         node->type = CallType::FUNC_CALL;
         node->ident = node->className + "." + node->ident;

         return HandleFunctionCall(node);
      }

      enum_case:
      cl = SymbolTable::getClass(node->className);
      className = node->className;

      // enum case
      if (cl->isEnum()) {
         auto en = static_cast<Enum*>(cl);
         auto res = en->hasCase(node->ident, node->argTypes, node->generics);
         if (res.compatibility != CompatibilityType::COMPATIBLE) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Enum " + node->className + " does not define case " + node->ident +
               util::args_to_string(node->argTypes), node);
         }

         auto& case_ = en->getCase(node->ident);
         std::vector<Type*> neededTypes;
         for (const auto& ty : case_.associatedValues) {
            neededTypes.push_back(ty.second);
         }

         PrepareCallArgs(node->args, neededTypes, node->argTypes);
         ApplyCasts(node->args, node->argTypes, res.neededCasts);

         node->isEnumCase = true;
         node->caseVal = case_.rawValue;

         auto obj = ObjectType::get(node->className);
         obj->isEnum(true);
         obj->setKnownEnumCase(&en->getCase(node->ident), node->args, node->argTypes);

         std::vector<Type*> generics;
         for (const auto& gen : node->generics) {
            generics.push_back(gen->deepCopy());
         }

         obj->setUnqualGenerics(generics);
         Type::resolveUnqualified(obj);
         node->returnType = obj;
         return;
      }
   }
   else {
      assert(!typeStack.empty() && "Nothing to call method on!");

      latest = popTy();
      if (!latest->isObject()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call method on value of type " + latest->toString(), node);
      }

      className = latest->getClassName();
      if (!SymbolTable::hasClass(className)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Class " + latest->toString() + " does not exist", node);
      }

      cl = SymbolTable::getClass(className, importedNamespaces);
      node->className = className;
      if (cl->isProtocol()) {
         node->castFrom = latest;
         node->reverseProtoShift = true;
      }
   }

   auto &argTypes = node->argTypes;
   auto methodResult = cl->hasMethod(node->ident, argTypes, node->generics);
   if (methodResult.compatibility != CompatibilityType::COMPATIBLE) {
      Expression *cause = node;
      if (node->args.size() > methodResult.incompArg) {
         cause = node->args[methodResult.incompArg].second.get();
      }

      if (!methodResult.foundType.empty() && !methodResult.expectedType.empty()) {
         RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
            "No matching call for method " + node->ident + " found: Candidate function not viable: No known "
               "conversion from " + methodResult.foundType + " to " + methodResult.expectedType + " ",
            cause
         );
      } else {
         RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->ident + " does not exist on class " +
            className, cause);
      }
   }

   auto method = cl->getMethod(methodResult.method->mangledName);

   // check if method is accessible from current context
   if (method->accessModifier == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass))
   {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->methodName + " of class " +
         className + " is not accessible", node);
   }
   else if (method->accessModifier == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass))
   {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->methodName + " of class " +
         className + " is not accessible", node);
   }

   unordered_map<string, Type*> concreteGenerics;
   if (!node->generics.empty()) {
      concreteGenerics = Type::resolveUnqualified(node->generics, method->generics);
   }
   else {
      concreteGenerics = latest->getConcreteGenericTypes();
   }

   auto ty = method->returnType->deepCopy();
   auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass != cl->getName());

   node->returnType = ty;
   node->lvalueCast = isGeneric;

   // returning an object with non-resolved generics
   if (method->returnType->isObject()) {
      auto returnCl = SymbolTable::getClass(method->returnType->getClassName());
      if (returnCl->isGeneric()) {
         Type::resolveGeneric(&node->returnType, latest->getConcreteGenericTypes());
      }
   }

   // methods with 'ref' return type
   node->returnType->isLvalue(method->returnType->isLvalue());
   node->binding = method->mangledName;
   node->method = method;
   node->is_virtual = cl->isAbstract() || cl->isVirtual(method);

   auto& declaredArgTypes = method->argumentTypes;
   auto& defaultValues = method->argumentDefaults;

   PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
   ApplyCasts(node->args, argTypes, methodResult.neededCasts);

   // check if this method has a hidden byval struct parameter
   if (method->hasHiddenParam) {
      node->hasHiddenParamReturn = true;
      node->hiddenParamType = method->returnType;
   }
}

void TypeCheckVisitor::HandleConstructorCall(CallExpr *node) {

   Class* cl = SymbolTable::getClass(node->ident, importedNamespaces);
   auto& argTypes = node->argTypes;
   auto constrResult = cl->hasMethod("init", argTypes, node->generics);

   if (constrResult.compatibility != CompatibilityType::COMPATIBLE) {
      Expression* cause = node;
      if (node->args.size() > constrResult.incompArg) {
         cause = node->args[constrResult.incompArg].second.get();
      }

      if (node->generics.size() != cl->getGenerics().size()) {
         auto missingGeneric = cl->getGenerics().at(node->generics.size());

         RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer generic type " +
            missingGeneric->getGenericClassName() + " of class " + node->ident +  " from context", cause
         );
      }

      if (!constrResult.foundType.empty() && !constrResult.expectedType.empty()) {
         RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
            "No matching call for constructor " + node->ident + " found: Candidate function not viable: No known "
               "conversion from " + constrResult.foundType + " to " + constrResult.expectedType + " ",
            cause
         );
      }
      else {
         RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->ident + " does not define a constructor with "
            "given signature", cause);
      }
   }

   auto& method = constrResult.method;

   // check accessibility
   if (method->accessModifier == AccessModifier::PROTECTED &&
      !cl->protectedPropAccessibleFrom(currentClass))
   {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->methodName + " of class " +
         node->ident + " is not accessible", node);
   }
   else if (method->accessModifier == AccessModifier::PRIVATE &&
      !cl->privatePropAccessibleFrom(currentClass))
   {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->methodName + " of class " +
         node->ident + " is not accessible", node);
   }

   if (cl->isAbstract()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot instantiate abstract class " + node->ident, node);
   }

   auto concreteClassGenerics = cl->getConcreteGenerics();
   ObjectType* returnType;

   // specify supplied generics for this instance
   if (cl->isGeneric()) {
      auto& cl_gen = cl->getGenerics();
      returnType = ObjectType::get(node->ident);

      size_t i = 0;
      for (const auto& gen : cl_gen) {
         returnType->specifyGenericType(gen->getGenericClassName(), node->generics.at(i++)->deepCopy());
      }

      returnType = cast<ObjectType>(returnType->toRvalue());
   }
   else {
      returnType = cast<ObjectType>(method->returnType->deepCopy());
   }

   returnType->getConcreteGenericTypes().insert(concreteClassGenerics.begin(), concreteClassGenerics.end());
   node->returnType = returnType;

   node->type = CallType::CONSTR_CALL;
   node->binding = method->mangledName;

   auto& declaredArgTypes = method->argumentTypes;
   auto& defaultValues = method->argumentDefaults;

   PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
   ApplyCasts(node->args, argTypes, constrResult.neededCasts);
}

void TypeCheckVisitor::HandleCallOperator(CallExpr *node) {

   auto latest = popTy();
   if (!isa<ObjectType>(latest) || !SymbolTable::hasClass(latest->getClassName())) {
      return HandleAnonCall(node);
   }

   auto& className = latest->getClassName();
   auto cl = SymbolTable::getClass(className, importedNamespaces);

   auto& argTypes = node->argTypes;
   auto callOpResult = cl->hasMethod("postfix ()", argTypes);

   if (callOpResult.compatibility != CompatibilityType::COMPATIBLE) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Call operator with signature " + util::args_to_string(argTypes) + " does"
         " not exist on class " + className, node);
   }

   auto& method = callOpResult.method;

   node->type = CallType::METHOD_CALL;
   node->ident = "postfix ()";
   node->binding = method->mangledName;

   auto& declaredArgTypes = method->argumentTypes;
   auto& defaultValues = method->argumentDefaults;

   PrepareCallArgs(node->args, declaredArgTypes, argTypes, defaultValues);
   ApplyCasts(node->args, argTypes, callOpResult.neededCasts);

   node->returnType = method->returnType;
}

void TypeCheckVisitor::HandleAnonCall(CallExpr *node) {

   auto latest = popTy();
   auto& argTypes = node->argTypes;

   if (isa<PointerType>(latest)) {
      latest = cast<PointerType>(latest)->getPointeeType();
   }

   if (!isa<FunctionType>(latest)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Given object of type " + latest->toString() + " is not callable", node);
   }

   std::vector<string> argLabels;
   for (const auto& arg : node->args) {
      argLabels.push_back(arg.first);
   }

   auto func = cast<FunctionType>(latest);

   if (func->getFunction() != nullptr) {
      auto ordered = util::orderArgs(argLabels, argTypes, func->getFunction()->getArgNames(), node->args,
         func->getFunction()->getArgDefaults());

      node->args = ordered;
   }

   auto result = util::func_call_compatible(argTypes, func->getArgTypes());
   if (!result.is_compatible) {
      Expression* cause = node;
      if (node->args.size() > result.incomp_arg) {
         cause = node->args[result.incomp_arg].second.get();
      }

      if (!result.foundType.empty() && !result.expectedType.empty()) {
         RuntimeError::raise(ERR_UNDECLARED_VARIABLE,
            "No matching call for function " + node->ident + " found: No known "
               "conversion from " + result.foundType + " to " + result.expectedType + " ",
            cause
         );
      }
      else {
         RuntimeError::raise(ERR_TYPE_ERROR, "No matching call for function " + node->ident + " found", cause);
      }
   }

   node->type = CallType::ANON_CALL;
   auto asFun = cast<FunctionType>(latest);

   auto& declaredArgTypes = asFun->getArgTypes();
   PrepareCallArgs(node->args, declaredArgTypes, argTypes);

   node->returnType = asFun->getReturnType();
   node->functionType = asFun;
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(
   CallExpr *node)
{
   //MAINCALL
   auto& givenArgs = node->argTypes;
   givenArgs.reserve(node->args.size());

   for (size_t i = givenArgs.size(); i < node->args.size(); ++i) {
      const auto& arg = node->args.at(i);
      if (arg.second->get_type() == NodeType::LAMBDA_EXPR) {
         givenArgs.push_back(nullptr);
      }
      else {
         givenArgs.push_back(arg.second->accept(*this));
      }
   }

   if (node->type == CallType::FUNC_CALL) {
      if (node->ident.empty()) {
         HandleCallOperator(node);
      }
      else if (SymbolTable::hasClass(node->ident)) {
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

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(MemberRefExpr *node) {

   Type* latest;
   string className;
   Class* cl;

   if (node->parentExpr == nullptr && node->isEnumCase) {
      auto& inf = node->contextualType;

      if (node->contextualType == nullptr || !SymbolTable::hasClass(inf->getClassName())) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer type of enum case " + node->ident, node);
      }

      node->className = inf->getClassName();
      goto enum_case;
   }

   if (node->isNsMember) {
      if (!has_var(node->ident) && SymbolTable::hasClass(node->className)) {
         enum_case:
         cl = SymbolTable::getClass(node->className);

         if (cl->isEnum()) {
            auto en = static_cast<Enum*>(cl);

            auto genericParamsGiven = node->contextualType == nullptr ? 0
                 : node->contextualType->getConcreteGenericTypes().size();

            if (en->isGeneric() && genericParamsGiven < en->getGenerics().size()) {
               RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer generic type " +
                  en->getGenerics()[genericParamsGiven]->getGenericClassName(), node);
            }
            if (!en->hasCase(node->ident)) {
               RuntimeError::raise(ERR_TYPE_ERROR, "Enum " + node->className + " does not have a case '" +
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

         latest = cl->getType();
         node->ident = node->ident.substr(node->ident.find_last_of('.') + 1);
         goto implicit_method_call;
      }

      auto var = get_var(node->ident, node);
      node->binding = var.second;

      return var.first.first;
   }

   latest = popTy();
   className = latest->getClassName();

   if (node->isTupleAccess) {
      if (!latest->isTupleTy()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access indexed property on non-tuple value of type "
            + latest->toString(), node);
      }

      auto asTuple = cast<TupleType>(latest);
      if (asTuple->getArity() <= node->tupleIndex) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index " + std::to_string(node->tupleIndex) + " on tuple "
            "with arity " + std::to_string(asTuple->getArity()), node);
      }

      node->fieldType = asTuple->getContainedType(node->tupleIndex)->deepCopy();
      node->fieldType->isLvalue(true);
      node->fieldType->isConst(latest->isConst());

      return ReturnMemberExpr(node, node->fieldType->deepCopy());
   }

   if (className.empty()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access property " + node->ident + " on value of type "
         + latest->toString(), node);
   }

   cl = SymbolTable::getClass(className, importedNamespaces);
   node->className = className;

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
      if (currentClass == className && node->isLhsOfAssigment_ && uninitializedFields != nullptr) {
         auto index = std::find(uninitializedFields->begin(), uninitializedFields->end(), node->ident);
         if (index != uninitializedFields->end()) {
            uninitializedFields->erase(index);
            isUninitializedField = true;
         }
      }

      // Check if we're trying to assign a const field
      if (field->isConst && node->isLhsOfAssigment_ && !isUninitializedField) {
         RuntimeError::raise(
            ERR_TYPE_ERROR,
            "Field " + field->fieldName + " of " + cl->getTypeName() + " is constant",
            node
         );
      }

      // Use a getter if available
      if (field->hasGetter && !node->isLhsOfAssigment_ && currentClass != className) {
         auto call = std::make_shared<CallExpr>(
            CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{},
            field->getterName

         );
         call->setMemberExpr(node->memberExpr);
         call->setIndex(node->startIndex, node->endIndex, node->sourceFileId);
         call->setParent(node->parent);

         node->getterOrSetterCall = call;

         pushTy(latest);
         return call->accept(*this);
      }

      // Use a setter if available (and we're on the left side of an assignment)
      if (field->hasSetter && node->isLhsOfAssigment_ && currentClass != className) {
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
      auto isGeneric = castGenericIfNecessary(node, concreteGenerics, ty, currentClass != cl->getName());

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

      if (field->accessModifier == AccessModifier::PROTECTED && !cl->protectedPropAccessibleFrom(currentClass)) {
         RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected field " + field->fieldName + " of class " +
            className + " is not accessible", node);
      }
      else if (field->accessModifier == AccessModifier::PRIVATE && !cl->privatePropAccessibleFrom(currentClass)) {
         RuntimeError::raise(ERR_CONTEXT_ERROR, "Private field " + field->fieldName + " of class " +
            className + " is not accessible", node);
      }

      node->fieldType->isLvalue(true);
      node->fieldType->isConst(field->isConst && !isUninitializedField);

      return ReturnMemberExpr(node, node->fieldType->deepCopy());
   }
   // method call with omitted parens
   else {
      implicit_method_call:
      auto possibleMethod = cl->hasMethod(node->ident, {});
      if (possibleMethod.compatibility == CompatibilityType::COMPATIBLE) {
         Expression::SharedPtr call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
            std::vector<Expression::SharedPtr>{}, node->ident
         );

         CopyNodeProperties(node, call.get());
         pushTy(latest);

         node->parentExpr->memberExpr = call;
         return call->accept(*this);
      }
   }

   RuntimeError::raise(ERR_TYPE_ERROR, "Field " + node->ident + " does not exist on class " +
      latest->toString(), node);
   llvm_unreachable("");
}

Type* TypeCheckVisitor::HandleCastOp(Type *fst, BinaryOperator *node) {
   if (node->rhs->get_type() != NodeType::TYPE_REF) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Expected type name after 'as'", node->rhs.get());
   }

   auto toTypeRef = std::static_pointer_cast<TypeRef>(node->rhs);
   toTypeRef->accept(*this);

   auto& toType = toTypeRef->getType();
   node->operandType = fst;

   if (toType->isUnsafePointer() && !currentBlockUnsafe) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Raw Pointer types and C-Style arrays are only allowed inside unsafe "
         "blocks", node);
   }

   if (isa<ObjectType>(fst)) {
      auto& className = fst->getClassName();
      auto op = "infix as " + toType->toString();

      std::vector<Type*> argTypes;
      auto class_decl = SymbolTable::getClass(className, importedNamespaces);
      auto castOp = class_decl->hasMethod(op, argTypes);

      if (castOp.compatibility == CompatibilityType::COMPATIBLE) {
         auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{}, op);
         CopyNodeProperties(node, call.get());

         node->overridenCall = call;
         call->memberExpr = node->memberExpr;

         pushTy(fst);
         auto res = call->accept(*this);

         return res;
      }
   }

   toRvalueIfNecessary(fst, node->lhs);
   node->operandType = fst;

   if (node->op != "as!" && !fst->explicitlyCastableTo(toType)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast from " + fst->toString() + " to " +
         toType->toString(), node);
   }

   // check if cast returns an optional
   if (fst->isObject()) {
      auto fromClass = SymbolTable::getClass(fst->getClassName());
      if (fromClass->isBaseClassOf(toType->getClassName())) {
         SymbolTable::getClass(toType->getClassName())->needsTypeInfoGen(true);
         return ObjectType::getOptionOf(toType);
      }
   }

   return toType;
}

Type* TypeCheckVisitor::HandleAssignmentOp(Type *fst, Type *snd, BinaryOperator *node) {

   if (fst->isCStyleArray()) {
      //TODO check if sizes are comatible
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot reassign c-style array", node->lhs.get());
   }
   else if (!snd->implicitlyCastableTo(fst)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + snd->toString() + " to variable of "
         "type " + fst->toString(), node->lhs.get());
   }
   else if (*fst != snd || (snd->isCStyleArray() && fst->isPointerTy())) {
      Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node);
      wrapImplicitCast(node->rhs, snd, fst);
   }

   auto prevOp = node->op;
   auto op = util::isAssignmentOperator(prevOp);

   if (op != "=") {
      node->op = op;
      HandleBinaryOperator(fst, snd, cdot::getBinaryOpType(op), node);
      node->op = prevOp;
   }

   if (isa<VoidType>(snd)) {
      node->isNullAssignment = true;
   }

   node->operandType = fst;
   return new VoidType;
}

Type* TypeCheckVisitor::HandleArithmeticOp(Type *fst, Type *snd, BinaryOperator *node) {
   auto& op = node->op;

   if ((op == "+" || op == "-") && (!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd))) {
      if (!currentBlockUnsafe) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Pointer arithmetic is only allowed in unsafe blocks", node);
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
         RuntimeError::raise(ERR_TYPE_ERROR, "Right hand side of '**' must be of type integer",
            node->rhs.get());
      }

      Type* retType = FPType::getDoubleTy();

      if (*fst != retType) {
         wrapImplicitCast(node->lhs, fst, retType);
      }
      if (!snd->isInt64Ty()) {
         wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt64);
      }

      retType->isLvalue(false);
      node->operandType = retType;

      return retType;
   }

   err:
   RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
      ->toString() + " and " + snd->toString(), node);
   llvm_unreachable("");
}

Type* TypeCheckVisitor::HandleBitwiseOp(Type *fst, Type *snd, BinaryOperator *node) {
   auto& op = node->op;

   if (!fst->implicitlyCastableTo(IntegerType::ConstInt64)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
         ->toString() + " and " + snd->toString(), node);
   }
   if (!snd->implicitlyCastableTo(IntegerType::ConstInt64)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
         ->toString() + " and " + snd->toString(), node);
   }

   if (!fst->isInt64Ty()) {
      wrapImplicitCast(node->lhs, fst, IntegerType::ConstInt64);
   }
   if (!snd->isInt64Ty()) {
      wrapImplicitCast(node->rhs, snd, IntegerType::ConstInt64);
   }

   node->operandType = IntegerType::ConstInt64;

   return IntegerType::get(64);
}

Type* TypeCheckVisitor::HandleLogicalOp(Type *fst, Type *snd, BinaryOperator *node) {

   if (!fst->implicitlyCastableTo(IntegerType::ConstInt1) && !snd->implicitlyCastableTo(IntegerType::ConstInt1)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + node->op + "' to values of type " + fst
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

void TypeCheckVisitor::HandleEnumComp(
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

void TypeCheckVisitor::HandleTupleComp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   assert(fst->isTupleTy() && snd->isTupleTy() && "Can't compare tuple with non-tuple!");

   auto fstAsTuple = cast<TupleType>(fst);
   auto sndAsTuple = cast<TupleType>(snd);

   node->isTupleComp = true;
   node->arity = fstAsTuple->getArity();
   node->tupleTypes = fstAsTuple->getContainedTypes(false);
   node->llvmTupleType = fstAsTuple->getLlvmType();
}

Type* TypeCheckVisitor::HandleEqualityOp(
   Type *fst,
   Type *snd,
   BinaryOperator *node)
{
   // pointer comparison operators
   if (node->op.length() == 3) {
      if (!isa<PointerType>(fst) && !isa<PointerType>(snd) && !isa<ObjectType>(fst) && !isa<ObjectType>(snd)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Expected at least one operand of " + node->op + " to be a pointer",
            node);
      }

      if (!fst->explicitlyCastableTo(IntegerType::ConstInt64) ||
         !fst->explicitlyCastableTo(IntegerType::ConstInt64)
      ) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Binary operator " + node->op + " is not applicable to types " +
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

   if ((!isa<PrimitiveType>(fst) && !fst->isEnum()) && !fst->isTupleTy()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
         ->toString() + " and " + snd->toString() + " for equality", node);
   }

   if (!snd->implicitlyCastableTo(fst)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
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

   node->operandType = IntegerType::ConstInt1;
   return IntegerType::get(1);
}

Type* TypeCheckVisitor::HandleComparisonOp(Type *fst, Type *snd, BinaryOperator *node) {
   if (!isa<PrimitiveType>(fst) || !isa<PrimitiveType>(snd)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + node->op + "' to values of type " + fst
         ->toString() + " and " + snd->toString(), node);
   }

   if (*snd != fst) {
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->operandType = IntegerType::ConstInt1;
   return IntegerType::get(1);
}

Type* TypeCheckVisitor::HandleOtherOp(Type *fst, Type *snd, BinaryOperator *node) {
   auto& op = node->op;

   if (op == "..") {
      Type *intTy = IntegerType::get(64);
      if (!fst->implicitlyCastableTo(intTy)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
            ->toString() + " and " + snd->toString(), node);
      }
      if (!snd->implicitlyCastableTo(intTy)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
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

   RuntimeError::raise(ERR_TYPE_ERROR, "Binary operator " + node->op + " is not defined for arguments of type " +
      fst->toString() + " and " + snd->toString(), node);

   return nullptr;
}

Type* TypeCheckVisitor::HandleBinaryOperator(Type *lhs, Type *rhs, BinaryOperatorType opTy, BinaryOperator *node) {
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
      case BinaryOperatorType::OTHER:
      default:
         return HandleOtherOp(lhs, rhs, node);
   }
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(BinaryOperator *node) {
   //MAINBINARY
   auto opType = cdot::getBinaryOpType(node->op);
   node->opType = opType;

   auto isAssignment = opType == BinaryOperatorType::ASSIGNMENT;
   if (isAssignment) {
      node->lhs->isLhsOfAssigment();
   }

   if (util::is_reversible(node->op) && node->lhs->needsContextualInformation()
      && !node->rhs->needsContextualInformation())
   {
      auto _lhs = node->lhs;
      node->lhs = node->rhs;
      node->rhs = _lhs;
   }

   Type* fst = node->lhs->accept(*this);

   if (node->lhs->isSetterCall) {
      auto call = std::make_shared<CallExpr>(
         CallType::METHOD_CALL,
         std::vector<Expression::SharedPtr>{ node->rhs },
         node->lhs->setterName
      );

      node->overridenCall = call;

      CopyNodeProperties(node, call.get());
      call->setMemberExpr(node->memberExpr);
      call->setParent(node->parent);

      pushTy(fst);
      return call->accept(*this);
   }

   if (opType == BinaryOperatorType::CAST) {
      return HandleCastOp(fst, node);
   }

   bool is_overload = false;
   bool isPointerToStruct = false;
   string className;
   std::vector<Type*> overloadArgs;

   if (isAssignment) {
      if (fst->isConst() && !fst->isCarrayElement()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Trying to reassign constant", node->lhs.get());
      }
      else if (!fst->isLvalue()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign to rvalue of type " + fst->toString(),
            node->lhs.get());
      }

      // now that we know it's an lvalue, use the pointee type for compatibilty checks
      fst->isLvalue(false);
      node->isStructAssignment = fst->isStruct();
   }
   else {
      toRvalueIfNecessary(fst, node->lhs);
   }

   node->rhs->setContextualType(fst);
   Type* snd = node->rhs->accept(*this);

   if (!fst->getClassName().empty()) {
      className = fst->getClassName();
      overloadArgs.push_back(snd);
      is_overload = true;
   }
   else if (util::is_reversible(node->op) && !snd->getClassName().empty()) {
      className = snd->getClassName();
      overloadArgs.push_back(fst);
      is_overload = true;
   }

   if (is_overload && SymbolTable::hasClass(className)) {
      std::vector<Type*> argTypes{ snd };
      cdot::cl::Class* cl = SymbolTable::getClass(className, importedNamespaces);
      auto binOpResult = cl->hasMethod("infix " + node->op, argTypes, fst->getConcreteGenericTypes());

      if (binOpResult.compatibility == CompatibilityType::COMPATIBLE) {
         // custom operators need rvalue argument
         if (isAssignment) {
            lvalueToRvalue(node->lhs);
         }

         pushTy(fst);
         auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{
               node->rhs }, "infix " + node->op);

         ApplyCasts(call->args, argTypes, binOpResult.neededCasts);

         // we already resolved the argument, don't want to visit it again
         call->argTypes.push_back(snd);
         call->memberExpr = node->memberExpr;
         call->parentExpr = node;
         CopyNodeProperties(node, call.get());

         node->overridenCall = call;

         auto res = call->accept(*this);

         return res;
      }
   }

   toRvalueIfNecessary(snd, node->rhs);

   auto res = HandleBinaryOperator(fst, snd, opType, node);
   return ReturnMemberExpr(node, res);
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(TertiaryOperator *node) {
   Type* cond = node->condition->accept(*this);

   if (!cond->implicitlyCastableTo(IntegerType::get(1))) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Condition of tertiary operator '?:' must be boolean or implement "
         "instance method 'toBool() -> bool'", node);
   }

   Type* fst = node->lhs->accept(*this);
   Type* snd = node->rhs->accept(*this);

   if (!fst->implicitlyCastableTo(snd)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply tertiary operator '?:' to values of type " + fst->toString() +
         " and " + snd->toString(), node);
   }
   else if (*fst != snd) {
      Warning::issue("Implicit cast from " + snd->toString() + " to " + fst->toString(), node->rhs.get());
      wrapImplicitCast(node->rhs, snd, fst);
   }

   node->resultType = fst;

   return fst;
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(UnaryOperator *node) {

   string op = node->op;
   Type* target = node->target->accept(*this);

   if (target->isObject()) {
      auto& className = target->getClassName();

      std::vector<Type*> argTypes;
      auto class_decl = SymbolTable::getClass(className, importedNamespaces);
      auto unOpResult = class_decl->hasMethod((node->prefix ? "prefix " : "postfix ") + op, argTypes);

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
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " cannot be applied to rvalue of "
            "type " + target->toString(), node->target.get());
      }
      if (target->isConst()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " cannot be applied to 'let' constant",
            node->target.get());
      }

      auto pointee = target->toRvalue();
      if (!isa<PrimitiveType>(pointee)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target->toString(),
            node->target.get());
      }

      node->operandType = pointee;
      return ReturnMemberExpr(node, pointee);
   }

   if (op == "*") {
      if (!currentBlockUnsafe) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed inside unsafe blocks", node);
      }

      toRvalueIfNecessary(target, node->target, !node->isLhsOfAssigment_);

      if (!isa<PointerType>(target)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Trying to dereference non-pointer type", node->target.get());
      }

      target = cast<PointerType>(target)->getPointeeType();
      if (node->isLhsOfAssigment_) {
         target->isLvalue(true);
      }

      node->operandType = target;
      return ReturnMemberExpr(node, target);
   }

   if (op == "&") {
      if (!currentBlockUnsafe) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed inside unsafe blocks", node);
      }

      if (!target->isLvalue()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply unary operator '&' to non-reference value",
            node->target.get());
      }

      target->isLvalue(false);
      auto ptr = target->getPointerTo();

      return ReturnMemberExpr(node, ptr);
   }

   Type *result;

   lvalueToRvalue(node->target);
   target = target->toRvalue();
   node->operandType = target;

   if (op == "+" || op == "-") {
      if (!isa<PrimitiveType>(target)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target->toString(),
            node->target.get());
      }
      if (op == "-" && isa<IntegerType>(target) && cast<IntegerType>(target)->isUnsigned()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '-' cannot be applied to unsigned integer",
            node->target.get());
      }

      result = target;
   }
   else if (op == "~") {
      if (!isa<IntegerType>(target)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '~' is only applicable to type Int", node->target.get());
      }

      result = target;
   }
   else if (op == "!") {
      auto boolTy = IntegerType::get(1);
      if (!target->implicitlyCastableTo(boolTy)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '!' is not applicable to type " + target
            ->toString(), node->target.get());
      }

      if (target != boolTy) {
         Warning::issue("Implicit cast to boolean", node->target.get());
      }

      result = boolTy;
   }
   else {
      RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + node->op + " is not defined on class " +
         target->toString(), node);
   }

   return ReturnMemberExpr(node, result);
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(BreakStmt *node) {
   if (!breakable) {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in switch, for and while statements",
         node);
   }

   break_();

   return nullptr;
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ContinueStmt *node) {
   if (!continuable) {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements",
         node);
   }

   continue_();

   return nullptr;
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(IfStmt *node) {
   Type* cond = node->condition->accept(*this);

   if (!cond->implicitlyCastableTo(IntegerType::get(1))) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Condition must be boolean", node->condition.get());
   }

   // if there's no else, the remaining code path needs to return either way
   if (node->elseBranch) {

      TypeCheckVisitor if_visitor(this);
      if_visitor.returnable = true;

      node->ifBranch->accept(if_visitor);

      TypeCheckVisitor else_visitor(this);
      else_visitor.returnable = true;

      node->elseBranch->accept(else_visitor);

      // all branches return
      if (if_visitor.branches - if_visitor.returned <= 0 && !isa<VoidType>(declaredReturnType)
            && else_visitor.branches - else_visitor.returned <= 0) {
         returned++;
      }
   }
   else {
      TypeCheckVisitor if_visitor(this);
      if_visitor.returnable = returnable;

      node->ifBranch->accept(if_visitor);
   }

   return nullptr;
}

bool TypeCheckVisitor::matchableAgainst(Type*& matchTy, std::shared_ptr<CaseStmt> const& case_)
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
      auto& givenArgs = callExpr->argTypes;
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
            RuntimeError::raise(ERR_TYPE_ERROR, "Expected all arguments to be 'let' expressions", arg.second.get());
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
            givenArgs.push_back(nullptr);
         }
         else {
            givenArgs.push_back(arg.second->accept(*this));
         }
      }

      if (en->hasCase(callExpr->ident, givenArgs, callExpr->generics).compatibility != CompatibilityType::COMPATIBLE) {
         return false;
      }

      case_->enumCaseVal = &en->getCase(callExpr->ident);
      return true;
   }

   if (matchTy->isObject()) {
      caseVal->setContextualType(matchTy);
      auto givenTy = caseVal->accept(*this);
      case_->caseType = givenTy;

      if (!givenTy->implicitlyCastableTo(matchTy)) {
         return false;
      }

      return true;
   }

   return false;
}

Type* TypeCheckVisitor::visit(MatchStmt *node) {
   Type* switchType = node->switchValue->accept(*this);

   toRvalueIfNecessary(switchType, node->switchValue);

   if (switchType->isObject() && !switchType->isEnum()) {
      auto cl = SymbolTable::getClass(switchType->getClassName());
      string protName = "Equatable";

      if (!cl->conformsTo(protName)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Types used as a match value must conform to 'Equatable'", node);
      }
   }
   else if (!isa<IntegerType>(switchType) && !isa<FPType>(switchType) &&
      !switchType->isTupleTy() && !switchType->isEnum())
   {
      RuntimeError::raise(ERR_TYPE_ERROR, "Types used as a match value must conform to 'Equatable'", node);
   }

   std::vector<long> rawCaseValues;
   std::vector<size_t> checkIfContinuable;
   size_t numCases = node->cases.size();
   size_t i = 0;
   bool allCasesReturn = true;
   for (const auto& case_ : node->cases) {
      if (!matchableAgainst(switchType, case_)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot match given value against value of type " +
            switchType->toString(), case_->caseVal.get());
      }
      else if (case_->caseType != nullptr && *case_->caseType != switchType) {
         wrapImplicitCast(case_->caseVal, case_->caseType, switchType);
      }

      if (case_->isDefault) {
         node->hasDefault = true;
         node->defaultIndex = i;
      }

      ++branches;

      TypeCheckVisitor caseVisitor(this);
      caseVisitor.continuable = true;

      if (switchType->isEnum() && !case_->isDefault) {
         if (std::find(rawCaseValues.begin(), rawCaseValues.end(), case_->enumCaseVal->rawValue)
            != rawCaseValues.end())
         {
            RuntimeError::raise(ERR_TYPE_ERROR, "Duplicate case " + case_->enumCaseVal->name, case_.get());
         }

         rawCaseValues.push_back(case_->enumCaseVal->rawValue);

         if (case_->isEnumLetCase || case_->isEnumVarCase) {
            auto en = static_cast<Enum*>(SymbolTable::getClass(switchType->getClassName()));

            for (auto& val : case_->letIdentifiers) {
               auto ty = val.second->deepCopy();
               if (ty->isGeneric() && currentClass != en->getName()) {
                  case_->needsGenericCast = true;
                  case_->genericOriginTy = ty->deepCopy();

                  Type::resolveGeneric(&ty, switchType->getConcreteGenericTypes());

                  case_->genericDestTy = ty->deepCopy();
                  case_->genericDestTy->isLvalue(true);
               }

               ty->isLvalue(true);
               ty->isConst(case_->isEnumLetCase);

               case_->letBindings.push_back(caseVisitor.declare_var(val.first, ty));
            }
         }
      }

      case_->body->accept(caseVisitor);

      if (i == numCases - 1 && caseVisitor.continued) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot continue from last case statement", case_.get());
      }
      if (caseVisitor.continued && i < numCases - 1) {
         checkIfContinuable.push_back(i);
      }
      if (caseVisitor.returned) {
         ++returned;
      }
      else {
         allCasesReturn = false;
      }

      ++i;
   }

   for (const auto& ind : checkIfContinuable) {
      auto nextCase = node->cases[ind + 1];
      if (nextCase->isEnumLetCase) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Cannot continue to case with 'let' expression", nextCase.get());
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
      RuntimeError::raise(ERR_TYPE_ERROR, "Match statements must be exhaustive", node);
   }
   if (allCasesReturn) {
      ++returned;
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
Type* TypeCheckVisitor::visit(CaseStmt *node) {
   node->fallthrough = true;
   for (const auto& child : node->get_children()) {
      if (child->get_type() == NodeType::BREAK_STMT) {
         node->fallthrough = false;
      }
      child->accept(*this);
   }

   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(LabelStmt *node) {
   if (std::find(labels.begin(), labels.end(), node->labelName) != labels.end()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Label '" + node->labelName + "' already exists in the same scope", node);
   }

   labels.push_back(node->labelName);

   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(GotoStmt *node) {
   if (!has_label(node->labelName)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "No label '" + node->labelName + "' to go to", node);
   }

   return nullptr;
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FuncArgDecl *node) {

   if (!node->visitDefault) {
      node->argType->accept(*this);
      auto &ts = node->argType->getType();

      node->isStruct = ts->isStruct();
      node->mut = ts->isLvalue();

      if (node->mut) {
         node->isStruct = false; // we don't want to memcpy a ref parameter
         node->argType->type = ts;
      }

      return ts;
   }

   auto &ts = node->argType->getType();
   if (node->defaultVal) {
      node->defaultVal->setContextualType(node->argType->getType(true));
      Type* def_type = node->defaultVal->accept(*this);

      if (!ts->implicitlyCastableTo(def_type)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Default value for parameter " + node->argName + " must be of type "
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
Type* TypeCheckVisitor::visit(ReturnStmt *node) {

   if (node->returnValue) {
      bool isSelfReturn = node->returnValue->get_type() == NodeType::IDENTIFIER_EXPR &&
         std::static_pointer_cast<IdentifierRefExpr>(node->returnValue)->ident == "self";

      if (isSelfReturn) {
         SymbolTable::getClass(currentClass)->getMethod(currentFunction)->hasHiddenParam = false;
      }

      if (declaredReturnType->isStruct() && !isSelfReturn) {
         node->returnValue->isHiddenReturnValue();
         node->hiddenParamReturn = true;
      }
      else {
         node->returnValue->isReturnValue();
      }

      node->returnValue->setContextualType(declaredReturnType);

      Type* retType = node->returnValue->accept(*this);
      toRvalueIfNecessary(retType, node->returnValue, !declaredReturnType->isLvalue());

      node->returnType = declaredReturnType->deepCopy();
      return_(retType, node->returnValue.get());

      if (node->returnValue->declaration) {
         node->returnValue->declaration->isReturnValue();
         if (declaredReturnType->isStruct()) {
            // don't clone a 'self' return
            if (!isSelfReturn) {
               node->returnValue->declaration->isHiddenReturnValue();
            }
         }
      }

      if (*retType != declaredReturnType) {
         wrapImplicitCast(node->returnValue, retType, declaredReturnType);
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
Type* TypeCheckVisitor::visit(Expression *node) {
   return nullptr;
}

cdot::cl::Class* TypeCheckVisitor::DeclareClass(ClassDecl *node) {

   node->qualifiedName = ns_prefix() + node->className;

   if (node->isExtension()) {
      auto cl = SymbolTable::getClass(node->qualifiedName);
      node->declaredClass = cl;

      for (const auto& proto : node->conformsTo) {
         cl->addConformity(proto);
      }

      return cl;
   }

   if (isReservedIdentifier(node->className)) {
      RuntimeError::raise(ERR_TYPE_ERROR, node->className + " is a reserved identifier", node);
   }

   currentClass = node->qualifiedName;

   if (node->className != "Any") {
      node->conformsTo.push_back(ObjectType::get("Any"));
   }

   cdot::cl::Class* cl;
   if (node->is_protocol || node->is_struct) {
      cl = SymbolTable::declareClass(node->qualifiedName, node->conformsTo, node->generics, node->is_protocol, node);
   }
   else {
      cl = SymbolTable::declareClass(node->qualifiedName, node->parentClass, node->conformsTo, node->generics, node,
         node->is_abstract);
   }

   node->declaredClass = cl;
   currentClass = "";

   return cl;
}

cdot::cl::Class* TypeCheckVisitor::DeclareClassMethods(ClassDecl *node) {

   auto cl = node->declaredClass;

   if (!node->is_extension) {
      cl->defineParentClass();
      currentClass = node->qualifiedName;
   }

   inProtocolDefinition = cl->isProtocol();

   if (!node->is_extension && node->parentClass != nullptr) {
      if (!SymbolTable::hasClass(node->parentClass->getClassName())) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->parentClass->getClassName() + " does not exist", node);
      }

      auto parentClass = SymbolTable::getClass(node->parentClass->getClassName());
      Type::resolveUnqualified(node->parentClass);

      auto &concreteGenerics = node->parentClass->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = parentClass->getGenerics().size();

      if (givenCount != neededCount) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->parentClass->getClassName() + " expects " + std::to_string
            (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node
         );
      }

      size_t i = 0;
      for (const auto &needed : parentClass->getGenerics()) {
         auto &given = concreteGenerics[needed->getGenericClassName()];
         resolve(&given);

         if (isa<ObjectType>(given)) {
            checkExistance(cast<ObjectType>(given), node);
         }

         if (!Type::GenericTypesCompatible(given, needed)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Given type parameter " + given->toString() + " is not compatible"
               " with needed parameter " + needed->getGenericClassName() + " of class " +
               node->parentClass->getClassName(), node
            );
         }

         cl->defineConcreteGeneric(needed->getGenericClassName(), given);
      }
   }

   for (const auto& prot : node->conformsTo) {
      auto& protocolName = prot->getClassName();

      if (!SymbolTable::hasClass(protocolName)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Protocol " + protocolName + " does not exist", node);
      }

      Type::resolveUnqualified(prot);

      auto protocol = SymbolTable::getClass(protocolName);
      if (!protocol->isProtocol()) {
         RuntimeError::raise(ERR_TYPE_ERROR, protocolName + " is not a protocol", node);
      }

      auto& concreteGenerics = prot->getConcreteGenericTypes();
      auto givenCount = concreteGenerics.size();
      auto neededCount = protocol->getGenerics().size();

      if (givenCount != neededCount) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Protocol " + protocolName + " expects " + std::to_string
            (neededCount) + " generic type parameter(s), " + std::to_string(givenCount) + " were given", node);
      }

      size_t i = 0;
      for (const auto& needed : protocol->getGenerics()) {
         auto& given = concreteGenerics[needed->getGenericClassName()];
         resolve(&given);

         if (!Type::GenericTypesCompatible(given, needed)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Given type parameter " + given->toString() + " is not compatible"
               " with needed parameter " + needed->getGenericClassName() + " of protocol " + protocolName, node);
         }
      }
   }

   pushNamespace(node->className);
   node->declaredClass = cl;

   for (const auto& td : node->typedefs) {
      td->accept(*this);
   }
   for (const auto& field : node->fields) {
      DeclareField(field.get(), cl);
   }
   for (const auto& method : node->methods) {
      DeclareMethod(method.get(), cl);
   }

   if (!node->constructors.empty()) {
      for (const auto& constr : node->constructors) {
         DeclareConstr(constr.get(), cl);
      }
   }

   if (!node->is_extension && !node->is_abstract) {
      node->defaultConstr = cl->declareMethod("init.def", cl->getType()->toRvalue(),
         AccessModifier::PUBLIC, {}, {}, {}, {}, false, nullptr
      );

      node->selfBinding = SymbolTable::mangleVariable(self_str, TypeCheckVisitor(this).scope);
   }

   popNamespace();
   currentClass = "";
   inProtocolDefinition = false;

   if (!node->is_extension && !ObjectType::hasStructureType(node->qualifiedName)) {
      auto prefix = node->is_struct ? "struct." : (node->is_protocol ? "proto." : "class.");
      auto class_type = llvm::StructType::create(CodeGenVisitor::Context, prefix + node->qualifiedName);
      ObjectType::declareStructureType(node->qualifiedName, class_type);
   }

   return cl;
}

cdot::cl::Enum* TypeCheckVisitor::DeclareEnum(EnumDecl *node)
{
   node->qualifiedName = ns_prefix() + node->className;
   if (isReservedIdentifier(node->className)) {
      RuntimeError::raise(ERR_TYPE_ERROR, node->className + " is a reserved identifier", node);
   }

   node->conformsTo.push_back(ObjectType::get("Any"));

   auto en = SymbolTable::declareEnum(node->qualifiedName, node->conformsTo, node->generics);
   node->declaredEnum = en;

   return en;
}

namespace {

   Variant getStartValue(VariantType ty) {
      switch (ty) {
         case VariantType::INT: return Variant(0);
         case VariantType::FLOAT: return Variant(0.0);
         case VariantType::STRING: return Variant(string("a"));
         default:
            llvm_unreachable("Unsupported enum type");
      }
   }

   Variant getNextValue(Variant& v) {
      switch (v.type) {
         case VariantType::INT:
            return Variant(v.intVal + 1);
         case VariantType::FLOAT:
            return Variant(v.floatVal + 1.0);
         case VariantType::STRING: {
            auto str = v.strVal;
            std::transform(str.begin(), str.end(), str.begin(), tolower);

            if (str.back() == 'z') {
               return Variant(str + 'a');
            }

            return Variant(str.substr(0, str.length() - 1) + (char) (str.back() + 1));
         }
         default:
            llvm_unreachable("Unsupported enum type");
      }
   }

   int checkIfDuplicate(std::vector<Variant>& values) {
      if (values.empty()) {
         return -1;
      }

      int i = 0;
      switch (values.front().type) {
         case VariantType::INT: {
            std::vector<long> rawVals;
            rawVals.reserve(values.size());

            for (auto& val : values) {
               if (std::find(rawVals.begin(), rawVals.end(), val.intVal) != rawVals.end()) {
                  return i;
               }

               rawVals.push_back(val.intVal);
               ++i;
            }

            return -1;
         }
         case VariantType::FLOAT: {
            std::vector<double> rawVals;
            rawVals.reserve(values.size());

            for (auto& val : values) {
               if (std::find(rawVals.begin(), rawVals.end(), val.floatVal) != rawVals.end()) {
                  return i;
               }

               rawVals.push_back(val.floatVal);
               ++i;
            }

            return -1;
         }
         case VariantType::STRING: {
            std::vector<string> rawVals;
            rawVals.reserve(values.size());

            for (auto& val : values) {
               if (std::find(rawVals.begin(), rawVals.end(), val.strVal) != rawVals.end()) {
                  return i;
               }

               rawVals.push_back(val.strVal);
               ++i;
            }

            return -1;
         }
         default:
            llvm_unreachable("Unsupported enum type");
      }
   }
}

cdot::cl::Enum* TypeCheckVisitor::DeclareEnumMethods(EnumDecl *node)
{
   auto& en = node->declaredEnum;
   for (const auto& method : node->methods) {
      DeclareMethod(method.get(), en);
   }

   long last;
   bool first = true;
   std::vector<long> caseVals;

   for (const auto& case_ : node->cases) {
      EnumCase c;
      c.name = case_->caseName;
      for (const auto& assoc : case_->associatedTypes) {
         assoc.second->accept(*this);
         c.associatedValues.emplace_back(assoc.first, assoc.second->getType());
      }

      if (case_->hasRawValue) {
         last = case_->rawValue;
      }
      else if (first) {
         last = 0;
      }
      else {
         ++last;
      }

      if (std::find(caseVals.begin(), caseVals.end(), last) != caseVals.end()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Duplicate case value " + std::to_string(last), case_.get());
      }

      c.rawValue = last;
      caseVals.push_back(last);
      first = false;

      en->addCase(case_->caseName, std::move(c));
   }

   ObjectType::declareStructureType(node->qualifiedName,
      llvm::StructType::create(CodeGenVisitor::Context, "enum." + node->qualifiedName));

   return en;
}

void TypeCheckVisitor::DeclareField(FieldDecl *node, cdot::cl::Class *cl)
{
   node->type->accept(*this);
   auto field_type = node->type->getType()->deepCopy();

   if (cl->isStruct() && isa<ObjectType>(field_type) && field_type->getClassName() == cl->getName()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Structs cannot have members of their own type (use a pointer instead)",
         node);
   }

   if (!cl->isProtocol() && cl->getDeclaration()->constructors.empty() && !field_type->hasDefaultValue() &&
      node->defaultVal == nullptr && !field_type->isNullable()
   ){
      RuntimeError::raise(ERR_TYPE_ERROR, "Member " + node->fieldName + " does not have a default constructor and "
         "has to be explicitly initialized", node);
   }
   if (node->isConst && node->hasSetter) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Constant fields cannot define setters", node);
   }

   auto& qualified_name = cl->getName();
   node->className = qualified_name;

   if (node->isStatic) {
      return;
   }

   if (node->am == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->am = AccessModifier::PUBLIC;
      }
      else {
         node->am = AccessModifier::PRIVATE;
      }
   }

   auto field = cl->declareField(node->fieldName, field_type, node->am, node->defaultVal, node->isConst, node);
   node->declaredType = &field->fieldType;
   if (cl->isProtocol()) {
      node->isProtocolField = true;
   }

   if (node->hasGetter) {
      std::vector<Type*> argTypes;
      string getterName = "__" + util::generate_getter_name(node->fieldName);
      node->getterBinding = SymbolTable::mangleMethod(node->className, getterName, argTypes);

      auto getterRetType = field_type->deepCopy();

      node->getterMethod = cl->declareMethod(getterName, getterRetType, AccessModifier::PUBLIC, {}, {}, {}, {},
         false, nullptr);

      field->hasGetter = true;
      field->getterName = getterName;
   }

   if (node->hasSetter) {
      std::vector<Type*> argTypes{ field_type->deepCopy() };
      string setterName = "__" + util::generate_setter_name(node->fieldName);
      auto setterRetType = new VoidType;
      node->setterBinding = SymbolTable::mangleMethod(node->className, setterName, argTypes);

      node->setterMethod = cl->declareMethod(setterName, setterRetType, AccessModifier::PUBLIC, {node->fieldName},
         argTypes, {}, {}, false, nullptr);

      field->hasSetter = true;
      field->setterName = setterName;
   }
}

void TypeCheckVisitor::DefineField(FieldDecl *node, cdot::cl::Class *cl) {
   auto& field_type = node->type->getType();

   if (node->defaultVal != nullptr) {
      node->defaultVal->setContextualType(field_type);
      Type* def_type = node->defaultVal->accept(*this);

      if (field_type->isInferred()) {
         field_type = def_type;

         if (!node->isStatic) {
            *node->declaredType = field_type;
            cl->getField(node->fieldName)->fieldType = field_type;
         }
      }
      else if (!field_type->implicitlyCastableTo(def_type)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Default value for field " + node->fieldName + " must be of type " +
            node->type->toString(), node->defaultVal.get());
      }
      else if (*def_type != field_type) {
         Warning::issue("Implicit cast from " + def_type->toString() + " to " + node->type->toString(),
            node->defaultVal.get());
      }
   }

   if (node->isStatic) {
      node->binding = declare_var(node->fieldName, field_type, true);
      return;
   }

   if (node->hasGetter && node->getterBody != nullptr) {
      auto visitor = makeMethodVisitor(node->getterMethod->returnType, node->className);
      visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);

      node->getterBody->accept(visitor);
      node->getterSelfBinding = visitor.currentSelf;

      if (visitor.returned == 0) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Returning Void from a method with declared return type " +
            node->getterMethod->returnType->toString(), node);
      }

      if (visitor.branches - visitor.returned > 0) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->getterBody.get());
      }
   }

   if (node->hasSetter && node->setterBody != nullptr) {
      auto visitor = makeMethodVisitor(node->setterMethod->returnType, node->className);
      string newValStr = "newVal";

      auto typeref = std::make_shared<TypeRef>(field_type->deepCopy());
      typeref->resolved = true;

      node->newVal = std::make_shared<FuncArgDecl>(newValStr, typeref);

      visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);
      node->newVal->binding = visitor.declare_var(newValStr, field_type->deepCopy());

      node->setterSelfBinding = visitor.currentSelf;
      node->setterBody->accept(visitor);
   }
}

void TypeCheckVisitor::DeclareMethod(MethodDecl *node, cdot::cl::Class *cl) {
   std::vector<Type*> argTypes;
   std::vector<string> argNames;
   std::vector<Expression::SharedPtr> argDefaults;

   for (const auto &arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();

      argTypes.push_back(resolvedArg);
      argNames.push_back(arg->argName);
      argDefaults.push_back(arg->defaultVal);
   }

   node->class_name = cl->getName();
   node->binding = SymbolTable::mangleMethod(node->class_name, node->methodName, argTypes);

   auto result = cl->hasMethod(node->methodName, argTypes, false, false);
   if (result.compatibility == CompatibilityType::COMPATIBLE) {
      if (node->isAlias) {
         cl->declareMethodAlias(node->alias, result.method->mangledName);
         return;
      }

      RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->methodName + " cannot be redeclared with a "
         "similar signature to a previous declaration", node);
   }
   else if (node->isAlias) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Aliased method " + node->methodName + " does not exist", node);
   }

   if (node->am == AccessModifier::DEFAULT) {
      if (cl->isProtocol() || cl->isEnum() || cl->isStruct()) {
         node->am = AccessModifier::PUBLIC;
      }
      else {
         node->am = AccessModifier::PRIVATE;
      }
   }

   node->returnType->accept(*this);
   auto return_type = node->returnType->getType()->deepCopy();

   node->method = cl->declareMethod(node->methodName, return_type, node->am, argNames,
      argTypes, argDefaults, node->generics, node->isStatic, node);

   if (return_type->isStruct()) {
      node->method->hasHiddenParam = true;
   }
}

void TypeCheckVisitor::DeclareConstr(ConstrDecl *node, cdot::cl::Class *cl) {

   if (node->memberwise && cl->isAbstract()) {
      cl->declareMemberwiseInitializer();
      return;
   }

   std::vector<Type*> argTypes;
   std::vector<string> argNames;
   std::vector<Expression::SharedPtr> argDefaults;

   for (auto arg : node->args) {
      arg->accept(*this);
      auto& resolvedArg = arg->argType->getType();
      argTypes.push_back(resolvedArg);

      argNames.push_back(arg->argName);
      argDefaults.push_back(arg->defaultVal);
   }

   string method_name = "init";

   node->className = cl->getName();

   node->binding = SymbolTable::mangleMethod(node->className, method_name, argTypes);
   node->method = cl->declareMethod(method_name, cl->getType()->toRvalue(), node->am, argNames,
      argTypes, argDefaults, {}, false, nullptr);
}

void TypeCheckVisitor::DefineClass(ClassDecl *node, cdot::cl::Class *cl) {

   pushNamespace(node->className);
   currentClass = node->qualifiedName;
   inProtocolDefinition = cl->isProtocol();
   currentClassGenerics = &cl->getGenerics();

   for (const auto& field : node->fields) {
      DefineField(field.get(), cl);
   }

   if (node->isStruct() || (node->is_extension && cl->isStruct())) {
      cl->declareMemberwiseInitializer();
   }

   for (const auto& method : node->methods) {
      DefineMethod(method.get(), cl);
   }

   for (const auto& constr : node->constructors) {
      DefineConstr(constr.get(), cl);
   }

   popNamespace();
   currentClass = "";
   inProtocolDefinition = false;
   currentClassGenerics = nullptr;

   try {
      cl->finalize();
   } catch (string err) {
      RuntimeError::raise(ERR_TYPE_ERROR, err, node);
   }
}

void TypeCheckVisitor::DefineMethod(MethodDecl *node, cdot::cl::Class *cl) {

   if (node->isAlias) {
      return;
   }

   auto& return_type = node->returnType->getType();
   auto method_visitor = makeMethodVisitor(return_type, cl->getName());

   if (!node->isStatic) {
      method_visitor.currentSelf = SymbolTable::mangleVariable(self_str, method_visitor.scope);
      node->selfBinding = method_visitor.currentSelf;
   }

   attributes = node->attributes;

   if (node->body) {
      for (const auto &arg : node->args) {
         arg->binding = method_visitor.declare_var(arg->argName, arg->argType->getType());
      }

      method_visitor.currentFunction = node->method->mangledName;
      node->body->accept(method_visitor);

      if (method_visitor.returned == 0) {
         if (!isa<VoidType>(node->returnType->getType())) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Returning Void from a method with declared return type " +
               node->returnType->getType()->toString(), node->returnType.get());
         }
      }
      else {
         return_type = method_visitor.declaredReturnType;
      }

      node->method->returnType = return_type;

      if (method_visitor.branches - method_visitor.returned > 0 && !isa<VoidType>(return_type) &&
         !method_visitor.declaredReturnType->isNullable())
      {
         RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->body.get());
      }
   }
   else if (return_type->isInferred()) {
      return_type = new VoidType();
   }

   attributes.clear();
}

namespace {

   using cdot::cl::Field;

   bool fieldNeedsInitializing(const Field::SharedPtr& field) {
      return (!field->fieldType->hasDefaultValue() || field->isConst)
         && field->defaultVal == nullptr;
   }

}
void TypeCheckVisitor::DefineConstr(ConstrDecl *node, cdot::cl::Class *cl) {

   if (node->memberwise) {
      return;
   }

   auto visitor = makeMethodVisitor(new VoidType, cl->getName());
   visitor.currentSelf = SymbolTable::mangleVariable(self_str, visitor.scope);
   node->selfBinding = visitor.currentSelf;

   std::vector<string> uninitialized;
   for (const auto& field : cl->getFields()) {
      if (fieldNeedsInitializing(field.second)) {
         uninitialized.push_back(field.second->fieldName);
      }
   }

   visitor.uninitializedFields = &uninitialized;
   for (auto& arg : node->args) {
      arg->binding = visitor.declare_var(arg->argName, arg->argType->getType());
   }

   node->body->accept(visitor);

   if (!uninitialized.empty()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Non-nullable member " + uninitialized.front() + " of "
         "class " + cl->getName() + " does not define a default constructor and has to be explicitly initialized "
         "in every constructor", node->body.get());
   }
}

/**
 * Declares a new class
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ClassDecl *node) {
   currentClassGenerics = &node->generics;
   DefineClass(node, node->declaredClass);

   currentClassGenerics = nullptr;
   return nullptr;
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(ConstrDecl *node) {
   return nullptr;
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(FieldDecl *node) {
   return nullptr;
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(MethodDecl *node) {
   return nullptr;
}

Type* TypeCheckVisitor::visit(EnumDecl *node)
{
   auto en = node->declaredEnum;
   pushNamespace(node->className);
   currentClass = node->qualifiedName;
   currentClassGenerics = &en->getGenerics();

   for (const auto& method : node->methods) {
      DefineMethod(method.get(), en);
   }

   popNamespace();
   currentClass = "";
   inProtocolDefinition = false;
   currentClassGenerics = nullptr;

   try {
      en->finalize();
   } catch (string err) {
      RuntimeError::raise(ERR_TYPE_ERROR, err, node);
   }

   return nullptr;
}

bool TypeCheckVisitor::checkLambdaCompatibility(LambdaExpr *node, Type *neededTy) {
   if (!isa<FunctionType>(neededTy)) {
      return false;
   }

   auto asFunc = cast<FunctionType>(neededTy);
   auto& neededArgs = asFunc->getArgTypes();

   if (!node->returnType->resolved) {
      node->returnType->accept(*this);
   }

   if (!node->returnType->getType()->implicitlyCastableTo(asFunc->getReturnType())) {
      return false;
   }

   if (neededArgs.size() != node->args.size()) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible argument counts: Expected " +
            std::to_string(neededArgs.size()) + ", but found " + std::to_string(node->args.size()),
         node
      );
   }

   size_t i = 0;
   for (const auto& arg : node->args) {
      if (arg->argType != nullptr) {
         if (!arg->argType->resolved) {
            arg->accept(*this);
         }
         if (!arg->argType->getType()->implicitlyCastableTo(neededArgs[i])) {
            return false;
         }
      }
      ++i;
   }

   return true;
}

/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(LambdaExpr *node) {

   Type* returnType;
   node->returnType->accept(*this);

   if (node->contextualType != nullptr && isa<FunctionType>(node->contextualType)) {
      auto asFunc = cast<FunctionType>(node->contextualType);
      auto& neededArgs = asFunc->getArgTypes();
      returnType = asFunc->getReturnType();

      if (neededArgs.size() != node->args.size()) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible argument counts: Expected " +
            std::to_string(neededArgs.size()) + ", but found " + std::to_string(node->args.size()),
            node
         );
      }

      size_t i = 0;
      for (const auto& arg : node->args) {
         if (arg->argType != nullptr && !isa<AutoType>(arg->argType->getType(true))) {
            if (!arg->argType->resolved) {
               arg->accept(*this);
            }
            if (!arg->argType->getType()->implicitlyCastableTo(neededArgs[i])) {
               RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible argument types: No implicit conversion from " +
                  arg->argType->getType()->toString() + " to " + neededArgs[i]->toString(), arg.get()
               );
            }
         }
         else {
            auto ty = std::make_shared<TypeRef>(neededArgs[i]->deepCopy());
            ty->setIndex(arg->startIndex, arg->endIndex, arg->sourceFileId);
            arg->setType(ty);
         }

         ++i;
      }
   }
   else {
      returnType = node->returnType->getType()->deepCopy();
   }

   bool isSingleStmt = node->body->get_type() != NodeType::COMPOUND_STMT;
   if (isSingleStmt && !isa<VoidType>(returnType) && !isa<AutoType>(returnType)) {
      auto asExpr = std::dynamic_pointer_cast<Expression>(node->body);
      if (!asExpr) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Expected single-statement lambda to contain a valid expression",
            node->body.get());
      }

      node->body = std::make_shared<ReturnStmt>(asExpr);
   }

   std::vector<Type*> argTypes;
   string anon = "__anon";
   Function* fun = new Function(anon, returnType);
   std::vector<pair<string, Type*>> captures;

   TypeCheckVisitor visitor = makeFunctionVisitor(returnType);
   visitor.isLambdaVisitor = true;
   visitor.isLambdaRoot = true;
   visitor.capturedVariables = &captures;

   for (const auto& arg : node->args) {
      if (arg->argType == nullptr || isa<AutoType>(arg->argType->getType(true))) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer type of argument " + arg->argName, arg.get());
      }
      if (!arg->argType->resolved) {
         arg->accept(*this);
      }

      argTypes.push_back(arg->argType->getType()->deepCopy());
      fun->addArgument(arg->argType->getType(), arg->defaultVal, arg->argName);

      if (arg->argName != "_") {
         arg->binding = visitor.declare_var(arg->argName, arg->argType->getType()->deepCopy());
      }
   }

   auto ret = node->body->accept(visitor);
   if (visitor.branches - visitor.returned > 0 && !isa<VoidType>(returnType)) {
      if (isa<AutoType>(returnType)) {
         if (isSingleStmt) {
            returnType = ret;
            node->returnType->setType(ret->deepCopy());
         }
         else {
            returnType = new VoidType;
            node->returnType->setType(new VoidType);
         }
      }
      else {
         RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
      }
   }
   else if (ret && !ret->implicitlyCastableTo(returnType)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Returning value of type " + ret->toString() + " from function with "
         "declared return type " + returnType->toString(), node);
   }

   auto funcTy = new FunctionType(returnType->deepCopy(), argTypes);
   funcTy->setFunction(fun);

   node->lambdaType = funcTy;
   node->captures = captures;

   return ReturnMemberExpr(node, funcTy->deepCopy());
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(UsingStmt *node) {
   if (isBuilitinNamespace(node->nsName)) {
      Builtin::ImportBuiltin(node->nsName);
   }

   auto nsName = node->nsName;
   if (!SymbolTable::isNamespace(nsName)) {
      RuntimeError::raise(ERR_CONTEXT_ERROR, "Imported namespace " + node->nsName + " does not exist", node);
   }

   importedNamespaces.push_back(nsName + ".");

   return nullptr;
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type* TypeCheckVisitor::visit(EndOfFileStmt *node) {
   importedNamespaces.clear();
   importedNamespaces.push_back("");

   return nullptr;
}

Type* TypeCheckVisitor::visit(ImplicitCastExpr *node) {
   resolve(&node->to);
   node->target->accept(*this);

   return node->to;
}

Type* TypeCheckVisitor::visit(ExtendStmt *node) {

   if (!SymbolTable::hasClass(node->extended_class)) {
      RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->extended_class + " does not exist", node);
   }

   auto cl = SymbolTable::getClass(node->extended_class, importedNamespaces);
   currentClass = cl->getName();
   currentClassGenerics = &cl->getGenerics();

   for (const auto& field : node->fields) {
      field->accept(*this);
   }

   for (const auto& method : node->methods) {
      method->accept(*this);
   }

   for (const auto& method : node->methods) {
      method->accept(*this);
   }

   currentClass.clear();
   currentClassGenerics = nullptr;

   return nullptr;
}

Type* TypeCheckVisitor::visit(TypedefDecl *node) {

   node->origin->accept(*this);
   SymbolTable::declareTypedef(currentNamespace.back() + node->alias, node->origin->getType());

   return nullptr;
}

Type* TypeCheckVisitor::visit(TypeRef *node) {
   assert(!node->resolved && "Duplicate resolving");

   if (!node->resolved) {
      if (isa<ObjectType>(node->type) && node->type->getClassName() == "Self" && !inProtocolDefinition) {
         RuntimeError::raise(ERR_TYPE_ERROR, "'Self' is only valid in protocol definitions", node);
      }

      resolve(&node->type);
      node->resolved = true;

      if (node->type->isObject() && !SymbolTable::hasClass(node->type->getClassName())) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Unknown typename " + node->type->toString(), node);
      }

      if (node->type->isCStyleArray() && node->type->getLengthExpr() == nullptr && node->type->getLength() == -1) {
         RuntimeError::raise(ERR_TYPE_ERROR, "C-Style arrays have to have a specified length", node);
      }
   }

   return node->type;
}

Type* TypeCheckVisitor::visit(DeclareStmt *node) {
   if (node->type != nullptr) {
      node->type->accept(*this);
   }

   auto qualified_name = ns_prefix() + node->declaredName;

   switch (node->declKind) {
      case DeclarationType::VAR_DECL: {
         auto type = node->type->getType();
         declare_var(node->declaredName, type, true, node);
         break;
      }
      case DeclarationType::FUNC_DECL: {
         auto type = node->type->getType();
         Function::UniquePtr fun = std::make_unique<Function>(qualified_name, type, node->generics);

         std::vector<Type*> arg_types;
         for (const auto &arg : node->args) {
            arg_types.push_back(arg->accept(*this));
            fun->addArgument(arg_types.back(), arg->defaultVal, arg->argName);
         }

         declare_fun(std::move(fun), node->generics, node);
         node->bind(SymbolTable::mangleFunction(qualified_name, arg_types));

         break;
      }
      case DeclarationType::CLASS_DECL: {
         if (node->extends == nullptr) {
            node->extends = ObjectType::get("Any");
         }

         auto cl = SymbolTable::declareClass(qualified_name, node->extends, node->conformsTo, node->generics, nullptr,
            node->is_abstract);

         auto class_type = llvm::StructType::create(CodeGenVisitor::Context, "class." + qualified_name);
         ObjectType::declareStructureType(qualified_name, class_type);

         break;
      }
   }

   return nullptr;
}

Type* TypeCheckVisitor::visit(LvalueToRvalue* node) {
   llvm_unreachable("Should only be applied after evaluating the node");
}

Type* TypeCheckVisitor::visit(DebugStmt* node) {
   if (node->isUnreachable) {
      ++returned;
   }
   else {
      int i = 3;
   }

   return nullptr;
}

Type* TypeCheckVisitor::visit(Statement* node) {
   return nullptr;
}

Type* TypeCheckVisitor::visit(TupleLiteral* node) {
   std::vector<pair<string, Type*>> containedTypes;

   for (const auto& el : node->elements) {
      auto ty = el.second->accept(*this);
      containedTypes.emplace_back(el.first, ty);
   }

   auto tupleTy = new TupleType(containedTypes);
   if (node->contextualType && !isa<AutoType>(node->contextualType)) {
      if (!tupleTy->implicitlyCastableTo(node->contextualType)) {
         RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + tupleTy->toString() + " and " +
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