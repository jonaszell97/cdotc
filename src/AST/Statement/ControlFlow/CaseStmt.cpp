//
// Created by Jonas Zell on 28.06.17.
//

#include "CaseStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

CaseStmt::CaseStmt(std::shared_ptr<Expression> &&case_val)
   : LabelStmt("case")
{
   caseVal = std::move(case_val);
   typeID = CaseStmtID;
}

CaseStmt::CaseStmt()
   : LabelStmt("default")
{
   isDefault = true;
   typeID = CaseStmtID;
}

bool CaseStmt::isIsDefault() const
{
   return isDefault;
}

std::shared_ptr<Expression> &CaseStmt::getCaseVal()
{
   return caseVal;
}

const std::shared_ptr<Statement> &CaseStmt::getBody() const
{
   return body;
}

const QualType &CaseStmt::getCaseType() const
{
   return caseType;
}

bool CaseStmt::isIsEnumLetCase() const
{
   return isEnumLetCase;
}

bool CaseStmt::isIsEnumVarCase() const
{
   return isEnumVarCase;
}

cl::Method *CaseStmt::getOperatorEquals() const
{
   return operatorEquals;
}

cdot::cl::EnumCase *CaseStmt::getEnumCaseVal() const
{
   return enumCaseVal;
}

std::vector<pair<string, QualType>> &CaseStmt::getLetIdentifiers()
{
   return letIdentifiers;
}

std::vector<string> &CaseStmt::getLetBindings()
{
   return letBindings;
}

void CaseStmt::setIsDefault(bool isDefault)
{
   CaseStmt::isDefault = isDefault;
}

void CaseStmt::setCaseVal(const std::shared_ptr<Expression> &caseVal)
{
   CaseStmt::caseVal = caseVal;
}

void CaseStmt::setCaseType(const QualType &caseType)
{
   CaseStmt::caseType = caseType;
}

void CaseStmt::setIsEnumLetCase(bool isEnumLetCase)
{
   CaseStmt::isEnumLetCase = isEnumLetCase;
}

void CaseStmt::setIsEnumVarCase(bool isEnumVarCase)
{
   CaseStmt::isEnumVarCase = isEnumVarCase;
}

void CaseStmt::setOperatorEquals(cl::Method *operatorEquals)
{
   CaseStmt::operatorEquals = operatorEquals;
}

void CaseStmt::setEnumCaseVal(cdot::cl::EnumCase *enumCaseVal)
{
   CaseStmt::enumCaseVal = enumCaseVal;
}

void CaseStmt::setLetIdentifiers(
   const std::vector<pair<string, QualType>> &letIdentifiers)
{
   CaseStmt::letIdentifiers = letIdentifiers;
}

void CaseStmt::setLetBindings(const std::vector<string> &letBindings)
{
   CaseStmt::letBindings = letBindings;
}

} // namespace ast
} // namespace cdot