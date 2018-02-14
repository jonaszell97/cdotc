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
   explicit LvalueToRvalue(Expression* expr);

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == LvalueToRvalueID;
   }

protected:
   Expression* target;

public:
   Expression* getTarget() const;
   void setTarget(Expression* target);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LVALUETORVALUECONV_H
