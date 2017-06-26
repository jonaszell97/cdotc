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

ArrayAccessExpr::ArrayAccessExpr(const ArrayAccessExpr& cp) {
    _index = std::static_pointer_cast<Expression>(cp._index->clone());;
    _return_ref = cp._return_ref;
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    set_parent(cp._parent);
}

AstNode::SharedPtr ArrayAccessExpr::clone() const {
    return std::make_shared<ArrayAccessExpr>(*this);
}

Variant ArrayAccessExpr::evaluate(Variant parent) {
    Array::SharedPtr arr = std::dynamic_pointer_cast<Array>(parent.get<Object::SharedPtr>());
    if (arr == nullptr) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index of non-array element");
    }

    Variant v = arr->at(_index->evaluate().get<int>());
    if (_member_expr != nullptr) {
        _member_expr->return_ref(_return_ref);
        return _member_expr->evaluate(v);
    }
    else {
        return (_return_ref) ? v : *v;
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