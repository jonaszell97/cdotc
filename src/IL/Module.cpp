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

#include "IL/Serialize/Serialization.h"
#include "IL/Writer/ModuleWriter.h"

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

void Module::serializeTo(llvm::raw_ostream &out) const
{
   serializeModule(this, out);
}

} // namespace il
} // namespace cdot
