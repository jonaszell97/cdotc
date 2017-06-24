//
// Created by Jonas Zell on 21.06.17.
//

#include <iostream>
#include "CallExpr.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../Objects/Object.h"
#include "../../../Util.h"

CallExpr::CallExpr() :
    _arguments(std::vector<Expression::SharedPtr>()),
    _member_expr{}
{

}

Variant CallExpr::evaluate(Variant obj) {
    if (obj.get_type() != FUNCTION_T) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call value of type " + util::types[obj.get_type()]);
    }

    std::vector<Variant> _real_args;
    for (auto arg : _arguments) {
        auto arg_val = arg->evaluate();

        _real_args.push_back(arg_val);
    }

    auto res = obj.get<Function::SharedPtr>()->call(_real_args);

    if (_member_expr != nullptr) {
        return _member_expr->evaluate(res);
    }
    else {
        return res;
    }
}

void CallExpr::set_member_expr(std::shared_ptr<RefExpr> mem_expr) {
    _member_expr = mem_expr;
}

void CallExpr::return_ref(bool ref) {
    _return_ref = ref;
}

void CallExpr::add_argument(Expression::SharedPtr arg) {
    _arguments.push_back(arg);
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (auto arg : _arguments) {
        res.push_back(arg);
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