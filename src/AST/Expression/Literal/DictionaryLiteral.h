//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class ArrayLiteral : public Expression {
public:
   ArrayLiteral(std::vector<std::shared_ptr<Expression>> &&values);

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == ArrayLiteralID;
   }

protected:
   std::vector<std::shared_ptr<Expression>> values;

public:
   const std::vector<std::shared_ptr<Expression>> &getValues() const
   {
      return values;
   }
};

class DictionaryLiteral : public Expression {
public:
   DictionaryLiteral(std::vector<std::shared_ptr<Expression>> &&keys,
                     std::vector<std::shared_ptr<Expression>> &&values);

   typedef std::shared_ptr<DictionaryLiteral> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DictionaryLiteralID;
   }

protected:
   std::vector<std::shared_ptr<Expression>> keys;
   std::vector<std::shared_ptr<Expression>> values;

public:
   std::vector<std::shared_ptr<Expression>> &getKeys()
   {
      return keys;
   }

   std::vector<std::shared_ptr<Expression>> &getValues()
   {
      return values;
   }

   std::vector<std::shared_ptr<Expression>> const& getKeys() const
   {
      return keys;
   }

   std::vector<std::shared_ptr<Expression>> const& getValues() const
   {
      return values;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ARRAYLITERAL_H
