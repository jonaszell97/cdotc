//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "InputStmt.h"
#include "../../../Variant/Variant.h"
#include "../CompoundStmt.h"
#include "../../Expression/Literal/LiteralExpr.h"
#include "../../../Util.h"
#include "../../Visitor/Visitor.h"

InputStmt::InputStmt(std::string ident) : _ident(ident) {

}

std::vector<AstNode::SharedPtr> InputStmt::get_children() {
    return {};
}

void InputStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "InputStmt ['" << _ident << "']" << std::endl;
}