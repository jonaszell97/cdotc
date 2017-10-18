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

void UnaryOperator::__dump(int depth) {
   for (int i = 0; i < depth; i++) {
      std::cout << "\t";
   }

   std::cout << "UnaryOperator ['" << op << "']" << std::endl;

   for (auto child : get_children()) {
      child->__dump(depth + 1);
   }
}