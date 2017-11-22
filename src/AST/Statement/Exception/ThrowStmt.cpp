//
// Created by Jonas Zell on 05.10.17.
//

#include "ThrowStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ThrowStmt::ThrowStmt(std::shared_ptr<Expression> &&thrownVal)
   : Statement(ThrowStmtID), thrownVal(thrownVal)
{

}

const std::shared_ptr<Expression> &ThrowStmt::getThrownVal() const
{
   return thrownVal;
}

void ThrowStmt::setThrownVal(const std::shared_ptr<Expression> &thrownVal)
{
   ThrowStmt::thrownVal = thrownVal;
}

Type *ThrowStmt::getThrownType() const
{
   return thrownType;
}

void ThrowStmt::setThrownType(Type *thrownType)
{
   ThrowStmt::thrownType = thrownType;
}
cl::Method *ThrowStmt::getDescFn() const
{
   return descFn;
}

void ThrowStmt::setDescFn(cl::Method *descFn)
{
   ThrowStmt::descFn = descFn;
}

} // namespace ast
} // namespace cdot