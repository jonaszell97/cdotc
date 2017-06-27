//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "LiteralExpr.h"

LiteralExpr::LiteralExpr() : LiteralExpr(Variant()) {

}

LiteralExpr::LiteralExpr(Variant v) : _value(v), _type(v.get_type()) {
    set_val(v);
}

LiteralExpr::LiteralExpr(Variant v, ValueType type) : _value(v), _type(type) {
    set_val(v);
}

LiteralExpr::LiteralExpr(const LiteralExpr& cp) {
    set_val(Variant(cp._value));
    _type = _value.get_type();
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr LiteralExpr::clone() const {
    return std::make_shared<LiteralExpr>(*this);
}

void LiteralExpr::set_val(Variant val) {
    _value = val;
    switch (val.get_type()) {
        case INT_T:
        case LONG_T:
            __class_name = "IntegerLiteral";
            break;
        case FLOAT_T:
        case DOUBLE_T:
            __class_name = "FloatingPointLiteral";
            break;
        case BOOL_T:
            __class_name = "BoolLiteral";
            break;
        case CHAR_T:
            __class_name = "CharLiteral";
            break;
        case STRING_T:
            __class_name = "StringLiteral";
            break;
        case OBJECT_T:
            __class_name = "ObjectLiteral";
        case VOID_T:
            __class_name = "NullLiteral";
            break;

    }
}

std::vector<AstNode::SharedPtr> LiteralExpr::get_children() {
    return {};
}

void LiteralExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << __class_name << " ['" << _value.to_string(true) << "']" << std::endl;
}