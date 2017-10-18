//
// Created by Jonas Zell on 19.06.17.
//

#include "CompoundStmt.h"

CompoundStmt::CompoundStmt(bool keep_scope)
   : preserveScope(keep_scope)
{

}

void CompoundStmt::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   for (auto &stmt : statements) {
      if (stmt.get() == child) {
         stmt.reset((Statement*)replacement);
         return;
      }
   }

   llvm_unreachable("child does not exist");
}

void CompoundStmt::addStatement(Statement::SharedPtr stmt) {
   statements.emplace_back(stmt);
}

void CompoundStmt::runPasses()
{
   for (auto& pass : passes) {
      pass->visit(this);
      pass->finalize();

      delete pass;
   }
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