//
// Created by Jonas Zell on 02.07.17.
//

#include "ImportStmt.h"

#include <iostream>

ImportStmt::ImportStmt(std::vector<string> import) : import(import) {

}

std::vector<AstNode::SharedPtr> ImportStmt::get_children() {
    return { };
}

void ImportStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ImportStmt [" << "]" << std::endl;
}