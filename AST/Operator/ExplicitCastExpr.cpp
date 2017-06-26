//
// Created by Jonas Zell on 23.06.17.
//

#include <iostream>
#include "ExplicitCastExpr.h"
#include "../../Util.h"

ExplicitCastExpr::ExplicitCastExpr(std::string type) : _operator(type), _child{} {

}

ExplicitCastExpr::ExplicitCastExpr(const ExplicitCastExpr& cp) {
    _operator = cp._operator;
    _child = std::static_pointer_cast<Expression>(cp._child->clone());
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr ExplicitCastExpr::clone() const {
    return std::make_shared<ExplicitCastExpr>(*this);
}

Variant ExplicitCastExpr::evaluate(Variant) {
    return _child->evaluate().cast_to(util::typemap[_operator]);
}

std::vector<AstNode::SharedPtr> ExplicitCastExpr::get_children() {
    if (_child != nullptr) {
        return {_child};
    }

    return {};
}

void ExplicitCastExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ExplicitCastExpr [(" << _operator << ")]" << std::endl;

    _child->__dump(depth + 1);
}