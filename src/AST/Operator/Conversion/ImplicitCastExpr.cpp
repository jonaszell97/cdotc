//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

ImplicitCastExpr::ImplicitCastExpr(Type& from, Type& to, Expression::SharedPtr target) :
   from(from), to(to), target(target)
{
   this->target->setParent(this);
   children.push_back(&this->target);
}

std::vector<AstNode::SharedPtr> ImplicitCastExpr::get_children() {
   return { target };
}

void ImplicitCastExpr::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "ImplicitCastExpr [" << from.toString() << " to " << to.toString() << "]" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}