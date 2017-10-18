//
// Created by Jonas Zell on 19.06.17.
//

#include "TertiaryOperator.h"

TertiaryOperator::TertiaryOperator(Expression::SharedPtr cond, Expression::SharedPtr if_br,
      Expression::SharedPtr else_br) :
   condition(cond),
   lhs(if_br),
   rhs(else_br)
{
   children.push_back(&condition);
   children.push_back(&lhs);
   children.push_back(&rhs);
}

void TertiaryOperator::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   if (condition.get() == child) {
      condition.reset(replacement);
   }

   if (lhs.get() == child) {
      lhs.reset(replacement);
   }

   if (rhs.get() == child) {
      rhs.reset(replacement);
   }

   if (memberExpr.get() == child) {
      memberExpr.reset(replacement);
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> TertiaryOperator::get_children() {
   std::vector<AstNode::SharedPtr> children { condition, lhs, rhs };
   if (memberExpr != nullptr) {
      children.push_back(memberExpr);
   }

   return children;
}

void TertiaryOperator::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "TertiaryOperator ['?:']" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}