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
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::ARRAY_ACCESS_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
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
   Expression::SharedPtr _index;

   // codegen
   bool is_subscript_op = false;
   std::shared_ptr<CallExpr> overridenCall;
};


#endif //CDOT_ARRAYACCESSEXPR_H
