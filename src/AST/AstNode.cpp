//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "Statement/Declaration/DeclStmt.h"

AstNode::AstNode() {

}

void AstNode::replaceUsesWith(Expression *expr)
{
   assert(parent && "no parent to replace from");
   parent->replaceChildWith(this, expr);
}

void AstNode::isReturnValue() {
   returned_value = true;
   if (declaration != nullptr) {
      declaration->isReturnValue();
   }
}

void AstNode::isHiddenReturnValue() {
   returned_value = true;
   sret_value = true;
   if (declaration != nullptr) {
      declaration->isHiddenReturnValue();
   }
}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
   return std::vector<AstNode::SharedPtr>();
}

void AstNode::__tab(int depth) {
   for (int i = 0; i < depth; i++) {
      std::cout << "\t";
   }

   printf("<%p> ", this);
}