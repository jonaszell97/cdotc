//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "WhileStmt.h"
#include "../../../Parser.h"

WhileStmt::WhileStmt(Expression::SharedPtr cond, CompoundStmt::SharedPtr while_block) :
    _condition(cond),
    _while_block(while_block)
{

}

std::vector<AstNode::SharedPtr> WhileStmt::get_children() {
    return { _condition, _while_block };
}

void WhileStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "WhileStmt" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}