//
// Created by Jonas Zell on 16.11.17.
//

#include "CallInst.h"

#include "../Function/Method.h"
#include "../Function/BasicBlock.h"
#include "../../Module/Module.h"

#include "../Constant/ConstantVal.h"

#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/IntegerType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

CallInst::CallInst(Function *func, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : Instruction(CallInstID, func->getReturnType(), parent),
     MultiOperandInst(args),
     calledFunction(func)
{
   func->addUse(this);
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

CallInst::CallInst(TypeID id, llvm::ArrayRef<Value *> args, BasicBlock *parent)
   : Instruction(id, nullptr, parent), MultiOperandInst(args)
{
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

CallInst::CallInst(TypeID id, Function *func, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : CallInst(func, args, parent)
{
   this->id = id;
}

CallInst::CallInst(TypeID id, Value *func, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : Instruction(id, cast<FunctionType>(*func->getType())->getReturnType(),
                 parent),
     MultiOperandInst(args),
     indirectFunction(func)
{

}

llvm::ArrayRef<Value*> CallInst::getArgs() const
{
   return { Operands, numOperands };
}

IndirectCallInst::IndirectCallInst(Value *Func, llvm::ArrayRef<Value *> args,
                                   BasicBlock *parent)
   : CallInst(IndirectCallInstID, Func, args, parent)
{

}

LambdaCallInst::LambdaCallInst(Value *lambda, llvm::ArrayRef<Value *> args,
                               BasicBlock *parent)
   : CallInst(LambdaCallInstID, lambda, args, parent)
{

}

ProtocolCallInst::ProtocolCallInst(Method *M,
                                   llvm::ArrayRef<Value *> args,
                                   BasicBlock *parent)
   : CallInst(ProtocolCallInstID, M, args, parent)
{

}

VirtualCallInst::VirtualCallInst(Method *M,
                                 llvm::ArrayRef<Value *> args,
                                 BasicBlock *parent)
   : CallInst(VirtualCallInstID, M, args, parent)
{

}

InvokeInst::InvokeInst(Function *func, llvm::ArrayRef<Value *> args,
                       BasicBlock *NormalContinuation, BasicBlock *LandingPad,
                       BasicBlock *parent)
   : TerminatorInst(InvokeInstID, parent),
     MultiOperandInst(args),
     calledFunction(func), NormalContinuation(NormalContinuation),
     LandingPad(LandingPad)
{
   if (isa<Method>(func)) {
      SubclassData |= Flag::MethodCall;
   }

   NormalContinuation->addPredecessor(parent);
   LandingPad->addPredecessor(parent);

   func->addUse(this);
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

InvokeInst::InvokeInst(TypeID id,
                       Function *F,
                       llvm::ArrayRef<Value *> args,
                       BasicBlock *NormalContinuation,
                       BasicBlock *LandingPad,
                       BasicBlock *parent)
   : InvokeInst(F, args, NormalContinuation, LandingPad, parent)
{
   this->id = id;
}

bool InvokeInst::isMethodCall() const
{
   return (SubclassData & Flag::MethodCall) != 0;
}

llvm::ArrayRef<Value*> InvokeInst::getArgs() const
{
   return { Operands, numOperands };
}

llvm::ArrayRef<Value*> InvokeInst::getArgsWithoutSelf() const
{
   if (isMethodCall()) {
      return { Operands + 1, numOperands - 1 };
   }

   return { Operands, numOperands };
}

ProtocolInvokeInst::ProtocolInvokeInst(Method *M,
                                       llvm::ArrayRef<Value *> args,
                                       BasicBlock *NormalContinuation,
                                       BasicBlock *LandingPad,
                                       BasicBlock *parent)
   : InvokeInst(ProtocolInvokeInstID, M, args,
                NormalContinuation, LandingPad, parent)
{

}

VirtualInvokeInst::VirtualInvokeInst(Method *M,
                                     llvm::ArrayRef<Value *> args,
                                     BasicBlock *NormalContinuation,
                                     BasicBlock *LandingPad, BasicBlock *parent)
   : InvokeInst(VirtualInvokeInstID, M, args,
                NormalContinuation, LandingPad, parent)
{

}

IntrinsicCallInst::IntrinsicCallInst(Intrinsic id,
                                     llvm::ArrayRef<Value *> args,
                                     BasicBlock *parent)
   : Instruction(IntrinsicCallInstID, getIntrinsicReturnType(id), parent),
     MultiOperandInst(args),
     calledIntrinsic(id)
{

}

Type* IntrinsicCallInst::getIntrinsicReturnType(Intrinsic id)
{
   switch (id) {
      case Intrinsic::MemCpy:
      case Intrinsic::MemSet:
      case Intrinsic::LifetimeBegin:
      case Intrinsic::LifetimeEnd:
      case Intrinsic::Retain:
      case Intrinsic::Release:
         return VoidType::get();
      case Intrinsic::MemCmp:
         return IntegerType::getBoolTy();
      case Intrinsic::__ctfe_stacktrace:
         return VoidType::get();
   }
}

} // namespace il
} // namespace cdot