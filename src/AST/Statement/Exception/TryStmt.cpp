//
// Created by Jonas Zell on 05.10.17.
//

#include "TryStmt.h"

#include "../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

TryStmt::TryStmt(Statement::SharedPtr&& body)
   : Statement(TryStmtID), body(move(body))
{

}

TryStmt::TryStmt(std::shared_ptr<Statement> &&body,
                 std::vector<CatchBlock> &&catchBlocks,
                 std::shared_ptr<Statement> &&finally)
   : Statement(TryStmtID), body(move(body)),
     catchBlocks(std::move(catchBlocks)), finallyBlock(move(finally))
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