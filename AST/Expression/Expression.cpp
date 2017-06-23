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

void Expression::set_child(Expression::SharedPtr child) {
    _child = child;
}

VariantPtr Expression::evaluate(VariantPtr t) {
    return _child->evaluate(t);
}

std::vector<AstNode::SharedPtr> Expression::get_children() {
    return std::vector<AstNode::SharedPtr> { _child };
}

void Expression::__dump(int depth) {
    for (auto c : get_children()) {
        c->__dump(depth);
    }
}