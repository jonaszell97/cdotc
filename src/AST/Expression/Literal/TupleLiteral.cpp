//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleLiteral.h"
#include "../../../Variant/Type/TupleType.h"

TupleLiteral::TupleLiteral(std::vector<pair<string, Expression::SharedPtr>> elements) :
   elements(elements)
{

}

void TupleLiteral::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   for (auto &el : elements) {
      if (el.second.get() == child) {
         el.second.reset(replacement);
         return;
      }
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> TupleLiteral::get_children() {
   std::vector<AstNode::SharedPtr> children;
   for (auto& cont : elements) {
      children.push_back(cont.second);
   }
   if (memberExpr != nullptr) {
      children.push_back(memberExpr);
   }

   return children;
}

void TupleLiteral::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "TupleLiteral\n";
   for (const auto& child : get_children()) {
      child->__dump(depth + 1);
   }
}