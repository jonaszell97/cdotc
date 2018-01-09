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
   explicit StringLiteral(string &&str);

   typedef std::shared_ptr<StringLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == StringLiteralID;
   }

protected:
   string value;
   bool raw = false;

public:
   const string &getValue() const
   {
      return value;
   }

   void setValue(const string &value)
   {
      StringLiteral::value = value;
   }

   bool isRaw() const
   {
      return raw;
   }

   void setRaw(bool raw)
   {
      StringLiteral::raw = raw;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_STRINGLITERAL_H
