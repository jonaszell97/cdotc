//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../Expression/TypeRef.h"

FieldDecl::FieldDecl(std::string field_name, std::shared_ptr<TypeRef> type, AccessModifier am, bool is_static,
         Expression::SharedPtr def_val):
    fieldName(field_name),
    type(type),
    am(am),
    isStatic(is_static),
    defaultVal(def_val)
{

}

std::vector<AstNode::SharedPtr> FieldDecl::get_children() {
    return (defaultVal != nullptr) ? std::vector<AstNode::SharedPtr>{ defaultVal } : std::vector<AstNode::SharedPtr>{};
}

void FieldDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::string get_set = "";
    if (hasGetter || hasSetter) {
        get_set = " (";
        if (hasGetter) {
            get_set += "get,";
        }
        if (hasSetter) {
            get_set += "set,";
        }
        get_set.pop_back();
        get_set += ")";
    }

    std::cout << (isStatic ? "Static" : "") << "FieldDecl [" << util::am_map[am] << " " << type->toString()
              << " " << fieldName << get_set << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}