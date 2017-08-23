//
// Created by Jonas Zell on 02.07.17.
//

#include "UsingStmt.h"

UsingStmt::UsingStmt(string ns_name) : nsName(ns_name) {

}

std::vector<AstNode::SharedPtr> UsingStmt::get_children() {
    return { };
}

void UsingStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "UsingStmt [" << "]" << std::endl;
}