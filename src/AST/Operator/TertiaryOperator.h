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
   TertiaryOperator(std::shared_ptr<Expression> &&cond,
                    std::shared_ptr<Expression> &&lhs,
                    std::shared_ptr<Expression> &&rhs);

   typedef std::shared_ptr<TertiaryOperator> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TertiaryOperatorID;
   }

protected:
   std::shared_ptr<Expression> condition;
   std::shared_ptr<Expression> lhs;
   std::shared_ptr<Expression> rhs;

public:
   const std::shared_ptr<Expression> &getCondition() const
   {
      return condition;
   }

   const std::shared_ptr<Expression> &getLhs() const
   {
      return lhs;
   }

   const std::shared_ptr<Expression> &getRhs() const
   {
      return rhs;
   }

   std::shared_ptr<Expression> &getCondition()
   {
      return condition;
   }

   std::shared_ptr<Expression> &getLhs()
   {
      return lhs;
   }

   std::shared_ptr<Expression> &getRhs()
   {
      return rhs;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TERTIARYOPERATOR_H
