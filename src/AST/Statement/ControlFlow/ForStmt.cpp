//
// Created by Jonas Zell on 25.06.17.
//

#include "ForStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ForStmt::ForStmt(Statement* init,
                 Expression* term,
                 Statement* inc)
   : Statement(ForStmtID), initialization(init), termination(term),
     increment(inc)
{

}

ForStmt::ForStmt(Statement* init,
                 Expression* term,
                 Statement* inc,
                 Statement* body)
   : Statement(ForStmtID), initialization(init), termination(term),
     increment(inc), body(body)
{

}

} // namespace ast
} // namespace cdot