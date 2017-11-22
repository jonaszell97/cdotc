//
// Created by Jonas Zell on 15.08.17.
//

#ifndef CDOT_LVALUETORVALUECONV_H
#define CDOT_LVALUETORVALUECONV_H

#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

class LvalueToRvalue : public Expression {
public:
   explicit LvalueToRvalue(Expression::SharedPtr expr);

   typedef std::shared_ptr<LvalueToRvalue> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == LvalueToRvalueID;
   }

protected:
   Expression::SharedPtr target;

public:
   const Expression::SharedPtr &getTarget() const;
   void setTarget(const Expression::SharedPtr &target);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LVALUETORVALUECONV_H
