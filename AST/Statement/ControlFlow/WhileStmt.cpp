//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "WhileStmt.h"
#include "../../../Parser.h"
#include "../../../Option.h"

WhileStmt::WhileStmt(Expression::SharedPtr cond, CompoundStmt::SharedPtr while_block) :
    _condition(cond),
    _while_block(while_block)
{

}

WhileStmt::WhileStmt(const WhileStmt& cp) {
    _condition = std::static_pointer_cast<Expression>(cp._condition->clone());
    _while_block = std::static_pointer_cast<CompoundStmt>(cp._while_block->clone());
    set_parent(cp._parent);
}

AstNode::SharedPtr WhileStmt::clone() const {
    return std::make_shared<WhileStmt>(*this);
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