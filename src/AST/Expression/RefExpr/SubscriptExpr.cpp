//
// Created by Jonas Zell on 21.06.17.
//

#include "SubscriptExpr.h"

namespace cdot {
namespace ast {

SubscriptExpr::SubscriptExpr(Expression::SharedPtr &&idx)
   : Expression(SubscriptExprID), index(move(idx))
{

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