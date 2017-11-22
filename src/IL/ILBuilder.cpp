//
// Created by Jonas Zell on 16.11.17.
//

#include "ILBuilder.h"
#include "Module/Module.h"

#include "../AST/Passes/SemanticAnalysis/Function.h"
#include "../AST/Passes/SemanticAnalysis/Record/Record.h"

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

ILBuilder::ILBuilder(Module *M) : M(M), Ctx(M->getContext())
{

}

ILBuilder::~ILBuilder()
{
   Value::cleanup();
}

BasicBlock* ILBuilder::getInsertBlock()
{
   return InsertBlock;
}

void ILBuilder::SetInsertPoint(
   llvm::SmallVector<Instruction *, 8>::iterator it)
{
   if (!*it) {
      InsertBlock = nullptr;
      return;
   }

   insertPoint = it;
   InsertBlock = (*it)->getParent();
}

void ILBuilder::SetInsertPoint(BasicBlock *bb)
{
   InsertBlock = bb;

   if (bb) {
      insertPoint = bb->getInstructions().end();
   }
}

void ILBuilder::insertInstruction(Instruction *inst)
{
   auto BB = getInsertBlock();
   insertPoint = BB->insertInstructionAfter(inst, insertPoint);
   ++insertPoint;
}

BasicBlock* ILBuilder::CreateBasicBlock(const string &name,
                                        const SourceLocation &loc)
{
   return new BasicBlock(InsertBlock->getParent(), name, loc);
}

BasicBlock* ILBuilder::CreateBasicBlock(Function *func,
                                        bool setInsertPoint,
                                        const std::string &name,
                                        const SourceLocation &loc) {
   auto BB = new BasicBlock(func, name, loc);
   if (setInsertPoint) {
      SetInsertPoint(BB);
   }

   return BB;
}

ConstantInt* ILBuilder::CreateConstantInt(Type *ty, uint64_t value)
{
   return new ConstantInt(ty, value);
}

ConstantInt* ILBuilder::CreateTrue()
{
   return new ConstantInt(IntegerType::getBoolTy(), 1);
}

ConstantInt* ILBuilder::CreateFalse()
{
   return new ConstantInt(IntegerType::getBoolTy(), 0);
}

ConstantInt* ILBuilder::CreateChar(char c)
{
   return new ConstantInt(IntegerType::getCharTy(), c);
}

ConstantFloat* ILBuilder::CreateConstantFloat(float f)
{
   return new ConstantFloat(f);
}

ConstantFloat* ILBuilder::CreateConstantDouble(double d)
{
   return new ConstantFloat(d);
}

ConstantString* ILBuilder::CreateConstantString(const std::string &str)
{
   return M->getString(str);
}

ConstantStruct* ILBuilder::CreateConstantStruct(
   AggregateType *ty, llvm::ArrayRef<Constant *> elements)
{
   return new ConstantStruct(ty, elements);
}

ConstantArray * ILBuilder::CreateConstantArray(llvm::ArrayRef<Constant *> Arr)
{
   return new ConstantArray(Arr);
}

ConstantArray * ILBuilder::CreateConstantArray(
   llvm::SmallVector<Constant*, 4> &&Arr)
{
   return new ConstantArray(std::move(Arr));
}

ConstantArray* ILBuilder::CreateConstantArray(Type *ty, size_t numElements)
{
   return new ConstantArray(ty, numElements);
}

ConstantBitCastInst*
ILBuilder::CreateConstantBitCast(Constant *Val, Type *toType,
                                 const string &name,
                                 const SourceLocation &loc) {
   auto inst = new ConstantBitCastInst(Val, toType, getInsertBlock(),
                                       name, loc);
   insertInstruction(inst);
   return inst;
}

Argument* ILBuilder::CreateArgument(Type *type, bool vararg, Function *parent,
                                    const string &name,
                                    const SourceLocation &loc)
{
   return new Argument(type, vararg, parent, name, loc);
}

ClassType* ILBuilder::DeclareClass(const std::string &name,
                                   llvm::StringRef parentClass,
                                   const SourceLocation &loc) {
   return new ClassType(name, parentClass, M, loc);
}

StructType* ILBuilder::DeclareStruct(const std::string &name,
                                     const SourceLocation &loc) {
   return new StructType(name, M, loc);
}

EnumType* ILBuilder::DeclareEnum(const std::string &name, Type *rawType,
                                 const SourceLocation &loc)
{
   return new EnumType(name, rawType, M, loc);
}

UnionType* ILBuilder::DeclareUnion(const std::string &name,
                                   const SourceLocation &loc)
{
   return new UnionType(name, M, loc);
}

ProtocolType* ILBuilder::DeclareProtocol(const std::string &name,
                                         const SourceLocation &loc) {
   return new ProtocolType(name, M, loc);
}

Function* ILBuilder::CreateFunction(Callable *F)
{
   auto func = new Function(F, M);
   M->insertFunction(func);

   return func;
}

Function* ILBuilder::CreateFunction(const std::string &name,
                                    QualType returnType,
                                    llvm::ArrayRef<cdot::Argument> args,
                                    bool mightThrow,
                                    const SourceLocation &loc) {
   auto func = new Function(name, returnType, args, M, loc, mightThrow);
   M->insertFunction(func);

   return func;
}

Method * ILBuilder::CreateMethod(cl::Method *method)
{
   return new Method(method, M);
}

Method * ILBuilder::CreateMethod(AggregateType *forType,
                                 const std::string &methodName,
                                 QualType returnType,
                                 llvm::ArrayRef<cdot::Argument> args,
                                 bool isStatic, bool mightThrow,
                                 const SourceLocation &loc) {
   return new Method(methodName, returnType, args, forType, isStatic,
                     M, loc, mightThrow);
}

Initializer * ILBuilder::CreateInitializer(cl::Method *method)
{
   return new Initializer(method, M);
}

Initializer* ILBuilder::CreateInitializer(AggregateType *forType,
                                          const std::string &methodName,
                                          llvm::ArrayRef<cdot::Argument> args,
                                          bool mightThrow,
                                          const SourceLocation &loc)
{
   return new Initializer(methodName, args, forType, M, loc, mightThrow);
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Type *type, bool isConst,
                                                Constant *initializer,
                                                const std::string &name,
                                                const SourceLocation &loc) {
   return new GlobalVariable(type, isConst, M, initializer, name, loc);
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Constant *initializer,
                                                bool isConst,
                                                const string &name,
                                                const SourceLocation &loc) {
   return new GlobalVariable(*initializer->getType(), isConst, M,
                             initializer, name, loc);
}

CallInst* ILBuilder::CreateCall(Function *F,
                                llvm::ArrayRef<Value *> args,
                                const string &name,
                                const SourceLocation &loc) {
   CallInst *inst = new CallInst(F, args, getInsertBlock(), name, loc);

   insertInstruction(inst);
   return inst;
}

InvokeInst* ILBuilder::CreateInvoke(Function *F, llvm::ArrayRef<Value *> args,
                                    BasicBlock *NormalCont,
                                    BasicBlock *LandingPad, const string &name,
                                    const SourceLocation &loc)
{
   auto inst = new InvokeInst(F, args, NormalCont, LandingPad,
                              getInsertBlock(),name, loc);
   insertInstruction(inst);

   return inst;
}

CallInst* ILBuilder::CreateMethodCall(Method *M, Value *Self,
                                      llvm::ArrayRef<Value *> args,
                                      const string &name,
                                      const SourceLocation &loc) {
   auto inst = new CallInst(M, Self, args, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

CallInst* ILBuilder::CreateMethodInvoke(Method *M, Value *Self,
                                        llvm::ArrayRef<Value *> args,
                                        BasicBlock *NormalCont,
                                        BasicBlock *LandingPad,
                                        const string &name,
                                        const SourceLocation &loc)
{
   auto inst = new InvokeInst(M, Self, args, NormalCont, LandingPad,
                              getInsertBlock(),name, loc);
   insertInstruction(inst);

   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(Type *ofType,
                                    unsigned int align,
                                    bool heap,
                                    const std::string &name,
                                    const SourceLocation &loc) {
   auto inst = new AllocaInst(ofType, getInsertBlock(), align, heap,
                              name, loc);
   insertInstruction(inst);

   return inst;
}

StoreInst* ILBuilder::CreateStore(Value *val,
                                  Value *ptr,
                                  const std::string &name,
                                  const SourceLocation &loc) {
   auto inst = new StoreInst(ptr, val, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

GEPInst* ILBuilder::CreateGEP(Value *val, size_t idx,
                              const std::string &name,
                              const SourceLocation &loc) {
   auto gep = new GEPInst(val, idx, getInsertBlock(), name, loc);
   insertInstruction(gep);

   return gep;
}

GEPInst* ILBuilder::CreateStructGEP(AggregateType *Ty,
                                    Value *val, size_t idx,
                                    const std::string &name,
                                    const SourceLocation &loc) {
   auto gep = new GEPInst(Ty, val, idx, getInsertBlock(), name, loc);
   insertInstruction(gep);

   return gep;
}

TupleExtractInst* ILBuilder::CreateTupleExtract(Value *val, size_t idx,
                                                const string &name,
                                                const SourceLocation &loc) {
   auto gep = new TupleExtractInst(val, idx, getInsertBlock(), name, loc);
   insertInstruction(gep);

   return gep;
}

LoadInst* ILBuilder::CreateLoad(Value *val, const string &name,
                                const SourceLocation &loc)
{
   auto inst = new LoadInst(val, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

InitInst* ILBuilder::CreateInit(StructType *InitializedType,
                                Method *Init,
                                llvm::ArrayRef<Value *> args,
                                const std::string &name,
                                const SourceLocation &loc) {
   auto inst = new InitInst(InitializedType,Init, args,
                                   getInsertBlock(), name,loc);
   insertInstruction(inst);

   return inst;
}

UnionInitInst* ILBuilder::CreateUnionInit(UnionType *UnionTy,
                                          Value *InitializerVal,
                                          const string &name,
                                          const SourceLocation &loc) {
   auto inst = new UnionInitInst(UnionTy, InitializerVal, getInsertBlock(),
                                   name, loc);
   insertInstruction(inst);

   return inst;
}

EnumInitInst* ILBuilder::CreateEnumInit(EnumType *EnumTy,
                                        llvm::StringRef caseName,
                                        llvm::ArrayRef<Value *> args,
                                        const string &name,
                                        const SourceLocation &loc) {
   auto inst = new EnumInitInst(EnumTy, caseName, args, getInsertBlock(),
                                  name, loc);
   insertInstruction(inst);

   return inst;
}

LambdaInitInst* ILBuilder::CreateLambda(Constant *Function,
                                        llvm::SmallVector<Value*, 4>&&Captures,
                                        const string &name,
                                        const SourceLocation &loc) {
   auto inst = new LambdaInitInst(Function, std::move(Captures),
                                  getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

UnionCastInst* ILBuilder::CreateUnionCast(Value *target, UnionType *UnionTy,
                                          llvm::StringRef fieldName,
                                          const std::string &name,
                                          const SourceLocation &loc) {
   auto inst = new UnionCastInst(target, UnionTy, fieldName, getInsertBlock(),
                                 name, loc);
   insertInstruction(inst);

   return inst;
}

ExceptionCastInst* ILBuilder::CreateExceptionCast(Value *val,
                                                  Type *toType,
                                                  const string &name,
                                                  const SourceLocation &loc) {
   auto inst = new ExceptionCastInst(val, toType, getInsertBlock(), name, loc);

   insertInstruction(inst);

   return inst;
}

RetInst* ILBuilder::CreateRet(Value *Val, const string &name,
                              const SourceLocation &loc) {
   auto inst = new RetInst(Val, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

RetInst* ILBuilder::CreateRetVoid(const string &name,
                                  const SourceLocation &loc) {
   auto inst = new RetInst(getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

ThrowInst* ILBuilder::CreateThrow(Value *thrownVal, const string &name,
                                  const SourceLocation &loc) {
   auto inst = new ThrowInst(thrownVal, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

UnreachableInst* ILBuilder::CreateUnreachable(const string &name,
                                              const SourceLocation &loc) {
   auto inst = new UnreachableInst(getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

BrInst* ILBuilder::CreateBr(BasicBlock *target,
                            llvm::SmallVector<Value*, 4> &&BlockArgs,
                            const string &name,
                            const SourceLocation &loc) {
   auto inst = new BrInst(target, std::move(BlockArgs), getInsertBlock(),
                          name, loc);
   insertInstruction(inst);

   return inst;
}

BrInst* ILBuilder::CreateUnresolvedBr(const string &name,
                                      const SourceLocation &loc)
{
   auto inst = new BrInst(getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

BrInst* ILBuilder::CreateCondBr(Value *Condition,
                                BasicBlock *IfBranch,
                                BasicBlock *ElseBranch,
                                llvm::SmallVector<Value*, 4> &&TargetArgs,
                                llvm::SmallVector<Value*, 4> &&ElseArgs,
                                const string &name,
                                const SourceLocation &loc) {
   auto inst = new BrInst(Condition, IfBranch, std::move(TargetArgs),
                          ElseBranch, std::move(ElseArgs), getInsertBlock(),
                          name, loc);
   insertInstruction(inst);

   return inst;
}

SwitchInst* ILBuilder::CreateSwitch(Value *SwitchVal,
                                    const string &name,
                                    const SourceLocation &loc) {
   auto inst = new SwitchInst(SwitchVal, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

LandingPadInst* ILBuilder::CreateLandingPad(const string &name,
                                            const SourceLocation &loc) {
   auto lpad = new LandingPadInst(getInsertBlock(), name, loc);
   insertInstruction(lpad);

   return lpad;
}

/// Binary Operators Synopsis
//   AddInst *ILBuilder::CreateAdd(Value *lhs, Value *rhs,
//                                 const std::string &name,
//                                 const SourceLocation &loc)
//   {
//      auto inst = new AddInst(lhs, rhs, getInsertBlock(), name, loc);
//      insertInstruction(inst);
//
//      return inst;
//   }

#define CDOT_BUILDER_OP(Name)                                              \
   Name##Inst *ILBuilder::Create##Name(Value *lhs, Value *rhs,             \
                                       const std::string &name,            \
                                       const SourceLocation &loc)          \
   {                                                                       \
      auto inst = new Name##Inst(lhs, rhs, getInsertBlock(), name, loc);   \
      insertInstruction(inst);                                             \
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
//      auto inst = new MinInst(target, getInsertBlock(), name, loc);
//      insertInstruction(inst);
//
//      return inst;
//   }

#define CDOT_BUILDER_OP(Name) \
   Name##Inst *ILBuilder::Create##Name(Value *target,                      \
                                       const std::string &name,            \
                                       const SourceLocation &loc)          \
   {                                                                       \
      auto inst = new Name##Inst(target, getInsertBlock(), name, loc);     \
      insertInstruction(inst);                                             \
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
//      auto inst = new BitCastInst(val, toType, getInsertBlock(), name, loc);
//      insertInstruction(inst);
//
//      return inst;
//   }

#define CDOT_BUILDER_CAST(Name)                                               \
   Name##Inst * ILBuilder::Create##Name(Value *val, Type *toType,             \
                                        const std::string &name,              \
                                        const SourceLocation &loc)            \
   {                                                                          \
      auto inst = new Name##Inst(val, toType, getInsertBlock(), name, loc);   \
      insertInstruction(inst);                                                \
                                                                              \
      return inst;                                                            \
   }

CDOT_BUILDER_CAST(BitCast)
CDOT_BUILDER_CAST(FPCast)
CDOT_BUILDER_CAST(DynamicCast)
CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

IntegerCastInst * ILBuilder::CreateIntegerCast(Value *val, Type *toType,
                                               const string &name,
                                               const SourceLocation &loc) {
   auto from = val->getType();
   if (from->isIntegerTy() && toType->isIntegerTy()
       && from->getBitwidth() != toType->getBitwidth()
       && from->isUnsigned() != toType->isUnsigned()) {
      auto newSign = IntegerType::get(from->getBitwidth(),
                                      toType->isUnsigned());
      auto SignFlipInst = new IntegerCastInst(val, newSign, getInsertBlock(),
                                              "", loc);
      insertInstruction(SignFlipInst);

      val = SignFlipInst;
   }

   auto inst = new IntegerCastInst(val, toType, getInsertBlock(), name, loc);
   insertInstruction(inst);

   return inst;
}

} // namespace il
} // namespace cdot