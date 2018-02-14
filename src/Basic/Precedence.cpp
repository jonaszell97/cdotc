//
// Created by Jonas Zell on 30.01.18.
//

#include "Precedence.h"
#include "../lex/TokenKinds.h"

using namespace cdot::lex;
using namespace cdot::prec;

namespace cdot {

prec::PrecedenceLevel getOperatorPrecedence(lex::tok::TokenType kind)
{
   switch (kind) {
      case tok::equals:
      case tok::times_equals:
      case tok::div_equals:
      case tok::mod_equals:
      case tok::plus_equals:
      case tok::minus_equals:
      case tok::shl_equals:
      case tok::ashr_equals:
      case tok::lshr_equals:
      case tok::and_equals:
      case tok::or_equals:
      case tok::xor_equals:
         return Assignment;
      case tok::question:
         return Conditional;
      case tok::logical_or:
         return LogicalOr;
      case tok::logical_and:
         return LogicalAnd;
      case tok::op_or:
         return InclusiveOr;
      case tok::caret:
         return ExclusiveOr;
      case tok::op_and:
         return And;
      case tok::double_equals:
      case tok::triple_equals:
      case tok::exclaim_equals:
      case tok::exclaim_double_equals:
         return Equality;
      case tok::greater:
      case tok::greater_equals:
      case tok::smaller:
      case tok::smaller_equals:
         return Relational;
      case tok::shl:
      case tok::ashr:
      case tok::lshr:
         return Shift;
      case tok::plus:
      case tok::minus:
         return Additive;
      case tok::times:
      case tok::div:
      case tok::percent:
         return Multiplicative;
      case tok::times_times:
         return Exponentiation;
      case tok::as:
      case tok::as_exclaim:
      case tok::as_question:
         return Cast;
      default:
         return Unknown;
   }
}

} // namespace cdot