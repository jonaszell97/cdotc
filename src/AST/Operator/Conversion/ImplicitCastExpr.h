//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_IMPLICITCASTEXPR_H
#define CDOT_IMPLICITCASTEXPR_H

#include "../../Expression/Expression.h"

class ImplicitCastExpr : public Expression {
public:
   ImplicitCastExpr(Type* from, Type* to, Expression::SharedPtr);

   ~ImplicitCastExpr() override;

   typedef std::shared_ptr<ImplicitCastExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass &v) override {
     v.visit(this);
   }

   Variant accept(ConstExprPass &v) override {
     return v.visit(this);
   }

   NodeType get_type() override {
      return target->get_type();
   }

   void isHiddenReturnValue() override {
      isHiddenReturnValue_ = true;
      target->isHiddenReturnValue();
   }

   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;
   friend class ConstExprPass;

protected:
   Type* from = nullptr;
   Type* to = nullptr;
   Expression::SharedPtr target;
};


#endif //CDOT_IMPLICITCASTEXPR_H
