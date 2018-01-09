//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H

#include "../Expression/Expression.h"
#include "../../Basic/CastKind.h"

namespace cdot {

namespace cl {
struct Method;
} // namespace cl

namespace ast {

class CallExpr;
class ConstExprPass;

enum class OperatorKind : unsigned;

enum class BinaryOperatorType: unsigned int {
   ARITHMETIC,
   ASSIGNMENT,
   EQUALITY,
   COMPARISON,
   BITWISE,
   LOGICAL,
   CAST,
   OTHER
};

BinaryOperatorType getBinaryOpType(const string &op);

class BinaryOperator: public Expression {
public:
   explicit BinaryOperator(string &&op);
   BinaryOperator(string &&op, Expression::SharedPtr &&lhs,
                  Expression::SharedPtr &&rhs);

   typedef std::shared_ptr<BinaryOperator> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == BinaryOperatorID;
   }

protected:
   std::shared_ptr<Expression> lhs;
   std::shared_ptr<Expression> rhs;
   string op;
   OperatorKind kind;
   BinaryOperatorType opType;

   std::shared_ptr<CallExpr> overridenCall = nullptr;
   CastResult requiredCast;

   bool typePredicate        : 1;
   bool typePredicateResult  : 1;

public:
   std::shared_ptr<Expression> &getLhs()
   {
      return lhs;
   }

   std::shared_ptr<Expression> &getRhs()
   {
      return rhs;
   }

   std::shared_ptr<Expression> const& getLhs() const
   {
      return lhs;
   }

   std::shared_ptr<Expression> const& getRhs() const
   {
      return rhs;
   }

   const string &getOp() const
   {
      return op;
   }

   OperatorKind getKind() const
   {
      return kind;
   }

   BinaryOperatorType getOpType() const
   {
      return opType;
   }

   const std::shared_ptr<CallExpr> &getOverridenCall() const
   {
      return overridenCall;
   }

   void setKind(OperatorKind kind)
   {
      BinaryOperator::kind = kind;
   }

   void setOpType(BinaryOperatorType opType)
   {
      BinaryOperator::opType = opType;
   }

   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall)
   {
      BinaryOperator::overridenCall = overridenCall;
   }

   const CastResult &getRequiredCast() const
   {
      return requiredCast;
   }

   void setRequiredCast(const CastResult &requiredCast)
   {
      BinaryOperator::requiredCast = requiredCast;
   }

   bool getTypePredicateResult() const
   {
      return typePredicateResult;
   }

   void setTypePredicateResult(bool typeConstraintResult)
   {
      BinaryOperator::typePredicateResult = typeConstraintResult;
   }

   bool isTypePredicate() const
   {
      return typePredicate;
   }

   void setTypePredicate(bool typePredicate)
   {
      BinaryOperator::typePredicate = typePredicate;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_BINARYOPERATOR_H
