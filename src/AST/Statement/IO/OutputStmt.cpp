//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "OutputStmt.h"

OutputStmt::OutputStmt(Expression::SharedPtr expr, bool newline) : _value(expr), _newline(newline) {}

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