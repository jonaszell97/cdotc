//
// Created by Jonas Zell on 16.11.17.
//

#include "Module.h"
#include "Context.h"

#include "IL/Constants.h"
#include "IL/Function.h"
#include "IL/BasicBlock.h"
#include "IL/Argument.h"
#include "IL/GlobalVariable.h"
#include "IL/Instruction.h"
#include "IL/ValueSymbolTable.h"
#include "IL/Writer/ModuleWriter.h"
#include "Serialization/ModuleFile.h"

#include <llvm/Support/FileSystem.h>

using namespace cdot::support;

namespace cdot {
namespace il {

Module::Module(Context &Ctx, size_t fileID,
               llvm::StringRef fileName, llvm::StringRef path)
   : Functions(this), GlobalVariables(this), Ctx(Ctx),
     fileID(fileID), fileName(fileName), path(path)
{
   Ctx.registerModule(this);
}

Module::~Module()
{
   if (ExternalLookup)
      ExternalLookup->~ILModuleFile();

   Ctx.removeModule(this);
}

Function *Module::insertFunction(Function *Fn,
                                 bool OverridePrevious,
                                 Function **Previous) {
   if (OverridePrevious) {
      auto *Prev = Functions.find(Fn->getName());
      if (Prev && (!Fn->isDeclared() || Fn->getLazyFnInfo())
          && Prev->overridePreviousDefinition()) {
         if (Previous)
            *Previous = Prev;

         Prev->replaceAllUsesWith(Fn);
         Prev->detachAndErase();
      }
      else if (Prev) {
         return Prev;
      }
   }

   Fn->setParent(this);
   Functions.push_back(Fn);
   HasExternallyVisibleSymbols |= Fn->isExternallyVisible();

   return Fn;
}

Function *Module::getFunction(llvm::StringRef name)
{
   auto fun = Functions.find(name);
   if (!fun) {
      auto func = Ctx.getFunction(name);
      return func->getDeclarationIn(this);
   }

   return fun;
}

Function *Module::getOwnFunction(llvm::StringRef name)
{
   auto Fn = Functions.find(name);
   if (Fn)
      return Fn;

   if (ExternalLookup) {
      Fn = dyn_cast_or_null<Function>(ExternalLookup->Lookup(name));
   }

   return Fn;
}

GlobalVariable *Module::insertGlobal(GlobalVariable *G,
                                     bool OverridePrevious,
                                     GlobalVariable **Previous) {
   if (OverridePrevious) {
      auto *Prev = GlobalVariables.find(G->getName());
      if (Prev && (!G->isDeclared() || G->getLazyGlobalInfo())
          && Prev->overridePreviousDefinition()) {
         if (Previous)
            *Previous = Prev;

         Prev->replaceAllUsesWith(G);
         Prev->detachAndErase();
      }
      else if (Prev) {
         return Prev;
      }
   }

   G->setParent(this);
   GlobalVariables.push_back(G);
   HasExternallyVisibleSymbols |= G->isExternallyVisible();

   return G;
}

GlobalVariable *Module::getGlobal(llvm::StringRef name)
{
   auto val = GlobalVariables.find(name);
   if (!val) {
      auto glob = Ctx.getGlobal(name);
      return glob->getDeclarationIn(this);
   }

   return val;
}

GlobalVariable *Module::getOwnGlobal(llvm::StringRef name)
{
   return GlobalVariables.find(name);
}

void Module::addRecord(ast::RecordDecl *R)
{
   Records.insert(R);
}

void Module::addTypeInfo(ast::RecordDecl *R, il::GlobalVariable *TI)
{
   TIMap[R] = TI;
}

GlobalVariable *Module::getTypeInfo(ast::RecordDecl *R) const
{
   auto it = TIMap.find(R);
   if (it == TIMap.end())
      return nullptr;

   return it->getSecond();
}

void Module::addVTable(ast::RecordDecl *R, il::GlobalVariable *VT)
{
   VTMap[R] = VT;
}

GlobalVariable *Module::getVTable(ast::RecordDecl *R) const
{
   auto it = VTMap.find(R);
   if (it == VTMap.end())
      return nullptr;

   return it->getSecond();
}

void Module::addPTable(ast::RecordDecl *R, il::GlobalVariable *PT)
{
   PTMap[R] = PT;
}

GlobalVariable *Module::getPTable(ast::RecordDecl *R) const
{
   auto it = PTMap.find(R);
   if (it == PTMap.end())
      return nullptr;

   return it->getSecond();
}

Context &Module::getContext() const
{
   return Ctx;
}

void Module::dump() const
{
   writeTo(llvm::errs());
}

void Module::writeTo(llvm::raw_ostream &out) const
{
   ModuleWriter Writer(this);
   Writer.WriteTo(out);
}

void Module::writeToFile(const char *FileName) const
{
   std::error_code EC;
   llvm::raw_fd_ostream OS(FileName, EC, llvm::sys::fs::F_RW);

   if (EC)
      llvm::report_fatal_error(EC.message());

   writeTo(OS);
}

bool Module::linkInModule(std::unique_ptr<Module> &&M,
                          llvm::function_ref<void(GlobalObject*, GlobalObject*)>
                              Callback) {
   if (&M->getContext() != &getContext())
      return true;

   // copy all of the modules referenced records
   for (auto R : M->getRecords())
      addRecord(R);

   // move all of the modules globals
   for (auto it = M->GlobalVariables.begin(), end = M->GlobalVariables.end();
        it != end;) {
      GlobalVariable &G = *it;
      auto NextIt = it;
      ++NextIt;

      M->GlobalVariables.remove(it);
      it = NextIt;

      GlobalVariable *Prev = nullptr;
      auto NewGlob = insertGlobal(&G, true, &Prev);

      if (Callback && Prev)
         Callback(Prev ? Prev : &G, NewGlob);

      if (NewGlob != &G) {
         G.replaceAllUsesWith(NewGlob);
         G.eraseValue();
      }
   }

   // move all of the modules functions
   for (auto it = M->Functions.begin(), end = M->Functions.end(); it != end;) {
      Function &F = *it;
      auto NextIt = it;
      ++NextIt;

      M->Functions.remove(it);
      it = NextIt;

      Function *Prev = nullptr;
      auto NewFn = insertFunction(&F, !isa<Lambda>(F), &Prev);

      if (Callback)
         Callback(Prev ? Prev : &F, NewFn);

      if (NewFn != &F) {
         F.replaceAllUsesWith(NewFn);
         F.eraseValue();
      }
   }

   return false;
}

} // namespace il
} // namespace cdot
