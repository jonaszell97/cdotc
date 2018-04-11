//
// Created by Jonas Zell on 07.04.18.
//

#ifndef CDOT_DECLDENSEMAPINFO_H
#define CDOT_DECLDENSEMAPINFO_H

#include <cstdint>

namespace cdot {
namespace ast {
   class RecordDecl;
   class Statement;
   class Decl;
} // namespace ast
} // namespace cdot

namespace llvm {

namespace detail {
   enum { RecordDeclAlign = 2 };
}

template <typename T> class PointerLikeTypeTraits;
template<class T> struct DenseMapInfo;

template <typename T>
class PointerLikeTypeTraits;

template<>
class PointerLikeTypeTraits<::cdot::ast::Statement*> {
public:
   static inline void *getAsVoidPointer(::cdot::ast::Statement* P) { return P; }

   static inline ::cdot::ast::Statement *getFromVoidPointer(void *P)
   {
      return static_cast<::cdot::ast::Statement*>(P);
   }

   enum { NumLowBitsAvailable = detail::RecordDeclAlign };
};

template<>
class PointerLikeTypeTraits<::cdot::ast::Decl*> {
public:
   static inline void *getAsVoidPointer(::cdot::ast::Decl* P) { return P; }

   static inline ::cdot::ast::Decl *getFromVoidPointer(void *P)
   {
      return static_cast<::cdot::ast::Decl*>(P);
   }

   enum { NumLowBitsAvailable = detail::RecordDeclAlign };
};

template<>
class PointerLikeTypeTraits<::cdot::ast::RecordDecl*> {
public:
   static inline void *getAsVoidPointer(::cdot::ast::RecordDecl*P) { return P; }

   static inline ::cdot::ast::RecordDecl*getFromVoidPointer(void *P)
   {
      return static_cast<::cdot::ast::RecordDecl*>(P);
   }

   enum { NumLowBitsAvailable = detail::RecordDeclAlign };
};

template<> struct DenseMapInfo<::cdot::ast::RecordDecl*> {
   static ::cdot::ast::RecordDecl *getEmptyKey()
   {
      uintptr_t Val = static_cast<uintptr_t>(-1);
      Val <<= detail::RecordDeclAlign;
      return (::cdot::ast::RecordDecl*)Val;
   }

   static ::cdot::ast::RecordDecl *getTombstoneKey()
   {
      uintptr_t Val = static_cast<uintptr_t>(-2);
      Val <<= detail::RecordDeclAlign;
      return (::cdot::ast::RecordDecl*)Val;
   }

   static int getHashValue(const ::cdot::ast::RecordDecl* P)
   {
      return (int)(uintptr_t)P;
   }

   static bool isEqual(const ::cdot::ast::RecordDecl *LHS,
                       const ::cdot::ast::RecordDecl *RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm

#endif //CDOT_DECLDENSEMAPINFO_H
