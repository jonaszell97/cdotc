#ifndef CDOT_INSTRUCTIONS_H
#define CDOT_INSTRUCTIONS_H

#include "cdotc/IL/Instruction.h"

#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Basic/Precedence.h"

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

class MultiOperandInst {
public:
   static bool classof(Value const* T) { return classofKind(T->getTypeID()); }

   static bool classofKind(Value::TypeID ID)
   {
      switch (ID) {
#define CDOT_MULTI_OP_INST(NAME)                                               \
   case Value::NAME##ID:                                                       \
      return true;
#include "cdotc/IL/Instructions.def"

      default:
         return false;
      }
   }

   unsigned int getMultiNumOperands() const { return numOperands; }

   llvm::ArrayRef<Value*> getMultiOperands() const
   {
      return {Operands, numOperands};
   }

   friend class Instruction;

protected:
   explicit MultiOperandInst(llvm::ArrayRef<Value*> operands)
       : numOperands((unsigned)operands.size()),
         Operands(new Value*[operands.empty() ? 1 : operands.size()])
   {

      unsigned i = 0;
      for (const auto& op : operands) {
         Operands[i] = op;
         ++i;
      }
   }

   MultiOperandInst(llvm::ArrayRef<Value*> operands, unsigned numOperands)
       : numOperands(numOperands), Operands(new Value*[numOperands])
   {
      assert(numOperands >= operands.size());

      unsigned i = 0;
      for (const auto& op : operands) {
         Operands[i] = op;
         ++i;
      }
   }

   explicit MultiOperandInst(unsigned NumOperands)
       : numOperands(NumOperands), Operands(new Value*[NumOperands])
   {
   }

   ~MultiOperandInst() { delete[] Operands; }

   unsigned numOperands;
   Value** Operands;
};

class AllocaInst : public Instruction {
   Value* allocSize;
   unsigned Alignment = 0;

public:
   AllocaInst(ValueType ty, bool IsLet, BasicBlock* parent,
              unsigned alignment = 0, bool heap = false);

   AllocaInst(ValueType ty, bool IsLet, BasicBlock* parent,
              Value* allocSize = nullptr, unsigned alignment = 0,
              bool heap = false);

   unsigned int getAlignment() const { return Alignment; }
   bool isHeapAlloca() const { return AllocaBits.Heap; }

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   bool canElideCopy() const { return AllocaBits.CanElideCopy; }
   void setCanElideCopy() { AllocaBits.CanElideCopy = true; }

   bool isInitializer() const { return AllocaBits.IsLocalVarDecl; }
   void setIsInitializer(bool b) { AllocaBits.IsLocalVarDecl = b; }

   Value* getAllocSize() const { return allocSize; }
   bool isLet() const { return AllocaBits.IsLet; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == AllocaInstID;
   }
};

class AllocBoxInst : public Instruction {
   Function* Deinitializer;

public:
   AllocBoxInst(ValueType ty, Function* Deinitializer, BasicBlock* parent);

   Function* getDeinitializer() const { return Deinitializer; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == AllocBoxInstID;
   }
};

class TerminatorInst : public Instruction {
public:
   TerminatorInst(TypeID id, Context& Ctx, BasicBlock* parent);

   BasicBlock* getSuccessorAt(size_t idx) const;
   size_t getNumSuccessors() const;

   static inline bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstTerminator && kind < _lastTerminator;
   }
};

class RetInst : public TerminatorInst {
public:
   RetInst(Value* returnedValue, BasicBlock* parent);

   RetInst(Context& Ctx, BasicBlock* parent);

   Value* getReturnedValue() const { return returnedValue; }
   bool isVoidReturn() const { return !returnedValue; }

   bool canUseSRetValue() const { return RetBits.CanUseSRetValue; }
   void setCanUseSRetValue() { RetBits.CanUseSRetValue = true; }

   BasicBlock* getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("RetInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

   bool IsFallibleInitNoneRet() const { return RetBits.IsFallibleInitNoneRet; }
   void setIsFallibleInitNoneRet(bool B) { RetBits.IsFallibleInitNoneRet = B; }

   op_iterator op_begin_impl() { return &returnedValue; }
   op_const_iterator op_begin_impl() const { return &returnedValue; }
   unsigned getNumOperandsImpl() const { return returnedValue ? 1 : 0; }

protected:
   Value* returnedValue;

public:
   static bool classof(Value const* T) { return T->getTypeID() == RetInstID; }
};

class YieldInst : public TerminatorInst, public MultiOperandInst {
public:
   YieldInst(Value* yieldedValue, BasicBlock* ResumeDst,
             ArrayRef<Value*> ResumeArgs, bool FinalYield, BasicBlock* parent);

   YieldInst(Context& Ctx, BasicBlock* ResumeDst, ArrayRef<Value*> ResumeArgs,
             bool FinalYield, BasicBlock* parent);

   Value* getYieldedValue() const { return Operands[numOperands - 2]; }
   bool isVoidYield() const { return !getYieldedValue(); }

   ArrayRef<Value*> getResumeArgs() const
   {
      return {Operands, numOperands - 2};
   }

   BasicBlock* getResumeDst() const;

   bool isFinalYield() const { return YieldBits.IsFinalYield; }

   BasicBlock* getSuccessorAtImpl(size_t i) const
   {
      assert(i == 0);
      return getResumeDst();
   }

   size_t getNumSuccessorsImpl() const { return getResumeDst() ? 1 : 0; }

public:
   static bool classof(Value const* T) { return T->getTypeID() == YieldInstID; }
};

class ThrowInst : public TerminatorInst {
public:
   ThrowInst(Value* thrownValue, GlobalVariable* typeInfo, BasicBlock* parent);

   Value* getThrownValue() const { return thrownValue; }

   GlobalVariable* getTypeInfo() const { return typeInfo; }

   Function* getCleanupFn() const { return cleanupFn; }
   void setCleanupFn(Function* descFn) { ThrowInst::cleanupFn = descFn; }

   BasicBlock* getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("ThrowInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

   op_iterator op_begin_impl() { return &thrownValue; }
   op_iterator op_end_impl() { return &thrownValue + 2; }
   op_const_iterator op_begin_impl() const { return &thrownValue; }
   op_const_iterator op_end_impl() const { return &thrownValue + 2; }
   unsigned getNumOperandsImpl() const { return 2; }

protected:
   Value* thrownValue;
   GlobalVariable* typeInfo;

   Function* cleanupFn = nullptr;

public:
   static bool classof(Value const* T) { return T->getTypeID() == ThrowInstID; }
};

class RethrowInst : public TerminatorInst {
public:
   RethrowInst(Value* thrownValue, BasicBlock* parent);

   Value* getThrownValue() const { return thrownValue; }

   BasicBlock* getSuccessorAtImpl(size_t) const
   {
      llvm_unreachable("ThrowInst has no successors!");
   }

   size_t getNumSuccessorsImpl() const { return 0; }

protected:
   Value* thrownValue;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == RethrowInstID;
   }
};

class UnreachableInst : public TerminatorInst {
public:
   UnreachableInst(Context& Ctx, BasicBlock* parent);

   BasicBlock* getSuccessorAtImpl(size_t) const
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

class SwitchInst : public TerminatorInst {
public:
   typedef std::pair<ConstantInt*, BasicBlock*> CasePair;

   SwitchInst(Value* SwitchVal, BasicBlock* DefaultDst = nullptr,
              BasicBlock* parent = nullptr);

   Value* getSwitchVal() const;
   const llvm::SmallVector<CasePair, 4>& getCases() const;

   BasicBlock* getDefault() const { return DefaultDst; }
   void setDefault(BasicBlock* BB);

   void addCase(ConstantInt* val, BasicBlock* Dst);

   op_iterator op_begin_impl() { return &SwitchVal; }
   op_iterator op_end_impl() { return &SwitchVal + 1; }
   op_const_iterator op_begin_impl() const { return &SwitchVal; }
   op_const_iterator op_end_impl() const { return &SwitchVal + 1; }

   unsigned getNumOperandsImpl() const { return 1; }

   BasicBlock* getSuccessorAtImpl(size_t idx) const
   {
      if (!idx)
         return DefaultDst;

      return Cases[idx - 1].second;
   }

   size_t getNumSuccessorsImpl() const { return Cases.size() + 1; }

protected:
   Value* SwitchVal;

   BasicBlock* DefaultDst;
   llvm::SmallVector<CasePair, 4> Cases;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == SwitchInstID;
   }
};

class LandingPadInst : public TerminatorInst {
public:
   struct CatchClause {
      QualType CaughtType;
      BasicBlock* TargetBB;
   };

   typedef llvm::SmallVector<CatchClause, 2> CatchClauseList;

   LandingPadInst(Context& Ctx, BasicBlock* parent);

   const CatchClauseList& getCatchClauses() const { return CatchClauses; }

   void addCatch(CatchClause&& Clause);

   BasicBlock* getSuccessorAtImpl(size_t idx) const
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

class CallSite;
class ImmutableCallSite;

class CallInst : public Instruction, public MultiOperandInst {
public:
   CallInst(Value* Callee, llvm::ArrayRef<Value*> Args, BasicBlock* Parent);

   Value* getCallee() const;
   Function* getCalledFunction() const;
   Method* getCalledMethod() const;

   Value* getSelf() const { return Operands[0]; }

   llvm::ArrayRef<Value*> getArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

   CallSite getAsCallSite();
   ImmutableCallSite getAsImmutableCallSite() const;

   bool isTaggedDeinit() const;

   bool isDeinitCall() const { return InstBits.IsDeinit; }
   void setDeinitCall(bool V) { InstBits.IsDeinit = V; }

protected:
   CallInst(TypeID id, Context& Ctx, ArrayRef<Value*> args, BasicBlock* parent);

   CallInst(TypeID id, Function* func, ArrayRef<Value*> args,
            BasicBlock* parent);

   CallInst(TypeID id, Value* func, FunctionType* FuncTy, ArrayRef<Value*> args,
            BasicBlock* parent);

public:
   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstCallInst && kind < _lastCallInst;
   }
};

class VirtualCallInst : public CallInst {
   FunctionType* FnTy;
   il::GlobalVariable* ProtocolTypeInfo;
   unsigned Offset;

public:
   VirtualCallInst(Value* VTableOwner, FunctionType* FnTy,
                   il::GlobalVariable* ProtocolTypeInfo, unsigned Offset,
                   ArrayRef<Value*> args, BasicBlock* parent);

   FunctionType* getFunctionType() const { return FnTy; }
   il::GlobalVariable* getProtocolTypeInfo() const { return ProtocolTypeInfo; }
   unsigned getOffset() const { return Offset; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == VirtualCallInstID;
   }
};

class LambdaCallInst : public CallInst {
public:
   LambdaCallInst(Value* lambda, ArrayRef<Value*> args, BasicBlock* parent);

   Value* getLambda() const { return Operands[numOperands - 1]; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaCallInstID;
   }
};

class InvokeInst : public TerminatorInst, public MultiOperandInst {
public:
   InvokeInst(Value* Callee, ArrayRef<Value*> args,
              BasicBlock* NormalContinuation, BasicBlock* LandingPad,
              BasicBlock* parent);

   BasicBlock* getNormalContinuation() const { return NormalContinuation; }
   BasicBlock* getLandingPad() const { return LandingPad; }

   Value* getCallee() const;
   Function* getCalledFunction() const;
   Method* getCalledMethod() const;

   Value* getSelf() const { return Operands[0]; }

   llvm::ArrayRef<Value*> getArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

   CallSite getAsCallSite();
   ImmutableCallSite getAsImmutableCallSite() const;

   BasicBlock* getSuccessorAtImpl(size_t idx) const
   {
      if (!idx)
         return NormalContinuation;

      assert(idx == 1 && "invalid idx");
      return LandingPad;
   }

   size_t getNumSuccessorsImpl() const { return 2; }

protected:
   InvokeInst(TypeID id, Value* F, llvm::ArrayRef<Value*> args,
              BasicBlock* NormalContinuation, BasicBlock* LandingPad,
              BasicBlock* parent);

   BasicBlock* NormalContinuation;
   BasicBlock* LandingPad;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == InvokeInstID
             || T->getTypeID() == VirtualInvokeInstID;
   }
};

class VirtualInvokeInst : public InvokeInst {
   FunctionType* FnTy;
   il::GlobalVariable* ProtocolTypeInfo;
   unsigned Offset;

public:
   VirtualInvokeInst(Value* VTableOwner, FunctionType* FnTy,
                     il::GlobalVariable* ProtocolTypeInfo, unsigned Offset,
                     ArrayRef<Value*> args, BasicBlock* NormalContinuation,
                     BasicBlock* LandingPad, BasicBlock* parent);

   FunctionType* getFunctionType() const { return FnTy; }
   il::GlobalVariable* getProtocolTypeInfo() const { return ProtocolTypeInfo; }
   unsigned getOffset() const { return Offset; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == VirtualInvokeInstID;
   }
};

enum class Intrinsic : unsigned char {
#define CDOT_INTRINSIC(Name, Spelling) Name,
#include "cdotc/IL/Intrinsics.def"
};

class IntrinsicCallInst : public Instruction, public MultiOperandInst {
   Intrinsic calledIntrinsic;

public:
   IntrinsicCallInst(Intrinsic id, ValueType returnType,
                     llvm::ArrayRef<Value*> args, BasicBlock* parent);

   llvm::ArrayRef<Value*> getArgs() const { return {Operands, numOperands}; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }
   Intrinsic getCalledIntrinsic() const { return calledIntrinsic; }

   llvm::StringRef getIntrinsicName() const
   {
      switch (calledIntrinsic) {
#define CDOT_INTRINSIC(Name, Spelling)                                         \
   case Intrinsic::Name:                                                       \
      return #Name;
#include "Intrinsics.def"
      }
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntrinsicCallInstID;
   }

   enum FatalErrorKind : unsigned char {
      UnexpectedThrownError,
   };
};

class LLVMIntrinsicCallInst : public Instruction, public MultiOperandInst {
   IdentifierInfo* calledIntrinsic;

public:
   LLVMIntrinsicCallInst(IdentifierInfo* id, ValueType returnType,
                         llvm::ArrayRef<Value*> args, BasicBlock* parent);

   llvm::ArrayRef<Value*> getArgs() const { return {Operands, numOperands}; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }
   IdentifierInfo* getIntrinsicName() const { return calledIntrinsic; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == LLVMIntrinsicCallInstID;
   }
};

template<class CallInstTy, class InvokeInstTy, class InstTy>
class CallSiteBase {
protected:
   explicit CallSiteBase(CallInstTy* C) : Val(C) {}

   explicit CallSiteBase(InvokeInstTy* I) : Val(I) {}

   explicit CallSiteBase(std::nullptr_t) : Val(nullptr) {}

public:
   operator bool() const { return Val != nullptr; }

   bool isCallInst() const { return support::isa<CallInst>(Val); }
   bool isInvokeInst() const { return support::isa<InvokeInst>(Val); }

   CallInstTy* getAsCallInst() const
   {
      assert(isCallInst() && "not a CallInst");
      return reinterpret_cast<CallInstTy*>(Val);
   }

   InvokeInstTy* getAsInvokeInst() const
   {
      assert(!isCallInst() && "not an InvokeInst");
      return reinterpret_cast<InvokeInstTy*>(Val);
   }

   InstTy* getInstruction() const { return Val; }

   using op_iterator = Instruction::op_iterator;
   using op_const_iterator = Instruction::op_const_iterator;

#define DISPATCH_CALLSITE_VALUE(METHOD)                                        \
   isCallInst() ? getAsCallInst()->METHOD : getAsInvokeInst()->METHOD

#define DISPATCH_CALLSITE_VOID(METHOD)                                         \
   if (isCallInst()) {                                                         \
      getAsCallInst()->METHOD                                                  \
   }                                                                           \
   else {                                                                      \
      getAsInvokeInst()->METHOD                                                \
   }

   op_iterator op_begin() { return DISPATCH_CALLSITE_VALUE(op_begin()); }

   op_iterator op_end() { return DISPATCH_CALLSITE_VALUE(op_end()); }

   op_const_iterator op_begin() const
   {
      return DISPATCH_CALLSITE_VALUE(op_begin());
   }

   op_const_iterator op_end() const
   {
      return DISPATCH_CALLSITE_VALUE(op_end());
   }

   Function* getCalledFunction() const
   {
      return DISPATCH_CALLSITE_VALUE(getCalledFunction());
   }

   Value::TypeID getTypeID() const
   {
      return DISPATCH_CALLSITE_VALUE(getTypeID());
   }

   ValueType getType() const { return DISPATCH_CALLSITE_VALUE(getType()); }

   llvm::ArrayRef<Value*> getArgs() const
   {
      return DISPATCH_CALLSITE_VALUE(getArgs());
   }

#undef DISPATCH_CALLSITE_VALUE
#undef DISPATCH_CALLSITE_VOID

private:
   InstTy* Val;
};

class CallSite : public CallSiteBase<CallInst, InvokeInst, Instruction> {
public:
   explicit CallSite(CallInst* C) : CallSiteBase(C) {}
   explicit CallSite(InvokeInst* C) : CallSiteBase(C) {}
   explicit CallSite() : CallSiteBase(nullptr) {}
};

class ImmutableCallSite
    : public CallSiteBase<CallInst const, InvokeInst const, Instruction const> {
public:
   explicit ImmutableCallSite(CallInst const* C) : CallSiteBase(C) {}
   explicit ImmutableCallSite(InvokeInst const* C) : CallSiteBase(C) {}
   explicit ImmutableCallSite() : CallSiteBase(nullptr) {}
};

inline CallSite CallInst::getAsCallSite() { return CallSite(this); }

inline ImmutableCallSite CallInst::getAsImmutableCallSite() const
{
   return ImmutableCallSite(this);
}

inline CallSite InvokeInst::getAsCallSite() { return CallSite(this); }

inline ImmutableCallSite InvokeInst::getAsImmutableCallSite() const
{
   return ImmutableCallSite(this);
}

class BrInst : public TerminatorInst, public MultiOperandInst {
public:
   BrInst(Value* Condition, BasicBlock* IfBranch,
          llvm::ArrayRef<Value*> TargetArgs, BasicBlock* ElseBranch,
          llvm::ArrayRef<Value*> ElseArgs, BasicBlock* parent);

   BrInst(BasicBlock* TargetBranch, llvm::ArrayRef<Value*> TargetArgs,
          BasicBlock* parent);

   BrInst(Context& Ctx, BasicBlock* parent);

   BasicBlock* getTargetBranch() const { return TargetBranch; }
   void setTargetBranch(BasicBlock* TargetBranch);

   Value* getCondition() const { return Condition; }
   BasicBlock* getElseBranch() const { return ElseBranch; }

   llvm::ArrayRef<Value*> getTargetArgs() const;
   llvm::ArrayRef<Value*> getElseArgs() const;

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

   BasicBlock* getSuccessorAtImpl(size_t idx) const
   {
      if (idx == 0)
         return TargetBranch;

      assert(idx == 1 && ElseBranch && "invalid successor index");
      return ElseBranch;
   }

   size_t getNumSuccessorsImpl() const { return Condition ? 2 : 1; }

protected:
   Value* Condition;
   BasicBlock* TargetBranch;
   BasicBlock* ElseBranch;

   size_t numTargetArgs;

public:
   static bool classof(Value const* T) { return T->getTypeID() == BrInstID; }
};

class OperatorInst : public Instruction {
protected:
   OperatorInst(TypeID id, Context& Ctx, QualType resultType,
                BasicBlock* parent)
       : Instruction(id, ValueType(Ctx, resultType), parent)
   {
   }

public:
   static bool classof(OperatorInst const* T) { return true; }
   static inline bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
#define CDOT_UNARY_INST(Name) case Name##ID:
#define CDOT_BINARY_INST(Name) case Name##ID:
#define CDOT_CAST_INST(Name) case Name##ID:
#define CDOT_COMP_INST(Name) case Name##ID:
#include "Instructions.def"

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

class BinaryInstruction : public OperatorInst {
public:
   BinaryInstruction(TypeID id, Value* lhs, Value* rhs, QualType resultType,
                     BasicBlock* parent);

   friend class Instruction;

private:
   unsigned getNumOperandsImpl() const { return 2; }

   Value* getOperandImpl(unsigned idx)
   {
      assert(idx < 2);
      return Operands[idx];
   }

   void setOperandImpl(unsigned idx, Value* V)
   {
      assert(idx < 2);
      Operands[idx] = V;
   }

   op_iterator op_begin_impl() { return Operands; }
   op_iterator op_end_impl() { return Operands + 2; }

   op_const_iterator op_begin_impl() const { return Operands; }
   op_const_iterator op_end_impl() const { return Operands + 2; }

protected:
   Value* Operands[2];

public:
   static bool classof(Value const* T) { return classofKind(T->getTypeID()); }

   static bool classofKind(TypeID ID)
   {
      switch (ID) {
      case TypeID::BinaryOperatorInstID:
      case TypeID::CompInstID:
      case TypeID::GEPInstID:
      case TypeID::TupleExtractInstID:
      case InitInstID:
      case AssignInstID:
      case StoreInstID:
      case GenericInitInstID:
         return true;
      default:
         return false;
      }
   }
};

class UnaryInstruction : public OperatorInst {
public:
   UnaryInstruction(TypeID id, Value* operand, QualType resultType,
                    BasicBlock* parent);

   friend class Instruction;

private:
   unsigned getNumOperandsImpl() const { return 1; }

   Value* getOperandImpl(unsigned idx)
   {
      assert(idx == 0);
      return Operand;
   }

   void setOperandImpl(unsigned idx, Value* V)
   {
      assert(idx == 0);
      Operand = V;
   }

   op_iterator op_begin_impl() { return &Operand; }
   op_iterator op_end_impl() { return &Operand + 1; }

   op_const_iterator op_begin_impl() const { return &Operand; }
   op_const_iterator op_end_impl() const { return &Operand + 1; }

protected:
   Value* Operand;

public:
   static bool classof(Value const* T) { return classofKind(T->getTypeID()); }

   static bool classofKind(TypeID ID)
   {
      switch (ID) {
#define CDOT_UNARY_INST(Name) case Name##ID:
#define CDOT_CAST_INST(Name) case Name##ID:

#include "Instructions.def"

      case TypeID::LoadInstID:
      case TypeID::FieldRefInstID:
      case TypeID::EnumExtractInstID:
      case TypeID::EnumRawValueInstID:
      case TypeID::UnionInitInstID:
      case TypeID::CaptureExtractInstID:
      case TypeID::PtrToLvalueInstID:
      case TypeID::AddrOfInstID:
      case TypeID::DeallocInstID:
      case TypeID::DeallocBoxInstID:
      case TypeID::WeakRetainInstID:
      case TypeID::WeakReleaseInstID:
      case TypeID::StrongRetainInstID:
      case TypeID::StrongReleaseInstID:
      case TypeID::BeginBorrowInstID:
      case TypeID::EndBorrowInstID:
      case TypeID::MoveInstID:
         return true;
      default:
         return false;
      }
   }
};

class BinaryOperatorInst : public BinaryInstruction {
public:
   enum OpCode : unsigned char {
#define CDOT_BINARY_OP(Name, OP) Name,
#include "cdotc/IL/Instructions.def"
   };

   BinaryOperatorInst(OpCode opCode, Value* lhs, Value* rhs,
                      BasicBlock* parent);

   OpCode getOpCode() const { return opCode; }
   il::Value* getLhs() const { return getOperand(0); }
   il::Value* getRhs() const { return getOperand(1); }

   static bool classof(Value const* V)
   {
      return V->getTypeID() == BinaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class UnaryOperatorInst : public UnaryInstruction {
public:
   enum OpCode : unsigned char {
#define CDOT_UNARY_OP(Name, OP) Name,
#include "cdotc/IL/Instructions.def"
   };

   UnaryOperatorInst(OpCode opCode, Value* target, BasicBlock* parent);

   OpCode getOpCode() const { return opCode; }
   il::Value* getTarget() const { return getOperand(0); }

   static bool classof(Value const* V)
   {
      return V->getTypeID() == UnaryOperatorInstID;
   }

private:
   OpCode opCode;
};

class CompInst : public BinaryInstruction {
public:
   enum OpCode : unsigned char {
#define CDOT_COMP_OP(Name, OP) Name,
#include "cdotc/IL/Instructions.def"
   };

   CompInst(OpCode opCode, Value* lhs, Value* rhs, BasicBlock* parent);

   OpCode getOpCode() const { return opCode; }
   il::Value* getLhs() const { return getOperand(0); }
   il::Value* getRhs() const { return getOperand(1); }

   static bool classof(Value const* V) { return V->getTypeID() == CompInstID; }

private:
   OpCode opCode;
};

class FieldRefInst : public UnaryInstruction {
public:
   FieldRefInst(Value* val, DeclarationName fieldName, bool IsLet,
                BasicBlock* parent);

   DeclarationName getFieldName() const { return fieldName; }
   ast::StructDecl* getAccessedType() const;

   bool isLet() const { return FieldRefBits.IsLet; }
   unsigned getOffset() const { return Offset; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == FieldRefInstID;
   }

private:
   unsigned Offset;
   DeclarationName fieldName;
};

class GEPInst : public BinaryInstruction {
   unsigned Offset;

public:
   GEPInst(Value* val, size_t idx, bool IsLet, BasicBlock* parent);

   GEPInst(Value* val, Value* idx, bool IsLet, BasicBlock* parent);

   Value* getIndex() const { return Operands[1]; }
   Value* getVal() const { return Operands[0]; }

   bool isLet() const { return GEPBits.IsLet; }
   unsigned getOffset() const { return Offset; }

   static bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
      case GEPInstID:
      case TupleExtractInstID:
         return true;
      default:
         return false;
      }
   }
};

class TupleExtractInst : public GEPInst {
public:
   TupleExtractInst(Value* val, ConstantInt* idx, bool IsLet,
                    BasicBlock* parent);

   ConstantInt* getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == TupleExtractInstID;
   }
};

class EnumRawValueInst : public UnaryInstruction {
public:
   EnumRawValueInst(Value* Val, bool LoadVal, BasicBlock* parent);

   Value* getValue() { return Operand; }
   Value const* getValue() const { return Operand; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumRawValueInstID;
   }
};

class EnumExtractInst : public UnaryInstruction {
public:
   EnumExtractInst(Value* Val, ast::EnumCaseDecl* Case, ConstantInt* caseVal,
                   bool IsLet, BasicBlock* parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumExtractInstID;
   }

   DeclarationName getCaseName() const;
   ast::EnumCaseDecl* getCase() const { return Case; }

   bool isIndirect() const { return EnumExtractBits.IsIndirect; }
   void setIndirect(bool indirect) { EnumExtractBits.IsIndirect = indirect; }

   ConstantInt* getCaseVal() const { return caseVal; }
   ast::EnumDecl* getEnumTy() const;

   bool isLet() const { return EnumExtractBits.IsLet; }

private:
   ast::EnumCaseDecl* Case;
   ConstantInt* caseVal;
};

class CaptureExtractInst : public UnaryInstruction {
public:
   CaptureExtractInst(ConstantInt* idx, BasicBlock* parent);
   ConstantInt* getIdx() const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == CaptureExtractInstID;
   }
};

class LandingPadInst;

static const unsigned short FirstCast = Value::BitCastInstID;
extern const char* CastNames[];

class CastInst : public UnaryInstruction {
public:
   CastInst(TypeID id, Value* target, QualType toType, BasicBlock* parent)
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

class IntegerCastInst : public CastInst {
public:
   IntegerCastInst(CastKind kind, Value* target, QualType toType,
                   BasicBlock* parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == IntegerCastInstID;
   }
};

class FPCastInst : public CastInst {
public:
   FPCastInst(CastKind kind, Value* target, QualType toType,
              BasicBlock* parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == FPCastInstID;
   }
};

class UnionCastInst : public CastInst {
public:
   UnionCastInst(Value* target, ast::UnionDecl* UnionTy,
                 const IdentifierInfo* fieldName, BasicBlock* parent);

   ast::UnionDecl* getUnionTy() const { return UnionTy; }
   llvm::StringRef getFieldName() const { return fieldName->getIdentifier(); }

protected:
   ast::UnionDecl* UnionTy;
   const IdentifierInfo* fieldName;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionCastInstID;
   }
};

class ExistentialInitInst : public Instruction, public MultiOperandInst {
public:
   ExistentialInitInst(Value* target, QualType toType, il::Value* ValueTypeInfo,
                       il::GlobalVariable* ProtocolTypeInfo, bool Preallocated,
                       BasicBlock* parent);

   Value* getTarget() const { return Operands[0]; }
   Value* getValueTypeInfo() const;
   GlobalVariable* getProtocolTypeInfo() const;

   bool isPreallocated() const { return ExistentialBits.Preallocated; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   unsigned getNumOperandsImpl() const { return numOperands; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExistentialInitInstID;
   }
};

class GenericInitInst : public BinaryInstruction {
public:
   GenericInitInst(Value* Val, Value* GenericEnvironment,
                   QualType TemplateParamType, BasicBlock* Parent);

   Value* getValue() const { return Operands[0]; }
   Value* getGenericEnvironment() const { return Operands[1]; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == GenericInitInstID;
   }
};

class ExceptionCastInst : public CastInst {
public:
   ExceptionCastInst(Value* target, QualType toType, BasicBlock* parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExceptionCastInstID;
   }
};

class BitCastInst : public CastInst {
public:
   BitCastInst(CastKind kind, Value* target, QualType toType,
               BasicBlock* parent);

   CastKind getKind() const { return kind; }

protected:
   CastKind kind;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == BitCastInstID;
   }
};

class DynamicCastInst : public CastInst {
   il::Value* TargetTypeInfo;

public:
   DynamicCastInst(Value* target, il::Value* TargetTypeInfo, QualType Type,
                   BasicBlock* parent);

   il::Value* getTargetTypeInfo() const { return TargetTypeInfo; }
   unsigned getNumOperandsImpl() const { return 2; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DynamicCastInstID;
   }
};

class ExistentialCastInst : public CastInst {
   il::Value* TargetTypeInfo;
   CastKind Kind;

public:
   ExistentialCastInst(Value* Target, il::Value* TargetTypeInfo, CastKind Kind,
                       QualType TargetType, BasicBlock* parent);

   Value* getTargetTypeInfo() const { return TargetTypeInfo; }
   CastKind getKind() const { return Kind; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ExistentialCastInstID;
   }
};

class StructInitInst : public CallInst {
public:
   StructInitInst(ast::StructDecl* InitializedType, Method* Init,
                  llvm::ArrayRef<Value*> args, bool Fallible,
                  QualType FallibleTy, BasicBlock* parent);

   Method* getInit() const { return getCalledMethod(); }
   ast::StructDecl* getInitializedType() const { return InitializedType; }

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   bool isHeapAllocated() const { return AllocaBits.Heap; }
   void setHeapAllocated(bool H) { AllocaBits.Heap = H; }

   bool isFallible() const { return AllocaBits.FallibleInit; }

   ast::EnumCaseDecl* getSomeCase() const { return SomeCase; }
   void setSomeCase(ast::EnumCaseDecl* V) { SomeCase = V; }

protected:
   ast::StructDecl* InitializedType;
   ast::EnumCaseDecl* SomeCase = nullptr;

public:
   static bool classof(StructInitInst const* T) { return true; }
   static inline bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
      case StructInitInstID:
         return true;
      default:
         return false;
      }
   }
};

class UnionInitInst : public UnaryInstruction {
public:
   UnionInitInst(ast::UnionDecl* UnionTy, Value* InitializerVal,
                 BasicBlock* parent);

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   ast::UnionDecl* getUnionTy() const { return UnionTy; }
   Value* getInitializerVal() const { return Operand; }

protected:
   ast::UnionDecl* UnionTy;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionInitInstID;
   }
};

class EnumInitInst : public Instruction, public MultiOperandInst {
public:
   EnumInitInst(Context& Ctx, ast::EnumDecl* EnumTy, ast::EnumCaseDecl* Case,
                llvm::ArrayRef<Value*> args, BasicBlock* parent);

   llvm::ArrayRef<Value*> getArgs() const { return {Operands, numOperands}; }

   bool canUseSRetValue() const { return EnumInitBits.CanUseSRetValue; }
   void setCanUseSRetValue() { EnumInitBits.CanUseSRetValue = true; }

   bool isIndirect() const { return EnumInitBits.IsIndirect; }
   void setIndirect(bool indirect) { EnumInitBits.IsIndirect = indirect; }

   ast::EnumDecl* getEnumTy() const { return EnumTy; }
   ast::EnumCaseDecl* getCase() const { return Case; }
   DeclarationName getCaseName() const;

protected:
   ast::EnumDecl* EnumTy;
   ast::EnumCaseDecl* Case;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumInitInstID;
   }
};

class LambdaInitInst : public Instruction, public MultiOperandInst {
public:
   LambdaInitInst(Function* Function, QualType LambdaTy,
                  llvm::ArrayRef<Value*> Captures, BasicBlock* parent);

   bool canUseSRetValue() const { return AllocaBits.CanUseSRetValue; }
   void setCanUseSRetValue() { AllocaBits.CanUseSRetValue = true; }

   Function* getFunction() const { return F; }

   op_iterator op_begin_impl() { return &Operands[0]; }
   op_iterator op_end_impl() { return Operands + numOperands; }
   op_const_iterator op_begin_impl() const { return &Operands[0]; }
   op_const_iterator op_end_impl() const { return Operands + numOperands; }

   unsigned getNumOperandsImpl() const { return numOperands; }

protected:
   Function* F;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == LambdaInitInstID;
   }
};

class DeallocInst : public UnaryInstruction {
public:
   DeallocInst(Value* V, bool Heap, BasicBlock* P);

   Value* getValue() const { return Operand; }

   bool isHeap() const { return AllocaBits.Heap; }
   void setHeap(bool H) { AllocaBits.Heap = H; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeallocInstID;
   }
};

class DeallocBoxInst : public UnaryInstruction {
public:
   DeallocBoxInst(Value* V, BasicBlock* P);

   Value* getValue() const { return Operand; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == DeallocBoxInstID;
   }
};

class AssignInst : public BinaryInstruction {
public:
   AssignInst(Value* dst, Value* src, BasicBlock* parent);

   Value* getDst() const { return Operands[0]; }
   Value* getSrc() const { return Operands[1]; }

   MemoryOrder getMemoryOrder() const { return StoreBits.memoryOrder; }
   void setMemoryOrder(MemoryOrder V) { StoreBits.memoryOrder = V; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == AssignInstID;
   }
};

class StoreInst : public BinaryInstruction {
public:
   StoreInst(Value* dst, Value* src, BasicBlock* parent);

   Value* getDst() const { return Operands[0]; }
   Value* getSrc() const { return Operands[1]; }

   MemoryOrder getMemoryOrder() const { return StoreBits.memoryOrder; }
   void setMemoryOrder(MemoryOrder V) { StoreBits.memoryOrder = V; }

   static bool classof(Value const* T) { return T->getTypeID() == StoreInstID; }
};

class InitInst : public BinaryInstruction {
public:
   InitInst(Value* dst, Value* src, BasicBlock* parent);

   Value* getDst() const { return Operands[0]; }
   Value* getSrc() const { return Operands[1]; }

   MemoryOrder getMemoryOrder() const { return StoreBits.memoryOrder; }
   void setMemoryOrder(MemoryOrder V) { StoreBits.memoryOrder = V; }

   static bool classof(Value const* T) { return T->getTypeID() == InitInstID; }
};

class LoadInst : public UnaryInstruction {
public:
   LoadInst(Value* target, BasicBlock* parent);

   MemoryOrder getMemoryOrder() const { return LoadBits.memoryOrder; }
   void setMemoryOrder(MemoryOrder V) { LoadBits.memoryOrder = V; }

   bool isFieldAccessLoad() const { return LoadBits.IsFieldAccessLoad; }
   void setFieldAccessLoad(bool b) { LoadBits.IsFieldAccessLoad = b; }

   Value* getTarget() const { return Operand; }

   static bool classof(Value const* T) { return T->getTypeID() == LoadInstID; }
};

class AddrOfInst : public UnaryInstruction {
public:
   AddrOfInst(Value* target, BasicBlock* parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == AddrOfInstID;
   }
};

class PtrToLvalueInst : public UnaryInstruction {
public:
   PtrToLvalueInst(Value* target, BasicBlock* parent);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == PtrToLvalueInstID;
   }
};

class RefcountingInst : public UnaryInstruction {
protected:
   RefcountingInst(TypeID ID, Value* Val, BasicBlock* Parent);

public:
   Value* getTarget() const { return Operand; }

   static bool classof(const Value* T)
   {
      switch (T->getTypeID()) {
      case StrongRetainInstID:
      case StrongReleaseInstID:
      case WeakRetainInstID:
      case WeakReleaseInstID:
         return true;
      default:
         return false;
      }
   }
};

class RetainInst : public RefcountingInst {
protected:
   RetainInst(TypeID ID, Value* Val, BasicBlock* Parent);

public:
   static bool classof(const Value* T)
   {
      switch (T->getTypeID()) {
      case StrongRetainInstID:
      case WeakRetainInstID:
         return true;
      default:
         return false;
      }
   }
};

class ReleaseInst : public RefcountingInst {
protected:
   ReleaseInst(TypeID ID, Value* Val, BasicBlock* Parent);

public:
   static bool classof(const Value* T)
   {
      switch (T->getTypeID()) {
      case StrongReleaseInstID:
      case WeakReleaseInstID:
         return true;
      default:
         return false;
      }
   }
};

class StrongRetainInst : public RetainInst {
public:
   StrongRetainInst(Value* Val, BasicBlock* Parent);

   static bool classof(const Value* V)
   {
      return V->getTypeID() == StrongRetainInstID;
   }
};

class StrongReleaseInst : public ReleaseInst {
public:
   StrongReleaseInst(Value* Val, BasicBlock* Parent);

   static bool classof(const Value* V)
   {
      return V->getTypeID() == StrongReleaseInstID;
   }
};

class WeakRetainInst : public RetainInst {
public:
   WeakRetainInst(Value* Val, BasicBlock* Parent);

   static bool classof(const Value* V)
   {
      return V->getTypeID() == WeakRetainInstID;
   }
};

class WeakReleaseInst : public ReleaseInst {
public:
   WeakReleaseInst(Value* Val, BasicBlock* Parent);

   static bool classof(const Value* V)
   {
      return V->getTypeID() == WeakReleaseInstID;
   }
};

class MoveInst : public UnaryInstruction {
public:
   MoveInst(Value* Target, BasicBlock* Parent);

   static bool classof(const Value* V) { return V->getTypeID() == MoveInstID; }
};

class EndBorrowInst;

class BeginBorrowInst : public UnaryInstruction {
   SourceLocation BeginLoc;
   SourceLocation EndLoc;

public:
   BeginBorrowInst(Value* Target, SourceLocation BeginLoc,
                   SourceLocation EndLoc, bool IsMutableBorrow,
                   BasicBlock* Parent);

   bool isMutableBorrow() const { return BorrowBits.IsMutableBorrow; }
   SourceLocation getBeginBorrowLoc() const { return BeginLoc; }
   SourceLocation getEndBorrowLoc() const { return EndLoc; }

   static bool classof(const Value* V)
   {
      return V->getTypeID() == BeginBorrowInstID;
   }
};

class EndBorrowInst : public UnaryInstruction {
   SourceLocation Loc;

public:
   EndBorrowInst(Value* Target, SourceLocation Loc, bool IsMutableBorrow,
                 BasicBlock* Parent);

   bool isMutableBorrow() const { return BorrowBits.IsMutableBorrow; }
   SourceLocation getEndBorrowLoc() const { return Loc; }

   static bool classof(const Value* V)
   {
      return V->getTypeID() == EndBorrowInstID;
   }
};

class DebugLocInst : public Instruction {
   SourceLocation Loc;
   unsigned Line;
   unsigned Col;
   unsigned FileID;

public:
   DebugLocInst(SourceLocation Loc, unsigned Line, unsigned Col,
                unsigned FileID, ValueType VoidTy, BasicBlock* Parent);

   SourceLocation getLoc() const { return Loc; }
   unsigned int getLine() const { return Line; }
   unsigned int getCol() const { return Col; }
   unsigned int getFileID() const { return FileID; }

   static bool classof(const Value* V)
   {
      return V->getTypeID() == DebugLocInstID;
   }
};

class DebugLocalInst : public Instruction {
   const IdentifierInfo* Name;
   Value* Val;

public:
   DebugLocalInst(const IdentifierInfo* Name, Value* V, BasicBlock* Parent);

   const IdentifierInfo* getName() const { return Name; }
   Value* getVal() const { return Val; }

   static bool classof(const Value* V)
   {
      return V->getTypeID() == DebugLocalInstID;
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_INSTRUCTIONS_H
