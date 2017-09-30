//
// Created by Jonas Zell on 10.09.17.
//

#ifndef CDOT_STRINGINTERPOLATION_H
#define CDOT_STRINGINTERPOLATION_H


#include "../Expression.h"

class StringInterpolation: public Expression {
public:
   StringInterpolation(std::vector<Expression::SharedPtr>&& strings);

   typedef std::shared_ptr<StringInterpolation> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   inline virtual NodeType get_type() override {
      return NodeType::STRING_INTERPOLATION;
   }

   ADD_FRIEND_PASSES

protected:
   std::vector<Expression::SharedPtr> strings;
};


#endif //CDOT_STRINGINTERPOLATION_H
