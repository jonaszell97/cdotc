//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

namespace cdot {
namespace ast {

ImplicitCastExpr::ImplicitCastExpr(const QualType& from,
                                   const QualType& to,
                                   Expression::SharedPtr &&target,
                                   CastResult &&requiredCast)
   : Expression(ImplicitCastExprID), from(from), to(to), target(move(target)),
     requiredCast(std::move(requiredCast))
{
   this->target->setParent(this);
}

void ImplicitCastExpr::setFrom(const QualType &from)
{
   ImplicitCastExpr::from = from;
}

void ImplicitCastExpr::setTo(const QualType &to)
{
   ImplicitCastExpr::to = to;
}

void ImplicitCastExpr::setTarget(const Expression::SharedPtr &target)
{
   ImplicitCastExpr::target = target;
}

} // namespace ast
} // namespace cdot