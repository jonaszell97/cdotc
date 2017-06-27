//
// Created by Jonas Zell on 27.06.17.
//

#include "ClassDecl.h"

#include <iostream>

ClassDecl::ClassDecl(std::string class_name, std::vector<FieldDecl::SharedPtr> fields,
         std::vector<MethodDecl::SharedPtr> methods, ConstrDecl::SharedPtr constr, AccessModifier am) :
    class_name(class_name),
    fields(fields),
    methods(methods),
    constr(constr),
    am(am)
{

}

ClassDecl::ClassDecl(const ClassDecl& cp) {
    set_parent(cp._parent);
}

AstNode::SharedPtr ClassDecl::clone() const {
    return std::make_shared<ClassDecl>(*this);
}

std::vector<AstNode::SharedPtr> ClassDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (constr != nullptr) {
        children.push_back(constr);
    }
    for (auto field : fields) {
        children.push_back(field);
    }
    for (auto method : methods) {
        children.push_back(method);
    }

    return children;
}

void ClassDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ClassDecl [" << class_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}