//
// Created by Jonas Zell on 28.06.17.
//

#include "CaseStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

CaseStmt::CaseStmt(std::shared_ptr<Statement> &&body)
   : Statement(CaseStmtID), is_default(true), pattern(nullptr),
     body(move(body))
{

}

CaseStmt::CaseStmt(std::shared_ptr<PatternExpr> &&pattern,
                   std::shared_ptr<Statement> &&body)
   : Statement(CaseStmtID), pattern(move(pattern)), body(move(body))
{

}

} // namespace ast
} // namespace cdot