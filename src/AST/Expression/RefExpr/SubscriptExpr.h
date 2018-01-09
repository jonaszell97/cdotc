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
   explicit SubscriptExpr(Expression::SharedPtr &&index);

   typedef std::shared_ptr<SubscriptExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == SubscriptExprID;
   }

protected:
   std::shared_ptr<Expression> index;

   // codegen
   std::shared_ptr<CallExpr> overridenCall;

public:
   std::shared_ptr<Expression> const& getIndex() const
   {
      return index;
   }

   std::shared_ptr<Expression> &getIndex()
   {
      return index;
   }

   const std::shared_ptr<CallExpr> &getOverridenCall() const;
   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ARRAYACCESSEXPR_H
