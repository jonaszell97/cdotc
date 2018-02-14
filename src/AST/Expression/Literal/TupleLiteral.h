//
// Created by Jonas Zell on 28.08.17.
//

#ifndef CDOT_TUPLELITERAL_H
#define CDOT_TUPLELITERAL_H


#include "../Expression.h"

namespace cdot {
   class TupleType;
}

namespace cdot {
namespace ast {

class TupleLiteral: public Expression {
public:
   explicit TupleLiteral(
      std::vector<std::pair<string, Expression* >> &&elements)
      : Expression(TupleLiteralID), elements(std::move(elements))
   {

   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TupleLiteralID;
   }

   friend class TransformImpl;

protected:
   std::vector<std::pair<string, Expression* >> elements;

   // codegen
   TupleType* tupleType;
   bool is_meta_ty = false;

public:
   const std::vector<std::pair<string, Expression* >> &getElements()
   const
   {
      return elements;
   }

   std::vector<std::pair<string, Expression* >> &getElements()
   {
      return elements;
   }

   TupleType *getTupleType() const
   {
      return tupleType;
   }

   void setTupleType(TupleType *tupleType)
   {
      TupleLiteral::tupleType = tupleType;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TUPLELITERAL_H
