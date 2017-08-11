//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "BinaryOperator.h"
#include "../../Util.h"
#include "../Statement/Block/CompoundStmt.h"

BinaryOperator::BinaryOperator(std::string op) : _operator(op) {

}

void BinaryOperator::set_fst_child(Expression::SharedPtr exp) {
    _first_child = exp;
}

void BinaryOperator::set_snd_child(Expression::SharedPtr exp) {
    _second_child = exp;
}

std::string BinaryOperator::get_operator() {
    return _operator;
}

std::vector<AstNode::SharedPtr> BinaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_first_child);
    res.push_back(_second_child);
    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void BinaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "BinaryOperator ['" + _operator << "']" << std::endl;

    for (auto child : get_children()) {
        child->__dump(depth + 1);
    }
}