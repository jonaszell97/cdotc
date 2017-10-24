//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H

#include "../Expression/Expression.h"

class CallExpr;
class ConstExprPass;

namespace cdot {

   enum class BinaryOperatorType : unsigned int {
      ARITHMETIC,
      ASSIGNMENT,
      EQUALITY,
      COMPARISON,
      BITWISE,
      LOGICAL,
      CAST,
      OTHER
   };

   BinaryOperatorType getBinaryOpType(string op);

   namespace cl {
      class Method;
   }
}

namespace cdot {

   class BinaryOperator : public Expression {
   public:
      explicit BinaryOperator(string);
      BinaryOperator(string, Expression::SharedPtr, Expression::SharedPtr);

      void setLhs(Expression::SharedPtr lhs);
      void setRhs(Expression::SharedPtr rhs);

      typedef std::shared_ptr<BinaryOperator> SharedPtr;

      std::vector<AstNode::SharedPtr> get_children() override;

      void __dump(int depth) override;

      NodeType get_type() override {
        return NodeType::BINARY_OPERATOR;
      }

      llvm::Value *accept(CodeGen &v) override {
        return v.visit(this);
      }

      Type accept(SemaPass &v) override {
        return v.visit(this);
      }

      void accept(AbstractPass* v) override {
        v->visit(this);
      }

      Variant accept(ConstExprPass &v) override {
        return v.visit(this);
      }

      void replaceChildWith(AstNode *child, Expression *replacement) override;

      ADD_FRIEND_PASSES

   protected:
      Expression::SharedPtr lhs;
      Expression::SharedPtr rhs;
      BuiltinType *operandType = nullptr;
      string op;
      cdot::BinaryOperatorType opType;

      BuiltinType *pointerArithmeticType = nullptr;

      BinaryOperator* preAssignmentOp = nullptr;
      Type lhsType;
      Type rhsType;

      llvm::Value* lhsVal = nullptr;
      llvm::Value* rhsVal = nullptr;

      // codegen
      std::shared_ptr<CallExpr> overridenCall = nullptr;
      string className;
      cdot::cl::Method *method;
      bool isStructAssignment = false;
      bool isSelfAssignment = false;
      bool isNullAssignment = false;
      bool isProtocolAssignment = false;

      bool boxedPrimitiveOp = false;
      bool lhsIsBoxed = false;
      bool rhsIsBoxed = false;
      string boxedResultType;

      // special comparison operators
      bool isEnumComp = false;
      EnumCase* knownCase = nullptr;
      std::vector<Expression::SharedPtr> caseValues;
      std::vector<BuiltinType*> caseTypes;

      bool isTupleComp = false;
      size_t arity;
      std::vector<BuiltinType*> tupleTypes;
      llvm::Type* llvmTupleType;

      bool updateRefCount = false;
      
   public:
      const Expression::SharedPtr &getLhs() const;

      const Expression::SharedPtr &getRhs() const;

      BuiltinType *getOperandType() const;

      void setOperandType(BuiltinType *operandType);

      const string &getOp() const;

      void setOp(const string &op);

      BinaryOperatorType getOpType() const;

      void setOpType(BinaryOperatorType opType);

      BuiltinType *getPointerArithmeticType() const;

      void setPointerArithmeticType(BuiltinType *pointerArithmeticType);

      BinaryOperator *getPreAssignmentOp() const;

      void setPreAssignmentOp(BinaryOperator *preAssignmentOp);

      Type &getLhsType();

      void setLhsType(const Type &lhsType);

      const Type &getRhsType() const;

      void setRhsType(const Type &rhsType);

      llvm::Value *getLhsVal() const;

      void setLhsVal(llvm::Value *lhsVal);

      llvm::Value *getRhsVal() const;

      void setRhsVal(llvm::Value *rhsVal);

      const std::shared_ptr<CallExpr> &getOverridenCall() const;

      void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall);

      const string &getClassName() const;

      void setClassName(const string &className);

      Method *getMethod() const;

      void setMethod(Method *method);

      bool isIsStructAssignment() const;

      void setIsStructAssignment(bool isStructAssignment);

      bool isIsSelfAssignment() const;

      void setIsSelfAssignment(bool isSelfAssignment);

      bool isIsNullAssignment() const;

      void setIsNullAssignment(bool isNullAssignment);

      bool isIsProtocolAssignment() const;

      void setIsProtocolAssignment(bool isProtocolAssignment);

      bool isBoxedPrimitiveOp() const;

      void setBoxedPrimitiveOp(bool boxedPrimitiveOp);

      bool isLhsIsBoxed() const;

      void setLhsIsBoxed(bool lhsIsBoxed);

      bool isRhsIsBoxed() const;

      void setRhsIsBoxed(bool rhsIsBoxed);

      const string &getBoxedResultType() const;

      void setBoxedResultType(const string &boxedResultType);

      bool isIsEnumComp() const;

      void setIsEnumComp(bool isEnumComp);

      EnumCase *getKnownCase() const;

      void setKnownCase(EnumCase *knownCase);

      const std::vector<Expression::SharedPtr> &getCaseValues() const;

      void setCaseValues(const std::vector<Expression::SharedPtr> &caseValues);

      const std::vector<BuiltinType *> &getCaseTypes() const;

      void setCaseTypes(const std::vector<BuiltinType *> &caseTypes);

      bool isIsTupleComp() const;

      void setIsTupleComp(bool isTupleComp);

      size_t getArity() const;

      void setArity(size_t arity);

      const std::vector<BuiltinType *> &getTupleTypes() const;

      void setTupleTypes(const std::vector<BuiltinType *> &tupleTypes);

      llvm::Type *getLlvmTupleType() const;

      void setLlvmTupleType(llvm::Type *llvmTupleType);

      bool isUpdateRefCount() const;

      void setUpdateRefCount(bool updateRefCount);
   };

}


#endif //CDOT_BINARYOPERATOR_H
