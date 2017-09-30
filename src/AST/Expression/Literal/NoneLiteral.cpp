//
// Created by Jonas Zell on 30.08.17.
//

#include "NoneLiteral.h"

NoneLiteral::NoneLiteral() {

}

void NoneLiteral::saveOrResetState()
{
   if (prevState == nullptr) {
      prevState = new NoneLiteral(*this);
   }
   else {
      assert(prevState->get_type() == NodeType::NONE_LITERAL && "Not a none literal");
      *this = *static_cast<NoneLiteral *>(prevState);
   }
}

std::vector<AstNode::SharedPtr> NoneLiteral::get_children() {
   return {};
}

void NoneLiteral::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "NoneLiteral\n";
}