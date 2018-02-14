//
// Created by Jonas Zell on 30.01.18.
//

#ifndef CDOT_IDENTIFIERINFO_H
#define CDOT_IDENTIFIERINFO_H

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/Support/Allocator.h>

#include "../lex/TokenKinds.h"

namespace cdot {

class IdentifierTable;

class IdentifierInfo {
public:
   friend class IdentifierTable;

   llvm::StringRef getIdentifier() const
   {
      assert(EntryPtr && "no entry for identifier");
      return EntryPtr->getKey();
   }

   lex::tok::TokenType getKeywordTokenKind() const
   {
      return keywordTokenKind;
   }

   template <std::size_t StrLen>
   bool isStr(const char (&Str)[StrLen]) const
   {
      return getLength() == StrLen - 1
             && ::memcmp(getNameStart(), Str, StrLen - 1) == 0;
   }

   unsigned getLength() const
   {
      return EntryPtr->getKeyLength();
   }

   const char *getNameStart() const
   {
      return EntryPtr->getKeyData();
   }

private:
   IdentifierInfo()
      : EntryPtr(nullptr), keywordTokenKind(lex::tok::sentinel)
   {}

   llvm::StringMapEntry<IdentifierInfo*> *EntryPtr;
   lex::tok::TokenType keywordTokenKind;
};

class IdentifierTable {
public:
   using AllocatorTy = llvm::BumpPtrAllocator;
   using MapTy = llvm::StringMap<IdentifierInfo*, AllocatorTy>;

   IdentifierTable(unsigned initialSize = 8192)
      : IdentMap(initialSize)
   {}

   IdentifierInfo &get(llvm::StringRef key)
   {
      auto &Entry = *IdentMap.insert(std::make_pair(key, nullptr)).first;

      IdentifierInfo *&Info = Entry.second;
      if (Info) return *Info;

      auto *Mem = getAllocator().Allocate<IdentifierInfo>();
      Info = new (Mem) IdentifierInfo;

      Info->EntryPtr = &Entry;

      return *Info;
   }

   IdentifierInfo &get(llvm::StringRef key, lex::tok::TokenType kind)
   {
      auto &Info = get(key);
      Info.keywordTokenKind = kind;

      return Info;
   }

   AllocatorTy &getAllocator()
   {
      return IdentMap.getAllocator();
   }

   using iterator = MapTy::const_iterator;
   using const_iterator = MapTy::const_iterator;

   iterator begin() const { return IdentMap.begin(); }
   iterator end()   const { return IdentMap.end(); }
   unsigned size()  const { return IdentMap.size(); }

   void addKeywords();
   void addTblGenKeywords();
   void addModuleKeywords();
   void addILKeywords();

private:
   MapTy IdentMap;

   void addKeyword(lex::tok::TokenType kind, llvm::StringRef kw);
};

} // namespace cdot

#endif //CDOT_IDENTIFIERINFO_H
