//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_IMPLICITCASTEXPR_H
#define CDOT_IMPLICITCASTEXPR_H

#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

class ImplicitCastExpr : public Expression {
public:
   ImplicitCastExpr(const QualType& from, const QualType& to,
                    Expression::SharedPtr &&target);

   typedef std::shared_ptr<ImplicitCastExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ImplicitCastExprID;
   }

protected:
   QualType from;
   QualType to;
   Expression::SharedPtr target;

public:
   const QualType &getFrom() const;
   void setFrom(const QualType &from);

   const QualType &getTo() const;
   void setTo(const QualType &to);

   const Expression::SharedPtr &getTarget() const;
   void setTarget(const Expression::SharedPtr &target);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IMPLICITCASTEXPR_H
