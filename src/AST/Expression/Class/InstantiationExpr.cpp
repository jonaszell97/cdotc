//
// Created by Jonas Zell on 27.06.17.
//

#include "InstantiationExpr.h"

#include <iostream>

InstantiationExpr::InstantiationExpr(std::string class_name, std::vector<Expression::SharedPtr> args):
    class_name(class_name),
    constr_args(args)
{

}

InstantiationExpr::InstantiationExpr(const InstantiationExpr& cp) {

}

AstNode::SharedPtr InstantiationExpr::clone() const {
    return std::make_shared<InstantiationExpr>(*this);
}

std::vector<AstNode::SharedPtr> InstantiationExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : constr_args) {
        children.push_back(arg);
    }

    return children;
}

void InstantiationExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "InstantiationExpr [new " << class_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}