//
// Created by Jonas Zell on 19.06.17.
//

#include "TertiaryOperator.h"

namespace cdot {
namespace ast {

TertiaryOperator::TertiaryOperator(Expression* cond,
                                   Expression* lhs,
                                   Expression* rhs)
   : Expression(TertiaryOperatorID), condition(cond), lhs(lhs),
     rhs(rhs)
{

}

} // namespace ast
} // namespace cdot