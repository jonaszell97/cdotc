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
   TertiaryOperator(Expression::SharedPtr &&cond,
                    Expression::SharedPtr &&lhs,
                    Expression::SharedPtr &&rhs);

   typedef std::shared_ptr<TertiaryOperator> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TertiaryOperatorID;
   }

protected:
   Expression::SharedPtr condition;
   Expression::SharedPtr lhs;
   Expression::SharedPtr rhs;

public:
   Expression::SharedPtr &getCondition();
   Expression::SharedPtr &getLhs();
   Expression::SharedPtr &getRhs();
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TERTIARYOPERATOR_H
