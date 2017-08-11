//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "../Variant/Variant.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../Message/Exceptions.h"
#include "../Util.h"
#include "../Token.h"
#include "Statement/Block/CompoundStmt.h"
#include "Visitor/Visitor.h"
#include "../Parser.h"

AstNode::AstNode() {

}

void AstNode::alloc_on_heap() {
    heap_alloc = true;
    for (const auto& child : get_children()) {
        child->alloc_on_heap();
        if (child->declaration != nullptr) {
            child->declaration->heap_alloc = true;
        }
    }
}

string AstNode::get_source() {
    return Parser::get_source_file(source_file);
}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
    return std::vector<AstNode::SharedPtr>();
}

void AstNode::set_parent(AstNode *parent) {
    _parent = parent;
}

void AstNode::__tab(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
}