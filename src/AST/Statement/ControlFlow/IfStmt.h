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
   IfStmt(Expression* cond,
          Statement* body);

   IfStmt(Expression* cond,
          Statement* body,
          Statement* elseBody);

   inline void setElseBranch(Statement* elseBr)
   {
      elseBranch = std::move(elseBr);
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == IfStmtID;
   }

   friend class TransformImpl;

protected:
   Expression* condition;
   Statement* ifBranch;
   Statement* elseBranch;

public:
   Expression* getCondition() const
   {
      return condition;
   }

   Expression* &getCondition()
   {
      return condition;
   }

   Statement* getIfBranch() const
   {
      return ifBranch;
   }

   Statement* getElseBranch() const
   {
      return elseBranch;
   }

};

} // namespace ast
} // namespace cdot

#endif //IFSTATEMENT_H
