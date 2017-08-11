//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "../../../../Util.h"

#include <iostream>

MethodDecl::MethodDecl(std::string method_name, TypeRef::SharedPtr return_type, std::vector<FuncArgDecl::SharedPtr> args,
        CompoundStmt::SharedPtr body, AccessModifier am, bool is_static) :
    method_name(method_name),
    return_type(return_type),
    args(args),
    body(body),
    am(am),
    is_static(is_static),
    is_abstract(false)
{

}

MethodDecl::MethodDecl(std::string method_name, TypeRef::SharedPtr return_type, std::vector<FuncArgDecl::SharedPtr> args,
        AccessModifier am, bool is_static) :
    method_name(method_name),
    return_type(return_type),
    args(args),
    am(am),
    is_static(is_static),
    is_abstract(true)
{

}

std::vector<AstNode::SharedPtr> MethodDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : args) {
        children.push_back(arg);
    }

    if (!is_abstract) {
        children.push_back(body);
    }

    return children;
}

void MethodDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_static ? "Static" : "") << "MethodDecl [" << (is_abstract ? "abstract " : "") << util::am_map[am]
            << " " << method_name << " => " << return_type->to_string() << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}