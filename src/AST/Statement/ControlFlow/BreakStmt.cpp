//
// Created by Jonas Zell on 25.06.17.
//

#include "BreakStmt.h"

BreakStmt::BreakStmt() {

}

std::vector<AstNode::SharedPtr> BreakStmt::get_children() {
    return {};
}

void BreakStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "BreakStmt" << std::endl;
}