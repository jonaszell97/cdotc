//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H

#include "LabelStmt.h"

namespace cdot {

namespace cl {

struct EnumCase;

} // namespace cdot

namespace ast {

class Expression;
class PatternExpr;

class CaseStmt : public Statement {
public:
   explicit CaseStmt(std::shared_ptr<Statement> &&body);
   CaseStmt(std::shared_ptr<PatternExpr> &&caseVal,
            std::shared_ptr<Statement> &&body);

   typedef std::shared_ptr<CaseStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CaseStmtID;
   }

protected:
   bool is_default = false;
   std::shared_ptr<PatternExpr> pattern;
   std::shared_ptr<Statement> body;

   cl::Method *comparisonOp = nullptr;

public:
   bool isDefault() const
   {
      return is_default;
   }

   const std::shared_ptr<PatternExpr> &getPattern() const
   {
      return pattern;
   }

   const std::shared_ptr<Statement> &getBody() const
   {
      return body;
   }

   cl::Method *getComparisonOp() const
   {
      return comparisonOp;
   }

   void setComparisonOp(cl::Method *comparisonOp)
   {
      CaseStmt::comparisonOp = comparisonOp;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_CASESTMT_H
