//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayLiteral.h"
#include "../../../Util.h"
#include "../../../StdLib/Objects/Array.h"

#include <iostream>

ArrayLiteral::ArrayLiteral(ValueType type, Expression::SharedPtr length) :
    _type(type),
    _length_expr(length),
    _elements(std::vector<Expression::SharedPtr>())
{

}

ArrayLiteral::ArrayLiteral(ValueType type) : _type(type), _elements(std::vector<Expression::SharedPtr>()) {

}

ArrayLiteral::ArrayLiteral(const ArrayLiteral& cp) {
    _type = cp._type;
    _length_expr = std::static_pointer_cast<Expression>(cp._length_expr->clone());
    _elements = std::vector<Expression::SharedPtr>();
    for (auto el : cp._elements) {
        _elements.push_back(std::static_pointer_cast<Expression>(el->clone()));
    }
    set_parent(cp._parent);
}

AstNode::SharedPtr ArrayLiteral::clone() const {
    return std::make_shared<ArrayLiteral>(*this);
}

void ArrayLiteral::add_element(Expression::SharedPtr el) {
    _elements.push_back(el);
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
    std::cout << "ArrayLiteral [" << val::typetostr(_type) << "]" << std::endl;

    if (_length_expr != nullptr) {
        _length_expr->__dump(depth + 1);
    }

    for (auto c : _elements) {
        c->__dump(depth + 1);
    }
}