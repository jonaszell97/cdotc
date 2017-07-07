//
// Created by Jonas Zell on 28.06.17.
//

#include "SwitchStmt.h"

#include <iostream>

SwitchStmt::SwitchStmt(Expression::SharedPtr switch_val) :
    switch_val(switch_val),
    cases(std::vector<CaseStmt::SharedPtr>())
{

}

std::vector<AstNode::SharedPtr> SwitchStmt::get_children() {
    std::vector<AstNode::SharedPtr> children = { switch_val };
    for (auto case_ : cases) {
        children.push_back(case_);
    }

    return children;
}

void SwitchStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "SwitchStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}