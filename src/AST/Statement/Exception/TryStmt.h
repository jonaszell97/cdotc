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
   CatchBlock(std::shared_ptr<TypeRef> &&caughtType,
              string &&identifier, Statement::SharedPtr &&body)
      : caughtType(move(caughtType)), identifier(move(identifier)),
        body(move(body))
   { }

   CatchBlock() = default;

   std::shared_ptr<TypeRef> caughtType;
   string identifier;

   ast::Statement::SharedPtr body;

   bool needsCast = false;
   QualType castTo;
};

class TryStmt: public Statement {
public:
   explicit TryStmt(Statement::SharedPtr &&body);

   TryStmt(std::shared_ptr<Statement> &&body,
           std::vector<CatchBlock> &&catchBlocks,
           std::shared_ptr<Statement> &&finally);

   void addCatch(CatchBlock&& catchBlock)
   {
      catchBlocks.push_back(catchBlock);
   }

   void setFinally(Statement::SharedPtr&& finallyBlock)
   {
      this->finallyBlock = finallyBlock;
   }

   typedef std::shared_ptr<TryStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TryStmtID;
   }

protected:
   Statement::SharedPtr body;

   std::vector<CatchBlock> catchBlocks;
   Statement::SharedPtr finallyBlock = nullptr;

public:
   const Statement::SharedPtr &getBody() const;
   std::vector<CatchBlock> &getCatchBlocks();
   const Statement::SharedPtr &getFinallyBlock() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TRYSTMT_H
