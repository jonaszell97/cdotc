//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYACCESSEXPR_H
#define CDOT_ARRAYACCESSEXPR_H

#include "../Expression.h"

class CallExpr;

class SubscriptExpr : public Expression {
public:
   explicit SubscriptExpr(Expression::SharedPtr);

   typedef std::shared_ptr<SubscriptExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::ARRAY_ACCESS_EXPR;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   Expression::SharedPtr _index;

   // codegen
   bool is_subscript_op = false;
   std::shared_ptr<CallExpr> overridenCall;
};


#endif //CDOT_ARRAYACCESSEXPR_H
