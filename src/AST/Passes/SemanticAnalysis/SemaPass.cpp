//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "SemaPass.h"
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
std::vector<string> SemaPass::currentNamespace = { "" };
std::vector<string> SemaPass::importedNamespaces = { "" };
std::vector<std::vector<GenericConstraint>*> SemaPass::GenericsStack = {};

SemaPass::SemaPass()
{
   pushScope();
   classScopeStack.push("");

   util::TCPass = this;
}

void SemaPass::pushClassScope(cl::Class *cl, string& className)
{
   pushNamespace(className);
   latestScope->inProtocol = cl->isProtocol();

   classScopeStack.push(currentNamespace.back());
   GenericsStack.push_back(&cl->getGenerics());
}

void SemaPass::popClassScope()
{
   popNamespace();
   latestScope->inProtocol = false;

   classScopeStack.pop();
   GenericsStack.pop_back();
}

void SemaPass::doInitialPass(std::vector<std::shared_ptr<Statement>>& statements)
{
   for (const auto& stmt : statements) {
      switch (stmt->get_type()) {
         case NodeType::CLASS_DECL: {
            auto node = std::static_pointer_cast<ClassDecl>(stmt);
            auto cl = node->declaredClass;

            pushClassScope(cl, node->className);

            doInitialPass(node->innerDeclarations);
            for (const auto &field : node->fields) {
               DefineField(field.get(), cl);
            }

            popClassScope();

            break;
         }
//         case NodeType::DECLARATION: {
//            auto node = std::static_pointer_cast<DeclStmt>(stmt);
//            if (node->isGlobal()) {
//               globalVars.push_back(node);
//            }
//
//            break;
//         }
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

void SemaPass::CopyScopeProps(
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

bool SemaPass::warnCast(Type &lhs, Type &rhs)
{
   return !lhs->isBoxedEquivOf(*rhs) && !rhs->isBoxedEquivOf(*lhs);
}

void SemaPass::raiseTypeError(Type &lhs, Type &rhs, AstNode* cause)
{
   diag::err(err_type_mismatch) << lhs << rhs << cause << diag::term;
}

void SemaPass::pushScope() {
   Scope scope;
   scope.id = lastScopeID++;
   CopyScopeProps(latestScope, &scope);

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void SemaPass::pushFunctionScope(
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

void SemaPass::pushMethodScope(
   Method *method)
{
   Scope scope;
   scope.id = lastScopeID++;
   scope.isFunctionRoot = true;
   scope.returnable = true;

   CopyScopeProps(latestScope, &scope);
   if (method->getName() != "init") {
      scope.declaredReturnType = method->getReturnType();
   }
   else {
      scope.declaredReturnType = Type(VoidType::get());
   }

   scope.currentFunction = std::make_pair(method->getName(), method->getMangledName());

   Scopes.push(scope);
   latestScope = &Scopes.top();
}

void SemaPass::pushLoopScope(
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

void SemaPass::popScope() {
   Scopes.pop();
   latestScope = &Scopes.top();
}

void SemaPass::connectTree(
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
string SemaPass::declareVariable(
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
void SemaPass::pushTy(Type& type)
{
   typeStack.push(type);
}

/**
 * Removes a type from the type stack and returns it
 * @return
 */
Type SemaPass::popTy()
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
pair<pair<Variable, string>, bool> SemaPass::getVariable(
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

bool SemaPass::hasVariable(string ident)
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
CallCompatability SemaPass::getFunction(
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

void SemaPass::ApplyCasts(
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
void SemaPass::return_(
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

void SemaPass::continue_(ContinueStmt* continueStmt)
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

void SemaPass::break_(BreakStmt *breakStmt)
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

void SemaPass::resolve(Type* ty, AstNode* node)
{

}

void SemaPass::pushNamespace(string &ns)
{
   auto newNs = currentNamespace.size() == 1 ? ns : currentNamespace.back() + "." + ns;
   currentNamespace.push_back(newNs);
   importedNamespaces.push_back(newNs + ".");
}

void SemaPass::popNamespace()
{
   importedNamespaces.pop_back();
   currentNamespace.pop_back();
}

void SemaPass::wrapImplicitCast(
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

void SemaPass::lvalueToRvalue(
   std::shared_ptr<Expression> &target)
{
   auto copy = target;
   auto ltor = new LvalueToRvalue(copy);

   target.reset(ltor);
   CopyNodeProperties(copy.get(), target.get());
}

void SemaPass::toRvalueIfNecessary(
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

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Type SemaPass::visit(NamespaceDecl *node)
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
Type SemaPass::visit(CompoundStmt *node)
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

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
Type SemaPass::visit(FunctionDecl *node)
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
      auto& externKind = node->getAttribute(Attr::Extern).args.front().strVal;
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

   currentCallable = node->declaredFunction;

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

void SemaPass::CopyNodeProperties(
   Expression *src,
   Expression *dst)
{
   dst->lhs_of_assignment = src->lhs_of_assignment;
   dst->function_argument = src->function_argument;
   dst->part_of_return_value = src->part_of_return_value;
   dst->loc = src->loc;
   dst->isAssigned(src->isAssigned());
   dst->isTemporary(src->isTemporary());
   dst->setTempType(src->getTempType());

   if (src->isStructRetVal()) {
      dst->isHiddenReturnValue();
   }
   else if (src->isReturnedValue()) {
      dst->isReturnValue();
   }
}

BuiltinType* SemaPass::HandleBuiltinIdentifier(IdentifierRefExpr *node)
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
Type SemaPass::visit(IdentifierRefExpr *node)
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

   return ReturnMemberExpr(node, res.first.first.type);
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
Type SemaPass::visit(DeclStmt *node)
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
Type SemaPass::visit(ForStmt *node)
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

Type SemaPass::visit(ForInStmt *node)
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
   
   getIterator.method->addUse();

   auto iteratorCl = SymbolTable::getClass(getIterator.method->getReturnType()->getClassName());
   iteratorCl->addUse();

   auto nextFunc = iteratorCl->hasMethod("next", {}, {}, ObjectType::get(iteratorCl->getName()));
   assert(nextFunc.compatibility == CompatibilityType::COMPATIBLE
      && "Iterator<> not implemented correctly?");

   nextFunc.method->addUse();

   node->iteratorGetter = getIterator.method->getMangledName();
   node->iteratorClass = getIterator.method->getReturnType()->getClassName();
   node->nextFunc = nextFunc.method->getMangledName();
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
Type SemaPass::visit(
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

pair<Type, std::vector<Type>> SemaPass::unify(
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

Type SemaPass::HandleDictionaryLiteral(CollectionLiteral *node)
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
Type SemaPass::visit(CollectionLiteral *node)
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
Type SemaPass::visit(NumericLiteral *node)
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

Type SemaPass::visit(NoneLiteral *node)
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

Type SemaPass::visit(StringLiteral *node)
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

Type SemaPass::visit(StringInterpolation *node)
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
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Type SemaPass::visit(BreakStmt *node)
{
   break_(node);
   return {};
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Type SemaPass::visit(ContinueStmt *node)
{
   continue_(node);
   return {};
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Type SemaPass::visit(IfStmt *node)
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

bool SemaPass::matchableAgainst(
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

Type SemaPass::visit(MatchStmt *node)
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
Type SemaPass::visit(CaseStmt *node)
{
   return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Type SemaPass::visit(LabelStmt *node)
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
Type SemaPass::visit(GotoStmt *node)
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
Type SemaPass::visit(FuncArgDecl *node)
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
Type SemaPass::visit(ReturnStmt *node)
{
   auto &retVal = node->getReturnValue();
   if (retVal) {
      retVal->addUse();
      retVal->isAssigned(true);
      retVal->setContextualType(latestScope->declaredReturnType);

      if (latestScope->declaredReturnType->needsStructReturn()) {
         retVal->isHiddenReturnValue();
         retVal->isPartOfReturnValue(true);
         node->hiddenParamReturn = true;
      }
      else {
         retVal->isReturnValue();
      }

      IdentifierRefExpr::SharedPtr ident = nullptr;
      if (retVal->get_type() == NodeType::IDENTIFIER_EXPR) {
         ident = std::static_pointer_cast<IdentifierRefExpr>(retVal);
      }

      Type retType = retVal->accept(*this);
      toRvalueIfNecessary(retType, retVal, !latestScope->declaredReturnType.isLvalue());

      node->returnType = latestScope->declaredReturnType;
      return_(retType, retVal.get());

      if (ident) {
         auto it = declarations.find(ident->getBinding());
         if (it != declarations.end()) {
            it->second->isReturnedValue(true);
         }
      }

      if (retType != latestScope->declaredReturnType) {
         wrapImplicitCast(retVal, retType, latestScope->declaredReturnType);
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
Type SemaPass::visit(Expression *node)
{
   return {};
}


/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Type SemaPass::visit(LambdaExpr *node)
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
Type SemaPass::visit(UsingStmt *node)
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
Type SemaPass::visit(EndOfFileStmt *node)
{
   importedNamespaces.clear();
   importedNamespaces.push_back("");
   SymbolTable::clearTemporaryAliases();

   return {};
}

Type SemaPass::visit(ImplicitCastExpr *node)
{
   resolve(&node->to);
   node->target->accept(*this);

   return node->to;
}

Type SemaPass::visit(TypedefDecl *node)
{
   return {};
}

Type SemaPass::visit(TypeRef *node)
{
   assert(!node->resolved && "Duplicate resolving");
   DeclPass::resolveType(node, GenericsStack, importedNamespaces, currentNamespace);
   return node->type;
}

Type SemaPass::visit(DeclareStmt *node)
{
   for (const auto& decl : node->declarations) {
      decl->accept(*this);
   }

   return {};
}

Type SemaPass::visit(LvalueToRvalue* node)
{
   llvm_unreachable("Should only be applied after evaluating the node");
}

Type SemaPass::visit(DebugStmt* node)
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

Type SemaPass::visit(Statement* node)
{
   return {};
}

Type SemaPass::visit(TupleLiteral* node)
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

Type SemaPass::visit(TryStmt *node)
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

Type SemaPass::visit(ThrowStmt *node)
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

   assert(currentCallable && "no function?");
   currentCallable->addThrownType(*thrown);

   return {};
}

