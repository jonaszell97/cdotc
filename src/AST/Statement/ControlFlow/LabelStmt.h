//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_LABELSTMT_H
#define CDOT_LABELSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class LabelStmt : public Statement {
public:
   explicit LabelStmt(string &&name);

   typedef std::shared_ptr<LabelStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == LabelStmtID || T->getTypeID() == CaseStmtID;
   }

protected:
   string labelName;

public:
   const string &getLabelName() const;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LABELSTMT_H
