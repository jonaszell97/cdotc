//
// Created by Jonas Zell on 28.06.17.
//

#include "CaseStmt.h"

#include <iostream>

CaseStmt::CaseStmt(Expression::SharedPtr case_val) : case_val(case_val) {

}

CaseStmt::CaseStmt() : is_default(true) {

}

std::vector<AstNode::SharedPtr> CaseStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (case_val != nullptr) {
        children.push_back(case_val);
    }
    for (auto child : LabelStmt::get_children()) {
        children.push_back(child);
    }

    return children;
}

void CaseStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_default ? "DefaultStmt" : "CaseStmt") << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}