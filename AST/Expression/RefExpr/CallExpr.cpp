//
// Created by Jonas Zell on 21.06.17.
//

#include <iostream>
#include "CallExpr.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../StdLib/Objects/Object.h"
#include "../../../Util.h"

CallExpr::CallExpr() :
    _arguments(std::vector<Expression::SharedPtr>())
{

}

CallExpr::CallExpr(const CallExpr& cp) {
    _return_ref = cp._return_ref;
    if (cp._member_expr != nullptr) {
        _member_expr = std::static_pointer_cast<RefExpr>(cp._member_expr->clone());
    }
    _arguments = std::vector<Expression::SharedPtr>();
    for (auto arg : cp._arguments) {
        _arguments.push_back(std::static_pointer_cast<Expression>(arg->clone()));
    }
    set_parent(cp._parent);
}

AstNode::SharedPtr CallExpr::clone() const {
    return std::make_shared<CallExpr>(*this);
}

Variant CallExpr::evaluate(Variant obj) {
    Function::SharedPtr fun = std::dynamic_pointer_cast<Function>(obj.get<Object::SharedPtr>());
    if (fun == nullptr) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call value of type " + val::typetostr(obj.get_type()));
    }

    std::vector<Variant> _real_args;
    for (auto arg : _arguments) {
        auto arg_val = arg->evaluate();

        _real_args.push_back(arg_val);
    }

    Variant res = fun->call(_real_args);

    if (_member_expr != nullptr) {
        _member_expr->return_ref(_return_ref);
        return _member_expr->evaluate(res);
    }
    else {
        return _return_ref ? res : *res;
    }
}

void CallExpr::add_argument(Expression::SharedPtr arg) {
    _arguments.push_back(arg);
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (auto arg : _arguments) {
        res.push_back(arg);
    }
    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void CallExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "CallExpr" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}