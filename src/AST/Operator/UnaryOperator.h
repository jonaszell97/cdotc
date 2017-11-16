//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "../Expression/Expression.h"

class CallExpr;

namespace cdot {
namespace cl {
   struct Method;
}
}

class UnaryOperator : public Expression {
public:
   UnaryOperator(string, string);

   void setTarget(Expression::SharedPtr t) {
      target = t;
   }

   void isLhsOfAssigment() override {
      lhs_of_assignment = true;
      if (memberExpr != nullptr) {
         memberExpr->isLhsOfAssigment();
      }
      if (target != nullptr) {
         target->isLhsOfAssigment();
      }
   }

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<UnaryOperator> SharedPtr;

   NodeType get_type() override {
      return NodeType::UNARY_OPERATOR;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   Expression::SharedPtr target;
   string op;
   BuiltinType* operandType;
   bool prefix;

   // codegen
   std::shared_ptr<CallExpr> overridenCall;
   cdot::cl::Method* method;
   string className;

   bool isPointerArithmetic = false;
   bool needsDereferenceLoad = true;
};


#endif //CDOT_UNARYOPERATOR_H
