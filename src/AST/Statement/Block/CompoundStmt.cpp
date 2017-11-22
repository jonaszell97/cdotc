//
// Created by Jonas Zell on 19.06.17.
//

#include "CompoundStmt.h"

namespace cdot {
namespace ast {

CompoundStmt::CompoundStmt(bool keep_scope)
   : Statement(CompoundStmtID), preserveScope(keep_scope)
{

}

void CompoundStmt::addStatement(Statement::SharedPtr &&stmt)
{
   statements.push_back(std::move(stmt));
}

} // namespace ast
} // namespace cdot