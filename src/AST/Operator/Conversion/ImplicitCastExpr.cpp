//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

namespace cdot {
namespace ast {

ImplicitCastExpr::ImplicitCastExpr(const QualType& from,
                                   const QualType& to,
                                   Expression* target,
                                   CastResult &&requiredCast)
   : Expression(ImplicitCastExprID), from(from), to(to), target(target),
     requiredCast(std::move(requiredCast))
{
   assert(from != to);
}

void ImplicitCastExpr::setFrom(const QualType &from)
{
   ImplicitCastExpr::from = from;
}

void ImplicitCastExpr::setTo(const QualType &to)
{
   ImplicitCastExpr::to = to;
}

void ImplicitCastExpr::setTarget(Expression* target)
{
   ImplicitCastExpr::target = target;
}

} // namespace ast
} // namespace cdot