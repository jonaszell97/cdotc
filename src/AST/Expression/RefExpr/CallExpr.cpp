//
// Created by Jonas Zell on 21.06.17.
//

#include <iostream>
#include "CallExpr.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../StdLib/Objects/Object.h"
#include "../../../Util.h"

CallExpr::CallExpr() :
    _arguments(std::vector<Expression::SharedPtr>())
{

}

CallExpr::CallExpr(std::vector<Expression::SharedPtr> args) : _arguments(args) {

}

void CallExpr::add_argument(Expression::SharedPtr arg) {
    _arguments.push_back(arg);
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (auto arg : _arguments) {
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