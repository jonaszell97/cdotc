//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H

#include "../Expression/Expression.h"

class CallExpr;
class ConstExprVisitor;

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

      ~BinaryOperator() override;

      void setLhs(Expression::SharedPtr lhs);

      void setRhs(Expression::SharedPtr rhs);

      typedef std::shared_ptr<BinaryOperator> SharedPtr;

      std::vector<AstNode::SharedPtr> get_children() override;

      void __dump(int depth) override;

      NodeType get_type() override {
         return NodeType::BINARY_OPERATOR;
      }

      llvm::Value *accept(CodeGenVisitor &v) override {
         return v.visit(this);
      }

      Type *accept(TypeCheckVisitor &v) override {
         return v.visit(this);
      }

      friend class ::ConstExprVisitor;

      friend class ::CodeGenVisitor;

      friend class ::TypeCheckVisitor;

   protected:
      Expression::SharedPtr lhs;
      Expression::SharedPtr rhs;
      Type *operandType = nullptr;
      string op;
      cdot::BinaryOperatorType opType;

      Type *pointerArithmeticType = nullptr;

      // codegen
      std::shared_ptr<CallExpr> overridenCall = nullptr;
      string className;
      cdot::cl::Method *method;
      bool isStructAssignment = false;
      bool isNullAssignment = false;

      // special comparison operators
      bool isEnumComp = false;
      EnumCase* knownCase = nullptr;
      std::vector<Expression::SharedPtr> caseValues;
      std::vector<Type*> caseTypes;

      bool isTupleComp = false;
      size_t arity;
      std::vector<Type*> tupleTypes;
      llvm::Type* llvmTupleType;
   };

}


#endif //CDOT_BINARYOPERATOR_H
