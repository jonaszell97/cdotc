//
// Created by Jonas Zell on 21.06.17.
//

#include "ArrayLiteral.h"
#include "../../../Util.h"
#include "../../../StdLib/Objects/Array.h"

#include <iostream>

ArrayLiteral::ArrayLiteral(TypeSpecifier ts) :
    type(ts),
    _elements(std::vector<Expression::SharedPtr>())
{

}

void ArrayLiteral::add_element(Expression::SharedPtr el) {
    _elements.push_back(el);
}

std::vector<AstNode::SharedPtr> ArrayLiteral::get_children() {
    std::vector<AstNode::SharedPtr> res;
    if (type.length != nullptr) {
        res.push_back(type.length);
    }

    for (auto el : _elements) {
        res.push_back(el);
    }

    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void ArrayLiteral::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ArrayLiteral [" << val::typetostr(type) << "]" << std::endl;

    if (type.length != nullptr) {
        type.length->__dump(depth + 1);
    }

    for (auto c : _elements) {
        c->__dump(depth + 1);
    }
}