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
   explicit MatchStmt(std::shared_ptr<Expression> &&switchVal);
   MatchStmt(std::shared_ptr<Expression> &&switchVal,
             std::vector<std::shared_ptr<CaseStmt>> &&cases);

   void addCase(std::shared_ptr<CaseStmt> &&case_)
   {
      cases.push_back(move(case_));
   }

   typedef std::shared_ptr<MatchStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == MatchStmtID;
   }

protected:
   std::shared_ptr<Expression> switchValue;
   Type *switchType = nullptr;
   std::vector<std::shared_ptr<CaseStmt>> cases;

   cl::Method* operatorEquals = nullptr;

   // codegen
   bool hasDefault = false;
   unsigned defaultIndex = -1;
   
   bool isIntegralSwitch = false;
   bool allCasesReturn;

public:
   const std::shared_ptr<Expression> &getSwitchValue() const
   {
      return switchValue;
   }

   Type *getSwitchType() const;

   const std::vector<std::shared_ptr<CaseStmt>> &getCases() const;

   cl::Method *getOperatorEquals() const;

   void setSwitchType(Type *switchType);

   void setCases(const std::vector<std::shared_ptr<CaseStmt>> &cases);

   void setOperatorEquals(cl::Method *operatorEquals);

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
