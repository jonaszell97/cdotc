//
// Created by Jonas Zell on 25.06.17.
//

#include "ContinueStmt.h"
#include "ForStmt.h"
#include "WhileStmt.h"

#include <iostream>

ContinueStmt::ContinueStmt() {

}

ContinueStmt::ContinueStmt(const ContinueStmt& cp) {
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr ContinueStmt::clone() const {
    return std::make_shared<ContinueStmt>(*this);
}

void ContinueStmt::set_stmt(Statement *stmt, std::string type) {
    _stmt = stmt;
    _type = type;
}

Variant ContinueStmt::evaluate(Variant) {
    if (_type == "for") {
        ForStmt* for_stmt = static_cast<ForStmt*>(_stmt);
        for_stmt->get_current_instance()->continue_();
    }
    else if (_type == "while") {
        WhileStmt* while_stmt = static_cast<WhileStmt*>(_stmt);
        while_stmt->get_current_instance()->continue_();
    }

    RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements");
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