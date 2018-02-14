//
// Created by Jonas Zell on 18.08.17.
//

#ifndef CDOT_DEBUGSTMT_H
#define CDOT_DEBUGSTMT_H


#include "Statement.h"

namespace cdot {
namespace ast {

class DebugStmt : public Statement {
public:
   explicit DebugStmt(bool unreachable = false)
      : Statement(DebugStmtID), unreachable(unreachable) {

   }

   bool isUnreachable() const
   {
      return unreachable;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DebugStmtID;
   }

   friend class TransformImpl;

protected:
   bool unreachable;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DEBUGSTMT_H
