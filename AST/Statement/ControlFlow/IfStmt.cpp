//
// Created by Jonas Zell on 19.06.17.
//

#include "IfStmt.h"

#include <iostream>

IfStmt::IfStmt(Expression::SharedPtr cond, Statement::SharedPtr if_branch) :
        _condition(cond), _if_branch(if_branch), _else_branch{} {

}

Variant IfStmt::evaluate(Variant) {
    bool cond = _condition->evaluate().get<bool>();
    if (cond) {
        return _if_branch->evaluate();
    }
    else if (!cond && _else_branch != nullptr) {
        return _else_branch->evaluate();
    }

    return {};
}

std::vector<AstNode::SharedPtr> IfStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    children.push_back(_condition);
    children.push_back(_if_branch);

    if (_else_branch != nullptr) {
        children.push_back(_else_branch);
    }

    return children;
}

void IfStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "IfStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}