//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

ImplicitCastExpr::ImplicitCastExpr(const Type& from, const Type& to,
                                   Expression::SharedPtr target)
   : from(from), to(to), target(target)
{
   this->target->setParent(this);
   children.push_back(&this->target);
}

std::vector<AstNode::SharedPtr> ImplicitCastExpr::get_children() {
   return { target };
}