//
// Created by Jonas Zell on 19.06.17.
//

#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;

class IfStmt : public Statement {
public:
   IfStmt(std::shared_ptr<Expression> &&cond,
          Statement::SharedPtr &&body);

   IfStmt(std::shared_ptr<Expression> &&cond,
          Statement::SharedPtr &&body,
          Statement::SharedPtr &&elseBody);

   inline void setElseBranch(Statement::SharedPtr &&elseBr)
   {
      elseBranch = std::move(elseBr);
   }

   typedef std::shared_ptr<IfStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IfStmtID;
   }

protected:
   std::shared_ptr<Expression> condition;
   std::shared_ptr<Statement> ifBranch;
   std::shared_ptr<Statement> elseBranch;

public:
   const std::shared_ptr<Expression> &getCondition() const
   {
      return condition;
   }

   std::shared_ptr<Expression> &getCondition()
   {
      return condition;
   }

   const std::shared_ptr<Statement> &getIfBranch() const
   {
      return ifBranch;
   }

   const std::shared_ptr<Statement> &getElseBranch() const
   {
      return elseBranch;
   }

};

} // namespace ast
} // namespace cdot

#endif //IFSTATEMENT_H
