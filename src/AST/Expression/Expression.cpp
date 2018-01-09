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
#include "../../Variant/Type/TypeGroup.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

Expression::~Expression() = default;

void Expression::setExprType(const QualType &exprType)
{
   if (exprType->isTypeGroup())
      return;

   Expression::exprType = exprType;
   if (exprType->isGenericTy())
      setIsTypeDependent(true);
}

void Expression::setIsLhsOfAssignment()
{
   lhs_of_assignment = true;
   if (memberExpr != nullptr) {
      memberExpr->setIsLhsOfAssignment();
   }
}

void Expression::setMemberExpr(const Expression::SharedPtr &memberExpr)
{
   Expression::memberExpr = memberExpr;

   if (memberExpr)
      memberExpr->setParentExpr(this);
}

Type* Expression::getPlaceholderType() const
{
   if (auto Int = dyn_cast<IntegerLiteral>(this)) {
      return Int->getType()->isUnsigned()
               ? IntegerTypeGroup::getUnsigned()
               : IntegerTypeGroup::getSigned();
   }
   else if (auto E = dyn_cast<EnumCaseExpr>(this)) {
      return EnumTypeGroup::get(E->getIdent());
   }

   switch (typeID) {
      case FPLiteralID:
         return FPTypeGroup::get();
      case StringLiteralID:
         return StringTypeGroup::get();
      default:
         return nullptr;
   }
}

bool Expression::isTripleColon() const
{
   if (auto Ident = dyn_cast<IdentifierRefExpr>(this))
      return Ident->getIdent() == "...";

   return false;
}

void Expression::setIsVariadicArgPackExpansion(bool variadicArgPackExpansion)
{
   Expression::variadicArgPackExpansion = variadicArgPackExpansion;
   if (auto Ident = dyn_cast<IdentifierRefExpr>(this)) {
      for (const auto &TA : Ident->getTemplateArgs())
         if (TA.isTypeName())
            TA.getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
   else if (auto MemExpr = dyn_cast<MemberRefExpr>(this)) {
      for (const auto &TA : MemExpr->getTemplateArgs())
         if (TA.isTypeName())
            TA.getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
   else if (auto Call = dyn_cast<CallExpr>(this)) {
      for (const auto &TA : Call->getTemplateArgs())
         if (TA.isTypeName())
            TA.getType()
              ->setAllowUnexpandedTemplateArgs(variadicArgPackExpansion);
   }
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

const Expression::SharedPtr &Expression::getMemberExpr() const
{
   return memberExpr;
}

Expression *Expression::getParentExpr() const
{
   return parentExpr;
}

void Expression::setParentExpr(Expression *parentExpr)
{
   Expression::parentExpr = parentExpr;
}

bool Expression::getIsLhsOfAssigment() const
{
   return lhs_of_assignment;
}

void Expression::setIsLhsOfAssignment(bool lhsOfAssignment)
{
   lhs_of_assignment = lhsOfAssignment;
   if (lhsOfAssignment)
      this->setIsLhsOfAssignment();
}

bool Expression::isSetterCall() const
{
   return setter_call;
}

void Expression::isSetterCall(bool isSetterCall)
{
   Expression::setter_call = isSetterCall;
}

} // namespace ast
} // namespace cdot
