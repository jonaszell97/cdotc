//
// Created by Jonas Zell on 21.06.17.
//

#include "SubscriptExpr.h"

SubscriptExpr::SubscriptExpr(Expression::SharedPtr expr) {
    _index = expr;
    children.push_back(&_index);
}

std::vector<AstNode::SharedPtr> SubscriptExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_index);
    if ( memberExpr != nullptr) {
        res.push_back(memberExpr);
    }

    return res;
}

void SubscriptExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "SubscriptExpr" << std::endl;

    _index->__dump(depth + 1);
    if (memberExpr != nullptr) {
        memberExpr->__dump(depth + 1);
    }
}