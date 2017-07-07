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

std::vector<AstNode::SharedPtr> Expression::get_children() {
    if (_child == nullptr) {
        return {};
    }
    else {
        return { _child };
    }
}

void Expression::__dump(int depth) {
    for (auto c : get_children()) {
        c->__dump(depth);
    }
}