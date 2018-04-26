//
// Created by Jonas Zell on 17.11.17.
//

#include "Context.h"
#include "Module.h"

#include "IL/Constants.h"
#include "IL/Function.h"
#include "IL/ValueSymbolTable.h"

using namespace cdot::support;

namespace cdot {
namespace il {

Context::Context(ast::ASTContext &ASTCtx) : ASTCtx(ASTCtx)
{

}

template<class T>
void destroyFoldingSet(llvm::FoldingSet<T> &Set)
{
   for (T &Val : Set)
      delete &Val;
}

Context::~Context()
{
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

GlobalVariable *Context::getGlobalDefinition(llvm::StringRef name)
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