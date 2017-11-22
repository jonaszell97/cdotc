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

class Context {
public:
   Context();

   const llvm::SmallVector<Module *, 8> &getModules() const;
   const llvm::StringMap<AggregateType *> &getTypes() const;

   void registerModule(Module *M);
   void registerType(AggregateType *Type);

   AggregateType *hasType(llvm::StringRef name) const;
   AggregateType *getType(llvm::StringRef name, Module *M,
                          bool recursive = true) const;

   Function *getFunction(llvm::StringRef name);
   GlobalVariable *getGlobal(llvm::StringRef name);

protected:
   static llvm::SmallVector<Context*, 4> CreatedContexts;

   llvm::SmallVector<Module*, 8> Modules;

   llvm::StringMap<AggregateType*> Types;
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONTEXT_H
