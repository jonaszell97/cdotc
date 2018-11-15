
#ifndef CDOT_LOOKUP_H
#define CDOT_LOOKUP_H

#include "AST/DeclBase.h"

namespace cdot {

class LambdaScope;

struct SingleLevelLookupResult: public ast::DeclContextLookupResult {
   SingleLevelLookupResult(const ast::DeclContextLookupResult &Result,
                           LambdaScope *LS)
      : DeclContextLookupResult(Result), LS(LS)
   {}

   SingleLevelLookupResult()
      : DeclContextLookupResult(), LS(nullptr)
   {}

   bool unique() const;

   LambdaScope *LS = nullptr;
};

class MultiLevelLookupResult {
   using ArrayTy = SmallVector<SingleLevelLookupResult, 2>;

   ArrayTy Vec;

public:
   using iterator       = ArrayTy::iterator;
   using const_iterator = ArrayTy::const_iterator;
   using reference      = SingleLevelLookupResult&;
   using const_reference = const SingleLevelLookupResult&;

   iterator begin() { return Vec.begin(); }
   iterator end()   { return Vec.end(); }

   const_iterator begin() const { return Vec.begin(); }
   const_iterator end()   const { return Vec.end(); }

   size_t size() const { return Vec.size(); }
   bool empty() const { return Vec.empty(); }

   const_reference front() const { return Vec.front(); }
   const_reference back() const { return Vec.back(); }

   reference front() { return Vec.front(); }
   reference back() { return Vec.back(); }

   reference operator[](unsigned Idx) { return Vec[Idx]; };
   const_reference operator[](unsigned Idx) const { return Vec[Idx]; };

   template<class ArrayTy, class ArrPtr, class ArrIt>
   struct all_decl_iterator_t {
   private:
      ArrPtr CurrArr;
      ArrPtr ArrEnd;
      ArrIt CurrDecl;

   public:
      // Begin iterator
      all_decl_iterator_t(ArrayTy &Arr) : CurrDecl(nullptr)
      {
         CurrArr = Arr.begin();
         ArrEnd = Arr.end();

         if (!Arr.empty()) {
            CurrDecl = CurrArr->begin();
         }
      }

      // End iterator
      all_decl_iterator_t(ArrayTy &Arr, bool) : CurrDecl(nullptr)
      {
         CurrArr = Arr.end();
         ArrEnd = Arr.end();

         if (!Arr.empty()) {
            CurrDecl = Arr.back().end();
         }
      }

      all_decl_iterator_t()
         : CurrArr(nullptr), ArrEnd(nullptr), CurrDecl(nullptr)
      {}

      all_decl_iterator_t &operator++()
      {
         ++CurrDecl;
         if (CurrDecl == CurrArr->end()) {
            ++CurrArr;

            if (CurrArr != ArrEnd)
               CurrDecl = CurrArr->begin();
         }

         return *this;
      }

      all_decl_iterator_t operator++(int)
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

      bool operator==(const all_decl_iterator_t &rhs)
      {
         return CurrArr == rhs.CurrArr;
      }

      bool operator!=(const all_decl_iterator_t &rhs)
      {
         return !(*this == rhs);
      }
   };

   using all_decl_iterator = all_decl_iterator_t<
      ArrayTy,
      SingleLevelLookupResult*,
      SingleLevelLookupResult::iterator>;

   using const_all_decl_iterator = all_decl_iterator_t<
      const ArrayTy,
      const SingleLevelLookupResult*,
      SingleLevelLookupResult::const_iterator>;

   using all_decl_range = llvm::iterator_range<all_decl_iterator>;
   using const_all_decl_range = llvm::iterator_range<const_all_decl_iterator>;

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

   const_all_decl_range allDecls() const
   {
      return const_all_decl_range(all_decl_begin(), all_decl_end());
   }

   const_all_decl_iterator all_decl_begin() const
   {
      return const_all_decl_iterator(Vec);
   }

   const_all_decl_iterator all_decl_end() const
   {
      return const_all_decl_iterator(Vec, bool());
   }

   /*implicit*/ operator bool() const { return !empty(); }

   void addResult(ast::DeclContextLookupResult Result,
                  LambdaScope *LS = nullptr) {
      Vec.emplace_back(Result, LS);
   }

   void addResult(const MultiLevelLookupResult &Result)
   {
      for (auto &Res : Result)
         Vec.emplace_back(Res);
   }

   void addResult(const SingleLevelLookupResult &Result)
   {
      Vec.push_back(Result);
   }

   bool unique() const;
};

} // namespace cdot

#endif //CDOT_LOOKUP_H
