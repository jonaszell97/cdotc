//
// Created by Jonas Zell on 28.06.17.
//

#include "MatchStmt.h"
#include "../../Expression/Expression.h"
#include "CaseStmt.h"

namespace cdot {
namespace ast {

MatchStmt::MatchStmt(Expression* switchVal)
   : Statement(MatchStmtID), switchValue(switchVal)
{

}

MatchStmt::MatchStmt(Expression* switchVal,
                     std::vector<CaseStmt* > &&cases)
   : Statement(MatchStmtID), switchValue(switchVal), cases(move(cases))
{

}

Type *MatchStmt::getSwitchType() const
{
   return switchType;
}

const std::vector<CaseStmt* > &MatchStmt::getCases() const
{
   return cases;
}

void MatchStmt::setSwitchType(Type *switchType)
{
   MatchStmt::switchType = switchType;
}

void MatchStmt::setCases(const std::vector<CaseStmt* > &cases)
{
   MatchStmt::cases = cases;
}

bool MatchStmt::isHasDefault() const
{
   return hasDefault;
}

void MatchStmt::setHasDefault(bool hasDefault)
{
   MatchStmt::hasDefault = hasDefault;
}

unsigned int MatchStmt::getDefaultIndex() const
{
   return defaultIndex;
}

void MatchStmt::setDefaultIndex(unsigned int defaultIndex)
{
   MatchStmt::defaultIndex = defaultIndex;
}

bool MatchStmt::isIsIntegralSwitch() const
{
   return isIntegralSwitch;
}

void MatchStmt::setIsIntegralSwitch(bool isIntegralSwitch)
{
   MatchStmt::isIntegralSwitch = isIntegralSwitch;
}

bool MatchStmt::isAllCasesReturn() const
{
   return allCasesReturn;
}

void MatchStmt::setAllCasesReturn(bool allCasesReturn)
{
   MatchStmt::allCasesReturn = allCasesReturn;
}

} // namespace ast
} // namespace cdot