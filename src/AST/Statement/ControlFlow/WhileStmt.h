//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_WHILESTATEMENT_H
#define CDOT_WHILESTATEMENT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;

class WhileStmt : public Statement {
public:
   WhileStmt(Expression* cond,
             Statement* body,
             bool atLeastOnce);

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == WhileStmtID;
   }

   friend class TransformImpl;

protected:
   Expression* condition;
   Statement* body;

   bool atLeastOnce = false;

public:
   Expression* getCondition() const
   {
      return condition;
   }

   Expression* &getCondition()
   {
      return condition;
   }

   Statement* const& getBody() const
   {
      return body;
   }

   bool isAtLeastOnce() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_WHILESTATEMENT_H
