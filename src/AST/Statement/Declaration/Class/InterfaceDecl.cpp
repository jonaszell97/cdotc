//
// Created by Jonas Zell on 09.07.17.
//

#include "InterfaceDecl.h"

#include <iostream>

InterfaceDecl::InterfaceDecl(std::string name) : name(name) {

}

void InterfaceDecl::add_field(FieldDecl::SharedPtr field) {
    fields.push_back(field);
}

void InterfaceDecl::add_method(MethodDecl::SharedPtr method) {
    methods.push_back(method);
}

void InterfaceDecl::add_operator(OperatorDecl::SharedPtr op) {
    operators.push_back(op);
}

std::vector<AstNode::SharedPtr> InterfaceDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;

    for (auto field : fields) {
        children.push_back(field);
    }
    for (auto method : methods) {
        children.push_back(method);
    }
    for (auto op : operators) {
        children.push_back(op);
    }

    return children;
}

void InterfaceDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "InterfaceDecl [" << name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}