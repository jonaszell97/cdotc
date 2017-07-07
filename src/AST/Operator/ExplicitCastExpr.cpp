//
// Created by Jonas Zell on 23.06.17.
//

#include <iostream>
#include "ExplicitCastExpr.h"
#include "../../Util.h"

ExplicitCastExpr::ExplicitCastExpr(std::string type) : _operator(type), _child{} {

}

std::vector<AstNode::SharedPtr> ExplicitCastExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (_child != nullptr) {
        children.push_back(_child);
    }
    if (_member_expr != nullptr) {
        children.push_back(_member_expr);
    }

    return children;
}

void ExplicitCastExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ExplicitCastExpr [(" << _operator << ")]" << std::endl;

    _child->__dump(depth + 1);
}