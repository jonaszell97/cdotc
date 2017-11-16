//
// Created by Jonas Zell on 21.06.17.
//

#include "SubscriptExpr.h"

SubscriptExpr::SubscriptExpr(Expression::SharedPtr expr) {
   _index = expr;
   children.push_back(&_index);
}

void SubscriptExpr::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   if (memberExpr.get() == child) {
      memberExpr.reset(replacement);
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> SubscriptExpr::get_children() {
   std::vector<AstNode::SharedPtr> res;
   res.push_back(_index);
   if ( memberExpr != nullptr) {
      res.push_back(memberExpr);
   }

   return res;
}