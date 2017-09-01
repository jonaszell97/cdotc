//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"
#include "../Statement/Declaration/DeclStmt.h"

void Expression::isHiddenReturnValue() {
   if (memberExpr) {
      memberExpr->isHiddenReturnValue();
   }
   else {
      isHiddenReturnValue_ = true;
      isReturnValue_ = true;
      if (declaration) {
         declaration->isHiddenReturnValue();
      }
   }
}