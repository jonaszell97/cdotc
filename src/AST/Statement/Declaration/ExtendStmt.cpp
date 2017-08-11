//
// Created by Jonas Zell on 31.07.17.
//

#include "ExtendStmt.h"

ExtendStmt::ExtendStmt(std::string extended, std::vector<FieldDecl::SharedPtr> fields,
    std::vector<MethodDecl::SharedPtr> methods) : fields(fields), methods(methods), extended_class(extended) {

}

std::vector<AstNode::SharedPtr> ExtendStmt::get_children() {
    return {};
}

void ExtendStmt::__dump(int depth) {
    AstNode::__tab(depth);
}