//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "AST/ASTContext.h"
#include "Basic/CastKind.h"
#include "Compiler.h"
#include "Files/FileUtils.h"
#include "Files/FileManager.h"
#include "IL/Utils/BlockIterator.h"
#include "AST/Type.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/DIBuilder.h>

using namespace cdot::support;
using namespace cdot::fs;

namespace cdot {
namespace il {

IRGen::IRGen(CompilationUnit &CI,
             llvm::LLVMContext &Ctx,
             llvm::Module *M,
             bool emitDebugInfo)
   : CI(CI), TI(CI.getContext().getTargetInfo()), emitDebugInfo(emitDebugInfo),
     DI(nullptr), Ctx(Ctx), M(M), Builder(Ctx)
{
   WordTy = llvm::IntegerType::get(Ctx, sizeof(void*) * 8);
   Int8PtrTy = llvm::IntegerType::get(Ctx, 8)->getPointerTo();
   Int1Ty = llvm::IntegerType::get(Ctx, 1);
   VoidTy = llvm::Type::getVoidTy(Ctx);

   TypeInfoTy = llvm::StructType::create(Ctx, "cdot.TypeInfo");
   DeinitializerTy = llvm::FunctionType::get(VoidTy, { Int8PtrTy }, false);

   llvm::Type *typeInfoContainedTypes[] = {
      WordTy,                          // type id
      Int8PtrTy,                       // type name
      DeinitializerTy->getPointerTo(), // deinitializer
      TypeInfoTy->getPointerTo(),      // parent class
      TypeInfoTy->getPointerTo()       // conformances
                ->getPointerTo(),
   };

   TypeInfoTy->setBody(typeInfoContainedTypes);

   llvm::Type *classInfoContainedTypes[] = {
      Int8PtrTy,                  // vtable ptr
      TypeInfoTy->getPointerTo(), // type info
      WordTy                      // refcount
   };

   ClassInfoTy = llvm::StructType::create(Ctx, classInfoContainedTypes,
                                          "cdot.ClassInfo");

   llvm::Type *protocolContainedTypes[] = {
      Int8PtrTy,                 // object ptr
      Int8PtrTy,                 // ptable ptr
      TypeInfoTy->getPointerTo() // type info
   };

   ProtocolTy = llvm::StructType::create(Ctx, protocolContainedTypes,
                                         "cdot.Protocol");

   llvm::Type *lambdaContainedTypes[] = {
      Int8PtrTy,                // function ptr
      Int8PtrTy->getPointerTo() // env ptr
   };

   LambdaTy = llvm::StructType::create(Ctx, lambdaContainedTypes,
                                       "cdot.Lambda");
}

IRGen::~IRGen()
{

}

void IRGen::visitCompilationUnit(CompilationUnit &CU)
{
   if (emitDebugInfo) {
      assert(CU.getSourceLoc()
             && "translation unit with invalid source location");

      DI = new llvm::DIBuilder(*this->M);
      File = getFileDI(CU.getSourceLoc());
      this->CU = DI->createCompileUnit(
         llvm::dwarf::DW_LANG_C,
         File,
         "cdotc v0.1",
         false,
         "",
         0
      );

      beginScope(this->CU);
   }

   visitModule(*CU.getILModule());
   finalize(CU);

   if (DI) {
      endScope();

      delete DI;
      DI = nullptr;
   }
}

void IRGen::visitModule(Module& ILMod)
{
   this->ILMod = &ILMod;

   MallocFn = nullptr;
   FreeFn = nullptr;
   ThrowFn = nullptr;
   AllocExcFn = nullptr;
   RetainFn = nullptr;
   ReleaseFn = nullptr;
   PrintfFn = nullptr;
   MemCmpFn = nullptr;
   IntPowFn = nullptr;

   for (auto Ty : ILMod.getReferencedTypes()) {
      // forward declare all types, they might be referenced when defining
      // others
      ForwardDeclareType(Ty);
   }

   for (auto Ty : ILMod.getReferencedTypes())
      DeclareType(Ty);

   for (const auto &F : ILMod.getFuncList()) {
      DeclareFunction(&F);

      if (F.isGlobalInitFn()) {
         auto FnTy = llvm::FunctionType::get(Builder.getVoidTy(), false)
            ->getPointerTo();

         auto ctorStructTy =
            llvm::StructType::get(Builder.getInt32Ty(), FnTy,
                                  Int8PtrTy);

         auto ctorArrayTy = llvm::ArrayType::get(ctorStructTy, 1);
         auto ctors =
            new llvm::GlobalVariable(*M, ctorArrayTy, true,
                                     llvm::GlobalVariable::AppendingLinkage,
                                     nullptr, "llvm.global_ctors");

         ctors->setInitializer(llvm::ConstantArray::get(
            ctorArrayTy,
            llvm::ConstantStruct::get(ctorStructTy, Builder.getInt32(65535),
                                      M->getFunction(F.getName()),
                                      llvm::ConstantPointerNull::get(
                                         Int8PtrTy))));
      }
   }

   for (const auto &G : ILMod.getGlobalList())
      ForwardDeclareGlobal(&G);

   for (const auto &G : ILMod.getGlobalList())
      DeclareGlobal(&G);

   for (auto &F : ILMod.getFuncList())
      visitFunction(F);
}

llvm::Constant* IRGen::getMallocFn()
{
   if (!MallocFn)
      MallocFn = M->getOrInsertFunction("malloc",
                                        Int8PtrTy, WordTy);

   return MallocFn;
}

llvm::Constant* IRGen::getFreeFn()
{
   if (!FreeFn)
      FreeFn = M->getOrInsertFunction("free",
                                      VoidTy, Int8PtrTy);

   return FreeFn;
}

llvm::Constant* IRGen::getThrowFn()
{
   if (!ThrowFn)
      ThrowFn = M->getOrInsertFunction("__cdot_throw",
                                       VoidTy, Int8PtrTy);

   return ThrowFn;
}

llvm::Constant* IRGen::getAllocExcnFn()
{
   if (!AllocExcFn)
      AllocExcFn = M->getOrInsertFunction("__cdot_allocate_exception",
                                          Int8PtrTy, WordTy, Int8PtrTy,
                                          Int8PtrTy, Int8PtrTy);

   return AllocExcFn;
}

llvm::Constant* IRGen::getRetainFn()
{
   if (!RetainFn)
      RetainFn = M->getOrInsertFunction("__cdot_retain", VoidTy, Int8PtrTy);

   return RetainFn;
}

llvm::Constant* IRGen::getReleaseFn()
{
   if (!ReleaseFn)
      ReleaseFn = M->getOrInsertFunction("__cdot_release", VoidTy, Int8PtrTy);

   return ReleaseFn;
}

llvm::Constant* IRGen::getPrintfFn()
{
   if (!PrintfFn)
      PrintfFn = M->getOrInsertFunction("printf",
            llvm::FunctionType::get(Builder.getInt32Ty(), { Int8PtrTy }, true));

   return PrintfFn;
}

llvm::Constant* IRGen::getMemCmpFn()
{
   if (!MemCmpFn)
      MemCmpFn = M->getOrInsertFunction("memcmp",
                                        llvm::FunctionType::get(
                                           Builder.getInt32Ty(), { Int8PtrTy,
                                              Int8PtrTy, WordTy }, false));

   return MemCmpFn;
}

llvm::Constant* IRGen::getIntPowFn(QualType IntTy)
{
   auto llvmTy = getLlvmType(IntTy);
   llvm::StringRef funcName;

   if (IntTy->getBitwidth() == 128) {
      funcName = "__cdot_intpow_u128";
   }
   else {
      funcName = "__cdot_intpow";
   }

   return M->getOrInsertFunction(funcName,
                                 llvm::FunctionType::get(
                                    llvmTy, { llvmTy, llvmTy }, false));
}

llvm::StructType* IRGen::getStructTy(Type *Ty)
{
   if (Ty->isProtocol())
      return ProtocolTy;

   if (Ty->getClassName() == "cdot.Lambda")
      return LambdaTy;

   auto it = TypeMap.find((uintptr_t)Ty->getRecord());
   assert(it != TypeMap.end() && "referenced type was not imported");

   return it->getSecond();
}

llvm::StructType* IRGen::getStructTy(AggregateType *Ty)
{
   if (isa<ProtocolType>(Ty))
      return ProtocolTy;

   if (Ty->getName() == "cdot.Lambda")
      return LambdaTy;

   return Ty->getLlvmTy();
}

llvm::StructType* IRGen::getStructTy(llvm::StringRef name)
{
   auto ty = M->getTypeByName(name);
   assert(ty && "referenced type was not imported");

   return ty;
}

llvm::Type* IRGen::getLlvmType(Type *Ty)
{
   using TypeID = cdot::TypeID;

   switch (Ty->getTypeID()) {
      case TypeID::BuiltinTypeID:
         switch (Ty->asBuiltinType()->getKind()) {
            case BuiltinType::Void: return VoidTy;
#           define CDOT_BUILTIN_INT(Name, BW, isUnsigned)                     \
            case BuiltinType::Name: return Builder.getIntNTy(BW);
#           define CDOT_BUILTIN_FP(Name, Prec)                                \
            case BuiltinType::Name: return Prec == 32 ? Builder.getFloatTy()  \
                                                      : Builder.getDoubleTy();
#           include "Basic/BuiltinTypes.def"

            default:
               llvm_unreachable("bad builtin type");
         }
      case TypeID::PointerTypeID:
         return getLlvmType(Ty->getPointeeType())->getPointerTo();
      case TypeID::ReferenceTypeID: {
         QualType referenced = Ty->asReferenceType()->getReferencedType();
         if (referenced->needsStructReturn())
            return getLlvmType(referenced);

         return getLlvmType(referenced)->getPointerTo();
      }
      case TypeID::ArrayTypeID: {
         ArrayType *ArrTy = Ty->asArrayType();
         return llvm::ArrayType::get(getLlvmType(ArrTy->getElementType()),
                                     ArrTy->getNumElements());
      }
      case TypeID::LambdaTypeID:
         return LambdaTy;
      case TypeID::FunctionTypeID: {
         FunctionType *FuncTy = Ty->asFunctionType();
         auto ret = getLlvmType(*FuncTy->getReturnType());
         llvm::SmallVector<llvm::Type*, 4> argTypes;

         for (const auto &arg : FuncTy->getParamTypes()) {
            auto ty = getLlvmType(arg);
            if (ty->isStructTy())
               ty = ty->getPointerTo();

            argTypes.push_back(ty);
         }

         return llvm::FunctionType::get(ret, argTypes,
                                        FuncTy->isCStyleVararg())
            ->getPointerTo();
      }
      case TypeID::TupleTypeID: {
         TupleType *TupleTy = Ty->asTupleType();
         llvm::SmallVector<llvm::Type*, 4> argTypes;

         for (const auto &cont : TupleTy->getContainedTypes())
            argTypes.push_back(getLlvmType(cont));

         return llvm::StructType::get(Ctx, argTypes);
      }
      case TypeID::RecordTypeID: {
         if (Ty->isRefcounted())
            return getStructTy(Ty)->getPointerTo();

         if (Ty->isRawEnum())
            return getLlvmType(
               cast<EnumType>(ILMod->getType(Ty->getClassName()))
                  ->getRawType());

         return getStructTy(Ty);
      }
      default:
         llvm_unreachable("type should note be possible here");
   }
}

llvm::Type* IRGen::getLlvmType(QualType Ty)
{
   return getLlvmType(*Ty);
}

llvm::Type* IRGen::getFieldType(ValueType Ty)
{
   return getLlvmType(Ty);
}

void IRGen::DeclareFunction(il::Function const* F)
{
   if (auto M = dyn_cast<il::Method>(F))
      if (M->isProtocolMethod())
         return;

   auto funcTy = F->getType()->asFunctionType();

   llvm::SmallVector<llvm::Type*, 8> argTypes;
   llvm::Type *retType;

   bool sret = false;
   unsigned sretIdx = 0;

   if (F->isLambda()) {
      argTypes.push_back(Int8PtrTy->getPointerTo());
      ++sretIdx;
   }

   if (F->hasStructReturn()) {
      auto sretTy = getLlvmType(funcTy->getReturnType());
      if (sretTy->isStructTy() || sretTy->isArrayTy())
         sretTy = sretTy->getPointerTo();

      sret = true;
      argTypes.push_back(sretTy);
      retType = VoidTy;
   }
   else {
      retType = getLlvmType(funcTy->getReturnType());
   }

   for (const auto &arg : funcTy->getParamTypes()) {
      auto argTy = getLlvmType(arg);
      if (argTy->isStructTy() || argTy->isArrayTy())
         argTy = argTy->getPointerTo();

      argTypes.push_back(argTy);
   }

   auto linkage = F->isGlobalInitFn()
                  ? llvm::Function::InternalLinkage
                  : llvm::Function::ExternalLinkage;

   auto llvmTy = llvm::FunctionType::get(retType, argTypes,
                                         funcTy->isCStyleVararg());

   auto fn = llvm::Function::Create(llvmTy, linkage, F->getName(), M);

   if (sret) {
      fn->addAttribute(sretIdx + 1, llvm::Attribute::StructRet);
   }

   if (DI)
      emitFunctionDI(*F, fn);
}

llvm::Value* IRGen::getCurrentSRetValue()
{
   auto fn = Builder.GetInsertBlock()->getParent();
   assert(!fn->arg_empty());

   auto it = fn->arg_begin();

   // lambda environment comes before sret value
   if (!it->hasAttribute(llvm::Attribute::StructRet))
      ++it;

   assert(it->hasAttribute(llvm::Attribute::StructRet));
   return &*it;
}

void IRGen::ForwardDeclareType(il::AggregateType const *Ty)
{
   if (isa<ProtocolType>(Ty))
      return;

   llvm::SmallString<128> typeName;
   switch (Ty->getTypeID()) {
      case Value::TypeID::ClassTypeID:
         typeName += "class";
         break;
      case Value::TypeID::StructTypeID:
         typeName += "struct";
         break;
      case Value::TypeID::EnumTypeID:
         typeName += "enum";
         break;
      case Value::TypeID::UnionTypeID:
         typeName += "union";
         break;
      default:
         llvm_unreachable("bad type kind");
   }

   typeName += '.';
   typeName += Ty->getName();

   Ty->setLlvmTy(llvm::StructType::create(Ctx, typeName));

   TypeMap.try_emplace((uintptr_t)Ty->getType()->getRecord(), Ty->getLlvmTy());
}

void IRGen::DeclareType(il::AggregateType const* Ty)
{
   if (isa<ProtocolType>(Ty))
      return;

   llvm::SmallVector<llvm::Type*, 8> ContainedTypes;

   if (auto S = dyn_cast<StructType>(Ty)) {
      for (const auto &F : S->getFields()) {
         auto fieldTy = getFieldType(F.type);
         ContainedTypes.push_back(fieldTy);
      }
   }
   else if (auto E = dyn_cast<EnumType>(Ty)) {
      ContainedTypes.push_back(getLlvmType(E->getRawType()));
      for (size_t i = 0; i < E->getMaxAssociatedValues(); ++i)
         ContainedTypes.push_back(Int8PtrTy);
   }
   else if (auto U = dyn_cast<UnionType>(Ty)) {
      ContainedTypes.push_back(Int8PtrTy);
   }

   if (ContainedTypes.empty())
      ContainedTypes.push_back(Int8PtrTy);

   Ty->getLlvmTy()->setBody(ContainedTypes);
}

void IRGen::ForwardDeclareGlobal(il::GlobalVariable const *G)
{
   auto globalTy = getLlvmType(G->getType()->getReferencedType());
   new llvm::GlobalVariable(*M, globalTy,
                            G->isConstant() && !G->isLateInitialized(),
                            llvm::GlobalVariable::ExternalLinkage,
                            nullptr, G->getName());
}

void IRGen::DeclareGlobal(il::GlobalVariable const *G)
{
   auto glob = M->getNamedGlobal(G->getName());
   if (auto Init = G->getInitializer())
      glob->setInitializer(getConstantVal(Init));
   else if (G->isLateInitialized())
      glob->setInitializer(llvm::ConstantAggregateZero::get(
         glob->getType()->getPointerElementType()));
}

void IRGen::visitFunction(Function &F)
{
   if (F.isDeclared())
      return;

   auto func = M->getFunction(F.getName());
   assert(func && "func not declared?");

   if (DI) {
      auto Sub = DIFuncMap.find((uintptr_t)&F);
      assert(Sub != DIFuncMap.end());

      beginScope(Sub->getSecond());
   }

   auto AllocaBB = llvm::BasicBlock::Create(Ctx, "alloca_block", func);

   for (auto &BB : F.getBasicBlocks())
      llvm::BasicBlock::Create(Ctx, BB.getName(), func);

   for (auto &BB : F.getBasicBlocks())
      visitBasicBlock(BB);

   for (auto &B : F.getBasicBlocks()) {
      if (!B.isEntryBlock() && !B.getArgs().empty()) {
         for (size_t i = 0; i < B.getArgs().size(); ++i) {
            auto llvmBB = getBasicBlock(&B);
            auto val = &llvmBB->getInstList().front();
            for (size_t j = 0; j < i; ++j)
               val = val->getNextNode();

            auto phi = cast<llvm::PHINode>(val);
            for (const auto &Pred : getPredecessors(B)) {
               auto Term = Pred.getTerminator();
               assert(Term && "no terminator for basic block");
               Value *PassedVal;

               if (auto Br = dyn_cast<BrInst>(Term)) {
                  PassedVal = &B == Br->getTargetBranch()
                              ? Br->getTargetArgs()[i]
                              : Br->getElseArgs()[i];
               }
               else {
                  llvm_unreachable("bad terminator kind");
               }

               phi->addIncoming(getLlvmValue(PassedVal),
                                getBasicBlock(Pred.getName()));
            }
         }
      }
   }

   ValueMap.clear();

   Builder.SetInsertPoint(AllocaBB);
   Builder.CreateBr(getBasicBlock(&F.getBasicBlocks().front()));

   if (DI) {
      emitArgumentDI(F, func);
      endScope();
   }
}

void IRGen::visitBasicBlock(BasicBlock &B)
{
   Builder.SetCurrentDebugLocation(llvm::DebugLoc());
   Builder.SetInsertPoint(getBasicBlock(&B));

   if (!B.isEntryBlock() && !B.getArgs().empty()) {
      for (const auto &arg : B.getArgs()) {
         auto ty = getLlvmType(arg.getType());
         if (ty->isStructTy())
            ty = ty->getPointerTo();

         Builder.CreatePHI(ty, 0);
      }
   }

   for (auto const& I : B.getInstructions()) {
      if (DI) {
         auto Loc = I.getSourceLoc();
         auto LineAndCol = CI.getFileMgr().getLineAndCol(Loc);

         Builder.SetCurrentDebugLocation(llvm::DebugLoc::get(
            LineAndCol.line,
            LineAndCol.col,
            ScopeStack.top()
         ));
      }

      auto val = visit(I);
      auto res = ValueMap.try_emplace(&I, val);
      assert(res.second && "duplicate name");

      if (DI && I.isInitializer())
         emitLocalVarDI(I, val);
   }

//   assert(Builder.GetInsertBlock()->getTerminator()
//          && "basic block not terminated!");
}

llvm::BasicBlock *IRGen::getBasicBlock(llvm::StringRef name)
{
   assert(Builder.GetInsertBlock());
   auto func = Builder.GetInsertBlock()->getParent();

   for (auto &BB : func->getBasicBlockList())
      if (BB.getName() == name)
         return &BB;

   return nullptr;
}

llvm::BasicBlock* IRGen::getBasicBlock(BasicBlock *BB)
{
   auto func = getFunction(BB->getParent());

   for (auto &block : func->getBasicBlockList())
      if (block.getName() == BB->getName())
         return &block;

   return nullptr;
}

llvm::Function* IRGen::getFunction(il::Function *F)
{
   return M->getFunction(F->getName());
}

llvm::ConstantInt* IRGen::wordSizedInt(uint64_t val)
{
   return llvm::ConstantInt::get(WordTy, val);
}

llvm::Value* IRGen::toInt8Ptr(llvm::Value *V)
{
   return Builder.CreateBitCast(V, Int8PtrTy);
}

llvm::Value* IRGen::CreateCall(il::Function *F,
                               llvm::SmallVector<llvm::Value*, 8> &args) {
   auto fun = getFunction(F);
   llvm::Value *Ret = nullptr;

   if (F->hasStructReturn()) {
      auto alloca = CreateAlloca(getLlvmType(*F->getReturnType()));

      args.insert(args.begin(), alloca);
      Ret = alloca;
   }

   auto call = Builder.CreateCall(fun, args);
   if (!Ret)
      Ret = call;

   return Ret;
}

llvm::Value* IRGen::CreateAlloca(QualType AllocatedType,
                                 size_t allocatedSize,
                                 unsigned int alignment) {
   if (!alignment)
      alignment = TI.getAlignOfType(AllocatedType);

   return CreateAlloca(getLlvmType(AllocatedType), allocatedSize, alignment);
}

llvm::Value* IRGen::CreateAlloca(llvm::Type *AllocatedType,
                                 size_t allocatedSize,
                                 unsigned alignment) {
   auto EB = &Builder.GetInsertBlock()->getParent()->getEntryBlock();
   auto IP = Builder.saveIP();

   Builder.SetInsertPoint(EB);

   llvm::AllocaInst *alloca;
   if (allocatedSize > 1)
      alloca = Builder.CreateAlloca(AllocatedType, wordSizedInt(allocatedSize));
   else
      alloca = Builder.CreateAlloca(AllocatedType);

   alloca->setAlignment(alignment);

   Builder.restoreIP(IP);
   return alloca;
}

void IRGen::debugPrint(const llvm::Twine &str)
{
   Builder.CreateCall(getPrintfFn(),
                      { toInt8Ptr(Builder.CreateGlobalString(str.str())) });
}

llvm::Value* IRGen::AccessField(StructType *Ty,
                                Value *Val,
                                llvm::StringRef fieldName,
                                bool load) {
   auto offset = Ty->getFieldOffset(fieldName);
   auto val = Builder.CreateStructGEP(getStructTy(Ty), getLlvmValue(Val),
                                      offset);

   return load ? (llvm::Value*)Builder.CreateLoad(val) : val;
}

llvm::Value* IRGen::getLlvmValue(il::Value const *V)
{
   if (auto B = dyn_cast<BasicBlock>(V))
      return getBasicBlock(B->getName());

   if (auto C = dyn_cast<Constant>(V))
      return getConstantVal(C);

   if (auto A = dyn_cast<Argument>(V))
      return getBlockArg(A);

   auto it = ValueMap.find(V);
   assert(it != ValueMap.end());

   return it->second;
}

llvm::Constant* IRGen::getConstantVal(il::Constant const* C)
{
   if (auto Int = dyn_cast<ConstantInt>(C)) {
      if (Int->isCTFE())
         return Builder.getFalse();

      return llvm::ConstantInt::get(getLlvmType(Int->getType()),
                                    Int->getValue());
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      if (F->getType()->isFloatTy())
         return llvm::ConstantFP::get(Builder.getFloatTy(),
                                      F->getFloatVal());

      return llvm::ConstantFP::get(Builder.getDoubleTy(),
                                   F->getDoubleVal());
   }

   if (auto S = dyn_cast<ConstantString>(C)) {
      auto StrConstant = llvm::ConstantDataArray::getString(Ctx, S->getValue());
      auto GV = new llvm::GlobalVariable(*M, StrConstant->getType(), true,
                                         llvm::GlobalValue::PrivateLinkage,
                                         StrConstant);

      GV->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
      return llvm::ConstantExpr::getBitCast(GV, Int8PtrTy);
   }

   if (auto A = dyn_cast<ConstantArray>(C)) {
      llvm::SmallVector<llvm::Constant*, 8> Els;
      for (const auto &el : A->getVec()) {
         auto V = getConstantVal(el);
         Els.push_back(V);
      }

      return llvm::ConstantArray::get(
         cast<llvm::ArrayType>(getLlvmType(A->getType())), Els);
   }

   if (auto S = dyn_cast<ConstantStruct>(C)) {
      llvm::SmallVector<llvm::Constant*, 8> Vals;
      for (const auto &el : S->getElements())
         Vals.push_back(getConstantVal(el));

      return llvm::ConstantStruct::get(cast<llvm::StructType>(
         getLlvmType(S->getType())), Vals);
   }

   if (auto F = dyn_cast<il::Function>(C))
      return M->getFunction(F->getName());

   if (auto P = dyn_cast<ConstantPointer>(C)) {
      assert(P->getValue() == 0);
      auto ty = getLlvmType(P->getType());
      if (!ty->isPointerTy())
         ty = ty->getPointerTo();

      return llvm::ConstantPointerNull::get(cast<llvm::PointerType>(ty));
   }

   if (auto G = dyn_cast<GlobalVariable>(C)) {
      return M->getNamedGlobal(G->getName());
   }

   if (auto BC = dyn_cast<ConstantBitCastInst>(C)) {
      return llvm::ConstantExpr::getBitCast(getConstantVal(BC->getTarget()),
                                            getLlvmType(BC->getType()));
   }

   if (auto AddrOf = dyn_cast<ConstantAddrOfInst>(C)) {
      return getConstantVal(AddrOf->getTarget());
   }

   llvm_unreachable("bad constant kind");
}

llvm::Value* IRGen::getBlockArg(il::Argument const* A)
{
   auto BB = A->getParent();
   auto Fn = BB->getParent();
   auto hasSret = BB->getParent()->hasStructReturn();

   // function argument
   if (BB == A->getParent()->getParent()->getEntryBlock()) {
      auto it = Builder.GetInsertBlock()->getParent()->arg_begin();
      auto arg_it = A->getParent()->arg_begin();

      if (hasSret)
         ++it;
      if (Fn->isLambda())
         ++it;

      while (&*arg_it != A) {
         ++arg_it;
         ++it;
      }

      return &*it;
   }
   else {
      auto inst_it = Builder.GetInsertBlock()->begin();
      auto arg_it = A->getParent()->arg_begin();

      while (&*arg_it != A) {
         ++arg_it;
         ++inst_it;
      }

      auto phi = &*inst_it;
      assert(isa<llvm::PHINode>(phi));

      return phi;
   }
}

llvm::Value *IRGen::visitAllocaInst(AllocaInst const& I)
{
   if (I.canUseSRetValue())
      return getCurrentSRetValue();

   unsigned alignment = I.getAlignment();
   if (!alignment) {
      alignment = TI.getAlignOfType(I.getType()->getReferencedType());
   }

   llvm::Type *allocatedType = getLlvmType(I.getType()->getReferencedType());

   llvm::Value *V;
   if (I.isHeapAlloca()) {
      auto TypeSize = TI.getAllocSizeOfType(I.getType()->getReferencedType());
      auto size = wordSizedInt(TypeSize * I.getAllocSize());
      auto buff = Builder.CreateCall(getMallocFn(), { size });

      V = Builder.CreateBitCast(buff, allocatedType->getPointerTo());
   }
   else {
      V = CreateAlloca(allocatedType, I.getAllocSize(), alignment);
   }

   return V;
}

llvm::Value *IRGen::visitLambdaInitInst(LambdaInitInst const& I)
{
   auto fun = getLlvmValue(I.getFunction());
   llvm::Value *env;
   if (I.getNumOperands()) {
      env = Builder.CreateCall(getMallocFn(),
                               wordSizedInt(I.getNumOperands()
                                              * sizeof(void*)));

      env = Builder.CreateBitCast(env, Int8PtrTy->getPointerTo());
   }
   else {
      env = llvm::ConstantPointerNull::get(Int8PtrTy->getPointerTo());
   }

   size_t i = 0;
   for (const auto &capture : I.getOperands()) {
      auto val = toInt8Ptr(getLlvmValue(capture));
      auto ptr = Builder.CreateInBoundsGEP(env, wordSizedInt(i));

      Builder.CreateStore(val, ptr);
   }

   auto alloca = CreateAlloca(LambdaTy, 1, TI.getPointerAlignInBytes());

   auto funPtr = Builder.CreateStructGEP(LambdaTy, alloca, 0);
   Builder.CreateStore(toInt8Ptr(fun), funPtr);

   auto envPtr = Builder.CreateStructGEP(LambdaTy, alloca, 1);
   Builder.CreateStore(env, envPtr);

   return alloca;
}

llvm::Value * IRGen::visitStoreInst(StoreInst const& I)
{
   auto Dst = getLlvmValue(I.getDst());
   auto Src = getLlvmValue(I.getSrc());
   auto Ty = I.getSrc()->getType();

   unsigned alignment = 0;
   if (auto Alloca = dyn_cast<llvm::AllocaInst>(Dst)) {
      alignment = Alloca->getAlignment();
   }
   else {
      alignment = TI.getAlignOfType(Ty);
   }

   if (Ty->needsStructReturn()) {
      return Builder.CreateMemCpy(Dst, Src, TI.getSizeOfType(Ty),
                                  alignment);
   }

   auto Store = Builder.CreateStore(Src, Dst);
   Store->setAlignment(alignment);

   return Store;
}

llvm::Value *IRGen::visitGEPInst(GEPInst const& I)
{
   auto val = getLlvmValue(I.getVal());
   if (auto AggrTy = I.getAggrTy()) {
      assert(isa<ConstantInt>(I.getIndex()));
      return Builder.CreateStructGEP(getStructTy(AggrTy), val,
                                     cast<ConstantInt>(I.getIndex())->getU32());
   }
   else if (val->getType()->getPointerElementType()->isArrayTy()) {
      return Builder.CreateInBoundsGEP(val, {
         wordSizedInt(0),
         getLlvmValue(I.getIndex())
      });
   }
   else {
      return Builder.CreateInBoundsGEP(val, getLlvmValue(I.getIndex()));
   }
}

llvm::Value * IRGen::visitCaptureExtractInst(const CaptureExtractInst &I)
{
   auto F = Builder.GetInsertBlock()->getParent();
   auto env = F->arg_begin();

   auto gep = Builder.CreateInBoundsGEP(env, { getLlvmValue(I.getIdx()) });
   auto ld = Builder.CreateLoad(gep);

   auto ty = getLlvmType(I.getType());
   if (!ty->isPointerTy())
      ty = ty->getPointerTo();

   return Builder.CreateBitCast(ld, ty);
}

llvm::Value *IRGen::visitFieldRefInst(FieldRefInst const& I)
{
   return AccessField(I.getAccessedType(), I.getOperand(0), I.getFieldName());
}

llvm::Value * IRGen::visitTupleExtractInst(TupleExtractInst const& I)
{
   auto tup = getLlvmValue(I.getOperand(0));
   return Builder.CreateStructGEP(tup->getType()->getPointerElementType(),
                                  tup, I.getIdx()->getU32());
}

llvm::Value* IRGen::visitEnumExtractInst(const EnumExtractInst &I)
{
   auto &Case = I.getEnumTy()->getCase(I.getCaseName());
   auto E = getLlvmValue(I.getOperand(0));
   auto gep = Builder.CreateStructGEP(getStructTy(I.getEnumTy()), E,
                                      I.getCaseVal()->getU32() + 1);

   auto caseTy = Case.AssociatedTypes[I.getCaseVal()->getU32()];
   auto llvmCaseTy = getLlvmType(caseTy);

   if (!llvmCaseTy->isPointerTy())
      llvmCaseTy = llvmCaseTy->getPointerTo();

   if (caseTy->isIntegerType() || caseTy->isPointerType()
       || caseTy->isRawEnum())
      return Builder.CreateBitCast(gep, llvmCaseTy);

   if (caseTy->needsStructReturn())
      gep = Builder.CreateLoad(gep);
   else
      llvmCaseTy = llvmCaseTy->getPointerTo();

   return Builder.CreateBitCast(gep, llvmCaseTy);
}

llvm::Value* IRGen::visitEnumRawValueInst(EnumRawValueInst const& I)
{
   auto E = getLlvmValue(I.getValue());
   if (E->getType()->isIntegerTy())
      return E;

   return Builder.CreateLoad(Builder.CreateStructGEP(E->getType()
                                             ->getPointerElementType(), E, 0));
}

llvm::Value * IRGen::visitLoadInst(LoadInst const& I)
{
   auto val = getLlvmValue(I.getTarget());
   auto pointee = val->getType()->getPointerElementType();
   if (pointee->isStructTy() || pointee->isArrayTy())
      return val;

   unsigned alignment = 0;
   if (auto Alloca = dyn_cast<llvm::AllocaInst>(val)) {
      alignment = Alloca->getAlignment();
   }
   else {
      alignment = TI.getAlignOfType(I.getTarget()->getType()
                                     ->getReferencedType());
   }

   auto Ld = Builder.CreateLoad(val);
   Ld->setAlignment(alignment);

   return Ld;
}

llvm::Value* IRGen::visitAddrOfInst(AddrOfInst const& I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value * IRGen::visitPtrToLvalueInst(const PtrToLvalueInst &I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitRetInst(RetInst const& I)
{
   if (I.canUseSRetValue())
      return Builder.CreateRetVoid();

   if (auto V = I.getReturnedValue()) {
      auto retVal = getLlvmValue(V);
      if (retVal->getType()->isVoidTy())
         return Builder.CreateRetVoid();

      auto func = I.getParent()->getParent();
      if (func->hasStructReturn()) {
         auto argIt = Builder.GetInsertBlock()->getParent()->arg_begin();
         if (I.getParent()->getParent()->isLambda())
            ++argIt;

         auto sretVal = &*argIt;
         Builder.CreateMemCpy(sretVal, retVal,
                              TI.getSizeOfType(V->getType()),
                              TI.getAlignOfType(V->getType()));
      }
      else {
         return Builder.CreateRet(retVal);
      }
   }

   return Builder.CreateRetVoid();
}

llvm::Value* IRGen::visitThrowInst(ThrowInst const& I)
{
   auto thrownVal = getLlvmValue(I.getThrownValue());
   llvm::Value *descFn;

   if (auto Fn = I.getDescFn()) {
      descFn = toInt8Ptr(getLlvmValue(Fn));
   }
   else {
      descFn = llvm::ConstantPointerNull::get(Int8PtrTy);
   }

   auto exc = Builder.CreateCall(getAllocExcnFn(), {
      wordSizedInt(TI.getSizeOfType(I.getThrownValue()->getType())),
      toInt8Ptr(thrownVal),
      toInt8Ptr(getLlvmValue(I.getTypeInfo())),
      descFn
   });

   Builder.CreateCall(getThrowFn(), { exc });
   return Builder.CreateUnreachable();
}

llvm::Value* IRGen::visitUnreachableInst(UnreachableInst const& I)
{
   return Builder.CreateUnreachable();
}

llvm::Value* IRGen::visitBrInst(BrInst const& I)
{
   if (!I.getCondition())
      return Builder.CreateBr(getBasicBlock(I.getTargetBranch()));

   return Builder.CreateCondBr(getLlvmValue(I.getCondition()),
                               getBasicBlock(I.getTargetBranch()),
                               getBasicBlock(I.getElseBranch()));
}

llvm::Value* IRGen::visitSwitchInst(SwitchInst const& I)
{
   llvm::BasicBlock *DefaultBB = nullptr;
   if (auto BB = I.getDefault()) {
      DefaultBB = getBasicBlock(BB);
   }
   else {
      auto IP = Builder.saveIP();
      auto unreachableBB = llvm::BasicBlock::Create(Ctx, "",
                                        Builder.GetInsertBlock()->getParent());

      Builder.SetInsertPoint(unreachableBB);
      Builder.CreateUnreachable();

      Builder.restoreIP(IP);
      DefaultBB = unreachableBB;
   }

   auto SwitchInst = Builder.CreateSwitch(getLlvmValue(I.getSwitchVal()),
                                          DefaultBB,
                                          unsigned(I.getCases().size()));

   for (const auto &C : I.getCases()) {
      if (!C.first)
         continue;

      SwitchInst->addCase(cast<llvm::ConstantInt>(getLlvmValue(C.first)),
                          getBasicBlock(C.second));
   }

   return SwitchInst;
}

llvm::Value* IRGen::visitInvokeInst(InvokeInst const& I)
{
   auto Fun = getFunction(I.getCalledFunction());
   llvm::SmallVector<llvm::Value*, 8> args;

   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   return Builder.CreateInvoke(Fun, getBasicBlock(I.getNormalContinuation()),
                               getBasicBlock(I.getLandingPad()), args);
}

llvm::Value* IRGen::visitVirtualInvokeInst(const VirtualInvokeInst &I)
{
   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   auto Self = args.front();
   auto StructTy = cast<llvm::StructType>(Self->getType()
                                              ->getPointerElementType());

   auto ClassInfoPtr = Builder.CreateLoad(Builder.CreateStructGEP(StructTy,
                                                                  Self, 0));

   auto VTablePtr = Builder.CreateLoad(Builder.CreateStructGEP(ClassInfoTy,
                                                               ClassInfoPtr,
                                                               0));

   auto GEP = Builder.CreateInBoundsGEP(VTablePtr,
                                        Builder.getInt64(I.getCalledMethod()
                                                          ->getVtableOffset()));

   auto funcPtr = Builder.CreateBitCast(GEP, getLlvmType(I.getCalledMethod()
                                                          ->getType()));

   return Builder.CreateInvoke(funcPtr,
                               getBasicBlock(I.getNormalContinuation()),
                               getBasicBlock(I.getLandingPad()), args);
}

llvm::Value * IRGen::visitProtocolInvokeInst(const ProtocolInvokeInst &I)
{
   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   auto Self = args.front();
   auto PTablePtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                               Self, 0));

   auto GEP = Builder.CreateInBoundsGEP(PTablePtr,
                                        Builder.getInt64(I.getCalledMethod()
                                                          ->getPtableOffset()));

   auto funcPtr = Builder.CreateBitCast(GEP, getLlvmType(I.getCalledMethod()
                                                          ->getType()));

   auto ObjPtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                            Self, 0));
   auto Obj = Builder.CreateBitCast(ObjPtr, getLlvmType(I.getCalledMethod()
                                                         ->getReturnType()));

   args[0] = Obj;
   return Builder.CreateInvoke(funcPtr,
                               getBasicBlock(I.getNormalContinuation()),
                               getBasicBlock(I.getLandingPad()), args);
}

llvm::Value* IRGen::visitLandingPadInst(LandingPadInst const& I)
{
   llvm_unreachable("NOT YET");
}

llvm::Value* IRGen::visitIntrinsicCallInst(IntrinsicCallInst const& I)
{
   switch (I.getCalledIntrinsic()) {
   case Intrinsic::__ctfe_stacktrace:
      return nullptr;
   case Intrinsic::memcpy:
   case Intrinsic::memset:
   case Intrinsic::memcmp: {
      llvm::Value *Args[] = {
         getLlvmValue(I.getOperand(0)),
         getLlvmValue(I.getOperand(1)),
         getLlvmValue(I.getOperand(2))
      };

      if (I.getCalledIntrinsic() == Intrinsic::memcpy) {
         return Builder.CreateMemCpy(Args[0], Args[1], Args[2], 1);
      }
      else if (I.getCalledIntrinsic() == Intrinsic::memcmp) {
         auto cmp = Builder.CreateCall(getMemCmpFn(), {
            toInt8Ptr(Args[0]), toInt8Ptr(Args[1]), Args[2]
         });
         return Builder.CreateIsNull(cmp);
      }
      else {
         return Builder.CreateMemSet(Args[0], Args[1], Args[2], 1);
      }
   }
   case Intrinsic::release:
      return Builder.CreateCall(getReleaseFn(), {
         toInt8Ptr(getLlvmValue(I.getOperand(0)))
      });
   case Intrinsic::retain:
      return Builder.CreateCall(getRetainFn(), {
         toInt8Ptr(getLlvmValue(I.getOperand(0)))
      });
   case Intrinsic::lifetime_begin:
      return Builder.CreateLifetimeStart(getLlvmValue(I.getArgs()[0]),
                      cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));
   case Intrinsic::lifetime_end: {
      auto Self = getLlvmValue(I.getArgs()[0]);
      Builder.CreateLifetimeEnd(Self,
                      cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));
//
//      if (I.getArgs().front()->getType()->isClass())
//         Builder.CreateCall(getFreeFn(), toInt8Ptr(Self));

      return nullptr;
   }
   case Intrinsic::get_lambda_env: {
      auto lambda = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(LambdaTy, lambda, 1);
   }
   case Intrinsic::get_lambda_funcptr: {
      auto lambda = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(LambdaTy, lambda, 0);
   }
   case Intrinsic::strong_refcount: {
      auto val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(val->getType()->getPointerElementType(),
                                     val, 0);
   }
   case Intrinsic::weak_refcount: {
      auto val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(val->getType()->getPointerElementType(),
                                     val, 1);
   }
   case Intrinsic::vtable_ref: {
      auto val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(val->getType()->getPointerElementType(),
                                     val, 2);
   }
   case Intrinsic::typeinfo_ref: {
      auto val = getLlvmValue(I.getArgs()[0]);
      auto vtbl = Builder.CreateStructGEP(val->getType()
                                             ->getPointerElementType(),
                                          val, 2);

      return Builder.CreateConstInBoundsGEP2_64(vtbl, 0, 0);
   }
   }
}

llvm::Value* IRGen::visitCallInst(CallInst const& I)
{
   llvm::SmallVector<llvm::Value*, 8> args;

   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   return CreateCall(I.getCalledFunction(), args);
}

llvm::Value * IRGen::visitVirtualCallInst(const VirtualCallInst &I)
{
   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   auto Self = args.front();
   auto StructTy = cast<llvm::StructType>(Self->getType()
                                              ->getPointerElementType());

   auto ClassInfoPtr = Builder.CreateLoad(Builder.CreateStructGEP(StructTy,
                                                                  Self, 0));

   auto VTablePtr = Builder.CreateLoad(Builder.CreateStructGEP(ClassInfoTy,
                                                               ClassInfoPtr,
                                                               0));

   auto GEP = Builder.CreateInBoundsGEP(VTablePtr,
                                        Builder.getInt64(I.getCalledMethod()
                                                          ->getVtableOffset()));

   auto funcPtr = Builder.CreateBitCast(GEP, getLlvmType(I.getCalledMethod()
                                                          ->getType()));

   return Builder.CreateCall(funcPtr, args);
}

llvm::Value* IRGen::visitProtocolCallInst(const ProtocolCallInst &I)
{
   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   auto Self = args.front();
   auto PTablePtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                               Self, 1));

   auto GEP = Builder.CreateInBoundsGEP(PTablePtr,
                                        Builder.getInt64(I.getCalledMethod()
                                                          ->getPtableOffset()));
   auto funcPtr = Builder.CreateBitCast(GEP, getLlvmType(I.getCalledMethod()
                                                          ->getType()));

   auto ObjPtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                            Self, 0));
   auto Obj = Builder.CreateBitCast(ObjPtr, getLlvmType(I.getCalledMethod()
                                                         ->getReturnType()));

   args[0] = Obj;
   return Builder.CreateCall(funcPtr, args);
}

llvm::Value * IRGen::visitIndirectCallInst(IndirectCallInst const& I)
{
   auto Fun = getLlvmValue(I.getCalledFunction());
   llvm::SmallVector<llvm::Value*, 8> args;

   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   return Builder.CreateCall(Fun, args);
}

llvm::FunctionType* IRGen::getLambdaType(FunctionType *FTy)
{
   llvm::SmallVector<llvm::Type*, 8> ArgTypes{ Int8PtrTy->getPointerTo() };
   llvm::Type *retType;

   if (FTy->getReturnType()->needsStructReturn()) {
      retType = VoidTy;
      ArgTypes.push_back(getLlvmType(FTy->getReturnType())->getPointerTo());
   }
   else {
      retType = getLlvmType(FTy->getReturnType());
   }

   for (const auto &arg : FTy->getParamTypes()) {
      auto argTy = getLlvmType(arg);
      if (argTy->isStructTy())
         argTy = argTy->getPointerTo();

      ArgTypes.push_back(argTy);
   }

   return llvm::FunctionType::get(retType, ArgTypes, false);
}

llvm::Value * IRGen::visitLambdaCallInst(LambdaCallInst const& I)
{
   auto Lambda = getLlvmValue(I.getLambda());

   auto Env = Builder.CreateLoad(Builder.CreateStructGEP(LambdaTy,
                                                         Lambda, 1));

   llvm::Value *Fun = Builder.CreateLoad(Builder.CreateStructGEP(LambdaTy,
                                                                 Lambda, 0));

   FunctionType *funTy = I.getLambda()->getType()->asFunctionType();
   llvm::SmallVector<llvm::Value*, 8> args{ Env };

   bool sret = funTy->getReturnType()->needsStructReturn();
   if (sret) {
      auto alloca = CreateAlloca(funTy->getReturnType());
      args.push_back(alloca);
   }

   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   Fun = Builder.CreateBitCast(Fun, getLambdaType(funTy)->getPointerTo());
   llvm::Value *ret = Builder.CreateCall(Fun, args);

   if (sret)
      ret = args[1];

   return ret;
}

llvm::Value* IRGen::visitInitInst(InitInst const& I)
{
   llvm::Value *alloca;
   if (I.canUseSRetValue()) {
      alloca = getCurrentSRetValue();
   }
   else if (I.getType()->isClass()) {
      alloca = Builder.CreateCall(
         getMallocFn(), wordSizedInt(TI.getAllocSizeOfType(I.getType())));

      alloca = Builder.CreateBitCast(alloca, getStructTy(I.getInitializedType())
                                        ->getPointerTo());
   }
   else {
      alloca = CreateAlloca(I.getInitializedType()->getType());
   }

   llvm::SmallVector<llvm::Value*, 8> args { alloca };
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   Builder.CreateCall(getFunction(I.getInit()), args);

   return alloca;
}

llvm::Value* IRGen::visitUnionInitInst(UnionInitInst const& I)
{
   auto UnionTy = getStructTy(I.getUnionTy());
   auto alloca = CreateAlloca(I.getUnionTy()->getType());
   auto val = getLlvmValue(I.getInitializerVal());

   auto gep = Builder.CreateStructGEP(UnionTy, alloca, 0);
   gep = Builder.CreateBitCast(gep, val->getType()->getPointerTo());

   Builder.CreateStore(val, gep);

   return alloca;
}

llvm::Value* IRGen::visitEnumInitInst(EnumInitInst const& I)
{
   auto EnumTy = I.getEnumTy();
   auto &Case = EnumTy->getCase(I.getCaseName());

   if (EnumTy->getMaxAssociatedValues() == 0) {
      auto rawTy = getLlvmType(EnumTy->getRawType());
      auto alloca = CreateAlloca(EnumTy->getRawType());
      Builder.CreateStore(llvm::ConstantInt::get(rawTy,
                                                 Case.caseVal->getZExtValue()),
                          alloca);

      return alloca;
   }

   auto LlvmTy = getStructTy(EnumTy);
   auto alloca = CreateAlloca(EnumTy->getType());

   auto CaseVal = getLlvmValue(Case.caseVal);

   auto gep = Builder.CreateStructGEP(LlvmTy, alloca, 0);
   Builder.CreateStore(CaseVal, gep);

   unsigned i = 1;
   for (const auto &AV : I.getArgs()) {
      gep = Builder.CreateStructGEP(LlvmTy, alloca, i);

      auto val = getLlvmValue(AV);
      if (AV->getType()->isPointerType() || AV->getType()->isIntegerType()) {
         auto bc = Builder.CreateBitCast(gep, val->getType()->getPointerTo());
         Builder.CreateStore(val, bc);
      }
      else if (AV->getType()->needsStructReturn()) {
         auto TypeSize =  TI.getAllocSizeOfType(AV->getType());
         auto ptr = Builder.CreateCall(getMallocFn(), {
            llvm::ConstantInt::get(WordTy, TypeSize)
         });

         Builder.CreateMemCpy(ptr, val, TypeSize,
                              TI.getAllocAlignOfType(AV->getType()));

         Builder.CreateStore(toInt8Ptr(val), gep);
      }
      else {
         Builder.CreateStore(toInt8Ptr(val), gep);
      }

      ++i;
   }

   return alloca;
}

llvm::Value* IRGen::visitBinaryOperatorInst(const BinaryOperatorInst &I)
{
   using OPC = BinaryOperatorInst::OpCode;

   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));
   
   switch (I.getOpCode()) {
   case OPC::And:  return Builder.CreateAnd(lhs, rhs);
   case OPC::Or:   return Builder.CreateOr(lhs, rhs);
   case OPC::Xor:  return Builder.CreateXor(lhs, rhs);
   case OPC::LShr: return Builder.CreateLShr(lhs, rhs);
   case OPC::AShr: return Builder.CreateAShr(lhs, rhs);
   case OPC::Shl:  return Builder.CreateShl(lhs, rhs);
#  define INT_OR_FP_OP(Name)                                      \
   case OPC::Name:                                                \
      if (ty->isIntegerType()) {                                  \
         if (ty->isUnsigned())                                    \
            return Builder.CreateNUW##Name(lhs, rhs);             \
                                                                  \
         return Builder.CreateNSW##Name(lhs, rhs);                \
      }                                                           \
                                                                  \
      return Builder.CreateF##Name(lhs, rhs);
   INT_OR_FP_OP(Add)
   INT_OR_FP_OP(Sub)
   INT_OR_FP_OP(Mul)

#  undef INT_OR_FP_OP
   case OPC::Div:
      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateUDiv(lhs, rhs);

         return Builder.CreateSDiv(lhs, rhs);
      }

      return Builder.CreateFDiv(lhs, rhs);
   case OPC::Mod:
      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateURem(lhs, rhs);

         return Builder.CreateSRem(lhs, rhs);
      }

      return Builder.CreateFRem(lhs, rhs);
   case OPC::Exp: {
      llvm::Constant *powFn;
      if (lhs->getType()->isIntegerTy()) {
         powFn = getIntPowFn(I.getOperand(0)->getType());
      }
      else if (rhs->getType()->isIntegerTy()) {
         powFn = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::ID::powi,
                                                 { lhs->getType() });
      }
      else {
         powFn = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::ID::pow,
                                                 { lhs->getType() });
      }

      return Builder.CreateCall(powFn, { lhs, rhs });
   }
   }

   llvm_unreachable("bad opcode");
}

llvm::Value* IRGen::visitCompInst(const CompInst &I)
{
   using OPC = CompInst::OpCode;

   auto ty = I.getLhs()->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   switch (I.getOpCode()) {
   case OPC::CompEQ:
      if (ty->isIntegerType() || ty->isPointerType()) {
         return Builder.CreateICmpEQ(lhs, rhs);
      }

      return Builder.CreateFCmpOEQ(lhs, rhs);
   case OPC::CompNE:
      if (ty->isIntegerType() || ty->isPointerType()) {
         return Builder.CreateICmpNE(lhs, rhs);
      }

      return Builder.CreateFCmpONE(lhs, rhs);
   case OPC::CompGE:
      if (ty->isPointerType())
         return Builder.CreateICmpUGE(lhs, rhs);

      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateICmpUGE(lhs, rhs);

         return Builder.CreateICmpSGE(lhs, rhs);
      }

      return Builder.CreateFCmpOGE(lhs, rhs);
   case OPC::CompLE:
      if (ty->isPointerType())
         return Builder.CreateICmpULE(lhs, rhs);

      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateICmpULE(lhs, rhs);

         return Builder.CreateICmpSLE(lhs, rhs);
      }

      return Builder.CreateFCmpOLE(lhs, rhs);
   case OPC::CompGT:
      if (ty->isPointerType())
         return Builder.CreateICmpUGT(lhs, rhs);

      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateICmpUGT(lhs, rhs);

         return Builder.CreateICmpSGT(lhs, rhs);
      }

      return Builder.CreateFCmpOGT(lhs, rhs);
   case OPC::CompLT:
      if (ty->isPointerType())
         return Builder.CreateICmpULT(lhs, rhs);

      if (ty->isIntegerType()) {
         if (ty->isUnsigned())
            return Builder.CreateICmpULT(lhs, rhs);

         return Builder.CreateICmpSLT(lhs, rhs);
      }

      return Builder.CreateFCmpOLT(lhs, rhs);
   }
}

llvm::Value* IRGen::visitUnaryOperatorInst(const UnaryOperatorInst &I)
{
   auto target = getLlvmValue(I.getOperand(0));

   switch (I.getOpCode()) {
   case UnaryOperatorInst::Neg: {
      auto rhs = llvm::ConstantInt::getAllOnesValue(target->getType());
      return Builder.CreateXor(target, rhs);
   }
   case UnaryOperatorInst::Min: {
      return Builder.CreateNSWSub(
         Builder.getIntN(target->getType()->getIntegerBitWidth(), 0),
         target);
   }
   }

   llvm_unreachable("bad unary inst");
}

llvm::Value* IRGen::visitBitCastInst(BitCastInst const& I)
{
   return Builder.CreateBitCast(getLlvmValue(I.getOperand(0)),
                                getLlvmType(I.getType()));
}

llvm::Value * IRGen::visitIntegerCastInst(IntegerCastInst const& I)
{
   auto val = getLlvmValue(I.getOperand(0));
   auto fromTy = I.getOperand(0)->getType();
   auto toTy = getLlvmType(I.getType());

   switch (I.getKind()) {
      case CastKind::IntToFP:
         if (fromTy->isUnsigned())
            return Builder.CreateUIToFP(val, toTy);

         return Builder.CreateSIToFP(val, toTy);
      case CastKind::FPToInt:
         if (I.getType()->isUnsigned())
            return Builder.CreateFPToUI(val, toTy);

         return Builder.CreateFPToSI(val, toTy);
      case CastKind::IntToPtr:
         return Builder.CreateIntToPtr(val, toTy);
      case CastKind::PtrToInt:
         return Builder.CreatePtrToInt(val, toTy);
      case CastKind::Ext:
      case CastKind::Trunc:
         if (fromTy->isUnsigned())
            return Builder.CreateZExtOrTrunc(val, toTy);

         return Builder.CreateSExtOrTrunc(val, toTy);
      case CastKind::SignFlip:
         return val;
      default:
         llvm_unreachable("not an integer cast!");
   }
}

llvm::Value * IRGen::visitFPCastInst(FPCastInst const& I)
{
   auto val = getLlvmValue(I.getOperand(0));
   auto toTy = getLlvmType(I.getType());

   switch (I.getKind()) {
      case CastKind::FPTrunc:
         return Builder.CreateFPTrunc(val, toTy);
      case CastKind::FPExt:
         return Builder.CreateFPExt(val, toTy);
      default:
         llvm_unreachable("not a fp cast");
   }
}

llvm::Value* IRGen::visitIntToEnumInst(IntToEnumInst const& I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitUnionCastInst(UnionCastInst const& I)
{
   auto U = getLlvmValue(I.getOperand(0));
   auto toTy = getLlvmType(I.getType());

   return Builder.CreateBitCast(U, toTy);
}

llvm::Value* IRGen::visitProtoCastInst(ProtoCastInst const& I)
{
   auto val = getLlvmValue(I.getOperand(0));

   if (I.isWrap()) {
      auto alloca = CreateAlloca(ProtocolTy, 1, TI.getPointerAlignInBytes());
      auto gep = Builder.CreateStructGEP(ProtocolTy, alloca, 0);
      Builder.CreateStore(toInt8Ptr(val), gep);

      return alloca;
   }
   else {
      auto gep = Builder.CreateStructGEP(ProtocolTy, val, 0);
      return Builder.CreateLoad(gep);
   }
}

llvm::Value* IRGen::visitExceptionCastInst(ExceptionCastInst const& I)
{
   auto Exc = getLlvmValue(I.getOperand(0));
   auto toTy = getLlvmType(I.getType());

   return Builder.CreateBitCast(Exc, toTy);
}

llvm::Value* IRGen::visitDynamicCastInst(const DynamicCastInst &I)
{
   llvm_unreachable("not yet");
}

} // namespace il
} // namespace cdot