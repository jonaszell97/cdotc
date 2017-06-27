//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FunctionDecl.h"
#include "../../../Util.h"

FunctionDecl::FunctionDecl(std::string func_name, ValueType type) :
    _func_name(func_name),
    _return_type(type),
    _args(std::vector<FuncArgDecl::SharedPtr>())
{

}

FunctionDecl::FunctionDecl(const FunctionDecl& cp) {
    _func_name = cp._func_name;
    _return_type = cp._return_type;
    _args = std::vector<FuncArgDecl::SharedPtr>();
    for (auto arg : cp._args) {
        _args.push_back(std::static_pointer_cast<FuncArgDecl>(arg->clone()));
    }
    set_parent(cp._parent);
}

AstNode::SharedPtr FunctionDecl::clone() const {
    return std::make_shared<FunctionDecl>(*this);
}

void FunctionDecl::add_arg(FuncArgDecl::SharedPtr arg) {
    _args.push_back(arg);
}

void FunctionDecl::set_body(CompoundStmt::SharedPtr body) {
    _body = body;
}

void FunctionDecl::set_return_type(ValueType type) {
    _return_type = type;
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
    std::cout << "FunctionDecl ["<< _func_name << " => " << val::typetostr(_return_type)  << "]" << std::endl;

    for (auto arg : _args) {
        arg->__dump(depth + 1);
    }

    _body->__dump(depth + 1);
}

