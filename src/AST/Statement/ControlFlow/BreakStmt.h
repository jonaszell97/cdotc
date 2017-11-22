//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_BREAKSTMT_H
#define CDOT_BREAKSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class BreakStmt : public Statement {
public:
   BreakStmt() : Statement(BreakStmtID) {}

   typedef std::shared_ptr<BreakStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == BreakStmtID;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_BREAKSTMT_H
