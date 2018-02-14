//
// Created by Jonas Zell on 29.06.17.
//

#ifndef CDOT_GOTOSTMT_H
#define CDOT_GOTOSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class GotoStmt : public Statement {
public:
   explicit GotoStmt(string &&label);

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == GotoStmtID;
   }

protected:
   string labelName;

public:
   const string &getLabelName() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_GOTOSTMT_H
