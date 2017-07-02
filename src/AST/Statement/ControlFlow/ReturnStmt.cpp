//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "ReturnStmt.h"
#include "../CompoundStmt.h"

#include <iostream>

ReturnStmt::ReturnStmt(Expression::SharedPtr return_val) : _return_val(return_val) {

}

ReturnStmt::ReturnStmt(const ReturnStmt& cp) {
    _return_val = std::static_pointer_cast<Expression>(cp._return_val->clone());
    func = cp.func;
    set_parent(cp._parent);
}

AstNode::SharedPtr ReturnStmt::clone() const {
    return std::make_shared<ReturnStmt>(*this);
}

std::vector<AstNode::SharedPtr> ReturnStmt::get_children() {
    return { _return_val };
}

void ReturnStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ReturnStmt" << std::endl;

    _return_val->__dump(depth + 1);
}