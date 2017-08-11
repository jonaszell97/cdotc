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

LiteralExpr::LiteralExpr(Variant v, TypeSpecifier type) : _value(v), _type(type) {
    set_val(v);
}

void LiteralExpr::set_val(Variant val) {
    _value = val;
    switch (val.get_type().type) {
        case INT_T:
        case LONG_T:
            class_name = "IntegerLiteral";
            break;
        case FLOAT_T:
        case DOUBLE_T:
            class_name = "FloatingPointLiteral";
            break;
        case BOOL_T:
            class_name = "BoolLiteral";
            break;
        case CHAR_T:
            class_name = "CharLiteral";
            break;
        case STRING_T:
            class_name = "StringLiteral";
            break;
        case OBJECT_T:
            class_name = "ObjectLiteral";
        case VOID_T:
            class_name = "NullLiteral";
            break;

    }
}

std::vector<AstNode::SharedPtr> LiteralExpr::get_children() {
    return _member_expr == nullptr ? std::vector<AstNode::SharedPtr>() : std::vector<AstNode::SharedPtr>{_member_expr };
}

void LiteralExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << class_name << " ['" << _value.to_string(true) << "']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}