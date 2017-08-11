//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FunctionDecl.h"
#include "../../../Util.h"

FunctionDecl::FunctionDecl(std::string func_name, TypeRef::SharedPtr type) :
    _func_name(func_name),
    _return_type(type),
    _args(std::vector<FuncArgDecl::SharedPtr>())
{

}

void FunctionDecl::add_arg(FuncArgDecl::SharedPtr arg) {
    _args.push_back(arg);
}

std::vector<AstNode::SharedPtr> FunctionDecl::get_children() {
    std::vector<AstNode::SharedPtr> res;
    if (_body != nullptr) {
        res.push_back(_body);
    }
    for (auto arg : _args) {
        res.push_back(arg);
    }

    return res;
}

void FunctionDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "FunctionDecl ["<< _func_name << " => " << _return_type->to_string()  << "]" << std::endl;

    for (auto arg : _args) {
        arg->__dump(depth + 1);
    }

    _body->__dump(depth + 1);
}

