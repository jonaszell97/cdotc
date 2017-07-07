//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "MemberRefExpr.h"
#include "../../../StdLib/Objects/Object.h"

MemberRefExpr::MemberRefExpr(std::string ident) :  _ident(ident) {

}

MemberRefExpr::MemberRefExpr(Variant val) : MemberRefExpr(val.get<std::string>()) {

}

std::vector<AstNode::SharedPtr> MemberRefExpr::get_children() {
    if (_member_expr != nullptr) {
        return { _member_expr };
    }
    else {
        return { };
    }
}

void MemberRefExpr::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "MemberRefExpr [" << _ident << "]" << std::endl;

    if (_member_expr != nullptr) {
        _member_expr->__dump(depth + 1);
    }
}