//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "MemberRefExpr.h"
#include "../../../Objects/Object.h"

MemberRefExpr::MemberRefExpr(std::string ident) : _parent_obj{}, _ident(ident) {

}

MemberRefExpr::MemberRefExpr(Variant val) : MemberRefExpr(val.get<std::string>()) {

}

void MemberRefExpr::set_member_expr(std::shared_ptr<RefExpr> member_expr) {
    _member_expr = member_expr;
}

void MemberRefExpr::return_ref(bool ref) {
    _return_ref = ref;
}

Variant MemberRefExpr::evaluate(Variant parent) {
    if (_member_expr == nullptr) {
        return parent.get<Object::SharedPtr>()->access_property(_ident);
    }
    else {
        if (parent.get_type() != OBJECT_T) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on primitve value.");
        }

        Variant current = parent.get<Object::SharedPtr>()->access_property(_ident);

        return _member_expr->evaluate(current);
    }
}

std::vector<AstNode::SharedPtr> MemberRefExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }
    else {
        return { };
    }
}

void MemberRefExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MemberRefExpr [" << _ident << "]" << std::endl;

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}