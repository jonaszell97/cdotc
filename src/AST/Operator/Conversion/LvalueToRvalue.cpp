//
// Created by Jonas Zell on 15.08.17.
//

#include "LvalueToRvalue.h"


LvalueToRvalue::LvalueToRvalue(Expression::SharedPtr expr) :
    target(expr)
{
    children.push_back(&target);
    target->setParent(this);
}

std::vector<AstNode::SharedPtr> LvalueToRvalue::get_children() {
    return { target };
}