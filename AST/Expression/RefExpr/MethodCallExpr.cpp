//
// Created by Jonas Zell on 25.06.17.
//

#include "MethodCallExpr.h"
#include "../../../StdLib/Objects/Object.h"

#include <iostream>

MethodCallExpr::MethodCallExpr(std::string ident) : _ident(ident) {

}

MethodCallExpr::MethodCallExpr(const MethodCallExpr& cp) {
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    set_parent(cp._parent);
}

MethodCallExpr::MethodCallExpr(const CallExpr& cp, std::string ident) {
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    _arguments = std::vector<Expression::SharedPtr>(cp._arguments);
    _ident = ident;
    set_parent(cp._parent);
}

AstNode::SharedPtr MethodCallExpr::clone() const {
    return std::make_shared<MethodCallExpr>(*this);
}

std::vector<AstNode::SharedPtr> MethodCallExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }

    return {};
}

void MethodCallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MethodCallExpr ['" << _ident << "']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}