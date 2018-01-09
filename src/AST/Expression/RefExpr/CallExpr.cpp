//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"
#include "IdentifierRefExpr.h"

#include "../../SymbolTable.h"

#include "../../Passes/SemanticAnalysis/Record/Class.h"
#include "../../Passes/SemanticAnalysis/Record/Enum.h"
#include "../../Passes/SemanticAnalysis/Function.h"
#include "../../Passes/SemanticAnalysis/Builtin.h"

#include "../../../Variant/Type/FunctionType.h"

namespace cdot {
namespace ast {


CallExpr::CallExpr(std::vector<std::shared_ptr<Expression>> &&args,
                   string &&ident)
   : IdentifiedExpr(CallExprID, move(ident)), args(move(args)),
     is_pointer_access(false),
     implicitSelf(false),
     builtinFnKind(BuiltinFn::None)
{

}

void CallExpr::destroyValueImpl()
{
   if (kind == CallKind::AnonymousCall && identExpr)
      delete identExpr;
}

CallKind CallExpr::getKind() const {
   return kind;
}

void CallExpr::setKind(CallKind type) {
   CallExpr::kind = type;
}

bool CallExpr::isPointerAccess_() const {
   return is_pointer_access;
}

void CallExpr::setIsPointerAccess_(bool isPointerAccess_) {
   CallExpr::is_pointer_access = isPointerAccess_;
}

QualType &CallExpr::getReturnType()
{
   return returnType;
}

void CallExpr::setReturnType(const QualType &returnType) {
   CallExpr::returnType = returnType;
}

BuiltinFn CallExpr::getBuiltinFnKind() const {
   return builtinFnKind;
}

void CallExpr::setBuiltinFnKind(BuiltinFn builtinFnKind) {
   CallExpr::builtinFnKind = builtinFnKind;
}

Method *CallExpr::getMethod() const {
   return method;
}

void CallExpr::setMethod(Method *method)
{
   declaredArgTypes = method->getArguments();
   CallExpr::method = method;
}

std::vector<Argument> &CallExpr::getResolvedArgs()
{
   return resolvedArgs;
}

void CallExpr::setResolvedArgs(const std::vector<Argument> &resolvedArgs) {
   CallExpr::resolvedArgs = resolvedArgs;
}

FunctionType *CallExpr::getFunctionType() const {
   return functionType;
}

void CallExpr::setFunctionType(FunctionType *functionType)
{
   declaredArgTypes = functionType->getArgTypes();
   CallExpr::functionType = functionType;
}

Function *CallExpr::getFunc() const
{
   return func;
}

void CallExpr::setFunc(Function *func)
{
   declaredArgTypes = func->getArguments();
   CallExpr::func = func;
}

} // namespace ast
} // namespace cdot