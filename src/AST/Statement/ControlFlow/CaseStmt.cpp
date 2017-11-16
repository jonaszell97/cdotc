//
// Created by Jonas Zell on 28.06.17.
//

#include "CaseStmt.h"
#include "../../Expression/Expression.h"

CaseStmt::CaseStmt(std::shared_ptr<Expression> case_val) : LabelStmt("case") {
   caseVal = case_val;
   children.push_back(&caseVal);
}

CaseStmt::CaseStmt() : LabelStmt("default") {
   isDefault = true;
}

std::vector<AstNode::SharedPtr> CaseStmt::get_children() {
   std::vector<AstNode::SharedPtr> children;
   if (caseVal != nullptr) {
      children.push_back(caseVal);
   }
   for (auto child : LabelStmt::get_children()) {
      children.push_back(child);
   }

   return children;
}