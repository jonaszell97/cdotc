//
// Created by Jonas Zell on 19.06.17.
//

#include "UnaryOperator.h"

namespace cdot {
namespace ast {

UnaryOperator::UnaryOperator(string &&op, string &&fix)
   : Expression(UnaryOperatorID), op(move(op)), prefix(fix == "prefix")
{

}

Expression::SharedPtr &UnaryOperator::getTarget()
{
   return target;
}

const string &UnaryOperator::getOp() const
{
   return op;
}

void UnaryOperator::setOp(const string &op)
{
   UnaryOperator::op = op;
}

Type *UnaryOperator::getOperandType() const
{
   return operandType;
}

void UnaryOperator::setOperandType(Type *operandType)
{
   UnaryOperator::operandType = operandType;
}

bool UnaryOperator::isPrefix() const
{
   return prefix;
}

void UnaryOperator::setPrefix(bool prefix)
{
   UnaryOperator::prefix = prefix;
}

const std::shared_ptr<CallExpr> &UnaryOperator::getOverridenCall() const
{
   return overridenCall;
}

void
UnaryOperator::setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall)
{
   UnaryOperator::overridenCall = overridenCall;
}

cl::Method *UnaryOperator::getMethod() const
{
   return method;
}

void UnaryOperator::setMethod(cl::Method *method)
{
   UnaryOperator::method = method;
}

const string &UnaryOperator::getClassName() const
{
   return className;
}

void UnaryOperator::setClassName(const string &className)
{
   UnaryOperator::className = className;
}

bool UnaryOperator::isIsPointerArithmetic() const
{
   return isPointerArithmetic;
}

void UnaryOperator::setIsPointerArithmetic(bool isPointerArithmetic)
{
   UnaryOperator::isPointerArithmetic = isPointerArithmetic;
}

bool UnaryOperator::isNeedsDereferenceLoad() const
{
   return needsDereferenceLoad;
}

void UnaryOperator::setNeedsDereferenceLoad(bool needsDereferenceLoad)
{
   UnaryOperator::needsDereferenceLoad = needsDereferenceLoad;
}

} // namespace ast
} // namespace cdot