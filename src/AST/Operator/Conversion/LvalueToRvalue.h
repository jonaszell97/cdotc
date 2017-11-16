//
// Created by Jonas Zell on 15.08.17.
//

#ifndef CDOT_LVALUETORVALUECONV_H
#define CDOT_LVALUETORVALUECONV_H


#include "../../Expression/Expression.h"
using namespace cdot;

class LvalueToRvalue : public Expression {
public:
   explicit LvalueToRvalue(Expression::SharedPtr expr);

   typedef std::shared_ptr<LvalueToRvalue> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::LVALUE_TO_RVALUE;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   Expression::SharedPtr target;
};


#endif //CDOT_LVALUETORVALUECONV_H
