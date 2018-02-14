//
// Created by Jonas Zell on 24.11.17.
//

#include "ExprSequence.h"

namespace cdot {
namespace ast {

using SequenceElement = ExprSequence::SequenceElement;

SequenceElement::SequenceElement(OperatorKind opKind,
                                 SourceLocation loc)
   : operatorKind(opKind), kind(EF_Operator), loc(loc)
{

}

SequenceElement::SequenceElement(std::string &&possibleOp,
                                 SourceLocation loc)
   : op(move(possibleOp)), kind(EF_PossibleOperator), loc(loc)
{

}

SequenceElement::SequenceElement(Expression* expr)
   : expr(expr), kind(EF_Expression)
{
   if (expr)
      loc = expr->getSourceLoc();
}

SequenceElement::SequenceElement(SequenceElement &&other)
{
   kind = other.kind;
   loc = other.loc;

   if (kind == EF_PossibleOperator) {
      new (&op) std::string(move(other.op));
   }
   else if (kind == EF_Expression) {
      expr = other.expr;
   }
   else {
      operatorKind = other.operatorKind;
   }
}

SequenceElement& SequenceElement::operator=(SequenceElement &&other)
{
   if (kind == EF_PossibleOperator) {
      op.~string();
   }

   kind = other.kind;
   loc = other.loc;

   if (kind == EF_PossibleOperator) {
      new (&op) std::string(move(other.op));
   }
   else if (kind == EF_Expression) {
      expr = other.expr;
   }
   else {
      operatorKind = other.operatorKind;
   }

   return *this;
}

SequenceElement::~SequenceElement()
{
   if (kind == EF_PossibleOperator) {
      op.~string();
   }
}

ExprSequence::ExprSequence(std::vector<SequenceElement> &&fragments,
                           bool parenthesized)
   : Expression(ExprSequenceID), fragments(move(fragments)),
     parenthesized(parenthesized)
{

}

string operatorToString(OperatorKind kind)
{
   switch (kind) {
#  define CDOT_BINARY_OP(Name, Op) \
      case OperatorKind::Name: \
         return Op;
#  define CDOT_UNARY_OP(Name, Op) CDOT_BINARY_OP(Name, Op)
#  include "Operators.def"

      default:
         llvm_unreachable("bad operator kind");
   }
}

OperatorKind stringToOperator(const string &str)
{
#  define CDOT_BINARY_OP(Name, Op) \
   if (str == (Op)) return OperatorKind::Name;
#  define CDOT_UNARY_OP(Name, Op) CDOT_BINARY_OP(Name, Op)
#  include "Operators.def"

   return OperatorKind::None;
}

bool isAssignmentOperator(OperatorKind operatorKind)
{
   switch (operatorKind) {
      case OperatorKind::Assign:
      case OperatorKind::AddAssign:
      case OperatorKind::SubAssign:
      case OperatorKind::MulAssign:
      case OperatorKind::DivAssign:
      case OperatorKind::ExpAssign:
      case OperatorKind::ModAssign:
      case OperatorKind::AndAssign:
      case OperatorKind::OrAssign:
      case OperatorKind::XorAssign:
      case OperatorKind::ShlAssign:
      case OperatorKind::AShrAssign:
      case OperatorKind::LShrAssign:
         return true;
      default:
         return false;
   }
}

} // namespace ast
} // namespace cdot