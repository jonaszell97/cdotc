//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "IdentifierRefExpr.h"
#include "../../../Variant/Variant.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../Util.h"
#include "MemberRefExpr.h"

IdentifierRefExpr::IdentifierRefExpr(std::string ident) : _ident(ident) {

}

IdentifierRefExpr::IdentifierRefExpr(Variant val) : IdentifierRefExpr(val.get<std::string>()) {

}

void IdentifierRefExpr::set_member_expr(RefExpr::SharedPtr mem_expr) {
    _member_expr = mem_expr;
}

void IdentifierRefExpr::return_ref(bool ref) {
    _return_ref = ref;
}

Variant IdentifierRefExpr::evaluate(Variant opt) {
    if (auto root = _root.lock()) {
        Variant ref = root->has_variable(_ident) ? root->get_variable(_ident) : root->get_function(_ident);
        Variant var = _return_ref ? ref : ref.dereference();

        if (_member_expr == nullptr) {
            return var;
        } else {
            if (var.get_type() != OBJECT_T && var.get_type() != ARRAY_T && var.get_type() != FUNCTION_T) {
                RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on primitive value.");
            }

            _member_expr->return_ref(_return_ref);
            return _member_expr->evaluate(var);
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

    std::cout << "Identifier" << (_return_ref ? "Ref" : "") << "Expr" << " [" << _ident << "]" << std::endl;

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}