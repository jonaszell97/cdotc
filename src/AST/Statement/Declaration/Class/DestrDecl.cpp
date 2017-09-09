//
// Created by Jonas Zell on 02.09.17.
//

#include "DestrDecl.h"
#include "../../Block/CompoundStmt.h"

DestrDecl::DestrDecl(std::shared_ptr<CompoundStmt> &&body) :
   body(body)
{

}

std::vector<std::shared_ptr<AstNode>> DestrDecl::get_children() {
   return { body };
}

void DestrDecl::__dump(int depth) {
   __tab(depth);
   std::cout << "DestrDecl\n";
   body->__dump(depth + 1);
}