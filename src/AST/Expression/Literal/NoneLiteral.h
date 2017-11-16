//
// Created by Jonas Zell on 30.08.17.
//

#ifndef CDOT_NONELITERAL_H
#define CDOT_NONELITERAL_H


#include "../Expression.h"

class NoneLiteral: public Expression {
public:
   NoneLiteral();

   bool needsContextualInformation() const override
   {
      return true;
   }

   typedef std::shared_ptr<NoneLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::NONE_LITERAL;
   }

   bool createsTemporary() override
   {
      return true;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES
};


#endif //CDOT_NONELITERAL_H
