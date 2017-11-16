//
// Created by Jonas Zell on 02.07.17.
//

#include "UsingStmt.h"

UsingStmt::UsingStmt(
   string&& importNamespace,
   std::vector<string>&& item) :
   importNamespace(importNamespace),
   importedItems(item)
{
   
}

UsingStmt::UsingStmt(std::vector<string> &&fullNames) : fullNames(fullNames)
{

}

std::vector<AstNode::SharedPtr> UsingStmt::get_children() {
   return { };
}