//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_IMPLICITCASTEXPR_H
#define CDOT_IMPLICITCASTEXPR_H

#include "../../Expression/Expression.h"

class ImplicitCastExpr : public Expression {
public:
   ImplicitCastExpr(Type& from, Type& to, Expression::SharedPtr);

   typedef std::shared_ptr<ImplicitCastExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
     v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
     return v.visit(this);
   }

   NodeType get_type() override {
      return target->get_type();
   }

   void isHiddenReturnValue() override {
      sret_value = true;
      target->isHiddenReturnValue();
   }

   ADD_FRIEND_PASSES

protected:
   Type from;
   Type to;
   Expression::SharedPtr target;
};


#endif //CDOT_IMPLICITCASTEXPR_H
