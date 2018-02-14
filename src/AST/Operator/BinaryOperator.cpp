//
// Created by Jonas Zell on 19.06.17.
//

#include "BinaryOperator.h"
#include "ExprSequence.h"

#include "../../Variant/Type/Type.h"
#include "../../Support/Casting.h"

namespace cdot {
namespace ast {

BinaryOperatorType getBinaryOpType(const string &op) {
   if (isAssignmentOperator(stringToOperator(op))) {
      return BinaryOperatorType::ASSIGNMENT;
   }
   if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%"
       || op == "**") {
      return BinaryOperatorType::ARITHMETIC;
   }
   if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>"
       || op == ">>>") {
      return BinaryOperatorType::BITWISE;
   }
   if (op == "&&" || op == "||") {
      return BinaryOperatorType::LOGICAL;
   }
   if (op == "==" || op == "!=" || op == "===" || op == "!==") {
      return BinaryOperatorType::EQUALITY;
   }
   if (op == "<=" || op == ">=" || op == "<" || op == ">") {
      return BinaryOperatorType::COMPARISON;
   }
   if (op == "as" || op == "as!" || op == "as?") {
      return BinaryOperatorType::CAST;
   }

   return BinaryOperatorType::OTHER;
}

BinaryOperator::BinaryOperator(string &&op,
                               Expression* lhs,
                               Expression* rhs)
   : Expression(BinaryOperatorID),
     lhs(lhs), rhs(rhs), op(move(op)),
     kind(stringToOperator(this->op)), typePredicate(false),
     typePredicateResult(false)
{

}

} // namespace ast
} // namespace cdot