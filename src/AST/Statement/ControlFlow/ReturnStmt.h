//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;

class ReturnStmt : public Statement {
public:
   ReturnStmt()
      : Statement(ReturnStmtID), returnValue(nullptr)
   {}

   explicit ReturnStmt(Expression* val)
      : Statement(ReturnStmtID), returnValue(val)
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ReturnStmtID;
   }

   friend class TransformImpl;

protected:
   Expression* returnValue;

public:
   Expression* getReturnValue() const
   {
      return returnValue;
   }

   Expression* &getReturnValue()
   {
      return returnValue;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_RETURNSTATEMENT_H
