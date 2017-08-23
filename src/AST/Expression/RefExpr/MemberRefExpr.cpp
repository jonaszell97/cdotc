//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"

MemberRefExpr::MemberRefExpr(std::string ident) {
    this->ident = ident;
}

std::vector<AstNode::SharedPtr> MemberRefExpr::get_children() {
    if (memberExpr != nullptr) {
        return { memberExpr };
    }
    else {
        return { };
    }
}

void MemberRefExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MemberRefExpr [" << ident << "]\n";

    if (memberExpr != nullptr) {
        memberExpr->__dump(depth + 1);
    }
}