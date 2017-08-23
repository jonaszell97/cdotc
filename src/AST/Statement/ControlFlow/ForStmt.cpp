//
// Created by Jonas Zell on 25.06.17.
//

#include "ForStmt.h"

ForStmt::ForStmt(Statement::SharedPtr init, Statement::SharedPtr term, Statement::SharedPtr inc) :
    initialization(init),
    termination(term),
    increment(inc)
{

}

std::vector<AstNode::SharedPtr> ForStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (body != nullptr) {
        children.push_back(body);
    }
    if (initialization) {
        children.push_back(initialization);
    }
    if (increment) {
        children.push_back(increment);
    }
    if (termination) {
        children.push_back(termination);
    }

    return children;
}

void ForStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (rangeBased ? "ForInStmt" : "ForStmt") << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}