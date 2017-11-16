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

   NodeType get_type() override {
      return NodeType::TUPLE_LITERAL;
   }

   bool createsTemporary() override
   {
      return true;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::vector<pair<string, Expression::SharedPtr>> elements;

   // codegen
   TupleType* tupleType;
   bool is_meta_ty = false;

public:
   bool isMetaTy() const;
   void isMetaTy(bool is_meta_ty);

   const std::vector<pair<string, std::shared_ptr<Expression>>>
   &getElements() const;
   void setElements(
      const std::vector<pair<string, std::shared_ptr<Expression>>> &elements);

   TupleType *getTupleType() const;
   void setTupleType(TupleType *tupleType);
};


#endif //CDOT_TUPLELITERAL_H
