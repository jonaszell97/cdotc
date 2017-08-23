//
// Created by Jonas Zell on 15.08.17.
//

#include "LvalueToRvalue.h"


LvalueToRvalue::LvalueToRvalue()
{

}

LvalueToRvalue::LvalueToRvalue(Expression::SharedPtr expr) :
    target(expr)
{
    children.push_back(&target);
    target->setParent(this);
}

std::vector<AstNode::SharedPtr> LvalueToRvalue::get_children() {
    return { target };
}

void LvalueToRvalue::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "ImplicitLvalueToRvalue" << std::endl;

    target->__dump(depth + 1);
}