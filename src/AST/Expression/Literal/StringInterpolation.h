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
   explicit StringInterpolation(std::vector<Expression::SharedPtr>&& strings);

   typedef std::shared_ptr<StringInterpolation> SharedPtr;

   bool createsTemporary() override
   {
      return true;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == StringInterpolationID;
   }

protected:
   std::vector<Expression::SharedPtr> strings;

public:
   std::vector<Expression::SharedPtr> &getStrings()
   {
      return strings;
   }

   void setStrings(const std::vector<Expression::SharedPtr> &strings)
   {
      StringInterpolation::strings = strings;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STRINGINTERPOLATION_H
