//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H

#include "../Expression.h"
#include "../../../Variant/Variant.h"

class NumericLiteral : public Expression {
public:
   NumericLiteral(cdot::Variant);
   ~NumericLiteral();

   bool needsContextualInformation() override {
      return true;
   }

   void saveOrResetState() override;

   typedef std::shared_ptr<NumericLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::LITERAL_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override{
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   cdot::Variant value;
   Type* type = nullptr;
   string className;

   bool isChar = false;
   bool isBool = false;

   // codegen
   bool isPrimitive = false;
};


#endif //CDOT_LITERALEXPRESSION_H
