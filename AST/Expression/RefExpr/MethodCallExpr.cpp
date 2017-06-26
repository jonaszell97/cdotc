//
// Created by Jonas Zell on 25.06.17.
//

#include "MethodCallExpr.h"
#include "../../../StdLib/Objects/Object.h"

#include <iostream>

MethodCallExpr::MethodCallExpr(std::string ident) : _ident(ident) {

}

MethodCallExpr::MethodCallExpr(const MethodCallExpr& cp) {
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    set_parent(cp._parent);
}

MethodCallExpr::MethodCallExpr(const CallExpr& cp, std::string ident) {
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    _arguments = std::vector<Expression::SharedPtr>(cp._arguments);
    _ident = ident;
    set_parent(cp._parent);
}

AstNode::SharedPtr MethodCallExpr::clone() const {
    return std::make_shared<MethodCallExpr>(*this);
}

Variant MethodCallExpr::evaluate(Variant obj) {
    Variant v;
    std::vector<Variant> _real_args;
    for (auto arg : _arguments) {
        auto arg_val = arg->evaluate();

        _real_args.push_back(arg_val);
    }

    if (obj.get_type() == OBJECT_T) {
        v = obj.get<Object::SharedPtr>()->call_method(_ident, _real_args);
    }
    else if (obj.get_type() == CLASS_T) {
        v = obj.get<Class*>()->call_static_method(_ident, _real_args);
    }
    else if (val::base_class(obj.get_type()) != "") {
//        v = GlobalContext::get_class(val::base_class(obj.get_type()))->call_method(_ident, obj, _real_args);
    }
    else {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call method on primitve value.");
    }

    if (_member_expr == nullptr) {
        return _return_ref ? v : *v;
    }
    else {
        _member_expr->return_ref(_return_ref);
        return _member_expr->evaluate(v);
    }
}

std::vector<AstNode::SharedPtr> MethodCallExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }

    return {};
}

void MethodCallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MethodCallExpr ['" << _ident << "']" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}