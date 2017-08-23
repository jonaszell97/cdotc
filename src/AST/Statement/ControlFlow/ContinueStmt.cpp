//
// Created by Jonas Zell on 25.06.17.
//

#include "ContinueStmt.h"

ContinueStmt::ContinueStmt() {

}

std::vector<AstNode::SharedPtr> ContinueStmt::get_children() {
    return {};
}

void ContinueStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ContinueStmt\n";
}