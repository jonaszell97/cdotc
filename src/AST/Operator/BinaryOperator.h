//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H

#include "../Expression/Expression.h"

namespace cdot {

namespace cl {
class Method;
} // namespace cl

namespace ast {

class CallExpr;
class ConstExprPass;

enum class BinaryOperatorType: unsigned int {
   ARITHMETIC,
   ASSIGNMENT,
   EQUALITY,
   COMPARISON,
   BITWISE,
   LOGICAL,
   CAST,
   OTHER
};

BinaryOperatorType getBinaryOpType(const string &op);

class BinaryOperator: public Expression {
public:
   explicit BinaryOperator(string &&op);
   BinaryOperator(string &&op, Expression::SharedPtr &&lhs,
                  Expression::SharedPtr &&rhs);

   void setLhs(Expression::SharedPtr lhs);
   void setRhs(Expression::SharedPtr rhs);

   typedef std::shared_ptr<BinaryOperator> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == BinaryOperatorID;
   }

protected:
   Expression::SharedPtr lhs;
   Expression::SharedPtr rhs;
   Type *operandType = nullptr;
   string op;
   BinaryOperatorType opType;

   Type *pointerArithmeticType = nullptr;

   BinaryOperator *preAssignmentOp = nullptr;
   QualType lhsType;
   QualType rhsType;

   llvm::Value *lhsVal = nullptr;
   llvm::Value *rhsVal = nullptr;

   // codegen
   std::shared_ptr<CallExpr> overridenCall = nullptr;
   string className;
   cdot::cl::Method *method;
   bool needs_memcpy = false;
   bool is_self_assignment = false;
   bool is_null_assignment = false;
   bool is_protocol_assignment = false;

   bool boxed_primitive_op = false;
   bool lhs_is_boxed = false;
   bool rhs_is_boxed = false;
   string boxedResultType;

   bool is_enum_comp = false;
   bool is_tuple_comp = false;
   bool update_refcount = false;

public:
   Expression::SharedPtr &getLhs();

   Expression::SharedPtr &getRhs();

   Type *getOperandType() const;

   void setOperandType(Type *operandType);

   const string &getOp() const;

   void setOp(const string &op);

   BinaryOperatorType getOpType() const;

   void setOpType(BinaryOperatorType opType);

   Type *getPointerArithmeticType() const;

   void setPointerArithmeticType(Type *pointerArithmeticType);

   BinaryOperator *getPreAssignmentOp() const;

   void setPreAssignmentOp(BinaryOperator *preAssignmentOp);

   QualType &getLhsType();

   void setLhsType(const QualType &lhsType);

   const QualType &getRhsType() const;

   void setRhsType(const QualType &rhsType);

   llvm::Value *getLhsVal() const;

   void setLhsVal(llvm::Value *lhsVal);

   llvm::Value *getRhsVal() const;

   void setRhsVal(llvm::Value *rhsVal);

   const std::shared_ptr<CallExpr> &getOverridenCall() const;

   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall);

   const string &getClassName() const;

   void setClassName(const string &className);

   cdot::cl::Method *getMethod() const;
   void setMethod(cdot::cl::Method *method);

   bool needsMemCpy() const;

   void needsMemCpy(bool isStructAssignment);

   bool isSelfAssignment() const;

   void isSelfAssignment(bool isSelfAssignment);

   bool isNullAssignment() const;

   void isNullAssignment(bool isNullAssignment);

   bool isProtocolAssignment() const;

   void isProtocolAssignment(bool isProtocolAssignment);

   bool isBoxedPrimitiveOp() const;

   void isBoxedPrimitiveOp(bool boxedPrimitiveOp);

   bool lhsIsBoxed() const;

   void lhsIsBoxed(bool lhsIsBoxed);

   bool rhsIsBoxed() const;

   void rhsIsBoxed(bool rhsIsBoxed);

   const string &getBoxedResultType() const;

   void setBoxedResultType(const string &boxedResultType);

   bool isEnumComp() const;

   void isEnumComp(bool isEnumComp);

   bool isTupleComp() const;

   void isTupleComp(bool isTupleComp);

   bool updateRefCount() const;

   void updateRefCount(bool updateRefCount);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_BINARYOPERATOR_H
