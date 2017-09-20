//
// Created by Jonas Zell on 19.06.17.
//

#include "WhileStmt.h"
#include "../../Expression/Expression.h"

WhileStmt::WhileStmt(Expression::SharedPtr cond, Statement::SharedPtr while_block, bool atLeastOnce) :
   condition(cond),
   body(while_block),
   atLeastOnce(atLeastOnce)
{
   children.push_back(&condition);
}

std::vector<AstNode::SharedPtr> WhileStmt::get_children() {
   std::vector<AstNode::SharedPtr> children{ body };
   return { condition, body };
}

void WhileStmt::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "WhileStmt" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}