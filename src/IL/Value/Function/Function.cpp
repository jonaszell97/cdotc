//
// Created by Jonas Zell on 16.11.17.
//

#include "Function.h"
#include "BasicBlock.h"
#include "Argument.h"

#include "../../Module/Module.h"
#include "../Record/AggregateType.h"

#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

#include "../../../Variant/Type/Generic.h"
#include "../../../Variant/Type/FunctionType.h"

#include "../../../AST/Passes/SemanticAnalysis/Function.h"
#include "../../../AST/Statement/Declaration/CallableDecl.h"

#include "Method.h"

using namespace cdot::support;

namespace cdot {
namespace il {

namespace {

FunctionType *makeFuncTy(QualType &returnType,
                         llvm::ArrayRef<Argument*> args) {
   std::vector<cdot::Argument> realArgs;
   for (const auto &arg : args) {
      realArgs.emplace_back(arg->getName(), arg->getType(),
                            nullptr, false, arg->isVararg());
   }

   return FunctionType::get(returnType, realArgs, true);
}

} // anonymous namespace

Function::Function(const std::string &name,
                   QualType returnType,
                   llvm::ArrayRef<Argument *> args,
                   Module *parent,
                   bool mightThrow,
                   bool isExternC)
   : GlobalObject(FunctionID, makeFuncTy(returnType, args),
                  parent, name),
     returnType(returnType), BasicBlocks(this)
{
   auto EntryBlock = new BasicBlock(this);
   EntryBlock->setName("entry");

   for (const auto &arg : args) {
      EntryBlock->getArgs().push_back(arg);
   }

   setIsExternC(isExternC);
   SubclassData |= Flag::Declared;

   if (mightThrow) {
      SubclassData |= Flag::Throws;
   }

   if (returnType->needsStructReturn())
      SubclassData |= Flag::SRet;

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(TypeID id, FunctionType *Ty, const std::string &name,
                   QualType returnType, llvm::ArrayRef<Argument *> args,
                   Module *parent, bool mightThrow, bool isExternC)
   : GlobalObject(id, Ty, parent, name),
     returnType(returnType), BasicBlocks(this)
{
   auto EntryBlock = new BasicBlock(this);
   EntryBlock->setName("entry");

   for (const auto &arg : args) {
      EntryBlock->getArgs().push_back(arg);
   }

   setIsExternC(isExternC);
   SubclassData |= Flag::Declared;

   if (mightThrow) {
      SubclassData |= Flag::Throws;
   }

   if (returnType->needsStructReturn())
      SubclassData |= Flag::SRet;

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(const Function &other)
   : GlobalObject(FunctionID, *other.getType(), nullptr, other.name),
     returnType(other.returnType), BasicBlocks(this)
{
   auto EntryBlock = new BasicBlock(this);
   EntryBlock->setName("entry");

   for (const auto &arg : other.getEntryBlock()->getArgs()) {
      EntryBlock->getArgs().push_back(new Argument(arg.getType(),
                                                   arg.isVararg(),
                                                   EntryBlock));
   }

   metaData = other.metaData;
   SubclassData = other.SubclassData | Flag::Declared;

   if (auto Loc = other.getLocation())
      addMetaData(Loc);
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
   if (!isDeclared()) {
      return;
   }

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

Lambda::Lambda(QualType returnType,
               llvm::ArrayRef<Argument *> args,
               Module *parent,
               bool mightThrow)
   : Function(LambdaID, makeFuncTy(returnType, args),
              "__anonymous_lambda", returnType, args, parent,
              mightThrow, false)
{

}

} // namespace il
} // namespace cdot