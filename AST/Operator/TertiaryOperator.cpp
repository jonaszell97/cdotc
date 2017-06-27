//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "TertiaryOperator.h"

TertiaryOperator::TertiaryOperator(Expression::SharedPtr cond, Expression::SharedPtr if_br,
                                   Expression::SharedPtr else_br) :
    _condition(cond),
    _if_branch(if_br),
    _else_branch(else_br)
{

}

TertiaryOperator::TertiaryOperator(const TertiaryOperator& cp) {
    _condition = std::static_pointer_cast<Expression>(cp._condition->clone());
    _if_branch = std::static_pointer_cast<Expression>(cp._if_branch->clone());
    _else_branch = std::static_pointer_cast<Expression>(cp._else_branch->clone());
    set_parent(cp._parent);
}

AstNode::SharedPtr TertiaryOperator::clone() const {
    return std::make_shared<TertiaryOperator>(*this);
}

std::vector<AstNode::SharedPtr> TertiaryOperator::get_children() {
    return { _condition, _if_branch, _else_branch };
}

void TertiaryOperator::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "TertiaryOperator ['?:']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}