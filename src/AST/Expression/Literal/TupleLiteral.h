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
      std::vector<pair<string, std::shared_ptr<Expression>>> &&elements);

   typedef std::shared_ptr<TupleLiteral> SharedPtr;
   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TupleLiteralID;
   }

protected:
   std::vector<pair<string, std::shared_ptr<Expression>>> elements;

   // codegen
   TupleType* tupleType;
   bool is_meta_ty = false;

public:
   const std::vector<pair<string, std::shared_ptr<Expression>>> &getElements()
   const
   {
      return elements;
   }

   std::vector<pair<string, std::shared_ptr<Expression>>> &getElements()
   {
      return elements;
   }

   TupleType *getTupleType() const;
   void setTupleType(TupleType *tupleType);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TUPLELITERAL_H
