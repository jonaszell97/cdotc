//
// Created by Jonas Zell on 25.06.17.
//

#include "ForStmt.h"

#include <iostream>

ForStmt::ForStmt(Statement::SharedPtr init, Statement::SharedPtr term, Statement::SharedPtr inc) :
    _initialization(init),
    _termination(term),
    _increment(inc)
{

}

ForStmt::ForStmt(IdentifierRefExpr::SharedPtr ident, Expression::SharedPtr range) : ident(ident), range(range),
                                                                                    range_based(true) {

}

std::vector<AstNode::SharedPtr> ForStmt::get_children() {
    if (range_based) {
        std::vector<AstNode::SharedPtr> children = {ident, range};
        if (_body != nullptr) {
            children.push_back(_body);
        }

        return children;
    }
    else {
        std::vector<AstNode::SharedPtr> children = {_initialization, _termination, _increment};
        if (_body != nullptr) {
            children.push_back(_body);
        }

        return children;
    }
}

void ForStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (range_based ? "ForInStmt" : "ForStmt") << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}