//
// Created by Jonas Zell on 19.06.17.
//

#include "CompoundStmt.h"
#include "../UsingStmt.h"

namespace cdot {
namespace ast {

CompoundStmt::CompoundStmt(bool keep_scope)
   : Statement(CompoundStmtID), preserveScope(keep_scope)
{

}

CompoundStmt::CompoundStmt(std::vector<Statement* > &&stmts,
                           bool preserveScope)
   : Statement(CompoundStmtID), statements(std::move(stmts)),
     preserveScope(preserveScope)
{

}

} // namespace ast
} // namespace cdot