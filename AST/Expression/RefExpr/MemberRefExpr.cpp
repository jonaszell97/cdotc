//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "MemberRefExpr.h"
#include "../../../StdLib/Objects/Object.h"

MemberRefExpr::MemberRefExpr(std::string ident) :  _ident(ident) {

}

MemberRefExpr::MemberRefExpr(Variant val) : MemberRefExpr(val.get<std::string>()) {

}

MemberRefExpr::MemberRefExpr(const MemberRefExpr& cp) {
    _ident = cp._ident;
    _return_ref = cp._return_ref;
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    set_parent(cp._parent);
}

AstNode::SharedPtr MemberRefExpr::clone() const {
    return std::make_shared<MemberRefExpr>(*this);
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