//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTINUESTMT_H
#define CDOT_CONTINUESTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class ContinueStmt : public Statement {
public:
   ContinueStmt() : Statement(ContinueStmtID) {}

   typedef std::shared_ptr<ContinueStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ContinueStmtID;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CONTINUESTMT_H
