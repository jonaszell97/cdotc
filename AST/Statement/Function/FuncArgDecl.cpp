//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FuncArgDecl.h"
#include "../../../Util.h"

FuncArgDecl::FuncArgDecl(std::string name, ValueType type, Expression::SharedPtr def) :
    _arg_name(name),
    _arg_type(type),
    _default_val(def)
{

}

FuncArgDecl::FuncArgDecl() : FuncArgDecl("", ANY_T, {}) {

}

void FuncArgDecl::set_name(std::string name) {
    _arg_name = name;
}

void FuncArgDecl::set_type(ValueType type) {
    _arg_type = type;
}

void FuncArgDecl::set_default(Expression::SharedPtr def) {
    _default_val = def;
}

Variant FuncArgDecl::evaluate(Variant) {
    return {};
}

FuncArg FuncArgDecl::specific_eval() {
    Variant def_val = _default_val == nullptr ? Variant() : _default_val->evaluate();
    FuncArg fa;
    fa.type = _arg_type;
    fa.name = _arg_name;
    fa.default_val = def_val;

    return fa;
}

void FuncArgDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "FuncArgDecl [" << util::types[_arg_type] << " " << _arg_name << "]" << std::endl;

    if (_default_val != nullptr) {
        _default_val->__dump(depth + 1);
    }
}