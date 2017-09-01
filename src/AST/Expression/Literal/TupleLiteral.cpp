//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleLiteral.h"
#include "../../../Variant/Type/TupleType.h"

TupleLiteral::TupleLiteral(std::vector<pair<string, Expression::SharedPtr>> elements) :
   elements(elements)
{

}

TupleLiteral::~TupleLiteral() {
   delete tupleType;
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