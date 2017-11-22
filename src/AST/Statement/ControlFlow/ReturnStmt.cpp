//
// Created by Jonas Zell on 19.06.17.
//

#include "ReturnStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ReturnStmt::ReturnStmt(Expression::SharedPtr &&retVal)
   : Statement(ReturnStmtID), returnValue(move(retVal))
{

}

ReturnStmt::ReturnStmt() : Statement(ReturnStmtID)
{

}

std::shared_ptr<Expression> &ReturnStmt::getReturnValue()
{
   return returnValue;
}

void ReturnStmt::setReturnValue(const std::shared_ptr<Expression> &returnValue)
{
   ReturnStmt::returnValue = returnValue;
}

bool ReturnStmt::isHiddenParamReturn() const
{
   return hiddenParamReturn;
}

void ReturnStmt::setHiddenParamReturn(bool hiddenParamReturn)
{
   ReturnStmt::hiddenParamReturn = hiddenParamReturn;
}

const QualType &ReturnStmt::getReturnType() const
{
   return returnType;
}

void ReturnStmt::setReturnType(const QualType &returnType)
{
   ReturnStmt::returnType = returnType;
}

} // namespace ast
} // namespace cdot
