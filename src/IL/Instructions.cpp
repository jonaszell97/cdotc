//
// Created by Jonas Zell on 09.04.18.
//

#include "Instructions.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Constants.h"
#include "Context.h"
#include "Function.h"
#include "Module.h"

using namespace cdot::ast;
using namespace cdot::support;

namespace cdot {
namespace il {

AllocaInst::AllocaInst(ValueType ty,
                       BasicBlock *parent,
                       unsigned alignment,
                       bool heap)
   : Instruction(AllocaInstID, ty, parent)
{
   setIsLvalue(true);
   AllocaBits.Heap = heap;
   AllocaBits.Alignment = alignment;
}

AllocaInst::AllocaInst(ValueType ty,
                       BasicBlock *parent,
                       size_t allocSize,
                       unsigned alignment,
                       bool heap)
   : Instruction(AllocaInstID, ty, parent), allocSize(allocSize)
{
   setIsLvalue(true);
   AllocaBits.Heap = heap;
   AllocaBits.Alignment = alignment;
}

AllocBoxInst::AllocBoxInst(il::ValueType ty,
                           Function *Deinitializer,
                           il::BasicBlock *parent)
   : Instruction(AllocBoxInstID, ty, parent),
     Deinitializer(Deinitializer)
{
   setIsLvalue(true);
}

FieldRefInst::FieldRefInst(Value *val,
                           DeclarationName fieldName,
                           BasicBlock *parent)
   : UnaryInstruction(FieldRefInstID, val, nullptr, parent),
     fieldName(fieldName)
{
   auto Field = val->getType()->getRecord()->getField(fieldName);
   assert(Field && "field does not exist on type!");

   type = Field->getType();
   setIsLvalue(true);
}

StructDecl* FieldRefInst::getAccessedType() const
{
   return cast<StructDecl>(getOperand(0)->getType()->getRecord());
}

GEPInst::GEPInst(il::Value *val, size_t idx, il::BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val,
                       ConstantInt::get(ValueType(val->getCtx(),
                                                  val->getASTCtx()
                                                     .getUIntTy()),
                                        idx), nullptr, parent)
{
   val->addUse(this);

   QualType valTy = val->getType()->stripReference();

   assert(valTy->isRecordType() && "struct GEP on non-record type!");
   auto R = cast<StructDecl>(valTy->getRecord());

   size_t i = 0;
   for (auto F : R->getFields()) {
      if (i++ == idx) {
         type = F->getType();
         break;
      }
   }

   type.makeReference();
}

GEPInst::GEPInst(Value *val, Value *idx, BasicBlock *parent)
   : BinaryInstruction(GEPInstID, val, idx, nullptr, parent)
{
   val->addUse(this);

   QualType valTy = val->getType();
   QualType resultTy;

   if (valTy->isReferenceType()) {
      valTy = valTy->getReferencedType();
   }

   if (valTy->isPointerType()) {
      resultTy = valTy->getPointeeType();
   }
   else if (valTy->isTupleType()) {
      assert(isa<ConstantInt>(idx));
      resultTy = valTy->asTupleType()
                      ->getContainedType(cast<ConstantInt>(idx)->getZExtValue());
   }
   else if (valTy->isArrayType()) {
      resultTy = valTy->asArrayType()->getElementType();
   }
   else {
      llvm_unreachable("cannot GEP on given type!");
   }

   type = ValueType(val->getCtx(), resultTy);
   type.makeReference();
}

TupleExtractInst::TupleExtractInst(Value *val, ConstantInt *idx,
                                   BasicBlock *parent)
   : GEPInst(val, idx, parent)
{
   id = TupleExtractInstID;
}

ConstantInt* TupleExtractInst::getIdx() const
{
   return cast<ConstantInt>(getIndex());
}

EnumRawValueInst::EnumRawValueInst(Value *Val,
                                   BasicBlock *parent)
   : UnaryInstruction(EnumRawValueInstID, Val, nullptr, parent)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   type = ValueType(Val->getCtx(), cast<EnumDecl>(rec)->getRawType());
}

EnumExtractInst::EnumExtractInst(Value *Val, const IdentifierInfo *caseName,
                                 ConstantInt *caseVal, BasicBlock *parent)
   : UnaryInstruction(EnumExtractInstID, Val, nullptr, parent),
     caseName(caseName), caseVal(caseVal)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   auto E = cast<EnumDecl>(Val->getType()->getRecord());
   auto Case = E->hasCase(caseName);

   auto idx = caseVal->getZExtValue();
   auto AssociatedValues = Case->getArgs();
   assert(AssociatedValues.size() > idx && "invalid case index");

   type = AssociatedValues[idx]->getType();

   setIsLvalue(true);
}

EnumDecl* EnumExtractInst::getEnumTy() const
{
   return cast<EnumDecl>(getOperand(0)->getType()->getRecord());
}

CaptureExtractInst::CaptureExtractInst(ConstantInt *idx, BasicBlock *parent)
   : UnaryInstruction(CaptureExtractInstID, idx, nullptr, parent)
{
   auto F = dyn_cast<Lambda>(parent->getParent());
   assert(F && "cannot extract capture in non-lambda func");
   assert(F->getCaptures().size() > idx->getZExtValue()
          && "invalid capture idx");

   type = ValueType(idx->getCtx(), F->getCaptures()[idx->getZExtValue()]);
}

ConstantInt* CaptureExtractInst::getIdx() const
{
   return cast<ConstantInt>(Operand);
}

TerminatorInst::TerminatorInst(TypeID id, Context &Ctx, BasicBlock *parent)
   : Instruction(id, ValueType(Ctx, Ctx.getASTCtx().getVoidType()), parent)
{

}

size_t TerminatorInst::getNumSuccessors() const
{
   switch (id) {
#     define CDOT_TERM_INST(Name)                              \
         case Name##ID:                                        \
            return cast<Name>(this)->getNumSuccessorsImpl();
#     include "Instructions.def"

   default:
      llvm_unreachable("not a terminator inst!");
   }
}

BasicBlock* TerminatorInst::getSuccessorAt(size_t idx) const
{
   switch (id) {
#     define CDOT_TERM_INST(Name)                              \
         case Name##ID:                                        \
            return cast<Name>(this)->getSuccessorAtImpl(idx);
#     include "Instructions.def"

   default:
      llvm_unreachable("not a terminator inst!");
   }
}

RetInst::RetInst(Value *returnedValue,
                 BasicBlock *parent)
   : TerminatorInst(RetInstID, returnedValue->getCtx(), parent),
     returnedValue(returnedValue)
{
   returnedValue->addUse(this);
}

RetInst::RetInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(RetInstID, Ctx, parent),
     returnedValue(nullptr)
{

}

bool RetInst::isVoidReturn() const
{
   return returnedValue == nullptr;
}

Value *RetInst::getReturnedValue() const
{
   return returnedValue;
}

ThrowInst::ThrowInst(Value *thrownValue,
                     GlobalVariable *typeInfo,
                     BasicBlock *parent)
   : TerminatorInst(ThrowInstID, thrownValue->getCtx(), parent),
     thrownValue(thrownValue), typeInfo(typeInfo)
{
   thrownValue->addUse(this);
}

UnreachableInst::UnreachableInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(UnreachableInstID, Ctx, parent)
{

}

CallInst::CallInst(Function *func, llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : Instruction(CallInstID, ValueType(func->getCtx(), func->getReturnType()),
                 parent),
     MultiOperandInst(args),
     calledFunction(func)
{
   func->addUse(this);
   for (const auto &arg : args) {
      arg->addUse(this);
   }
}

CallInst::CallInst(TypeID id, Context &Ctx,
                   llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : Instruction(id, ValueType(Ctx, Ctx.getASTCtx().getVoidType()), parent),
     MultiOperandInst(args)
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
   : Instruction(id, ValueType(func->getCtx(),
                               func->getType()->asFunctionType()
                                   ->getReturnType()),
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

LambdaCallInst::LambdaCallInst(Value *lambda,
                               llvm::ArrayRef<Value *> args,
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
   : TerminatorInst(InvokeInstID, func->getCtx(), parent),
     MultiOperandInst(args),
     calledFunction(func), NormalContinuation(NormalContinuation),
     LandingPad(LandingPad)
{
   NormalContinuation->addUse(this);
   LandingPad->addUse(this);

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

llvm::ArrayRef<Value*> InvokeInst::getArgs() const
{
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
                                     ValueType returnType,
                                     llvm::ArrayRef<Value *> args,
                                     BasicBlock *parent)
   : Instruction(IntrinsicCallInstID, returnType, parent),
     MultiOperandInst(args),
     calledIntrinsic(id)
{

}

BrInst::BrInst(Value *Condition,
               BasicBlock *IfBranch,
               llvm::ArrayRef<Value*> TargetArgs,
               BasicBlock *ElseBranch,
               llvm::ArrayRef<Value*> ElseArgs,
               BasicBlock *parent)
   : TerminatorInst(BrInstID, Condition->getCtx(), parent),
     MultiOperandInst(unsigned(TargetArgs.size()) + unsigned(ElseArgs.size())),
     Condition(Condition),
     TargetBranch(IfBranch),
     ElseBranch(ElseBranch),
     numTargetArgs(TargetArgs.size())
{
   Condition->addUse(this);
   IfBranch->addUse(this);
   ElseBranch->addUse(this);

   size_t i = 0;
   for (const auto &arg : TargetArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
   for (const auto &arg : ElseArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
}

BrInst::BrInst(BasicBlock *TargetBranch,
               llvm::ArrayRef<Value*> BlockArgs,
               BasicBlock *parent)
   : TerminatorInst(BrInstID, TargetBranch->getCtx(), parent),
     MultiOperandInst(BlockArgs),
     Condition(nullptr),
     TargetBranch(TargetBranch),
     ElseBranch(nullptr),
     numTargetArgs(BlockArgs.size())
{
   TargetBranch->addUse(this);
   for (const auto &arg : BlockArgs) {
      arg->addUse(this);
   }
}

BrInst::BrInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(BrInstID, Ctx, parent),
     MultiOperandInst(0),
     Condition(nullptr), TargetBranch(nullptr),
     ElseBranch(nullptr), numTargetArgs(0)
{

}

llvm::ArrayRef<Value*> BrInst::getTargetArgs() const
{
   return { Operands, numTargetArgs };
}

llvm::ArrayRef<Value*> BrInst::getElseArgs() const
{
   return { Operands + numTargetArgs, numOperands - numTargetArgs };
}

void BrInst::setTargetBranch(BasicBlock *TargetBranch)
{
   TargetBranch->addUse(this);
   BrInst::TargetBranch = TargetBranch;
}

SwitchInst::SwitchInst(Value *SwitchVal,
                       BasicBlock *DefaultDst,
                       BasicBlock *parent)
   : TerminatorInst(SwitchInstID, SwitchVal->getCtx(), parent),
     SwitchVal(SwitchVal), DefaultDst(DefaultDst)
{
   if (DefaultDst)
      DefaultDst->addUse(this);
}

void SwitchInst::setDefault(BasicBlock *BB)
{
   DefaultDst = BB;
   BB->addUse(this);
}

void SwitchInst::addCase(ConstantInt *val, BasicBlock *Dst)
{
   if (val) {
      val->addUse(this);
   }

   Dst->addUse(this);
   Cases.emplace_back(val, Dst);
}

Value *SwitchInst::getSwitchVal() const
{
   return SwitchVal;
}

const llvm::SmallVector<SwitchInst::CasePair, 4> &SwitchInst::getCases() const
{
   return Cases;
}

LandingPadInst::LandingPadInst(Context &Ctx, BasicBlock *parent)
   : TerminatorInst(LandingPadInstID, Ctx, parent)
{
   type = ValueType(Ctx, Ctx.getASTCtx().getInt8PtrTy());
}

void LandingPadInst::addCatch(CatchClause &&Clause)
{
   Clause.TargetBB->addUse(this);
   CatchClauses.push_back(std::move(Clause));
}

BinaryInstruction::BinaryInstruction(TypeID id, Value *lhs, Value *rhs,
                                     QualType resultType, BasicBlock *parent)
   : OperatorInst(id, lhs->getCtx(), resultType, parent),
     Operands{ lhs, rhs }
{
   lhs->addUse(this);
   rhs->addUse(this);
}

UnaryInstruction::UnaryInstruction(TypeID id,
                                   Value *operand,
                                   QualType resultType,
                                   BasicBlock *parent)
   : OperatorInst(id, operand->getCtx(), resultType, parent),
     Operand(operand)
{
   operand->addUse(this);
}

BinaryOperatorInst::BinaryOperatorInst(OpCode opCode, Value *lhs,
                                       Value *rhs, BasicBlock *parent)
   : BinaryInstruction(BinaryOperatorInstID, lhs, rhs, lhs->getType(),
                       parent),
     opCode(opCode)
{

}

UnaryOperatorInst::UnaryOperatorInst(OpCode opCode,
                                     Value *target,
                                     BasicBlock *parent)
   : UnaryInstruction(UnaryOperatorInstID, target, target->getType(), parent),
     opCode(opCode)
{

}

CompInst::CompInst(OpCode opCode,
                   Value *lhs, Value *rhs,
                   BasicBlock *parent)
   : BinaryInstruction(CompInstID, lhs, rhs, lhs->getASTCtx().getBoolTy(),
                       parent),
     opCode(opCode)
{

}

const char* CastNames[] = {
   "bitcast", "intcast", "fpcast", "int_to_enum", "union_cast", "dyn_cast",
   "proto_cast", "exception_cast",

   "ext", "trunc", "inttoptr", "inttofp", "ptrtoint", "sign_cast", "fptoint",
   "box", "unbox", "noop",

   "fpext", "fptrunc", "box", "unbox"
};

IntegerCastInst::IntegerCastInst(CastKind kind,
                                 Value *target,
                                 QualType toType,
                                 BasicBlock *parent)
   : CastInst(IntegerCastInstID, target, toType, parent), kind(kind)
{

}

FPCastInst::FPCastInst(CastKind kind,
                       Value *target,
                       QualType toType,
                       BasicBlock *parent)
   : CastInst(FPCastInstID, target, *toType, parent), kind(kind)
{

}

UnionCastInst::UnionCastInst(Value *target, ast::UnionDecl *UnionTy,
                             const IdentifierInfo *fieldName,
                             BasicBlock *parent)
   : CastInst(UnionCastInstID, target, nullptr, parent),
     UnionTy(UnionTy), fieldName(fieldName)
{
   type = UnionTy->getField(fieldName)->getType();
   type.makeReference();
}

ProtoCastInst::ProtoCastInst(Value *target, QualType toType, BasicBlock *parent)
   : CastInst(ProtoCastInstID, target, toType, parent)
{

}

bool ProtoCastInst::isWrap() const
{
   return type->isProtocol();
}

ExceptionCastInst::ExceptionCastInst(Value *target, QualType toType,
                                     BasicBlock *parent)
   : CastInst(ExceptionCastInstID, target, toType, parent)
{

}

BitCastInst::BitCastInst(CastKind kind, Value *target, QualType toType,
                         BasicBlock *parent)
   : CastInst(BitCastInstID, target, toType, parent), kind(kind)
{

}

DynamicCastInst::DynamicCastInst(Value *target,
                                 ast::ClassDecl *TargetTy,
                                 QualType Type,
                                 BasicBlock *parent)
   : CastInst(DynamicCastInstID, target, Type, parent),
     TargetTy(TargetTy)
{
   setIsLvalue(true);
}

InitInst::InitInst(StructDecl *InitializedType,
                   Method *Init,
                   llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : CallInst(InitInstID, Init, args, parent),
     InitializedType(InitializedType)
{
   type = getASTCtx().getRecordType(InitializedType);
}

UnionInitInst::UnionInitInst(UnionDecl *UnionTy,
                             Value *InitializerVal,
                             BasicBlock *parent)
   : CallInst(UnionInitInstID, InitializerVal->getCtx(), { InitializerVal },
              parent),
     UnionTy(UnionTy)
{
   type = getASTCtx().getRecordType(UnionTy);
}

EnumInitInst::EnumInitInst(Context &Ctx,
                           EnumDecl *EnumTy,
                           const IdentifierInfo *caseName,
                           llvm::ArrayRef<Value *> args,
                           BasicBlock *parent)
   : CallInst(EnumInitInstID, Ctx, args, parent),
     EnumTy(EnumTy), caseName(caseName)
{
   type = getASTCtx().getRecordType(EnumTy);
}

LambdaInitInst::LambdaInitInst(il::Function *F,
                               QualType LambdaTy,
                               llvm::ArrayRef<Value*> Captures,
                               BasicBlock *parent)
   : Instruction(LambdaInitInstID, ValueType(F->getCtx(), LambdaTy), parent),
     MultiOperandInst(Captures),
     F(F)
{

}

DeallocInst::DeallocInst(il::Value *V, bool Heap, il::BasicBlock *P)
   : Instruction(DeallocInstID,
                 ValueType(V->getCtx(), V->getASTCtx().getVoidType()), P),
     V(V)
{
   AllocaBits.Heap = Heap;
}

DeallocBoxInst::DeallocBoxInst(il::Value *V, il::BasicBlock *P)
   : Instruction(DeallocBoxInstID,
                 ValueType(V->getCtx(), V->getASTCtx().getVoidType()), P),
     V(V)
{

}

DeinitializeLocalInst::DeinitializeLocalInst(il::Value *RefcountedVal,
                                             il::BasicBlock *Parent)
   : Instruction(DeinitializeLocalInstID,
                 ValueType(RefcountedVal->getCtx(),
                           RefcountedVal->getASTCtx().getVoidType()), Parent),
     Val(RefcountedVal), Deinitializer(nullptr)
{
   assert(RefcountedVal->isLvalue());
}

DeinitializeLocalInst::DeinitializeLocalInst(il::Function *DeinitFn,
                                             il::Value *ValueToDeinit,
                                             il::BasicBlock *Parent)
   : Instruction(DeinitializeLocalInstID,
                 ValueType(ValueToDeinit->getCtx(),
                           ValueToDeinit->getASTCtx().getVoidType()), Parent),
     Val(ValueToDeinit), Deinitializer(DeinitFn)
{
   assert(ValueToDeinit->isLvalue());
}

DeinitializeTemporaryInst::DeinitializeTemporaryInst(il::Value *RefcountedVal,
                                                     il::BasicBlock *Parent)
   : Instruction(DeinitializeTemporaryInstID,
                 ValueType(RefcountedVal->getCtx(),
                           RefcountedVal->getASTCtx().getVoidType()), Parent),
     Val(RefcountedVal), Deinitializer(nullptr)
{

}

DeinitializeTemporaryInst::DeinitializeTemporaryInst(il::Function *DeinitFn,
                                                     il::Value *ValueToDeinit,
                                                     il::BasicBlock *Parent)
   : Instruction(DeinitializeTemporaryInstID,
                 ValueType(ValueToDeinit->getCtx(),
                           ValueToDeinit->getASTCtx().getVoidType()), Parent),
     Val(ValueToDeinit), Deinitializer(DeinitFn)
{

}

StoreInst::StoreInst(Value *dst, Value *src, BasicBlock *parent)
   : BinaryInstruction(StoreInstID, dst, src, nullptr, parent)
{

}

bool StoreInst::useMemCpy() const
{
   return type->needsMemCpy();
}

LoadInst::LoadInst(Value *target,
                   BasicBlock *parent)
   : UnaryInstruction(LoadInstID, target, nullptr, parent)
{
   if (target->isLvalue()) {
      type = target->getType();
   }
   else {
      assert(target->getType()->isPointerType());
      type = ValueType(target->getCtx(),
                       target->getType()->getPointeeType());
   }

   setIsLvalue(false);
}

AddrOfInst::AddrOfInst(Value *target,
                       BasicBlock *parent)
   : UnaryInstruction(AddrOfInstID, target,
                      target->getASTCtx().getPointerType(
                         target->getType()->getReferencedType()),
                      parent)
{

}

PtrToLvalueInst::PtrToLvalueInst(Value *target, BasicBlock *parent)
   : UnaryInstruction(PtrToLvalueInstID, target,
                      target->getASTCtx().getReferenceType(
                         target->getType()->getPointeeType()),
                      parent)
{

}

DebugLocInst::DebugLocInst(unsigned Line,
                           unsigned Col,
                           unsigned FileID,
                           ValueType VoidTy,
                           il::BasicBlock *Parent)
   : Instruction(DebugLocInstID, VoidTy, Parent),
     Line(Line), Col(Col), FileID(FileID)
{

}

DebugLocalInst::DebugLocalInst(const IdentifierInfo *Name,
                               Value *V,
                               BasicBlock *Parent)
   : Instruction(DebugLocalInstID, ValueType(V->getCtx(),
                                             V->getASTCtx().getVoidType()),
                 Parent),
     Name(Name), Val(V)
{

}

} // namespace il
} // namespace cdot