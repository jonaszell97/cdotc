//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_SWITCHSTMT_H
#define CDOT_SWITCHSTMT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class Expression;
class CaseStmt;

class MatchStmt : public Statement {
public:
   explicit MatchStmt(Expression* switchVal);
   MatchStmt(Expression* switchVal,
             std::vector<CaseStmt* > &&cases);

   void addCase(CaseStmt* case_)
   {
      cases.push_back(case_);
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MatchStmtID;
   }

   friend class TransformImpl;

protected:
   Expression* switchValue;
   Type *switchType = nullptr;
   std::vector<CaseStmt* > cases;

   // codegen
   bool hasDefault = false;
   unsigned defaultIndex = -1;
   
   bool isIntegralSwitch = false;
   bool allCasesReturn;

public:
   Expression* getSwitchValue() const
   {
      return switchValue;
   }

   Type *getSwitchType() const;

   const std::vector<CaseStmt* > &getCases() const;

   void setSwitchType(Type *switchType);

   void setCases(const std::vector<CaseStmt* > &cases);

   bool isHasDefault() const;

   void setHasDefault(bool hasDefault);

   unsigned int getDefaultIndex() const;

   void setDefaultIndex(unsigned int defaultIndex);

   bool isIsIntegralSwitch() const;

   void setIsIntegralSwitch(bool isIntegralSwitch);

   bool isAllCasesReturn() const;

   void setAllCasesReturn(bool allCasesReturn);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_SWITCHSTMT_H
