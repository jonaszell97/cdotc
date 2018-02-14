//
// Created by Jonas Zell on 30.06.17.
//

#ifndef CDOT_STRINGLITERAL_H
#define CDOT_STRINGLITERAL_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class StringLiteral : public Expression {
public:
   explicit StringLiteral(std::string &&str)
      : Expression(StringLiteralID, true),
        value(move(str))
   {}

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == StringLiteralID;
   }

protected:
   std::string value;
   bool cstring = false;

public:
   const std::string &getValue() const
   {
      return value;
   }

   bool isCString() const
   {
      return cstring;
   }

   void setCString(bool raw)
   {
      StringLiteral::cstring = raw;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STRINGLITERAL_H
