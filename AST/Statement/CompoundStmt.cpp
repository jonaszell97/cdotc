//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "CompoundStmt.h"

CompoundStmt::CompoundStmt() :
    _statements(std::vector<AstNode::SharedPtr>()),
    context(std::make_shared<Context>())
{

}

CompoundStmt::CompoundStmt(const CompoundStmt& cp) : CompoundStmt() {
    for (auto stmt : cp._statements) {
        _statements.push_back(stmt->clone());
    }

    _returnable = cp._returnable;
    set_parent(cp._parent);
}

AstNode::SharedPtr CompoundStmt::clone() const {
    return std::make_shared<CompoundStmt>(*this);
}

void CompoundStmt::add_statement(AstNode::SharedPtr stmt) {
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