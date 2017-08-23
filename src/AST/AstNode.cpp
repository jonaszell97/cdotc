//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "Statement/Declaration/DeclStmt.h"
#include "../Parser.h"

AstNode::AstNode() {

}

void AstNode::heapAllocate() {
    isHeapAllocated = true;
    for (const auto& child : get_children()) {
        child->heapAllocate();
        if (child->declaration != nullptr) {
            child->declaration->isHeapAllocated = true;
        }
    }
}

void AstNode::isReturnValue() {
    isReturnValue_ = true;
    if (declaration != nullptr) {
        declaration->isReturnValue();
    }
}

void AstNode::isHiddenReturnValue() {
    isReturnValue_ = true;
    isHiddenReturnValue_ = true;
    if (declaration != nullptr) {
        declaration->isHiddenReturnValue();
    }
}

string AstNode::getSourceFile() {
    return Parser::get_source_file(sourceFileId);
}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
    return std::vector<AstNode::SharedPtr>();
}

void AstNode::__tab(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    printf("<%p> ", this);
}