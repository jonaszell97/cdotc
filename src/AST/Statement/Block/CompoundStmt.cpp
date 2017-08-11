//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "CompoundStmt.h"

CompoundStmt::CompoundStmt(bool keep_scope)
    : keep_scope(keep_scope)
{

}

void CompoundStmt::add_statement(Statement::SharedPtr stmt) {
    _statements.emplace_back(stmt);
}

std::vector<AstNode::SharedPtr> CompoundStmt::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (int i = 0; i < _statements.size(); i++) {
        res.push_back(_statements[i]);
    }

    return res;
}

void CompoundStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "CompoundStmt" << std::endl;

    for (int i = 0; i < _statements.size(); i++) {
        _statements[i]->__dump(depth + 1);
    }
}