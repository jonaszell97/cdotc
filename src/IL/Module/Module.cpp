//
// Created by Jonas Zell on 16.11.17.
//

#include "../Value/Record/AggregateType.h"

#include "Module.h"
#include "Context.h"

#include "../Value/Function/Function.h"
#include "../Value/Function/BasicBlock.h"
#include "../Value/Function/Argument.h"
#include "../Value/Instruction/Instruction.h"
#include "../Value/GlobalVariable.h"
#include "../Value/Constant/ConstantVal.h"
#include "../Writer/ModuleWriter.h"

namespace cdot {
namespace il {

Module::Module(Context &Ctx, CompilationUnit &CU)
   : Ctx(Ctx), CU(CU)
{
   Ctx.registerModule(this);
}

Function *Module::insertFunction(Function *func)
{
   auto name = func->getLinkageName();
   if (Functions.find(name) != Functions.end()) {
      size_t i = 0;

      auto newName = name + '.' + std::to_string(i);
      while (Functions.find(newName) != Functions.end()) {
         newName = name + '.' + std::to_string(++i);
      }

      func->setName(newName);
   }

   return Functions.try_emplace(func->getLinkageName(), func).first->second;
}

Function *Module::getFunction(llvm::StringRef name)
{
   auto it = Functions.find(name);
   if (it == Functions.end()) {
      auto func = Ctx.getFunction(name);
      return func->getDeclarationIn(this);
   }

   return it->second;
}

Function *Module::getOwnFunction(llvm::StringRef name)
{
   auto it = Functions.find(name);
   if (it == Functions.end()) {
      return nullptr;
   }

   return it->second;
}

GlobalVariable *Module::insertGlobal(GlobalVariable *global)
{
   assert(GlobalVariables.find(global->getName()) == GlobalVariables.end());
   return GlobalVariables.try_emplace(global->getName(), global).first->second;
}

GlobalVariable * Module::getGlobal(llvm::StringRef name)
{
   auto it = GlobalVariables.find(name);
   if (it == GlobalVariables.end()) {
      auto glob = Ctx.getGlobal(name);
      return glob->getDeclarationIn(this);
   }

   return it->second;
}

GlobalVariable *Module::getOwnGlobal(llvm::StringRef name)
{
   auto it = GlobalVariables.find(name);
   if (it == GlobalVariables.end()) {
      return nullptr;
   }

   return it->second;
}

CompilationUnit& Module::getCU() const
{
   return CU;
}

const llvm::StringMap<Function *> &Module::getFunctions() const
{
   return Functions;
}

const llvm::StringMap<GlobalVariable *> &Module::getGlobalVariables() const
{
   return GlobalVariables;
}

ConstantString* Module::getString(const std::string &str)
{
   auto it = Strings.find(str);
   if (it != Strings.end()) {
      return it->second;
   }

   return Strings.try_emplace(str, new ConstantString(str)).first->second;
}

AggregateType *Module::getType(llvm::StringRef name, bool recursive)
{
   return Ctx.getType(name, this, recursive);
}

void Module::addTypeReference(AggregateType *ty, bool recursive)
{
   auto isNew = ReferencedTypes.insert(ty);
   if (!isNew.second) {
      return;
   }

   if (recursive && isa<StructType>(ty)) {
      for (const auto &cont : cast<StructType>(ty)->getFields()) {
         if (cont.type->isObjectTy()) {
            Ctx.getType(cont.type->getClassName(), this);
         }
      }
   }
}

Context &Module::getContext() const
{
   return Ctx;
}

namespace {

void CheckIfDuplicate(Value *V, llvm::StringMap<size_t> &Names)
{
   auto name = V->getName();
   auto it = Names.find(name);
   if (it != Names.end()) {
      V->setName(name.str() + '.' + std::to_string(it->second));
      ++it->second;
   }
   else {
      Names.try_emplace(name, 0);
   }
}

} // anonymous namespace

void Module::AssignNames()
{
   llvm::StringMap<size_t> InstNames;

   for (auto &F : Functions) {
      size_t count = 0;
      for (auto &arg : F.second->getArgs()) {
         if (!arg->hasName()) {
            arg->setName(std::to_string(count++));
         }
         else {
            CheckIfDuplicate(arg, InstNames);
         }
      }

      for (auto &BB : F.second->getBasicBlocks()) {
         if (!BB->hasName()) {
            BB->setName("label" + std::to_string(count++));
         }
         else {
            CheckIfDuplicate(BB, InstNames);
         }

         for (auto &Arg : BB->getArgs()) {
            if (!Arg->hasName()) {
               Arg->setName(std::to_string(count++));
            }
            else {
               CheckIfDuplicate(Arg, InstNames);
            }
         }

         for (auto &I : BB->getInstructions()) {
            if (!I->hasName()) {
               I->setName(std::to_string(count++));
            }
            else {
               CheckIfDuplicate(I, InstNames);
            }
         }
      }

      InstNames.clear();
   }
}

void Module::dump()
{
   ModuleWriter Writer(this);
   Writer.WriteTo(llvm::errs());
}

void Module::writeTo(llvm::raw_ostream &out)
{
   ModuleWriter Writer(this);
   Writer.WriteTo(out);
}

const llvm::DenseSet<AggregateType *> &Module::getReferencedTypes() const
{
   return ReferencedTypes;
}

} // namespace il
} // namespace cdot
