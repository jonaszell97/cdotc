//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"
#include "../../SymbolTable.h"
#include "../../Passes/SemanticAnalysis/Record/Class.h"
#include "../../Passes/SemanticAnalysis/Record/Enum.h"

namespace cdot {
namespace ast {

CallExpr::CallExpr(CallType type,
                   std::vector<pair<string, Expression::SharedPtr>> &&args,
                   string &&_ident)
   : Expression(CallExprID), type(type), args(args),
     templateArgs(new ResolvedTemplateArgList({}))
{
   ident = std::move(_ident);
}

CallExpr::CallExpr(CallType type,
                   std::vector<Expression::SharedPtr> &&args,
                   string &&_ident)
   : Expression(CallExprID), type(type),
     templateArgs(new ResolvedTemplateArgList({}))
{
   ident = std::move(_ident);
   for (const auto &arg : args) {
      this->args.emplace_back("", move(arg));
   }
}

bool CallExpr::createsTemporary()
{
   return returnType->isObjectTy();
}

CallType CallExpr::getType() const {
   return type;
}

void CallExpr::setType(CallType type) {
   CallExpr::type = type;
}

std::vector<pair<string, Expression::SharedPtr>> &
CallExpr::getArgs()
{
   return args;
}

void CallExpr::setArgs(
   const std::vector<pair<string, Expression::SharedPtr>> &args) {
   CallExpr::args = args;
}

bool CallExpr::isCallOp() const {
   return is_call_op;
}

void CallExpr::isCallOp(bool isCallOp) {
   CallExpr::is_call_op = isCallOp;
}

const string &CallExpr::getCallOpBinding() const {
   return callOpBinding;
}

void CallExpr::setCallOpBinding(const string &callOpBinding) {
   CallExpr::callOpBinding = callOpBinding;
}

bool CallExpr::isCapturedVar() const {
   return is_captured_var;
}

void CallExpr::setIsCapturedVar(bool isCapturedVar) {
   CallExpr::is_captured_var = isCapturedVar;
}

bool CallExpr::hasStructReturn() const {
   return has_struct_return;
}

void CallExpr::setHasStructReturn(bool hasStructReturn) {
   CallExpr::has_struct_return = hasStructReturn;
}

bool CallExpr::isPointerAccess_() const {
   return is_pointer_access;
}

void CallExpr::setIsPointerAccess_(bool isPointerAccess_) {
   CallExpr::is_pointer_access = isPointerAccess_;
}

CallCompatability *CallExpr::getCompatibility() const {
   return compatibility;
}

void CallExpr::setCompatibility(CallCompatability *compatibility) {
   CallExpr::compatibility = compatibility;
}

bool CallExpr::isNsMember() const {
   return is_ns_member;
}

void CallExpr::setIsNsMember(bool isNsMember) {
   CallExpr::is_ns_member = isNsMember;
}

bool CallExpr::isStatic() const {
   return is_static;
}

void CallExpr::setIsStatic(bool isStatic) {
   CallExpr::is_static = isStatic;
}

QualType &CallExpr::getReturnType()
{
   return returnType;
}

void CallExpr::setReturnType(const QualType &returnType) {
   CallExpr::returnType = returnType;
}

bool CallExpr::isVirtual() const {
   return is_virtual;
}

void CallExpr::setIsVirtual(bool is_virtual) {
   CallExpr::is_virtual = is_virtual;
}

bool CallExpr::isBuiltin() const {
   return is_builtin;
}

void CallExpr::setIsBuiltin(bool isBuiltin) {
   CallExpr::is_builtin = isBuiltin;
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

bool CallExpr::isAnonymousCall() const {
   return anonymous_call;
}

void CallExpr::setIsAnonymousCall(bool isAnonymousCall_) {
   CallExpr::anonymous_call = isAnonymousCall_;
}

std::vector<Argument> &CallExpr::getResolvedArgs()
{
   return resolvedArgs;
}

void CallExpr::setResolvedArgs(const std::vector<Argument> &resolvedArgs) {
   CallExpr::resolvedArgs = resolvedArgs;
}

std::vector<Argument> &CallExpr::getDeclaredArgTypes() {
   return declaredArgTypes;
}

void CallExpr::setDeclaredArgTypes(std::vector<Argument> &&declaredArgTypes) {
   CallExpr::declaredArgTypes = std::move(declaredArgTypes);
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

bool CallExpr::isProtocolCall() const {
   return is_protocol_call;
}

void CallExpr::setIsProtocolCall(bool isProtocolCall) {
   CallExpr::is_protocol_call = isProtocolCall;
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

TemplateArgList *&CallExpr::getTemplateArgs()
{
   return templateArgs;
}

void CallExpr::setTemplateArgs(TemplateArgList *templateArgs)
{
   delete CallExpr::templateArgs;
   CallExpr::templateArgs = templateArgs;
}

bool CallExpr::loadBeforeCall() const
{
   return load_before_call;
}

void CallExpr::loadBeforeCall(bool load_before_call)
{
   CallExpr::load_before_call = load_before_call;
}

Function *CallExpr::getFunc() const
{
   return func;
}

} // namespace ast
} // namespace cdot