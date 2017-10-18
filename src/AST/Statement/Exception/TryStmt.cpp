//
// Created by Jonas Zell on 05.10.17.
//

#include "TryStmt.h"

#include "../../Expression/TypeRef.h"

TryStmt::TryStmt(Statement::SharedPtr&& body)
   : body(body)
{

}

std::vector<AstNode::SharedPtr> TryStmt::get_children()
{
   std::vector<AstNode::SharedPtr> children;
   for (const auto& catch_ : catchBlocks) {
      children.push_back(catch_.caughtType);
      children.push_back(catch_.body);
   }

   if (finallyBlock != nullptr) {
      children.push_back(finallyBlock);
   }

   return children;
}

void TryStmt::__dump(int depth)
{

}