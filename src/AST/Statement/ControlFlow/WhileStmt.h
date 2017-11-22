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
   WhileStmt(std::shared_ptr<Expression> &&cond,
             Statement::SharedPtr &&body,
             bool atLeastOnce);

   typedef std::shared_ptr<WhileStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == WhileStmtID;
   }

protected:
   std::shared_ptr<Expression> condition;
   Statement::SharedPtr body;

   bool atLeastOnce = false;

public:
   std::shared_ptr<Expression> &getCondition();
   const Statement::SharedPtr &getBody() const;
   bool isAtLeastOnce() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_WHILESTATEMENT_H
