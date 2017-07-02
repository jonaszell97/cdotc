//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "Expression.h"

#include <iostream>

Expression::Expression(Expression::SharedPtr child) : _child(child) {

}

Expression::Expression() : _child{} {

}

Expression::Expression(const Expression& cp) {
    _child = std::static_pointer_cast<Expression>(cp.clone());
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr Expression::clone() const {
    return std::make_shared<Expression>(*this);
}

void Expression::set_child(Expression::SharedPtr child) {
    _child = child;
}

std::vector<AstNode::SharedPtr> Expression::get_children() {
    return std::vector<AstNode::SharedPtr> { _child };
}

void Expression::__dump(int depth) {
    for (auto c : get_children()) {
        c->__dump(depth);
    }
}