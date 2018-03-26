//
// Created by Jonas Zell on 16.11.17.
//

#include "ILBuilder.h"
#include "Module/Module.h"
#include "Value/Record/AggregateType.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"

#define CDOT_VALUE_INCLUDE
#include "Value/ValueIncludes.def"
#include "Module/Context.h"
#include "../Basic/CastKind.h"

using std::string;

namespace cdot {
namespace il {

ILBuilder::ILBuilder(ast::ASTContext &ASTCtx, Context &Ctx)
   : ASTCtx(ASTCtx), Ctx(Ctx), M(nullptr)
{

}

ILBuilder::ILBuilder(ast::ASTContext &ASTCtx, Module *M)
   : ASTCtx(ASTCtx), Ctx(M->getContext()), M(M)
{

}

ILBuilder::~ILBuilder()
{

}

void ILBuilder::importType(QualType Ty)
{
   if (Ty->isPointerType())
      importType(Ty->asPointerType()->getPointeeType());

   if (!Ty->isRecordType())
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
                                  llvm::StringRef name) {
   auto BB = getInsertBlock();

   insertPoint = BB->getInstructions().insert(insertPoint, inst);
   ++insertPoint;

   if (!name.empty())
      inst->setName(name);

   if (debugLoc)
      inst->setLocation(debugLoc);

   if (!inst->getType())
      inst->type = ValueType(Ctx, ASTCtx.getVoidType());

   importType(inst->getType());
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

ConstantStruct* ILBuilder::GetConstantStruct(
   AggregateType *ty, llvm::ArrayRef<Constant *> elements)
{
   return ConstantStruct::get(ty, elements);
}

ConstantArray* ILBuilder::GetConstantArray(llvm::ArrayRef<Constant *> Arr)
{
   auto ty = ASTCtx.getArrayType(Arr.front()->getType(), Arr.size());
   return ConstantArray::get(ValueType(Ctx, ty), Arr);
}

ConstantArray * ILBuilder::GetConstantArray(
   llvm::SmallVector<Constant *, 4> &&Arr)
{
   auto ty = ASTCtx.getArrayType(Arr.front()->getType(), Arr.size());
   return ConstantArray::get(ValueType(Ctx, ty), std::move(Arr));
}

ConstantArray* ILBuilder::GetConstantArray(QualType ty, size_t numElements)
{
   importType(*ty);
   return ConstantArray::get(ValueType(Ctx,
                                       ASTCtx.getArrayType(ty, numElements)));
}

ConstantPointer* ILBuilder::GetConstantPtr(QualType ty, uintptr_t val)
{
   importType(*ty);
   return ConstantPointer::get(ValueType(Ctx, ty), val);
}

Argument* ILBuilder::CreateArgument(QualType type, bool vararg,
                                    BasicBlock *parent,
                                    llvm::StringRef name,
                                    const SourceLocation &loc) {
   importType(*type);
   auto arg = new Argument(ValueType(Ctx, type),
                           vararg, parent, name);

   if (loc)
      arg->setLocation(loc);

   return arg;
}

ClassType* ILBuilder::CreateClass(ast::ClassDecl *C,
                                  llvm::StringRef name,
                                  const SourceLocation &loc) {
   auto CT = new ClassType(ASTCtx.getRecordType(C), name, M);

   if (loc)
      CT->setLocation(loc);

   return CT;
}

StructType* ILBuilder::CreateStruct(ast::RecordDecl *S,
                                    llvm::StringRef name,
                                    const SourceLocation &loc) {
   auto ST = new StructType(ASTCtx.getRecordType(S), name, M);
   if (loc)
      ST->setLocation(loc);

   return ST;
}

EnumType* ILBuilder::CreateEnum(ast::EnumDecl *E,
                                llvm::StringRef name,
                                const SourceLocation &loc) {
   auto ET = new EnumType(ASTCtx.getRecordType(E), name, M);
   if (loc)
      ET->setLocation(loc);

   return ET;
}

UnionType* ILBuilder::CreateUnion(ast::UnionDecl *U,
                                  llvm::StringRef name,
                                  const SourceLocation &loc) {
   auto UT = new UnionType(ASTCtx.getRecordType(U), name, M);
   if (loc)
      UT->setLocation(loc);

   return UT;
}

ProtocolType* ILBuilder::CreateProtocol(ast::ProtocolDecl *P,
                                        llvm::StringRef name,
                                        const SourceLocation &loc) {
   auto PT = new ProtocolType(ASTCtx.getRecordType(P), name, M);
   if (loc)
      PT->setLocation(loc);

   return PT;
}

Function* ILBuilder::CreateFunction(llvm::StringRef name,
                                    QualType returnType,
                                    llvm::ArrayRef<Argument*> args,
                                    bool mightThrow,
                                    bool vararg,
                                    bool isExternC,
                                    const SourceLocation &loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   importType(*returnType);
   auto F = new Function(name, FuncTy, M, isExternC);
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
                                bool mightThrow, const SourceLocation &loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   importType(*returnType);
   auto L = new Lambda(FuncTy, M);
   if (loc)
      L->setLocation(loc);

   auto EntryBlock = new BasicBlock(L);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

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
                                 bool vararg,
                                 const SourceLocation &loc,
                                 bool addSelfArg) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (!mightThrow)
      flags |= FunctionType::NoThrow;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   importType(*returnType);
   auto Me = new Method(methodName, FuncTy, forType, isStatic,
                        isVirtual, isProperty, isOperator, isConversionOp,
                        M, addSelfArg);

   if (loc)
      Me->setLocation(loc);

   auto EntryBlock = new BasicBlock(Me);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   return Me;
}

Initializer* ILBuilder::CreateInitializer(AggregateType *forType,
                                          const std::string &methodName,
                                          llvm::ArrayRef<Argument *> args,
                                          bool mightThrow,
                                          bool vararg,
                                          const SourceLocation &loc,
                                          bool addSelfArg) {
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

   auto I = new Initializer(methodName, FuncTy, forType, M, addSelfArg);
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
                                                const SourceLocation &loc) {
   importType(type);
   auto G = new GlobalVariable(type, isConst, name, M, initializer);
   if (loc)
      G->setLocation(loc);

   return G;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Constant *initializer,
                                                bool isConst,
                                                llvm::StringRef name,
                                                const SourceLocation &loc) {
   importType(initializer->getType());
   auto G = new GlobalVariable(initializer->getType(), isConst, name, M,
                               initializer);

   if (loc)
      G->setLocation(loc);

   return G;
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
      return Ctx.getInt8PtrTy()->getPointerTo(Ctx)->getPointerTo(Ctx);
   case Intrinsic::get_lambda_funcptr:
      return Ctx.getInt8PtrTy()->getPointerTo(Ctx);
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

StoreInst* ILBuilder::CreateStore(Value *val,
                                  Value *ptr,
                                  llvm::StringRef name) {
   if (val->isLvalue()) {
      val = CreateLoad(val); // FIXME this is ugly
      assert(val->getType()->needsStructReturn() && "unloaded value");
   }

   auto inst = new StoreInst(ptr, val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

FieldRefInst* ILBuilder::CreateFieldRef(Value *val,
                                        StructType *ty,
                                        llvm::StringRef fieldName,
                                        llvm::StringRef name) {
   auto inst = new FieldRefInst(val, ty, fieldName,
                                getInsertBlock());
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

GEPInst* ILBuilder::CreateStructGEP(AggregateType *Ty,
                                    Value *val, size_t idx,
                                    llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto gep = new GEPInst(Ty, val, CI, getInsertBlock());
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
                                              llvm::StringRef caseName,
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

InitInst* ILBuilder::CreateInit(StructType *InitializedType,
                                Method *Init,
                                llvm::ArrayRef<Value *> args,
                                llvm::StringRef name) {
   auto inst = new InitInst(InitializedType,Init, args, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnionInitInst* ILBuilder::CreateUnionInit(UnionType *UnionTy,
                                          Value *InitializerVal,
                                          llvm::StringRef name) {
   auto inst = new UnionInitInst(UnionTy, InitializerVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumInitInst* ILBuilder::CreateEnumInit(EnumType *EnumTy,
                                        std::string const& caseName,
                                        llvm::ArrayRef<Value *> args,
                                        llvm::StringRef name) {
   auto inst = new EnumInitInst(EnumTy, caseName, args, getInsertBlock());
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

UnionCastInst* ILBuilder::CreateUnionCast(Value *target, UnionType *UnionTy,
                                          std::string const& fieldName,
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

BinaryOperatorInst* ILBuilder::CreateBinOp(BinaryOperatorInst::OpCode opc,
                                           il::Value *lhs, il::Value *rhs,
                                           llvm::StringRef name,
                                           SourceLocation loc) {
   auto inst = new BinaryOperatorInst(opc, lhs, rhs, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

UnaryOperatorInst* ILBuilder::CreateUnaryOp(UnaryOperatorInst::OpCode opc,
                                            il::Value *target,
                                            llvm::StringRef name,
                                            SourceLocation loc) {
   auto inst = new UnaryOperatorInst(opc, target, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

CompInst* ILBuilder::CreateComp(CompInst::OpCode opc,
                                il::Value *lhs, il::Value *rhs,
                                llvm::StringRef name,
                                SourceLocation loc) {
   auto inst = new CompInst(opc, lhs, rhs, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

#define CDOT_BINARY_OP(Name, OP)                                           \
   BinaryOperatorInst *ILBuilder::Create##Name(Value *lhs, Value *rhs,     \
                                               llvm::StringRef name,       \
                                               const SourceLocation &loc)  \
   {                                                                       \
      auto inst = new BinaryOperatorInst(BinaryOperatorInst::Name, lhs,    \
                                         rhs, getInsertBlock());           \
      insertInstruction(inst, name);                                       \
      return inst;                                                         \
   }

#define CDOT_COMP_OP(Name, OP)                                             \
   CompInst *ILBuilder::Create##Name(Value *lhs, Value *rhs,               \
                                     llvm::StringRef name,                 \
                                     const SourceLocation &loc)            \
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

#include "IL/Value/Instructions.def"

#define CDOT_BUILDER_CAST(Name)                                               \
   Name##Inst * ILBuilder::Create##Name(Value *val, Type *toType,             \
                                        llvm::StringRef name) {               \
      auto inst = new Name##Inst(val, toType, getInsertBlock());              \
      insertInstruction(inst, name);                                          \
                                                                              \
      return inst;                                                            \
   }

CDOT_BUILDER_CAST(DynamicCast)
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
                                      Type *toType, llvm::StringRef name)
{
   auto inst = new BitCastInst(kind, val, toType, getInsertBlock());
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
      comp = ConstantPointer::get(V->getType(), val);
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