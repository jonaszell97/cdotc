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

namespace cdot {
namespace il {

Function::Function(Callable *c, Module *parent)
   : Constant(FunctionID,
              FunctionType::get(c->getReturnType(), c->getArguments(), true),
              c->getName(),
              c->getDeclaration() ? c->getDeclaration()->getSourceLoc()
                                  : SourceLocation()),
     parent(parent), returnType(c->getReturnType())
{
   metaData->addNode(new MDFunction(c));
   for (const auto &arg : c->getArguments()) {
      this->args.push_back(new Argument(*arg.type, arg.isVararg, this,
                                        arg.label));
   }

   if (c->getDeclaration()) {
      setIsExternC(c->getDeclaration()->getExternKind() == ExternKind::C);
   }

   if (parent) {
      parent->insertFunction(this);
   }

   SubclassData |= Flag::Declared;
   if (c->throws()) {
      SubclassData |= Flag::Throws;
   }
}

Function::Function(const std::string &name,
                   QualType returnType,
                   llvm::ArrayRef<cdot::Argument> args,
                   Module *parent,
                   SourceLocation loc,
                   bool mightThrow)
   : Constant(FunctionID, FunctionType::get(returnType, args, true),
              name, loc),
     parent(parent), returnType(returnType)
{
   for (const auto &arg : args) {
      this->args.push_back(new Argument(*arg.type, arg.isVararg, this,
                                        arg.label));
   }

   SubclassData |= Flag::Declared;
   SubclassData |= Flag::Lambda;

   if (mightThrow) {
      SubclassData |= Flag::Throws;
   }

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(const std::string &name,
                   FunctionType *ty,
                   QualType returnType,
                   llvm::ArrayRef<Argument *> args, Module *parent,
                   SourceLocation loc,
                   unsigned SubClassData)
   : Constant(FunctionID, ty, name, loc),
     parent(parent), returnType(returnType),
     args(args.begin(), args.end())
{
   this->SubclassData = SubClassData | Flag::Declared;
   if (parent) {
      parent->insertFunction(this);
   }
}

bool Function::hasCallable() const
{
   return metaData->hasNode(MDFunctionID);
}

Callable* Function::getCallable() const
{
   return cast<MDFunction>(metaData->getNode(MDFunctionID))
      ->getCallable();
}

bool Function::isLambda() const
{
   return (SubclassData & Flag::Lambda) != 0;
}

const string &Function::getMangledName() const
{
   if (isLambda()) {
      return name;
   }

   return getCallable()->getMangledName();
}

string Function::getLinkageName() const
{
   if (isLambda()) {
      return name;
   }

   return getCallable()->getLinkageName();
}

QualType Function::getReturnType() const
{
   return returnType;
}

const Function::ArgList &Function::getArgs() const
{
   return args;
}

Function::BBList::reference Function::getEntryBlock()
{
   if (BasicBlocks.empty()) {
      addDefinition();
   }

   return BasicBlocks.front();
}

Function::iterator Function::getIteratorForBB(BasicBlock *bb)
{
   auto it = BasicBlocks.begin();
   while (it != BasicBlocks.end()) {
      if (*it == bb) {
         return it;
      }

      ++it;
   }

   llvm_unreachable("Basic Block does not belong to function!");
}

Function::const_iterator Function::getIteratorForBB(const BasicBlock *bb) const
{
   auto it = BasicBlocks.begin();
   while (it != BasicBlocks.end()) {
      if (*it == bb) {
         return it;
      }

      ++it;
   }

   llvm_unreachable("Basic Block does not belong to function!");
}

Function::iterator Function::removeBasicBlock(const BasicBlock *bb)
{
   auto it = getIteratorForBB(bb);
   return BasicBlocks.erase(it);
}

Function::iterator Function::insertBasicBlockAfter(BasicBlock *bb,
                                                   iterator it) {
   bb->addUse();
   return BasicBlocks.insert(++it, bb);
}

Function::iterator Function::insertBasicBlockBefore(BasicBlock *bb,
                                                    iterator it) {
   bb->addUse();
   return BasicBlocks.insert(it, bb);
}

Function::iterator Function::insertBasicBlockAtEnd(BasicBlock *bb)
{
   bb->addUse();
   BasicBlocks.push_back(bb);

   return BasicBlocks.end();
}

Function::iterator Function::insertBasicBlockAtBegin(BasicBlock *bb)
{
   bb->addUse();
   return BasicBlocks.insert(BasicBlocks.begin(), bb);
}

Module *Function::getParent() const
{
   return parent;
}

const Function::BBList &Function::getBasicBlocks() const
{
   return BasicBlocks;
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
   return (SubclassData & Flag::ExternC) != 0;
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

void Function::addDefinition()
{
   if (!isDeclared()) {
      return;
   }

   SubclassData &= ~Flag::Declared;

   auto EntryBlock = new BasicBlock(this, "entry", getLocation());
   if (auto M = dyn_cast<Method>(this)) {
      if (!M->isStatic()) {
         EntryBlock->addBlockArg(
            new Argument(ObjectType::get(M->getRecordType()->getName()),
                         false, this, "self"));
      }
   }
   for (const auto &arg : args) {
      EntryBlock->addBlockArg(arg);
   }
}

void Function::addArgument(const std::string &name, ILType ty)
{
   addArgument(new Argument(ty, false, this, name));
}

void Function::addArgument(Argument *arg)
{
   getEntryBlock()->addBlockArg(arg);
   args.push_back(arg);
}

void Function::addArgumentAtBegin(Argument *arg)
{
   auto &BBArgs = getEntryBlock()->getArgs();
   BBArgs.insert(BBArgs.begin(), arg);

   args.insert(args.begin(), arg);
}

void Function::addArgumentAtBegin(const std::string &name, ILType ty)
{
   addArgumentAtBegin(new Argument(ty, false, this, name));
}

Argument* Function::getArgument(unsigned idx) const
{
   assert(args.size() > idx);
   return args[idx];
}

Function* Function::getDeclarationIn(Module *M) const
{
   if (isa<Initializer>(this)) {
      if (hasCallable()) {
         return new Initializer(static_cast<cl::Method*>(getCallable()), M);
      }

      return new Initializer(name, type->asFunctionTy(), returnType, args,
                             cast<Method>(this)->getRecordType(), M,
                             getLocation(), mightThrow());
   }
   if (isa<Method>(this)) {
      if (hasCallable()) {
         return new Method(static_cast<cl::Method*>(getCallable()), M);
      }

      auto method = cast<Method>(this);
      return new Method(name, type->asFunctionTy(), returnType, args,
                        method->getRecordType(), method->isStatic(), M,
                        getLocation(), mightThrow());
   }

   if (hasCallable()) {
      return new Function(getCallable(), M);
   }

   return new Function(name, type->asFunctionTy(), returnType, args, M,
                       getLocation(), SubclassData);
}

MDFunction::MDFunction(Callable *c)
   : MetaData(MDFunctionID), callable(c)
{

}

Callable *MDFunction::getCallable() const
{
   return callable;
}

cl::Method *MDFunction::getMethod() const
{
   return method;
}

ast::Function *MDFunction::getFunction() const
{
   return function;
}

} // namespace il
} // namespace cdot