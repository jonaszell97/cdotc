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
      std::vector<pair<string, Expression::SharedPtr>> &&elements);

   typedef std::shared_ptr<TupleLiteral> SharedPtr;

   bool createsTemporary() override
   {
      return true;
   }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TupleLiteralID;
   }

protected:
   std::vector<pair<string, Expression::SharedPtr>> elements;

   // codegen
   TupleType* tupleType;
   bool is_meta_ty = false;

public:
   bool isMetaTy() const;
   void isMetaTy(bool is_meta_ty);

   std::vector<pair<string, std::shared_ptr<Expression>>> &getElements();
   void setElements(
      const std::vector<pair<string, std::shared_ptr<Expression>>> &elements);

   TupleType *getTupleType() const;
   void setTupleType(TupleType *tupleType);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TUPLELITERAL_H
