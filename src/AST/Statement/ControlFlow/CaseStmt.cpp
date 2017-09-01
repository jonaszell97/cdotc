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

CaseStmt::~CaseStmt() {
   delete genericDestTy;
   delete genericOriginTy;
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

void CaseStmt::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << (isDefault ? "DefaultStmt" : "CaseStmt") << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}