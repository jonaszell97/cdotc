//
// Created by Jonas Zell on 19.06.17.
//

#include "BinaryOperator.h"
#include "ExprSequence.h"

#include "../../Variant/Type/ObjectType.h"
#include "../Passes/SemanticAnalysis/Record/Record.h"

#include "../../Support/Casting.h"

using namespace cdot::cl;

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

BinaryOperator::BinaryOperator(string &&op)
   : Expression(BinaryOperatorID), op(move(op)),
     kind(stringToOperator(this->op)), typePredicate(false),
     typePredicateResult(false)
{

}

BinaryOperator::BinaryOperator(string &&op,
                               Expression::SharedPtr &&lhs,
                               Expression::SharedPtr &&rhs)
   : Expression(BinaryOperatorID),
     lhs(move(lhs)), rhs(move(rhs)), op(move(op)),
     kind(stringToOperator(this->op)), typePredicate(false),
     typePredicateResult(false)
{
   if (auto obj = support::dyn_cast<ObjectType>(*this->lhs->getExprType())) {
      if (obj->getRecord()->getMethod("infix " + this->op))
         return;
   }

   if (isAssignmentOperator(kind) && kind != OperatorKind::Assign) {
      llvm::SmallString<32> preAssignOperator(this->op);
      assert(preAssignOperator.back() == '=');

      preAssignOperator.pop_back();
      this->op = "=";
      this->kind = OperatorKind::Assign;

      auto rhsLoc = this->rhs->getSourceLoc();
      this->rhs = std::make_shared<BinaryOperator>(
         preAssignOperator.str().str(),
         std::shared_ptr<Expression>(this->lhs),
         std::shared_ptr<Expression>(this->rhs));

      this->rhs->setSourceLoc(rhsLoc);
   }
}

} // namespace ast
} // namespace cdot