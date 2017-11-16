//
// Created by Jonas Zell on 19.06.17.
//

#include "AstNode.h"
#include "Statement/Declaration/DeclStmt.h"

AstNode::AstNode() {

}

bool AstNode::hasAttribute(Attr kind) const
{
   for (const auto& attr : attributes) {
      if (attr.kind == kind) {
         return true;
      }
   }

   return false;
}

void AstNode::replaceUsesWith(Expression *expr)
{
   assert(parent && "no parent to replace from");
   parent->replaceChildWith(this, expr);
}

void AstNode::isReturnValue()
{
   returned_value = true;
   if (declaration != nullptr) {
      declaration->isReturnValue();
   }
}

void AstNode::isHiddenReturnValue()
{
   returned_value = true;
   sret_value = true;
   if (declaration != nullptr) {
      declaration->isHiddenReturnValue();
   }
}

std::vector<AstNode::SharedPtr> AstNode::get_children() {
   return std::vector<AstNode::SharedPtr>();
}