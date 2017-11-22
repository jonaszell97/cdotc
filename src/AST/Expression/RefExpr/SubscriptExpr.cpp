//
// Created by Jonas Zell on 21.06.17.
//

#include "SubscriptExpr.h"

namespace cdot {
namespace ast {

SubscriptExpr::SubscriptExpr(Expression::SharedPtr expr)
   : Expression(SubscriptExprID)
{
   index = expr;
}

Expression::SharedPtr &SubscriptExpr::getIndex()
{
   return index;
}

void SubscriptExpr::setIndex(const Expression::SharedPtr &index)
{
   SubscriptExpr::index = index;
}

bool SubscriptExpr::isSubscriptOperator() const
{
   return is_subscript_op;
}

void SubscriptExpr::setIsSubscriptOperator(bool is_subscript_op)
{
   SubscriptExpr::is_subscript_op = is_subscript_op;
}

const std::shared_ptr<CallExpr> &SubscriptExpr::getOverridenCall() const
{
   return overridenCall;
}

void
SubscriptExpr::setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall)
{
   SubscriptExpr::overridenCall = overridenCall;
}

} // namespace ast
} // namespace cdot