//
// Created by Jonas Zell on 25.06.17.
//

#include "MethodCallExpr.h"
#include "../../../StdLib/Objects/Object.h"

#include <iostream>

MethodCallExpr::MethodCallExpr(std::string ident, std::vector<Expression::SharedPtr> args) : _ident(ident),
    _arguments(args) {

}

std::vector<AstNode::SharedPtr> MethodCallExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;

    if (_member_expr != nullptr) {
        children.push_back(_member_expr);
    }
    for (auto arg : _arguments) {
        children.push_back(arg);
    }

    return children;
}

void MethodCallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MethodCallExpr ['" << _ident << "']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}