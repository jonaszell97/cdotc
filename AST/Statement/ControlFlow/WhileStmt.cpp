//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "WhileStmt.h"
#include "../../../Interpreter.h"
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

Variant WhileStmt::evaluate(Variant) {
    int i = 0;
    while (!_broke && _condition->evaluate().get<bool>()) {
        if (++i > cdot::opt::options[cdot::opt::CompilerOption::OPT_MAX_WHILE_COUNT]) {
            break;
        }

        _current_instance = new CompoundStmt(*_while_block);
        _current_instance->evaluate();
        delete _current_instance;
    }

    return { };
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