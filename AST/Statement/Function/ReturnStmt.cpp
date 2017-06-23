//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "ReturnStmt.h"
#include "../CompoundStmt.h"

#include <iostream>

ReturnStmt::ReturnStmt(Expression::SharedPtr return_val) : _return_val(return_val) {

}

VariantPtr ReturnStmt::evaluate(VariantPtr) {
    if (auto root = _root.lock()) {
        root->terminate(_return_val->evaluate());
    }
    else {
        RuntimeError::raise(ERR_MISSING_CONTEXT, "No context to return from");
    }

    return { };
}

std::vector<AstNode::SharedPtr> ReturnStmt::get_children() {
    return { _return_val };
}

void ReturnStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ReturnStmt" << std::endl;

    _return_val->__dump(depth + 1);
}