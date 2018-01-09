//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CALLINST_H
#define CDOT_CALLINST_H

#include <llvm/ADT/ArrayRef.h>

#include "MultiOperandInst.h"
#include "Terminator/TerminatorInst.h"

namespace cdot {
namespace il {

class Function;
class Method;
class ProtocolType;
class ClassType;

class CallInst: public Instruction, public MultiOperandInst {
public:
   CallInst(Function *func,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent);

   Function *getCalledFunction() const { return calledFunction; }
   Method *getCalledMethod() const { return calledMethod; }
   Value *getSelf() const { return Operands[0]; }

   llvm::ArrayRef<Value*> getArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

protected:
   CallInst(TypeID id,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent);

   CallInst(TypeID id,
            Function *func,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent);

   CallInst(TypeID id,
            Value *func,
            llvm::ArrayRef<Value*> args,
            BasicBlock *parent);

   union {
      Function *calledFunction;
      Method *calledMethod;
      Value *indirectFunction;
   };

public:
   static bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_CALL_INST(Name) \
         case Name##ID:           \
            return true;
#     include "../Instructions.def"
         default:
            return false;
      }
   }
};

class IndirectCallInst: public CallInst {
public:
   IndirectCallInst(Value *Func,
                    llvm::ArrayRef<Value*> args,
                    BasicBlock *parent);

   Value const *getCalledFunction() const { return indirectFunction; }

   static bool classof(Value const *T)
   {
      return T->getTypeID() == IndirectCallInstID;
   }
};

class LambdaCallInst: public CallInst {
public:
   LambdaCallInst(Value *lambda,
                  llvm::ArrayRef<Value*> args,
                  BasicBlock *parent);

   Value const* getLambda() const { return indirectFunction; }

   static bool classof(Value const *T)
   {
      return T->getTypeID() == LambdaCallInstID;
   }
};

class ProtocolCallInst: public CallInst {
public:
   ProtocolCallInst(Method *M,
                    llvm::ArrayRef<Value *> args,
                    BasicBlock *parent);

   static bool classof(Value const *T)
   {
      return T->getTypeID() == ProtocolCallInstID;
   }
};

class VirtualCallInst: public CallInst {
public:
   VirtualCallInst(Method *M,
                   llvm::ArrayRef<Value *> args,
                   BasicBlock *parent);

public:
   static bool classof(Value const *T)
   {
      return T->getTypeID() == VirtualCallInstID;
   }
};

class InvokeInst: public TerminatorInst, public MultiOperandInst {
public:
   InvokeInst(Function *func,
              llvm::ArrayRef<Value*> args,
              BasicBlock *NormalContinuation,
              BasicBlock *LandingPad,
              BasicBlock *parent);

   BasicBlock *getNormalContinuation() const { return NormalContinuation; }
   BasicBlock *getLandingPad() const { return LandingPad; }
   Function *getCalledFunction() const { return calledFunction; }
   Method *getCalledMethod() const { return calledMethod; }
   Value *getSelf() const { return Operands[0]; }

   bool isMethodCall() const;
   llvm::ArrayRef<Value*> getArgs() const;
   llvm::ArrayRef<Value*> getArgsWithoutSelf() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

protected:
   InvokeInst(TypeID id,
              Function *F,
              llvm::ArrayRef<Value*> args,
              BasicBlock *NormalContinuation,
              BasicBlock *LandingPad,
              BasicBlock *parent);

   union {
      Function *calledFunction;
      Method *calledMethod;
   };

   enum Flag {
      MethodCall = 1
   };

   BasicBlock *NormalContinuation;
   BasicBlock *LandingPad;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == InvokeInstID;
   }
};

class ProtocolInvokeInst: public InvokeInst {
public:
   ProtocolInvokeInst(Method *M,
                      llvm::ArrayRef<Value *> args,
                      BasicBlock *NormalContinuation,
                      BasicBlock *LandingPad,
                      BasicBlock *parent);

public:
   static bool classof(Value const *T)
   {
      return T->getTypeID() == ProtocolInvokeInstID;
   }
};

class VirtualInvokeInst: public InvokeInst {
public:
   VirtualInvokeInst(Method *M,
                     llvm::ArrayRef<Value *> args,
                     BasicBlock *NormalContinuation,
                     BasicBlock *LandingPad,
                     BasicBlock *parent);

   static bool classof(Value const *T)
   {
      return T->getTypeID() == VirtualInvokeInstID;
   }
};

enum class Intrinsic : unsigned char {
#define CDOT_INTRINSIC(Name, Spelling) \
   Name,
#include "Intrinsics.def"
};

class IntrinsicCallInst: public Instruction, public MultiOperandInst {
public:
   IntrinsicCallInst(Intrinsic id,
                     llvm::ArrayRef<Value*> args,
                     BasicBlock *parent);

   llvm::ArrayRef<Value*> getArgs() const { return { Operands, numOperands }; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }
   Intrinsic getCalledIntrinsic() const { return calledIntrinsic; }

   static Type *getIntrinsicReturnType(Intrinsic id);

   std::string getIntrinsicName() const
   {
      switch (calledIntrinsic) {
#     define CDOT_INTRINSIC(Name, Spelling) \
         case Intrinsic::Name:              \
            return #Name;
#     include "Intrinsics.def"
      }
   }

protected:
   Intrinsic calledIntrinsic;

public:
   static bool classof(Value const* T) {
      return T->getTypeID() == IntrinsicCallInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CALLINST_H
