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
   explicit SubscriptExpr(Expression::SharedPtr);

   typedef std::shared_ptr<SubscriptExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == SubscriptExprID;
   }

protected:
   Expression::SharedPtr index;

   // codegen
   bool is_subscript_op = false;
   std::shared_ptr<CallExpr> overridenCall;

public:
   Expression::SharedPtr &getIndex();
   void setIndex(const Expression::SharedPtr &index);

   bool isSubscriptOperator() const;
   void setIsSubscriptOperator(bool is_subscript_op);

   const std::shared_ptr<CallExpr> &getOverridenCall() const;
   void setOverridenCall(const std::shared_ptr<CallExpr> &overridenCall);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_ARRAYACCESSEXPR_H
