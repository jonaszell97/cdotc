//
// Created by Jonas Zell on 19.06.17.
//

#include "CompoundStmt.h"

CompoundStmt::CompoundStmt(bool keep_scope)
   : preserveScope(keep_scope)
{

}

void CompoundStmt::addStatement(Statement::SharedPtr stmt) {
   statements.emplace_back(stmt);
}

std::vector<AstNode::SharedPtr> CompoundStmt::get_children() {
   std::vector<AstNode::SharedPtr> res;
   for (int i = 0; i < statements.size(); i++) {
      res.push_back(statements[i]);
   }

   return res;
}

void CompoundStmt::__dump(int depth) {
   for (int i = 0; i < depth; i++) {
      std::cout << "\t";
   }

   std::cout << "CompoundStmt" << std::endl;

   for (int i = 0; i < statements.size(); i++) {
      statements[i]->__dump(depth + 1);
   }
}