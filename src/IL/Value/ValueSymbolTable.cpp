//
// Created by Jonas Zell on 23.11.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallString.h>
#include "ValueSymbolTable.h"
#include "Value.h"

namespace cdot {
namespace il {

ValueSymbolTable::ValueSymbolTable()
{
   vmap.insert(std::make_pair("", (Value*)nullptr));
}

Value *ValueSymbolTable::lookup(llvm::StringRef name)
{
   auto it = vmap.find(name);
   return it == vmap.end() ? nullptr : it->second;
}

void ValueSymbolTable::insertValue(Value *V)
{
   if (V->hasName()) {
      auto InsertResult = vmap.insert(std::make_pair(V->getName(), V));
      if (InsertResult.second)
         return;
   }

   llvm::SmallString<256> UniqueName(V->getName());
   auto UN = CreateUniqueName(V, UniqueName);
   V->setNameNoCheck(UN->getKey());
}

void ValueSymbolTable::reinsertWithName(llvm::StringRef newName,
                                        Value *V) {
   if (V->hasName()) {
      vmap.erase(vmap.find(V->getName()));
   }

   auto InsertResult = vmap.insert(std::make_pair(newName, V));
   if (InsertResult.second) {
      V->setNameNoCheck(InsertResult.first->getKey());
      return;
   }

   llvm::SmallString<256> UniqueName(newName);
   auto UN = CreateUniqueName(V, UniqueName);
   V->setNameNoCheck(UN->getKey());
}

void ValueSymbolTable::removeValue(Value *V)
{
   assert(V->hasName() && "can't erase unnamed value");
   auto it = vmap.find(V->getName());
   assert(it != vmap.end());

   vmap.erase(it);
}

void ValueSymbolTable::removeValue(iterator it)
{
   vmap.erase(it);
}

llvm::StringMapEntry<Value*>*
ValueSymbolTable::CreateUniqueName(Value *V,
                                   llvm::SmallString<256> &UniqueName) {
   auto base = UniqueName.size();
   for (;;) {
      UniqueName.resize(base);
      llvm::raw_svector_ostream S(UniqueName);
      if (base)
         S << '.';

      S << lastUnique++;

      auto InsertResult = vmap.insert(std::make_pair(UniqueName, V));
      if (InsertResult.second) {
         return &*InsertResult.first;
      }
   }
}

} // namespace il
} // namespace cdot