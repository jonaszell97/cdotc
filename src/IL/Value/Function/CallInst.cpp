//
// Created by Jonas Zell on 16.11.17.
//

#include "CallInst.h"

#include "Method.h"
#include "BasicBlock.h"
#include "../../Module/Module.h"

namespace cdot {
namespace il {

CallInst::CallInst(Function *func, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent, const std::string &name,
                   const SourceLocation &loc)
   : MultiOperandInst(CallInstID, args, *func->getReturnType(), parent,  name,
                      loc), calledFunction(func), Self(nullptr)
{
   func->addUse(this);
}

CallInst::CallInst(Method *method, Value *Self, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent, const std::string &name,
                   const SourceLocation &loc)
   : MultiOperandInst(CallInstID, args, *method->getReturnType(), parent,
                      name, loc), calledMethod(method), Self(Self)
{
   method->addUse(this);
}

CallInst::CallInst(llvm::StringRef funcName, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent, const std::string &name,
                   const SourceLocation &loc)
   : MultiOperandInst(CallInstID, args, nullptr, parent,  name, loc),
     Self(nullptr)
{
   auto Module = parent->getParent()->getParent();
   calledFunction = Module->getFunction(funcName);
   *type = *calledFunction->getReturnType();

   calledFunction->addUse(this);
}

bool CallInst::isMethodCall() const
{
   return Self != nullptr;
}

Function *CallInst::getCalledFunction() const
{
   return calledFunction;
}

llvm::ArrayRef<Value*> CallInst::getArgs()
{
   return llvm::ArrayRef<Value*>(Operands, numOperands);
}

Method *CallInst::getCalledMethod() const
{
   return calledMethod;
}

Value *CallInst::getSelf() const
{
   return Self;
}

InvokeInst::InvokeInst(Function *func, llvm::ArrayRef<Value *> args,
                       BasicBlock *NormalContinuation, BasicBlock *LandingPad,
                       BasicBlock *parent, const string &name,
                       const SourceLocation &loc)
   : CallInst(func, args, parent, name, loc),
     NormalContinuation(NormalContinuation), LandingPad(LandingPad)
{
   NormalContinuation->addPredecessor(parent);
   LandingPad->addPredecessor(parent);
   id = InvokeInstID;

   func->addUse(this);
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

InvokeInst::InvokeInst(Method *method, Value *Self,
                       llvm::ArrayRef<Value *> args,
                       BasicBlock *NormalContinuation, BasicBlock *LandingPad,
                       BasicBlock *parent, const string &name,
                       const SourceLocation &loc)
   : CallInst(method, Self, args, parent, name, loc),
     NormalContinuation(NormalContinuation), LandingPad(LandingPad)
{
   NormalContinuation->addPredecessor(parent);
   LandingPad->addPredecessor(parent);
   id = InvokeInstID;

   method->addUse(this);
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

} // namespace il
} // namespace cdot