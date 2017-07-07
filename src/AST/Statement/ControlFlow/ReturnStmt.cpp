//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "ReturnStmt.h"
#include "../CompoundStmt.h"

#include <iostream>

ReturnStmt::ReturnStmt(Expression::SharedPtr return_val) : _return_val(return_val) {

}

ReturnStmt::ReturnStmt() {

}

std::vector<AstNode::SharedPtr> ReturnStmt::get_children() {
    if (_return_val) {
        return { _return_val };
    }
    else {
        return {};
    }
}

void ReturnStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ReturnStmt" << std::endl;

    for (auto child : get_children()) {
        child->__dump(depth + 1);
    }
}