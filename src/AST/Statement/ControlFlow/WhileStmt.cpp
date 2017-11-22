//
// Created by Jonas Zell on 19.06.17.
//

#include "WhileStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

WhileStmt::WhileStmt(Expression::SharedPtr &&cond,
                     Statement::SharedPtr &&body,
                     bool atLeastOnce)
   : Statement(WhileStmtID), condition(move(cond)), body(move(body)),
     atLeastOnce(atLeastOnce)
{

}

std::shared_ptr<Expression> &WhileStmt::getCondition()
{
   return condition;
}

const Statement::SharedPtr &WhileStmt::getBody() const
{
   return body;
}

bool WhileStmt::isAtLeastOnce() const
{
   return atLeastOnce;
}

} // namespace ast
} // namespace cdot