
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

   struct all_decl_iterator {
   private:
      ast::DeclContextLookupResult *CurrArr;
      ast::DeclContextLookupResult *ArrEnd;
      ast::DeclContextLookupResult::iterator CurrDecl;

   public:
      // Begin iterator
      all_decl_iterator(ArrayTy &Arr) : CurrDecl(nullptr)
      {
         CurrArr = Arr.begin();
         ArrEnd = Arr.end();

         if (!Arr.empty()) {
            CurrDecl = CurrArr->begin();
         }
      }

      // End iterator
      all_decl_iterator(ArrayTy &Arr, bool) : CurrDecl(nullptr)
      {
         CurrArr = Arr.end();
         ArrEnd = Arr.end();

         if (!Arr.empty()) {
            CurrDecl = Arr.back().end();
         }
      }

      all_decl_iterator()
         : CurrArr(nullptr), ArrEnd(nullptr), CurrDecl(nullptr)
      {}

      all_decl_iterator &operator++()
      {
         ++CurrDecl;
         if (CurrDecl == CurrArr->end()) {
            ++CurrArr;

            if (CurrArr != ArrEnd)
               CurrDecl = CurrArr->begin();
         }

         return *this;
      }

      all_decl_iterator operator++(int)
      {
         auto cpy = *this;

         ++CurrDecl;
         if (CurrDecl == CurrArr->end()) {
            ++CurrArr;

            if (CurrArr != ArrEnd)
               CurrDecl = CurrArr->begin();
         }

         return cpy;
      }

      ast::NamedDecl *operator*() const { return *CurrDecl; }
      ast::NamedDecl *operator->() const { return *CurrDecl; }

      bool operator==(const all_decl_iterator &rhs)
      {
         return CurrArr == rhs.CurrArr;
      }

      bool operator!=(const all_decl_iterator &rhs)
      {
         return !(*this == rhs);
      }
   };

   using all_decl_range = llvm::iterator_range<all_decl_iterator>;

   all_decl_range allDecls()
   {
      return all_decl_range(all_decl_begin(), all_decl_end());
   }

   all_decl_iterator all_decl_begin()
   {
      return all_decl_iterator(Vec);
   }

   all_decl_iterator all_decl_end()
   {
      return all_decl_iterator(Vec, bool());
   }

   /*implicit*/ operator bool() const { return !empty(); }

   LambdaScope* getLambdaScope() const { return LS; }
   void setLambdaScope(LambdaScope* V) { LS = V; }

   void addResult(ast::DeclContextLookupResult Result)
   {
      Vec.push_back(Result);
   }

   void addResult(const MultiLevelLookupResult &Result)
   {
      for (auto &Res : Result)
         Vec.emplace_back(Res);
   }
};

} // namespace cdot

#endif //CDOT_LOOKUP_H
