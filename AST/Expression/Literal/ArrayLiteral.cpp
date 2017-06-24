//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayLiteral.h"
#include "../../../Util.h"
#include "../../../Objects/Array.h"

#include <iostream>

ArrayLiteral::ArrayLiteral(ValueType type, Expression::SharedPtr length) :
    _type(type),
    _length_expr(length),
    _elements(std::vector<Expression::SharedPtr>())
{

}

ArrayLiteral::ArrayLiteral(ValueType type) : _type(type), _elements(std::vector<Expression::SharedPtr>()) {

}

void ArrayLiteral::add_element(Expression::SharedPtr el) {
    _elements.push_back(el);
}

Variant ArrayLiteral::evaluate(Variant) {
    if (_length_expr != nullptr) {
        int length = _length_expr->evaluate().get<int>();
        if (length > INT32_MAX) {
            RuntimeError::raise(ERR_VAL_TOO_LARGE, "An array can hold a maxium of " + std::to_string(INT32_MAX) + " values, " +
                "tried to allocate " + std::to_string(int(length)));
        }

        _length = int(length);
    }

    std::shared_ptr<Array> arr = std::make_shared<Array>(_type, _length);
    for (auto el : _elements) {
        auto res = el->evaluate();
        if (res.get_type() == DOUBLE_T && _type != DOUBLE_T) {
            res.cast_to(FLOAT_T);
        }

        arr->push(res);
    }

    return { arr };
}

std::vector<AstNode::SharedPtr> ArrayLiteral::get_children() {
    std::vector<AstNode::SharedPtr> res;
    if (_length_expr != nullptr) {
        res.push_back(_length_expr);
    }

    for (auto el : _elements) {
        res.push_back(el);
    }

    return res;
}

void ArrayLiteral::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ArrayLiteral [" << util::types[_type];
    std::cout << "]" << std::endl;

    if (_length_expr != nullptr) {
        _length_expr->__dump(depth + 1);
    }

    for (auto c : _elements) {
        c->__dump(depth + 1);
    }
}