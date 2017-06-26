//
// Created by Jonas Zell on 19.06.17.
//

#include "IfStmt.h"

#include <iostream>

IfStmt::IfStmt(Expression::SharedPtr cond, Statement::SharedPtr if_branch) :
        _condition(cond), _if_branch(if_branch), _else_branch{} {

}

IfStmt::IfStmt(Expression::SharedPtr cond) : _condition(cond), _if_branch{}, _else_branch{} {

}

IfStmt::IfStmt(const IfStmt& cp) {
    _condition = std::static_pointer_cast<Expression>(cp._condition->clone());
    if (cp._if_branch != nullptr) {
        _if_branch = std::static_pointer_cast<Statement>(cp._if_branch->clone());
    }
    if (cp._else_branch != nullptr) {
        _else_branch = std::static_pointer_cast<Statement>(cp._else_branch->clone());
    }
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr IfStmt::clone() const {
    return std::make_shared<IfStmt>(*this);
}

Variant IfStmt::evaluate(Variant) {
    bool cond = _condition->evaluate().get<bool>();
    if (cond && _if_branch != nullptr) {
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

    if (_if_branch != nullptr) {
        children.push_back(_if_branch);
    }
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