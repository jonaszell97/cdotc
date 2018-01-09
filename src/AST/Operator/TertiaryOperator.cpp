//
// Created by Jonas Zell on 19.06.17.
//

#include "TertiaryOperator.h"

namespace cdot {
namespace ast {

TertiaryOperator::TertiaryOperator(Expression::SharedPtr &&cond,
                                   Expression::SharedPtr &&lhs,
                                   Expression::SharedPtr &&rhs)
   : Expression(TertiaryOperatorID), condition(move(cond)), lhs(move(lhs)),
     rhs(move(rhs))
{

}

} // namespace ast
} // namespace cdot