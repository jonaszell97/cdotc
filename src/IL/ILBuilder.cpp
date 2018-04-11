//
// Created by Jonas Zell on 16.11.17.
//

#include "ILBuilder.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Basic/CastKind.h"
#include "Basic/FileManager.h"
#include "Context.h"
#include "Constants.h"
#include "Instructions.h"
#include "Module.h"

using std::string;

namespace cdot {
namespace il {

ILBuilder::ILBuilder(ast::ASTContext &ASTCtx, Context &Ctx,
                     fs::FileManager &FileMgr)
   : ASTCtx(ASTCtx), Ctx(Ctx), M(nullptr), FileMgr(FileMgr)
{

}

ILBuilder::ILBuilder(ast::ASTContext &ASTCtx, fs::FileManager &FileMgr,
                     Module *M)
   : ASTCtx(ASTCtx), Ctx(M->getContext()), M(M), FileMgr(FileMgr)
{

}

ILBuilder::~ILBuilder()
{

}

BasicBlock* ILBuilder::getInsertBlock()
{
   return InsertBlock;
}

void ILBuilder::SetInsertPoint(iterator it)
{
   insertPoint = it;
   InsertBlock = it->getParent();
   LastDebugLoc = SourceLocation();
}

void ILBuilder::SetInsertPoint(BasicBlock *bb)
{
   InsertBlock = bb;
   LastDebugLoc = SourceLocation();

   if (bb) {
      insertPoint = bb->getInstructions().end();
   }
}

void ILBuilder::insertInstruction(Instruction *inst,
                                  llvm::StringRef name) {
   auto BB = getInsertBlock();

   insertPoint = BB->getInstructions().insert(insertPoint, inst);
   ++insertPoint;

   if (!name.empty())
      inst->setName(name);

   if (!inst->getType())
      inst->type = ValueType(Ctx, ASTCtx.getVoidType());
}

void ILBuilder::SetDebugLoc(SourceLocation debugLoc)
{
   if (!InsertBlock || !debugLoc || LastDebugLoc == debugLoc)
      return;

   CreateDebugLoc(debugLoc);
   LastDebugLoc = debugLoc;
}

il::DebugLocInst* ILBuilder::CreateDebugLoc(SourceLocation Loc)
{
   auto ID = FileMgr.getSourceId(Loc);
   auto LineAndCol = FileMgr.getLineAndCol(Loc);

   auto inst = new DebugLocInst(LineAndCol.line, LineAndCol.col, ID,
                                ValueType(Ctx, ASTCtx.getVoidType()),
                                getInsertBlock());

   insertInstruction(inst, "");
   return inst;
}

il::DebugLocalInst* ILBuilder::CreateDebugLocal(const IdentifierInfo *II,
                                                il::Value *Val) {
   auto inst = new DebugLocalInst(II, Val, getInsertBlock());
   insertInstruction(inst, "");

   return inst;
}

BasicBlock* ILBuilder::CreateBasicBlock(llvm::StringRef name)
{
   auto BB = new BasicBlock(InsertBlock->getParent());

   if (!name.empty())
      BB->setName(name);

   return BB;
}

BasicBlock* ILBuilder::CreateBasicBlock(Function *func,
                                        bool setInsertPoint,
                                        llvm::StringRef name) {
   auto BB = new BasicBlock(func);
   if (!name.empty())
      BB->setName(name);

   if (setInsertPoint)
      SetInsertPoint(BB);

   return BB;
}

ConstantInt* ILBuilder::GetConstantInt(QualType ty, uint64_t value)
{
   return ConstantInt::get(ValueType(Ctx, ty), value);
}

ConstantInt* ILBuilder::GetConstantInt(QualType ty, llvm::APSInt &&Val)
{
   return ConstantInt::get(ValueType(Ctx, ty), std::move(Val));
}

ConstantInt* ILBuilder::GetConstantInt(QualType ty,
                                       const llvm::APSInt &Val) {
   return ConstantInt::get(ValueType(Ctx, ty), llvm::APSInt(Val));
}

ConstantInt* ILBuilder::GetTrue()
{
   return ConstantInt::getTrue(Ctx);
}

ConstantInt* ILBuilder::GetFalse()
{
   return ConstantInt::getFalse(Ctx);
}

ConstantInt* ILBuilder::GetChar(char c)
{
   return ConstantInt::get(ValueType(Ctx, ASTCtx.getCharTy()), c);
}

ConstantFloat* ILBuilder::GetConstantFP(QualType ty, double d)
{
   if (ty->isFloatTy())
      return ConstantFloat::get(ValueType(Ctx, ASTCtx.getFloatTy()), (float)d);

   assert(ty->isDoubleTy());
   return ConstantFloat::get(ValueType(Ctx, ASTCtx.getDoubleTy()), d);
}

ConstantFloat* ILBuilder::GetConstantFP(QualType ty, llvm::APFloat &&Val)
{
   return ConstantFloat::get(ValueType(Ctx, ty), std::move(Val));
}

ConstantFloat* ILBuilder::GetConstantFP(QualType ty,
                                        const llvm::APFloat &Val) {
   return ConstantFloat::get(ValueType(Ctx, ty), llvm::APFloat(Val));
}

ConstantFloat* ILBuilder::GetConstantFloat(float f)
{
   return ConstantFloat::get(ValueType(Ctx, ASTCtx.getFloatTy()), f);
}

ConstantFloat* ILBuilder::GetConstantDouble(double d)
{
   return ConstantFloat::get(ValueType(Ctx, ASTCtx.getDoubleTy()), d);
}

ConstantString* ILBuilder::GetConstantString(llvm::StringRef str)
{
   return ConstantString::get(Ctx, str);
}

ConstantStruct* ILBuilder::GetConstantStruct(ast::StructDecl *S,
                                             llvm::ArrayRef<Constant*>elements){
   return ConstantStruct::get(ValueType(Ctx, ASTCtx.getRecordType(S)),
                              elements);
}

ConstantArray *ILBuilder::GetConstantArray(QualType ArrTy,
                                           llvm::ArrayRef<il::Constant *> Arr) {
   return ConstantArray::get(ValueType(Ctx, ArrTy), Arr);
}

ConstantArray* ILBuilder::GetConstantArray(llvm::ArrayRef<Constant *> Arr)
{
   auto ty = ASTCtx.getArrayType(Arr.front()->getType(), Arr.size());
   return ConstantArray::get(ValueType(Ctx, ty), Arr);
}

ConstantArray* ILBuilder::GetConstantArray(QualType ty, size_t numElements)
{
   return ConstantArray::get(ValueType(Ctx,
                                       ASTCtx.getArrayType(ty, numElements)),
                             {});
}

Constant* ILBuilder::GetConstantPtr(QualType ty, uintptr_t val)
{
   if (val == 0)
      return ConstantPointer::get(ValueType(Ctx, ty));

   return ConstantExpr::getIntToPtr(GetConstantInt(ASTCtx.getUIntTy(), val),
                                    ty);
}

Argument* ILBuilder::CreateArgument(QualType type, bool vararg,
                                    BasicBlock *parent,
                                    llvm::StringRef name,
                                    SourceLocation loc) {
   auto arg = new Argument(ValueType(Ctx, type),
                           vararg, parent, name);

   if (loc)
      arg->setLocation(loc);

   return arg;
}

Function* ILBuilder::CreateFunction(llvm::StringRef name,
                                    QualType returnType,
                                    llvm::ArrayRef<Argument*> args,
                                    bool mightThrow,
                                    bool vararg,
                                    SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto F = new Function(name, FuncTy, M);
   if (loc)
      F->setLocation(loc);

   auto EntryBlock = new BasicBlock(F);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   return F;
}

Lambda* ILBuilder::CreateLambda(QualType returnType,
                                llvm::ArrayRef<Argument *> args,
                                bool mightThrow, SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto L = new Lambda(FuncTy, M);
   if (loc)
      L->setLocation(loc);

   auto EntryBlock = new BasicBlock(L);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   return L;
}

Method *ILBuilder::CreateMethod(llvm::StringRef methodName,
                                QualType returnType,
                                llvm::ArrayRef<Argument *> args,
                                bool isStatic,
                                bool isVirtual,
                                bool mightThrow,
                                bool vararg,
                                SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto M = new Method(methodName, FuncTy, isStatic, isVirtual, this->M);
   if (loc)
      M->setLocation(loc);

   auto EntryBlock = new BasicBlock(M);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   return M;
}

Initializer* ILBuilder::CreateInitializer(llvm::StringRef methodName,
                                          llvm::ArrayRef<Argument *> args,
                                          bool mightThrow,
                                          bool vararg,
                                          SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(ASTCtx.getVoidType(),
                                        argTypes, flags);

   auto I = new Initializer(methodName, FuncTy, M);
   if (loc)
      I->setLocation(loc);

   auto EntryBlock = new BasicBlock(I);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   return I;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(QualType type, bool isConst,
                                                Constant *initializer,
                                                llvm::StringRef name,
                                                SourceLocation loc) {
   auto G = new GlobalVariable(type, isConst, name, M, initializer);
   if (loc)
      G->setLocation(loc);

   return G;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Constant *initializer,
                                                bool isConst,
                                                llvm::StringRef name,
                                                SourceLocation loc) {
   auto G = new GlobalVariable(initializer->getType(), isConst, name, M,
                               initializer);

   if (loc)
      G->setLocation(loc);

   return G;
}

VTable *ILBuilder::CreateVTable(llvm::ArrayRef<il::Function *> Entries,
                                ast::ClassDecl *ClassTy) {
   return VTable::Create(Ctx, Entries, ClassTy);
}

CallInst* ILBuilder::CreateCall(Function *F,
                                llvm::ArrayRef<Value *> args,
                                llvm::StringRef name) {
   CallInst *inst = new CallInst(F, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ProtocolCallInst* ILBuilder::CreateProtocolCall(Method *M,
                                                llvm::ArrayRef<Value *> args,
                                                llvm::StringRef name) {
   auto inst = new ProtocolCallInst(M, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

VirtualCallInst* ILBuilder::CreateVirtualCall(Method *M,
                                              llvm::ArrayRef<Value *> args,
                                              llvm::StringRef name) {
   auto inst = new VirtualCallInst(M, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

namespace {

#ifndef NDEBUG

bool isPointerLike(Value *V)
{
   return V->getType()->isRecordType() || V->getType()->isPointerType()
          || V->getType()->isReferenceType();
}

bool isRetainable(Value *V)
{
   return V->getType()->isRefcounted()
          || support::isa<AllocBoxInst>(V);
}

void checkIntrinsicArgs(Intrinsic id, llvm::ArrayRef<Value *> args)
{
   switch (id) {
   case Intrinsic::memcpy:
      assert(args.size() == 3);
      assert(isPointerLike(args[0]) && isPointerLike(args[1]));
      assert(args[2]->getType()->isIntegerType());
      break;
   case Intrinsic::memset:
      assert(args.size() == 3);
      assert(isPointerLike(args[0]));
      assert(args[1]->getType()->isInt8Ty()
             || args[1]->getType()->isInt8Ty(true));
      assert(args[2]->getType()->isIntegerType());
      break;
   case Intrinsic::memcmp:
      assert(args.size() == 3);
      assert(isPointerLike(args[0]) && isPointerLike(args[1]));
      assert(args[2]->getType()->isIntegerType());
      break;
   case Intrinsic::__ctfe_stacktrace:
      assert(args.empty());
   case Intrinsic::get_lambda_env:
   case Intrinsic::get_lambda_funcptr:
      assert(args.size() == 1 && args.front()->getType()->isLambdaType());
      break;
   case Intrinsic::retain:
   case Intrinsic::release:
      assert(args.size() == 1 && isRetainable(args.front()));
      break;
   case Intrinsic::strong_refcount:
   case Intrinsic::weak_refcount:
   case Intrinsic::vtable_ref:
   case Intrinsic::typeinfo_ref:
      assert(args.size() == 1 && args.front()->getType()->isClass());
      break;
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end:
      assert(args.size() == 2);
      assert(args[0]->getType()->isRecordType()
             || args[0]->getType()->isPointerType());
      assert(args[1]->getType()->isIntegerType());
      break;
   }
}

#endif

QualType getIntrinsicReturnType(ast::ASTContext &Ctx, Intrinsic id)
{
   switch (id) {
   case Intrinsic::memcpy:
   case Intrinsic::memset:
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end:
   case Intrinsic::retain:
   case Intrinsic::release:
   case Intrinsic::__ctfe_stacktrace:
      return Ctx.getVoidType();
   case Intrinsic::memcmp:
      return Ctx.getInt32Ty();
   case Intrinsic::get_lambda_env:
      return Ctx.getReferenceType(Ctx.getInt8PtrTy()->getPointerTo(Ctx));
   case Intrinsic::get_lambda_funcptr:
      return Ctx.getPointerType(Ctx.getInt8PtrTy());
   case Intrinsic::strong_refcount:
      return Ctx.getReferenceType(Ctx.getUIntTy());
   case Intrinsic::weak_refcount:
      return Ctx.getReferenceType(Ctx.getUIntTy());
   case Intrinsic::vtable_ref:
      return Ctx.getReferenceType(Ctx.getInt8PtrTy());
   case Intrinsic::typeinfo_ref:
      return Ctx.getReferenceType(Ctx.getInt8PtrTy());
   }
}

} // anonymous namespace

IntrinsicCallInst* ILBuilder::CreateIntrinsic(Intrinsic id,
                                              llvm::ArrayRef<Value *> args,
                                              llvm::StringRef name) {
#ifndef NDEBUG
   checkIntrinsicArgs(id, args);
#endif

   auto inst = new IntrinsicCallInst(
      id, ValueType(Ctx, getIntrinsicReturnType(ASTCtx, id)),
      args, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

Instruction* ILBuilder::GetStrongRefcount(il::Value *V,
                                          llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::strong_refcount, V, name);
}

Instruction* ILBuilder::GetWeakRefcount(il::Value *V,
                                        llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::weak_refcount, V, name);
}

Instruction* ILBuilder::GetVTable(il::Value *V,
                                  llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::vtable_ref, V, name);
}

Instruction* ILBuilder::GetTypeInfo(il::Value *V,
                                    llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::typeinfo_ref, V, name);
}

Instruction* ILBuilder::CreateRetain(il::Value *V,
                                     llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::retain, V, name);
}

Instruction* ILBuilder::CreateRelease(il::Value *V,
                                      llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::release, V, name);
}

Instruction* ILBuilder::CreateLifetimeBegin(il::Value *V,
                                            llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::lifetime_begin, V, name);
}

Instruction* ILBuilder::CreateLifetimeEnd(il::Value *V,
                                          llvm::StringRef name) {
   return CreateIntrinsic(Intrinsic::lifetime_end, V, name);
}

IndirectCallInst* ILBuilder::CreateIndirectCall(Value *Func,
                                                llvm::ArrayRef<Value *> args,
                                                llvm::StringRef name) {
   auto inst = new IndirectCallInst(Func, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LambdaCallInst* ILBuilder::CreateLambdaCall(Value *Func,
                                            llvm::ArrayRef<Value *> args,
                                            llvm::StringRef name) {
   auto inst = new LambdaCallInst(Func, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

InvokeInst* ILBuilder::CreateInvoke(Function *F, llvm::ArrayRef<Value *> args,
                                    BasicBlock *NormalCont,
                                    BasicBlock *LandingPad, llvm::StringRef name) {
   auto inst = new InvokeInst(F, args, NormalCont, LandingPad,
                              getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ProtocolInvokeInst* ILBuilder::CreateProtocolInvoke(Method *M,
                                                    llvm::ArrayRef<Value*> args,
                                                    BasicBlock *NormalCont,
                                                    BasicBlock *LandingPad,
                                                    llvm::StringRef name) {
   auto inst = new ProtocolInvokeInst(M, args, NormalCont, LandingPad,
                                      getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

VirtualInvokeInst* ILBuilder::CreateVirtualInvoke(Method *M,
                                                  llvm::ArrayRef<Value *> args,
                                                  BasicBlock *NormalCont,
                                                  BasicBlock *LandingPad,
                                                  llvm::StringRef name) {
   auto inst = new VirtualInvokeInst(M, args, NormalCont, LandingPad,
                                     getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(QualType ofType,
                                    unsigned int align,
                                    bool heap,
                                    llvm::StringRef name) {
   auto inst = new AllocaInst(ValueType(Ctx, ofType),
                              getInsertBlock(), align, heap);

   insertInstruction(inst, name);
   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(QualType ofType,
                                    size_t size,
                                    unsigned int align,
                                    bool heap,
                                    llvm::StringRef name) {
   auto inst = new AllocaInst(ValueType(Ctx, ofType), getInsertBlock(), size,
                              align, heap);

   insertInstruction(inst, name);
   return inst;
}

Instruction* ILBuilder::AllocUninitialized(size_t size,
                                           unsigned int align,
                                           bool heap,
                                           llvm::StringRef name) {
   auto ArrTy = ASTCtx.getArrayType(ASTCtx.getUInt8Ty(), size);
   auto alloca = new AllocaInst(ValueType(Ctx, ArrTy), getInsertBlock(),
                                1, 0, heap);

   insertInstruction(alloca, "");
   return CreateBitCast(CastKind::BitCast, CreateLoad(alloca),
                        ASTCtx.getInt8PtrTy());
}

AllocBoxInst *ILBuilder::CreateAllocBox(QualType Ty,
                                        Function *Deinitializer,
                                        llvm::StringRef name) {
   auto inst = new AllocBoxInst(ValueType(Ctx, Ty), Deinitializer,
                                getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

DeallocInst* ILBuilder::CreateDealloc(il::Value *V,
                                      bool Heap,
                                      llvm::StringRef name) {
   auto inst = new DeallocInst(V, Heap, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DeallocBoxInst* ILBuilder::CreateDeallocBox(il::Value *V,
                                            llvm::StringRef name) {
   auto inst = new DeallocBoxInst(V, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

StoreInst* ILBuilder::CreateStore(Value *val,
                                  Value *ptr,
                                  llvm::StringRef name) {
   auto inst = new StoreInst(ptr, val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FieldRefInst* ILBuilder::CreateFieldRef(Value *val,
                                        const DeclarationName &fieldName,
                                        llvm::StringRef name) {
   auto inst = new FieldRefInst(val, fieldName, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

GEPInst* ILBuilder::CreateStructGEP(il::Value *val,
                                    size_t idx,
                                    llvm::StringRef name) {
   auto inst = new GEPInst(val, idx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

GEPInst* ILBuilder::CreateGEP(Value *val,
                              size_t idx,
                              llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto gep = new GEPInst(val, CI, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

Instruction* ILBuilder::CreateExtractValue(Value *val,
                                           size_t idx,
                                           llvm::StringRef name) {
   return CreateLoad(CreateGEP(val, idx, name));
}

GEPInst* ILBuilder::CreateGEP(Value *val, Value *idx,
                              llvm::StringRef name) {
   auto gep = new GEPInst(val, idx, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

CaptureExtractInst* ILBuilder::CreateCaptureExtract(size_t idx,
                                                    llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto inst = new CaptureExtractInst(CI, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

TupleExtractInst* ILBuilder::CreateTupleExtract(Value *val, size_t idx,
                                                llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto gep = new TupleExtractInst(val, CI, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

EnumRawValueInst* ILBuilder::CreateEnumRawValue(Value *Val,
                                                llvm::StringRef name) {
   auto inst = new EnumRawValueInst(Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumExtractInst* ILBuilder::CreateEnumExtract(Value *Val,
                                              const IdentifierInfo *caseName,
                                              size_t caseVal,
                                              llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), caseVal);

   auto inst = new EnumExtractInst(Val, caseName, CI, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LoadInst* ILBuilder::CreateLoad(Value *val, llvm::StringRef name) {
   auto inst = new LoadInst(val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

AddrOfInst* ILBuilder::CreateAddrOf(Value *target,
                                    llvm::StringRef name) {
   auto inst = new AddrOfInst(target, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

PtrToLvalueInst* ILBuilder::CreatePtrToLvalue(Value *target,
                                              llvm::StringRef name) {
   auto inst = new PtrToLvalueInst(target, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

InitInst* ILBuilder::CreateInit(ast::StructDecl *InitializedType,
                                Method *Init,
                                llvm::ArrayRef<Value *> args,
                                llvm::StringRef name) {
   auto inst = new InitInst(InitializedType,Init, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnionInitInst* ILBuilder::CreateUnionInit(ast::UnionDecl *UnionTy,
                                          Value *InitializerVal,
                                          llvm::StringRef name) {
   auto inst = new UnionInitInst(UnionTy, InitializerVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumInitInst* ILBuilder::CreateEnumInit(ast::EnumDecl *EnumTy,
                                        const IdentifierInfo *caseName,
                                        llvm::ArrayRef<Value *> args,
                                        llvm::StringRef name) {
   auto inst = new EnumInitInst(Ctx, EnumTy, caseName, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LambdaInitInst* ILBuilder::CreateLambdaInit(Function *Function,
                                            QualType LambdaTy,
                                            llvm::ArrayRef<Value*> Captures,
                                            llvm::StringRef name) {
   auto inst = new LambdaInitInst(Function, LambdaTy,
                                  Captures, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DeinitializeLocalInst* ILBuilder::CreateDeinitializeLocal(Value *RefcountedVal,
                                                          llvm::StringRef name){
   auto inst = new DeinitializeLocalInst(RefcountedVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DeinitializeLocalInst* ILBuilder::CreateDeinitializeLocal(Function *DeinitFn,
                                                          Value *Val,
                                                          llvm::StringRef name){
   auto inst = new DeinitializeLocalInst(DeinitFn, Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DeinitializeTemporaryInst* ILBuilder::CreateDeinitializeTemp(Value*RefcountedVal,
                                                          llvm::StringRef name){
   auto inst = new DeinitializeTemporaryInst(RefcountedVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DeinitializeTemporaryInst* ILBuilder::CreateDeinitializeTemp(Function *DeinitFn,
                                                             Value *Val,
                                                          llvm::StringRef name){
   auto inst = new DeinitializeTemporaryInst(DeinitFn, Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnionCastInst* ILBuilder::CreateUnionCast(Value *target,
                                          ast::UnionDecl *UnionTy,
                                          const IdentifierInfo *fieldName,
                                          llvm::StringRef name) {
   auto inst = new UnionCastInst(target, UnionTy, fieldName, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ExceptionCastInst* ILBuilder::CreateExceptionCast(Value *val,
                                                  QualType toType,
                                                  llvm::StringRef name) {
   auto inst = new ExceptionCastInst(val, toType, getInsertBlock());

   insertInstruction(inst, name);

   return inst;
}

RetInst* ILBuilder::CreateRet(Value *Val, llvm::StringRef name) {
   auto inst = new RetInst(Val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

RetInst* ILBuilder::CreateRetVoid(llvm::StringRef name) {
   auto inst = new RetInst(Ctx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

ThrowInst* ILBuilder::CreateThrow(Value *thrownVal, GlobalVariable *typeInfo,
                                  llvm::StringRef name) {
   auto inst = new ThrowInst(thrownVal, typeInfo, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnreachableInst* ILBuilder::CreateUnreachable(llvm::StringRef name) {
   auto inst = new UnreachableInst(Ctx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateBr(BasicBlock *target,
                            llvm::ArrayRef<Value*> BlockArgs,
                            llvm::StringRef name) {
   auto inst = new BrInst(target, BlockArgs, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateUnresolvedBr(llvm::StringRef name)
{
   auto inst = new BrInst(Ctx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

BrInst* ILBuilder::CreateCondBr(Value *Condition,
                                BasicBlock *IfBranch,
                                BasicBlock *ElseBranch,
                                llvm::ArrayRef<Value*> TargetArgs,
                                llvm::ArrayRef<Value*> ElseArgs,
                                llvm::StringRef name) {
   auto inst = new BrInst(Condition, IfBranch, TargetArgs,
                          ElseBranch, ElseArgs, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

SwitchInst* ILBuilder::CreateSwitch(Value *SwitchVal,
                                    BasicBlock *DefaultDst,
                                    llvm::StringRef name) {
   auto inst = new SwitchInst(SwitchVal, DefaultDst, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

LandingPadInst* ILBuilder::CreateLandingPad(llvm::StringRef name) {
   auto lpad = new LandingPadInst(Ctx, getInsertBlock());
   insertInstruction(lpad);

   return lpad;
}

BinaryOperatorInst* ILBuilder::CreateBinOp(unsigned char OpCode,
                                           il::Value *lhs, il::Value *rhs,
                                           llvm::StringRef name,
                                           SourceLocation loc) {
   auto inst = new BinaryOperatorInst((BinaryOperatorInst::OpCode)OpCode,
                                      lhs, rhs, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

UnaryOperatorInst* ILBuilder::CreateUnaryOp(unsigned char OpCode,
                                            il::Value *target,
                                            llvm::StringRef name,
                                            SourceLocation loc) {
   auto inst = new UnaryOperatorInst((UnaryOperatorInst::OpCode)OpCode,
                                     target, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

CompInst* ILBuilder::CreateComp(unsigned char OpCode,
                                il::Value *lhs, il::Value *rhs,
                                llvm::StringRef name,
                                SourceLocation loc) {
   auto inst = new CompInst((CompInst::OpCode)OpCode,
                            lhs, rhs, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

#define CDOT_BINARY_OP(Name, OP)                                           \
   BinaryOperatorInst *ILBuilder::Create##Name(Value *lhs, Value *rhs,     \
                                               llvm::StringRef name,       \
                                               SourceLocation loc)  \
   {                                                                       \
      auto inst = new BinaryOperatorInst(BinaryOperatorInst::Name, lhs,    \
                                         rhs, getInsertBlock());           \
      insertInstruction(inst, name);                                       \
      return inst;                                                         \
   }

#define CDOT_COMP_OP(Name, OP)                                             \
   CompInst *ILBuilder::Create##Name(Value *lhs, Value *rhs,               \
                                     llvm::StringRef name,                 \
                                     SourceLocation loc)            \
   {                                                                       \
      auto inst = new CompInst(CompInst::Name, lhs,                        \
                               rhs, getInsertBlock());                     \
      insertInstruction(inst, name);                                       \
      return inst;                                                         \
   }

#define CDOT_UNARY_OP(Name, OP)                                            \
   UnaryOperatorInst *ILBuilder::Create##Name(Value *target,               \
                                              llvm::StringRef name)        \
   {                                                                       \
      auto inst = new UnaryOperatorInst(UnaryOperatorInst::Name, target,   \
                                        getInsertBlock());                 \
      insertInstruction(inst, name);                                       \
                                                                           \
      return inst;                                                         \
   }

#include "Instructions.def"

#define CDOT_BUILDER_CAST(Name)                                               \
   Name##Inst * ILBuilder::Create##Name(Value *val, Type *toType,             \
                                        llvm::StringRef name) {               \
      auto inst = new Name##Inst(val, toType, getInsertBlock());              \
      insertInstruction(inst, name);                                          \
                                                                              \
      return inst;                                                            \
   }

CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

IntegerCastInst* ILBuilder::CreateIntegerCast(CastKind kind, Value *val,
                                              QualType toType,
                                              llvm::StringRef name) {
   auto inst = new IntegerCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FPCastInst* ILBuilder::CreateFPCast(CastKind kind, Value *val,
                                     QualType toType, llvm::StringRef name) {
   auto inst = new FPCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}


BitCastInst* ILBuilder::CreateBitCast(CastKind kind, Value *val,
                                      QualType toType, llvm::StringRef name)
{
   auto inst = new BitCastInst(kind, val, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

DynamicCastInst* ILBuilder::CreateDynamicCast(il::Value *val,
                                              ast::ClassDecl *TargetTy,
                                              QualType Type,
                                              llvm::StringRef name) {
   auto inst = new DynamicCastInst(val, TargetTy, Type, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

IntToEnumInst* ILBuilder::CreateIntToEnum(Value *target, Type *toType,
                                          llvm::StringRef name) {
   auto inst = new IntToEnumInst(target, toType, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Value* ILBuilder::CreateIsX(Value *V, uint64_t val)
{
   il::Value *comp;
   if (V->getType()->isIntegerType())
      comp = ConstantInt::get(V->getType(), val);
   else if (V->getType()->isPointerType())
      comp = GetConstantPtr(V->getType(), val);
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
   auto PtrToInt = CreateIntegerCast(CastKind::PtrToInt, V, ASTCtx.getUIntTy());
   return CreateIsZero(PtrToInt);
}


} // namespace il
} // namespace cdot