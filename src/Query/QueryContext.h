//
// Created by Jonas Zell on 24.08.18.
//

#ifndef CDOT_QUERYCONTEXT_H
#define CDOT_QUERYCONTEXT_H

#include "Query.h"

#include <llvm/ADT/DenseSet.h>
#include <llvm/Support/Allocator.h>

namespace llvm {
   // Provide DenseMapInfo for unsigned chars.
   template<> struct DenseMapInfo<unsigned char> {
      static inline unsigned char getEmptyKey() { return ~0; }
      static inline unsigned char getTombstoneKey() { return ~0 - 1; }
      static unsigned getHashValue(const unsigned char& Val) { return Val * 37U; }
      static bool isEqual(const unsigned char &LHS, const unsigned char &RHS) {
         return LHS == RHS;
      }
   };

   ENUM_DENSE_MAP_INFO(::cdot::ast::ImplicitConformanceKind);
} // namespace llvm

namespace cdot {

class CompilerInstance;

class QueryContext {
public:
   /// \brief Reference to the compiler instance.
   CompilerInstance &CI;

   /// \brief Pointer to the Sema instance. Can't be a reference, since Sema
   // has a refernce to this query context.
   ast::SemaPass *Sema = nullptr;

   /// \brief Reference to the ASTContext instance.
   ast::ASTContext &Context;

   /// \brief Allocator used to allocate memory for queries and their needed
   /// data.
   mutable llvm::BumpPtrAllocator Allocator;

   /// \brief Keeps track of queries that are currently being evaluated.
   SmallVector<Query*, 8> QueryStack;

private:
#  include "Inc/QueryContextFields.inc"

   void diagnoseCircularDependency(const Query *Q);

   /// \brief Helper RAII to keep track of running queries.
   struct ExecutingQuery {
      /// C'tor.
      ExecutingQuery(QueryContext &QC, Query *Q);

      /// D'tor.
      ~ExecutingQuery();

   private:
      QueryContext &QC;
   };

public:
   /// \brief Creates a new query context associated with the compiler instance
   /// \param CI.
   explicit QueryContext(CompilerInstance &CI);

   /// \brief Allocate \param size bytes.
   void *Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template <typename T>
   /// \brief Allocate \param Num * sizeof(T) bytes.
   T *Allocate(size_t Num = 1) const
   {
      return static_cast<T *>(Allocate(Num * sizeof(T), alignof(T)));
   }

   /// \brief Does nothing.
   void Deallocate(void*) const {}

   /// \brief Return a query if it already exists, or nullptr if not.
   template<class Q, class ...Args>
   Q *getQuery(Args...) { llvm_unreachable(""); }

#  include "Inc/QueryContextDecls.inc"
};

#include "Inc/QueryContextSpecializations.inc"

} // namespace cdot

inline void *operator new(size_t size, cdot::QueryContext const& Ctx,
                          size_t alignment = 8) {
   return Ctx.Allocate(size, alignment);
}

inline void operator delete(void *ptr, cdot::QueryContext const& Ctx,
                            size_t) {
   return Ctx.Deallocate(ptr);
}

inline void *operator new[](size_t size, cdot::QueryContext const& Ctx,
                            size_t alignment = 8) {
   return Ctx.Allocate(size, alignment);
}

inline void operator delete[](void *ptr, cdot::QueryContext const& Ctx,
                              size_t) {
   return Ctx.Deallocate(ptr);
}

#endif //CDOT_QUERYCONTEXT_H
