//
// Created by Jonas Zell on 15.08.17.
//

#include "LvalueToRvalue.h"

namespace cdot {
namespace ast {

LvalueToRvalue::LvalueToRvalue(Expression::SharedPtr expr)
   : Expression(LvalueToRvalueID), target(expr)
{
    target->setParent(this);
}

const Expression::SharedPtr &LvalueToRvalue::getTarget() const
{
   return target;
}

void LvalueToRvalue::setTarget(const Expression::SharedPtr &target)
{
   LvalueToRvalue::target = target;
}

} // namespace ast
} // namespace cdot