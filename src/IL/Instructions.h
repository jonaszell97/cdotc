//
// Created by Jonas Zell on 09.04.18.
//

#ifndef CDOT_INSTRUCTIONS_H
#define CDOT_INSTRUCTIONS_H

#include "Instruction.h"

#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"

namespace cdot {

class IdentifierInfo;
enum class CastKind : unsigned char;

namespace ast {
   class UnionDecl;
   class ProtocolDecl;
   class ClassDecl;
   class EnumDecl;
   class EnumCaseDecl;
   class StructDecl;
} // namespace ast

namespace il {

class GlobalVariable;
class ConstantInt;
class Function;
class Method;

class AllocaInst: public Instruction {
   size_t allocSize = 1;
   unsigned Alignment = 0;

public:
   AllocaInst(ValueType ty,
              bool IsLet,
              BasicBlock *parent,
              unsigned alignment = 0,
              bool heap = false);

   AllocaInst(ValueType ty,
              bool IsLet,
              BasicBlock *parent,
              size_t allocSize,
              unsigned alignment = 0,
              bool heap = false);

   unsigned int getAlignment() const { return Alignment; }
   bool isHeapAlloca() const { return AllocaBits.Heap; }

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   bool canElideCopy() const { return AllocaBits.CanElideCopy; }
   void setCanElideCopy() { AllocaBits.CanElideCopy = true; }

   bool isInitializer() const { return AllocaBits.IsLocalVarDecl; }
   void setIsInitializer(bool b) { AllocaBits.IsLocalVarDecl = b; }

   size_t getAllocSize() const { return allocSize; }
   bool isLet() const { return AllocaBits.IsLet; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == AllocaInstID;
   }
};

class AllocBoxInst: public Instruction {
   Function *Deinitializer;

public:
   AllocBoxInst(ValueType ty,
                Function *Deinitializer,
                BasicBlock *parent);

   Function *getDeinitializer() const { return Deinitializer; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == AllocBoxInstID;
   }
};

class TerminatorInst: public Instruction {
public:
   TerminatorInst(TypeID id,
                  Context &Ctx,
                  BasicBlock *parent);

   BasicBlock *getSuccessorAt(size_t idx) const;
   size_t getNumSuccessors() const;

   static inline bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstTerminator && kind < _lastTerminator;
   }
};

class RetInst: public TerminatorInst {
public:
   RetInst(Value *returnedValue,
           BasicBlock *parent);

   RetInst(Context &Ctx, BasicBlock *parent);

   Value *getReturnedValue() const;
   bool isVoidReturn() const;

   bool canUseSRetValue() const { return RetBits.CanUseSRetValue; }
   void setCanUseSRetValue() { RetBits.CanUseSRetValue = true; }

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("RetInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

protected:
   Value *returnedValue;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == RetInstID;
   }
};

class ThrowInst: public TerminatorInst {
public:
   ThrowInst(Value *thrownValue,
             GlobalVariable *typeInfo,
             BasicBlock *parent);

   Value *getThrownValue() const
   {
      return thrownValue;
   }

   GlobalVariable *getTypeInfo() const
   {
      return typeInfo;
   }

   Function *getDescFn() const
   {
      return descFn;
   }

   void setDescFn(Function *descFn)
   {
      ThrowInst::descFn = descFn;
   }

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("ThrowInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

protected:
   Value *thrownValue;
   GlobalVariable *typeInfo;

   Function *descFn = nullptr;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ThrowInstID;
   }
};

class UnreachableInst: public TerminatorInst {
public:
   UnreachableInst(Context &Ctx, BasicBlock *parent);

   BasicBlock *getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("UnreachableInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnreachableInstID;
   }
};

class SwitchInst: public TerminatorInst {
public:
   typedef std::pair<ConstantInt*, BasicBlock*> CasePair;

   SwitchInst(Value *SwitchVal,
              BasicBlock *DefaultDst = nullptr,
              BasicBlock *parent = nullptr);

   Value *getSwitchVal() const;
   const llvm::SmallVector<CasePair, 4> &getCases() const;

   BasicBlock* getDefault() const { return DefaultDst; }
   void setDefault(BasicBlock *BB);

   void addCase(ConstantInt *val, BasicBlock *Dst);

   op_iterator op_begin_impl() { return &SwitchVal; }
   op_iterator op_end_impl() { return &SwitchVal + 1; }
   op_const_iterator op_begin_impl() const { return &SwitchVal; }
   op_const_iterator op_end_impl() const { return &SwitchVal + 1; }

   unsigned getNumOperandsImpl() const { return 1; }

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      if (!idx)
         return DefaultDst;

      return Cases[idx - 1].second;
   }

   size_t getNumSuccessorsImpl() const { return Cases.size() + 1; }

protected:
   Value *SwitchVal;

   BasicBlock *DefaultDst;
   llvm::SmallVector<CasePair, 4> Cases;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == SwitchInstID;
   }
};

class LandingPadInst: public TerminatorInst {
public:
   struct CatchClause {
      QualType CaughtType;
      BasicBlock *TargetBB;
   };

   typedef llvm::SmallVector<CatchClause, 2> CatchClauseList;

   LandingPadInst(Context &Ctx, BasicBlock *parent);

   const CatchClauseList &getCatchClauses() const
   {
      return CatchClauses;
   }

   void addCatch(CatchClause &&Clause);

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      return CatchClauses[idx].TargetBB;
   }

   size_t getNumSuccessorsImpl() const { return CatchClauses.size(); }

protected:
   CatchClauseList CatchClauses;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LandingPadInstID;
   }
};

class MultiOperandInst {
public:
   static bool classof(Value const *T)
   {
      switch (T->getTypeID()) {
      case Value::CallInstID:
      case Value::InvokeInstID:
         return true;
      default:
         return false;
      }
   }

   friend class Instruction;

protected:
   explicit MultiOperandInst(llvm::ArrayRef<Value*> operands)
      : numOperands((unsigned)operands.size()),
        Operands(new Value*[operands.empty() ? 1 : operands.size()])
   {

      unsigned i = 0;
      for (const auto &op : operands) {
         Operands[i] = op;
         ++i;
      }
   }

   explicit MultiOperandInst(unsigned NumOperands)
      : numOperands(NumOperands), Operands(new Value*[NumOperands])
   {}

   ~MultiOperandInst() {
      delete[] Operands;
   }

   unsigned numOperands;
   Value **Operands;
};

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
   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstCallInst && kind < _lastCallInst;
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

   llvm::ArrayRef<Value*> getArgs() const;

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

   llvm::StringRef getIntrinsicName() const
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

class BrInst: public TerminatorInst, public MultiOperandInst {
public:
   BrInst(Value *Condition,
          BasicBlock *IfBranch,
          llvm::ArrayRef<Value*> TargetArgs,
          BasicBlock *ElseBranch,
          llvm::ArrayRef<Value*> ElseArgs,
          BasicBlock *parent);

   BrInst(BasicBlock *TargetBranch,
          llvm::ArrayRef<Value*> TargetArgs,
          BasicBlock *parent);

   BrInst(Context &Ctx, BasicBlock *parent);

   BasicBlock *getTargetBranch() const { return TargetBranch; }
   void setTargetBranch(BasicBlock *TargetBranch);

   Value *getCondition() const { return Condition; }
   BasicBlock *getElseBranch() const { return ElseBranch; }

   llvm::ArrayRef<Value*> getTargetArgs() const;
   llvm::ArrayRef<Value*> getElseArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

   BasicBlock *getSuccessorAtImpl(size_t idx) const
   {
      if (idx == 0)
         return TargetBranch;

      assert(idx == 1 && ElseBranch && "invalid successor index");
      return ElseBranch;
   }

   size_t getNumSuccessorsImpl() const { return Condition ? 2 : 1; }

protected:
   Value *Condition;
   BasicBlock *TargetBranch;
   BasicBlock *ElseBranch;

   size_t numTargetArgs;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BrInstID;
   }
};

class OperatorInst: public Instruction {
protected:
   OperatorInst(TypeID id,
                Context &Ctx,
                QualType resultType,
                BasicBlock *parent)
      : Instruction(id, ValueType(Ctx, resultType), parent)
   {

   }

public:
   static bool classof(OperatorInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
#     define CDOT_UNARY_INST(Name)  \
         case Name##ID:
#     define CDOT_BINARY_INST(Name) \
         case Name##ID:
#     define CDOT_CAST_INST(Name)   \
         case Name##ID:
#     define CDOT_COMP_INST(Name)   \
         case Name##ID:
#     include "Instructions.def"

      case TypeID::LoadInstID:
      case TypeID::FieldRefInstID:
      case TypeID::EnumExtractInstID:
      case TypeID::EnumRawValueInstID:
      case TypeID::CaptureExtractInstID:
      case TypeID::PtrToLvalueInstID:
      case TypeID::AddrOfInstID:
      case TypeID::StoreInstID:
      case TypeID::GEPInstID:
      case TypeID::TupleExtractInstID:
         return true;
      default:
         return false;
      }
   }
};

class BinaryInstruction: public OperatorInst {
public:
   BinaryInstruction(TypeID id,
                     Value *lhs,
                     Value *rhs,
                     QualType resultType,
                     BasicBlock *parent);

   friend class Instruction;

private:
   unsigned getNumOperandsImpl() const { return 2; }

   Value *getOperandImpl(unsigned idx)
   {
      assert(idx < 2);
      return Operands[idx];
   }

   void setOperandImpl(unsigned idx, Value *V)
   {
      assert(idx < 2);
      Operands[idx] = V;
   }

   op_iterator op_begin_impl() { return Operands; }
   op_iterator op_end_impl()   { return Operands + 2; }

   op_const_iterator op_begin_impl() const { return Operands; }
   op_const_iterator op_end_impl()   const { return Operands + 2; }

protected:
   Value *Operands[2];

public:
   static bool classof(Value const *T)
   {
      switch(T->getTypeID()) {
      case TypeID::BinaryOperatorInstID:
      case TypeID::CompInstID:
      case TypeID::StoreInstID:
      case TypeID::GEPInstID:
      case TypeID::TupleExtractInstID:
         return true;
      default:
         return false;
      }
   }
};

class UnaryInstruction: public OperatorInst {
public:
   UnaryInstruction(TypeID id,
                    Value *operand,
                    QualType resultType,
                    BasicBlock *parent);

   friend class Instruction;

private:
   unsigned getNumOperandsImpl() const { return 1; }

   Value *getOperandImpl(unsigned idx)
   {
      assert(idx == 0);
      return Operand;
   }

   void setOperandImpl(unsigned idx, Value *V)
   {
      assert(idx == 0);
      Operand = V;
   }

   op_iterator op_begin_impl() { return &Operand; }
   op_iterator op_end_impl()   { return &Operand + 1; }

   op_const_iterator op_begin_impl() const { return &Operand; }
   op_const_iterator op_end_impl()   const { return &Operand + 1; }

protected:
   Value *Operand;

public:
   static bool classof(Value const *T)
   {
      switch (T->getTypeID()) {
#     define CDOT_UNARY_INST(Name) \
      case Name##ID:
#     define CDOT_CAST_INST(Name) \
      case Name##ID:

#     include "Instructions.def"

      case TypeID::LoadInstID:
      case TypeID::FieldRefInstID:
      case TypeID::EnumExtractInstID:
      case TypeID::EnumRawValueInstID:
      case TypeID::CaptureExtractInstID:
      case TypeID::PtrToLvalueInstID:
      case TypeID::AddrOfInstID:
         return true;
      default:
         return false;
      }
   }
};

class BinaryOperatorInst: public BinaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_BINARY_OP(Name, OP)               \
      Name,
#  include "Instructions.def"
   };

   BinaryOperatorInst(OpCode opCode,
                      Value *lhs, Value *rhs,
                      BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getLhs() const { return getOperand(0); }
   il::Value *getRhs() const { return getOperand(1); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == BinaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class UnaryOperatorInst: public UnaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_UNARY_OP(Name, OP)               \
      Name,
#  include "Instructions.def"
   };

   UnaryOperatorInst(OpCode opCode,
                     Value *target,
                     BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getTarget() const { return getOperand(0); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == UnaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class CompInst: public BinaryInstruction {
public:
   enum OpCode: unsigned char {
#  define CDOT_COMP_OP(Name, OP)               \
      Name,
#  include "Instructions.def"
   };

   CompInst(OpCode opCode,
            Value *lhs, Value *rhs,
            BasicBlock *parent);

   OpCode getOpCode() const { return opCode; }
   il::Value *getLhs() const { return getOperand(0); }
   il::Value *getRhs() const { return getOperand(1); }

   static bool classof(Value const *V)
   {
      return V->getTypeID() == CompInstID;
   }

private:
   OpCode opCode;
};

class FieldRefInst: public UnaryInstruction {
public:
   FieldRefInst(Value *val,
                DeclarationName fieldName,
                bool IsLet,
                BasicBlock *parent);

   DeclarationName getFieldName() const { return fieldName; }
   ast::StructDecl *getAccessedType() const;

   bool isLet() const { return FieldRefBits.IsLet; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == FieldRefInstID;
   }

private:
   DeclarationName fieldName;
};

class GEPInst: public BinaryInstruction {
public:
   GEPInst(Value *val,
           size_t idx,
           bool IsLet,
           BasicBlock *parent);

   GEPInst(Value *val,
           Value *idx,
           bool IsLet,
           BasicBlock *parent);

   Value *getIndex() const  { return Operands[1]; }
   Value *getVal() const    { return Operands[0]; }

   bool isLet() const { return GEPBits.IsLet; }

   static bool classof(Value const* T)
   {
      switch(T->getTypeID()) {
      case GEPInstID:
      case TupleExtractInstID:
         return true;
      default:
         return false;
      }
   }
};

class TupleExtractInst: public GEPInst {
public:
   TupleExtractInst(Value *val,
                    ConstantInt *idx,
                    bool IsLet,
                    BasicBlock *parent);

   ConstantInt *getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == TupleExtractInstID;
   }
};

class EnumRawValueInst: public UnaryInstruction {
public:
   EnumRawValueInst(Value *Val,
                    BasicBlock *parent);

   Value *getValue() { return Operand; }
   Value const* getValue() const { return Operand; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumRawValueInstID;
   }
};

class EnumExtractInst: public UnaryInstruction {
public:
   EnumExtractInst(Value *Val,
                   ast::EnumCaseDecl *Case,
                   ConstantInt *caseVal,
                   bool IsLet,
                   BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumExtractInstID;
   }

   DeclarationName getCaseName() const;
   ast::EnumCaseDecl *getCase() const { return Case; }

   bool isIndirect() const { return EnumExtractBits.IsIndirect; }
   void setIndirect(bool indirect) { EnumExtractBits.IsIndirect = indirect; }

   ConstantInt *getCaseVal() const { return caseVal; }
   ast::EnumDecl *getEnumTy() const;

   bool isLet() const { return EnumExtractBits.IsLet; }

private:
   ast::EnumCaseDecl *Case;
   ConstantInt *caseVal;
};

class CaptureExtractInst: public UnaryInstruction {
public:
   CaptureExtractInst(ConstantInt *idx, BasicBlock *parent);
   ConstantInt *getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == CaptureExtractInstID;
   }
};

class LandingPadInst;

static const unsigned short FirstCast = Value::BitCastInstID;
extern const char* CastNames[];

class CastInst: public UnaryInstruction {
public:
   CastInst(TypeID id,
            Value *target,
            QualType toType,
            BasicBlock *parent)
      : UnaryInstruction(id, target, toType, parent)
   {

   }

public:
   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstCastInst && kind < _lastCastInst;
   }
};

class IntegerCastInst: public CastInst {
public:
   IntegerCastInst(CastKind kind,
                   Value *target,
                   QualType toType,
                   BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntegerCastInstID;
   }
};

class IntToEnumInst: public CastInst {
public:
   IntToEnumInst(Value *target,
                 QualType toType,
                 BasicBlock *parent)
      : CastInst(IntToEnumInstID, target, toType, parent)
   {}

   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntToEnumInstID;
   }
};

class FPCastInst: public CastInst {
public:
   FPCastInst(CastKind kind,
              Value *target,
              QualType toType,
              BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == FPCastInstID;
   }
};

class UnionCastInst: public CastInst {
public:
   UnionCastInst(Value *target,
                 ast::UnionDecl *UnionTy,
                 const IdentifierInfo *fieldName,
                 BasicBlock *parent);

   ast::UnionDecl *getUnionTy() const { return UnionTy; }
   llvm::StringRef getFieldName() const { return fieldName->getIdentifier(); }

protected:
   ast::UnionDecl *UnionTy;
   const IdentifierInfo *fieldName;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionCastInstID;
   }
};

class ProtoCastInst: public CastInst {
public:
   ProtoCastInst(Value *target,
                 QualType toType,
                 BasicBlock *parent);

   bool isWrap() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ProtoCastInstID;
   }
};

class ExceptionCastInst: public CastInst {
public:
   ExceptionCastInst(Value *target,
                     QualType toType,
                     BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExceptionCastInstID;
   }
};

class BitCastInst: public CastInst {
public:
   BitCastInst(CastKind kind,
               Value *target,
               QualType toType,
               BasicBlock *parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BitCastInstID;
   }
};

class DynamicCastInst: public CastInst {
   ast::ClassDecl *TargetTy;

public:
   DynamicCastInst(Value *target,
                   ast::ClassDecl *TargetTy,
                   QualType Type,
                   BasicBlock *parent);

   ast::ClassDecl *getTargetType() const { return TargetTy; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DynamicCastInstID;
   }
};

class InitInst: public CallInst {
public:
   InitInst(ast::StructDecl *InitializedType,
            Method *Init,
            llvm::ArrayRef<Value *> args,
            BasicBlock *parent);

   Method *getInit() const { return calledMethod; }
   ast::StructDecl *getInitializedType() const { return InitializedType; }

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

protected:
   ast::StructDecl *InitializedType;

public:
   static bool classof(InitInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
      case InitInstID:
         return true;
      default:
         return false;
      }
   }
};

class UnionInitInst: public CallInst {
public:
   UnionInitInst(ast::UnionDecl *UnionTy,
                 Value *InitializerVal,
                 BasicBlock *parent);

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   ast::UnionDecl *getUnionTy() const { return UnionTy; }
   Value *getInitializerVal() const { return Operands[0]; }

protected:
   ast::UnionDecl *UnionTy;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionInitInstID;
   }
};

class EnumInitInst: public CallInst {
public:
   EnumInitInst(Context &Ctx,
                ast::EnumDecl *EnumTy,
                ast::EnumCaseDecl *Case,
                llvm::ArrayRef<Value *> args,
                BasicBlock *parent);

   bool canUseSRetValue() const { return EnumInitBits.CanUseSRetValue; }
   void setCanUseSRetValue() { EnumInitBits.CanUseSRetValue = true; }

   bool isIndirect() const { return EnumInitBits.IsIndirect; }
   void setIndirect(bool indirect) { EnumInitBits.IsIndirect = indirect; }

   ast::EnumDecl *getEnumTy() const { return EnumTy; }
   ast::EnumCaseDecl *getCase() const { return Case; }
   DeclarationName getCaseName() const;

protected:
   ast::EnumDecl *EnumTy;
   ast::EnumCaseDecl *Case;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumInitInstID;
   }
};

class LambdaInitInst: public Instruction, public MultiOperandInst {
public:
   LambdaInitInst(Function *Function,
                  QualType LambdaTy,
                  llvm::ArrayRef<Value*> Captures,
                  BasicBlock *parent);

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   Function *getFunction() const { return F; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

protected:
   Function *F;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaInitInstID;
   }
};

class DeallocInst: public Instruction {
   Value *V;

public:
   DeallocInst(Value *V,
               bool Heap,
               BasicBlock *P);

   Value *getValue() const { return V; }

   bool isHeap() const { return AllocaBits.Heap; }
   void setHeap(bool H) { AllocaBits.Heap = H; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeallocInstID;
   }
};

class DeallocBoxInst: public Instruction {
   Value *V;

public:
   DeallocBoxInst(Value *V, BasicBlock *P);

   Value *getValue() const { return V; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeallocBoxInstID;
   }
};

class DeinitializeLocalInst: public Instruction {
public:
   DeinitializeLocalInst(Value *RefcountedVal, BasicBlock *Parent);
   DeinitializeLocalInst(Function *DeinitFn,
                         Value *ValueToDeinit,
                         BasicBlock *Parent);

   Value *getVal() const { return Val; }
   Function *getDeinitializer() const { return Deinitializer; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeinitializeLocalInstID;
   }

private:
   Value *Val;
   Function *Deinitializer;
};

class DeinitializeTemporaryInst: public Instruction {
public:
   DeinitializeTemporaryInst(Value *RefcountedVal, BasicBlock *Parent);
   DeinitializeTemporaryInst(Function *DeinitFn,
                             Value *ValueToDeinit,
                             BasicBlock *Parent);

   Value *getVal() const { return Val; }
   Function *getDeinitializer() const { return Deinitializer; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeinitializeTemporaryInstID;
   }

private:
   Value *Val;
   Function *Deinitializer;
};

class StoreInst: public BinaryInstruction {
public:
   StoreInst(Value *dst,
             Value *src,
             bool IsInit,
             BasicBlock *parent);

   bool useMemCpy() const;

   Value *getDst() const { return Operands[0]; }
   Value *getSrc() const { return Operands[1]; }

   bool isInit() const { return StoreBits.IsInit; }
   void setIsInit(bool I) { StoreBits.IsInit = I; }

   op_iterator op_begin_impl() { return Operands; }
   op_iterator op_end_impl()   { return Operands + 2; }

   op_const_iterator op_begin_impl() const { return Operands; }
   op_const_iterator op_end_impl()   const { return Operands + 2; }

   unsigned getNumOperandsImpl() const { return 2; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == StoreInstID;
   }
};

class LoadInst: public UnaryInstruction {
public:
   explicit LoadInst(Value *target,
                     BasicBlock *parent);

   op_iterator op_begin_impl() { return &Operand; }
   op_iterator op_end_impl()   { return &Operand + 1; }

   op_const_iterator op_begin_impl() const { return &Operand; }
   op_const_iterator op_end_impl()   const { return &Operand + 1; }

   Value *getTarget() const { return Operand; }

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LoadInstID;
   }
};

class AddrOfInst: public UnaryInstruction {
public:
   AddrOfInst(Value *target,
              BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == AddrOfInstID;
   }
};

class PtrToLvalueInst: public UnaryInstruction {
public:
   PtrToLvalueInst(Value *target, BasicBlock *parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == PtrToLvalueInstID;
   }
};

class DebugLocInst: public Instruction {
   SourceLocation Loc;
   unsigned Line;
   unsigned Col;
   unsigned FileID;

public:
   DebugLocInst(SourceLocation Loc,
                unsigned Line,
                unsigned Col,
                unsigned FileID,
                ValueType VoidTy,
                BasicBlock *Parent);

   SourceLocation getLoc() const { return Loc; }
   unsigned int getLine() const { return Line; }
   unsigned int getCol() const { return Col; }
   unsigned int getFileID() const { return FileID; }

   static bool classof(const Value *V)
   {
      return V->getTypeID() == DebugLocInstID;
   }
};

class DebugLocalInst: public Instruction {
   const IdentifierInfo *Name;
   Value *Val;

public:
   DebugLocalInst(const IdentifierInfo *Name, Value *V, BasicBlock *Parent);

   const IdentifierInfo *getName() const { return Name; }
   Value *getVal() const { return Val; }

   static bool classof(const Value *V)
   {
      return V->getTypeID() == DebugLocalInstID;
   }
};

} // namespace il
} // namespace cdot

#endif //CDOT_INSTRUCTIONS_H
