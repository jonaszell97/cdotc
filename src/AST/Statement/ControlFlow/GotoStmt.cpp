//
// Created by Jonas Zell on 29.06.17.
//

#include "GotoStmt.h"

#include <iostream>

GotoStmt::GotoStmt(std::string label_name) : label_name(label_name) {

}

std::vector<AstNode::SharedPtr> GotoStmt::get_children() {
    return { };
}

void GotoStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "GotoStmt [" << label_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}