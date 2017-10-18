//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLELITERAL_H
#define CDOT_TUPLELITERAL_H


#include "../Expression.h"

namespace cdot {
   class TupleType;
}

class TupleLiteral: public Expression {
public:
   explicit TupleLiteral(std::vector<pair<string, Expression::SharedPtr>> elements);

   typedef std::shared_ptr<TupleLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::TUPLE_LITERAL;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override
   {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override
   {
      return v.visit(this);
   }

   bool createsTemporary() override
   {
      return true;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ADD_FRIEND_PASSES

protected:
   std::vector<pair<string, Expression::SharedPtr>> elements;

   // codegen
   TupleType* tupleType;
};


#endif //CDOT_TUPLELITERAL_H
