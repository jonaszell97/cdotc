//
// Created by Jonas Zell on 01.07.17.
//

#include "OperatorDecl.h"

#include <iostream>

OperatorDecl::OperatorDecl(std::string op, std::vector<FuncArgDecl::SharedPtr> args, AccessModifier am, bool
    is_binary) : _operator(op), args(args), am(am), is_binary(is_binary) {

}

OperatorDecl::OperatorDecl(const OperatorDecl& cp) {
    set_parent(cp._parent);
}

AstNode::SharedPtr OperatorDecl::clone() const {
    return std::make_shared<OperatorDecl>(*this);
}

std::vector<AstNode::SharedPtr> OperatorDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : args) {
        children.push_back(arg);
    }
    children.push_back(body);

    return children;
}

void OperatorDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "OperatorDecl [" << _operator << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}