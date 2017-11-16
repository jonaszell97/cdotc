//
// Created by Jonas Zell on 01.09.17.
//

#include "ForInStmt.h"
#include "../Declaration/DeclStmt.h"
#include "../../Expression/Expression.h"

ForInStmt::ForInStmt(std::shared_ptr<DeclStmt>&& decl, std::shared_ptr<Expression>&& range,
   std::shared_ptr<Statement>&& body) :
   decl(decl),
   rangeExpr(range),
   body(body)
{

}

std::vector<AstNode::SharedPtr> ForInStmt::get_children() {
   std::vector<AstNode::SharedPtr> children;
   children.push_back(decl);
   children.push_back(rangeExpr);
   children.push_back(body);

   return children;
}