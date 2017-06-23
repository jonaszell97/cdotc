//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "MemberRefExpr.h"
#include "../../../Objects/Object.h"

MemberRefExpr::MemberRefExpr(std::string ident) : _parent_obj{}, _ident(ident) {

}

MemberRefExpr::MemberRefExpr(Variant val) : MemberRefExpr(val.s_val) {

}

void MemberRefExpr::set_member_expr(std::shared_ptr<RefExpr> member_expr) {
    _member_expr = member_expr;
}

void MemberRefExpr::return_ref(bool ref) {
    _return_ref = ref;
}

VariantPtr MemberRefExpr::evaluate(VariantPtr parent) {
    if (parent->type == OBJECT_PROP_T) {
        parent = parent->op_val->get();
    }

    if (_member_expr == nullptr) {
        return parent->o_val->access_property(_ident);
    }
    else {
        if (parent->type != OBJECT_T) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on primitve value.");
        }

        VariantPtr current = parent->o_val->access_property(_ident);

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