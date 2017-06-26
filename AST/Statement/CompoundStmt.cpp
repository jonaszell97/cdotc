//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "CompoundStmt.h"

CompoundStmt::CompoundStmt() :
    _statements(std::vector<AstNode::SharedPtr>()),
    _return_reached(false),
    _return_val{}
{

}

CompoundStmt::CompoundStmt(const CompoundStmt& cp) {
    _statements = std::vector<AstNode::SharedPtr>();
    for (auto stmt : cp._statements) {
        _statements.push_back(stmt->clone());
    }

    _terminable = cp._terminable;
    _continuable = cp._continuable;

    _return_reached = false;
    _return_val = {};

    set_parent(cp._parent);
}

AstNode::SharedPtr CompoundStmt::clone() const {
    return std::make_shared<CompoundStmt>(*this);
}

void CompoundStmt::terminate(Variant v) {
    if (!_terminable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Cannot return from global context");
    }

    _return_reached = true;
    _return_val = v;
}

void CompoundStmt::continue_() {
    if (!_continuable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements");
    }

    _return_reached = true;
    _return_val = {};
}

void CompoundStmt::add_statement(AstNode::SharedPtr stmt) {
    _statements.emplace_back(stmt);
}

Variant CompoundStmt::evaluate(Variant) {
    while (_statements.size() > 0 && !_return_reached) {
        _statements.front()->evaluate();
        _statements.erase(_statements.begin());
    }

    return _return_val;
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