//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayAccessExpr.h"
#include "../../../Objects/Array.h"
#include "../../../Objects/Object.h"
#include "../Literal/LiteralExpr.h"

#include <iostream>

ArrayAccessExpr::ArrayAccessExpr(int index) {
    _index = std::make_shared<LiteralExpr>(index);
}

ArrayAccessExpr::ArrayAccessExpr(Expression::SharedPtr expr) {
    _index = expr;
}

void ArrayAccessExpr::set_member_expr(std::shared_ptr<RefExpr> member_expr) {
    _member_expr = member_expr;
}

void ArrayAccessExpr::return_ref(bool ref) {
    _return_ref = ref;
}

Variant ArrayAccessExpr::evaluate(Variant parent) {
    if (parent.get_type() != ARRAY_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index of non-array element");
    }

    // return reference
    if (_member_expr != nullptr) {
        return _member_expr->evaluate({ parent.get<Array::SharedPtr>()->at(_index->evaluate().get<int>()) });
    }
    else {
        return { parent.get<Array::SharedPtr>()->at(_index->evaluate().get<int>()) };
    }
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