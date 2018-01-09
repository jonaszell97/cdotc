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

IfStmt::IfStmt(Expression::SharedPtr &&cond,
               Statement::SharedPtr &&body,
               Statement::SharedPtr &&elseBody)
   : Statement(IfStmtID), condition(move(cond)), ifBranch(move(body)),
     elseBranch(move(elseBody))
{

}

} // namespace ast
} // namespace cdot