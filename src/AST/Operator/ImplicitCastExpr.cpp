//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

#include <iostream>

ImplicitCastExpr::ImplicitCastExpr(TypeSpecifier from, TypeSpecifier to, Expression::SharedPtr target) :
    from(from), to(to), target(target)
{

}

std::vector<AstNode::SharedPtr> ImplicitCastExpr::get_children() {
    return { target };
}

void ImplicitCastExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ImplicitCastExpr [" << from.to_string() << " -> " << to.to_string() << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}