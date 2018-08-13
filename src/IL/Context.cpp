//
// Created by Jonas Zell on 17.11.17.
//

#include "Context.h"

#include "Driver/Compiler.h"
#include "Module.h"
#include "IL/Constants.h"
#include "IL/Function.h"
#include "IL/ValueSymbolTable.h"
#include "Serialization/ModuleFile.h"

using namespace cdot::support;

namespace cdot {
namespace il {

Context::Context(CompilerInstance &CI) : CI(CI)
{

}

template<class T>
void destroyFoldingSet(llvm::FoldingSet<T> &Set)
{
   // The nodes themselves are iterators, so be careful not to access the
   // next pointer after freeing.
   for (auto It = Set.begin(), End = Set.end(); It != End;) {
      auto Copy = It;
      ++Copy;

      delete &*It;
      It = Copy;
   }
}

Context::~Context()
{
   // Modules remove themselves when deleted, so be careful not to invalidate
   // iterators.
   SmallVector<Module*, 4> Mods(Modules.begin(), Modules.end());
   for (auto *Mod : Mods) {
      delete Mod;
   }

   delete TokNone;
   delete TrueVal;
   delete FalseVal;

   destroyFoldingSet(ArrayConstants);
   destroyFoldingSet(StructConstants);
   destroyFoldingSet(ClassConstants);
   destroyFoldingSet(UnionConstants);
   destroyFoldingSet(EnumConstants);
   destroyFoldingSet(BitCastConstants);
   destroyFoldingSet(IntCastConstants);
   destroyFoldingSet(AddrOfConstants);
   destroyFoldingSet(LoadConstants);
   destroyFoldingSet(OperatorConstants);
   destroyFoldingSet(GEPConstants);
}

void Context::registerModule(Module *M) { Modules.insert(M); }
void Context::removeModule(Module *M) { Modules.erase(M); }

ast::ASTContext& Context::getASTCtx() const
{
   return CI.getContext();
}

Function *Context::getFunction(llvm::StringRef name)
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
      if (fun) {
         if (auto Inf = fun->getLazyFnInfo())
            Inf->loadFunctionBody();

         if (!fun->isDeclared())
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

GlobalVariable *Context::getGlobalDefinition(llvm::StringRef name)
{
   for (const auto &M : Modules) {
      auto glob = M->getOwnGlobal(name);
      if (glob) {
         if (auto Inf = glob->getLazyGlobalInfo())
            Inf->loadGlobalInitializer();

         if (!glob->isDeclared())
            return glob;
      }
   }

   return nullptr;
}

} // namespace il
} // namespace cdot