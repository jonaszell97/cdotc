//
// Created by Jonas Zell on 19.06.17.
//

#include "BinaryOperator.h"

namespace cdot {
   BinaryOperatorType getBinaryOpType(string op) {
      if (!util::isAssignmentOperator(op).empty()) {
         return BinaryOperatorType::ASSIGNMENT;
      }
      if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "**") {
         return BinaryOperatorType::ARITHMETIC;
      }
      if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>" || op == ">>>") {
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

   BinaryOperator::BinaryOperator(string op) : op(op) {

   }

   BinaryOperator::BinaryOperator(string op, Expression::SharedPtr lhs, Expression::SharedPtr rhs) :
      op(op),
      lhs(lhs),
      rhs(rhs) {

   }

   void BinaryOperator::setLhs(Expression::SharedPtr exp) {
      lhs = exp;
      children.push_back(&lhs);
   }

   void BinaryOperator::setRhs(Expression::SharedPtr exp) {
      rhs = exp;
      children.push_back(&rhs);
   }

   void BinaryOperator::replaceChildWith(
      AstNode *child,
      Expression *replacement)
   {
      if (lhs.get() == child) {
         lhs.reset(replacement);
      }

      if (rhs.get() == child) {
         rhs.reset(replacement);
      }

      if (memberExpr.get() == child) {
         memberExpr.reset(replacement);
      }

      llvm_unreachable("child does not exist");
   }

   std::vector<AstNode::SharedPtr> BinaryOperator::get_children() {
      std::vector<AstNode::SharedPtr> res;
      res.push_back(lhs);
      res.push_back(rhs);
      if (memberExpr != nullptr) {
         res.push_back(memberExpr);
      }

      return res;
   }

   void BinaryOperator::__dump(int depth) {
      AstNode::__tab(depth);

      std::cout << "BinaryOperator ['" + op << "']" << std::endl;

      for (auto child : get_children()) {
         child->__dump(depth + 1);
      }
   }

   const Expression::SharedPtr &BinaryOperator::getLhs() const {
      return lhs;
   }

   const Expression::SharedPtr &BinaryOperator::getRhs() const {
      return rhs;
   }

   BuiltinType *BinaryOperator::getOperandType() const {
      return operandType;
   }

   void BinaryOperator::setOperandType(BuiltinType *operandType) {
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

   BuiltinType *BinaryOperator::getPointerArithmeticType() const {
      return pointerArithmeticType;
   }

   void BinaryOperator::setPointerArithmeticType(BuiltinType *pointerArithmeticType) {
      BinaryOperator::pointerArithmeticType = pointerArithmeticType;
   }

   BinaryOperator *BinaryOperator::getPreAssignmentOp() const {
      return preAssignmentOp;
   }

   void BinaryOperator::setPreAssignmentOp(BinaryOperator *preAssignmentOp) {
      BinaryOperator::preAssignmentOp = preAssignmentOp;
   }

   Type &BinaryOperator::getLhsType() {
      return lhsType;
   }

   void BinaryOperator::setLhsType(const Type &lhsType) {
      BinaryOperator::lhsType = lhsType;
   }

   const Type &BinaryOperator::getRhsType() const {
      return rhsType;
   }

   void BinaryOperator::setRhsType(const Type &rhsType) {
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

   void BinaryOperator::setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall) {
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

   bool BinaryOperator::isIsStructAssignment() const {
      return isStructAssignment;
   }

   void BinaryOperator::setIsStructAssignment(bool isStructAssignment) {
      BinaryOperator::isStructAssignment = isStructAssignment;
   }

   bool BinaryOperator::isIsSelfAssignment() const {
      return isSelfAssignment;
   }

   void BinaryOperator::setIsSelfAssignment(bool isSelfAssignment) {
      BinaryOperator::isSelfAssignment = isSelfAssignment;
   }

   bool BinaryOperator::isIsNullAssignment() const {
      return isNullAssignment;
   }

   void BinaryOperator::setIsNullAssignment(bool isNullAssignment) {
      BinaryOperator::isNullAssignment = isNullAssignment;
   }

   bool BinaryOperator::isIsProtocolAssignment() const {
      return isProtocolAssignment;
   }

   void BinaryOperator::setIsProtocolAssignment(bool isProtocolAssignment) {
      BinaryOperator::isProtocolAssignment = isProtocolAssignment;
   }

   bool BinaryOperator::isBoxedPrimitiveOp() const {
      return boxedPrimitiveOp;
   }

   void BinaryOperator::setBoxedPrimitiveOp(bool boxedPrimitiveOp) {
      BinaryOperator::boxedPrimitiveOp = boxedPrimitiveOp;
   }

   bool BinaryOperator::isLhsIsBoxed() const {
      return lhsIsBoxed;
   }

   void BinaryOperator::setLhsIsBoxed(bool lhsIsBoxed) {
      BinaryOperator::lhsIsBoxed = lhsIsBoxed;
   }

   bool BinaryOperator::isRhsIsBoxed() const {
      return rhsIsBoxed;
   }

   void BinaryOperator::setRhsIsBoxed(bool rhsIsBoxed) {
      BinaryOperator::rhsIsBoxed = rhsIsBoxed;
   }

   const string &BinaryOperator::getBoxedResultType() const {
      return boxedResultType;
   }

   void BinaryOperator::setBoxedResultType(const string &boxedResultType) {
      BinaryOperator::boxedResultType = boxedResultType;
   }

   bool BinaryOperator::isIsEnumComp() const {
      return isEnumComp;
   }

   void BinaryOperator::setIsEnumComp(bool isEnumComp) {
      BinaryOperator::isEnumComp = isEnumComp;
   }

   EnumCase *BinaryOperator::getKnownCase() const {
      return knownCase;
   }

   void BinaryOperator::setKnownCase(EnumCase *knownCase) {
      BinaryOperator::knownCase = knownCase;
   }

   const std::vector<Expression::SharedPtr> &BinaryOperator::getCaseValues() const {
      return caseValues;
   }

   void BinaryOperator::setCaseValues(const std::vector<Expression::SharedPtr> &caseValues) {
      BinaryOperator::caseValues = caseValues;
   }

   const std::vector<BuiltinType *> &BinaryOperator::getCaseTypes() const {
      return caseTypes;
   }

   void BinaryOperator::setCaseTypes(const std::vector<BuiltinType *> &caseTypes) {
      BinaryOperator::caseTypes = caseTypes;
   }

   bool BinaryOperator::isIsTupleComp() const {
      return isTupleComp;
   }

   void BinaryOperator::setIsTupleComp(bool isTupleComp) {
      BinaryOperator::isTupleComp = isTupleComp;
   }

   size_t BinaryOperator::getArity() const {
      return arity;
   }

   void BinaryOperator::setArity(size_t arity) {
      BinaryOperator::arity = arity;
   }

   const std::vector<BuiltinType *> &BinaryOperator::getTupleTypes() const {
      return tupleTypes;
   }

   void BinaryOperator::setTupleTypes(const std::vector<BuiltinType *> &tupleTypes) {
      BinaryOperator::tupleTypes = tupleTypes;
   }

   llvm::Type *BinaryOperator::getLlvmTupleType() const {
      return llvmTupleType;
   }

   void BinaryOperator::setLlvmTupleType(llvm::Type *llvmTupleType) {
      BinaryOperator::llvmTupleType = llvmTupleType;
   }

   bool BinaryOperator::isUpdateRefCount() const {
      return updateRefCount;
   }

   void BinaryOperator::setUpdateRefCount(bool updateRefCount) {
      BinaryOperator::updateRefCount = updateRefCount;
   }

}