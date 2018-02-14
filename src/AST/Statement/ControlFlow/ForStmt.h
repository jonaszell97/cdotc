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
   ForStmt(Statement* init,
           Expression* term,
           Statement* inc);

   ForStmt(Statement* init,
           Expression* term,
           Statement* inc,
           Statement* body);

   void setBody(Statement* _body)
   {
      body = std::move(_body);
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ForStmtID;
   }

   friend class TransformImpl;

protected:
   Statement* initialization;
   Expression* termination;
   Statement* increment;
   Statement* body;

public:
   Statement* getInitialization() const
   {
      return initialization;
   }

   void setInitialization(Statement* initialization)
   {
      ForStmt::initialization = initialization;
   }

   Expression* getTermination() const
   {
      return termination;
   }

   void setTermination(Expression* termination)
   {
      ForStmt::termination = termination;
   }

   Statement* getIncrement() const
   {
      return increment;
   }

   void setIncrement(Statement* increment)
   {
      ForStmt::increment = increment;
   }

   Statement* getBody() const
   {
      return body;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_FORSTMT_H
