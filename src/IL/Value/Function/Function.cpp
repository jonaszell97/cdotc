//
// Created by Jonas Zell on 16.11.17.
//

#include "Function.h"
#include "Method.h"
#include "BasicBlock.h"
#include "Argument.h"

#include "IL/Module/Module.h"
#include "IL/Value/Record/AggregateType.h"

#include "AST/NamedDecl.h"

#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

Function::Function(const std::string &name,
                   FunctionType *funcTy,
                   Module *parent,
                   bool isExternC)
   : GlobalObject(FunctionID, funcTy, parent, name),
     BasicBlocks(this)
{
   setIsExternC(isExternC);
   SubclassData |= Flag::Declared;

   if (!funcTy->isNoThrow())
      SubclassData |= Flag::Throws;

   if (funcTy->getReturnType()->needsStructReturn())
      SubclassData |= Flag::SRet;

   if (funcTy->isCStyleVararg())
      SubclassData |= Flag::Vararg;

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(TypeID id, FunctionType *Ty,
                   const std::string &name,
                   Module *parent,
                   bool isExternC)
   : GlobalObject(id, Ty, parent, name),
     BasicBlocks(this)
{
   setIsExternC(isExternC);
   SubclassData |= Flag::Declared;

   if (!Ty->isNoThrow())
      SubclassData |= Flag::Throws;

   if (Ty->getReturnType()->needsStructReturn())
      SubclassData |= Flag::SRet;

   if (Ty->isCStyleVararg())
      SubclassData |= Flag::Vararg;

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(const Function &other)
   : GlobalObject(FunctionID, other.getType(), nullptr, other.name),
     BasicBlocks(this)
{
   metaData = other.metaData;
   SubclassData = other.SubclassData | Flag::Declared;
   loc = other.loc;
}

bool Function::isLambda() const
{
   return isa<Lambda>(this);
}

bool Function::hasStructReturn() const
{
   return (SubclassData & Flag::SRet) != 0;
}

BasicBlock const* Function::getEntryBlock() const
{
   return &BasicBlocks.front();
}

BasicBlock* Function::getEntryBlock()
{
   return &BasicBlocks.front();
}

bool Function::isDeclared() const
{
   return (SubclassData & Flag::Declared) != 0;
}

bool Function::mightThrow() const
{
   return (SubclassData & Flag::Throws) != 0;
}

bool Function::isExternC() const
{
   return isLambda() || (SubclassData & Flag::ExternC) != 0;
}

void Function::setIsExternC(bool ext)
{
   if (ext) {
      SubclassData |= Flag::ExternC;
   }
   else {
      SubclassData &= ~Flag::ExternC;
   }
}

bool Function::isGlobalInitFn() const
{
   return this == parent->getGlobalInitFn();
}

void Function::addDefinition()
{
   SubclassData &= ~Flag::Declared;
}

Function* Function::getDeclarationIn(Module *M)
{
   if (parent == M)
      return this;

   Function *f;
   if (auto Init = dyn_cast<Initializer>(this)) {
      f = new Initializer(*Init);
   }
   else if (auto M = dyn_cast<Method>(this)) {
      f = new Method(*M);
   }
   else {
      f = new Function(*this);
   }

   f->parent = M;
   M->insertFunction(f);

   return f;
}

Lambda::Lambda(FunctionType *funcTy,
               Module *parent)
   : Function(LambdaID, funcTy, "__anonymous_lambda", parent, false)
{

}

} // namespace il
} // namespace cdot