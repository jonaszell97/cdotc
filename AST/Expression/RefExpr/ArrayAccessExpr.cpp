//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayAccessExpr.h"
#include "../../../Objects/Array.h"
#include "../../../Objects/Object.h"
#include "../Literal/LiteralExpr.h"

#include <iostream>

ArrayAccessExpr::ArrayAccessExpr(int index) {
    _index = std::make_shared<LiteralExpr>(Variant::from(index));
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

VariantPtr ArrayAccessExpr::evaluate(VariantPtr parent) {
    if (parent->type == OBJECT_PROP_T) {
        parent = parent->op_val->get();
    }

    if (parent->type != ARRAY_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index of non-array element");
    }

    // return reference
    if (_member_expr != nullptr) {
        _member_expr->return_ref(_return_ref);

        return _member_expr->evaluate({ parent->arr_val->at(int(_index->evaluate()->d_val)) });
    }
    else {
        return { parent->arr_val->at(int(_index->evaluate()->d_val)) };
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