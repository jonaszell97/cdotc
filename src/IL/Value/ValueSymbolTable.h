//
// Created by Jonas Zell on 23.11.17.
//

#ifndef CDOT_VALUESYMBOLTABLE_H
#define CDOT_VALUESYMBOLTABLE_H

#include <llvm/ADT/StringMap.h>

namespace llvm {
template <unsigned InternalLen> class SmallString;
}

namespace cdot {
namespace il {

class Context;
class Module;
class GlobalVariable;
class AggregateType;
class Function;
class Argument;
class BasicBlock;
class Instruction;

class Value;

class ValueSymbolTable {
public:
   using VMap           = llvm::StringMap<Value*>;
   using iterator       = VMap::iterator;
   using const_iterator = VMap::const_iterator;

   ValueSymbolTable();

   Value *lookup(llvm::StringRef name);

   iterator begin()             { return vmap.begin(); }
   iterator end()               { return vmap.end(); }
   const_iterator begin() const { return vmap.begin(); }
   const_iterator end()   const { return vmap.end(); }

   size_t size()          const { return vmap.size(); }
   bool empty()           const { return vmap.empty(); }

   void reinsertWithName(llvm::StringRef newName, Value *V);
   void insertValue(Value *V);

   void removeValue(Value *V);
   void removeValue(iterator it);

   llvm::StringMapEntry<Value*> *CreateUniqueName(Value *V,
                                            llvm::SmallString<256> &UniqueName);

private:
   VMap vmap;
   mutable size_t lastUnique = 0;
};

} // namespace il
} // namespace cdot


#endif //CDOT_VALUESYMBOLTABLE_H
