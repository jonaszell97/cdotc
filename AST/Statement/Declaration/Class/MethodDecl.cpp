//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "../../../../Util.h"

#include <iostream>

MethodDecl::MethodDecl(std::string method_name, TypeSpecifier return_type, std::vector<FuncArgDecl::SharedPtr> args,
        CompoundStmt::SharedPtr body, AccessModifier am, bool is_static) :
    method_name(method_name),
    return_type(return_type),
    args(args),
    body(body),
    am(am),
    is_static(is_static)
{

}

MethodDecl::MethodDecl(const MethodDecl& cp) {
    set_parent(cp._parent);
}

AstNode::SharedPtr MethodDecl::clone() const {
    return std::make_shared<MethodDecl>(*this);
}

std::vector<AstNode::SharedPtr> MethodDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : args) {
        children.push_back(arg);
    }

    children.push_back(body);

    return children;
}

void MethodDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_static ? "Static" : "") << "MethodDecl [" << util::am_map[am] << " " << return_type.to_string()
              << " " << method_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}