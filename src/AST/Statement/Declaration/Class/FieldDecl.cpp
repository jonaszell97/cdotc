//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../../Util.h"

#include <iostream>

FieldDecl::FieldDecl(std::string field_name, TypeRef::SharedPtr type, AccessModifier am, bool is_static,
         Expression::SharedPtr def_val):
    field_name(field_name),
    type(type),
    am(am),
    is_static(is_static),
    default_val(def_val)
{

}

std::vector<AstNode::SharedPtr> FieldDecl::get_children() {
    return (default_val != nullptr) ? std::vector<AstNode::SharedPtr>{ default_val } : std::vector<AstNode::SharedPtr>{};
}

void FieldDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::string get_set = "";
    if (generate_getter || generate_setter) {
        get_set = " (";
        if (generate_getter) {
            get_set += "get,";
        }
        if (generate_setter) {
            get_set += "set,";
        }
        get_set.pop_back();
        get_set += ")";
    }

    std::cout << (is_static ? "Static" : "") << "FieldDecl [" << util::am_map[am] << " " << type->to_string()
              << " " << field_name << get_set << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}