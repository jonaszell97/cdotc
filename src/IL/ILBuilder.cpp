//
// Created by Jonas Zell on 16.11.17.
//

#include "ILBuilder.h"
#include "Module/Module.h"
#include "Value/Record/AggregateType.h"

#include "../AST/Passes/SemanticAnalysis/Function.h"
#include "../AST/Passes/SemanticAnalysis/Record/Record.h"

#include "../Variant/Type/PointerType.h"

#define CDOT_VALUE_INCLUDE
#include "Value/ValueIncludes.def"
#include "Module/Context.h"
#include "../Variant/Type/IntegerType.h"

using std::string;

namespace cdot {
namespace il {

ILBuilder::ILBuilder(Context &Ctx) : Ctx(Ctx), M(nullptr)
{

}

ILBuilder::ILBuilder(Module *M) : Ctx(M->getContext()), M(M)
{

}

ILBuilder::~ILBuilder()
{
   Value::cleanup();
}

void ILBuilder::importType(Type *Ty)
{
   if (Ty->isPointerTy())
      importType(*Ty->asPointerTy()->getPointeeType());

   if (!Ty->isObjectTy())
      return;

   auto &Ctx = M->getContext();
   if (auto AggrTy = Ctx.hasType(Ty->getClassName())) {
      M->addTypeReference(AggrTy);
   }
}

BasicBlock* ILBuilder::getInsertBlock()
{
   return InsertBlock;
}

void ILBuilder::SetInsertPoint(iterator it)
{
   insertPoint = it;
   InsertBlock = it->getParent();
}

void ILBuilder::SetInsertPoint(BasicBlock *bb)
{
   InsertBlock = bb;

   if (bb) {
      insertPoint = bb->getInstructions().end();
   }
}

void ILBuilder::insertInstruction(Instruction *inst,
                                  const string &name) {
   auto BB = getInsertBlock();

   insertPoint = BB->getInstructions().insert(insertPoint, inst);
   ++insertPoint;

   if (!name.empty())
      inst->setName(name);

   if (debugLoc)
      inst->setLocation(debugLoc);

   importType(*inst->getType());
}

BasicBlock* ILBuilder::CreateBasicBlock(const string &name)
{
   auto BB = new BasicBlock(InsertBlock->getParent());

   if (!name.empty())
      BB->setName(name);

   return BB;
}

BasicBlock* ILBuilder::CreateBasicBlock(Function *func,
                                        bool setInsertPoint,
                                        const std::string &name) {
   auto BB = new BasicBlock(func);
   if (!name.empty())
      BB->setName(name);

   if (setInsertPoint)
      SetInsertPoint(BB);

   return BB;
}

ConstantInt* ILBuilder::CreateConstantInt(Type *ty, uint64_t value)
{
   return ConstantInt::get(ty, value);
}

ConstantInt* ILBuilder::CreateTrue()
{
   return ConstantInt::get(IntegerType::getBoolTy(), 1);
}

ConstantInt* ILBuilder::CreateFalse()
{
   return ConstantInt::get(IntegerType::getBoolTy(), 0);
}

ConstantInt* ILBuilder::CreateChar(char c)
{
   return ConstantInt::get(IntegerType::getCharTy(), c);
}

ConstantFloat* ILBuilder::CreateConstantFP(Type *ty, double d)
{
   if (ty->isFloatTy())
      return ConstantFloat::get((float)d);

   assert(ty->isDoubleTy());
   return ConstantFloat::get(d);
}

ConstantFloat* ILBuilder::CreateConstantFloat(float f)
{
   return ConstantFloat::get(f);
}

ConstantFloat* ILBuilder::CreateConstantDouble(double d)
{
   return ConstantFloat::get(d);
}

ConstantString* ILBuilder::CreateConstantString(const std::string &str)
{
   return ConstantString::get(str);
}

ConstantStruct* ILBuilder::CreateConstantStruct(
   AggregateType *ty, llvm::ArrayRef<Constant *> elements)
{
   return ConstantStruct::get(ty, elements);
}

ConstantArray * ILBuilder::CreateConstantArray(llvm::ArrayRef<Constant *> Arr)
{
   return ConstantArray::get(Arr);
}

ConstantArray * ILBuilder::CreateConstantArray(
   llvm::SmallVector<Constant*, 4> &&Arr)
{
   return ConstantArray::get(std::move(Arr));
}

ConstantArray* ILBuilder::CreateConstantArray(Type *ty, size_t numElements)
{
   importType(ty);
   return ConstantArray::get(ty, numElements);
}

Argument* ILBuilder::CreateArgument(QualType type, bool vararg,
                                    BasicBlock *parent,
                                    const string &name,
                                    const SourceLocation &loc) {
   importType(*type);
   auto arg = new Argument(type, vararg, parent, name);

   if (loc)
      arg->setLocation(loc);

   return arg;
}

ClassType* ILBuilder::DeclareClass(const std::string &name,
                                   llvm::StringRef parentClass,
                                   const SourceLocation &loc) {
   auto CT = new ClassType(name, parentClass, M);

   if (loc)
      CT->setLocation(loc);

   return CT;
}

StructType* ILBuilder::DeclareStruct(const std::string &name,
                                     const SourceLocation &loc) {
   auto ST = new StructType(name, M);
   if (loc)
      ST->setLocation(loc);

   return ST;
}

EnumType* ILBuilder::DeclareEnum(const std::string &name, Type *rawType,
                                 const SourceLocation &loc) {
   auto ET = new EnumType(name, rawType, M);
   if (loc)
      ET->setLocation(loc);

   return ET;
}

UnionType* ILBuilder::DeclareUnion(const std::string &name,
                                   const SourceLocation &loc) {
   auto UT = new UnionType(name, M);
   if (loc)
      UT->setLocation(loc);

   return UT;
}

ProtocolType* ILBuilder::DeclareProtocol(const std::string &name,
                                         const SourceLocation &loc) {
   auto PT = new ProtocolType(name, M);
   if (loc)
      PT->setLocation(loc);

   return PT;
}

Function* ILBuilder::CreateFunction(const std::string &name,
                                    QualType returnType,
                                    llvm::ArrayRef<Argument*> args,
                                    bool mightThrow,
                                    bool isExternC,
                                    const SourceLocation &loc) {
   importType(*returnType);
   auto F = new Function(name, returnType, args, M, mightThrow, isExternC);
   if (loc)
      F->setLocation(loc);

   return F;
}

Lambda* ILBuilder::CreateLambda(QualType returnType,
                                llvm::ArrayRef<Argument *> args,
                                bool mightThrow, const SourceLocation &loc) {
   importType(*returnType);
   auto L = new Lambda(returnType, args, M, mightThrow);
   if (loc)
      L->setLocation(loc);

   return L;
}

Method * ILBuilder::CreateMethod(AggregateType *forType,
                                 const std::string &methodName,
                                 QualType returnType,
                                 llvm::ArrayRef<Argument *> args,
                                 bool isStatic,
                                 bool isVirtual,
                                 bool isProperty,
                                 bool isOperator,
                                 bool isConversionOp,
                                 bool mightThrow,
                                 const SourceLocation &loc) {
   importType(*returnType);
   auto Me = new Method(methodName, returnType, args, forType, isStatic,
                        isVirtual, isProperty, isOperator, isConversionOp,
                        M, mightThrow);

   if (loc)
      Me->setLocation(loc);

   return Me;
}

Initializer* ILBuilder::CreateInitializer(AggregateType *forType,
                                          const std::string &methodName,
                                          llvm::ArrayRef<Argument *> args,
                                          bool mightThrow,
                                          const SourceLocation &loc) {
   auto I = new Initializer(methodName, args, forType, M, mightThrow);
   if (loc)
      I->setLocation(loc);

   return I;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Type *type, bool isConst,
                                                Constant *initializer,
                                                const std::string &name,
                                                const SourceLocation &loc) {
   importType(type);
   auto G = new GlobalVariable(type, isConst, name, M, initializer);
   if (loc)
      G->setLocation(loc);

   return G;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Constant *initializer,
                                                bool isConst,
                                                const string &name,
                                                const SourceLocation &loc) {
   importType(*initializer->getType());
   auto G = new GlobalVariable(*initializer->getType(), isConst, name, M,
                               initializer);

   if (loc)
      G->setLocation(loc);

   return G;
}

CallInst* ILBuilder::CreateCall(Function *F,
                                llvm::ArrayRef<Value *> args,
                                const string &name) {
   CallInst *inst = new CallInst(F, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ProtocolCallInst* ILBuilder::CreateProtocolCall(Method *M,
                                                llvm::ArrayRef<Value *> args,
                                                const std::string &name) {
   auto inst = new ProtocolCallInst(M, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

VirtualCallInst* ILBuilder::CreateVirtualCall(Method *M,
                                              llvm::ArrayRef<Value *> args,
                                              const std::string &name) {
   auto inst = new VirtualCallInst(M, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

IntrinsicCallInst* ILBuilder::CreateIntrinsic(Intrinsic id,
                                              llvm::ArrayRef<Value *> args,
                                              const string &name) {
   auto inst = new IntrinsicCallInst(id, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

IndirectCallInst* ILBuilder::CreateIndirectCall(Value *Func,
                                                llvm::ArrayRef<Value *> args,
                                                const std::string &name) {
   auto inst = new IndirectCallInst(Func, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LambdaCallInst* ILBuilder::CreateLambdaCall(Value *Func,
                                            llvm::ArrayRef<Value *> args,
                                            const std::string &name) {
   auto inst = new LambdaCallInst(Func, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

InvokeInst* ILBuilder::CreateInvoke(Function *F, llvm::ArrayRef<Value *> args,
                                    BasicBlock *NormalCont,
                                    BasicBlock *LandingPad, const string &name) {
   auto inst = new InvokeInst(F, args, NormalCont, LandingPad,
                              getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ProtocolInvokeInst* ILBuilder::CreateProtocolInvoke(Method *M,
                                                    llvm::ArrayRef<Value*> args,
                                                    BasicBlock *NormalCont,
                                                    BasicBlock *LandingPad,
                                                    const std::string &name) {
   auto inst = new ProtocolInvokeInst(M, args, NormalCont, LandingPad,
                                      getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

VirtualInvokeInst* ILBuilder::CreateVirtualInvoke(Method *M,
                                                  llvm::ArrayRef<Value *> args,
                                                  BasicBlock *NormalCont,
                                                  BasicBlock *LandingPad,
                                                  const std::string &name) {
   auto inst = new VirtualInvokeInst(M, args, NormalCont, LandingPad,
                                     getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(Type *ofType,
                                    unsigned int align,
                                    bool heap,
                                    const std::string &name) {
   auto inst = new AllocaInst(ofType, getInsertBlock(), align, heap);
   insertInstruction(inst, name);

   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(Type *ofType,
                                    size_t size,
                                    unsigned int align,
                                    bool heap,
                                    const std::string &name) {
   auto inst = new AllocaInst(ofType, getInsertBlock(), size, align, heap);
   insertInstruction(inst, name);

   return inst;
}

StoreInst* ILBuilder::CreateStore(Value *val,
                                  Value *ptr,
                                  const std::string &name) {
   auto inst = new StoreInst(ptr, val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FieldRefInst* ILBuilder::CreateFieldRef(Value *val,
                                        llvm::StringRef fieldName,
                                        const std::string &name) {
   auto ty = getModule()->getType(val->getType()->getClassName());
   auto inst = new FieldRefInst(val, support::cast<StructType>(ty), fieldName,
                                getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

GEPInst* ILBuilder::CreateGEP(Value *val, int idx,
                              const std::string &name) {
   auto gep = new GEPInst(val, size_t(idx), getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

GEPInst* ILBuilder::CreateGEP(Value *val, Value *idx,
                              const std::string &name) {
   auto gep = new GEPInst(val, idx, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

GEPInst* ILBuilder::CreateStructGEP(AggregateType *Ty,
                                    Value *val, size_t idx,
                                    const std::string &name) {
   auto gep = new GEPInst(Ty, val, idx, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

CaptureExtractInst* ILBuilder::CreateCaptureExtract(size_t idx,
                                                    const std::string &name) {
   auto inst = new CaptureExtractInst(idx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

TupleExtractInst* ILBuilder::CreateTupleExtract(Value *val, size_t idx,
                                                const string &name) {
   auto gep = new TupleExtractInst(val, idx, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

EnumRawValueInst* ILBuilder::CreateEnumRawValue(Value *Val,
                                                const string &name) {
   auto inst = new EnumRawValueInst(Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumExtractInst* ILBuilder::CreateEnumExtract(Value *Val,
                                              llvm::StringRef caseName,
                                              size_t caseVal,
                                              const string &name) {
   auto inst = new EnumExtractInst(Val, caseName, caseVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LoadInst* ILBuilder::CreateLoad(Value *val, const string &name) {
   auto inst = new LoadInst(val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

AddrOfInst* ILBuilder::CreateAddrOf(Value *target,
                                    const std::string &name) {
   auto inst = new AddrOfInst(target, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

PtrToLvalueInst* ILBuilder::CreatePtrToLvalue(Value *target,
                                              const std::string &name) {
   auto inst = new PtrToLvalueInst(target, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

InitInst* ILBuilder::CreateInit(StructType *InitializedType,
                                Method *Init,
                                llvm::ArrayRef<Value *> args,
                                const std::string &name) {
   auto inst = new InitInst(InitializedType,Init, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnionInitInst* ILBuilder::CreateUnionInit(UnionType *UnionTy,
                                          Value *InitializerVal,
                                          const string &name) {
   auto inst = new UnionInitInst(UnionTy, InitializerVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumInitInst* ILBuilder::CreateEnumInit(EnumType *EnumTy,
                                        std::string const& caseName,
                                        llvm::ArrayRef<Value *> args,
                                        const string &name) {
   auto inst = new EnumInitInst(EnumTy, caseName, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LambdaInitInst* ILBuilder::CreateLambdaInit(Function *Function,
                                            llvm::SmallVector<Value *, 4>
                                                                     &&Captures,
                                            const string &name) {
   auto inst = new LambdaInitInst(Function, std::move(Captures),
                                  getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnionCastInst* ILBuilder::CreateUnionCast(Value *target, UnionType *UnionTy,
                                          std::string const& fieldName,
                                          const std::string &name) {
   auto inst = new UnionCastInst(target, UnionTy, fieldName, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ExceptionCastInst* ILBuilder::CreateExceptionCast(Value *val,
                                                  Type *toType,
                                                  const string &name) {
   auto inst = new ExceptionCastInst(val, toType, getInsertBlock());

   insertInstruction(inst, name);

   return inst;
}

RetInst* ILBuilder::CreateRet(Value *Val, const string &name) {
   auto inst = new RetInst(Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

RetInst* ILBuilder::CreateRetVoid(const string &name) {
   auto inst = new RetInst(getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ThrowInst* ILBuilder::CreateThrow(Value *thrownVal, GlobalVariable *typeInfo,
                                  const string &name) {
   auto inst = new ThrowInst(thrownVal, typeInfo, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnreachableInst* ILBuilder::CreateUnreachable(const string &name) {
   auto inst = new UnreachableInst(getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateBr(BasicBlock *target,
                            llvm::SmallVector<Value*, 4> &&BlockArgs,
                            const string &name) {
   auto inst = new BrInst(target, std::move(BlockArgs), getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateUnresolvedBr(const string &name)
{
   auto inst = new BrInst(getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateCondBr(Value *Condition,
                                BasicBlock *IfBranch,
                                BasicBlock *ElseBranch,
                                llvm::SmallVector<Value*, 4> &&TargetArgs,
                                llvm::SmallVector<Value*, 4> &&ElseArgs,
                                const string &name) {
   auto inst = new BrInst(Condition, IfBranch, std::move(TargetArgs),
                          ElseBranch, std::move(ElseArgs), getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

SwitchInst* ILBuilder::CreateSwitch(Value *SwitchVal,
                                    const string &name) {
   auto inst = new SwitchInst(SwitchVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LandingPadInst* ILBuilder::CreateLandingPad(const string &name) {
   auto lpad = new LandingPadInst(getInsertBlock());
   insertInstruction(lpad);

   return lpad;
}

/// Binary Operators Synopsis
//   AddInst *ILBuilder::CreateAdd(Value *lhs, Value *rhs,
//                                 const std::string &name,
//                                 const SourceLocation &loc)
//   {
//      auto inst = new AddInst(lhs, rhs, getInsertBlock());
//      insertInstruction(inst, name);
//
//      return inst;
//   }

#define CDOT_BUILDER_OP(Name)                                              \
   Name##Inst *ILBuilder::Create##Name(Value *lhs, Value *rhs,             \
                                       const std::string &name,            \
                                       const SourceLocation &loc)          \
   {                                                                       \
      auto inst = new Name##Inst(lhs, rhs, getInsertBlock());   \
      insertInstruction(inst, name);                                             \
      return inst;                                                         \
   }

CDOT_BUILDER_OP(Add)
CDOT_BUILDER_OP(Sub)
CDOT_BUILDER_OP(Mul)
CDOT_BUILDER_OP(Div)
CDOT_BUILDER_OP(Mod)
CDOT_BUILDER_OP(Exp)

CDOT_BUILDER_OP(And)
CDOT_BUILDER_OP(Or)
CDOT_BUILDER_OP(Xor)

CDOT_BUILDER_OP(AShr)
CDOT_BUILDER_OP(LShr)
CDOT_BUILDER_OP(Shl)

CDOT_BUILDER_OP(CompEQ)
CDOT_BUILDER_OP(CompNE)
CDOT_BUILDER_OP(CompLT)
CDOT_BUILDER_OP(CompGT)
CDOT_BUILDER_OP(CompLE)
CDOT_BUILDER_OP(CompGE)

#undef CDOT_BUILDER_OP

/// Unary Operator Synopsis
//   MinInst *ILBuilder::CreateMin(Value *target, const std::string &name,
//                                 const SourceLocation &loc) {
//      auto inst = new MinInst(target, getInsertBlock());
//      insertInstruction(inst, name);
//
//      return inst;
//   }

#define CDOT_BUILDER_OP(Name) \
   Name##Inst *ILBuilder::Create##Name(Value *target,                      \
                                       const std::string &name)            \
   {                                                                       \
      auto inst = new Name##Inst(target, getInsertBlock());                \
      insertInstruction(inst, name);                                       \
                                                                           \
      return inst;                                                         \
   }

CDOT_BUILDER_OP(Min)
CDOT_BUILDER_OP(Neg)

#undef CDOT_BUILDER_OP

/// Cast Synopsis
//   BitCastInst * ILBuilder::CreateBitCast(Value *val, Type *toType,
//                                          const std::string &name,
//                                          const SourceLocation &loc) {
//      auto inst = new BitCastInst(val, toType, getInsertBlock());
//      insertInstruction(inst, name);
//
//      return inst;
//   }

#define CDOT_BUILDER_CAST(Name)                                               \
   Name##Inst * ILBuilder::Create##Name(Value *val, Type *toType,             \
                                        const std::string &name)              \
   {                                                                          \
      auto inst = new Name##Inst(val, toType, getInsertBlock());              \
      insertInstruction(inst, name);                                          \
                                                                              \
      return inst;                                                            \
   }

CDOT_BUILDER_CAST(DynamicCast)
CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

IntegerCastInst* ILBuilder::CreateIntegerCast(CastKind kind,
                                              Value *val, Type *toType,
                                              const string &name) {
   auto inst = new IntegerCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

IntegerCastInst* ILBuilder::CreateIntegerCast(CastKind kind, Value *val,
                                              QualType toType,
                                              const std::string &name) {
   auto inst = new IntegerCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FPCastInst* ILBuilder::CreateFPCast(CastKind kind, Value *val,
                                    Type *toType, const string &name)
{
   auto inst = new FPCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FPCastInst* ILBuilder::CreateFPCast(CastKind kind, Value *val,
                                     QualType toType, const string &name) {
   auto inst = new FPCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}


BitCastInst* ILBuilder::CreateBitCast(CastKind kind, Value *val,
                                      Type *toType, const string &name)
{
   auto inst = new BitCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

IntToEnumInst* ILBuilder::CreateIntToEnum(Value *target, Type *toType,
                                          const std::string &name) {
   auto inst = new IntToEnumInst(target, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Value* ILBuilder::CreateIsX(Value *V, uint64_t val)
{
   il::Value *comp;
   if (V->getType()->isIntegerTy())
      comp = ConstantInt::get(*V->getType(), val);
   else if (V->getType()->isPointerTy())
      comp = ConstantPointer::get(*V->getType(), val);
   else
      llvm_unreachable("can't comp given value");

   return CreateCompEQ(V, comp);
}

Value* ILBuilder::CreateIsZero(Value *V)
{
   return CreateIsX(V, 0);
}

Value* ILBuilder::CreateIsOne(Value *V)
{
   return CreateIsX(V, 1);
}

Value* ILBuilder::CreateIsNotZero(Value *V)
{
   return CreateNeg(CreateIsZero(V));
}

Value* ILBuilder::CreateIsNull(Value *V)
{
   auto PtrToInt = CreateIntegerCast(CastKind::PtrToInt, V, IntegerType::get());
   return CreateIsZero(PtrToInt);
}


} // namespace il
} // namespace cdot