//
// Created by Jonas Zell on 21.06.17.
//

#include <iostream>
#include "CallExpr.h"
#include "../../Statement/Block/CompoundStmt.h"
#include "../../../Util.h"

CallExpr::CallExpr(CallType type, std::vector<Expression::SharedPtr> args, std::string ident) : type(type), args
    (args), ident(ident) {

}

void CallExpr::add_argument(Expression::SharedPtr arg) {
    args.push_back(arg);
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (auto arg : args) {
        res.push_back(arg);
    }
    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
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