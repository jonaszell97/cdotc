//
// Created by Jonas Zell on 18.03.18.
//

#ifndef CDOT_SOURCETYPE_H
#define CDOT_SOURCETYPE_H

#include "Lex/SourceLocation.h"
#include "Type.h"

namespace cdot {
namespace ast {

class Expression;

struct SourceType {
   explicit SourceType(Expression *TypeExpr)
      : TypeExpr(TypeExpr)
   {}

   /*implicit*/ SourceType(QualType ResolvedType = QualType())
      : TypeExpr(nullptr), ResolvedType(ResolvedType)
   {}

   SourceType(Expression *TypeExpr, QualType ResolvedType)
      : TypeExpr(TypeExpr), ResolvedType(ResolvedType)
   {}

   void Profile(llvm::FoldingSetNodeID &ID) const
   {
      ResolvedType.Profile(ID);
      ID.AddPointer(TypeExpr);
   }

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

   /*implicit*/ operator CanType() const
   {
      assert(isResolved() && "unresolved type!");
      return ResolvedType;
   }

   SourceRange getSourceRange(SourceRange defaultVal = {}) const;

   Expression *getTypeExpr()  const { return TypeExpr; }
   QualType getResolvedType() const { return ResolvedType; }

   void setResolvedType(QualType Ty) const { ResolvedType = Ty; }
   void setTypeExpr(Expression *E) const { TypeExpr = E; }

   bool operator==(QualType Q) const { return ResolvedType == Q; }
   bool operator!=(QualType Q) const { return !(*this == Q); }

   bool operator==(const SourceType &T) const
   {
      if (isResolved() && T.isResolved()) {
         return ResolvedType == T.ResolvedType;
      }

      return TypeExpr == T.TypeExpr;
   }

   bool operator!=(const SourceType &T) const { return !(*this == T); }

   const SourceType &operator=(QualType Ty) const
   {
      ResolvedType = Ty; return *this;
   }

private:
   mutable Expression *TypeExpr;
   mutable QualType ResolvedType;
};

} // namespace ast
} // namespace cdot

namespace llvm {

template<class T> struct simplify_type;
template<class T> struct DenseMapInfo;

// teach isa etc. to treat QualType like a type
template<> struct simplify_type<::cdot::ast::SourceType> {
   using SimpleType = ::cdot::Type*;

   static SimpleType getSimplifiedValue(::cdot::ast::SourceType Val)
   {
      return Val.getResolvedType();
   }
};

template<>
struct PointerLikeTypeTraits<::cdot::ast::SourceType> {
public:
   static inline void *getAsVoidPointer(::cdot::ast::SourceType P)
   {
      return P.getResolvedType().getAsOpaquePtr();
   }

   static inline ::cdot::ast::SourceType getFromVoidPointer(void *P)
   {
      return ::cdot::ast::SourceType(::cdot::QualType::getFromOpaquePtr(P));
   }

   // Various qualifiers go in low bits.
   enum { NumLowBitsAvailable = 0 };
};

template<> struct DenseMapInfo<::cdot::ast::SourceType> {
   static ::cdot::ast::SourceType getEmptyKey() {
      uintptr_t Val = static_cast<uintptr_t>(-1);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::ast::SourceType(::cdot::QualType::getFromOpaquePtr(
         (void*)Val));
   }

   static ::cdot::ast::SourceType getTombstoneKey() {
      uintptr_t Val = static_cast<uintptr_t>(-2);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::ast::SourceType(::cdot::QualType::getFromOpaquePtr(
         (void*)Val));
   }

   static int getHashValue(const ::cdot::ast::SourceType &P) {
      return (int)(uintptr_t)P.getResolvedType().getAsOpaquePtr();
   }

   static bool isEqual(const ::cdot::ast::SourceType &LHS,
                       const ::cdot::ast::SourceType &RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm

#endif //CDOT_SOURCETYPE_H
