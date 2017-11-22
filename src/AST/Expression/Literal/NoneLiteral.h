//
// Created by Jonas Zell on 30.08.17.
//

#ifndef CDOT_NONELITERAL_H
#define CDOT_NONELITERAL_H


#include "../Expression.h"

namespace cdot {
namespace ast {

class NoneLiteral: public Expression {
public:
   NoneLiteral() : Expression(NoneLiteralID) {}

   typedef std::shared_ptr<NoneLiteral> SharedPtr;

   bool createsTemporary() override
   {
      return true;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == NoneLiteralID;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_NONELITERAL_H
