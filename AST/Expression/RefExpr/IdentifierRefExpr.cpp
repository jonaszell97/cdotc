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

IdentifierRefExpr::IdentifierRefExpr(const IdentifierRefExpr& cp) {
    _ident = cp._ident;
    _return_ref = cp._return_ref;
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    context = std::make_shared<Context>(*cp.context);
    set_parent(cp._parent);
}

AstNode::SharedPtr IdentifierRefExpr::clone() const {
    return std::make_shared<IdentifierRefExpr>(*this);
}

Variant IdentifierRefExpr::evaluate(Variant opt) {
    if (context != nullptr) {
        Variant var;
        if (GlobalContext::is_declared_class(_ident)) {
            var = { GlobalContext::get_class(_ident) };
        }
        else {
            Variant::SharedPtr ref = context->get_variable(_ident);
            var = _return_ref ? ref : *ref;
        }

        if (_member_expr == nullptr) {
            return var;
        } else {
            _member_expr->return_ref(_return_ref);
            return _member_expr->evaluate(var);
        }
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "No context to get variable " + _ident + " from.");
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
    if (_implicit_ref) {
        std::cout << "ImplicitRefExpr" << " [" << _ident << "]" << std::endl;
    }
    else {
        std::cout << "Identifier" << (_return_ref ? "Ref" : "") << "Expr" << " [" << _ident << "]" << std::endl;
    }

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}