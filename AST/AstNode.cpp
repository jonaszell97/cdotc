//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "../Variant.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../Exceptions.h"
#include "../Objects/Object.h"
#include "../Util.h"
#include "../Token.h"
#include "./Statement/CompoundStmt.h"

AstNode::AstNode() : _parent{}, _root{} {

}

/**
 * Returns the node's parent node
 * @return
 */
std::weak_ptr<AstNode> AstNode::get_parent() {
    return _parent;
}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
    return std::vector<AstNode::SharedPtr>();
}

/**
 * Sets the node's parent node
 * @param p
 */
void AstNode::set_parent(AstNode::WeakPtr p) {
    _parent = p;
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