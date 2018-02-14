//
// Created by Jonas Zell on 19.06.17.
//

#include "IfStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

IfStmt::IfStmt(Expression* cond,
               Statement* body)
   : Statement(IfStmtID), condition(cond), ifBranch(body),
     elseBranch{}
{

}

IfStmt::IfStmt(Expression* cond,
               Statement* body,
               Statement* elseBody)
   : Statement(IfStmtID), condition(cond), ifBranch(body),
     elseBranch(elseBody)
{

}

} // namespace ast
} // namespace cdot