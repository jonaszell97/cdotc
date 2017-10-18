//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"
#include "../../SymbolTable.h"
#include "../../Passes/StaticAnalysis/Record/Class.h"
#include "../../Passes/StaticAnalysis/Record/Enum.h"

CallExpr::CallExpr(CallType type, std::vector<pair<string, Expression::SharedPtr>> args, std::string _ident) :
   type(type),
   args(args)
{
   ident = _ident;
   for (auto& arg : this->args) {
      children.push_back(&arg.second);
   }
}

CallExpr::CallExpr(CallType type, std::vector<Expression::SharedPtr> args, string _ident) :
   type(type)
{
   ident = _ident;
   for (auto& arg : args) {
      this->args.emplace_back("", arg);
      children.push_back(&this->args.back().second);
   }
}

bool CallExpr::createsTemporary()
{
   return returnType->isObject();
}

void CallExpr::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   for (auto &arg : args) {
      if (arg.second.get() == child) {
         arg.second.reset(replacement);
         return;
      }
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
   std::vector<AstNode::SharedPtr> res;
   for (auto arg : args) {
      res.push_back(arg.second);
   }
   if (memberExpr != nullptr) {
      res.push_back(memberExpr);
   }

   return res;
}

void CallExpr::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "CallExpr" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}

CallType CallExpr::getType() const {
   return type;
}

void CallExpr::setType(CallType type) {
   CallExpr::type = type;
}

const std::vector<pair<string, Expression::SharedPtr>> &CallExpr::getArgs() const {
   return args;
}

void CallExpr::setArgs(const std::vector<pair<string, Expression::SharedPtr>> &args) {
   CallExpr::args = args;
}

bool CallExpr::isIsCallOp() const {
   return isCallOp;
}

void CallExpr::setIsCallOp(bool isCallOp) {
   CallExpr::isCallOp = isCallOp;
}

const string &CallExpr::getCallOpBinding() const {
   return callOpBinding;
}

void CallExpr::setCallOpBinding(const string &callOpBinding) {
   CallExpr::callOpBinding = callOpBinding;
}

bool CallExpr::isIsCapturedVar() const {
   return isCapturedVar;
}

void CallExpr::setIsCapturedVar(bool isCapturedVar) {
   CallExpr::isCapturedVar = isCapturedVar;
}

bool CallExpr::isHasStructReturn() const {
   return hasStructReturn;
}

void CallExpr::setHasStructReturn(bool hasStructReturn) {
   CallExpr::hasStructReturn = hasStructReturn;
}

BuiltinType *CallExpr::getStructReturnType() const {
   return structReturnType;
}

void CallExpr::setStructReturnType(BuiltinType *structReturnType) {
   CallExpr::structReturnType = structReturnType;
}

bool CallExpr::isIsPointerAccess_() const {
   return isPointerAccess_;
}

void CallExpr::setIsPointerAccess_(bool isPointerAccess_) {
   CallExpr::isPointerAccess_ = isPointerAccess_;
}

CallCompatability *CallExpr::getCompatibility() const {
   return compatibility;
}

void CallExpr::setCompatibility(CallCompatability *compatibility) {
   CallExpr::compatibility = compatibility;
}

bool CallExpr::isIsNsMember() const {
   return isNsMember;
}

void CallExpr::setIsNsMember(bool isNsMember) {
   CallExpr::isNsMember = isNsMember;
}

bool CallExpr::isIsStatic() const {
   return isStatic;
}

void CallExpr::setIsStatic(bool isStatic) {
   CallExpr::isStatic = isStatic;
}

const Type &CallExpr::getReturnType() const {
   return returnType;
}

void CallExpr::setReturnType(const Type &returnType) {
   CallExpr::returnType = returnType;
}

bool CallExpr::isIs_virtual() const {
   return is_virtual;
}

void CallExpr::setIs_virtual(bool is_virtual) {
   CallExpr::is_virtual = is_virtual;
}

bool CallExpr::isIsBuiltin() const {
   return isBuiltin;
}

void CallExpr::setIsBuiltin(bool isBuiltin) {
   CallExpr::isBuiltin = isBuiltin;
}

BuiltinFn CallExpr::getBuiltinFnKind() const {
   return builtinFnKind;
}

void CallExpr::setBuiltinFnKind(BuiltinFn builtinFnKind) {
   CallExpr::builtinFnKind = builtinFnKind;
}

size_t CallExpr::getAlignment() const {
   return alignment;
}

void CallExpr::setAlignment(size_t alignment) {
   CallExpr::alignment = alignment;
}

const string &CallExpr::getClassName() const {
   return className;
}

void CallExpr::setClassName(const string &className) {
   CallExpr::className = className;
}

Method *CallExpr::getMethod() const {
   return method;
}

void CallExpr::setMethod(Method *method) {
   CallExpr::method = method;
}

const Type &CallExpr::getGenericOriginTy() const {
   return genericOriginTy;
}

void CallExpr::setGenericOriginTy(const Type &genericOriginTy) {
   CallExpr::genericOriginTy = genericOriginTy;
}

const Type &CallExpr::getGenericDestTy() const {
   return genericDestTy;
}

void CallExpr::setGenericDestTy(const Type &genericDestTy) {
   CallExpr::genericDestTy = genericDestTy;
}

bool CallExpr::isNeedsGenericCast() const {
   return needsGenericCast;
}

void CallExpr::setNeedsGenericCast(bool needsGenericCast) {
   CallExpr::needsGenericCast = needsGenericCast;
}

bool CallExpr::isAnonymousCall() const {
   return anonymous_call;
}

void CallExpr::setIsAnonymousCall(bool isAnonymousCall_) {
   CallExpr::anonymous_call = isAnonymousCall_;
}

const std::vector<std::shared_ptr<TypeRef>> &CallExpr::getGenerics() const {
   return generics;
}

void CallExpr::setGenerics(const std::vector<std::shared_ptr<TypeRef>> &generics) {
   CallExpr::generics = generics;
}

const std::vector<GenericType *> &CallExpr::getResolvedGenerics() const {
   return resolvedGenerics;
}

void CallExpr::setResolvedGenerics(const std::vector<GenericType *> &resolvedGenerics) {
   CallExpr::resolvedGenerics = resolvedGenerics;
}

const std::vector<Argument> &CallExpr::getResolvedArgs() const {
   return resolvedArgs;
}

void CallExpr::setResolvedArgs(const std::vector<Argument> &resolvedArgs) {
   CallExpr::resolvedArgs = resolvedArgs;
}

std::vector<Argument> *CallExpr::getDeclaredArgTypes() const {
   return declaredArgTypes;
}

void CallExpr::setDeclaredArgTypes(std::vector<Argument> *declaredArgTypes) {
   CallExpr::declaredArgTypes = declaredArgTypes;
}

FunctionType *CallExpr::getFunctionType() const {
   return functionType;
}

void CallExpr::setFunctionType(FunctionType *functionType) {
   CallExpr::functionType = functionType;
}

bool CallExpr::isImplicitSelfCall() const {
   return implicitSelfCall;
}

void CallExpr::setImplicitSelfCall(bool implicitSelfCall) {
   CallExpr::implicitSelfCall = implicitSelfCall;
}

const string &CallExpr::getSelfBinding() const {
   return selfBinding;
}

void CallExpr::setSelfBinding(const string &selfBinding) {
   CallExpr::selfBinding = selfBinding;
}

bool CallExpr::isIsProtocolCall() const {
   return isProtocolCall;
}

void CallExpr::setIsProtocolCall(bool isProtocolCall) {
   CallExpr::isProtocolCall = isProtocolCall;
}

bool CallExpr::isCastToBase() const {
   return castToBase;
}

void CallExpr::setCastToBase(bool castToBase) {
   CallExpr::castToBase = castToBase;
}

size_t CallExpr::getAnonymousFieldIndex() const {
   return anonymousFieldIndex;
}

void CallExpr::setAnonymousFieldIndex(size_t anonymousFieldIndex) {
   CallExpr::anonymousFieldIndex = anonymousFieldIndex;
}

bool CallExpr::isUnionConstr() const {
   return unionConstr;
}

void CallExpr::setUnionConstr(bool unionConstr) {
   CallExpr::unionConstr = unionConstr;
}
