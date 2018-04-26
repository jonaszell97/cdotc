//
// Created by Jonas Zell on 16.11.17.
//

#include "Function.h"
#include "Argument.h"
#include "BasicBlock.h"

#include "AST/Decl.h"
#include "Module.h"

#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {

Function::Function(llvm::StringRef name,
                   FunctionType *funcTy,
                   Module *parent)
   : GlobalObject(FunctionID, funcTy->getCanonicalType(), parent, name),
     BasicBlocks(this)
{
   auto Canon = cast<FunctionType>(funcTy->getCanonicalType());

   FnBits.Declared = true;
   FnBits.Throws = Canon->throws();
   FnBits.SRet = Canon->getReturnType()->needsStructReturn();
   FnBits.Vararg = Canon->isCStyleVararg();

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(TypeID id, FunctionType *Ty,
                   llvm::StringRef name,
                   Module *parent)
   : GlobalObject(id, Ty, parent, name),
     BasicBlocks(this)
{
   auto Canon = cast<FunctionType>(Ty->getCanonicalType());

   FnBits.Declared = true;
   FnBits.Throws = Canon->throws();
   FnBits.SRet = Canon->getReturnType()->needsStructReturn();
   FnBits.Vararg = Canon->isCStyleVararg();

   if (parent) {
      parent->insertFunction(this);
   }
}

Function::Function(const Function &other)
   : GlobalObject(FunctionID, other.getType(), nullptr, other.name),
     BasicBlocks(this)
{
   metaData = other.metaData;
   FnBits = other.FnBits;
   FnBits.Declared = true;
   loc = other.loc;
}

llvm::StringRef Function::getUnmangledName() const
{
   return getName();
}

bool Function::isLambda() const
{
   return isa<Lambda>(this);
}

BasicBlock const* Function::getEntryBlock() const
{
   return &BasicBlocks.front();
}

BasicBlock* Function::getEntryBlock()
{
   return &BasicBlocks.front();
}

bool Function::isGlobalInitFn() const
{
   return this == parent->getGlobalInitFn();
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
   : Function(LambdaID, funcTy, "__anonymous_lambda", parent)
{
   setLinkage(InternalLinkage);
}

Method::Method(llvm::StringRef name,
               FunctionType *FuncTy,
               bool isStatic,
               bool isVirtual,
               Module *parent)
   : Function(MethodID, FuncTy, name, parent),
     Self(nullptr)
{
   FnBits.Static = isStatic;
   FnBits.Virtual = isVirtual;
}

Method::Method(const Method &other)
   : Function(other), Self(nullptr)
{
   id = MethodID;
}

ast::RecordDecl* Method::getRecordType() const
{
   return BasicBlocks.front().getArgs()
                     .front().getType()->stripReference()->getRecord();
}

Argument *Method::getSelf()
{
   assert(!BasicBlocks.empty());
   return &BasicBlocks.front().getArgs().front();
}

Initializer::Initializer(llvm::StringRef methodName,
                         FunctionType *FuncTy,
                         ConstructorKind Kind,
                         Module *parent)
   : Method(methodName, FuncTy, false, false, parent)
{
   id = InitializerID;
   FnBits.CtorKind = (unsigned)Kind;
}

Initializer::Initializer(const Initializer &other)
   : Method(other)
{
   id = InitializerID;
}

} // namespace il
} // namespace cdot