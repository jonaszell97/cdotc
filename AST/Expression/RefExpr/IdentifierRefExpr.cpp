//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "IdentifierRefExpr.h"
#include "../../../Variant.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../Util.h"
#include "MemberRefExpr.h"

IdentifierRefExpr::IdentifierRefExpr(std::string ident) : _ident(ident) {

}

IdentifierRefExpr::IdentifierRefExpr(Variant val) : IdentifierRefExpr(val.s_val) {

}

void IdentifierRefExpr::set_member_expr(RefExpr::SharedPtr mem_expr) {
    _member_expr = mem_expr;
}

void IdentifierRefExpr::return_ref(bool ref) {
    _return_ref = ref;
}

VariantPtr IdentifierRefExpr::evaluate(VariantPtr opt) {
    // return a reference to the variable instead of the value
    if (auto root = _root.lock()) {
        if (_member_expr == nullptr) {
            return root->get_var_or_func(_ident);
        } else {
            VariantPtr obj = root->get_var_or_func(_ident);
            if (obj->type != OBJECT_T && obj->type != ARRAY_T && obj->type != FUNCTION_T) {
                RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on primitive value.");
            }

            return _member_expr->evaluate(obj);
        }
    }
    else {
        RuntimeError::raise(ERR_MISSING_CONTEXT, "No context to get variable " + _ident + " from.");
    }
}

std::vector<AstNode::SharedPtr> IdentifierRefExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }
    else {
        return { };
    }
}

void IdentifierRefExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "IdentifierRefExpr" << " [" << _ident << "]" << std::endl;

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}