//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayAccessExpr.h"
#include "../../../StdLib/Objects/Array.h"
#include "../../../StdLib/Objects/Object.h"
#include "../Literal/LiteralExpr.h"

#include <iostream>

ArrayAccessExpr::ArrayAccessExpr(int index) {
    _index = std::make_shared<LiteralExpr>(index);
}

ArrayAccessExpr::ArrayAccessExpr(Expression::SharedPtr expr) {
    _index = expr;
}

std::vector<AstNode::SharedPtr> ArrayAccessExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_index);
    if ( _member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void ArrayAccessExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ArrayAccessExpr" << std::endl;

    _index->__dump(depth + 1);
    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}