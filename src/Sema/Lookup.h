
#ifndef CDOT_LOOKUP_H
#define CDOT_LOOKUP_H

#include "AST/DeclBase.h"

namespace cdot {

class LambdaScope;

class MultiLevelLookupResult {
   using ArrayTy = SmallVector<ast::DeclContextLookupResult, 2>;

   ArrayTy Vec;
   LambdaScope *LS = nullptr;

public:
   using iterator       = ArrayTy::iterator;
   using const_iterator = ArrayTy::const_iterator;

   iterator begin() { return Vec.begin(); }
   iterator end()   { return Vec.end(); }

   const_iterator begin() const { return Vec.begin(); }
   const_iterator end()   const { return Vec.end(); }

   size_t size() const { return Vec.size(); }
   bool empty() const { return Vec.empty(); }

   ast::DeclContextLookupResult const& front() const { return Vec.front(); }
   ast::DeclContextLookupResult const& back() const { return Vec.back(); }

   ast::DeclContextLookupResult& front() { return Vec.front(); }
   ast::DeclContextLookupResult& back() { return Vec.back(); }

   /*implicit*/ operator bool() const { return !empty(); }

   LambdaScope* getLambdaScope() const { return LS; }
   void setLambdaScope(LambdaScope* V) { LS = V; }

  void addResult(ast::DeclContextLookupResult &&Result)
  {
      Vec.emplace_back(std::move(Result));
  }
};

} // namespace cdot

#endif //CDOT_LOOKUP_H
