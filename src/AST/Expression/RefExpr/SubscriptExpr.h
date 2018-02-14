//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYACCESSEXPR_H
#define CDOT_ARRAYACCESSEXPR_H

#include "../Expression.h"

namespace cdot {
namespace ast {

class CallExpr;

class SubscriptExpr : public Expression {
public:
   explicit SubscriptExpr(std::vector<Expression*> &&indices)
      : Expression(SubscriptExprID), indices(move(indices))
   {}

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == SubscriptExprID;
   }

   friend class TransformImpl;

protected:
   std::vector<Expression*> indices;

public:
   std::vector<Expression *> &getIndicesRef()
   {
      return indices;
   }

   std::vector<Expression *> const &getIndices() const
   {
      return indices;
   }

   bool hasSingleIndex() const
   {
      return indices.size() == 1;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ARRAYACCESSEXPR_H
