//
// Created by Jonas Zell on 25.06.17.
//

#include "ContinueStmt.h"
#include "ForStmt.h"
#include "WhileStmt.h"

#include <iostream>

ContinueStmt::ContinueStmt() {

}

void ContinueStmt::set_stmt(Statement *stmt, std::string type) {
    _stmt = stmt;
    _type = type;
}

std::vector<AstNode::SharedPtr> ContinueStmt::get_children() {
    return {};
}

void ContinueStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ContinueStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}