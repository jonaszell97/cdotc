//
// Created by Jonas Zell on 02.07.17.
//

#include "NamespaceDecl.h"
#include "../ImportStmt.h"
#include "../ExportStmt.h"

#include <iostream>

NamespaceDecl::NamespaceDecl(std::string ns_name, CompoundStmt::SharedPtr content) :
    ns_name(ns_name),
    contents(content)
{

}

std::vector<AstNode::SharedPtr> NamespaceDecl::get_children() {
    return { contents };
}

void NamespaceDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout <<  "NameSpaceDecl [" << ns_name << "]" << std::endl;

    contents->__dump(depth + 1);
}