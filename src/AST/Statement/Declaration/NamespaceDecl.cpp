//
// Created by Jonas Zell on 02.07.17.
//

#include "NamespaceDecl.h"
#include "../Block/CompoundStmt.h"


NamespaceDecl::NamespaceDecl(std::string ns_name, CompoundStmt::SharedPtr content, bool anonymous) :
   nsName(ns_name),
   contents(content),
   isAnonymousNamespace_(anonymous)
{

}

std::vector<AstNode::SharedPtr> NamespaceDecl::get_children() {
   return { contents };
}

void NamespaceDecl::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout <<  "NameSpaceDecl [" << nsName << "]" << std::endl;

   contents->__dump(depth + 1);
}