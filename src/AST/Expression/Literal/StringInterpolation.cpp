//
// Created by Jonas Zell on 10.09.17.
//

#include "StringInterpolation.h"

namespace cdot {
namespace ast {

StringInterpolation::StringInterpolation(
   std::vector<Expression*>&& strings)
   : Expression(StringInterpolationID), strings(move(strings))
{

}

} // namespace ast
} // namespace cdot