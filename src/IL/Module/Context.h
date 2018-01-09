//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_CONTEXT_H
#define CDOT_CONTEXT_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringMap.h>

namespace cdot {

class Type;

namespace il {

class Module;
class AggregateType;
class Function;
class GlobalVariable;
class Value;

class Context {
public:
   using ModuleList         = llvm::SmallVector<Module*, 8>;
   using mod_iterator       = ModuleList::iterator;
   using mod_const_iterator = ModuleList::const_iterator;

   using TypeList           = llvm::StringMap<AggregateType*>;

   Context();

   void registerModule(Module *M) { Modules.push_back(M); }
   void registerType(AggregateType *Type);

   AggregateType *hasType(llvm::StringRef name) const;
   AggregateType *getType(llvm::StringRef name, Module *M) const;
   AggregateType *getType(llvm::StringRef name) const;

   Function *getFunction(llvm::StringRef name);
   Function *getFunctionDefinition(llvm::StringRef name);

   GlobalVariable *getGlobal(llvm::StringRef name);
   GlobalVariable *getGlobalDefinition(llvm::StringRef name);

   TypeList::iterator type_begin() { return Types.begin(); }
   TypeList::iterator type_end()   { return Types.end(); }

   TypeList::const_iterator type_begin() const { return Types.begin(); }
   TypeList::const_iterator type_end() const { return Types.end(); }

protected:
   llvm::SmallVector<Module*, 8> Modules;
   TypeList Types;
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONTEXT_H
