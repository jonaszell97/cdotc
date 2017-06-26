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

Variant MemberRefExpr::evaluate(Variant parent) {
    Variant v;

    if (parent.get_type() == OBJECT_T) {
        auto obj = parent.get<Object::SharedPtr>();
        v = obj->access_property(_ident);
    }
    else if (parent.get_type() == CLASS_T) {
        auto cl = parent.get<Class*>();
        v = cl->access_static_property(_ident);
    }
    else if (val::base_class(parent.get_type()) != "") {
        v = GlobalContext::get_class(val::base_class(parent.get_type()))->access_static_property(_ident);
    }
    else {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on primitve value.");
    }

    if (_member_expr == nullptr) {
        return _return_ref ? v : *v;
    }
    else {
        _member_expr->return_ref(_return_ref);
        return _member_expr->evaluate(v);
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