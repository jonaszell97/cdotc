//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "DeclStmt.h"
#include "../Block/CompoundStmt.h"
#include "../../../Util.h"
#include "../../Expression/Literal/ArrayLiteral.h"


DeclStmt::DeclStmt(string identifier, TypeRef::SharedPtr type, bool is_const, bool is_global, Expression::SharedPtr val)
    : identifier(identifier),
      type(type),
      is_const(is_const),
      is_global(is_global),
      value(val)
{

}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    if (value) {
        children.push_back(value);
    }

    return children;
}

void DeclStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_const ? "Const" : "") << "DeclStmt [" << identifier << ": " +
        type->getTypeSpecifier().to_string() + "]" << std::endl;

    if (value) {
        value->__dump(depth + 1);
    }
}