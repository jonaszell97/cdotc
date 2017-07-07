//
// Created by Jonas Zell on 04.07.17.
//

#include "StructDecl.h"

#include <iostream>

StructDecl::StructDecl(std::string struct_name, std::vector<FieldDecl::SharedPtr> fields,
        std::vector<MethodDecl::SharedPtr> methods, ConstrDecl::SharedPtr constr,
        std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> unary_operators,
        std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> binary_operators) :
        ClassDecl(struct_name, fields,methods, constr, AccessModifier::PUBLIC, unary_operators, binary_operators,
                false, false, "", std::vector<std::string>())
{

}

std::vector<AstNode::SharedPtr> StructDecl::get_children() {
    return ClassDecl::get_children();
}

void StructDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "StructDecl [" << class_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}