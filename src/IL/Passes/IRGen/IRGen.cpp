//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "../../../Compiler.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Files/FileUtils.h"
#include "../../../Basic/CastKind.h"
#include "../../../Files/FileManager.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/DIBuilder.h>

using namespace cdot::support;
using namespace cdot::fs;

namespace cdot {
namespace il {

IRGen::IRGen(llvm::LLVMContext &Ctx,
             llvm::Module *M,
             bool emitDebugInfo)
   : InstructionVisitor(IRGenID), emitDebugInfo(emitDebugInfo),
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
      DI = new llvm::DIBuilder(*this->M);
      File = getFileDI(CU.getSourceLoc());
      this->CU =  DI->createCompileUnit(
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
                                  Int8PtrTy, nullptr);

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
                                         Int8PtrTy),
                                      nullptr))
         );
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
      MallocFn = M->getOrInsertFunction("__cdot_malloc",
                                        Int8PtrTy, WordTy,
                                        nullptr);

   return MallocFn;
}

llvm::Constant* IRGen::getFreeFn()
{
   if (!FreeFn)
      FreeFn = M->getOrInsertFunction("__cdot_free",
                                      VoidTy, Int8PtrTy,
                                      nullptr);

   return FreeFn;
}

llvm::Constant* IRGen::getThrowFn()
{
   if (!ThrowFn)
      ThrowFn = M->getOrInsertFunction("__cdot_throw",
                                       VoidTy, Int8PtrTy,
                                       nullptr);

   return ThrowFn;
}

llvm::Constant* IRGen::getAllocExcnFn()
{
   if (!AllocExcFn)
      AllocExcFn = M->getOrInsertFunction("__cdot_allocate_exception",
                                          Int8PtrTy, WordTy, Int8PtrTy,
                                          Int8PtrTy, Int8PtrTy, nullptr);

   return AllocExcFn;
}

llvm::Constant* IRGen::getRetainFn()
{
   if (!RetainFn)
      RetainFn = M->getOrInsertFunction("__cdot_retain", VoidTy, Int8PtrTy,
                                        nullptr);

   return RetainFn;
}

llvm::Constant* IRGen::getReleaseFn()
{
   if (!ReleaseFn)
      ReleaseFn = M->getOrInsertFunction("__cdot_release", VoidTy, Int8PtrTy,
                                         nullptr);

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

llvm::Constant* IRGen::getIntPowFn()
{
   if (!IntPowFn)
      IntPowFn = M->getOrInsertFunction("__cdot_intpow",
                                        llvm::FunctionType::get(
                                           WordTy, { WordTy, WordTy }, false));

   return IntPowFn;
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
      case TypeID::VoidTypeID:
         return VoidTy;
      case TypeID::IntegerTypeID:
         return Builder.getIntNTy(Ty->getBitwidth());
      case TypeID::FPTypeID:
         return Ty->isFloatTy() ? Builder.getFloatTy() : Builder.getDoubleTy();
      case TypeID::PointerTypeID:
         return getLlvmType(Ty->getPointeeType())->getPointerTo();
      case TypeID::ArrayTypeID: {
         ArrayType *ArrTy = Ty->asArrayType();
         return llvm::ArrayType::get(getLlvmType(ArrTy->getElementType()),
                                     ArrTy->getNumElements());
      }
      case TypeID::FunctionTypeID: {
         if (!Ty->isRawFunctionTy())
            return LambdaTy;

         FunctionType *FuncTy = Ty->asFunctionType();
         auto ret = getLlvmType(*FuncTy->getReturnType());
         llvm::SmallVector<llvm::Type*, 4> argTypes;

         for (const auto &arg : FuncTy->getArgTypes())
            argTypes.push_back(getLlvmType(arg));

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
      case TypeID::ObjectTypeID: {
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
   auto llvmTy = getLlvmType(*Ty);
   if (Ty.isLvalue() && !Ty->needsStructReturn())
      llvmTy = llvmTy->getPointerTo();

   return llvmTy;
}

llvm::Type* IRGen::getFieldType(QualType Ty)
{
   auto llvmTy = getLlvmType(*Ty);
   if (Ty.isLvalue())
      llvmTy = llvmTy->getPointerTo();

   return llvmTy;
}

void IRGen::DeclareFunction(il::Function const* F)
{
   if (auto M = dyn_cast<il::Method>(F))
      if (M->isProtocolMethod())
         return;

   auto funcTy = F->getType()->asFunctionType();

   llvm::SmallVector<llvm::Type*, 8> argTypes;
   llvm::Type *retType;

   if (F->isLambda())
      argTypes.push_back(Int8PtrTy->getPointerTo());

   if (F->hasStructReturn()) {
      auto sretTy = getLlvmType(funcTy->getReturnType());
      if (sretTy->isStructTy() || sretTy->isArrayTy())
         sretTy = sretTy->getPointerTo();

      argTypes.push_back(sretTy);
      retType = VoidTy;
   }
   else {
      retType = getLlvmType(funcTy->getReturnType());
   }

   for (const auto &arg : funcTy->getArgTypes()) {
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
   if (DI)
      emitFunctionDI(*F, fn);
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

   Ty->getLlvmTy()->setBody(ContainedTypes);
}

void IRGen::ForwardDeclareGlobal(il::GlobalVariable const *G)
{
   auto globalTy = getLlvmType(*G->getType());
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
            for (const auto &Pred : B.getPredecessors()) {
               auto Term = Pred->getTerminator();
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
                                getBasicBlock(Pred->getName()));
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
   Builder.SetInsertPoint(getBasicBlock(&B));

   if (!B.isEntryBlock() && !B.getArgs().empty()) {
      for (const auto &arg : B.getArgs()) {
         auto ty = getLlvmType(arg.getType());
         if (ty->isStructTy())
            ty = ty->getPointerTo();

         Builder.CreatePHI(ty,
                           B.getPredecessors().size());
      }
   }

   for (auto const& I : B.getInstructions()) {
      if (DI) {
         auto L = I.getLocation();
         assert(L && "no debug location for instruction!");

         Builder.SetCurrentDebugLocation(llvm::DebugLoc::get(
            (unsigned int)L->getLine(),
            (unsigned int)L->getCol(),
            ScopeStack.top()
         ));
      }

      auto val = visit(I);
      auto res = ValueMap.try_emplace(I.getName(), val);
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

llvm::Value* IRGen::CreateAlloca(llvm::Type *AllocatedType,
                                 size_t allocatedSize) {
   auto EB = &Builder.GetInsertBlock()->getParent()->getEntryBlock();
   auto IP = Builder.saveIP();

   Builder.SetInsertPoint(EB);

   llvm::Value *alloca;
   if (allocatedSize > 1)
      alloca = Builder.CreateAlloca(AllocatedType, wordSizedInt(allocatedSize));
   else
      alloca = Builder.CreateAlloca(AllocatedType);

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

   auto it = ValueMap.find(V->getName());
   assert(it != ValueMap.end());

   return it->second;
}

llvm::Constant* IRGen::getConstantVal(il::Constant const* C)
{
   if (auto Int = dyn_cast<ConstantInt>(C)) {
      if (Int->isCTFE())
         return Builder.getFalse();

      return llvm::ConstantInt::get(getLlvmType(Int->getType()),
                                    Int->getU64());
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      return llvm::ConstantFP::get(getLlvmType(F->getType()),
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
   auto allocatedType = getLlvmType(*I.getType());
   if (I.getType()->needsStructReturn() && allocatedType->isPointerTy())
      allocatedType = allocatedType->getPointerElementType();

   llvm::Value *V;
   if (I.isHeapAlloca()) {
      auto size = wordSizedInt(I.getType()->getSize() * I.getAllocSize());
      auto buff = Builder.CreateCall(getMallocFn(), { size });

      V = Builder.CreateBitCast(buff, allocatedType->getPointerTo());
   }
   else {
      V = CreateAlloca(allocatedType, I.getAllocSize());
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

   auto alloca = CreateAlloca(LambdaTy);

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

   if (Ty->needsStructReturn() && Dst->getType() == Src->getType())
      return Builder.CreateMemCpy(Dst, Src, Ty->getSize(), Ty->getAlignment());

   return Builder.CreateStore(Src, Dst);
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
   auto env = &F->getArgumentList().front();

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

   if (caseTy->isPrimitiveType() || caseTy->isPointerType()
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

   return Builder.CreateLoad(val);
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
         Builder.CreateMemCpy(sretVal, retVal, V->getType()->getSize(),
                              V->getType()->getAlignment());
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
      wordSizedInt(I.getThrownValue()->getType()->getSize()),
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
      case Intrinsic::MemCpy:
      case Intrinsic::MemSet:
      case Intrinsic::MemCmp: {
         llvm::Value *Args[] = {
            getLlvmValue(I.getOperand(0)),
            getLlvmValue(I.getOperand(1)),
            getLlvmValue(I.getOperand(2))
         };

         if (I.getCalledIntrinsic() == Intrinsic::MemCpy) {
            return Builder.CreateMemCpy(Args[0], Args[1], Args[2], 1);
         }
         else if (I.getCalledIntrinsic() == Intrinsic::MemCmp) {
            auto cmp = Builder.CreateCall(getMemCmpFn(), {
               toInt8Ptr(Args[0]), toInt8Ptr(Args[1]), Args[2]
            });
            return Builder.CreateIsNull(cmp);
         }
         else {
            return Builder.CreateMemSet(Args[0], Args[1], Args[2], 1);
         }
      }
      case Intrinsic::Release:
         return Builder.CreateCall(getReleaseFn(), {
            toInt8Ptr(getLlvmValue(I.getOperand(0)))
         });
      case Intrinsic::Retain:
         return Builder.CreateCall(getRetainFn(), {
            toInt8Ptr(getLlvmValue(I.getOperand(0)))
         });
      case Intrinsic::LifetimeBegin:
         return Builder.CreateLifetimeStart(getLlvmValue(I.getArgs()[0]),
                         cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));
      case Intrinsic::LifetimeEnd: {
         auto Self = getLlvmValue(I.getArgs()[0]);
         Builder.CreateLifetimeEnd(Self,
                         cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));

         if (I.getArgs().front()->getType()->isClass())
            Builder.CreateCall(getFreeFn(), toInt8Ptr(Self));

         return nullptr;
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

   for (const auto &arg : FTy->getArgTypes()) {
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
      auto alloca = CreateAlloca(getLlvmType(*funTy->getReturnType()));
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

llvm::Value * IRGen::visitInitInst(InitInst const& I)
{
   llvm::Value *alloca;
   if (I.getType()->isClass()) {
      alloca = Builder.CreateCall(getMallocFn(),
                                  wordSizedInt(I.getType()->getSize()));

      alloca = Builder.CreateBitCast(alloca, getStructTy(I.getInitializedType())
                                        ->getPointerTo());
   }
   else {
      alloca = CreateAlloca(getStructTy(I.getInitializedType()));
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
   auto alloca = CreateAlloca(UnionTy);
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
      auto alloca = Builder.CreateAlloca(rawTy);
      Builder.CreateStore(llvm::ConstantInt::get(rawTy,
                                                 Case.caseVal->getU64()),
                          alloca);

      return alloca;
   }

   auto LlvmTy = getStructTy(EnumTy);
   auto alloca = CreateAlloca(LlvmTy);

   auto CaseVal = getLlvmValue(Case.caseVal);

   auto gep = Builder.CreateStructGEP(LlvmTy, alloca, 0);
   Builder.CreateStore(CaseVal, gep);

   unsigned i = 1;
   for (const auto &AV : I.getArgs()) {
      gep = Builder.CreateStructGEP(LlvmTy, alloca, i);

      auto val = getLlvmValue(AV);
      if (AV->getType()->isPointerType() || AV->getType()->isPrimitiveType()
                                            || AV->getType()->isRawEnum()) {
         auto bc = Builder.CreateBitCast(gep, val->getType()->getPointerTo());
         Builder.CreateStore(val, bc);
      }
      else if (AV->getType()->needsStructReturn()) {
         auto ptr = Builder.CreateCall(getMallocFn(), {
            llvm::ConstantInt::get(WordTy, AV->getType()->getSize())
         });

         Builder.CreateMemCpy(ptr, val, AV->getType()->getSize(),
                              AV->getType()->getAlignment());

         Builder.CreateStore(toInt8Ptr(val), gep);
      }
      else {
         Builder.CreateStore(toInt8Ptr(val), gep);
      }

      ++i;
   }

   return alloca;
}

#define CDOT_BINARY_INST(Name)                       \
llvm::Value* IRGen::visit##Name##Inst(Name##Inst const& I) \
{                                                    \
   auto lhs = getLlvmValue(I.getOperand(0));         \
   auto rhs = getLlvmValue(I.getOperand(1));         \
   return Builder.Create##Name(lhs, rhs);            \
}

CDOT_BINARY_INST(And)
CDOT_BINARY_INST(Or)
CDOT_BINARY_INST(Xor)
CDOT_BINARY_INST(LShr)
CDOT_BINARY_INST(AShr)
CDOT_BINARY_INST(Shl)

llvm::Value* IRGen::visitAddInst(AddInst const& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateNUWAdd(lhs, rhs);

      return Builder.CreateNSWAdd(lhs, rhs);
   }

   return Builder.CreateFAdd(lhs, rhs);
}

llvm::Value* IRGen::visitSubInst(SubInst const& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateNUWSub(lhs, rhs);

      return Builder.CreateNSWSub(lhs, rhs);
   }

   return Builder.CreateFSub(lhs, rhs);
}

llvm::Value* IRGen::visitMulInst(MulInst const& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateNUWMul(lhs, rhs);

      return Builder.CreateNSWMul(lhs, rhs);
   }

   return Builder.CreateFMul(lhs, rhs);
}

llvm::Value* IRGen::visitDivInst(DivInst const& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateUDiv(lhs, rhs);

      return Builder.CreateSDiv(lhs, rhs);
   }

   return Builder.CreateFDiv(lhs, rhs);
}

llvm::Value* IRGen::visitModInst(ModInst const& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateURem(lhs, rhs);

      return Builder.CreateSRem(lhs, rhs);
   }

   return Builder.CreateFRem(lhs, rhs);
}

llvm::Value* IRGen::visitExpInst(ExpInst const& I)
{
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   llvm::Constant *powFn;
   if (lhs->getType()->isIntegerTy()) {
      powFn = getIntPowFn();
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

llvm::Value * IRGen::visitCompEQInst(CompEQInst const& I)
{
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));
   auto ty = lhs->getType();

   if (ty->isIntegerTy() || ty->isPointerTy()) {
      return Builder.CreateICmpEQ(lhs, rhs);
   }

   return Builder.CreateFCmpOEQ(lhs, rhs);
}

llvm::Value * IRGen::visitCompNEInst(CompNEInst const& I)
{
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));
   auto ty = lhs->getType();

   if (ty->isIntegerTy() || ty->isPointerTy()) {
      return Builder.CreateICmpNE(lhs, rhs);
   }

   return Builder.CreateFCmpONE(lhs, rhs);
}

llvm::Value * IRGen::visitCompGEInst(CompGEInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isPointerType())
      return Builder.CreateICmpUGE(lhs, rhs);

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateICmpUGE(lhs, rhs);

      return Builder.CreateICmpSGE(lhs, rhs);
   }

   return Builder.CreateFCmpOGE(lhs, rhs);
}

llvm::Value * IRGen::visitCompLEInst(CompLEInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isPointerType())
      return Builder.CreateICmpULE(lhs, rhs);

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateICmpULE(lhs, rhs);

      return Builder.CreateICmpSLE(lhs, rhs);
   }

   return Builder.CreateFCmpOLE(lhs, rhs);
}

llvm::Value * IRGen::visitCompGTInst(CompGTInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isPointerType())
      return Builder.CreateICmpUGT(lhs, rhs);

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateICmpUGT(lhs, rhs);

      return Builder.CreateICmpSGT(lhs, rhs);
   }

   return Builder.CreateFCmpOGT(lhs, rhs);
}

llvm::Value * IRGen::visitCompLTInst(CompLTInst const& I)
{
   auto ty = I.getOperand(0)->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   if (ty->isPointerType())
      return Builder.CreateICmpULT(lhs, rhs);

   if (ty->isIntegerType()) {
      if (ty->isUnsigned())
         return Builder.CreateICmpULT(lhs, rhs);

      return Builder.CreateICmpSLT(lhs, rhs);
   }

   return Builder.CreateFCmpOLT(lhs, rhs);
}

llvm::Value* IRGen::visitNegInst(NegInst const& I)
{
   return Builder.CreateXor(getLlvmValue(I.getOperand(0)), uint64_t(-1));
}

llvm::Value* IRGen::visitMinInst(MinInst const& I)
{
   auto rhs = getLlvmValue(I.getOperand(0));
   return Builder.CreateNSWSub(Builder.getIntN(rhs->getType()
                                                  ->getIntegerBitWidth(), 0),
                               rhs);
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
      case CastKind::IBox: {
         auto alloca = CreateAlloca(toTy);
         auto gep = Builder.CreateStructGEP(toTy, alloca, 0);
         Builder.CreateStore(val, gep);

         return alloca;
      }
      case CastKind::IUnbox: {
         auto from = getLlvmType(fromTy);
         auto gep = Builder.CreateStructGEP(from, val, 0);
         return I.isLvalue() ? gep : (llvm::Value*)Builder.CreateLoad(gep);
      }
      case CastKind::SignFlip:
         return val;
      default:
         llvm_unreachable("not an integer cast!");
   }
}

llvm::Value * IRGen::visitFPCastInst(FPCastInst const& I)
{
   auto val = getLlvmValue(I.getOperand(0));
   auto fromTy = I.getOperand(0)->getType();
   auto toTy = getLlvmType(I.getType());

   switch (I.getKind()) {
      case CastKind::FPTrunc:
         return Builder.CreateFPTrunc(val, toTy);
      case CastKind::FPExt:
         return Builder.CreateFPExt(val, toTy);
      case CastKind::FPBox: {
         auto alloca = CreateAlloca(toTy);
         auto gep = Builder.CreateStructGEP(toTy, alloca, 0);
         Builder.CreateStore(val, gep);

         return alloca;
      }
      case CastKind::FPUnbox: {
         auto from = getLlvmType(fromTy);
         auto gep = Builder.CreateStructGEP(from, val, 0);

         return I.isLvalue() ? gep : (llvm::Value*)Builder.CreateLoad(gep);
      }
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
      auto alloca = CreateAlloca(ProtocolTy);
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

llvm::Value * IRGen::visitDynamicCastInst(const DynamicCastInst &I)
{
   llvm_unreachable("not yet");
}

} // namespace il
} // namespace cdot