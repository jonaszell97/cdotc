//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../../Util.h"

#include <iostream>

FieldDecl::FieldDecl(std::string field_name, TypeSpecifier type, AccessModifier am, bool is_static,
         Expression::SharedPtr def_val):
    field_name(field_name),
    type(type),
    am(am),
    is_static(is_static),
    default_val(def_val)
{

}

FieldDecl::FieldDecl(const FieldDecl& cp) {
    set_parent(cp._parent);
}

AstNode::SharedPtr FieldDecl::clone() const {
    return std::make_shared<FieldDecl>(*this);
}

std::vector<AstNode::SharedPtr> FieldDecl::get_children() {
    return (default_val != nullptr) ? std::vector<AstNode::SharedPtr>{ default_val } : std::vector<AstNode::SharedPtr>{};
}

void FieldDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_static ? "Static" : "") << "FieldDecl [" << util::am_map[am] << " " << type.to_string()
              << " " << field_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}