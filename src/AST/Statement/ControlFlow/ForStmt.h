//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_FORSTMT_H
#define CDOT_FORSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;
class IdentifierRefExpr;

class ForStmt : public Statement {
public:
   ForStmt(Statement::SharedPtr &&init,
           std::shared_ptr<Expression> &&term,
           Statement::SharedPtr &&inc);

   ForStmt(Statement::SharedPtr &&init,
           std::shared_ptr<Expression> &&term,
           Statement::SharedPtr &&inc,
           Statement::SharedPtr &&body);

   void setBody(Statement::SharedPtr &&_body)
   {
      body = std::move(_body);
   }

   typedef std::shared_ptr<ForStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ForStmtID;
   }

protected:
   std::shared_ptr<Statement> initialization;
   std::shared_ptr<Expression> termination;
   Statement::SharedPtr increment;
   Statement::SharedPtr body;

public:
   const std::shared_ptr<Statement> &getInitialization() const
   {
      return initialization;
   }

   void setInitialization(const std::shared_ptr<Statement> &initialization)
   {
      ForStmt::initialization = initialization;
   }

   const std::shared_ptr<Expression> &getTermination() const
   {
      return termination;
   }

   void setTermination(const std::shared_ptr<Expression> &termination)
   {
      ForStmt::termination = termination;
   }

   const Statement::SharedPtr &getIncrement() const
   {
      return increment;
   }

   void setIncrement(const Statement::SharedPtr &increment)
   {
      ForStmt::increment = increment;
   }

   const Statement::SharedPtr &getBody() const
   {
      return body;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_FORSTMT_H
