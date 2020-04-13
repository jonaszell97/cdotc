#include "cdotc/IL/Instructions.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/Module.h"

using namespace cdot::ast;
using namespace cdot::support;

namespace cdot {
namespace il {

AllocaInst::AllocaInst(ValueType ty, bool IsLet, BasicBlock* parent,
                       unsigned alignment, bool heap)
    : Instruction(AllocaInstID, ty, parent), allocSize(nullptr)
{
   AllocaBits.Heap = heap;
   Alignment = alignment;
   AllocaBits.IsLet = IsLet;

   if (IsLet) {
      type = getASTCtx().getReferenceType(ty);
   }
   else {
      type = getASTCtx().getMutableReferenceType(ty);
   }
}

AllocaInst::AllocaInst(ValueType ty, bool IsLet, BasicBlock* parent,
                       Value* allocSize, unsigned alignment, bool heap)
    : Instruction(AllocaInstID, ty, parent), allocSize(allocSize)
{
   AllocaBits.Heap = heap;
   Alignment = alignment;
   AllocaBits.IsLet = IsLet;

   if (IsLet) {
      type = getASTCtx().getReferenceType(ty);
   }
   else {
      type = getASTCtx().getMutableReferenceType(ty);
   }
}

AllocBoxInst::AllocBoxInst(il::ValueType ty, Function* Deinitializer,
                           il::BasicBlock* parent)
    : Instruction(AllocBoxInstID, ty, parent), Deinitializer(Deinitializer)
{
   setIsLvalue(true);
}

FieldRefInst::FieldRefInst(Value* val, DeclarationName fieldName, bool IsLet,
                           BasicBlock* parent)
    : UnaryInstruction(FieldRefInstID, val, nullptr, parent),
      fieldName(fieldName)
{
   auto Field = val->getType()->getRecord()->getField(fieldName);
   assert(Field && "field does not exist on type!");

   FieldRefBits.IsLet = IsLet;
   Offset = Field->getOffset();
   type = Field->getType();

   if (IsLet) {
      type = getASTCtx().getReferenceType(type);
   }
   else {
      type = getASTCtx().getMutableReferenceType(type);
   }
}

StructDecl* FieldRefInst::getAccessedType() const
{
   return cast<StructDecl>(getOperand(0)->getType()->getRecord());
}

GEPInst::GEPInst(il::Value* val, size_t idx, bool IsLet, il::BasicBlock* parent)
    : BinaryInstruction(
          GEPInstID, val,
          ConstantInt::get(
              ValueType(val->getCtx(), val->getASTCtx().getUIntTy()), idx),
          nullptr, parent),
      Offset(0)
{
   QualType valTy = val->getType()->removeReference();

   assert(valTy->isRecordType() && "struct GEP on non-record type!");
   auto R = cast<StructDecl>(valTy->getRecord());

   auto& TI = getASTCtx().getTargetInfo();

   size_t i = 0;
   for (auto F : R->getFields()) {
      if (i++ == idx) {
         type = F->getType();
         break;
      }

      Offset += TI.getSizeOfType(F->getType());
   }

   GEPBits.IsLet = IsLet;
   if (IsLet) {
      type = getASTCtx().getReferenceType(type->removeReference());
   }
   else {
      type = getASTCtx().getMutableReferenceType(type->removeReference());
   }
}

GEPInst::GEPInst(Value* val, Value* idx, bool IsLet, BasicBlock* parent)
    : BinaryInstruction(GEPInstID, val, idx, nullptr, parent), Offset(0)
{
   QualType valTy = val->getType();
   QualType resultTy;

   if (valTy->isReferenceType()) {
      valTy = valTy->getReferencedType();
   }

   auto& TI = getASTCtx().getTargetInfo();

   if (valTy->isPointerType()) {
      resultTy = valTy->getPointeeType();

      if (auto CI = dyn_cast<ConstantInt>(idx)) {
         Offset += CI->getZExtValue() * TI.getPointerSizeInBytes();
      }
   }
   else if (valTy->isTupleType()) {
      assert(isa<ConstantInt>(idx));
      resultTy = valTy->asTupleType()->getContainedType(
          cast<ConstantInt>(idx)->getZExtValue());

      if (auto CI = dyn_cast<ConstantInt>(idx)) {
         auto Cont = valTy->asTupleType()->getContainedTypes();
         auto Idx = CI->getZExtValue();

         for (unsigned i = 0; i < Idx; ++i) {
            Offset += TI.getSizeOfType(Cont[i]);
         }
      }
   }
   else if (valTy->isArrayType()) {
      resultTy = valTy->asArrayType()->getElementType();

      if (auto CI = dyn_cast<ConstantInt>(idx)) {
         Offset += TI.getSizeOfType(resultTy) * CI->getZExtValue();
      }
   }
   else {
      llvm_unreachable("cannot GEP on given type!");
   }

   GEPBits.IsLet = IsLet;
   if (IsLet) {
      type = ValueType(val->getCtx(), getASTCtx().getReferenceType(resultTy));
   }
   else {
      type = ValueType(val->getCtx(),
                       getASTCtx().getMutableReferenceType(resultTy));
   }
}

TupleExtractInst::TupleExtractInst(Value* val, ConstantInt* idx, bool IsLet,
                                   BasicBlock* parent)
    : GEPInst(val, idx, IsLet, parent)
{
   id = TupleExtractInstID;
}

ConstantInt* TupleExtractInst::getIdx() const
{
   return cast<ConstantInt>(getIndex());
}

EnumRawValueInst::EnumRawValueInst(Value* Val, bool LoadVal, BasicBlock* parent)
    : UnaryInstruction(EnumRawValueInstID, Val, nullptr, parent)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   auto& ASTCtx = getASTCtx();
   type = ValueType(Val->getCtx(), ASTCtx.getIntTy());

   if (!LoadVal) {
      type = ASTCtx.getMutableReferenceType(type);
   }
}

EnumExtractInst::EnumExtractInst(Value* Val, ast::EnumCaseDecl* Case,
                                 ConstantInt* caseVal, bool IsLet,
                                 BasicBlock* parent)
    : UnaryInstruction(EnumExtractInstID, Val, nullptr, parent), Case(Case),
      caseVal(caseVal)
{
   auto rec = Val->getType()->getRecord();
   assert(isa<EnumDecl>(rec) && "can't extract raw value of non-enum");

   setIndirect(Case->isIndirect());

   auto idx = caseVal->getZExtValue();
   auto AssociatedValues = Case->getArgs();
   assert(AssociatedValues.size() > idx && "invalid case index");

   type = AssociatedValues[idx]->getType();
   if (IsLet) {
      type = getASTCtx().getReferenceType(type);
   }
   else {
      type = getASTCtx().getMutableReferenceType(type);
   }
}

DeclarationName EnumExtractInst::getCaseName() const
{
   return Case->getDeclName();
}

EnumDecl* EnumExtractInst::getEnumTy() const
{
   return cast<EnumDecl>(getOperand(0)->getType()->getRecord());
}

CaptureExtractInst::CaptureExtractInst(ConstantInt* idx, BasicBlock* parent)
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

TerminatorInst::TerminatorInst(TypeID id, Context& Ctx, BasicBlock* parent)
    : Instruction(id, ValueType(Ctx, Ctx.getASTCtx().getVoidType()), parent)
{
}

size_t TerminatorInst::getNumSuccessors() const
{
   switch (id) {
#define CDOT_TERM_INST(Name)                                                   \
   case Name##ID:                                                              \
      return cast<Name>(this)->getNumSuccessorsImpl();
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("not a terminator inst!");
   }
}

BasicBlock* TerminatorInst::getSuccessorAt(size_t idx) const
{
   switch (id) {
#define CDOT_TERM_INST(Name)                                                   \
   case Name##ID:                                                              \
      return cast<Name>(this)->getSuccessorAtImpl(idx);
#include "cdotc/IL/Instructions.def"

   default:
      llvm_unreachable("not a terminator inst!");
   }
}

RetInst::RetInst(Value* returnedValue, BasicBlock* parent)
    : TerminatorInst(RetInstID, returnedValue->getCtx(), parent),
      returnedValue(returnedValue)
{
   returnedValue->addUse(this);
}

RetInst::RetInst(Context& Ctx, BasicBlock* parent)
    : TerminatorInst(RetInstID, Ctx, parent), returnedValue(nullptr)
{
}

YieldInst::YieldInst(il::Value* yieldedValue, BasicBlock* ResumeDst,
                     ArrayRef<Value*> ResumeArgs, bool FinalYield,
                     il::BasicBlock* parent)
    : TerminatorInst(YieldInstID, yieldedValue->getCtx(), parent),
      MultiOperandInst(ResumeArgs, ResumeArgs.size() + 2)
{
   yieldedValue->addUse(this);
   if (ResumeDst)
      ResumeDst->addUse(this);

   YieldBits.IsFinalYield = FinalYield;
   Operands[numOperands - 2] = yieldedValue;
   Operands[numOperands - 1] = ResumeDst;
}

YieldInst::YieldInst(il::Context& Ctx, BasicBlock* ResumeDst,
                     ArrayRef<Value*> ResumeArgs, bool FinalYield,
                     il::BasicBlock* parent)
    : TerminatorInst(YieldInstID, Ctx, parent), MultiOperandInst(ResumeArgs)
{
   if (ResumeDst)
      ResumeDst->addUse(this);

   YieldBits.IsFinalYield = FinalYield;
   Operands[numOperands - 2] = nullptr;
   Operands[numOperands - 1] = ResumeDst;
}

BasicBlock* YieldInst::getResumeDst() const
{
   return cast<BasicBlock>(Operands[numOperands - 1]);
}

ThrowInst::ThrowInst(Value* thrownValue, GlobalVariable* typeInfo,
                     BasicBlock* parent)
    : TerminatorInst(ThrowInstID, thrownValue->getCtx(), parent),
      thrownValue(thrownValue), typeInfo(typeInfo)
{
   thrownValue->addUse(this);
   typeInfo->addUse(this);
}

RethrowInst::RethrowInst(Value* thrownValue, BasicBlock* parent)
    : TerminatorInst(RethrowInstID, thrownValue->getCtx(), parent),
      thrownValue(thrownValue)
{
   thrownValue->addUse(this);
}

UnreachableInst::UnreachableInst(Context& Ctx, BasicBlock* parent)
    : TerminatorInst(UnreachableInstID, Ctx, parent)
{
}

CallInst::CallInst(Value* Callee, ArrayRef<Value*> args, BasicBlock* parent)
    : Instruction(
          CallInstID,
          ValueType(Callee->getCtx(),
                    Callee->getType()->asFunctionType()->getReturnType()),
          parent),
      MultiOperandInst(args, args.size() + 1)
{
   Callee->addUse(this);
   for (const auto& arg : args) {
      arg->addUse(this);
   }

   Operands[numOperands - 1] = Callee;
}

CallInst::CallInst(TypeID id, Context& Ctx, llvm::ArrayRef<Value*> args,
                   BasicBlock* parent)
    : Instruction(id, ValueType(Ctx, Ctx.getASTCtx().getVoidType()), parent),
      MultiOperandInst(args, args.size() + 1)
{
   for (const auto& arg : args) {
      arg->addUse(this);
   }
}

CallInst::CallInst(TypeID id, Function* func, llvm::ArrayRef<Value*> args,
                   BasicBlock* parent)
    : CallInst(func, args, parent)
{
   this->id = id;
}

CallInst::CallInst(TypeID id, Value* func, FunctionType* FuncTy,
                   ArrayRef<Value*> args, BasicBlock* parent)
    : Instruction(id, ValueType(func->getCtx(), FuncTy->getReturnType()),
                  parent),
      MultiOperandInst(args, args.size() + 1)
{
   Operands[numOperands - 1] = func;
}

Value* CallInst::getCallee() const { return Operands[numOperands - 1]; }

Function* CallInst::getCalledFunction() const
{
   return support::dyn_cast<Function>(getCallee());
}

Method* CallInst::getCalledMethod() const
{
   return support::dyn_cast_or_null<Method>(getCallee());
}

bool CallInst::isTaggedDeinit() const
{
   if (isTagged())
      return true;

   auto M = getCalledMethod();
   if (!M)
      return false;

   return M->isDeinit() && getArgs().front()->isTagged();
}

llvm::ArrayRef<Value*> CallInst::getArgs() const
{
   return {Operands, numOperands - 1};
}

VirtualCallInst::VirtualCallInst(Value* VTableOwner, FunctionType* FnTy,
                                 il::GlobalVariable* ProtocolTypeInfo,
                                 unsigned Offset, ArrayRef<Value*> args,
                                 BasicBlock* parent)
    : CallInst(VirtualCallInstID, VTableOwner, FnTy, args, parent), FnTy(FnTy),
      ProtocolTypeInfo(ProtocolTypeInfo), Offset(Offset)
{
   if (ProtocolTypeInfo)
      ProtocolTypeInfo->addUse(this);
}

LambdaCallInst::LambdaCallInst(Value* lambda, llvm::ArrayRef<Value*> args,
                               BasicBlock* parent)
    : CallInst(LambdaCallInstID, lambda, lambda->getType()->asFunctionType(),
               args, parent)
{
}

InvokeInst::InvokeInst(Value* Callee, ArrayRef<Value*> args,
                       BasicBlock* NormalContinuation, BasicBlock* LandingPad,
                       BasicBlock* parent)
    : TerminatorInst(InvokeInstID, Callee->getCtx(), parent),
      MultiOperandInst(args, args.size() + 1),
      NormalContinuation(NormalContinuation), LandingPad(LandingPad)
{
   NormalContinuation->addUse(this);
   LandingPad->addUse(this);

   Callee->addUse(this);
   for (const auto& arg : args) {
      arg->addUse(this);
   }

   Operands[numOperands - 1] = Callee;
}

InvokeInst::InvokeInst(TypeID id, Value* F, llvm::ArrayRef<Value*> args,
                       BasicBlock* NormalContinuation, BasicBlock* LandingPad,
                       BasicBlock* parent)
    : InvokeInst(F, args, NormalContinuation, LandingPad, parent)
{
   this->id = id;
}

Value* InvokeInst::getCallee() const { return Operands[numOperands - 1]; }

Function* InvokeInst::getCalledFunction() const
{
   return support::dyn_cast<Function>(Operands[numOperands - 1]);
}

Method* InvokeInst::getCalledMethod() const
{
   return support::dyn_cast<Method>(Operands[numOperands - 1]);
}

llvm::ArrayRef<Value*> InvokeInst::getArgs() const
{
   return {Operands, numOperands - 1};
}

VirtualInvokeInst::VirtualInvokeInst(il::Value* VTableOwner, FunctionType* FnTy,
                                     il::GlobalVariable* ProtocolTypeInfo,
                                     unsigned Offset, ArrayRef<il::Value*> args,
                                     il::BasicBlock* NormalContinuation,
                                     il::BasicBlock* LandingPad,
                                     il::BasicBlock* parent)
    : InvokeInst(VirtualInvokeInstID, VTableOwner, args, NormalContinuation,
                 LandingPad, parent),
      FnTy(FnTy), ProtocolTypeInfo(ProtocolTypeInfo), Offset(Offset)
{
   if (ProtocolTypeInfo)
      ProtocolTypeInfo->addUse(this);
}

IntrinsicCallInst::IntrinsicCallInst(Intrinsic id, ValueType returnType,
                                     llvm::ArrayRef<Value*> args,
                                     BasicBlock* parent)
    : Instruction(IntrinsicCallInstID, returnType, parent),
      MultiOperandInst(args), calledIntrinsic(id)
{
   for (const auto& arg : args) {
      arg->addUse(this);
   }
}

LLVMIntrinsicCallInst::LLVMIntrinsicCallInst(IdentifierInfo* id,
                                             ValueType returnType,
                                             llvm::ArrayRef<Value*> args,
                                             BasicBlock* parent)
    : Instruction(LLVMIntrinsicCallInstID, returnType, parent),
      MultiOperandInst(args), calledIntrinsic(id)
{
   for (const auto& arg : args) {
      arg->addUse(this);
   }
}

BrInst::BrInst(Value* Condition, BasicBlock* IfBranch,
               llvm::ArrayRef<Value*> TargetArgs, BasicBlock* ElseBranch,
               llvm::ArrayRef<Value*> ElseArgs, BasicBlock* parent)
    : TerminatorInst(BrInstID, Condition->getCtx(), parent),
      MultiOperandInst(unsigned(TargetArgs.size()) + unsigned(ElseArgs.size())),
      Condition(Condition), TargetBranch(IfBranch), ElseBranch(ElseBranch),
      numTargetArgs(TargetArgs.size())
{
   Condition->addUse(this);
   IfBranch->addUse(this);
   ElseBranch->addUse(this);

   size_t i = 0;
   for (const auto& arg : TargetArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
   for (const auto& arg : ElseArgs) {
      Operands[i] = arg;
      arg->addUse(this);
      ++i;
   }
}

BrInst::BrInst(BasicBlock* TargetBranch, llvm::ArrayRef<Value*> BlockArgs,
               BasicBlock* parent)
    : TerminatorInst(BrInstID, TargetBranch->getCtx(), parent),
      MultiOperandInst(BlockArgs), Condition(nullptr),
      TargetBranch(TargetBranch), ElseBranch(nullptr),
      numTargetArgs(BlockArgs.size())
{
   TargetBranch->addUse(this);
   for (const auto& arg : BlockArgs) {
      arg->addUse(this);
   }
}

BrInst::BrInst(Context& Ctx, BasicBlock* parent)
    : TerminatorInst(BrInstID, Ctx, parent), MultiOperandInst(0),
      Condition(nullptr), TargetBranch(nullptr), ElseBranch(nullptr),
      numTargetArgs(0)
{
}

llvm::ArrayRef<Value*> BrInst::getTargetArgs() const
{
   return {Operands, numTargetArgs};
}

llvm::ArrayRef<Value*> BrInst::getElseArgs() const
{
   return {Operands + numTargetArgs, numOperands - numTargetArgs};
}

void BrInst::setTargetBranch(BasicBlock* TargetBranch)
{
   TargetBranch->addUse(this);
   BrInst::TargetBranch = TargetBranch;
}

SwitchInst::SwitchInst(Value* SwitchVal, BasicBlock* DefaultDst,
                       BasicBlock* parent)
    : TerminatorInst(SwitchInstID, SwitchVal->getCtx(), parent),
      SwitchVal(SwitchVal), DefaultDst(DefaultDst)
{
   if (DefaultDst)
      DefaultDst->addUse(this);
}

void SwitchInst::setDefault(BasicBlock* BB)
{
   DefaultDst = BB;
   BB->addUse(this);
}

void SwitchInst::addCase(ConstantInt* val, BasicBlock* Dst)
{
   if (val) {
      val->addUse(this);
   }

   Dst->addUse(this);
   Cases.emplace_back(val, Dst);
}

Value* SwitchInst::getSwitchVal() const { return SwitchVal; }

const llvm::SmallVector<SwitchInst::CasePair, 4>& SwitchInst::getCases() const
{
   return Cases;
}

LandingPadInst::LandingPadInst(Context& Ctx, BasicBlock* parent)
    : TerminatorInst(LandingPadInstID, Ctx, parent)
{
   type = ValueType(Ctx, Ctx.getASTCtx().getInt8PtrTy());
}

void LandingPadInst::addCatch(CatchClause&& Clause)
{
   Clause.TargetBB->addUse(this);
   CatchClauses.push_back(std::move(Clause));
}

BinaryInstruction::BinaryInstruction(TypeID id, Value* lhs, Value* rhs,
                                     QualType resultType, BasicBlock* parent)
    : OperatorInst(id, lhs->getCtx(), resultType, parent), Operands{lhs, rhs}
{
   lhs->addUse(this);
   rhs->addUse(this);
}

UnaryInstruction::UnaryInstruction(TypeID id, Value* operand,
                                   QualType resultType, BasicBlock* parent)
    : OperatorInst(id, operand->getCtx(), resultType, parent), Operand(operand)
{
   operand->addUse(this);
}

BinaryOperatorInst::BinaryOperatorInst(OpCode opCode, Value* lhs, Value* rhs,
                                       BasicBlock* parent)
    : BinaryInstruction(BinaryOperatorInstID, lhs, rhs, lhs->getType(), parent),
      opCode(opCode)
{
}

UnaryOperatorInst::UnaryOperatorInst(OpCode opCode, Value* target,
                                     BasicBlock* parent)
    : UnaryInstruction(UnaryOperatorInstID, target, target->getType(), parent),
      opCode(opCode)
{
}

CompInst::CompInst(OpCode opCode, Value* lhs, Value* rhs, BasicBlock* parent)
    : BinaryInstruction(CompInstID, lhs, rhs, lhs->getASTCtx().getBoolTy(),
                        parent),
      opCode(opCode)
{
}

const char* CastNames[]
    = {"bitcast",    "intcast",   "fpcast",     "int_to_enum",
       "union_cast", "dyn_cast",  "proto_cast", "exception_cast",

       "ext",        "trunc",     "inttoptr",   "inttofp",
       "ptrtoint",   "sign_cast", "fptoint",    "box",
       "unbox",      "noop",

       "fpext",      "fptrunc",   "box",        "unbox"};

IntegerCastInst::IntegerCastInst(CastKind kind, Value* target, QualType toType,
                                 BasicBlock* parent)
    : CastInst(IntegerCastInstID, target, toType, parent), kind(kind)
{
}

FPCastInst::FPCastInst(CastKind kind, Value* target, QualType toType,
                       BasicBlock* parent)
    : CastInst(FPCastInstID, target, *toType, parent), kind(kind)
{
}

UnionCastInst::UnionCastInst(Value* target, ast::UnionDecl* UnionTy,
                             const IdentifierInfo* fieldName,
                             BasicBlock* parent)
    : CastInst(UnionCastInstID, target, nullptr, parent), UnionTy(UnionTy),
      fieldName(fieldName)
{
   type = UnionTy->getField(fieldName)->getType();
   type.makeReference();
}

ExistentialInitInst::ExistentialInitInst(Value* target, QualType toType,
                                         il::Value* ValueTypeInfo,
                                         il::GlobalVariable* ProtocolTypeInfo,
                                         bool Preallocated, BasicBlock* parent)
    : Instruction(ExistentialInitInstID, ValueType(target->getCtx(), toType),
                  parent),
      MultiOperandInst({target, ValueTypeInfo, ProtocolTypeInfo})
{
   ExistentialBits.Preallocated = Preallocated;
}

Value* ExistentialInitInst::getValueTypeInfo() const { return Operands[1]; }

GlobalVariable* ExistentialInitInst::getProtocolTypeInfo() const
{
   return cast<GlobalVariable>(Operands[2]);
}

GenericInitInst::GenericInitInst(il::Value* Val, il::Value* GenericEnvironment,
                                 QualType TemplateParamType,
                                 il::BasicBlock* Parent)
    : BinaryInstruction(GenericInitInstID, Val, GenericEnvironment,
                        TemplateParamType, Parent)
{
}

ExceptionCastInst::ExceptionCastInst(Value* target, QualType toType,
                                     BasicBlock* parent)
    : CastInst(ExceptionCastInstID, target, toType, parent)
{
}

BitCastInst::BitCastInst(CastKind kind, Value* target, QualType toType,
                         BasicBlock* parent)
    : CastInst(BitCastInstID, target, toType, parent), kind(kind)
{
}

DynamicCastInst::DynamicCastInst(Value* target, il::Value* TargetTypeInfo,
                                 QualType Type, BasicBlock* parent)
    : CastInst(DynamicCastInstID, target, Type, parent),
      TargetTypeInfo(TargetTypeInfo)
{
   setIsLvalue(true);
}

ExistentialCastInst::ExistentialCastInst(il::Value* Target,
                                         il::Value* TargetTypeInfo,
                                         CastKind Kind, QualType TargetType,
                                         il::BasicBlock* parent)
    : CastInst(ExistentialCastInstID, Target, TargetType, parent),
      TargetTypeInfo(TargetTypeInfo), Kind(Kind)
{
}

StructInitInst::StructInitInst(StructDecl* InitializedType, Method* Init,
                               llvm::ArrayRef<Value*> args, bool Fallible,
                               QualType FallibleTy, BasicBlock* parent)
    : CallInst(StructInitInstID, Init, args, parent),
      InitializedType(InitializedType)
{
   if (FallibleTy)
      type = FallibleTy;
   else
      type = getASTCtx().getRecordType(InitializedType);

   AllocaBits.Heap = isa<ClassDecl>(InitializedType);
   AllocaBits.FallibleInit = Fallible;
}

UnionInitInst::UnionInitInst(UnionDecl* UnionTy, Value* InitializerVal,
                             BasicBlock* parent)
    : UnaryInstruction(UnionInitInstID, InitializerVal,
                       InitializerVal->getASTCtx().getRecordType(UnionTy),
                       parent),
      UnionTy(UnionTy)
{
}

EnumInitInst::EnumInitInst(Context& Ctx, EnumDecl* EnumTy,
                           ast::EnumCaseDecl* Case, llvm::ArrayRef<Value*> args,
                           BasicBlock* parent)
    : Instruction(EnumInitInstID,
                  ValueType(Ctx, Ctx.getASTCtx().getRecordType(EnumTy)),
                  parent),
      MultiOperandInst(args), EnumTy(EnumTy), Case(Case)
{
   setIndirect(Case->isIndirect());
}

DeclarationName EnumInitInst::getCaseName() const
{
   return Case->getDeclName();
}

LambdaInitInst::LambdaInitInst(il::Function* F, QualType LambdaTy,
                               llvm::ArrayRef<Value*> Captures,
                               BasicBlock* parent)
    : Instruction(LambdaInitInstID, ValueType(F->getCtx(), LambdaTy), parent),
      MultiOperandInst(Captures), F(F)
{
}

DeallocInst::DeallocInst(il::Value* V, bool Heap, il::BasicBlock* P)
    : UnaryInstruction(DeallocInstID, V, V->getASTCtx().getVoidType(), P)
{
   AllocaBits.Heap = Heap;
}

DeallocBoxInst::DeallocBoxInst(il::Value* V, il::BasicBlock* P)
    : UnaryInstruction(DeallocBoxInstID, V, V->getASTCtx().getVoidType(), P)
{
}

AssignInst::AssignInst(Value* dst, Value* src, BasicBlock* parent)
    : BinaryInstruction(
          AssignInstID, dst, src,
          ValueType(dst->getCtx(), dst->getASTCtx().getVoidType()), parent)
{
}

StoreInst::StoreInst(Value* dst, Value* src, BasicBlock* parent)
    : BinaryInstruction(
          StoreInstID, dst, src,
          ValueType(dst->getCtx(), dst->getASTCtx().getVoidType()), parent)
{
}

InitInst::InitInst(Value* dst, Value* src, BasicBlock* parent)
    : BinaryInstruction(
          InitInstID, dst, src,
          ValueType(dst->getCtx(), dst->getASTCtx().getVoidType()), parent)
{
}

LoadInst::LoadInst(Value* target, BasicBlock* parent)
    : UnaryInstruction(LoadInstID, target, nullptr, parent)
{
   if (target->isLvalue()) {
      type = target->getType();
   }
   else {
      assert(target->getType()->isPointerType());
      type = ValueType(target->getCtx(), target->getType()->getPointeeType());
   }

   setIsLvalue(false);
}

AddrOfInst::AddrOfInst(Value* target, BasicBlock* parent)
    : UnaryInstruction(AddrOfInstID, target,
                       target->getASTCtx().getPointerType(
                           target->getType()->getReferencedType()),
                       parent)
{
   if (target->getType()->isMutableReferenceType()) {
      type = ValueType(target->getCtx(),
                       target->getASTCtx().getMutablePointerType(
                           target->getType()->getReferencedType()));
   }
   else {
      assert(target->getType()->isReferenceType());
      type = ValueType(target->getCtx(),
                       target->getASTCtx().getPointerType(
                           target->getType()->getReferencedType()));
   }
}

PtrToLvalueInst::PtrToLvalueInst(Value* target, BasicBlock* parent)
    : UnaryInstruction(PtrToLvalueInstID, target,
                       target->getASTCtx().getReferenceType(
                           target->getType()->getPointeeType()),
                       parent)
{
}

RefcountingInst::RefcountingInst(TypeID ID, il::Value* Val,
                                 il::BasicBlock* Parent)
    : UnaryInstruction(ID, Val, Val->getASTCtx().getVoidType(), Parent)
{
}

RetainInst::RetainInst(TypeID ID, il::Value* Val, il::BasicBlock* Parent)
    : RefcountingInst(ID, Val, Parent)
{
}

ReleaseInst::ReleaseInst(TypeID ID, il::Value* Val, il::BasicBlock* Parent)
    : RefcountingInst(ID, Val, Parent)
{
}

StrongRetainInst::StrongRetainInst(il::Value* Val, il::BasicBlock* Parent)
    : RetainInst(StrongRetainInstID, Val, Parent)
{
}

StrongReleaseInst::StrongReleaseInst(il::Value* Val, il::BasicBlock* Parent)
    : ReleaseInst(StrongReleaseInstID, Val, Parent)
{
}

WeakRetainInst::WeakRetainInst(il::Value* Val, il::BasicBlock* Parent)
    : RetainInst(WeakRetainInstID, Val, Parent)
{
}

WeakReleaseInst::WeakReleaseInst(il::Value* Val, il::BasicBlock* Parent)
    : ReleaseInst(WeakReleaseInstID, Val, Parent)
{
}

MoveInst::MoveInst(il::Value* Target, il::BasicBlock* Parent)
    : UnaryInstruction(MoveInstID, Target, Target->getType(), Parent)
{
}

BeginBorrowInst::BeginBorrowInst(il::Value* Target, SourceLocation BeginLoc,
                                 SourceLocation EndLoc, bool IsMutableBorrow,
                                 il::BasicBlock* Parent)
    : UnaryInstruction(BeginBorrowInstID, Target, Target->getType(), Parent),
      BeginLoc(BeginLoc), EndLoc(EndLoc)
{
   assert(Target->isLvalue() && "cannot borrow rvalue!");
   BorrowBits.IsMutableBorrow = IsMutableBorrow;
}

EndBorrowInst::EndBorrowInst(il::Value* Target, SourceLocation Loc,
                             bool IsMutableBorrow, il::BasicBlock* Parent)
    : UnaryInstruction(
          EndBorrowInstID, Target,
          ValueType(Target->getCtx(), Target->getASTCtx().getVoidType()),
          Parent),
      Loc(Loc)
{
   BorrowBits.IsMutableBorrow = IsMutableBorrow;
}

DebugLocInst::DebugLocInst(SourceLocation Loc, unsigned Line, unsigned Col,
                           unsigned FileID, ValueType VoidTy,
                           il::BasicBlock* Parent)
    : Instruction(DebugLocInstID, VoidTy, Parent), Loc(Loc), Line(Line),
      Col(Col), FileID(FileID)
{
}

DebugLocalInst::DebugLocalInst(const IdentifierInfo* Name, Value* V,
                               BasicBlock* Parent)
    : Instruction(DebugLocalInstID,
                  ValueType(V->getCtx(), V->getASTCtx().getVoidType()), Parent),
      Name(Name), Val(V)
{
}

} // namespace il
} // namespace cdot