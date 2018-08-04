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
                     fs::FileManager &FileMgr, bool EmitDebugInfo)
   : ASTCtx(ASTCtx), Ctx(Ctx), M(nullptr), FileMgr(FileMgr),
     EmitDebugInfo(EmitDebugInfo)
{

}

ILBuilder::ILBuilder(ast::ASTContext &ASTCtx, fs::FileManager &FileMgr,
                     Module *M, bool EmitDebugInfo)
   : ASTCtx(ASTCtx), Ctx(M->getContext()), M(M), FileMgr(FileMgr),
     EmitDebugInfo(EmitDebugInfo)
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

void ILBuilder::SetInsertPoint(BasicBlock *bb, bool KeepDebugLoc)
{
   InsertBlock = bb;

   if (!KeepDebugLoc)
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

   inst->setSourceLoc(LastDebugLoc);

   if (!name.empty())
      inst->setName(name);

   if (!inst->getType())
      inst->type = ValueType(Ctx, ASTCtx.getVoidType());

   if (SyntesizedContext)
      inst->setSynthesized(true);
}

void ILBuilder::restoreIP(InsertPoint &IP)
{
   InsertBlock = IP.BB;
   insertPoint = IP.insertPoint;
   LastDebugLoc = IP.DebugLoc;

   if (InsertBlock)
      M = InsertBlock->getParent()->getParent();
}

void ILBuilder::SetDebugLoc(SourceLocation debugLoc)
{
   if (!EmitDebugInfo || !InsertBlock || !debugLoc || LastDebugLoc == debugLoc){
      LastDebugLoc = debugLoc;
      return;
   }

   CreateDebugLoc(debugLoc);
   LastDebugLoc = debugLoc;
}

il::DebugLocInst* ILBuilder::CreateDebugLoc(SourceLocation Loc)
{
   auto ID = FileMgr.getSourceId(Loc);
   auto LineAndCol = FileMgr.getLineAndCol(Loc);

   auto inst = new DebugLocInst(Loc, LineAndCol.line, LineAndCol.col, ID,
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

ConstantInt* ILBuilder::getInt32(uint64_t Value)
{
   return GetConstantInt(ASTCtx.getInt32Ty(), Value);
}

ConstantInt* ILBuilder::getInt64(uint64_t Value)
{
   return GetConstantInt(ASTCtx.getInt64Ty(), Value);
}

ConstantInt* ILBuilder::getUInt32(uint64_t Value)
{
   return GetConstantInt(ASTCtx.getUInt32Ty(), Value);
}

ConstantInt* ILBuilder::getUInt64(uint64_t Value)
{
   return GetConstantInt(ASTCtx.getUInt64Ty(), Value);
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

ConstantClass *ILBuilder::GetConstantClass(ast::ClassDecl *S,
                                           il::GlobalVariable *TI,
                                           llvm::ArrayRef<Constant*> elements,
                                           ConstantClass *Base) {
   auto CS = GetConstantStruct(S, elements);
   return ConstantClass::get(CS, TI, Base);
}

ConstantClass *ILBuilder::GetConstantClass(ConstantStruct *S,
                                           il::GlobalVariable *TI,
                                           ConstantClass *Base) {
   return ConstantClass::get(S, TI, Base);
}

ConstantClass *ILBuilder::ForwardDeclareConstantClass(ast::ClassDecl *S,
                                                      GlobalVariable *TI) {
   return ConstantClass::ForwardDeclare(ValueType(Ctx, ASTCtx.getRecordType(S)),
                                        TI);
}

ConstantClass* ILBuilder::ReplaceForwardDecl(ConstantClass *ForwardDecl,
                                             ConstantStruct *StructVal,
                                             ConstantClass *Base) {
   return ForwardDecl->ReplaceForwardDecl(StructVal, Base);
}

ConstantClass* ILBuilder::ReplaceForwardDecl(ConstantClass *ForwardDecl,
                                             llvm::ArrayRef<Constant*> elements,
                                             ConstantClass *Base) {
   return ReplaceForwardDecl(ForwardDecl,
                             ConstantStruct::get(ForwardDecl->getType(),
                                                 elements),
                             Base);
}

ConstantUnion *ILBuilder::GetConstantUnion(ast::UnionDecl *U,
                                           il::Constant *InitVal) {
   return ConstantUnion::get(ValueType(Ctx, ASTCtx.getRecordType(U)), InitVal);
}

ConstantEnum *ILBuilder::GetConstantEnum(ast::EnumCaseDecl *Case,
                                         llvm::ArrayRef<Constant*> CaseVals) {
   return ConstantEnum::get(Ctx, Case, CaseVals);
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

ConstantTuple *ILBuilder::GetConstantTuple(llvm::ArrayRef<Constant *> Arr)
{
   llvm::SmallVector<QualType, 4> TupleTypes;
   for (auto &Val : Arr)
      TupleTypes.push_back(Val->getType());

   return ConstantTuple::get(ValueType(Ctx, ASTCtx.getTupleType(TupleTypes)),
                             Arr);
}

ConstantTuple *ILBuilder::GetConstantTuple(QualType TupleTy,
                                           llvm::ArrayRef<Constant *> Arr) {
   return ConstantTuple::get(ValueType(Ctx, TupleTy), Arr);
}

ConstantTuple* ILBuilder::GetEmptyTuple()
{
   return ConstantTuple::getEmpty(Ctx);
}

Constant* ILBuilder::GetConstantPtr(QualType ty, uintptr_t val)
{
   if (val == 0)
      return ConstantPointer::get(ValueType(Ctx, ty));

   return ConstantExpr::getIntToPtr(GetConstantInt(ASTCtx.getUIntTy(), val),
                                    ty);
}

UndefValue *ILBuilder::GetUndefValue(QualType Ty)
{
   return UndefValue::get(ValueType(Ctx, Ty));
}

static QualType getMagicConstantType(ast::ASTContext &C,
                                     MagicConstant::Kind Kind) {
   switch (Kind) {
   case MagicConstant::__ctfe:
      return C.getBoolTy();
   }
}

MagicConstant* ILBuilder::GetMagicConstant(unsigned char Kind)
{
   auto RealKind = static_cast<MagicConstant::Kind>(Kind);
   return MagicConstant::get(ValueType(Ctx,
                                       getMagicConstantType(ASTCtx, RealKind)),
                             RealKind);
}

Constant* ILBuilder::GetTokenNone()
{
   return il::ConstantTokenNone::get(Ctx);
}

Constant *ILBuilder::GetAllZerosValue(QualType Ty)
{
   if (Ty->isIntegerType()) {
      return GetConstantInt(Ty, 0);
   }

   if (Ty->isFPType()) {
      return GetConstantFP(Ty, 0.0);
   }

   if (Ty->isPointerType()) {
      return GetConstantNull(Ty);
   }

   if (Ty->isArrayType()) {
      return ConstantArray::getAllZeros(ValueType(Ctx, Ty));
   }

   if (Ty->isStruct()) {
      return ConstantStruct::getAllZeros(ValueType(Ctx, Ty));
   }

   if (Ty->isTupleType()) {
      return ConstantTuple::getAllZeros(ValueType(Ctx, Ty));
   }

   llvm_unreachable("type does not have an all-zero value!");
}

Argument* ILBuilder::CreateArgument(QualType type,
                                    ArgumentConvention Convention,
                                    BasicBlock *parent,
                                    llvm::StringRef name,
                                    SourceLocation loc) {
   auto arg = new Argument(ValueType(Ctx, type), Convention, parent, name);
   if (loc)
      arg->setSourceLoc(loc);

   return arg;
}

Function* ILBuilder::CreateFunction(llvm::StringRef name,
                                    QualType returnType,
                                    llvm::ArrayRef<Argument*> args,
                                    bool mightThrow,
                                    bool vararg,
                                    SourceLocation loc,
                                    bool OverridePrevious) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (mightThrow)
      flags |= FunctionType::Throws;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto F = new Function(name, FuncTy, M);
   if (loc)
      F->setSourceLoc(loc);

   auto EntryBlock = new BasicBlock(F);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   M->insertFunction(F, overrideSymbols() | OverridePrevious);
   return F;
}

Function* ILBuilder::CreateFunction(Value::TypeID Kind,
                                    llvm::StringRef name,
                                    QualType functionType,
                                    SourceLocation Loc,
                                    bool OverridePrevious) {
   Function *Fn;
   switch (Kind) {
   case Value::FunctionID:
      Fn = new Function(name, support::cast<FunctionType>(*functionType), M);
      break;
   case Value::MethodID:
      Fn = new Method(name, support::cast<FunctionType>(*functionType),
                      false, false, false, M);
      break;
   case Value::InitializerID:
      Fn = new Initializer(name, support::cast<FunctionType>(*functionType),
                           ConstructorKind::Base, M);
      break;
   case Value::LambdaID:
      Fn = new Lambda(support::cast<FunctionType>(*functionType), M);
      break;
   default:
      llvm_unreachable("not a function!");
   }

   Fn->setSourceLoc(Loc);
   M->insertFunction(Fn, OverridePrevious);

   return Fn;
}

Lambda* ILBuilder::CreateLambda(QualType returnType,
                                llvm::ArrayRef<Argument *> args,
                                bool mightThrow, SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (mightThrow)
      flags |= FunctionType::Throws;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto L = new Lambda(FuncTy, M);
   if (loc)
      L->setSourceLoc(loc);

   auto EntryBlock = new BasicBlock(L);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   M->insertFunction(L);
   return L;
}

Method *ILBuilder::CreateMethod(llvm::StringRef methodName,
                                QualType returnType,
                                llvm::ArrayRef<Argument *> args,
                                bool isStatic,
                                bool isVirtual,
                                bool mightThrow,
                                bool vararg,
                                bool isDeinit,
                                SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (mightThrow)
      flags |= FunctionType::Throws;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(returnType, argTypes, flags);

   auto M = new Method(methodName, FuncTy, isStatic, isVirtual, isDeinit,
                       this->M);

   if (loc)
      M->setSourceLoc(loc);

   auto EntryBlock = new BasicBlock(M);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   this->M->insertFunction(M, overrideSymbols());
   return M;
}

Initializer* ILBuilder::CreateInitializer(llvm::StringRef methodName,
                                          llvm::ArrayRef<Argument *> args,
                                          bool mightThrow,
                                          bool vararg,
                                          ConstructorKind Kind,
                                          SourceLocation loc) {
   llvm::SmallVector<QualType, 8> argTypes;
   for (auto &arg : args)
      argTypes.push_back(arg->getType());

   unsigned flags = 0;
   if (mightThrow)
      flags |= FunctionType::Throws;

   if (vararg)
      flags |= FunctionType::CStyleVararg;

   auto FuncTy = ASTCtx.getFunctionType(ASTCtx.getVoidType(),
                                        argTypes, flags);

   auto I = new Initializer(methodName, FuncTy, Kind, M);
   if (loc)
      I->setSourceLoc(loc);

   auto EntryBlock = new BasicBlock(I);
   EntryBlock->setName("entry");

   for (const auto &arg : args)
      EntryBlock->getArgs().push_back(arg);

   M->insertFunction(I, overrideSymbols());
   return I;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(QualType type, bool isConst,
                                                Constant *initializer,
                                                llvm::StringRef name,
                                                SourceLocation loc,
                                                bool OverridePrevious) {
   auto G = new GlobalVariable(type, isConst, name, M, initializer);
   if (loc)
      G->setSourceLoc(loc);

   if (M) {
      M->insertGlobal(G, OverridePrevious);
   }

   return G;
}

GlobalVariable* ILBuilder::CreateGlobalVariable(Constant *initializer,
                                                bool isConst,
                                                llvm::StringRef name,
                                                SourceLocation loc,
                                                bool OverridePrevious) {
   auto G = new GlobalVariable(initializer->getType(), isConst, name, M,
                               initializer);

   if (loc)
      G->setSourceLoc(loc);

   if (M)
      M->insertGlobal(G, OverridePrevious);

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
   case Intrinsic::strong_refcount:
   case Intrinsic::weak_refcount:
   case Intrinsic::vtable_ref:
   case Intrinsic::typeinfo_ref:
      assert(args.size() == 1 && args.front()->getType()->isClass());
      break;
   case Intrinsic::virtual_method:
      assert(args.size() == 2 && args.front()->getType()->isClass());
      assert(args[1]->getType()->isIntegerType());
      break;
   case Intrinsic::indirect_case_ref:
      assert(args.size() == 1 && args.front()->getType()->isEnum());
      break;
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end:
      assert(args.size() == 1);
      break;
   case Intrinsic::unbox:
      assert(args.size() == 1);
      assert(args.front()->getType()->isBoxType());
      break;
   case Intrinsic::typeinfo_cmp:
      assert(args.size() == 2);
      assert(isPointerLike(args[0]) && isPointerLike(args[1]));
      break;
   case Intrinsic::excn_object_ref:
   case Intrinsic::excn_typeinfo_ref:
   case Intrinsic::print_exception:
   case Intrinsic::cleanup_exception:
      assert(args.size() == 1);
      assert(isPointerLike(args[0]));
      break;
   case Intrinsic::print_runtime_error:
      assert(args.size() == 1);
      assert(support::isa<il::ConstantInt>(args.front()));
      break;
   case Intrinsic::terminate:
      assert(args.empty());
      break;
   case Intrinsic::begin_unsafe:
      assert(args.empty());
      break;
   case Intrinsic::end_unsafe:
      assert(args.size() == 1);
      assert(support::cast<Instruction>(args.front())->isBeginUnsafe());
      break;
   case Intrinsic::atomic_cmpxchg: {
      assert(args.size() == 5);
      assert(support::isa<ConstantInt>(args[4])
         && support::isa<ConstantInt>(args[3]));
      break;
   }
   case Intrinsic::atomic_rmw: {
      assert(args.size() == 4);
      assert(support::isa<ConstantInt>(args[3]));
      break;
   }
   case Intrinsic::coro_size:
      break;
   case Intrinsic::coro_suspend:
      assert(args.size() == 2
             && args.front()->getType()->isTokenType()
             && args[1]->getType()->isInt1Ty());
      break;
   case Intrinsic::coro_begin:
      assert(args.size() == 2
         && args.front()->getType()->isTokenType()
         && args[1]->getType()->isPointerType());
      break;
   case Intrinsic::coro_id:
      assert(args.size() == 1 && isPointerLike(args[0]));
      break;
   case Intrinsic::coro_end:
   case Intrinsic::coro_resume:
   case Intrinsic::coro_destroy:
   case Intrinsic::coro_save:
   case Intrinsic::coro_return:
      assert(args.size() == 1 && args.front()->getType()->isPointerType());
      break;
   case Intrinsic::coro_alloc:
      assert(args.size() == 1 && args.front()->getType()->isTokenType());
      break;
   case Intrinsic::coro_free:
      assert(args.size() == 2
             && args.front()->getType()->isTokenType()
             && args[1]->getType()->isPointerType());
      break;
   }
}

#endif

QualType getIntrinsicReturnType(ast::ASTContext &Ctx,
                                llvm::ArrayRef<Value *> args,
                                Intrinsic id) {
#ifndef NDEBUG
   checkIntrinsicArgs(id, args);
#endif

   switch (id) {
   case Intrinsic::memcpy:
   case Intrinsic::memset:
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end:
   case Intrinsic::__ctfe_stacktrace:
   case Intrinsic::print_exception:
   case Intrinsic::cleanup_exception:
   case Intrinsic::print_runtime_error:
   case Intrinsic::terminate:
   case Intrinsic::begin_unsafe:
   case Intrinsic::end_unsafe:
      return Ctx.getVoidType();
   case Intrinsic::memcmp:
      return Ctx.getInt32Ty();
   case Intrinsic::get_lambda_env:
      return Ctx.getMutableReferenceType(Ctx.getInt8PtrTy()->getPointerTo(Ctx));
   case Intrinsic::get_lambda_funcptr:
      return Ctx.getPointerType(Ctx.getInt8PtrTy());
   case Intrinsic::strong_refcount:
      return Ctx.getMutableReferenceType(Ctx.getUIntTy());
   case Intrinsic::weak_refcount:
      return Ctx.getMutableReferenceType(Ctx.getUIntTy());
   case Intrinsic::vtable_ref:
      return Ctx.getMutableReferenceType(Ctx.getInt8PtrTy());
   case Intrinsic::typeinfo_ref:
      return Ctx.getMutableReferenceType(Ctx.getInt8PtrTy());
   case Intrinsic::indirect_case_ref:
      return Ctx.getMutableReferenceType(Ctx.getInt8PtrTy());
   case Intrinsic::unbox:
      return Ctx.getMutableReferenceType(args.front()->getType()
                                             ->getBoxedType());
   case Intrinsic::typeinfo_cmp:
      return Ctx.getBoolTy();
   case Intrinsic::virtual_method:
      return Ctx.getUInt8PtrTy();
   case Intrinsic::excn_object_ref:
   case Intrinsic::excn_typeinfo_ref:
      return Ctx.getMutableReferenceType(Ctx.getInt8PtrTy());
   case Intrinsic::atomic_rmw:
      return args[2]->getType();
   case Intrinsic::atomic_cmpxchg:
      return args[2]->getType();
   case Intrinsic::coro_id:
   case Intrinsic::coro_save:
      return Ctx.getTokenType();
   case Intrinsic::coro_size:
      return Ctx.getUIntTy();
   case Intrinsic::coro_suspend:
      return Ctx.getUInt8Ty();
   case Intrinsic::coro_begin:
      return Ctx.getVoidType()->getPointerTo(Ctx);
   case Intrinsic::coro_end:
   case Intrinsic::coro_destroy:
   case Intrinsic::coro_resume:
   case Intrinsic::coro_return:
      return Ctx.getVoidType();
   case Intrinsic::coro_alloc:
      return Ctx.getBoolTy();
   case Intrinsic::coro_free:
      return Ctx.getInt8PtrTy();
   }
}

} // anonymous namespace

IntrinsicCallInst* ILBuilder::CreateIntrinsicCall(Intrinsic id,
                                                  llvm::ArrayRef<Value *> args,
                                                  llvm::StringRef name) {
   auto inst = new IntrinsicCallInst(
      id, ValueType(Ctx, getIntrinsicReturnType(ASTCtx, args, id)),
      args, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

LLVMIntrinsicCallInst*
ILBuilder::CreateLLVMIntrinsicCall(IdentifierInfo *intrinsic,
                                   QualType returnType,
                                   ArrayRef<il::Value *> args,
                                   StringRef name) {
   auto inst = new LLVMIntrinsicCallInst(intrinsic,
                                         ValueType(Ctx, returnType),
                                         args, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

Instruction* ILBuilder::GetStrongRefcount(il::Value *V,
                                          llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::strong_refcount, V, name);
}

Instruction* ILBuilder::GetWeakRefcount(il::Value *V,
                                        llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::weak_refcount, V, name);
}

Instruction* ILBuilder::GetVTable(il::Value *V,
                                  llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::vtable_ref, V, name);
}

Instruction* ILBuilder::GetTypeInfo(il::Value *V,
                                    llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::typeinfo_ref, V, name);
}

Instruction* ILBuilder::CreateRetain(il::Value *V,
                                     llvm::StringRef name) {
   auto inst = new StrongRetainInst(V, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Instruction* ILBuilder::CreateRelease(il::Value *V,
                                      llvm::StringRef name) {
   auto inst = new StrongReleaseInst(V, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Instruction* ILBuilder::CreateWeakRetain(il::Value *V,
                                         llvm::StringRef name) {
   auto inst = new WeakRetainInst(V, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Instruction* ILBuilder::CreateWeakRelease(il::Value *V,
                                          llvm::StringRef name) {
   auto inst = new WeakReleaseInst(V, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

Instruction* ILBuilder::CreateLifetimeBegin(il::Value *V,
                                            llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::lifetime_begin, V, name);
}

Instruction* ILBuilder::CreateLifetimeEnd(il::Value *V,
                                          llvm::StringRef name) {
   return CreateIntrinsicCall(Intrinsic::lifetime_end, V, name);
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

AllocaInst* ILBuilder::CreateAlloca(QualType ofType,
                                    unsigned int align,
                                    bool heap,
                                    bool IsLet,
                                    llvm::StringRef name) {
   auto inst = new AllocaInst(ValueType(Ctx, ofType), IsLet,
                              getInsertBlock(), align, heap);

   insertInstruction(inst, name);
   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(QualType ofType,
                                    Value *allocSize,
                                    unsigned int align,
                                    bool heap,
                                    bool IsLet,
                                    llvm::StringRef name) {
   auto inst = new AllocaInst(ValueType(Ctx, ofType), IsLet,
                              getInsertBlock(), allocSize, align, heap);

   insertInstruction(inst, name);
   return inst;
}

AllocaInst* ILBuilder::CreateAlloca(QualType ofType,
                                    size_t allocSize,
                                    unsigned int align,
                                    bool heap,
                                    bool IsLet,
                                    llvm::StringRef name) {
   auto size = GetConstantInt(ASTCtx.getUIntTy(), allocSize);
   auto inst = new AllocaInst(ValueType(Ctx, ofType), IsLet,
                              getInsertBlock(), size, align, heap);

   insertInstruction(inst, name);
   return inst;
}

Instruction* ILBuilder::AllocUninitialized(size_t size,
                                           unsigned int align,
                                           bool heap,
                                           llvm::StringRef name) {
   auto ArrTy = ASTCtx.getArrayType(ASTCtx.getUInt8Ty(), size);
   auto alloca = new AllocaInst(ValueType(Ctx, ArrTy), false,
                                getInsertBlock(),
                                nullptr, align, heap);

   insertInstruction(alloca, name);
   return CreateBitCast(CastKind::BitCast, CreateLoad(alloca),
                        ASTCtx.getInt8PtrTy());
}

AllocBoxInst *ILBuilder::CreateAllocBox(QualType Ty,
                                        Function *Deinitializer,
                                        llvm::StringRef name) {
   auto BoxTy = ASTCtx.getBoxType(Ty);
   auto inst = new AllocBoxInst(ValueType(Ctx, BoxTy), Deinitializer,
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

AssignInst* ILBuilder::CreateAssign(Value *val,
                                    Value *ptr,
                                    MemoryOrder memoryOrder,
                                    llvm::StringRef name) {
   auto inst = new AssignInst(ptr, val, getInsertBlock());
   inst->setMemoryOrder(memoryOrder);
   insertInstruction(inst, name);

   return inst;
}

StoreInst* ILBuilder::CreateStore(Value *val,
                                  Value *ptr,
                                  MemoryOrder memoryOrder,
                                  llvm::StringRef name) {
   auto inst = new StoreInst(ptr, val, getInsertBlock());
   inst->setMemoryOrder(memoryOrder);
   insertInstruction(inst, name);

   return inst;
}

InitInst* ILBuilder::CreateInit(Value *val,
                                Value *ptr,
                                llvm::StringRef name) {
   auto inst = new InitInst(ptr, val, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

MoveInst* ILBuilder::CreateMove(il::Value *Target, llvm::StringRef Name)
{
   auto inst = new MoveInst(Target, getInsertBlock());
   insertInstruction(inst, Name);

   return inst;
}

BeginBorrowInst* ILBuilder::CreateBeginBorrow(il::Value *Target,
                                              SourceLocation BeginLoc,
                                              SourceLocation EndLoc,
                                              bool Mutable,
                                              llvm::StringRef Name) {
   auto Inst = new BeginBorrowInst(Target, BeginLoc, EndLoc,
                                   Mutable, getInsertBlock());

   insertInstruction(Inst, Name);
   return Inst;
}

EndBorrowInst* ILBuilder::CreateEndBorrow(il::Value *Target,
                                          SourceLocation Loc,
                                          bool Mutable,
                                          llvm::StringRef Name) {
   auto Inst = new EndBorrowInst(Target, Loc, Mutable, getInsertBlock());
   insertInstruction(Inst, Name);

   return Inst;
}

FieldRefInst* ILBuilder::CreateFieldRef(Value *val,
                                        const DeclarationName &fieldName,
                                        bool IsLet,
                                        llvm::StringRef name) {
   auto inst = new FieldRefInst(val, fieldName, IsLet, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

GEPInst* ILBuilder::CreateStructGEP(il::Value *val,
                                    size_t idx,
                                    bool IsLet,
                                    llvm::StringRef name) {
   auto inst = new GEPInst(val, idx, IsLet, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

GEPInst* ILBuilder::CreateGEP(Value *val,
                              size_t idx,
                              bool IsLet,
                              llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto gep = new GEPInst(val, CI, IsLet, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

Instruction* ILBuilder::CreateExtractValue(Value *val,
                                           size_t idx, bool IsLet,
                                           llvm::StringRef name) {
   return CreateLoad(CreateGEP(val, idx, IsLet, name));
}

GEPInst* ILBuilder::CreateGEP(Value *val, Value *idx,
                              bool IsLet, llvm::StringRef name) {
   auto gep = new GEPInst(val, idx, IsLet, getInsertBlock());
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
                                                bool IsLet,
                                                llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), idx);

   auto gep = new TupleExtractInst(val, CI, IsLet, getInsertBlock());
   insertInstruction(gep, name);

   return gep;
}

EnumRawValueInst* ILBuilder::CreateEnumRawValue(Value *Val,
                                                bool LoadVal,
                                                llvm::StringRef name) {
   auto inst = new EnumRawValueInst(Val, LoadVal, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumExtractInst* ILBuilder::CreateEnumExtract(Value *Val,
                                              ast::EnumCaseDecl *Case,
                                              size_t caseVal, bool IsLet,
                                              llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), caseVal);

   auto inst = new EnumExtractInst(Val, Case, CI, IsLet, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

EnumExtractInst* ILBuilder::CreateEnumExtract(Value *Val,
                                              const IdentifierInfo *CaseName,
                                              size_t caseVal, bool IsLet,
                                              llvm::StringRef name) {
   auto CI = ConstantInt::get(ValueType(Ctx, ASTCtx.getUIntTy()), caseVal);

   auto E = support::cast<ast::EnumDecl>(Val->getType()->getRecord());
   auto inst = new EnumExtractInst(Val, E->hasCase(CaseName), CI, IsLet,
                                   getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

LoadInst* ILBuilder::CreateLoad(Value *val,
                                MemoryOrder memoryOrder,
                                llvm::StringRef name) {
   auto inst = new LoadInst(val, getInsertBlock());
   inst->setMemoryOrder(memoryOrder);
   insertInstruction(inst, name);

   return inst;
}

Instruction* ILBuilder::CreateAddrOf(Value *target,
                                    llvm::StringRef name) {
   auto inst = new AddrOfInst(target, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

Instruction* ILBuilder::CreatePtrToLvalue(Value *target, StringRef name)
{
   QualType PtrTy = target->getType();
   QualType RetTy;

   if (PtrTy->isMutablePointerType()) {
      RetTy = ASTCtx.getMutableReferenceType(PtrTy->getPointeeType());
   }
   else {
      RetTy = ASTCtx.getReferenceType(PtrTy->getPointeeType());
   }

   return CreateBitCast(CastKind::BitCast, target, RetTy);
}

StructInitInst* ILBuilder::CreateStructInit(ast::StructDecl *InitializedType,
                                            Method *Init,
                                            llvm::ArrayRef<Value *> args,
                                            bool Fallible,
                                            QualType FallibleTy,
                                            llvm::StringRef name) {
   auto inst = new StructInitInst(InitializedType, Init, args, Fallible,
                                  FallibleTy, getInsertBlock());

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
                                        ast::EnumCaseDecl *Case,
                                        llvm::ArrayRef<Value *> args,
                                        llvm::StringRef name) {
   auto inst = new EnumInitInst(Ctx, EnumTy, Case, args, getInsertBlock());
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

RetInst* ILBuilder::CreateRetVoid(llvm::StringRef name)
{
   auto inst = new RetInst(Ctx, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

YieldInst *ILBuilder::CreateYield(il::Value *Val,
                                  BasicBlock *ResumeDst,
                                  ArrayRef<Value*> ResumeArgs,
                                  bool FinalYield,
                                  StringRef name) {
   auto inst = new YieldInst(Val, ResumeDst, ResumeArgs,
                             FinalYield, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

YieldInst *ILBuilder::CreateYieldVoid(BasicBlock *ResumeDst,
                                      ArrayRef<Value*> ResumeArgs,
                                      bool FinalYield,
                                      StringRef name) {
   auto inst = new YieldInst(Ctx, ResumeDst, ResumeArgs,
                             FinalYield, getInsertBlock());

   insertInstruction(inst, name);
   return inst;
}

ThrowInst* ILBuilder::CreateThrow(Value *thrownVal, GlobalVariable *typeInfo,
                                  llvm::StringRef name) {
   auto inst = new ThrowInst(thrownVal, typeInfo, getInsertBlock());
   insertInstruction(inst, name);

   return inst;
}

RethrowInst* ILBuilder::CreateRethrow(il::Value *thrownVal,
                                      llvm::StringRef name) {
   auto inst = new RethrowInst(thrownVal, getInsertBlock());
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