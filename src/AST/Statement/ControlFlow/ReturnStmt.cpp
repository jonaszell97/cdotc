//
// Created by Jonas Zell on 19.06.17.
//

#include "ReturnStmt.h"
#include "../../Expression/Expression.h"

ReturnStmt::ReturnStmt(Expression::SharedPtr return_val) : returnValue(return_val) {
   children.push_back(&returnValue);
}

ReturnStmt::ReturnStmt() {

}

std::vector<AstNode::SharedPtr> ReturnStmt::get_children() {
   if (returnValue) {
      return { returnValue };
   }
   else {
      return {};
   }
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

const Type &ReturnStmt::getReturnType() const
{
   return returnType;
}

void ReturnStmt::setReturnType(const Type &returnType)
{
   ReturnStmt::returnType = returnType;
}
