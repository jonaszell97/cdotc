//
// Created by Jonas Zell on 28.06.17.
//

#include "MatchStmt.h"
#include "../../Expression/Expression.h"
#include "CaseStmt.h"

namespace cdot {
namespace ast {

MatchStmt::MatchStmt(Expression::SharedPtr &&switchVal)
   : Statement(MatchStmtID), switchValue(move(switchVal))
{

}

MatchStmt::MatchStmt(std::shared_ptr<Expression> &&switchVal,
                     std::vector<std::shared_ptr<CaseStmt>> &&cases)
   : Statement(MatchStmtID), switchValue(move(switchVal)), cases(move(cases))
{

}

Type *MatchStmt::getSwitchType() const
{
   return switchType;
}

const std::vector<std::shared_ptr<CaseStmt>> &MatchStmt::getCases() const
{
   return cases;
}

cl::Method *MatchStmt::getOperatorEquals() const
{
   return operatorEquals;
}

void MatchStmt::setSwitchType(Type *switchType)
{
   MatchStmt::switchType = switchType;
}

void MatchStmt::setCases(const std::vector<std::shared_ptr<CaseStmt>> &cases)
{
   MatchStmt::cases = cases;
}

void MatchStmt::setOperatorEquals(cl::Method *operatorEquals)
{
   MatchStmt::operatorEquals = operatorEquals;
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