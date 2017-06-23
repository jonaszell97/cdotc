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

VariantPtr FuncArgDecl::evaluate(VariantPtr) {
    VariantPtr def_val = _default_val == nullptr ? std::make_shared<Variant>() : _default_val->evaluate();
    FuncArgument::SharedPtr fa = std::make_shared<FuncArgument>(_arg_name, _arg_type, def_val);

    return std::make_shared<Variant>(fa);
}

void FuncArgDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "FuncArgDecl [" << util::types[_arg_type] << " " << _arg_name << "]" << std::endl;

    if (_default_val != nullptr) {
        _default_val->__dump(depth + 1);
    }
}