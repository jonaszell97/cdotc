//
// Created by Jonas Zell on 10.09.17.
//

#ifndef CDOT_STRINGINTERPOLATION_H
#define CDOT_STRINGINTERPOLATION_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class StringInterpolation: public Expression {
public:
   explicit StringInterpolation(std::vector<Expression*>&& strings);

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == StringInterpolationID;
   }

   friend class TransformImpl;

protected:
   std::vector<Expression*> strings;

public:
   std::vector<Expression*> &getStrings()
   {
      return strings;
   }

   std::vector<Expression*> const& getStrings() const
   {
      return strings;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STRINGINTERPOLATION_H
