//
// Created by Jonas Zell on 10.09.17.
//

#ifndef CDOT_STRINGINTERPOLATION_H
#define CDOT_STRINGINTERPOLATION_H


#include "../Expression.h"

class StringInterpolation: public Expression {
public:
   explicit StringInterpolation(std::vector<Expression::SharedPtr>&& strings);

   typedef std::shared_ptr<StringInterpolation> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   inline virtual NodeType get_type() override {
      return NodeType::STRING_INTERPOLATION;
   }

   bool createsTemporary() override
   {
      return true;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::vector<Expression::SharedPtr> strings;

public:
   const std::vector<Expression::SharedPtr> &getStrings() const
   {
      return strings;
   }

   void setStrings(const std::vector<Expression::SharedPtr> &strings)
   {
      StringInterpolation::strings = strings;
   }
};


#endif //CDOT_STRINGINTERPOLATION_H
