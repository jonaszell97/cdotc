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

Variant BreakStmt::evaluate(Variant) {
    if (_type == "for") {
        ForStmt* for_stmt = static_cast<ForStmt*>(_stmt);
        for_stmt->get_current_instance()->continue_();
        for_stmt->break_();
    }
    else if (_type == "while") {
        WhileStmt* while_stmt = static_cast<WhileStmt*>(_stmt);
        while_stmt->get_current_instance()->continue_();
        while_stmt->break_();
    }
    else if (_type == "switch") {

    }

    RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in for, while and switch statements");
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