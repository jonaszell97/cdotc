//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "UnaryOperator.h"
#include "../../Variant/Variant.h"
#include "../../StdLib/Objects/Object.h"
#include "../../Util.h"

UnaryOperator::UnaryOperator(std::string op, std::string fix) :
        _operator(op),
        prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(Variant v, std::string fix) : UnaryOperator(v.get<std::string>(), fix) {

}

UnaryOperator::UnaryOperator(const UnaryOperator& cp) {
    _operator = cp._operator;
    _child = std::static_pointer_cast<Expression>(cp._child->clone());
    set_parent(cp._parent);
}

AstNode::SharedPtr UnaryOperator::clone() const {
    return std::make_shared<UnaryOperator>(*this);
}

std::string UnaryOperator::get_operator() {
    return _operator;
}

std::vector<AstNode::SharedPtr> UnaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_child);

    return res;
}

void UnaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "UnaryOperator ['" << _operator << "']" << std::endl;

    _child->__dump(depth + 1);
}