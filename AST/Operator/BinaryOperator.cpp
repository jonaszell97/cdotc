//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "BinaryOperator.h"
#include "../../StdLib/Objects/Object.h"
#include "../../Util.h"
#include "../Statement/CompoundStmt.h"

BinaryOperator::BinaryOperator(std::string op) : _operator(op) {

}

BinaryOperator::BinaryOperator(const BinaryOperator& cp) {
    _operator = cp._operator;
    _first_child = std::static_pointer_cast<Expression>(cp._first_child->clone());
    _second_child = std::static_pointer_cast<Expression>(cp._second_child->clone());
    set_parent(cp._parent);
}

AstNode::SharedPtr BinaryOperator::clone() const {
    return std::make_shared<BinaryOperator>(*this);
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

    return res;
}

void BinaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "BinaryOperator ['" + _operator << "']" << std::endl;

    _first_child->__dump(depth + 1);
    _second_child->__dump(depth + 1);
}