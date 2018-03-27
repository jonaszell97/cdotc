//
// Created by Jonas Zell on 30.01.18.
//

#include "Precedence.h"
#include "Lex/TokenKinds.h"

#include <llvm/ADT/StringSwitch.h>

using namespace cdot::lex;
using namespace cdot::prec;

namespace cdot {
namespace op {

OperatorKind fromString(FixKind fix, llvm::StringRef str)
{
   auto Switch = llvm::StringSwitch<OperatorKind>(str);
#  define CDOT_OPERATOR(Name, Symbol, PG, Fix)    \
   if (FixKind::Fix == fix) Switch.Case(Symbol, Name);
#  include "BuiltinOperators.def"

   return Switch.Default(UnknownOp);
}

OperatorKind fromString(llvm::StringRef str)
{
   return llvm::StringSwitch<OperatorKind>(str)
#  define CDOT_OPERATOR(Name, Symbol, PG, Fix)    \
      .Case(Symbol, Name)
#  include "BuiltinOperators.def"
      .Default(UnknownOp);
}

OperatorKind fromToken(lex::tok::TokenType kind)
{
   switch (kind) {
   case tok::equals: return op::Assign;
   case tok::times_equals: return op::MulAssign;
   case tok::div_equals: return op::DivAssign;
   case tok::mod_equals: return op::ModAssign;
   case tok::plus_equals: return op::AddAssign;
   case tok::minus_equals: return op::SubAssign;
   case tok::shl_equals: return op::ShlAssign;
   case tok::ashr_equals: return op::AShrAssign;
   case tok::lshr_equals: return op::LShrAssign;
   case tok::and_equals: return op::AndAssign;
   case tok::or_equals: return op::OrAssign;
   case tok::xor_equals: return op::XorAssign;
   case tok::logical_or: return op::LOr;
   case tok::logical_and: return op::LAnd;
   case tok::op_or: return op::Or;
   case tok::caret: return op::Xor;
   case tok::op_and: return op::And;
   case tok::double_equals: return op::CompEQ;
   case tok::triple_equals: return op::CompRefEQ;
   case tok::exclaim_equals: return op::CompNE;
   case tok::exclaim_double_equals: return op::CompRefNE;
   case tok::greater: return op::CompGT;
   case tok::greater_equals: return op::CompGE;
   case tok::smaller: return op::CompLT;
   case tok::smaller_equals: return op::CompLE;
   case tok::shl: return op::Shl;
   case tok::ashr: return op::AShr;
   case tok::lshr: return op::LShr;
   case tok::plus: return op::Add;
   case tok::minus: return op::Sub;
   case tok::times: return op::Mul;
   case tok::div: return op::Div;
   case tok::percent: return op::Mod;
   case tok::times_times: return op::Exp;
   default: return op::UnknownOp;
   }
}

static const char *operatorSymbs[] = {
#  define CDOT_OPERATOR(Name, Symbol, PG, Fix)    \
      Symbol,
#  include "BuiltinOperators.def"
};

llvm::StringRef toString(OperatorKind operatorKind)
{
   return operatorSymbs[operatorKind];
}

} // namespace op

Associativity getAssociativity(op::OperatorKind kind)
{
   switch (kind) {
   case op::Assign: case op::AddAssign:
   case op::SubAssign: case op::MulAssign: case op::DivAssign:
   case op::ModAssign: case op::ExpAssign: case op::AndAssign:
   case op::OrAssign: case op::XorAssign: case op::ShlAssign:
   case op::AShrAssign: case op::LShrAssign: case op::Exp:
      return Associativity::Right;
   default:
      return Associativity::Left;
   }
}

prec::PrecedenceLevel getOperatorPrecedence(op::OperatorKind kind)
{
   switch (kind) {
   case op::Assign: case op::AddAssign:
   case op::SubAssign: case op::MulAssign: case op::DivAssign:
   case op::ModAssign: case op::ExpAssign: case op::AndAssign:
   case op::OrAssign: case op::XorAssign: case op::ShlAssign:
   case op::AShrAssign: case op::LShrAssign:
      return Assignment;
   case op::LOr:
      return LogicalOr;
   case op::LAnd:
      return LogicalAnd;
   case op::Or:
      return InclusiveOr;
   case op::Xor:
      return ExclusiveOr;
   case op::And:
      return BitwiseAnd;
   case op::CompEQ:
   case op::CompRefEQ:
   case op::CompNE:
   case op::CompRefNE:
      return Equality;
   case op::CompLE: case op::CompLT: case op::CompGE: case op::CompGT:
      return Relational;
   case op::Shl: case op::LShr: case op::AShr:
      return Shift;
   case op::Add: case op::Sub:
      return Additive;
   case op::Mul: case op::Div: case op::Mod:
      return Multiplicative;
   case op::Exp:
      return Exponentiation;
   default:
      return Unknown;
   }
}

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
         return BitwiseAnd;
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
         return Cast;
      default:
         return Unknown;
   }
}

} // namespace cdot