//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_IMPLICITCASTEXPR_H
#define CDOT_IMPLICITCASTEXPR_H

#include "../../Expression/Expression.h"
#include "../../../Basic/CastKind.h"

namespace cdot {
namespace ast {

class ImplicitCastExpr : public Expression {
public:
   ImplicitCastExpr(const QualType& from,
                    const QualType& to,
                    Expression::SharedPtr &&target,
                    CastResult &&cast);

   typedef std::shared_ptr<ImplicitCastExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == ImplicitCastExprID;
   }

protected:
   QualType from;
   QualType to;
   Expression::SharedPtr target;

   CastResult requiredCast;

public:
   const QualType &getFrom() const { return from; }
   void setFrom(const QualType &from);

   const QualType &getTo() const { return to; }
   void setTo(const QualType &to);

   const Expression::SharedPtr &getTarget() const { return target; }
   void setTarget(const Expression::SharedPtr &target);

   const CastResult &getRequiredCast() const { return requiredCast; }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_IMPLICITCASTEXPR_H
