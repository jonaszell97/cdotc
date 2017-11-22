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
   Statement::SharedPtr ifBranch;
   Statement::SharedPtr elseBranch;

public:
   std::shared_ptr<Expression> &getCondition();
   void setCondition(const std::shared_ptr<Expression> &condition);

   const Statement::SharedPtr &getIfBranch() const;
   void setIfBranch(const Statement::SharedPtr &ifBranch);

   const Statement::SharedPtr &getElseBranch() const;
};

} // namespace ast
} // namespace cdot

#endif //IFSTATEMENT_H
