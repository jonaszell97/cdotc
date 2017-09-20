//
// Created by Jonas Zell on 30.08.17.
//

#include "NoneLiteral.h"

NoneLiteral::NoneLiteral() {

}

bool NoneLiteral::canReturn(Type *ty)
{
   return ty->isObject() && ty->getClassName() == "Option";
}

std::vector<AstNode::SharedPtr> NoneLiteral::get_children() {
   return {};
}

void NoneLiteral::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "NoneLiteral\n";
}