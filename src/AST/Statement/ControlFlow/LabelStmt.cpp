//
// Created by Jonas Zell on 28.06.17.
//

#include "LabelStmt.h"
#include <iostream>

LabelStmt::LabelStmt() : parent_cmpnd{}, visitor{} {

}

LabelStmt::LabelStmt(std::string label_name) : label_name(label_name), parent_cmpnd{}, visitor{} {

}

std::vector<AstNode::SharedPtr> LabelStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto child : CompoundStmt::get_children()) {
        children.push_back(child);
    }

    return children;
}

void LabelStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "LabelStmt [" << label_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}