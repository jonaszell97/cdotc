//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_TERTIARYOPERATOR_H
#define CDOT_TERTIARYOPERATOR_H

#include "../Expression/Expression.h"

namespace cdot {
namespace ast {

class TertiaryOperator : public Expression {
public:
   TertiaryOperator(Expression* cond,
                    Expression* lhs,
                    Expression* rhs);

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TertiaryOperatorID;
   }

protected:
   Expression* condition;
   Expression* lhs;
   Expression* rhs;

public:
   Expression* getCondition() const
   {
      return condition;
   }

   Expression* getLhs() const
   {
      return lhs;
   }

   Expression* getRhs() const
   {
      return rhs;
   }

   Expression* &getCondition()
   {
      return condition;
   }

   Expression* &getLhs()
   {
      return lhs;
   }

   Expression* &getRhs()
   {
      return rhs;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TERTIARYOPERATOR_H
