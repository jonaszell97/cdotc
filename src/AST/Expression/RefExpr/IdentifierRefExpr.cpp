//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "IdentifierRefExpr.h"
#include "../../../Variant/Variant.h"
#include "../../Statement/CompoundStmt.h"
#include "../../../Util.h"
#include "MemberRefExpr.h"

IdentifierRefExpr::IdentifierRefExpr(std::string ident) : _ident(ident) {

}

IdentifierRefExpr::IdentifierRefExpr(Variant val) : IdentifierRefExpr(val.get<std::string>()) {

}

std::vector<AstNode::SharedPtr> IdentifierRefExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }
    else {
        return { };
    }
}

void IdentifierRefExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    if (_implicit_ref) {
        std::cout << "ImplicitRefExpr" << " [" << _ident << "]" << std::endl;
    }
    else {
        std::cout << "Identifier" << (_return_ref ? "Ref" : "") << "Expr" << " [" << _ident << "]" << std::endl;
    }

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}