//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"
#include "TypeRef.h"

#include "RefExpr/IdentifierRefExpr.h"
#include "RefExpr/MemberRefExpr.h"
#include "RefExpr/CallExpr.h"

#include "Literal/IntegerLiteral.h"
#include "Literal/StringLiteral.h"

#include "../../Support/Casting.h"

#include "../Statement/Declaration/NamedDecl.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

void Expression::setIsLhsOfAssignment(bool isLhsOfAssign)
{
   for (auto curr = this; curr; curr = curr->getSubExpr()) {
      if (auto MemExpr = dyn_cast<MemberRefExpr>(curr)) {
         if (!MemExpr->getSubExpr()) {
            MemExpr->setIsLhsOfAssignment(isLhsOfAssign);
            return;
         }
      }
   }
}

MethodDecl* Expression::getAccessorMethod()
{
   for (auto curr = this; curr; curr = curr->getSubExpr()) {
      if (auto MemExpr = dyn_cast<MemberRefExpr>(curr)) {
         if (!MemExpr->getSubExpr()
             && MemExpr->getKind() == MemberKind::Accessor) {
            return MemExpr->getAccessorMethod();
         }
      }
   }

   return nullptr;
}

void Expression::setIsVariadicArgPackExpansion(bool variadicArgPackExpansion)
{
   Expression::variadicArgPackExpansion = variadicArgPackExpansion;
   if (auto Ident = dyn_cast<IdentifierRefExpr>(this)) {
      for (const auto &TA : Ident->getTemplateArgs())
         if (TA->isTypeName())
            TA->getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
   else if (auto MemExpr = dyn_cast<MemberRefExpr>(this)) {
      for (const auto &TA : MemExpr->getTemplateArgs())
         if (TA->isTypeName())
            TA->getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
   else if (auto Call = dyn_cast<CallExpr>(this)) {
      for (const auto &TA : Call->getTemplateArgs())
         if (TA->isTypeName())
            TA->getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
}

} // namespace ast
} // namespace cdot
