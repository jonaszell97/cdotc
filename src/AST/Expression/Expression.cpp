//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"

namespace cdot {
namespace ast {

bool Expression::createsTemporary()
{
   return false;
}

void Expression::setGlobalVar(llvm::GlobalVariable* glob)
{
   globalVar = glob;
   if (memberExpr != nullptr) {
      memberExpr->setGlobalVar(glob);
   }
}

void Expression::isLhsOfAssigment()
{
   lhs_of_assignment = true;
   if (memberExpr != nullptr) {
      memberExpr->isLhsOfAssigment();
   }
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

void Expression::isTemporary(bool tmp)
{
   temporary = tmp;
}

bool Expression::isTemporary() const
{
   return temporary;
}

void Expression::setTempType(cl::Record *ty)
{
   tempType = ty;
}

cl::Record *Expression::getTempType() const
{
   return tempType;
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

string &Expression::getIdent()
{
   return ident;
}

void Expression::setIdent(const string &ident)
{
   Expression::ident = ident;
}

bool Expression::needsCast() const
{
   return needs_cast;
}

void Expression::needsCast(bool needsCast)
{
   Expression::needs_cast = needsCast;
}

const QualType &Expression::getCastFrom() const
{
   return castFrom;
}

void Expression::setCastFrom(const QualType &castFrom)
{
   Expression::castFrom = castFrom;
}

const QualType &Expression::getCastTo() const
{
   return castTo;
}

void Expression::setCastTo(const QualType &castTo)
{
   Expression::castTo = castTo;
}

bool Expression::castHandled() const
{
   return cast_handled;
}

void Expression::castHandled(bool castHandled)
{
   Expression::cast_handled = castHandled;
}

bool Expression::isLvalueCast() const
{
   return lvalueCast;
}

void Expression::isLvalueCast(bool lvalueCast)
{
   Expression::lvalueCast = lvalueCast;
}

bool Expression::needsByValPass() const
{
   return byval_pass;
}

void Expression::needsByValPass(bool needsByValPass)
{
   Expression::byval_pass = needsByValPass;
}

bool Expression::getIsLhsOfAssigment() const
{
   return lhs_of_assignment;
}

void Expression::setIsLhsOfAssignment(bool lhsOfAssignment)
{
   lhs_of_assignment = lhsOfAssignment;
}

bool Expression::isSetterCall() const
{
   return setter_call;
}

void Expression::isSetterCall(bool isSetterCall)
{
   Expression::setter_call = isSetterCall;
}

Variant &Expression::getStaticVal()
{
   return staticVal;
}

void Expression::setStaticVal(const Variant &staticVal)
{
   Expression::staticVal = staticVal;
}

llvm::GlobalVariable *Expression::getGlobalVar() const
{
   return globalVar;
}

bool Expression::isEnumCase() const
{
   return enum_case;
}

void Expression::isEnumCase(bool enum_case)
{
   Expression::enum_case = enum_case;
}

long Expression::getCaseVal() const
{
   return caseVal;
}

void Expression::setCaseVal(long caseVal)
{
   Expression::caseVal = caseVal;
}

void Expression::setTemporary(bool temporary)
{
   Expression::temporary = temporary;
}

} // namespace ast
} // namespace cdot
