//
// Created by Jonas Zell on 15.08.17.
//

#include "LvalueToRvalue.h"

namespace cdot {
namespace ast {

LvalueToRvalue::LvalueToRvalue(Expression* expr)
   : Expression(LvalueToRvalueID), target(expr)
{

}

Expression* LvalueToRvalue::getTarget() const
{
   return target;
}

void LvalueToRvalue::setTarget(Expression* target)
{
   LvalueToRvalue::target = target;
}

} // namespace ast
} // namespace cdot