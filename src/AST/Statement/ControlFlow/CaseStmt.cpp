//
// Created by Jonas Zell on 28.06.17.
//

#include "CaseStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

CaseStmt::CaseStmt(Statement* body)
   : Statement(CaseStmtID),
     is_default(true), pattern(nullptr), body(body)
{

}

CaseStmt::CaseStmt(PatternExpr* pattern,
                   Statement* body)
   : Statement(CaseStmtID), pattern(pattern), body(body)
{

}

} // namespace ast
} // namespace cdot