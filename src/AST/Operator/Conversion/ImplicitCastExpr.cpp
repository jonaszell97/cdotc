//
// Created by Jonas Zell on 13.07.17.
//

#include "ImplicitCastExpr.h"

namespace cdot {
namespace ast {

ImplicitCastExpr::ImplicitCastExpr(const QualType& from, const QualType& to,
                                   Expression::SharedPtr &&target)
   : Expression(ImplicitCastExprID), from(from), to(to), target(move(target))
{
   this->target->setParent(this);
}

const QualType &ImplicitCastExpr::getFrom() const
{
   return from;
}

void ImplicitCastExpr::setFrom(const QualType &from)
{
   ImplicitCastExpr::from = from;
}

const QualType &ImplicitCastExpr::getTo() const
{
   return to;
}

void ImplicitCastExpr::setTo(const QualType &to)
{
   ImplicitCastExpr::to = to;
}

const Expression::SharedPtr &ImplicitCastExpr::getTarget() const
{
   return target;
}

void ImplicitCastExpr::setTarget(const Expression::SharedPtr &target)
{
   ImplicitCastExpr::target = target;
}

} // namespace ast
} // namespace cdot