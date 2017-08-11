//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "TertiaryOperator.h"

TertiaryOperator::TertiaryOperator(Expression::SharedPtr cond, Expression::SharedPtr if_br,
                                   Expression::SharedPtr else_br) :
    condition(cond),
    lhs(if_br),
    rhs(else_br)
{

}

std::vector<AstNode::SharedPtr> TertiaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> children { condition, lhs, rhs };
    if (_member_expr != nullptr) {
        children.push_back(_member_expr);
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