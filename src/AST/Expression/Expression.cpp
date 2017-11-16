//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"
#include "../Statement/Declaration/DeclStmt.h"

void Expression::isAssigned(bool ass)
{
   is_assigned = ass;
   if (memberExpr) {
      memberExpr->isAssigned(ass);
   }
}

void Expression::isHiddenReturnValue()
{
   if (memberExpr) {
      memberExpr->isHiddenReturnValue();
   }
   else {
      sret_value = true;
      returned_value = true;
      if (declaration) {
         declaration->isHiddenReturnValue();
      }
   }
}

void Expression::isReturnValue()
{
   returned_value = true;
   if (declaration != nullptr) {
      declaration->isReturnValue();
   }

   isPartOfReturnValue(true);
}

void Expression::setMemberExpr(const Expression::SharedPtr &memberExpr)
{
   Expression::memberExpr = memberExpr;
}

cl::Method *Expression::getAccessorMethod() const
{
   return accessorMethod;
}

void Expression::setAccessorMethod(cl::Method *accessorMethod)
{
   Expression::accessorMethod = accessorMethod;
}

bool Expression::isGetterCall() const
{
   return getter_call;
}

void Expression::isGetterCall(bool getter_call)
{
   Expression::getter_call = getter_call;
}
