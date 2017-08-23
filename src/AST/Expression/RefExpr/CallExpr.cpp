//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"

CallExpr::CallExpr(CallType type, std::vector<Expression::SharedPtr> args, std::string _ident) :
    type(type),
    args(args)
{
    ident = _ident;
    for (auto& arg : this->args) {
        children.push_back(&arg);
    }
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (auto arg : args) {
        res.push_back(arg);
    }
    if (memberExpr != nullptr) {
        res.push_back(memberExpr);
    }

    return res;
}

void CallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "CallExpr" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}