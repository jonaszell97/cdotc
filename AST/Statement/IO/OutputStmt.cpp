//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "OutputStmt.h"

OutputStmt::OutputStmt(Expression::SharedPtr expr, bool newline) : _value(expr), _newline(newline) {}

OutputStmt::OutputStmt(const OutputStmt& cp) {
    _value = std::static_pointer_cast<Expression>(cp._value->clone());
    _newline = cp._newline;
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr OutputStmt::clone() const {
    return std::make_shared<OutputStmt>(*this);
}

std::vector<AstNode::SharedPtr> OutputStmt::get_children() {
    return std::vector<AstNode::SharedPtr> { _value };
}

void OutputStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "OutputStmt" << std::endl;

    _value->__dump(depth + 1);
}