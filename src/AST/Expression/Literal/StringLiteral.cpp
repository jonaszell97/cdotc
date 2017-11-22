//
// Created by Jonas Zell on 30.06.17.
//

#include "StringLiteral.h"

namespace cdot {
namespace ast {

StringLiteral::StringLiteral(std::string &&value)
   : Expression(StringLiteralID), value(std::move(value))
{

}

} // namespace ast
} // namespace cdot