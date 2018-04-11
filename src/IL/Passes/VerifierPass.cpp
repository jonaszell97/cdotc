//
// Created by Jonas Zell on 22.11.17.
//

#include "VerifierPass.h"

#include "Basic/CastKind.h"
#include "IL/Constants.h"
#include "IL/Writer/ModuleWriter.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/FileSystem.h>
#include <IL/Instructions.h>

using namespace cdot::support;

namespace cdot {
namespace il {

void VerifierPass::emitErrorMessage(llvm::StringRef msg)
{
   llvm::outs() << "\033[21;31merror:\033[0m " << msg << "\n";
}

void VerifierPass::emitError(Instruction const& I)
{
   emitError(*I.getParent()->getParent());
   llvm::outs() << "\n      ...\n      ";

   ModuleWriter Writer(&I);
   Writer.WriteTo(llvm::outs());
}

void VerifierPass::emitError(Function const& F)
{
   llvm::outs() << "   ";

   ModuleWriter Writer(&F);
   Writer.WriteFunctionDeclTo(llvm::outs());
}

void VerifierPass::emitError(GlobalVariable const& G)
{
   llvm::outs() << "   ";

   ModuleWriter Writer(&G);
   Writer.WriteTo(llvm::outs());
}

void VerifierPass::emitError(BasicBlock const &B)
{
   emitError(*B.getParent());
   llvm::outs() << "\n   ";

   ModuleWriter Writer(&B);
   Writer.WriteBasicBlockDeclTo(llvm::outs());
}

void VerifierPass::visitModule(Module& M)
{
   for (const auto &G : M.getGlobalList())
      visitGlobalVariable(G);

   for (const auto &F : M.getFuncList())
      visitFunction(F);

   if (!IsValid) {
      std::error_code EC;
      llvm::raw_fd_ostream fd("/Users/Jonas/CDotProjects/ex/stdlib/_error"
                                 ".cdotil",
                              EC, llvm::sys::fs::F_RW);

      M.writeTo(fd);
      fd.flush();
   }
}

namespace {

bool typesCompatible(const QualType &lhs, const QualType &rhs)
{
   return *lhs == *rhs;
}

} // anonymous namespace

void VerifierPass::checkOperandAccessibility(il::Instruction const& I)
{
   auto op_end = I.op_end();
   for (auto it = I.op_begin(); it != op_end; ++it) {
      if (auto Inst = dyn_cast<Instruction>(*it))
         errorIf(I.getParent()->getParent() != Inst->getParent()->getParent(),
                 "referencing instruction in different function!", I, *Inst);
   }
}

void VerifierPass::visitGlobalVariable(GlobalVariable const &G)
{
   if (auto Init = G.getInitializer()) {
      errorIf(!typesCompatible(Init->getType(),
                               G.getType()->getReferencedType()),
              "global initializer type does not equal global type", G);
   }
}

void VerifierPass::visitFunction(Function const &F)
{
   if (F.isDeclared())
      return;

   for (const auto &BB : F.getBasicBlocks())
      visitBasicBlock(BB);
}

void VerifierPass::visitBasicBlock(BasicBlock const &B)
{
   errorIf(!B.getTerminator(), "basic block does not have a terminator!", B);

   size_t i = 0;
   size_t numInsts = B.getInstructions().size();

   for (const auto &I : B.getInstructions()) {
      checkOperandAccessibility(I);
      visit(I);

      errorIf(isa<TerminatorInst>(I) && i != numInsts - 1,
              "terminator in the middle of a basic block", I);

      ++i;
   }
}


void VerifierPass::visitAllocaInst(AllocaInst const& I)
{
   errorIf((I.getType()->getReferencedType()->isVoidType()),
           "allocated type cannot be void", I);
   errorIf(I.getAlignment() != 1 && I.getAlignment() % 2 != 0, "invalid "
      "alignment", I);
}

void VerifierPass::visitAllocBoxInst(const il::AllocBoxInst &I)
{
   errorIf((I.getType()->getReferencedType()->isVoidType()),
           "allocated type cannot be void", I);
}

void VerifierPass::visitDeallocInst(const il::DeallocInst &I)
{

}

void VerifierPass::visitDeallocBoxInst(const il::DeallocBoxInst &I)
{

}

void VerifierPass::visitLambdaInitInst(LambdaInitInst const& I)
{
   
}

void  VerifierPass::visitStoreInst(StoreInst const& I)
{
   auto dst = I.getDst()->getType();
   auto src = I.getSrc()->getType();

   if (dst->isReferenceType()) {
      errorIf(!typesCompatible(dst->getReferencedType(), src),
              "stored operand must have the same type as destination", I);
   }
   else if (dst->isPointerType()) {
      errorIf(!typesCompatible(dst->getPointeeType(), src),
              "stored operand must have the same type as destination", I);
   }
   else {
      errorIf(true, "invalid store target", I);
   }
}

void VerifierPass::visitGEPInst(GEPInst const& I)
{
   errorIf(!I.getIndex()->getType()->isIntegerType(), "gep index must be "
      "integral", I);
}

void VerifierPass::visitCaptureExtractInst(const CaptureExtractInst &I)
{

}

void VerifierPass::visitFieldRefInst(FieldRefInst const& I)
{
   
}

void  VerifierPass::visitTupleExtractInst(TupleExtractInst const& I)
{
   
}

void VerifierPass::visitEnumExtractInst(const EnumExtractInst &I)
{

}

void  VerifierPass::visitEnumRawValueInst(EnumRawValueInst const& I)
{
   
}

void  VerifierPass::visitLoadInst(LoadInst const& I)
{
   auto OpType = I.getOperand(0)->getType();
   errorIf(!OpType->isReferenceType() && !OpType->isPointerType(),
           "cannot load value", I);
}

void VerifierPass::visitAddrOfInst(AddrOfInst const& I)
{

}

void VerifierPass::visitPtrToLvalueInst(const PtrToLvalueInst &I)
{

}

void VerifierPass::visitRetInst(RetInst const& I)
{
   auto func = I.getParent()->getParent();
   if (auto Val = I.getReturnedValue()) {
      errorIf(!typesCompatible(Val->getType(), func->getReturnType()),
              "type of returned value does not equal declared return type", I);
   }
   else {
      errorIf(!func->getReturnType()->isVoidType(),
              "type of returned value does not equal declared return type", I);
   }
}

void VerifierPass::visitThrowInst(ThrowInst const& I)
{

}

void VerifierPass::visitUnreachableInst(UnreachableInst const& I)
{

}

void VerifierPass::visitBrInst(BrInst const& I)
{
   if (auto Cond = I.getCondition()) {
      errorIf(!Cond->getType()->isInt1Ty(false), "condition must have i1 "
         "type", I);
   }

   if (auto Br = I.getTargetBranch()) {
      auto &NeededArgs = Br->getArgs();
      auto GivenArgs = I.getTargetArgs();

      auto InstFn = I.getParent()->getParent();
      auto BBFn = Br->getParent();

      errorIf(InstFn != BBFn, "cannot branch to basic block in different "
         "function", I);

      if (NeededArgs.size() != GivenArgs.size()) {
         errorIf(true, "invalid number of arguments passed to br", I,
                 *Br);

         return;
      }

      size_t i = 0;
      for (const auto &needed : NeededArgs) {
         auto &given = GivenArgs[i];

         errorIf(!typesCompatible(needed.getType(), given->getType()),
                 "invalid argument type", I, *Br);

         ++i;
      }
   }
   if (auto Else = I.getElseBranch()) {
      auto &NeededArgs = Else->getArgs();
      auto GivenArgs = I.getElseArgs();

      auto InstFn = I.getParent()->getParent();
      auto BBFn = Else->getParent();

      errorIf(InstFn != BBFn, "cannot branch to basic block in different "
         "function", I);

      if (NeededArgs.size() != GivenArgs.size()) {
         errorIf(true, "invalid number of arguments passed to br", I,
                 *Else);

         return;
      }

      size_t i = 0;
      for (const auto &needed : NeededArgs) {
         auto &given = GivenArgs[i];

         errorIf(!typesCompatible(needed.getType(), given->getType()),
                 "invalid argument type", I, *Else);

         ++i;
      }
   }
}

void VerifierPass::visitSwitchInst(SwitchInst const& I)
{
   errorIf(!I.getSwitchVal()->getType()->isIntegerType(), "switch type must be "
      "integral", I);

   errorIf(!I.getDefault(), "no default destination provided for switch", I);

   auto InstFn = I.getParent()->getParent();

   llvm::SmallDenseSet<uint64_t> CaseVals;
   for (const auto &C : I.getCases()) {
      if (C.first) {
         errorIf(!CaseVals.insert(C.first->getZExtValue()).second,
                 "duplicate case value " + std::to_string(
                    C.first->getZExtValue()),
                 I);
      }

      auto BBFn = C.second->getParent();
      errorIf(InstFn != BBFn, "cannot branch to basic block in different "
         "function", I);
   }
}

namespace {

bool compatibleArgCount(BasicBlock::ArgList const &Needed,
                        llvm::ArrayRef<il::Value *> Given,
                        bool vararg,
                        bool omitSelf = false) {
   auto neededCnt = Needed.size();
   auto givenCnt = Given.size();

   if (omitSelf)
      --neededCnt;

   if (vararg) {
      return givenCnt >= neededCnt - 1;
   }

   return givenCnt == neededCnt;
}

bool compatibleArgCount(FunctionType const* FuncTy,
                        llvm::ArrayRef<il::Value *> Given) {
   bool vararg = FuncTy->isCStyleVararg();
   auto neededCnt = FuncTy->getParamTypes().size();
   auto givenCnt = Given.size();

   if (vararg) {
      return givenCnt >= neededCnt - 1;
   }

   return givenCnt == neededCnt;
}

} // anonymous namespace

void VerifierPass::visitInvokeInst(InvokeInst const& I)
{
   auto InstFn = I.getParent()->getParent();
   auto NormalBBFn = I.getNormalContinuation()->getParent();
   auto UnwindBBFn = I.getLandingPad()->getParent();

   errorIf(InstFn != NormalBBFn, "cannot branch to basic block in different "
      "function", I);
   errorIf(InstFn != UnwindBBFn, "cannot branch to basic block in different "
      "function", I);

   auto F = I.getCalledFunction();
   errorIf(F->getParent() != I.getParent()->getParent()->getParent(),
           "referencing function in a different module", I, *F);

   auto &NeededArgs = F->getEntryBlock()->getArgs();
   auto GivenArgs = I.getArgs();

   if (!compatibleArgCount(NeededArgs, GivenArgs, F->isCStyleVararg())) {
      errorIf(true, "invalid number of arguments for call", I);
      return;
   }

   size_t i = 0;
   for (const auto &needed : NeededArgs) {
      if (needed.isVararg())
         break;

      auto &given = GivenArgs[i];
      errorIf(!typesCompatible(needed.getType(), given->getType()),
              "invalid argument type for argument " + std::to_string(i), I);

      ++i;
   }
}

void VerifierPass::visitVirtualInvokeInst(const VirtualInvokeInst &I)
{
   errorIf(!I.getCalledMethod()->isVirtual(), "method is not virtual", I);
   visitInvokeInst(I);
}

void VerifierPass::visitProtocolInvokeInst(const ProtocolInvokeInst &I)
{
   visitInvokeInst(I);
}

void VerifierPass::visitLandingPadInst(LandingPadInst const& I)
{

}

void VerifierPass::visitIntrinsicCallInst(IntrinsicCallInst const& I)
{

}

void VerifierPass::visitCallInst(CallInst const& I)
{
   auto F = I.getCalledFunction();
   errorIf(F->getParent() != I.getParent()->getParent()->getParent(),
           "referencing function in a different module", I, *F);

   auto &NeededArgs = F->getEntryBlock()->getArgs();
   auto GivenArgs = I.getArgs();

   if (!compatibleArgCount(NeededArgs, GivenArgs, F->isCStyleVararg())) {
      errorIf(true, "invalid number of arguments for call", I, *F);
      return;
   }

   size_t i = 0;
   for (const auto &needed : NeededArgs) {
      if (needed.isVararg())
         break;

      auto &given = GivenArgs[i];
      errorIf(!typesCompatible(needed.getType(), given->getType()),
              "invalid argument type for argument " + std::to_string(i), I, *F);

      ++i;
   }
}

void VerifierPass::visitVirtualCallInst(const VirtualCallInst &I)
{
   errorIf(!I.getCalledMethod()->isVirtual(), "method is not virtual", I);
   visitCallInst(I);
}

void VerifierPass::visitProtocolCallInst(const ProtocolCallInst &I)
{
   visitCallInst(I);
}

void  VerifierPass::visitIndirectCallInst(IndirectCallInst const& I)
{
   auto F = I.getCalledFunction();
   errorIf(!(F->getType()->isFunctionType()), "called value is not a "
      "function", I);

   auto NeededArgs = F->getType()->asFunctionType()->getParamTypes();
   auto GivenArgs = I.getArgs();

   if (!compatibleArgCount(F->getType()->asFunctionType(), GivenArgs)) {
      errorIf(true, "invalid number of arguments for call", I);
      return;
   }

   size_t i = 0;
   for (const auto &needed : NeededArgs) {
      auto &given = GivenArgs[i];
      errorIf(!typesCompatible(needed, given->getType()),
              "invalid argument type for argument " + std::to_string(i), I);

      ++i;
   }
}

void  VerifierPass::visitLambdaCallInst(LambdaCallInst const& I)
{

}

void  VerifierPass::visitInitInst(InitInst const& I)
{
   auto F = I.getCalledFunction();
   errorIf(F->getParent() != I.getParent()->getParent()->getParent(),
           "referencing function in a different module", I, *F);

   auto &NeededArgs = F->getEntryBlock()->getArgs();
   auto GivenArgs = I.getArgs();

   if (!compatibleArgCount(NeededArgs, GivenArgs, F->isCStyleVararg(), true)) {
      errorIf(true, "invalid number of arguments for call", I, *F);
      return;
   }

   size_t i = 0;
   bool first = true;

   for (const auto &needed : NeededArgs) {
      if (first) {
         first = false;
         continue;
      }
      if (needed.isVararg())
         break;

      auto &given = GivenArgs[i];
      errorIf(!typesCompatible(needed.getType(), given->getType()),
              "invalid argument type for argument " + std::to_string(i), I, *F);

      ++i;
   }
}

void VerifierPass::visitUnionInitInst(UnionInitInst const& I)
{

}

void VerifierPass::visitEnumInitInst(EnumInitInst const& I)
{

}

namespace {

bool hasValidBinOpType(Value const* V)
{
   auto ty = V->getType();
   switch (ty->getTypeID()) {
      case Type::BuiltinTypeID:
         return ty->isIntegerType() || ty->isFPType();
      case Type::PointerTypeID:
         return true;
      default:
         return false;
   }
}

} // anonymous namespace

void VerifierPass::visitBinaryOperatorInst(const BinaryOperatorInst &I)
{
   using OP = BinaryOperatorInst::OpCode;

   switch (I.getOpCode()) {
   case OP::Add: case OP::Sub: case OP::Mul: case OP::Div: case OP::Mod:
      errorIf(!hasValidBinOpType(I.getOperand(0)), "invalid operand type", I);
      errorIf(!hasValidBinOpType(I.getOperand(1)), "invalid operand type", I);
      errorIf(!typesCompatible(I.getOperand(0)->getType(),
                               I.getOperand(1)->getType()),
              "operands are not of the same type", I);

      break;
   case OP::Exp:
      errorIf(!hasValidBinOpType(I.getOperand(0)), "invalid operand type", I);
      errorIf(!hasValidBinOpType(I.getOperand(1)), "invalid operand type", I);

      break;
   case OP::And: case OP::Or: case OP::Xor: case OP::Shl: case OP::AShr:
   case OP::LShr:
      errorIf(!I.getOperand(0)->getType()->isIntegerType(),
              "invalid operand type", I);
      errorIf(!I.getOperand(1)->getType()->isIntegerType(),
              "invalid operand type", I);
      errorIf(!typesCompatible(I.getOperand(0)->getType(),
                               I.getOperand(1)->getType()),
              "operands are not of the same type", I);

      break;
   }
}

void VerifierPass::visitCompInst(const CompInst &I)
{
   errorIf(!hasValidBinOpType(I.getOperand(0)), "invalid operand type", I);
   errorIf(!hasValidBinOpType(I.getOperand(1)), "invalid operand type", I);
   errorIf(!typesCompatible(I.getOperand(0)->getType(),
                            I.getOperand(1)->getType()),
           "operands are not of the same type", I);
}

void VerifierPass::visitUnaryOperatorInst(const UnaryOperatorInst &I)
{
   errorIf(!hasValidBinOpType(I.getOperand(0)), "invalid operand type", I);
}

static bool isValidBitCastType(QualType Ty)
{
   return Ty->isPointerType() || Ty->isReferenceType() || Ty->isClass()
          || Ty->isArrayType() || Ty->isMetaType()
          || Ty->isThinFunctionTy();
}

void VerifierPass::visitBitCastInst(BitCastInst const& I)
{
   errorIf(!isValidBitCastType(I.getType())
           || !isValidBitCastType(I.getOperand(0)->getType()),
           "bitcast type must be a pointer type", I);
}

void  VerifierPass::visitIntegerCastInst(IntegerCastInst const& I)
{
   auto from = I.getOperand(0)->getType();
   auto to = I.getType();

   switch (I.getKind()) {
   case CastKind::IntToPtr:
      errorIf(!from->isIntegerType(),"inttoptr operand must be integral", I);
      errorIf(!isValidBitCastType(to), "not a pointer type", I);
      break;
   case CastKind::PtrToInt:
      errorIf(!to->isIntegerType(), "ptrtoint result type must be integral", I);
      errorIf(!isValidBitCastType(from),
              "ptrtoint operand must be pointer", I);
      break;
   case CastKind::IntToFP:
      errorIf(!from->isIntegerType(), "inttofp operand must be integral", I);
      errorIf(!to->isFPType(), "not a floating point type", I);
      break;
   case CastKind::FPToInt:
      errorIf(!to->isIntegerType(), "fptoint result type must be integral", I);
      errorIf(!from->isFPType(), "fptoint operand must be floating", I);
      break;
   case CastKind::Ext:
      errorIf(!to->isIntegerType(), "not an integer type", I);
      errorIf(!from->isIntegerType(), "not an integer type", I);

      errorIf(to->getBitwidth() < from->getBitwidth(), "ext result "
         "bitwidth must be higher or equal", I);

      break;
   case CastKind::Trunc:
      errorIf(!to->isIntegerType(), "not an integer type", I);
      errorIf(!from->isIntegerType(), "not an integer type", I);

      errorIf(to->getBitwidth() > from->getBitwidth(), "trunc result "
         "bitwidth must be lower or equal", I);

      break;
   case CastKind::SignFlip:
      errorIf(!to->isIntegerType(), "not an integer type", I);
      errorIf(!from->isIntegerType(), "not an integer type", I);

      errorIf(to->isUnsigned() == from->isUnsigned(), "same sign on operand "
         "and result types", I);

      errorIf(to->getBitwidth() != from->getBitwidth(),
              "sign cast to different bitwidth", I);

      break;
   default:
      llvm_unreachable("bad integer cast kind");
   }
}

void  VerifierPass::visitFPCastInst(FPCastInst const& I)
{
   auto from = I.getOperand(0)->getType();
   auto to = I.getType();

   switch (I.getKind()) {
      case CastKind::FPExt:
         errorIf(!to->isFPType(), "not a floating type", I);
         errorIf(!from->isFPType(), "not a floating type", I);

         errorIf(to->getPrecision()
                 < from->getPrecision(),
                 "fpext result bitwidth must be higher or equal", I);

         break;
      case CastKind::FPTrunc:
         errorIf(!to->isFPType(), "not a floating type", I);
         errorIf(!from->isFPType(), "not a floating type", I);

         errorIf(to->getPrecision()
                 > from->getPrecision(),
                 "fpext result bitwidth must be higher or equal", I);

         break;
      default:
         llvm_unreachable("bad fp cast kind");
   }
}

void VerifierPass::visitIntToEnumInst(IntToEnumInst const& I)
{
   errorIf(!I.getType()->isRawEnum(), "cannot cast int to enum type with "
      "associated values!", I);
}

void VerifierPass::visitDeinitializeLocalInst(
                                           const il::DeinitializeLocalInst &I) {

}

void VerifierPass::visitDeinitializeTemporaryInst(
                                       const il::DeinitializeTemporaryInst &I) {

}

void VerifierPass::visitUnionCastInst(UnionCastInst const& I)
{

}

void VerifierPass::visitProtoCastInst(ProtoCastInst const& I)
{

}

void VerifierPass::visitExceptionCastInst(ExceptionCastInst const& I)
{

}

void  VerifierPass::visitDynamicCastInst(const DynamicCastInst &I)
{

}

void VerifierPass::visitDebugLocInst(const DebugLocInst &I)
{

}

void VerifierPass::visitDebugLocalInst(const DebugLocalInst &I)
{

}

} // namespace il
} // namespace cdot