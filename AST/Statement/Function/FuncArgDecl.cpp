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

FuncArgDecl::FuncArgDecl(const FuncArgDecl& cp) {
    _arg_name = cp._arg_name;
    _arg_type = cp._arg_type;
    if (cp._default_val != nullptr) {
        _default_val = std::static_pointer_cast<Expression>(cp._default_val->clone());
    }
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr FuncArgDecl::clone() const {
    return std::make_shared<FuncArgDecl>(*this);
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


std::vector<AstNode::SharedPtr> FuncArgDecl::get_children() {
    if (_default_val != nullptr) {
        return { _default_val };
    }

    return {};
}

void FuncArgDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "FuncArgDecl [" << val::typetostr(_arg_type) << " " << _arg_name << "]" << std::endl;

    if (_default_val != nullptr) {
        _default_val->__dump(depth + 1);
    }
}