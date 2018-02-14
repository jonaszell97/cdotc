//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H

#include "LabelStmt.h"

namespace cdot {

namespace ast {

class MethodDecl;
class Expression;
class PatternExpr;

class CaseStmt : public Statement {
public:
   explicit CaseStmt(Statement* body);
   CaseStmt(PatternExpr* caseVal,
            Statement* body);

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CaseStmtID;
   }

   friend class TransformImpl;

protected:
   bool is_default = false;
   PatternExpr* pattern;
   Statement* body;

   MethodDecl *comparisonOp = nullptr;

public:
   bool isDefault() const
   {
      return is_default;
   }

   PatternExpr* getPattern() const
   {
      return pattern;
   }

   Statement* getBody() const
   {
      return body;
   }

   MethodDecl *getComparisonOp() const
   {
      return comparisonOp;
   }

   void setComparisonOp(MethodDecl *comparisonOp)
   {
      CaseStmt::comparisonOp = comparisonOp;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CASESTMT_H
