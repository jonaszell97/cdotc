//
// Created by Jonas Zell on 19.06.17.
//

#include "DeclStmt.h"
#include "../../Expression/TypeRef.h"


DeclStmt::DeclStmt(string identifier, TypeRef::SharedPtr type, bool is_const, bool is_global, Expression::SharedPtr val)
    : identifier(identifier),
      type(type),
      is_const(is_const),
      is_global(is_global),
      value(val)
{

}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
    std::vector<AstNode::SharedPtr> children{ type };
    if (value) {
        children.push_back(value);
    }

    return children;
}

void DeclStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << (is_const ? "Const" : "") << "DeclStmt [" << identifier << ": " +
        type->getType(true)->toString() + "]" << std::endl;

    if (value) {
        value->__dump(depth + 1);
    }
}