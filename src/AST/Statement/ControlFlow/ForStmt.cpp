//
// Created by Jonas Zell on 25.06.17.
//

#include "ForStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ForStmt::ForStmt(Statement::SharedPtr &&init,
                 std::shared_ptr<Expression> &&term,
                 Statement::SharedPtr &&inc)
   : Statement(ForStmtID), initialization(move(init)), termination(move(term)),
     increment(move(inc))
{

}

} // namespace ast
} // namespace cdot