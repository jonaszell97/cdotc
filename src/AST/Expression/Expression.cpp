//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"
#include "../Statement/Declaration/DeclStmt.h"

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