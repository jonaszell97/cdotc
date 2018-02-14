//
// Created by Jonas Zell on 16.11.17.
//

#include "../Value/Record/AggregateType.h"

#include "Module.h"
#include "Context.h"

#include "../Value/ValueSymbolTable.h"

#include "../Value/Function/Function.h"
#include "../Value/Function/BasicBlock.h"
#include "../Value/Function/Argument.h"
#include "../Value/Instruction/Instruction.h"
#include "../Value/GlobalVariable.h"
#include "../Value/Constant/ConstantVal.h"

#include "../Serialize/Serialization.h"
#include "../Writer/ModuleWriter.h"

#include "../../Compiler.h"

using namespace cdot::support;

namespace cdot {
namespace il {

Module::Module(Context &Ctx, size_t fileID,
               llvm::StringRef fileName, llvm::StringRef path)
   : Types(this), Functions(this), GlobalVariables(this), Ctx(Ctx),
     fileID(fileID), fileName(fileName), path(path)
{
   Ctx.registerModule(this);
}

void Module::insertType(AggregateType *Ty)
{
   Types.push_back(Ty);
   Ctx.registerType(Ty);
}

Function *Module::insertFunction(Function *func)
{
   Functions.push_back(func);
   return func;
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
   return Functions.find(name);
}

GlobalVariable *Module::insertGlobal(GlobalVariable *global)
{
   GlobalVariables.push_back(global);
   return global;
}

GlobalVariable * Module::getGlobal(llvm::StringRef name)
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

AggregateType *Module::getType(llvm::StringRef name)
{
   return Ctx.getType(name, this);
}

bool Module::addTypeReference(AggregateType *ty)
{
   if (isa<ClassType>(ty))
      addTypeReference("cdot.ClassInfo");

   return ReferencedTypes.insert(ty).second;
}

bool Module::addTypeReference(llvm::StringRef name)
{
   auto ty = Ctx.getType(name);
   return ReferencedTypes.insert(ty).second;
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

void Module::serializeTo(llvm::raw_ostream &out) const
{
   serializeModule(this, out);
}

} // namespace il
} // namespace cdot
