//
// Created by Jonas Zell on 25.06.17.
//

#include "BreakStmt.h"
#include "WhileStmt.h"
#include "ForStmt.h"

#include <iostream>

BreakStmt::BreakStmt() {

}

BreakStmt::BreakStmt(const BreakStmt& cp) {
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr BreakStmt::clone() const {
    return std::make_shared<BreakStmt>(*this);
}

void BreakStmt::set_stmt(Statement *stmt, std::string type) {
    _stmt = stmt;
    _type = type;
}

std::vector<AstNode::SharedPtr> BreakStmt::get_children() {
    return {};
}

void BreakStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "BreakStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}