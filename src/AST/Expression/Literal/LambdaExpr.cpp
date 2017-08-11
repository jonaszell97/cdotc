//
// Created by Jonas Zell on 01.07.17.
//

#include "LambdaExpr.h"
#include <iostream>

LambdaExpr::LambdaExpr(TypeSpecifier return_type, std::vector<FuncArgDecl::SharedPtr> args) :
    _return_type(return_type),
    _args(args)
{

}

std::vector<AstNode::SharedPtr> LambdaExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : _args) {
        children.push_back(arg);
    }
    if (_body != nullptr) {
        children.push_back(_body);
    }
    if (_member_expr != nullptr) {
        children.push_back(_member_expr);
    }

    return children;
}

void LambdaExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::string signature = "(";
    for (int i = 0; i < _args.size(); ++i) {
        signature += _args[i]->get_arg_type()->to_string();
        if (i < _args.size() - 1) {
            signature += ", ";
        }
    }
    signature += ") -> " + _return_type.to_string();

    std::cout << "LambdaExpr [" << signature << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}