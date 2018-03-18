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

class CallSite;
class ImmutableCallSite;

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

   CallSite getAsCallSite();
   ImmutableCallSite getAsImmutableCallSite() const;

protected:
   CallInst(TypeID id,
            Context &Ctx,
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

   CallSite getAsCallSite();
   ImmutableCallSite getAsImmutableCallSite() const;

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      if (!idx)
         return NormalContinuation;

      assert(idx == 1 && "invalid idx");
      return LandingPad;
   }

   size_t getNumSuccessorsImpl() const { return 2; }

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
                     ValueType returnType,
                     llvm::ArrayRef<Value*> args,
                     BasicBlock *parent);

   llvm::ArrayRef<Value*> getArgs() const { return { Operands, numOperands }; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }
   Intrinsic getCalledIntrinsic() const { return calledIntrinsic; }

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

template<class CallInstTy, class InvokeInstTy, class InstTy>
class CallSiteBase {
protected:
   explicit CallSiteBase(CallInstTy *C)
      : Val(C, true)
   {}

   explicit CallSiteBase(InvokeInstTy *I)
      : Val(I, false)
   {}

public:
   operator bool() const
   {
      return Val.getPointer() != nullptr;
   }

   bool isCallInst() const
   {
      return Val.getInt();
   }

   CallInstTy const* getAsCallInst() const
   {
      assert(isCallInst() && "not a CallInst");
      return reinterpret_cast<CallInstTy*>(Val.getPointer());
   }

   InvokeInstTy const* getAsInvokeInst() const
   {
      assert(!isCallInst() && "not an InvokeInst");
      return reinterpret_cast<InvokeInstTy*>(Val.getPointer());
   }

   CallInstTy* getAsCallInst()
   {
      assert(isCallInst() && "not a CallInst");
      return reinterpret_cast<CallInstTy*>(Val.getPointer());
   }

   InvokeInstTy* getAsInvokeInst()
   {
      assert(!isCallInst() && "not an InvokeInst");
      return reinterpret_cast<InvokeInstTy*>(Val.getPointer());
   }

   using op_iterator = CallInst::op_iterator;
   using op_const_iterator = CallInst::op_const_iterator;

#  define DISPATCH_CALLSITE_VALUE(METHOD)  \
   isCallInst() ? getAsCallInst()->METHOD : getAsInvokeInst()->METHOD

#  define DISPATCH_CALLSITE_VOID(METHOD)   \
   if (isCallInst()) { getAsCallInst()->METHOD } \
   else { getAsInvokeInst()->METHOD }

   op_iterator op_begin()
   {
      return DISPATCH_CALLSITE_VALUE(op_begin());
   }

   op_iterator op_end()
   {
      return DISPATCH_CALLSITE_VALUE(op_end());
   }

   op_const_iterator op_begin() const
   {
      return DISPATCH_CALLSITE_VALUE(op_begin());
   }

   op_const_iterator op_end() const
   {
      return DISPATCH_CALLSITE_VALUE(op_end());
   }

   Function *getCalledFunction() const
   {
      return DISPATCH_CALLSITE_VALUE(getCalledFunction());
   }

   Value::TypeID getTypeID() const
   {
      return DISPATCH_CALLSITE_VALUE(getTypeID());
   }

   QualType getType() const
   {
      return DISPATCH_CALLSITE_VALUE(getType());
   }

   llvm::ArrayRef<Value*> getArgs() const
   {
      return DISPATCH_CALLSITE_VALUE(getArgs());
   }

#  undef DISPATCH_CALLSITE_VALUE
#  undef DISPATCH_CALLSITE_VOID

private:
   llvm::PointerIntPair<InstTy*, 1, bool> Val;
};

class CallSite : CallSiteBase<CallInst, InvokeInst, Instruction> {
public:
   explicit CallSite(CallInst* C) : CallSiteBase(C) {}
   explicit CallSite(InvokeInst* C) : CallSiteBase(C) {}
   explicit CallSite() : CallSiteBase((CallInst*)nullptr) {}
};

class ImmutableCallSite:
   CallSiteBase<CallInst const, InvokeInst const, Instruction const> {
public:
   explicit ImmutableCallSite(CallInst const* C) : CallSiteBase(C) {}
   explicit ImmutableCallSite(InvokeInst const* C) : CallSiteBase(C) {}
   explicit ImmutableCallSite() : CallSiteBase((CallInst*)nullptr) {}
};

inline CallSite CallInst::getAsCallSite()
{
   return CallSite(this);
}

inline ImmutableCallSite CallInst::getAsImmutableCallSite() const
{
   return ImmutableCallSite(this);
}

inline CallSite InvokeInst::getAsCallSite()
{
   return CallSite(this);
}

inline ImmutableCallSite InvokeInst::getAsImmutableCallSite() const
{
   return ImmutableCallSite(this);
}

} // namespace il
} // namespace cdot


#endif //CDOT_CALLINST_H
