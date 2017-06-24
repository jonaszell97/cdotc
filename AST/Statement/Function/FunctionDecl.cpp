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

void FunctionDecl::add_arg(FuncArgDecl::SharedPtr arg) {
    _args.push_back(arg);
}

void FunctionDecl::set_body(CompoundStmt::SharedPtr body) {
    _body = body;
}

void FunctionDecl::set_return_type(ValueType type) {
    _return_type = type;
}

Variant FunctionDecl::evaluate(Variant) {
    Function func(_func_name, _return_type);
    func.set_body(_body);

    for (auto arg : _args) {
        FuncArg result = arg->specific_eval();
        func.add_argument(result.name, result.type, result.default_val);
    }

    if (auto root = _root.lock()) {
        root->set_function(_func_name, func);
    }
    else {
        RuntimeError::raise(ERR_MISSING_CONTEXT, "Cannot create function in current context");
    }

    return { };
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
    std::cout << "FunctionDecl ["<< _func_name << ": " << util::types[_return_type]  << "]" << std::endl;

    for (auto arg : _args) {
        arg->__dump(depth + 1);
    }

    _body->__dump(depth + 1);
}

