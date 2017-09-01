//
// Created by Jonas Zell on 19.06.17.
//

#include "UnaryOperator.h"

UnaryOperator::UnaryOperator(string op, string fix) :
   op(op),
   prefix(fix == "prefix")
{

}

UnaryOperator::~UnaryOperator() {

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