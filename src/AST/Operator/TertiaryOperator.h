//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_TERTIARYOPERATOR_H
#define CDOT_TERTIARYOPERATOR_H

#include "../Expression/Expression.h"

class TertiaryOperator : public Expression {
public:
   TertiaryOperator(Expression::SharedPtr, Expression::SharedPtr, Expression::SharedPtr);

   typedef std::shared_ptr<TertiaryOperator> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::TERTIARY_OPERATOR;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   Expression::SharedPtr condition;
   Expression::SharedPtr lhs;
   Expression::SharedPtr rhs;

   // codegen
   BuiltinType* resultType;
};


#endif //CDOT_TERTIARYOPERATOR_H
