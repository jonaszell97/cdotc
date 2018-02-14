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
   explicit ArrayLiteral(std::vector<Expression* > &&values)
      : Expression(ArrayLiteralID), values(move(values))
   {

   }

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == ArrayLiteralID;
   }

   friend class TransformImpl;

protected:
   std::vector<Expression* > values;

public:
   const std::vector<Expression* > &getValues() const
   {
      return values;
   }
};

class DictionaryLiteral : public Expression {
public:
   DictionaryLiteral(std::vector<Expression* > &&keys,
                     std::vector<Expression* > &&values)
      : Expression(NodeType::DictionaryLiteralID),
        keys(move(keys)), values(move(values))
   {

   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DictionaryLiteralID;
   }

   friend class TransformImpl;

protected:
   std::vector<Expression* > keys;
   std::vector<Expression* > values;

public:
   std::vector<Expression* > &getKeys()
   {
      return keys;
   }

   std::vector<Expression* > &getValues()
   {
      return values;
   }

   std::vector<Expression* > const& getKeys() const
   {
      return keys;
   }

   std::vector<Expression* > const& getValues() const
   {
      return values;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ARRAYLITERAL_H
