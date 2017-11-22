//
// Created by Jonas Zell on 19.06.17.
//

#include "BinaryOperator.h"
#include "../../lex/Lexer.h"

using namespace cdot::cl;

namespace cdot {
namespace ast {

BinaryOperatorType getBinaryOpType(const string &op) {
   if (!util::isAssignmentOperator(op).empty()) {
      return BinaryOperatorType::ASSIGNMENT;
   }
   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%"
       || op == "**") {
      return BinaryOperatorType::ARITHMETIC;
   }
   if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>"
       || op == ">>>") {
      return BinaryOperatorType::BITWISE;
   }
   if (op == "&&" || op == "||") {
      return BinaryOperatorType::LOGICAL;
   }
   if (op == "==" || op == "!=" || op == "===" || op == "!==") {
      return BinaryOperatorType::EQUALITY;
   }
   if (op == "<=" || op == ">=" || op == "<" || op == ">") {
      return BinaryOperatorType::COMPARISON;
   }
   if (op == "as" || op == "as!") {
      return BinaryOperatorType::CAST;
   }

   return BinaryOperatorType::OTHER;
}

BinaryOperator::BinaryOperator(string &&op)
   : Expression(BinaryOperatorID), op(op)
{

}

BinaryOperator::BinaryOperator(string &&op,
                               Expression::SharedPtr &&lhs,
                               Expression::SharedPtr &&rhs)
   : Expression(BinaryOperatorID), op(move(op)), lhs(move(lhs)), rhs(move(rhs)) {

}

void BinaryOperator::setLhs(Expression::SharedPtr exp) {
   lhs = exp;
}

void BinaryOperator::setRhs(Expression::SharedPtr exp) {
   rhs = exp;
}

Expression::SharedPtr &BinaryOperator::getLhs()
{
   return lhs;
}

Expression::SharedPtr &BinaryOperator::getRhs()
{
   return rhs;
}

Type *BinaryOperator::getOperandType() const {
   return operandType;
}

void BinaryOperator::setOperandType(Type *operandType) {
   BinaryOperator::operandType = operandType;
}

const string &BinaryOperator::getOp() const {
   return op;
}

void BinaryOperator::setOp(const string &op) {
   BinaryOperator::op = op;
}

BinaryOperatorType BinaryOperator::getOpType() const {
   return opType;
}

void BinaryOperator::setOpType(BinaryOperatorType opType) {
   BinaryOperator::opType = opType;
}

Type *BinaryOperator::getPointerArithmeticType() const {
   return pointerArithmeticType;
}

void BinaryOperator::setPointerArithmeticType(Type *pointerArithmeticType) {
   BinaryOperator::pointerArithmeticType = pointerArithmeticType;
}

BinaryOperator *BinaryOperator::getPreAssignmentOp() const {
   return preAssignmentOp;
}

void BinaryOperator::setPreAssignmentOp(BinaryOperator *preAssignmentOp) {
   BinaryOperator::preAssignmentOp = preAssignmentOp;
}

QualType &BinaryOperator::getLhsType() {
   return lhsType;
}

void BinaryOperator::setLhsType(const QualType &lhsType) {
   BinaryOperator::lhsType = lhsType;
}

const QualType &BinaryOperator::getRhsType() const {
   return rhsType;
}

void BinaryOperator::setRhsType(const QualType &rhsType) {
   BinaryOperator::rhsType = rhsType;
}

llvm::Value *BinaryOperator::getLhsVal() const {
   return lhsVal;
}

void BinaryOperator::setLhsVal(llvm::Value *lhsVal) {
   BinaryOperator::lhsVal = lhsVal;
}

llvm::Value *BinaryOperator::getRhsVal() const {
   return rhsVal;
}

void BinaryOperator::setRhsVal(llvm::Value *rhsVal) {
   BinaryOperator::rhsVal = rhsVal;
}

const std::shared_ptr<CallExpr> &BinaryOperator::getOverridenCall() const {
   return overridenCall;
}

void BinaryOperator::setOverridenCall(
   const std::shared_ptr<CallExpr> &overridenCall) {
   BinaryOperator::overridenCall = overridenCall;
}

const string &BinaryOperator::getClassName() const {
   return className;
}

void BinaryOperator::setClassName(const string &className) {
   BinaryOperator::className = className;
}

Method *BinaryOperator::getMethod() const {
   return method;
}

void BinaryOperator::setMethod(Method *method) {
   BinaryOperator::method = method;
}

bool BinaryOperator::needsMemCpy() const {
   return needs_memcpy;
}

void BinaryOperator::needsMemCpy(bool isStructAssignment) {
   BinaryOperator::needs_memcpy = isStructAssignment;
}

bool BinaryOperator::isSelfAssignment() const {
   return is_self_assignment;
}

void BinaryOperator::isSelfAssignment(bool isSelfAssignment) {
   BinaryOperator::is_self_assignment = isSelfAssignment;
}

bool BinaryOperator::isNullAssignment() const {
   return is_null_assignment;
}

void BinaryOperator::isNullAssignment(bool isNullAssignment) {
   BinaryOperator::is_null_assignment = isNullAssignment;
}

bool BinaryOperator::isProtocolAssignment() const {
   return is_protocol_assignment;
}

void BinaryOperator::isProtocolAssignment(bool isProtocolAssignment) {
   BinaryOperator::is_protocol_assignment = isProtocolAssignment;
}

bool BinaryOperator::isBoxedPrimitiveOp() const {
   return boxed_primitive_op;
}

void BinaryOperator::isBoxedPrimitiveOp(bool boxedPrimitiveOp) {
   BinaryOperator::boxed_primitive_op = boxedPrimitiveOp;
}

bool BinaryOperator::lhsIsBoxed() const {
   return lhs_is_boxed;
}

void BinaryOperator::lhsIsBoxed(bool lhsIsBoxed) {
   BinaryOperator::lhs_is_boxed = lhsIsBoxed;
}

bool BinaryOperator::rhsIsBoxed() const {
   return rhs_is_boxed;
}

void BinaryOperator::rhsIsBoxed(bool rhsIsBoxed) {
   BinaryOperator::rhs_is_boxed = rhsIsBoxed;
}

const string &BinaryOperator::getBoxedResultType() const {
   return boxedResultType;
}

void BinaryOperator::setBoxedResultType(const string &boxedResultType) {
   BinaryOperator::boxedResultType = boxedResultType;
}

bool BinaryOperator::isEnumComp() const {
   return is_enum_comp;
}

void BinaryOperator::isEnumComp(bool isEnumComp) {
   BinaryOperator::is_enum_comp = isEnumComp;
}

bool BinaryOperator::isTupleComp() const {
   return is_tuple_comp;
}

void BinaryOperator::isTupleComp(bool isTupleComp) {
   BinaryOperator::is_tuple_comp = isTupleComp;
}

bool BinaryOperator::updateRefCount() const {
   return update_refcount;
}

void BinaryOperator::updateRefCount(bool updateRefCount) {
   BinaryOperator::update_refcount = updateRefCount;
}

} // namespace ast
} // namespace cdot