//
// Created by Jonas Zell on 25.06.17.
//

#include "ForStmt.h"

#include <iostream>

ForStmt::ForStmt(Statement::SharedPtr init, Statement::SharedPtr term, Statement::SharedPtr inc) :
    _initialization(init),
    _termination(term),
    _increment(inc),
    init_context{},
    body_context{}
{

}

ForStmt::ForStmt(const ForStmt& cp) {
    _initialization = std::static_pointer_cast<Statement>(cp._initialization->clone());
    _termination = std::static_pointer_cast<Statement>(cp._termination->clone());
    _increment = std::static_pointer_cast<Statement>(cp._increment->clone());
    if (cp._body != nullptr) {
        _body = std::static_pointer_cast<CompoundStmt>(cp._body->clone());
    }
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr ForStmt::clone() const {
    return std::make_shared<ForStmt>(*this);
}

std::vector<AstNode::SharedPtr> ForStmt::get_children() {
    std::vector<AstNode::SharedPtr> children = { _initialization, _termination, _increment };
    if (_body != nullptr) {
        children.push_back(_body);
    }

    return children;
}

void ForStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ForStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}