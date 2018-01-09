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
   ReturnStmt();
   explicit ReturnStmt(std::shared_ptr<Expression> &&val);

   typedef std::shared_ptr<ReturnStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ReturnStmtID;
   }

protected:
   std::shared_ptr<Expression> returnValue;

public:
   const std::shared_ptr<Expression> &getReturnValue() const
   {
      return returnValue;
   }

   std::shared_ptr<Expression> &getReturnValue()
   {
      return returnValue;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_RETURNSTATEMENT_H
