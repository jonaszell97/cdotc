//
// Created by Jonas Zell on 23.06.17.
//

#include <iostream>
#include "ExplicitCastExpr.h"
#include "../../Util.h"

ExplicitCastExpr::ExplicitCastExpr(std::string type) : _operator(type), _child{} {

}

Variant ExplicitCastExpr::evaluate(Variant) {
    return _child->evaluate().cast_to(util::typemap[_operator]);
}

std::vector<AstNode::SharedPtr> ExplicitCastExpr::get_children() {
    return { _child };
}

void ExplicitCastExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ExplicitCastExpr [(" << _operator << ")]" << std::endl;

    _child->__dump(depth + 1);
}