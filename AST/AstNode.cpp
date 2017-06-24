//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "../Variant/Variant.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../Exceptions.h"
#include "../Objects/Object.h"
#include "../Util.h"
#include "../Token.h"
#include "./Statement/CompoundStmt.h"

AstNode::AstNode() {

}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
    return std::vector<AstNode::SharedPtr>();
}

void AstNode::set_root(CompoundStmt::WeakPtr root, bool recursive) {
    if (recursive) {
        for (auto child : get_children()) {
            child->set_root(root);
        }
    }

    _root = root;
}

void AstNode::__tab(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
}