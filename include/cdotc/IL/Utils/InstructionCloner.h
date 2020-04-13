#ifndef CDOT_INSTRUCTIONCLONER_H
#define CDOT_INSTRUCTIONCLONER_H

#include "cdotc/IL/Passes/InstructionVisitor.h"

#include "cdotc/IL/Constants.h"
#include "cdotc/IL/ILBuilder.h"

namespace cdot {
namespace il {

template<class SubClass>
class InstructionCloner : public InstructionVisitor<SubClass, il::Value*> {
protected:
   il::ILBuilder& Builder;
   llvm::DenseMap<const il::Value*, il::Value*> Substitutions;

   il::Value* getSubstitution(const il::Value* V) { return Substitutions[V]; }

   template<class T> T* getSubstitution(const il::Value* V)
   {
      return support::cast_or_null<T>(Substitutions[V]);
   }

   il::Value* visit(const il::Value* V)
   {
      auto It = Substitutions.find(V);
      if (It != Substitutions.end())
         return It->getSecond();

      if (support::isa<Constant>(V))
         return const_cast<Value*>(V);

      if (auto* I = support::dyn_cast<Instruction>(V)) {
         auto* Val = InstructionVisitor<SubClass, il::Value*>::visit(*I);
         addSubstitution(*V, Val);

         return Val;
      }

      InstructionVisitor<SubClass, il::Value*>::visit(V);
      return getSubstitution(V);
   }

   SmallVector<il::Value*, 2> visit(ArrayRef<il::Value*> Arr)
   {
      return visit(Arr.begin(), Arr.end());
   }

   SmallVector<il::Value*, 2> visit(const Value* const* BeginIt,
                                    const Value* const* EndIt)
   {
      SmallVector<il::Value*, 2> Vals;
      while (BeginIt != EndIt)
         Vals.push_back(visit(*BeginIt++));

      return Vals;
   }

public:
   explicit InstructionCloner(ILBuilder& Builder) : Builder(Builder) {}
   void addSubstitution(const il::Value& OldVal, il::Value* NewVal)
   {
      Substitutions[&OldVal] = NewVal;
   }

   void visitArgument(const il::Argument& A)
   {
      return addSubstitution(
          A, Builder.CreateArgument(A.getType(), A.getConvention(), nullptr,
                                    A.getName(), A.getSourceLoc()));
   }

   void cloneBasicBlock(il::Function& F, const il::BasicBlock& B)
   {
      auto* Copy = Builder.CreateBasicBlock(&F, true, B.getName());
      addSubstitution(B, Copy);

      for (auto& Arg : B.getArgs()) {
         auto* ArgCopy = Copy->addBlockArg(Arg.getType(), Arg.getName());
         addSubstitution(Arg, ArgCopy);
      }
   }

   il::Value* visitAllocaInst(const AllocaInst& I)
   {
      QualType AllocType = I.getType()->getReferencedType();
      if (!I.getAllocSize())
         return Builder.CreateAlloca(AllocType, I.getAlignment(),
                                     I.isHeapAlloca(), I.isLet(), I.getName());

      return Builder.CreateAlloca(AllocType, I.getAllocSize(), I.getAlignment(),
                                  I.isHeapAlloca(), I.isLet(), I.getName());
   }

   il::Value* visitAllocBoxInst(const AllocBoxInst& I)
   {
      return Builder.CreateAllocBox(I.getType()->getBoxedType(),
                                    I.getDeinitializer(), I.getName());
   }

   il::Value* visitDeallocInst(const DeallocInst& I)
   {
      return Builder.CreateDealloc(visit(I.getValue()), I.isHeap(),
                                   I.getName());
   }

   il::Value* visitDeallocBoxInst(const DeallocBoxInst& I)
   {
      return Builder.CreateDeallocBox(visit(I.getValue()), I.getName());
   }

   il::Value* visitLambdaInitInst(const LambdaInitInst& I)
   {
      return Builder.CreateLambdaInit(I.getFunction(), I.getType(),
                                      visit(I.op_begin(), I.op_end()),
                                      I.getName());
   }

   il::Value* visitExistentialInitInst(const ExistentialInitInst& I)
   {
      return Builder.CreateExistentialInit(
          visit(I.getTarget()), I.getType(), I.getValueTypeInfo(),
          I.getProtocolTypeInfo(), I.isPreallocated(), I.getName());
   }

   il::Value* visitAssignInst(const AssignInst& I)
   {
      return Builder.CreateAssign(visit(I.getSrc()), visit(I.getDst()),
                                  I.getMemoryOrder(), I.getName());
   }

   il::Value* visitStoreInst(const StoreInst& I)
   {
      return Builder.CreateStore(visit(I.getSrc()), visit(I.getDst()),
                                 I.getMemoryOrder(), I.getName());
   }

   il::Value* visitInitInst(const InitInst& I)
   {
      return Builder.CreateInit(visit(I.getSrc()), visit(I.getDst()),
                                I.getName());
   }

   il::Value* visitGEPInst(const GEPInst& I)
   {
      return Builder.CreateGEP(visit(I.getVal()), visit(I.getIndex()),
                               I.isLet(), I.getName());
   }

   il::Value* visitFieldRefInst(const FieldRefInst& I)
   {
      return Builder.CreateFieldRef(visit(I.getOperand(0)), I.getFieldName(),
                                    I.isLet(), I.getName());
   }

   il::Value* visitTupleExtractInst(const TupleExtractInst& I)
   {
      return Builder.CreateTupleExtract(
          visit(I.getOperand(0)),
          support::cast<ConstantInt>(I.getIndex())->getZExtValue(), I.isLet(),
          I.getName());
   }

   il::Value* visitEnumRawValueInst(const EnumRawValueInst& I)
   {
      return Builder.CreateEnumRawValue(
          visit(I.getOperand(0)), !I.getType()->isReferenceType(), I.getName());
   }

   il::Value* visitEnumExtractInst(const EnumExtractInst& I)
   {
      return Builder.CreateEnumExtract(visit(I.getOperand(0)), I.getCase(),
                                       I.getCaseVal()->getZExtValue(),
                                       I.isLet(), I.getName());
   }

   il::Value* visitLoadInst(const LoadInst& I)
   {
      return Builder.CreateLoad(visit(I.getTarget()), I.getMemoryOrder(),
                                I.getName());
   }

   il::Value* visitAddrOfInst(const AddrOfInst& I)
   {
      return Builder.CreateAddrOf(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitPtrToLvalueInst(const PtrToLvalueInst& I)
   {
      return Builder.CreatePtrToLvalue(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitCaptureExtractInst(const CaptureExtractInst& I)
   {
      return Builder.CreateCaptureExtract(I.getIdx()->getZExtValue(),
                                          I.getName());
   }

   il::Value* visitStrongRetainInst(const StrongRetainInst& I)
   {
      return Builder.CreateRetain(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitStrongReleaseInst(const StrongReleaseInst& I)
   {
      return Builder.CreateRelease(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitWeakRetainInst(const WeakRetainInst& I)
   {
      return Builder.CreateWeakRetain(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitWeakReleaseInst(const WeakReleaseInst& I)
   {
      return Builder.CreateWeakRelease(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitRetInst(const RetInst& I)
   {
      if (!I.getReturnedValue())
         return Builder.CreateRetVoid();

      return Builder.CreateRet(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitYieldInst(const YieldInst& I)
   {
      return Builder.CreateYield(
          visit(I.getOperand(0)), getSubstitution<BasicBlock>(I.getResumeDst()),
          visit(I.getResumeArgs().begin(), I.getResumeArgs().end()),
          I.isFinalYield(), I.getName());
   }

   il::Value* visitThrowInst(const ThrowInst& I)
   {
      return Builder.CreateThrow(visit(I.getOperand(0)), I.getTypeInfo(),
                                 I.getName());
   }

   il::Value* visitRethrowInst(const RethrowInst& I)
   {
      return Builder.CreateRethrow(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitUnreachableInst(const UnreachableInst& I)
   {
      return Builder.CreateUnreachable();
   }

   il::Value* visitBrInst(const BrInst& I)
   {
      if (I.getElseBranch()) {
         return Builder.CreateCondBr(
             visit(I.getCondition()),
             getSubstitution<BasicBlock>(I.getTargetBranch()),
             getSubstitution<BasicBlock>(I.getElseBranch()),
             visit(I.getTargetArgs()), visit(I.getElseArgs()), I.getName());
      }

      return Builder.CreateBr(getSubstitution<BasicBlock>(I.getTargetBranch()),
                              visit(I.getTargetArgs()), I.getName());
   }

   il::Value* visitSwitchInst(const SwitchInst& I)
   {
      auto* Sw = Builder.CreateSwitch(
          visit(I.getSwitchVal()), getSubstitution<BasicBlock>(I.getDefault()),
          I.getName());

      for (auto& Case : I.getCases()) {
         Sw->addCase(Case.first, getSubstitution<BasicBlock>(Case.second));
      }

      return Sw;
   }

   il::Value* visitInvokeInst(const InvokeInst& I)
   {
      return Builder.CreateInvoke(
          visit(I.getCallee()), visit(I.getArgs()),
          getSubstitution<BasicBlock>(I.getNormalContinuation()),
          getSubstitution<BasicBlock>(I.getLandingPad()), I.getName());
   }

   il::Value* visitVirtualInvokeInst(const VirtualInvokeInst& I)
   {
      return Builder.CreateVirtualInvoke(
          visit(I.getCallee()), I.getFunctionType(), I.getProtocolTypeInfo(),
          I.getOffset(), visit(I.getArgs()),
          getSubstitution<BasicBlock>(I.getNormalContinuation()),
          getSubstitution<BasicBlock>(I.getLandingPad()), I.getName());
   }

   il::Value* visitLandingPadInst(const LandingPadInst& I)
   {
      llvm_unreachable("nah");
   }

   il::Value* visitIntrinsicCallInst(const IntrinsicCallInst& I)
   {
      return Builder.CreateIntrinsicCall(I.getCalledIntrinsic(),
                                         visit(I.getArgs()), I.getName());
   }

   il::Value* visitLLVMIntrinsicCallInst(const LLVMIntrinsicCallInst& I)
   {
      return Builder.CreateLLVMIntrinsicCall(I.getIntrinsicName(), I.getType(),
                                             visit(I.getArgs()), I.getName());
   }

   il::Value* visitMoveInst(const MoveInst& I)
   {
      return Builder.CreateMove(visit(I.getOperand(0)), I.getName());
   }

   il::Value* visitBeginBorrowInst(const BeginBorrowInst& I)
   {
      return Builder.CreateBeginBorrow(
          visit(I.getOperand(0)), I.getBeginBorrowLoc(), I.getEndBorrowLoc(),
          I.isMutableBorrow(), I.getName());
   }

   il::Value* visitEndBorrowInst(const EndBorrowInst& I)
   {
      return Builder.CreateEndBorrow(visit(I.getOperand(0)),
                                     I.getEndBorrowLoc(), I.isMutableBorrow(),
                                     I.getName());
   }

   il::Value* visitUnionInitInst(const UnionInitInst& I)
   {
      llvm_unreachable("nah");
   }

   il::Value* visitEnumInitInst(const EnumInitInst& I)
   {
      return Builder.CreateEnumInit(I.getEnumTy(), I.getCase(),
                                    visit(I.getArgs()), I.getName());
   }

   il::Value* visitDebugLocInst(const DebugLocInst& I)
   {
      return Builder.CreateDebugLoc(I.getLoc());
   }

   il::Value* visitDebugLocalInst(const DebugLocalInst& I)
   {
      return Builder.CreateDebugLocal(I.getName(), visit(I.getVal()));
   }

   il::Value* visitCallInst(const CallInst& I)
   {
      return Builder.CreateCall(visit(I.getCallee()), visit(I.getArgs()),
                                I.getName());
   }

   il::Value* visitVirtualCallInst(const VirtualCallInst& I)
   {
      return Builder.CreateVirtualCall(
          visit(I.getCallee()), I.getFunctionType(), I.getProtocolTypeInfo(),
          I.getOffset(), visit(I.getArgs()), I.getName());
   }

   il::Value* visitLambdaCallInst(const LambdaCallInst& I)
   {
      return Builder.CreateLambdaCall(visit(I.getLambda()), visit(I.getArgs()),
                                      I.getName());
   }

   il::Value* visitStructInitInst(const StructInitInst& I)
   {
      return Builder.CreateStructInit(I.getInitializedType(), I.getInit(),
                                      visit(I.getArgs()), I.isFallible(),
                                      I.getType(), I.getName());
   }

   il::Value* visitBinaryOperatorInst(const BinaryOperatorInst& I)
   {
      return Builder.CreateBinOp(I.getOpCode(), visit(I.getLhs()),
                                 visit(I.getRhs()), I.getName());
   }

   il::Value* visitUnaryOperatorInst(const UnaryOperatorInst& I)
   {
      return Builder.CreateUnaryOp(I.getOpCode(), visit(I.getTarget()),
                                   I.getName());
   }

   il::Value* visitCompInst(const CompInst& I)
   {
      return Builder.CreateComp(I.getOpCode(), visit(I.getLhs()),
                                visit(I.getRhs()), I.getName());
   }

   il::Value* visitBitCastInst(const BitCastInst& I)
   {
      return Builder.CreateBitCast(I.getKind(), visit(I.getOperand(0)),
                                   I.getType(), I.getName());
   }

   il::Value* visitIntegerCastInst(const IntegerCastInst& I)
   {
      return Builder.CreateIntegerCast(I.getKind(), visit(I.getOperand(0)),
                                       I.getType(), I.getName());
   }

   il::Value* visitFPCastInst(const FPCastInst& I)
   {
      return Builder.CreateFPCast(I.getKind(), visit(I.getOperand(0)),
                                  I.getType(), I.getName());
   }

   il::Value* visitUnionCastInst(const UnionCastInst& I)
   {
      llvm_unreachable("nah!");
   }

   il::Value* visitDynamicCastInst(const DynamicCastInst& I)
   {
      return Builder.CreateDynamicCast(visit(I.getOperand(0)),
                                       I.getTargetTypeInfo(), I.getType(),
                                       I.getName());
   }

   il::Value* visitExistentialCastInst(const ExistentialCastInst& I)
   {
      return Builder.CreateExistentialCast(
          visit(I.getOperand(0)), visit(I.getTargetTypeInfo()), I.getKind(),
          I.getType(), I.getName());
   }

   il::Value* visitExceptionCastInst(const ExceptionCastInst& I)
   {
      return Builder.CreateExceptionCast(visit(I.getOperand(0)), I.getType(),
                                         I.getName());
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_INSTRUCTIONCLONER_H
