//
// Created by Jonas Zell on 19.06.17.
//

#include "UnaryOperator.h"

UnaryOperator::UnaryOperator(string op, string fix) :
   op(op),
   prefix(fix == "prefix")
{

}

void UnaryOperator::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   if (target.get() == child) {
      target.reset(replacement);
   }

   if (memberExpr.get() == child) {
      memberExpr.reset(replacement);
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> UnaryOperator::get_children() {
   std::vector<AstNode::SharedPtr> res;
   res.push_back(target);
   if (memberExpr != nullptr) {
      res.push_back(memberExpr);
   }

   return res;
}