//
// Created by Jonas Zell on 19.06.17.
//

#include "IfStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

IfStmt::IfStmt(Expression::SharedPtr &&cond,
               Statement::SharedPtr &&body)
   : Statement(IfStmtID), condition(move(cond)), ifBranch(move(body)),
     elseBranch{}
{

}

std::shared_ptr<Expression> &IfStmt::getCondition()
{
   return condition;
}

void IfStmt::setCondition(const std::shared_ptr<Expression> &condition)
{
   IfStmt::condition = condition;
}

const Statement::SharedPtr &IfStmt::getIfBranch() const
{
   return ifBranch;
}

void IfStmt::setIfBranch(const Statement::SharedPtr &ifBranch)
{
   IfStmt::ifBranch = ifBranch;
}

const Statement::SharedPtr &IfStmt::getElseBranch() const
{
   return elseBranch;
}

} // namespace ast
} // namespace cdot