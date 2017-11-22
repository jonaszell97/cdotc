//
// Created by Jonas Zell on 17.11.17.
//

#include "Context.h"
#include "Module.h"
#include "../Value/Function/Method.h"
#include "../Value/Record/AggregateType.h"

namespace cdot {
namespace il {

llvm::SmallVector<Context*, 4> Context::CreatedContexts;

Context::Context()
{
   CreatedContexts.push_back(this);
}

AggregateType *Context::getType(llvm::StringRef name,
                                Module *M, bool recursive) const {
   auto it = Types.find(name);
   if (it == Types.end()) {
      llvm_unreachable("type not found");
   }

   M->addTypeReference(it->second, recursive);
   return it->second;
}

AggregateType * Context::hasType(llvm::StringRef name) const
{
   auto it = Types.find(name);
   if (it == Types.end()) {
      return nullptr;
   }

   return it->second;
}

Function *Context::getFunction(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto fun = M->getOwnFunction(name);
      if (fun) {
         return fun;
      }
   }

   return nullptr;
}

GlobalVariable *Context::getGlobal(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto glob = M->getOwnGlobal(name);
      if (glob) {
         return glob;
      }
   }

   return nullptr;
}

void Context::registerModule(Module *M)
{
   Modules.push_back(M);
}

const llvm::SmallVector<Module *, 8> &Context::getModules() const
{
   return Modules;
}

void Context::registerType(AggregateType *Type)
{
   assert(Types.find(Type->getName()) == Types.end()
          && "duplicate type declaration");

   Types.try_emplace(Type->getName(), Type);
}

const llvm::StringMap<AggregateType *> &Context::getTypes() const
{
   return Types;
}

} // namespace il
} // namespace cdot