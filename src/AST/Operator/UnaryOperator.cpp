//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "UnaryOperator.h"
#include "../../Variant/Variant.h"
#include "../../Util.h"

UnaryOperator::UnaryOperator(std::string op, std::string fix) :
        _operator(op),
        prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(Variant v, std::string fix) : UnaryOperator(v.get<std::string>(), fix) {

}

std::string UnaryOperator::get_operator() {
    return _operator;
}

std::vector<AstNode::SharedPtr> UnaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_child);
    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void UnaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "UnaryOperator ['" << _operator << "']" << std::endl;

    for (auto child : get_children()) {
        child->__dump(depth + 1);
    }
}