//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FuncArgDecl.h"
#include "../../../Util.h"

FuncArgDecl::FuncArgDecl(std::string name, TypeSpecifier type, Expression::SharedPtr def) :
    _arg_name(name),
    _arg_type(type),
    _default_val(def)
{

}

FuncArgDecl::FuncArgDecl() : FuncArgDecl("", TypeSpecifier(), {}) {

}

void FuncArgDecl::set_name(std::string name) {
    _arg_name = name;
}

void FuncArgDecl::set_default(Expression::SharedPtr def) {
    _default_val = def;
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