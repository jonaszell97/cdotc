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
   : Instruction(CallInstID, *func->getReturnType(), parent,  name, loc),
     args(args.begin(), args.end()), calledFunction(func), Self(nullptr)
{
   func->addUse();
}

CallInst::CallInst(Method *method, Value *Self, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent, const std::string &name,
                   const SourceLocation &loc)
   : Instruction(CallInstID, *method->getReturnType(), parent,  name, loc),
     args(args.begin(), args.end()), calledMethod(method), Self(Self)
{
   method->addUse();
}

CallInst::CallInst(llvm::StringRef funcName, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent, const std::string &name,
                   const SourceLocation &loc)
   : Instruction(CallInstID, nullptr, parent,  name, loc),
     args(args.begin(), args.end()), Self(nullptr)
{
   auto Module = parent->getParent()->getParent();
   calledFunction = Module->getFunction(funcName);
   *type = *calledFunction->getReturnType();
}

bool CallInst::isMethodCall() const
{
   return Self != nullptr;
}

Function *CallInst::getCalledFunction() const
{
   return calledFunction;
}

const CallInst::ArgList &CallInst::getArgs() const
{
   return args;
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
}

} // namespace il
} // namespace cdot