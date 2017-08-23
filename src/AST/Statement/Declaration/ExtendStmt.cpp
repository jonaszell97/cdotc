//
// Created by Jonas Zell on 31.07.17.
//

#include "ExtendStmt.h"
#include "Class/MethodDecl.h"
#include "Class/FieldDecl.h"
#include "Class/ConstrDecl.h"

ExtendStmt::ExtendStmt(std::string extended, std::vector<FieldDecl::SharedPtr> fields,
    std::vector<MethodDecl::SharedPtr> methods, std::vector<ConstrDecl::SharedPtr> constructors) :
    fields(fields),
    methods(methods),
    constructors(constructors),
    extended_class(extended)
{

}

std::vector<AstNode::SharedPtr> ExtendStmt::get_children() {
    return {};
}

void ExtendStmt::__dump(int depth) {
    AstNode::__tab(depth);
}