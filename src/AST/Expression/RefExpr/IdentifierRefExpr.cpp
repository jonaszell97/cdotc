//
// Created by Jonas Zell on 19.06.17.
//

#include "IdentifierRefExpr.h"

IdentifierRefExpr::IdentifierRefExpr(string ident) {
    this->ident = ident;
}

std::vector<AstNode::SharedPtr> IdentifierRefExpr::get_children() {
    if (memberExpr != nullptr) {
        return { memberExpr };
    }
    else {
        return { };
    }
}

void IdentifierRefExpr::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "Identifier" << (lvalue ? "Ref" : "") << "Expr" << " [" << ident << "]" << std::endl;

    if (memberExpr != nullptr) {
        memberExpr->__dump(depth + 1);
    }
}