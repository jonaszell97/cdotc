//
// Created by Jonas Zell on 07.07.17.
//

#include "FunctionCallExpr.h"

#include <iostream>

FunctionCallExpr::FunctionCallExpr(std::string ident, std::vector<Expression::SharedPtr> args) : _ident(ident), args
        (args) {

}

std::vector<AstNode::SharedPtr> FunctionCallExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : args) {
        children.push_back(arg);
    }

    if (_member_expr != nullptr) {
        children.push_back(_member_expr);
    }

    return children;
}

void FunctionCallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "FunctionCallExpr [" << _ident << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}