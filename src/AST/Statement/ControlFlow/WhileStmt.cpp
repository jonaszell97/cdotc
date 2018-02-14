//
// Created by Jonas Zell on 19.06.17.
//

#include "WhileStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

WhileStmt::WhileStmt(Expression* cond,
                     Statement* body,
                     bool atLeastOnce)
   : Statement(WhileStmtID), condition(cond), body(body),
     atLeastOnce(atLeastOnce)
{

}

bool WhileStmt::isAtLeastOnce() const
{
   return atLeastOnce;
}

} // namespace ast
} // namespace cdot