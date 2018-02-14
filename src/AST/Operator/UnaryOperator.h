//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "../Expression/Expression.h"

namespace cdot {
namespace ast {

class CallExpr;

class UnaryOperator : public Expression {
public:
   UnaryOperator(string &&op, Expression* target,
                 bool prefix)
      : Expression(UnaryOperatorID), target(target), op(move(op)),
        prefix(prefix)
   {}

   void setTarget(Expression* t)
   {
      target = t;
   }

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == UnaryOperatorID;
   }

protected:
   Expression* target;
   string op;
   bool prefix;

public:
   Expression* &getTarget()
   {
      return target;
   }

   Expression* const& getTarget() const
   {
      return target;
   }

   const string &getOp() const
   {
      return op;
   }

   void setOp(const string &op)
   {
      UnaryOperator::op = op;
   }

   bool isPrefix() const
   {
      return prefix;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_UNARYOPERATOR_H
