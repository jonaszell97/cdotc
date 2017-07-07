//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "../Variant/Variant.h"
#include <string>
#include <iostream>
#include <cmath>
#include "../Message/Exceptions.h"
#include "../StdLib/Objects/Object.h"
#include "../Util.h"
#include "../Token.h"
#include "./Statement/CompoundStmt.h"
#include "Visitor/Visitor.h"

AstNode::AstNode() {

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