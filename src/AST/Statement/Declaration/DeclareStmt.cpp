//
// Created by Jonas Zell on 13.08.17.
//

#include "DeclareStmt.h"
#include "../../Expression/TypeRef.h"
#include "FuncArgDecl.h"

DeclareStmt::DeclareStmt(ExternKind kind)
{
   externKind = kind;
}

std::vector<AstNode::SharedPtr> DeclareStmt::get_children() {
   std::vector<AstNode::SharedPtr> children;
   children.insert(children.begin(), declarations.begin(), declarations.end());

   return children;
}

void DeclareStmt::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "DeclareStmt []" << std::endl;
}