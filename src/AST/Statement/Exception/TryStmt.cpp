//
// Created by Jonas Zell on 05.10.17.
//

#include "TryStmt.h"

#include "../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

TryStmt::TryStmt(Statement::SharedPtr&& body)
   : Statement(TryStmtID), body(body)
{

}

const Statement::SharedPtr &TryStmt::getBody() const
{
   return body;
}

std::vector<CatchBlock> &TryStmt::getCatchBlocks()
{
   return catchBlocks;
}

const Statement::SharedPtr &TryStmt::getFinallyBlock() const
{
   return finallyBlock;
}

} // namespace ast
} // namespace cdot