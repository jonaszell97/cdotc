//
// Created by Jonas Zell on 10.12.17.
//

#include "IRGen.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Basic/CastKind.h"
#include "Compiler.h"
#include "Basic/FileUtils.h"
#include "Basic/FileManager.h"
#include "IL/Constants.h"
#include "IL/Context.h"
#include "IL/Utils/BlockIterator.h"
#include "AST/Type.h"

#include <llvm/IR/Module.h>
#include <llvm/IR/DIBuilder.h>
#include <IL/Instructions.h>

using namespace cdot::support;
using namespace cdot::ast;
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
   EmptyArrayPtrTy = llvm::ArrayType::get(Int8PtrTy, 0)->getPointerTo();
   Int1Ty = llvm::IntegerType::get(Ctx, 1);
   VoidTy = llvm::Type::getVoidTy(Ctx);

   TypeInfoTy = llvm::StructType::create(Ctx, "cdot.TypeInfo");
   DeinitializerTy = llvm::FunctionType::get(VoidTy, { Int8PtrTy }, false);

   llvm::Type *typeInfoContainedTypes[] = {
      Int8PtrTy,        // parent class
      EmptyArrayPtrTy,  // vtable
      EmptyArrayPtrTy,  // ptable
      Int8PtrTy,        // type name
      Int8PtrTy,        // deinitializer
      EmptyArrayPtrTy,  // conformances
   };

   TypeInfoTy->setBody(typeInfoContainedTypes);
   assert(TypeInfoTy->getNumContainedTypes() == MetaType::MemberCount);

   llvm::Type *protocolContainedTypes[] = {
      Int8PtrTy,                 // object ptr
      Int8PtrTy,                 // ptable ptr
      TypeInfoTy->getPointerTo() // type info
   };

   ProtocolTy = llvm::StructType::create(Ctx, protocolContainedTypes,
                                         "cdot.Protocol");

   llvm::Type *boxContainedTypes[] = {
      WordTy,                   // strong refcount
      WordTy,                   // weak refcount
      Int8PtrTy,                // deinitializer
      Int8PtrTy                 // data
   };

   BoxTy = llvm::StructType::create(Ctx, boxContainedTypes, "cdot.Box");

   llvm::Type *lambdaContainedTypes[] = {
      Int8PtrTy,                // function ptr
      WordTy,                   // strong refcount
      BoxTy->getPointerTo()     // first environment value
                                // (trailing objects follow)
   };

   LambdaTy = llvm::StructType::create(Ctx, lambdaContainedTypes,
                                       "cdot.Lambda");

   WordZero = Builder.getInt64(0);
   WordOne = Builder.getInt64(1);
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
   RetainLambdaFn = nullptr;
   ReleaseLambdaFn = nullptr;
   RetainBoxFn = nullptr;
   ReleaseBoxFn = nullptr;
   PrintfFn = nullptr;
   MemCmpFn = nullptr;
   IntPowFn = nullptr;

   for (auto Ty : ILMod.getRecords()) {
      // forward declare all types, they might be referenced when defining
      // others
      ForwardDeclareType(Ty);
   }

   for (auto Ty : ILMod.getRecords())
      DeclareType(Ty);

   for (const auto &F : ILMod.getFuncList()) {
      DeclareFunction(&F);
   }

   for (const auto &G : ILMod.getGlobalList())
      ForwardDeclareGlobal(&G);

   for (const auto &G : ILMod.getGlobalList())
      DeclareGlobal(&G);

   for (auto &F : ILMod.getFuncList())
      visitFunction(F);

   if (!GlobalInitFns.empty()) {
      auto FnTy = llvm::FunctionType::get(Builder.getVoidTy(), false)
         ->getPointerTo();

      auto ctorStructTy =
         llvm::StructType::get(Builder.getInt32Ty(), FnTy,
                               Int8PtrTy);

      llvm::SmallVector<llvm::Constant*, 4> Vals;
      for (auto &Fn : GlobalInitFns) {
         Vals.push_back(
            llvm::ConstantStruct::get(
               ctorStructTy, Builder.getInt32(65535), Fn,
               llvm::ConstantPointerNull::get(Int8PtrTy)));
      }

      auto ctorArrayTy = llvm::ArrayType::get(ctorStructTy,
                                              GlobalInitFns.size());

      auto ctors =
         new llvm::GlobalVariable(*M, ctorArrayTy, true,
                                  llvm::GlobalVariable::AppendingLinkage,
                                  nullptr, "llvm.global_ctors");

      ctors->setInitializer(llvm::ConstantArray::get(ctorArrayTy, Vals));
   }

   if (!GlobalDeinitFns.empty()) {
      auto FnTy = llvm::FunctionType::get(Builder.getVoidTy(), false)
         ->getPointerTo();

      auto ctorStructTy =
         llvm::StructType::get(Builder.getInt32Ty(), FnTy,
                               Int8PtrTy);

      llvm::SmallVector<llvm::Constant*, 4> Vals;
      for (auto &Fn : GlobalDeinitFns) {
         Vals.push_back(
            llvm::ConstantStruct::get(
               ctorStructTy, Builder.getInt32(65535), Fn,
               llvm::ConstantPointerNull::get(Int8PtrTy)));
      }

      auto ctorArrayTy = llvm::ArrayType::get(ctorStructTy,
                                              GlobalDeinitFns.size());

      auto ctors =
         new llvm::GlobalVariable(*M, ctorArrayTy, true,
                                  llvm::GlobalVariable::AppendingLinkage,
                                  nullptr, "llvm.global_dtors");

      ctors->setInitializer(llvm::ConstantArray::get(ctorArrayTy, Vals));
   }
}

llvm::Constant* IRGen::getMallocFn()
{
   if (!MallocFn)
      MallocFn = M->getOrInsertFunction("_cdot_Malloc",
                                        Int8PtrTy, WordTy);

   return MallocFn;
}

llvm::Constant* IRGen::getFreeFn()
{
   if (!FreeFn)
      FreeFn = M->getOrInsertFunction("_cdot_Free",
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
      RetainFn = M->getOrInsertFunction("_cdot_Retain", VoidTy, Int8PtrTy);

   return RetainFn;
}

llvm::Constant* IRGen::getReleaseFn()
{
   if (!ReleaseFn)
      ReleaseFn = M->getOrInsertFunction("_cdot_Release", VoidTy, Int8PtrTy);

   return ReleaseFn;
}

llvm::Constant* IRGen::getRetainLambdaFn()
{
   if (!RetainLambdaFn)
      RetainLambdaFn = M->getOrInsertFunction("_cdot_RetainLambda",
                                              VoidTy, Int8PtrTy);

   return RetainLambdaFn;
}

llvm::Constant* IRGen::getReleaseLambdaFn()
{
   if (!ReleaseLambdaFn)
      ReleaseLambdaFn = M->getOrInsertFunction("_cdot_ReleaseLambda",
                                               VoidTy, Int8PtrTy);

   return ReleaseLambdaFn;
}

llvm::Constant* IRGen::getRetainBoxFn()
{
   if (!RetainBoxFn)
      RetainBoxFn = M->getOrInsertFunction("_cdot_RetainBox",
                                           VoidTy, Int8PtrTy);

   return RetainBoxFn;
}

llvm::Constant* IRGen::getReleaseBoxFn()
{
   if (!ReleaseBoxFn)
      ReleaseBoxFn = M->getOrInsertFunction("_cdot_ReleaseBox",
                                            VoidTy, Int8PtrTy);

   return ReleaseBoxFn;
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
   auto llvmTy = getStorageType(IntTy);
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

llvm::StructType* IRGen::getEnumCaseTy(ast::EnumCaseDecl *Decl)
{
   auto it = EnumCaseTys.find(Decl);
   if (it != EnumCaseTys.end())
      return it->getSecond();

   unsigned Size = 0;
   llvm::SmallVector<llvm::Type*, 4> ContainedTys;

   for (auto &Val : Decl->getArgs()) {
      Size += TI.getSizeOfType(Val->getType());
      ContainedTys.push_back(getStorageType(Val->getType()));
   }

   // add padding to get to the same size as the Enum type
   auto NeededCaseValSize = Decl->getRecord()->getSize()
      - TI.getSizeOfType(cast<EnumDecl>(Decl->getRecord())->getRawType());

   if (Size < NeededCaseValSize) {
      ContainedTys.push_back(llvm::ArrayType::get(Builder.getInt8Ty(),
                                                  NeededCaseValSize - Size));
   }

   auto llvmTy = llvm::StructType::get(Ctx, ContainedTys);
   EnumCaseTys[Decl] = llvmTy;

   return llvmTy;
}

llvm::StructType* IRGen::getStructTy(QualType Ty)
{
   return getStructTy(Ty->getRecord());
}

llvm::StructType* IRGen::getStructTy(RecordDecl *R)
{
   if (isa<ProtocolDecl>(R))
      return ProtocolTy;

   return StructTypeMap[R];
}

llvm::Type* IRGen::getLlvmTypeImpl(QualType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
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
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID: {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         return Int8PtrTy->getPointerTo();

      return getStorageType(pointee)->getPointerTo();
   }
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID: {
      QualType referenced = Ty->asReferenceType()->getReferencedType();
      if (referenced->needsStructReturn())
         return getStorageType(referenced);

      return getStorageType(referenced)->getPointerTo();
   }
   case Type::ArrayTypeID: {
      ArrayType *ArrTy = Ty->asArrayType();
      return llvm::ArrayType::get(getStorageType(ArrTy->getElementType()),
                                  ArrTy->getNumElements());
   }
   case Type::LambdaTypeID:
      return LambdaTy->getPointerTo();
   case Type::FunctionTypeID: {
      FunctionType *FuncTy = Ty->asFunctionType();
      auto ret = getStorageType(*FuncTy->getReturnType());
      llvm::SmallVector<llvm::Type*, 4> argTypes;

      for (const auto &arg : FuncTy->getParamTypes()) {
         auto ty = getStorageType(arg);
         if (ty->isStructTy())
            ty = ty->getPointerTo();

         argTypes.push_back(ty);
      }

      return llvm::FunctionType::get(ret, argTypes,
                                     FuncTy->isCStyleVararg())
         ->getPointerTo();
   }
   case Type::TupleTypeID: {
      TupleType *TupleTy = Ty->asTupleType();
      llvm::SmallVector<llvm::Type*, 4> argTypes;

      for (const auto &cont : TupleTy->getContainedTypes())
         argTypes.push_back(getStorageType(cont));

      return llvm::StructType::get(Ctx, argTypes);
   }
   case Type::RecordTypeID: {
      if (Ty->isRefcounted())
         return getStructTy(Ty)->getPointerTo();

      if (Ty->isRawEnum())
         return getStorageType(cast<EnumDecl>(Ty->getRecord())->getRawType());

      return getStructTy(Ty);
   }
   case Type::MetaTypeID:
      return TypeInfoTy;
   default:
      llvm_unreachable("type should note be possible here");
   }
}

llvm::Type* IRGen::getStorageType(QualType Ty)
{
   auto it = TypeMap.find(Ty);
   if (it != TypeMap.end())
      return it->getSecond();

   auto llvmTy = getLlvmTypeImpl(Ty);
   TypeMap[Ty] = llvmTy;

   return llvmTy;
}

llvm::Type* IRGen::getParameterType(QualType Ty)
{
   auto llvmTy = getStorageType(Ty);
   if (llvmTy->isArrayTy() || llvmTy->isStructTy())
      llvmTy = llvmTy->getPointerTo();

   return llvmTy;
}

llvm::Type* IRGen::getGlobalType(QualType Ty)
{
//   if (Ty->isClass())
//      return getStructTy(Ty);

   return getStorageType(Ty);
}

void IRGen::DeclareFunction(il::Function const* F)
{
   auto funcTy = F->getType()->asFunctionType();

   llvm::SmallVector<llvm::Type*, 8> argTypes;
   llvm::Type *retType;

   bool sret = false;
   unsigned sretIdx = 0;

   if (F->isLambda()) {
      auto L = cast<Lambda>(F);

      llvm::SmallVector<llvm::Type*, 4> CapturedTypes;
      for (auto &Capt : L->getCaptures()) {
         (void)Capt;
         CapturedTypes.push_back(BoxTy->getPointerTo());
      }

      llvm::StructType *Ty = llvm::StructType::get(Ctx, CapturedTypes);
      argTypes.push_back(Ty->getPointerTo());

      ++sretIdx;
   }

   if (F->hasStructReturn()) {
      auto sretTy = getParameterType(funcTy->getReturnType());
      sret = true;

      argTypes.push_back(sretTy);
      retType = VoidTy;
   }
   else {
      retType = getStorageType(funcTy->getReturnType());
   }

   for (const auto &arg : funcTy->getParamTypes()) {
      argTypes.push_back(getParameterType(arg));
   }

   auto linkage =
      F->isGlobalInitFn()
         ? llvm::Function::InternalLinkage
         : (llvm::Function::LinkageTypes)F->getLinkage();

   auto llvmTy = llvm::FunctionType::get(retType, argTypes,
                                         funcTy->isCStyleVararg());

   auto fn = llvm::Function::Create(llvmTy, linkage, F->getName(), M);
   fn->setUnnamedAddr((llvm::Function::UnnamedAddr)F->getUnnamedAddr());
   fn->setVisibility((llvm::Function::VisibilityTypes)F->getVisibility());

   if (sret) {
      fn->addAttribute(sretIdx + 1, llvm::Attribute::StructRet);
   }

   if (F->isGlobalInitFn() || F->isGlobalCtor()) {
      GlobalInitFns.push_back(fn);
   }

   if (F->isGlobalDtor()) {
      GlobalDeinitFns.push_back(fn);
   }

   if (DI)
      emitFunctionDI(*F, fn);

   auto il_arg_it = F->getEntryBlock()->arg_begin();
   auto llvm_arg_it = fn->arg_begin();
   auto llvm_arg_end = fn->arg_end();

   if (F->isLambda())
      ++llvm_arg_it;

   if (sret)
      ++llvm_arg_it;

   for (; llvm_arg_it != llvm_arg_end; ++il_arg_it, ++llvm_arg_it) {
      ValueMap[&*il_arg_it] = &*llvm_arg_it;
   }

   ValueMap[F] = fn;
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

void IRGen::ForwardDeclareType(ast::RecordDecl *R)
{
   if (isa<ProtocolDecl>(R))
      return;

   llvm::SmallString<128> typeName;
   switch (R->getKind()) {
      case Decl::ClassDeclID:
         typeName += "class";
         break;
      case Decl::StructDeclID:
         typeName += "struct";
         break;
      case Decl::EnumDeclID:
         typeName += "enum";
         break;
      case Decl::UnionDeclID:
         typeName += "union";
         break;
      default:
         llvm_unreachable("bad type kind");
   }

   typeName += '.';
   typeName += R->getFullName();

   auto llvmTy = llvm::StructType::create(Ctx, typeName);
   StructTypeMap[R] = llvmTy;
}

void IRGen::DeclareType(ast::RecordDecl *R)
{
   if (isa<ProtocolDecl>(R))
      return;

   llvm::SmallVector<llvm::Type*, 8> ContainedTypes;

   if (auto U = dyn_cast<UnionDecl>(R)) {
      auto *ArrTy = llvm::ArrayType::get(Builder.getInt8Ty(), U->getSize());
      ContainedTypes.push_back(ArrTy);
   }
   else if (auto S = dyn_cast<StructDecl>(R)) {
      if (isa<ClassDecl>(R)) {
         ContainedTypes.push_back(Builder.getInt64Ty());   // strong refcount
         ContainedTypes.push_back(Builder.getInt64Ty());   // weak refcount
         ContainedTypes.push_back(Builder.getInt8PtrTy()); // typeinfo
      }

      for (auto F : S->getFields()) {
         auto fieldTy = getStorageType(F->getType());
         ContainedTypes.push_back(fieldTy);
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      ContainedTypes.push_back(getStorageType(E->getRawType()));

      auto CaseSize = R->getSize() - TI.getSizeOfType(E->getRawType());
      if (CaseSize > 0) {
         auto *ArrTy = llvm::ArrayType::get(Builder.getInt8Ty(), CaseSize);
         ContainedTypes.push_back(ArrTy);
      }
   }

   if (ContainedTypes.empty())
      ContainedTypes.push_back(Int8PtrTy);

   StructTypeMap[R]->setBody(ContainedTypes);
}

void IRGen::ForwardDeclareGlobal(il::GlobalVariable const *G)
{
   llvm::Type *globalTy = getGlobalType(G->getType()->getReferencedType());

   auto GV = new llvm::GlobalVariable(*M, globalTy,
                            G->isConstant() && !G->isLazilyInitialized(),
                            (llvm::GlobalVariable::LinkageTypes)G->getLinkage(),
                            nullptr, G->getName());

   GV->setUnnamedAddr((llvm::GlobalVariable::UnnamedAddr)G->getUnnamedAddr());
   GV->setVisibility((llvm::GlobalVariable::VisibilityTypes)G->getVisibility());

   ValueMap[G] = GV;
}

void IRGen::DeclareGlobal(il::GlobalVariable const *G)
{
   auto glob = M->getNamedGlobal(G->getName());
   if (auto Init = G->getInitializer()) {
      auto InitVal = getConstantVal(Init);

      // this can happen with ConstantEnums that can't exactly match the
      // required type
      if (glob->getValueType() != InitVal->getType()) {
         auto NewGlobal = new llvm::GlobalVariable(*M, InitVal->getType(),
            glob->isConstant(), glob->getLinkage(), InitVal, glob->getName());

         glob->replaceAllUsesWith(NewGlobal);
         glob->eraseFromParent();

         ValueMap[G] = NewGlobal;
      }
      else {
         glob->setInitializer(InitVal);
      }
   }
   else {
      glob->setInitializer(llvm::ConstantAggregateZero::get(
         glob->getType()->getPointerElementType()));
   }
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
      Builder.SetCurrentDebugLocation(llvm::DebugLoc());
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

            if (!phi->getNumIncomingValues()) {
               assert(B.hasNoPredecessors() && "branch didn't provide value "
                                               "for block argument!");
               llvmBB->eraseFromParent();
            }
         }
      }
   }

   Builder.SetInsertPoint(AllocaBB);
   Builder.CreateBr(getBasicBlock(&F.getBasicBlocks().front()));

   if (DI) {
      emitArgumentDI(F, func);
      endScope();
   }
}

llvm::Value *IRGen::visitDebugLocInst(const DebugLocInst &I)
{
   assert(DI && "produced debug loc without debug information enabled!");
   Builder.SetCurrentDebugLocation(llvm::DebugLoc::get(
      I.getLine(),
      I.getCol(),
      ScopeStack.top()
   ));

   return nullptr;
}

llvm::Value *IRGen::visitDebugLocalInst(const DebugLocalInst &I)
{
   emitLocalVarDI(I);
   return nullptr;
}

void IRGen::visitBasicBlock(BasicBlock &B)
{
   Builder.SetInsertPoint(getBasicBlock(&B));

   if (!B.isEntryBlock() && !B.getArgs().empty()) {
      for (const auto &arg : B.getArgs()) {
         auto ty = getStorageType(arg.getType());
         if (ty->isStructTy())
            ty = ty->getPointerTo();

         ValueMap[&arg] = Builder.CreatePHI(ty, 0);
      }
   }

   for (auto const& I : B.getInstructions()) {
      auto val = visit(I);
      ValueMap[&I] = val;
   }

   assert(Builder.GetInsertBlock()->getTerminator()
          && "basic block not terminated!");
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

llvm::Constant* IRGen::toInt8Ptr(llvm::Constant *V)
{
   return llvm::ConstantExpr::getBitCast(V, Int8PtrTy);
}

llvm::Value* IRGen::CreateSizeOf(llvm::Type *Ty)
{
   return Builder.CreatePtrToInt(
      Builder.CreateInBoundsGEP(
         llvm::ConstantPointerNull::get(Ty->getPointerTo()),
         { Builder.getInt32(1) }),
      Builder.getInt64Ty());
}

llvm::Value* IRGen::CreateCall(il::Function *F,
                               llvm::SmallVector<llvm::Value*, 8> &args) {
   auto fun = getFunction(F);

   bool sret = F->hasStructReturn();
   if (sret) {
      auto alloca = CreateAlloca(F->getReturnType());
      args.insert(args.begin(), alloca);
   }

   auto call = Builder.CreateCall(fun, args);

   if (sret) {
      call->addParamAttr(0, llvm::Attribute::StructRet);
      return args.front();
   }

   return call;
}

llvm::AllocaInst* IRGen::CreateAlloca(QualType AllocatedType,
                                      size_t allocatedSize,
                                      unsigned int alignment) {
   if (!alignment)
      alignment = TI.getAlignOfType(AllocatedType);

   return CreateAlloca(getStorageType(AllocatedType), allocatedSize, alignment);
}

llvm::AllocaInst* IRGen::CreateAlloca(llvm::Type *AllocatedType,
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

llvm::Value *IRGen::CreateCopy(il::Value *Val)
{
   return CreateCopy(Val->getType(), getLlvmValue(Val));
}

llvm::Value *IRGen::CreateCopy(QualType Ty, llvm::Value *Val)
{
   llvm_unreachable("TODO!");
}

void IRGen::debugPrint(const llvm::Twine &str)
{
   Builder.CreateCall(getPrintfFn(),
                      { toInt8Ptr(Builder.CreateGlobalString(str.str())) });
}

unsigned IRGen::getFieldOffset(StructDecl *S,
                               const DeclarationName &FieldName) {
   unsigned offset = 0;
   if (isa<ClassDecl>(S))
      offset += 3;

   for (auto F : S->getFields()) {
      if (F->getDeclName() == FieldName)
         break;

      ++offset;
   }

   return offset;
}

QualType IRGen::getFieldType(ast::StructDecl *S,
                             const DeclarationName &FieldName) {
   for (auto F : S->getFields()) {
      if (F->getDeclName() == FieldName)
         return F->getType();
   }

   llvm_unreachable("bad field name");
}

llvm::Value* IRGen::AccessField(ast::StructDecl *S,
                                Value *Val,
                                const DeclarationName &FieldName,
                                bool load) {
   if (isa<UnionDecl>(S)) {
      auto GEP = Builder.CreateStructGEP(getStructTy(S), getLlvmValue(Val), 0);
      return Builder.CreateBitCast(
         GEP, getStorageType(getFieldType(S, FieldName))->getPointerTo());
   }

   auto offset = getFieldOffset(S, FieldName);
   auto val = Builder.CreateStructGEP(getStructTy(S), getLlvmValue(Val),
                                      offset);

   return load ? (llvm::Value*)Builder.CreateLoad(val) : val;
}

llvm::Value* IRGen::getLlvmValue(il::Value const *V)
{
   auto val = getPotentiallyBoxedValue(V);
   if (isa<AllocBoxInst>(V)) {
      val = unboxValue(val, V->getType());
   }

   return val;
}

llvm::Value* IRGen::getPotentiallyBoxedValue(const il::Value *V)
{
   if (auto B = dyn_cast<BasicBlock>(V))
      return getBasicBlock(B->getName());

   if (auto C = dyn_cast<Constant>(V))
      return getConstantVal(C);

   auto it = ValueMap.find(V);
   assert(it != ValueMap.end());

   return it->second;
}

llvm::Value* IRGen::unboxValue(llvm::Value *V, QualType Ty)
{
   auto Unboxed = Builder.CreateStructGEP(
      BoxTy, Builder.CreateBitCast(V, BoxTy->getPointerTo()), 3);

   return Builder.CreateBitCast(Unboxed, getStorageType(Ty));
}

void IRGen::buildConstantClass(llvm::SmallVectorImpl<llvm::Constant*> &Vec,
                               const ConstantClass *Class) {
   if (auto Base = Class->getBase())
      buildConstantClass(Vec, Base);

   for (const auto &el : Class->getElements())
      Vec.push_back(getConstantVal(el));
}

llvm::Constant* IRGen::getConstantVal(il::Constant const* C)
{
   if (auto Int = dyn_cast<ConstantInt>(C)) {
      if (Int->isCTFE())
         return Builder.getFalse();

      auto it = ValueMap.find(Int);
      if (it != ValueMap.end())
         return cast<llvm::ConstantInt>(it->getSecond());

      auto V = llvm::ConstantInt::get(getStorageType(Int->getType()),
                                      Int->getValue());

      ValueMap[Int] = V;
      return V;
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      if (F->getType()->isFloatTy())
         return llvm::ConstantFP::get(Builder.getFloatTy(),
                                      F->getFloatVal());

      auto it = ValueMap.find(F);
      if (it != ValueMap.end())
         return cast<llvm::ConstantFP>(it->getSecond());

      auto V = llvm::ConstantFP::get(Builder.getDoubleTy(), F->getDoubleVal());
      ValueMap[F] = V;

      return V;
   }

   if (auto S = dyn_cast<ConstantString>(C)) {
      auto it = ValueMap.find(S);
      if (it != ValueMap.end())
         return cast<llvm::ConstantExpr>(it->getSecond());

      auto StrConstant = llvm::ConstantDataArray::getString(Ctx, S->getValue());
      auto GV = new llvm::GlobalVariable(*M, StrConstant->getType(), true,
                                         llvm::GlobalValue::PrivateLinkage,
                                         StrConstant);

      GV->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);

      auto V = llvm::ConstantExpr::getBitCast(GV, Int8PtrTy);
      ValueMap[S] = V;

      return V;
   }

   if (auto A = dyn_cast<ConstantArray>(C)) {
      auto it = ValueMap.find(A);
      if (it != ValueMap.end())
         return cast<llvm::Constant>(it->getSecond());

      bool NeedStructTy = false;
      llvm::Type *ElementTy = getStorageType(A->getElementType());
      llvm::SmallVector<llvm::Constant*, 8> Els;

      for (const auto &el : A->getVec()) {
         auto V = getConstantVal(el);
         if (!ElementTy) {
            ElementTy = V->getType();
         }
         else if (V->getType() != ElementTy) {
            // can happen with a constant enum element type
            NeedStructTy = true;
         }

         Els.push_back(V);
      }

      llvm::Constant *V;
      if (NeedStructTy) {
         V = llvm::ConstantStruct::getAnon(Els);
      }
      else {
         auto ArrTy = llvm::ArrayType::get(ElementTy, Els.size());
         V = llvm::ConstantArray::get(ArrTy, Els);
      }

      ValueMap[A] = V;
      return V;
   }

   if (auto S = dyn_cast<ConstantStruct>(C)) {
      auto it = ValueMap.find(S);
      if (it != ValueMap.end())
         return cast<llvm::ConstantStruct>(it->getSecond());

      llvm::SmallVector<llvm::Constant*, 8> Vals;
      for (const auto &el : S->getElements())
         Vals.push_back(getConstantVal(el));

      auto V = llvm::ConstantStruct::get(cast<llvm::StructType>(
         getStorageType(S->getType())), Vals);

      ValueMap[S] = V;
      return V;
   }

   if (auto Tup = dyn_cast<ConstantTuple>(C)) {
      auto it = ValueMap.find(Tup);
      if (it != ValueMap.end())
         return cast<llvm::ConstantStruct>(it->getSecond());

      llvm::SmallVector<llvm::Constant*, 8> Vals;
      for (const auto &el : Tup->getVec())
         Vals.push_back(getConstantVal(el));

      auto V = llvm::ConstantStruct::getAnon(Ctx, Vals);

      ValueMap[Tup] = V;
      return V;
   }

   if (auto Class = dyn_cast<ConstantClass>(C)) {
      auto it = ValueMap.find(Class);
      if (it != ValueMap.end()) {
         return llvm::ConstantExpr::getBitCast(
            cast<llvm::GlobalVariable>(it->getSecond()),
            getParameterType(C->getType()));
      }

      llvm::SmallVector<llvm::Constant*, 8> Vals{
         WordOne,                                        // strong refcount
         WordZero,                                       // weak refcount
         toInt8Ptr(getConstantVal(Class->getTypeInfo())) // typeinfo
      };

      buildConstantClass(Vals, Class);

      auto V = llvm::ConstantStruct::getAnon(Ctx, Vals);
      auto GV = new llvm::GlobalVariable(*M, V->getType(), true,
                                      llvm::GlobalVariable::InternalLinkage, V);

      ValueMap[Class] = GV;
      return GV;
   }

   if (auto U = dyn_cast<ConstantUnion>(C)) {
      auto it = ValueMap.find(U);
      if (it != ValueMap.end())
         return llvm::ConstantExpr::getBitCast(
            cast<llvm::Constant>(it->getSecond()),
            getStorageType(U->getType())->getPointerTo());

      auto InitVal = getConstantVal(U->getInitVal());
      auto GV = new llvm::GlobalVariable(*M, InitVal->getType(), true,
                                         llvm::GlobalVariable::InternalLinkage,
                                         InitVal);

      ValueMap[U] = GV;
      return GV;
   }

   if (auto E = dyn_cast<ConstantEnum>(C)) {
      if (E->getType()->isRawEnum()) {
         assert(E->getCaseValues().empty());
         return getConstantVal(E->getDiscriminator());
      }

      auto it = ValueMap.find(E);
      if (it != ValueMap.end())
         return cast<llvm::Constant>(it->getSecond());

      auto RawTy = cast<EnumDecl>(E->getCase()->getRecord())->getRawType();

      unsigned CaseSize = TI.getSizeOfType(RawTy);
      unsigned TypeSize = TI.getSizeOfType(E->getType());

      llvm::SmallVector<llvm::Constant*, 4> CaseVals;
      for (auto &Val : E->getCaseValues()) {
         CaseSize += TI.getSizeOfType(Val->getType());
         CaseVals.push_back(getConstantVal(Val));
      }

      // add padding if necessary
      if (CaseSize < TypeSize) {
         auto PaddingTy = llvm::ArrayType::get(Builder.getInt8Ty(),
                                               TypeSize - CaseSize);
         CaseVals.push_back(llvm::ConstantAggregateZero::get(PaddingTy));
      }

      llvm::Constant *Vals = llvm::ConstantStruct::getAnon(Ctx, CaseVals);
      if (E->getCase()->isIndirect()) {
         Vals = new llvm::GlobalVariable(*M, Vals->getType(), true,
                                         llvm::GlobalVariable::InternalLinkage,
                                         Vals);
      }

      llvm::Constant *Val = llvm::ConstantStruct::getAnon(
         Ctx, { getConstantVal(E->getCase()->getILValue()), Vals });

      ValueMap[E] = Val;
      return Val;
   }

   if (auto F = dyn_cast<il::Function>(C))
      return cast<llvm::Function>(ValueMap[F]);

   if (auto P = dyn_cast<ConstantPointer>(C)) {
      auto ty = getStorageType(P->getType());
      if (!ty->isPointerTy())
         ty = ty->getPointerTo();

      return llvm::ConstantPointerNull::get(cast<llvm::PointerType>(ty));
   }

   if (auto Undef = dyn_cast<UndefValue>(C)) {
      return llvm::UndefValue::get(getStorageType(Undef->getType()));
   }

   if (auto G = dyn_cast<GlobalVariable>(C)) {
      return llvm::ConstantExpr::getBitCast(
         cast<llvm::GlobalVariable>(ValueMap[G]),
         getParameterType(G->getType()));
   }

   if (auto VT = dyn_cast<VTable>(C)) {
      auto it = ValueMap.find(VT);
      if (it != ValueMap.end())
         return cast<llvm::ConstantArray>(it->getSecond());

      auto ArrTy = llvm::ArrayType::get(Int8PtrTy, VT->getNumFunctions());
      llvm::SmallVector<llvm::Constant*, 8> Fns;

      for (auto &fn : VT->getFunctions())
         Fns.push_back(toInt8Ptr(getFunction(fn)));

      auto V = llvm::ConstantArray::get(ArrTy, Fns);
      ValueMap[VT] = V;

      return V;
   }

   if (auto TI = dyn_cast<TypeInfo>(C)) {
      auto it = ValueMap.find(TI);
      if (it != ValueMap.end())
         return cast<llvm::ConstantStruct>(it->getSecond());

      size_t i = 0;
      llvm::Constant *Values[MetaType::MemberCount];

      for (auto &Val : TI->getValues()) {
         switch (i) {
         case MetaType::Conformances:
            break;
         case MetaType::VTable:
         case MetaType::PTable:
            Values[i] = llvm::ConstantExpr::getBitCast(getConstantVal(Val),
                                                       EmptyArrayPtrTy);
            break;
         default:
            Values[i] = toInt8Ptr(getConstantVal(Val));
            break;
         }

         ++i;
      }

      llvm::Constant *ConformanceArray;
      if (cast<ConstantArray>(TI->getConformances())->getNumElements() != 0) {
         auto Conformances = getConstantVal(TI->getConformances());
         ConformanceArray =
            new llvm::GlobalVariable(*M, Conformances->getType(), true,
                                     llvm::GlobalVariable::PrivateLinkage,
                                     Conformances);

         ConformanceArray = llvm::ConstantExpr::getBitCast(ConformanceArray,
                                                           EmptyArrayPtrTy);
      }
      else {
         ConformanceArray = llvm::ConstantPointerNull::get(EmptyArrayPtrTy);
      }

      Values[MetaType::Conformances] = ConformanceArray;
      auto V = llvm::ConstantStruct::get(TypeInfoTy, Values);

      ValueMap[TI] = V;
      return V;
   }

   if (auto BC = dyn_cast<ConstantBitCastInst>(C)) {
      return llvm::ConstantExpr::getBitCast(getConstantVal(BC->getTarget()),
                                            getParameterType(BC->getType()));
   }

   if (auto AddrOf = dyn_cast<ConstantAddrOfInst>(C)) {
      return getConstantVal(AddrOf->getTarget());
   }

   if (auto Op = dyn_cast<ConstantOperatorInst>(C)) {
      auto ty = Op->getLHS()->getType();
      auto lhs = getConstantVal(Op->getLHS());
      auto rhs = getConstantVal(Op->getRHS());

      using OPC = ConstantOperatorInst::OpCode;
      switch (Op->getOpCode()) {
      case OPC::And:  return llvm::ConstantExpr::getAdd(lhs, rhs);
      case OPC::Or:   return llvm::ConstantExpr::getOr(lhs, rhs);
      case OPC::Xor:  return llvm::ConstantExpr::getXor(lhs, rhs);
      case OPC::LShr: return llvm::ConstantExpr::getLShr(lhs, rhs);
      case OPC::AShr: return llvm::ConstantExpr::getAShr(lhs, rhs);
      case OPC::Shl:  return llvm::ConstantExpr::getShl(lhs, rhs);
#  define INT_OR_FP_OP(Name)                                         \
      case OPC::Name:                                                \
         if (ty->isIntegerType()) {                                  \
            if (ty->isUnsigned())                                    \
               return llvm::ConstantExpr::getNUW##Name(lhs, rhs);    \
                                                                     \
            return llvm::ConstantExpr::getNSW##Name(lhs, rhs);       \
         }                                                           \
                                                                     \
         return llvm::ConstantExpr::getF##Name(lhs, rhs);
      INT_OR_FP_OP(Add)
      INT_OR_FP_OP(Sub)
      INT_OR_FP_OP(Mul)

#  undef INT_OR_FP_OP
      case OPC::Div:
         if (ty->isIntegerType()) {
            if (ty->isUnsigned())
               return llvm::ConstantExpr::getUDiv(lhs, rhs);

            return llvm::ConstantExpr::getSDiv(lhs, rhs);
         }

         return llvm::ConstantExpr::getFDiv(lhs, rhs);
      case OPC::Mod:
         if (ty->isIntegerType()) {
            if (ty->isUnsigned())
               return llvm::ConstantExpr::getURem(lhs, rhs);

            return llvm::ConstantExpr::getSRem(lhs, rhs);
         }

         return llvm::ConstantExpr::getFRem(lhs, rhs);
      case OPC::Exp:
         llvm_unreachable("constant exp is not supported!");
      }

      llvm_unreachable("bad op kind!");
   }

   if (auto GEP = dyn_cast<ConstantGEPInst>(C)) {
      auto val = getConstantVal(GEP->getTarget());

      if (GEP->getTarget()->getType()->stripReference()->isRecordType()) {
         auto R = GEP->getTarget()->getType()->stripReference()->getRecord();

         unsigned idx = cast<ConstantInt>(GEP->getIdx())->getU32();
         if (isa<ClassDecl>(R))
            idx += 3;

         return llvm::ConstantExpr::getInBoundsGetElementPtr(
            val->getType(), val,
            llvm::ArrayRef<llvm::Constant*>{ WordZero,
               getConstantVal(GEP->getIdx()) });
      }
      if (val->getType()->getPointerElementType()->isArrayTy()) {
         return llvm::ConstantExpr::getInBoundsGetElementPtr(
            val->getType(), val,
            llvm::ArrayRef<llvm::Constant*>{ WordZero,
               getConstantVal(GEP->getIdx()) });
      }

      return llvm::ConstantExpr::getInBoundsGetElementPtr(
         val->getType(), val,
         llvm::ArrayRef<llvm::Constant*>{ getConstantVal(GEP->getIdx()) });
   }

   if (auto IC = dyn_cast<ConstantIntCastInst>(C)) {
      auto val = getConstantVal(IC->getTarget());
      auto fromTy = IC->getTarget()->getType();
      auto toTy = getStorageType(IC->getType());

      switch (IC->getKind()) {
      case CastKind::IntToFP:
         if (fromTy->isUnsigned())
            return llvm::ConstantExpr::getUIToFP(val, toTy);

         return llvm::ConstantExpr::getSIToFP(val, toTy);
      case CastKind::FPToInt:
         if (IC->getType()->isUnsigned())
            return llvm::ConstantExpr::getFPToUI(val, toTy);

         return llvm::ConstantExpr::getFPToSI(val, toTy);
      case CastKind::IntToPtr:
         return llvm::ConstantExpr::getIntToPtr(val, toTy);
      case CastKind::PtrToInt:
         return llvm::ConstantExpr::getPtrToInt(val, toTy);
      case CastKind::Ext:
      case CastKind::Trunc:
         if (toTy->getIntegerBitWidth() > fromTy->getBitwidth()) {
            if (fromTy->isUnsigned())
               return llvm::ConstantExpr::getZExt(val, toTy);

            return llvm::ConstantExpr::getSExt(val, toTy);
         }
         else {
            return llvm::ConstantExpr::getTrunc(val, toTy);
         }
      case CastKind::SignFlip:
         return val;
      default:
         llvm_unreachable("not an integer cast!");
      }
   }

   if (auto Ld = dyn_cast<ConstantLoadInst>(C)) {
      return getConstantVal(Ld->getTarget());
   }

   llvm_unreachable("bad constant kind");
}

llvm::Value *IRGen::visitAllocaInst(AllocaInst const& I)
{
   llvm::Value *V = nullptr;
   if (I.canUseSRetValue()) {
      V = getCurrentSRetValue();
   }
   // this instruction should be immediately followed by a store, its value
   // will be replaced with the store source
   else if (I.canElideCopy()) {
      return nullptr;
   }
   else {
      unsigned alignment = I.getAlignment();
      if (!alignment) {
         alignment = TI.getAlignOfType(I.getType()->getReferencedType());
      }

      llvm::Type *allocatedType = getStorageType(
         I.getType()->getReferencedType());

      if (I.isHeapAlloca()) {
         auto TypeSize = TI.getAllocSizeOfType(
            I.getType()->getReferencedType());
         auto size = wordSizedInt(TypeSize * I.getAllocSize());
         auto buff = Builder.CreateCall(getMallocFn(), { size });

         V = Builder.CreateBitCast(buff, allocatedType->getPointerTo());
      }
      else {
         V = CreateAlloca(allocatedType, I.getAllocSize(), alignment);
      }
   }

   return V;
}

llvm::Value *IRGen::visitAllocBoxInst(const il::AllocBoxInst &I)
{
   QualType Ty = I.getType()->getReferencedType();

   llvm::Type *allocatedType = getStorageType(Ty);
   auto STy = llvm::StructType::get(Ctx, {
      WordTy,       // strong refcount
      WordTy,       // weak refcount
      Int8PtrTy,    // deinitializer
      allocatedType // data
   });

   auto TypeSize = TI.getSizeOfType(I.getType()->getReferencedType());
   auto size = wordSizedInt(TypeSize + 3 * TI.getPointerSizeInBytes());

   llvm::Value *Mem = Builder.CreateCall(getMallocFn(), { size });
   Mem = Builder.CreateBitCast(Mem, STy->getPointerTo());

   auto strongRefcountGEP = Builder.CreateStructGEP(STy, Mem, 0);
   Builder.CreateStore(wordSizedInt(1), strongRefcountGEP);

   auto weakRefcountGEP = Builder.CreateStructGEP(STy, Mem, 1);
   Builder.CreateStore(wordSizedInt(0), weakRefcountGEP);

   auto deinitGEP = Builder.CreateStructGEP(STy, Mem, 2);

   llvm::Constant *Deinit = nullptr;
   if (Ty->isClass()) {
      Deinit = getReleaseFn();
   }
   else if (Ty->isLambdaType()) {
      Deinit = getReleaseLambdaFn();
   }
   else if (auto Fn = I.getDeinitializer()) {
      Deinit = getFunction(Fn);
   }
   else {
      Deinit = llvm::ConstantPointerNull::get(Int8PtrTy);
   }

   Builder.CreateStore(toInt8Ptr(Deinit), deinitGEP);
   return Builder.CreateBitCast(Mem, BoxTy->getPointerTo());
}

llvm::Value *IRGen::visitStoreInst(StoreInst const& I)
{
   auto Dst = getLlvmValue(I.getDst());
   auto Src = getLlvmValue(I.getSrc());

   // can happen if we elided the store already
   if (Dst == Src)
      return nullptr;

   // we can elide this store because the stored value is a newly initialized
   // struct
   if (!Dst) {
      assert(isa<AllocaInst>(I.getDst())
             && cast<AllocaInst>(I.getDst())->canElideCopy()
             && "missing alloca!");

      assert(Src->getType()->isPointerTy() && "cannot elide copy of value!");

      if (DI) {
         assert(ElidedDebugLocalInst != nullptr && "no debug locatl inst!");

         emitLocalVarDI(*ElidedDebugLocalInst, Src);
         ElidedDebugLocalInst = nullptr;
      }

      ValueMap[I.getDst()] = Src;
      return Src;
   }

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

   if (I.getVal()->getType()->stripReference()->isRecordType()) {
      auto R = I.getVal()->getType()->stripReference()->getRecord();
      assert(isa<ConstantInt>(I.getIndex()));

      unsigned idx = cast<ConstantInt>(I.getIndex())->getU32();
      if (isa<ClassDecl>(R))
         idx += 3;

      return Builder.CreateStructGEP(getStructTy(R), val, idx);
   }
   if (val->getType()->getPointerElementType()->isArrayTy()) {
      return Builder.CreateInBoundsGEP(val, {
         wordSizedInt(0),
         getLlvmValue(I.getIndex())
      });
   }

   return Builder.CreateInBoundsGEP(val, getLlvmValue(I.getIndex()));
}

llvm::Value *IRGen::visitCaptureExtractInst(const CaptureExtractInst &I)
{
   auto F = Builder.GetInsertBlock()->getParent();
   auto env = F->arg_begin();

   auto idx = I.getIdx()->getU32();
   auto gep = Builder.CreateStructGEP(env->getType()->getPointerElementType(),
                                      env, idx);

   return unboxValue(Builder.CreateLoad(gep), I.getType());
}

llvm::Value *IRGen::visitFieldRefInst(FieldRefInst const& I)
{
   return AccessField(I.getAccessedType(), I.getOperand(0),
                      I.getFieldName());
}

llvm::Value *IRGen::visitTupleExtractInst(TupleExtractInst const& I)
{
   auto tup = getLlvmValue(I.getOperand(0));
   return Builder.CreateStructGEP(tup->getType()->getPointerElementType(),
                                  tup, I.getIdx()->getU32());
}

llvm::Value* IRGen::visitEnumExtractInst(const EnumExtractInst &I)
{
   auto Case = I.getCase();
   auto E = getLlvmValue(I.getOperand(0));

   unsigned idx = I.getCaseVal()->getU32();
   auto BeginPtr = Builder.CreateStructGEP(getStructTy(I.getEnumTy()), E, 1);

   if (Case->isIndirect()) {
      BeginPtr = Builder.CreateLoad(
         Builder.CreateBitCast(BeginPtr, Int8PtrTy->getPointerTo()));
   }

   auto CaseTy = getEnumCaseTy(Case);
   BeginPtr = Builder.CreateBitCast(BeginPtr, CaseTy->getPointerTo());

   return Builder.CreateStructGEP(CaseTy, BeginPtr, idx);
}

llvm::Value* IRGen::visitEnumRawValueInst(EnumRawValueInst const& I)
{
   auto E = getLlvmValue(I.getValue());
   if (E->getType()->isIntegerTy())
      return E;

   return Builder.CreateLoad(Builder.CreateStructGEP(E->getType()
                                             ->getPointerElementType(), E, 0));
}

llvm::Value *IRGen::visitLoadInst(LoadInst const& I)
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
//   llvm::SmallVector<llvm::Value*, 8> args;
//   for (const auto &arg : I.getArgs())
//      args.push_back(getLlvmValue(arg));
//
//   auto Self = args.front();
//   auto StructTy = cast<llvm::StructType>(Self->getType()
//                                              ->getPointerElementType());
//
//   auto ClassInfoPtr = Builder.CreateLoad(Builder.CreateStructGEP(StructTy,
//                                                                  Self, 0));
//
//   auto VTablePtr = Builder.CreateLoad(Builder.CreateStructGEP(ClassInfoTy,
//                                                               ClassInfoPtr,
//                                                               0));
//
//   auto GEP = Builder.CreateInBoundsGEP(VTablePtr,
//                                        Builder.getInt64(I.getCalledMethod()
//                                                          ->getVtableOffset()));
//
//   auto funcPtr = Builder.CreateBitCast(GEP, getStorageType(I.getCalledMethod()
//                                                             ->getType()));
//
//   return Builder.CreateInvoke(funcPtr,
//                               getBasicBlock(I.getNormalContinuation()),
//                               getBasicBlock(I.getLandingPad()), args);
   llvm_unreachable("TODO!");
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

   auto funcPtr = Builder.CreateBitCast(GEP, getStorageType(I.getCalledMethod()
                                                             ->getType()));

   auto ObjPtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                            Self, 0));
   auto Obj = Builder.CreateBitCast(ObjPtr, getStorageType(I.getCalledMethod()
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

llvm::Value* IRGen::getVTable(llvm::Value *llvmVal)
{
   auto TypeInfo = Builder.CreateStructGEP(llvmVal->getType()
                                                  ->getPointerElementType(),
                                           llvmVal, 2);

   llvm::Value *Ld = Builder.CreateLoad(TypeInfo);
   Ld = Builder.CreateBitCast(Ld, TypeInfoTy->getPointerTo());

   return Builder.CreateStructGEP(TypeInfoTy, Ld, MetaType::VTable);
}

llvm::Value* IRGen::getTypeInfo(llvm::Value *llvmVal)
{
   return Builder.CreateStructGEP(llvmVal->getType()
                                         ->getPointerElementType(),
                                  llvmVal, 2);
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

      if (I.getCalledIntrinsic() == Intrinsic::memcmp) {
         return Builder.CreateCall(getMemCmpFn(), {
            toInt8Ptr(Args[0]), toInt8Ptr(Args[1]), Args[2]
         });
      }

      return Builder.CreateMemSet(Args[0], Args[1], Args[2], 1);
   }
   case Intrinsic::release: {
      auto Val = toInt8Ptr(getPotentiallyBoxedValue(I.getOperand(0)));
      if (isa<AllocBoxInst>(I.getOperand(0))) {
         return Builder.CreateCall(getReleaseBoxFn(), { Val });
      }

      if (I.getOperand(0)->getType()->isLambdaType()) {
         return Builder.CreateCall(getReleaseLambdaFn(), { Val });
      }

      return Builder.CreateCall(getReleaseFn(), { Val });
   }
   case Intrinsic::retain: {
      auto Val = toInt8Ptr(getPotentiallyBoxedValue(I.getOperand(0)));
      if (isa<AllocBoxInst>(I.getOperand(0))) {
         return Builder.CreateCall(getRetainBoxFn(), { Val });
      }

      if (I.getOperand(0)->getType()->isLambdaType()) {
         return Builder.CreateCall(getRetainLambdaFn(), { Val });
      }

      return Builder.CreateCall(getRetainFn(), { Val });
   }
   case Intrinsic::lifetime_begin:
      return Builder.CreateLifetimeStart(getLlvmValue(I.getArgs()[0]),
                      cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));
   case Intrinsic::lifetime_end: {
      auto Self = getLlvmValue(I.getArgs()[0]);
      Builder.CreateLifetimeEnd(Self,
                      cast<llvm::ConstantInt>(getLlvmValue(I.getArgs()[1])));

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
      return getVTable(getLlvmValue(I.getArgs().front()));
   }
   case Intrinsic::typeinfo_ref: {
      return getTypeInfo(getLlvmValue(I.getArgs().front()));
   }
   case Intrinsic::indirect_case_ref: {
      auto Val = getLlvmValue(I.getArgs().front());
      return Builder.CreateInBoundsGEP(
         Builder.CreateBitCast(Val, Int8PtrTy->getPointerTo()),
         { WordOne });
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

llvm::Value *IRGen::visitVirtualCallInst(const VirtualCallInst &I)
{
   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   auto Self = args.front();
   auto SelfTy = cast<ClassDecl>(I.getCalledMethod()->getRecordType());

   llvm::Value *VTableRef = Builder.CreateLoad(getVTable(Self));

   auto VTableTy = llvm::ArrayType::get(Int8PtrTy, SelfTy->getNumVirtualFns());
   auto VTable = Builder.CreateBitCast(VTableRef, VTableTy->getPointerTo());

   size_t Offset = I.getCalledMethod()->getVtableOffset();
   auto FuncPtr = Builder.CreateInBoundsGEP(
      VTable, { Builder.getInt32(0), Builder.getInt32((unsigned)Offset) });

   auto TypedFnPtr = Builder.CreateBitCast(
      Builder.CreateLoad(FuncPtr),
      getStorageType(I.getCalledMethod()->getType()));

   return Builder.CreateCall(TypedFnPtr, args);
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
   auto funcPtr = Builder.CreateBitCast(GEP, getStorageType(I.getCalledMethod()
                                                             ->getType()));

   auto ObjPtr = Builder.CreateLoad(Builder.CreateStructGEP(ProtocolTy,
                                                            Self, 0));
   auto Obj = Builder.CreateBitCast(ObjPtr, getStorageType(I.getCalledMethod()
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
   llvm::SmallVector<llvm::Type*, 8> ArgTypes{ BoxTy->getPointerTo()
                                                    ->getPointerTo() };

   llvm::Type *retType;
   if (FTy->getReturnType()->needsStructReturn()) {
      retType = VoidTy;
      ArgTypes.push_back(getStorageType(FTy->getReturnType())->getPointerTo());
   }
   else {
      retType = getStorageType(FTy->getReturnType());
   }

   for (const auto &arg : FTy->getParamTypes()) {
      auto argTy = getStorageType(arg);
      if (argTy->isStructTy())
         argTy = argTy->getPointerTo();

      ArgTypes.push_back(argTy);
   }

   return llvm::FunctionType::get(retType, ArgTypes, false);
}

llvm::Value * IRGen::visitLambdaCallInst(LambdaCallInst const& I)
{
   auto Lambda = getLlvmValue(I.getLambda());

   auto Env = Builder.CreateStructGEP(LambdaTy, Lambda, 2);
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
      alloca = CreateAlloca(I.getType());
   }

   llvm::SmallVector<llvm::Value*, 8> args { alloca };
   for (const auto &arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   Builder.CreateCall(getFunction(I.getInit()), args);

   return alloca;
}

llvm::Value *IRGen::visitLambdaInitInst(LambdaInitInst const& I)
{
   auto fun = getLlvmValue(I.getFunction());

   llvm::Value *alloca;
   if (I.getNumOperands() != 0) {
      llvm::Value *Size = CreateSizeOf(LambdaTy);

      size_t EnvSize = TI.getPointerSizeInBytes();
      for (unsigned i = 0, numOps = I.getNumOperands(); i != numOps; ++i) {
         EnvSize += TI.getPointerSizeInBytes();
      }

      llvm::Value *TotalSize = Builder.CreateAdd(Size, wordSizedInt(EnvSize));

      alloca = Builder.CreateCall(getMallocFn(), TotalSize);
      alloca = Builder.CreateBitCast(alloca, LambdaTy->getPointerTo());

      auto ptr = Builder.CreateStructGEP(LambdaTy, alloca, 2);

      unsigned i = 0;
      for (const auto &capture : I.getOperands()) {
         auto val = getPotentiallyBoxedValue(capture);
         auto gep = Builder.CreateInBoundsGEP(ptr, { wordSizedInt(i) });

         Builder.CreateStore(val, gep);
         ++i;
      }

      Builder.CreateStore(llvm::ConstantPointerNull::get(BoxTy->getPointerTo()),
                          Builder.CreateInBoundsGEP(ptr, { wordSizedInt(i) }));
   }
   else {
      alloca = Builder.CreateCall(getMallocFn(), { CreateSizeOf(LambdaTy) });
      alloca = Builder.CreateBitCast(alloca, LambdaTy->getPointerTo());

      auto envPtr = Builder.CreateStructGEP(LambdaTy, alloca, 2);
      Builder.CreateStore(
         llvm::ConstantPointerNull::get(BoxTy->getPointerTo()), envPtr);
   }

   auto funPtr = Builder.CreateStructGEP(LambdaTy, alloca, 0);
   Builder.CreateStore(toInt8Ptr(fun), funPtr);

   auto refcPtr = Builder.CreateStructGEP(LambdaTy, alloca, 1);
   Builder.CreateStore(wordSizedInt(1llu), refcPtr);

   return alloca;
}

llvm::Value* IRGen::visitUnionInitInst(UnionInitInst const& I)
{
   auto UnionTy = getStructTy(I.getUnionTy());
   llvm::Value *alloca;
   if (I.canUseSRetValue()) {
      alloca = getCurrentSRetValue();
   }
   else {
      alloca = CreateAlloca(I.getType());
   }

   auto val = getLlvmValue(I.getInitializerVal());
   auto gep = Builder.CreateStructGEP(UnionTy, alloca, 0);
   gep = Builder.CreateBitCast(gep, val->getType()->getPointerTo());

   Builder.CreateStore(val, gep);

   return alloca;
}

llvm::Value* IRGen::InitEnum(ast::EnumDecl *EnumTy,
                             ast::EnumCaseDecl *Case,
                             llvm::ArrayRef<llvm::Value*> CaseVals,
                             bool CanUseSRetValue) {
   if (EnumTy->getMaxAssociatedTypes() == 0) {
      auto rawTy = getStorageType(EnumTy->getRawType());
      return llvm::ConstantInt::get(rawTy,
                                    cast<ConstantInt>(Case->getILValue())
                                       ->getZExtValue());
   }

   auto LlvmTy = getStructTy(EnumTy);

   llvm::Value *alloca;
   if (CanUseSRetValue) {
      alloca = getCurrentSRetValue();
   }
   else {
      alloca = CreateAlloca(CI.getContext().getRecordType(EnumTy));
   }

   auto CaseVal = getLlvmValue(Case->getILValue());

   auto gep = Builder.CreateStructGEP(LlvmTy, alloca, 0);
   Builder.CreateStore(CaseVal, gep);

   auto CaseTy = getEnumCaseTy(Case);

   llvm::Value *StoreDst;
   llvm::Value *IndirectStorage = nullptr;
   llvm::Value *ValuePtr = Builder.CreateStructGEP(LlvmTy, alloca, 1);

   // if the case is indirect, we need to heap allocate the storage for it
   // and store a pointer to that storage
   if (Case->isIndirect()) {
      StoreDst = Builder.CreateCall(getMallocFn(), { CreateSizeOf(CaseTy) });
      IndirectStorage = StoreDst;
   }
   else {
      StoreDst = ValuePtr;
   }

   StoreDst = Builder.CreateBitCast(StoreDst, CaseTy->getPointerTo());

   assert(Case->getArgs().size() == CaseVals.size() && "bad argument count!");

   unsigned i = 0;
   for (const auto &Arg : Case->getArgs()) {
      auto GEP = Builder.CreateStructGEP(CaseTy, StoreDst, i);

      auto val = CaseVals[i];
      if (Arg->getType()->needsStructReturn()) {
         Builder.CreateMemCpy(GEP, val, TI.getSizeOfType(Arg->getType()),
                              TI.getAlignOfType(Arg->getType()));
      }
      else {
         Builder.CreateStore(val, GEP);
      }

      ++i;
   }

   if (Case->isIndirect()) {
      Builder.CreateStore(
         IndirectStorage,
         Builder.CreateBitCast(ValuePtr, Int8PtrTy->getPointerTo()));
   }

   return alloca;
}

llvm::Value* IRGen::visitEnumInitInst(EnumInitInst const& I)
{
   llvm::SmallVector<llvm::Value*, 4> CaseVals;
   for (auto &Arg : I.getArgs())
      CaseVals.push_back(getLlvmValue(Arg));

   return InitEnum(I.getEnumTy(), I.getCase(), CaseVals, I.canUseSRetValue());
}

llvm::Value *IRGen::visitDeallocInst(const DeallocInst &I)
{
   if (I.isHeap())
      Builder.CreateCall(getFreeFn(), toInt8Ptr(getLlvmValue(I.getValue())));

   return nullptr;
}

llvm::Value *IRGen::visitDeallocBoxInst(const DeallocBoxInst &I)
{
   return nullptr;
}

llvm::Value *IRGen::visitDeinitializeLocalInst(const DeinitializeLocalInst &I)
{
   auto Val = getPotentiallyBoxedValue(I.getVal());
   auto Ty = I.getVal()->getType()->stripReference();
   
   if (isa<AllocBoxInst>(I.getVal())) {
      return Builder.CreateCall(getReleaseBoxFn(), { toInt8Ptr(Val) });
   }

   if (!Ty->needsStructReturn())
      Val = Builder.CreateLoad(Val);

   if (auto fn = I.getDeinitializer()) {
      return Builder.CreateCall(getFunction(fn), { Val });
   }

   if (Ty->isLambdaType()) {
      return Builder.CreateCall(getReleaseLambdaFn(), { toInt8Ptr(Val) });
   }

   return Builder.CreateCall(getReleaseFn(), { toInt8Ptr(Val) });
}

llvm::Value * IRGen::visitDeinitializeTemporaryInst(
                                          const DeinitializeTemporaryInst &I) {
   auto Val = getLlvmValue(I.getVal());
   if (auto fn = I.getDeinitializer()) {
      return Builder.CreateCall(getFunction(fn), { Val });
   }

   if (I.getVal()->getType()->isLambdaType()) {
      return Builder.CreateCall(getReleaseLambdaFn(), { toInt8Ptr(Val) });
   }

   return Builder.CreateCall(getReleaseFn(), { toInt8Ptr(Val) });
}

llvm::Value* IRGen::applyBinaryOp(unsigned OpCode,
                                  QualType ty,
                                  llvm::Value *lhs,
                                  llvm::Value *rhs) {
   using OPC = BinaryOperatorInst::OpCode;
   switch ((OPC)OpCode) {
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
         powFn = getIntPowFn(ty);
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

llvm::Value* IRGen::visitBinaryOperatorInst(const BinaryOperatorInst &I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));
   
   return applyBinaryOp(I.getOpCode(), ty, lhs, rhs);
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
      if (target->getType()->isIntegerTy()) {
         return Builder.CreateNSWSub(
            Builder.getIntN(target->getType()->getIntegerBitWidth(), 0),
            target);
      }

      return Builder.CreateFSub(llvm::ConstantFP::get(target->getType(), 0.0),
                                target);
   }
   }

   llvm_unreachable("bad unary inst");
}

llvm::Value* IRGen::visitBitCastInst(BitCastInst const& I)
{
   return Builder.CreateBitCast(getLlvmValue(I.getOperand(0)),
                                getParameterType(I.getType()));
}

llvm::Value * IRGen::visitIntegerCastInst(IntegerCastInst const& I)
{
   auto val = getLlvmValue(I.getOperand(0));
   auto fromTy = I.getOperand(0)->getType();
   auto toTy = getStorageType(I.getType());

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
   auto toTy = getStorageType(I.getType());

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
   auto toTy = getStorageType(I.getType());

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
   auto toTy = getStorageType(I.getType());

   return Builder.CreateBitCast(Exc, toTy);
}

llvm::Value* IRGen::visitDynamicCastInst(const DynamicCastInst &I)
{
   auto Val = getLlvmValue(I.getOperand(0));
   assert(I.getOperand(0)->getType()->isClass() && "invalid dyn_cast type!");

   auto TargetTI = getLlvmValue(ILMod->getTypeInfo(I.getTargetType()));
   auto GivenTI = Builder.CreateLoad(getTypeInfo(Val));

   auto Alloc = CreateAlloca(getStorageType(I.getType()));

   auto SuccBB = llvm::BasicBlock::Create(
      Ctx, "dyn_cast.success", Builder.GetInsertBlock()->getParent());

   auto FailBB = llvm::BasicBlock::Create(
      Ctx, "dyn_cast.fail", Builder.GetInsertBlock()->getParent());

   auto MergeBB = llvm::BasicBlock::Create(
      Ctx, "dyn_cast.merge", Builder.GetInsertBlock()->getParent());

   auto Cmp = Builder.CreateICmpEQ(toInt8Ptr(TargetTI), toInt8Ptr(GivenTI));
   Builder.CreateCondBr(Cmp, SuccBB, FailBB);

   Builder.SetInsertPoint(SuccBB);
   Builder.CreateStore(Builder.getInt64(1),
                       Builder.CreateStructGEP(Alloc->getAllocatedType(),
                                               Alloc, 0));

   auto BC = Builder.CreateBitCast(Val, Int8PtrTy);
   auto Dst = Builder.CreateBitCast(
      Builder.CreateStructGEP(Alloc->getAllocatedType(), Alloc, 1),
      Int8PtrTy->getPointerTo());

   Builder.CreateStore(BC, Dst);

   Builder.CreateBr(MergeBB);

   Builder.SetInsertPoint(FailBB);
   Builder.CreateStore(Builder.getInt64(0),
                       Builder.CreateStructGEP(Alloc->getAllocatedType(),
                                               Alloc, 0));

   Builder.CreateBr(MergeBB);

   Builder.SetInsertPoint(MergeBB);
   return Alloc;
}

} // namespace il
} // namespace cdot