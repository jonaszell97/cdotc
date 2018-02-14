//
// Created by Jonas Zell on 05.10.17.
//

#ifndef CDOT_TRYSTMT_H
#define CDOT_TRYSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class TypeRef;

struct CatchBlock {
   CatchBlock(LocalVarDecl *varDecl, Statement* body)
      : varDecl(varDecl),
        body(body)
   { }

   CatchBlock() = default;

   LocalVarDecl *varDecl;
   Statement* body;
};

class TryStmt: public Statement {
public:
   explicit TryStmt(Statement* body)
      : Statement(TryStmtID), body(body)
   {}

   TryStmt(Statement* body,
           std::vector<CatchBlock> &&catchBlocks,
           Statement* finally)
      : Statement(TryStmtID), body(body),
        catchBlocks(std::move(catchBlocks)), finallyBlock(finally)
   {}

   void addCatch(CatchBlock const& catchBlock)
   {
      catchBlocks.push_back(catchBlock);
   }

   void setFinally(Statement* finallyBlock)
   {
      this->finallyBlock = finallyBlock;
   }

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TryStmtID;
   }

protected:
   Statement* body;

   std::vector<CatchBlock> catchBlocks;
   Statement* finallyBlock = nullptr;

public:
   Statement* getBody() const { return body; }

   std::vector<CatchBlock> &getCatchBlocks() { return catchBlocks; }
   std::vector<CatchBlock> const& getCatchBlocks() const
   {
      return catchBlocks;
   }

   Statement* getFinallyBlock() const { return finallyBlock; }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TRYSTMT_H
