//
// Created by Jonas Zell on 27.06.17.
//

#include "ConstrDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../FuncArgDecl.h"

#include <iostream>

ConstrDecl::ConstrDecl() : args{}, body{}, implicit(true), am(AccessModifier::PUBLIC) {

}

ConstrDecl::ConstrDecl(std::vector<FuncArgDecl::SharedPtr> args, CompoundStmt::SharedPtr body, AccessModifier am) :
        args(args), body(body), am(am) {

}

std::vector<AstNode::SharedPtr> ConstrDecl::get_children() {
    if (implicit) {
        return {};
    }

    std::vector<AstNode::SharedPtr> children;
    for (auto arg : args) {
        children.push_back(arg);
    }

    children.push_back(body);

    return children;
}

void ConstrDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (implicit ? "Implicit" : "") << "ConstrDecl" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}