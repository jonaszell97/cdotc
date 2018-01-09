//
// Created by Jonas Zell on 17.11.17.
//

#include "Context.h"
#include "Module.h"

#include "../Value/ValueSymbolTable.h"
#include "../Value/Function/Method.h"
#include "../Value/Record/AggregateType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

Context::Context()
{

}

void Context::registerType(AggregateType *Type)
{
   Types.try_emplace(Type->getName(), Type);
}

AggregateType *Context::getType(llvm::StringRef name,
                                Module *M) const {
   auto it = Types.find(name);
   if (it == Types.end()) {
      llvm_unreachable("type not found");
   }

   auto ty = cast<AggregateType>(it->second);
   M->addTypeReference(ty);

   return ty;
}

AggregateType * Context::getType(llvm::StringRef name) const
{
   auto it = Types.find(name);
   if (it == Types.end()) {
      llvm_unreachable("type not found");
   }

   return cast<AggregateType>(it->second);
}

AggregateType * Context::hasType(llvm::StringRef name) const
{
   auto it = Types.find(name);
   if (it == Types.end()) {
      return nullptr;
   }

   return dyn_cast<AggregateType>(it->second);
}

Function * Context::getFunction(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto fun = M->getOwnFunction(name);
      if (fun)
         return fun;
   }

   return nullptr;
}

Function *Context::getFunctionDefinition(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto fun = M->getOwnFunction(name);
      if (fun && !fun->isDeclared()) {
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

GlobalVariable * Context::getGlobalDefinition(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto glob = M->getOwnGlobal(name);
      if (glob && !glob->isDeclared()) {
         return glob;
      }
   }

   return nullptr;
}

} // namespace il
} // namespace cdot