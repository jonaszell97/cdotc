#ifndef CDOT_QUERYCONTEXT_H
#define CDOT_QUERYCONTEXT_H

#include "cdotc/Query/Query.h"

#include <llvm/ADT/DenseSet.h>
#include <llvm/Support/Allocator.h>

namespace llvm {
// Provide DenseMapInfo for unsigned chars.
template<> struct DenseMapInfo<unsigned char> {
   static inline unsigned char getEmptyKey() { return ~0; }
   static inline unsigned char getTombstoneKey() { return ~0 - 1; }
   static unsigned getHashValue(const unsigned char& Val) { return Val * 37U; }
   static bool isEqual(const unsigned char& LHS, const unsigned char& RHS)
   {
      return LHS == RHS;
   }
};

// Provide DenseMapInfo for unsigned chars.
template<> struct DenseMapInfo<::cdot::Module*> {
   static inline ::cdot::Module* getEmptyKey()
   {
      uintptr_t Val = static_cast<uintptr_t>(-1);
      return (::cdot::Module*)Val;
   }

   static inline ::cdot::Module* getTombstoneKey()
   {
      uintptr_t Val = static_cast<uintptr_t>(-2);
      return (::cdot::Module*)Val;
   }

   static unsigned getHashValue(const ::cdot::Module* Val)
   {
      return (int)(uintptr_t)Val;
   }

   static bool isEqual(const ::cdot::Module* LHS, const ::cdot::Module* RHS)
   {
      return LHS == RHS;
   }
};

template<> struct PointerLikeTypeTraits<::cdot::il::Constant*> {
public:
   static inline void* getAsVoidPointer(::cdot::il::Constant* P) { return P; }

   static inline ::cdot::il::Constant* getFromVoidPointer(void* P)
   {
      return static_cast<::cdot::il::Constant*>(P);
   }

   enum { NumLowBitsAvailable = 0 };
};

ENUM_DENSE_MAP_INFO(::cdot::ast::ImplicitConformanceKind);
} // namespace llvm

namespace cdot {

class CompilerInstance;

class QueryContext {
public:
   /// \brief Reference to the compiler instance.
   CompilerInstance& CI;

   /// \brief Pointer to the Sema instance. Can't be a reference, since Sema
   // has a refernce to this query context.
   ast::SemaPass* Sema = nullptr;

   /// \brief Reference to the ASTContext instance.
   ast::ASTContext& Context;

   /// \brief Allocator used to allocate memory for queries and their needed
   /// data.
   mutable llvm::BumpPtrAllocator Allocator;

   /// \brief Keeps track of queries that are currently being evaluated.
   SmallVector<Query*, 8> QueryStack;

#ifndef NDEBUG
   /// \brief If true, ignore query status and re-execute every query.
   bool rerunQueries = false;
#endif

private:
#include "cdotc/Query/Inc/QueryContextFields.inc"

   void diagnoseCircularDependency(const Query* Q);

   /// \brief Helper RAII to keep track of running queries.
   struct ExecutingQuery {
      /// C'tor.
      ExecutingQuery(QueryContext& QC, Query* Q);

      /// D'tor.
      ~ExecutingQuery();

   private:
      QueryContext& QC;
   };

#ifndef NDEBUG
   bool shouldReset() const { return rerunQueries; }
#else
   bool shouldReset() const { return false; }
#endif

public:
   /// \brief Creates a new query context associated with the compiler instance
   /// \param CI.
   explicit QueryContext(CompilerInstance& CI);

   /// \brief Allocate \param size bytes.
   void* Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template<typename T>
   /// \brief Allocate \param Num * sizeof(T) bytes.
   T* Allocate(size_t Num = 1) const
   {
      return static_cast<T*>(Allocate(Num * sizeof(T), alignof(T)));
   }

   /// \brief Does nothing.
   void Deallocate(void*) const {}

   /// \brief Return a query if it already exists, or nullptr if not.
   template<class Q, class... Args> Q* getQuery(Args...)
   {
      llvm_unreachable("");
   }

#include "cdotc/Query/Inc/QueryContextDecls.inc"
};

#include "cdotc/Query/Inc/QueryContextSpecializations.inc"

} // namespace cdot

inline void* operator new(size_t size, cdot::QueryContext const& Ctx,
                          size_t alignment = 8)
{
   return Ctx.Allocate(size, alignment);
}

inline void operator delete(void* ptr, cdot::QueryContext const& Ctx, size_t)
{
   return Ctx.Deallocate(ptr);
}

inline void* operator new[](size_t size, cdot::QueryContext const& Ctx,
                            size_t alignment = 8)
{
   return Ctx.Allocate(size, alignment);
}

inline void operator delete[](void* ptr, cdot::QueryContext const& Ctx, size_t)
{
   return Ctx.Deallocate(ptr);
}

#endif // CDOT_QUERYCONTEXT_H
