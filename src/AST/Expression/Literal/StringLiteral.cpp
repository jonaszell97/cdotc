//
// Created by Jonas Zell on 30.06.17.
//

#include "StringLiteral.h"

StringLiteral::StringLiteral(std::string value)
   : value(value) {

}

std::vector<AstNode::SharedPtr> StringLiteral::get_children() {
   return memberExpr == nullptr ? std::vector<AstNode::SharedPtr>()
                                : std::vector<AstNode::SharedPtr>{memberExpr };
}