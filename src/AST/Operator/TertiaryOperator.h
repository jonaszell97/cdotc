//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_TERTIARYOPERATOR_H
#define CDOT_TERTIARYOPERATOR_H

#include "../Expression/Expression.h"

class TertiaryOperator : public Expression {
public:
   TertiaryOperator(Expression::SharedPtr, Expression::SharedPtr, Expression::SharedPtr);
   ~TertiaryOperator() override;

   typedef std::shared_ptr<TertiaryOperator> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override ;

   NodeType get_type() override {
      return NodeType::TERTIARY_OPERATOR;
   }

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

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   Expression::SharedPtr condition;
   Expression::SharedPtr lhs;
   Expression::SharedPtr rhs;

   // codegen
   Type* resultType;
};


#endif //CDOT_TERTIARYOPERATOR_H
