//
// Created by Jonas Zell on 02.07.17.
//

#include "ExportStmt.h"

#include <iostream>

ExportStmt::ExportStmt(std::string target) : target(target) {

}

std::vector<AstNode::SharedPtr> ExportStmt::get_children() {
    return { };
}

void ExportStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ExportStmt [" << target << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}