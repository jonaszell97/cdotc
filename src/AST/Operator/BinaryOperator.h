//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H

#include "../Expression/Expression.h"
#include "../../Basic/CastKind.h"

namespace cdot {
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
   BinaryOperator(string &&op, Expression* lhs,
                  Expression* rhs);

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == BinaryOperatorID;
   }

protected:
   Expression* lhs;
   Expression* rhs;
   string op;

   OperatorKind kind;
   BinaryOperatorType opType;

   CastResult requiredCast;

   bool typePredicate        : 1;
   bool typePredicateResult  : 1;

   MethodDecl *accessorMethod = nullptr;

public:
   Expression* getLhs() const
   {
      return lhs;
   }

   Expression* getRhs() const
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

   void setKind(OperatorKind kind)
   {
      BinaryOperator::kind = kind;
   }

   void setOpType(BinaryOperatorType opType)
   {
      BinaryOperator::opType = opType;
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

   MethodDecl *getAccessorMethod() const
   {
      return accessorMethod;
   }

   void setAccessorMethod(MethodDecl *accessorMethod)
   {
      BinaryOperator::accessorMethod = accessorMethod;
   }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_BINARYOPERATOR_H
