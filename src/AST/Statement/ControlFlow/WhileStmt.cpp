//
// Created by Jonas Zell on 19.06.17.
//

#include "WhileStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

WhileStmt::WhileStmt(std::shared_ptr<Expression> &&cond,
                     std::shared_ptr<Statement> &&body,
                     bool atLeastOnce)
   : Statement(WhileStmtID), condition(move(cond)), body(move(body)),
     atLeastOnce(atLeastOnce)
{

}

bool WhileStmt::isAtLeastOnce() const
{
   return atLeastOnce;
}

} // namespace ast
} // namespace cdot