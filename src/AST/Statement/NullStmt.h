//
// Created by Jonas Zell on 21.11.17.
//

#ifndef CDOT_NULLSTMT_H
#define CDOT_NULLSTMT_H

#include "Statement.h"

namespace cdot {
namespace ast {

class NullStmt: public Statement {
public:
   NullStmt() : Statement(NullStmtID) {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == NullStmtID;
   }

   typedef std::shared_ptr<NullStmt> SharedPtr;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_NULLSTMT_H
