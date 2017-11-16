//
// Created by Jonas Zell on 28.06.17.
//

#include "MatchStmt.h"
#include "../../Expression/Expression.h"
#include "CaseStmt.h"

MatchStmt::MatchStmt(Expression::SharedPtr switch_val) :
   switchValue(switch_val),
   cases(std::vector<CaseStmt::SharedPtr>())
{
   children.push_back(&this->switchValue);
}

std::vector<AstNode::SharedPtr> MatchStmt::get_children() {
   std::vector<AstNode::SharedPtr> children = { switchValue };
   for (auto case_ : cases) {
      children.push_back(case_);
   }

   return children;
}