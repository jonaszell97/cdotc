//
// Created by Jonas Zell on 18.03.18.
//

#ifndef CDOT_SOURCETYPE_H
#define CDOT_SOURCETYPE_H

#include "Type.h"

namespace cdot {
namespace ast {

class Expression;

struct SourceType {
   explicit SourceType(Expression *TypeExpr)
      : TypeExpr(TypeExpr)
   {}

   explicit SourceType(QualType ResolvedType = QualType())
      : TypeExpr(nullptr), ResolvedType(ResolvedType)
   {}

   bool isValid() const
   {
      return TypeExpr != nullptr || !ResolvedType.isNull();
   }

   bool isResolved() const
   {
      return !ResolvedType.isNull();
   }

   Type *operator->() const
   {
      return *ResolvedType;
   }

   /*implicit*/ operator bool() const
   {
      return isValid();
   }

   /*implicit*/ operator QualType() const
   {
      assert(isResolved() && "unresolved type!");
      return ResolvedType;
   }

   Expression *getTypeExpr()  const { return TypeExpr; }
   QualType getResolvedType() const { return ResolvedType; }

   void setResolvedType(QualType Ty) const { ResolvedType = Ty; }

   bool operator==(QualType Q) const { return ResolvedType == Q; }
   bool operator!=(QualType Q) const { return !(*this == Q); }

   SourceType &operator=(QualType Ty) { ResolvedType = Ty; return *this; }

private:
   Expression *TypeExpr;
   mutable QualType ResolvedType;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_SOURCETYPE_H
