#include "cdotc/IRGen/IRGen.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Basic/CastKind.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/Basic/FileUtils.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/IL/Utils/BlockIterator.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Sema/SemaPass.h"

#include <llvm/IR/AssemblyAnnotationWriter.h>
#include <llvm/IR/DIBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>

using namespace cdot::support;
using namespace cdot::ast;
using namespace cdot::fs;

namespace cdot {
namespace il {

namespace {

class IRGenPrettyStackTraceEntry : public llvm::PrettyStackTraceEntry {
public:
   explicit IRGenPrettyStackTraceEntry(const il::Function& F) : F(F) {}

   void print(raw_ostream& OS) const override
   {
      OS << "while generating IR for function '" << F.getUnmangledName() << "'";

      if (!F.isDeclared()) {
         OS << " (dumped function contents)\n";
         F.print(OS);
      }

      OS << "\n";
   }

private:
   const il::Function& F;
};

} // anonymous namespace

#ifndef NDEBUG

LLVM_ATTRIBUTE_UNUSED
void dump(llvm::Function* F) { F->print(llvm::outs()); }

#endif

IRGen::IRGen(CompilerInstance& CI, llvm::LLVMContext& Ctx, bool emitDebugInfo)
    : CI(CI), Sema(CI.getSema()), TI(CI.getContext().getTargetInfo()),
      emitDebugInfo(emitDebugInfo), DI(nullptr), Ctx(Ctx), M(nullptr),
      Builder(Ctx)
{
   llvm::InitializeAllTargetInfos();
   llvm::InitializeAllTargets();
   llvm::InitializeAllTargetMCs();
   llvm::InitializeAllAsmParsers();
   llvm::InitializeAllAsmPrinters();

   WordTy = llvm::IntegerType::get(Ctx, sizeof(void*) * 8);
   Int8PtrTy = llvm::IntegerType::get(Ctx, 8)->getPointerTo();
   EmptyArrayPtrTy = llvm::ArrayType::get(Int8PtrTy, 0)->getPointerTo();
   Int1Ty = llvm::IntegerType::get(Ctx, 1);
   VoidTy = llvm::Type::getVoidTy(Ctx);

   DeinitializerTy = llvm::FunctionType::get(VoidTy, {Int8PtrTy}, false);

   EmptyTupleTy = llvm::StructType::get(Ctx, {});
   EmptyTuple = llvm::UndefValue::get(EmptyTupleTy->getPointerTo());

   llvm::Type* errorContainedTypes[] = {
       Int8PtrTy, // typeinfo ptr
       Int8PtrTy, // deinit fn
       Int8PtrTy  // object ptr
   };

   ErrorTy = llvm::StructType::create(Ctx, errorContainedTypes, "cdot.Error");

   llvm::Type* boxContainedTypes[] = {
       WordTy,             // strong refcount
       WordTy,             // weak refcount
       Int8PtrTy,          // deinitializer
       Builder.getInt8Ty() // data
   };

   BoxTy = llvm::StructType::create(Ctx, boxContainedTypes, "cdot.Box");

   llvm::Type* lambdaContainedTypes[] = {
       Int8PtrTy,            // function ptr
       WordTy,               // strong refcount
       BoxTy->getPointerTo() // first environment value
                             // (trailing objects follow)
   };

   LambdaTy
       = llvm::StructType::create(Ctx, lambdaContainedTypes, "cdot.Lambda");

   WordZero = Builder.getInt64(0);
   WordOne = Builder.getInt64(1);
}

IRGen::~IRGen() { delete TargetMachine; }

void IRGen::visitCompilationUnit(CompilerInstance& CU)
{
   visitModule(*CU.getCompilationModule()->getILModule());
}

void IRGen::visitModule(Module& ILMod)
{
   M = new llvm::Module(ILMod.getFileName(), Ctx);
   ILMod.setLLVMModule(M);

   if (emitDebugInfo) {
      assert(CI.getSourceLoc()
             && "translation unit with invalid source location");

      DI = new llvm::DIBuilder(*this->M);
      File = DI->createFile(ILMod.getFileName(), ILMod.getPath());

      this->CU = DI->createCompileUnit(llvm::dwarf::DW_LANG_C, File,
                                       "cdotc v0.1", false, "", 0);

      beginScope(this->CU);
   }

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
   TypeInfoCmpFn = nullptr;
   PrintExceptionFn = nullptr;
   CleanupExceptionFn = nullptr;
   ExitFn = nullptr;

   InitializeExistentialFn = nullptr;
   DeinitializeExistentialFn = nullptr;
   CopyExistentialFn = nullptr;
   CastExistentialFn = nullptr;
   CastExistentialFallibleFn = nullptr;
   UnwrapExistentialFn = nullptr;
   CopyClassFn = nullptr;
   GetConformanceFn = nullptr;
   GetProtocolVTableFn = nullptr;

   GetGenericArgumentFn = nullptr;
   GetTemplateParamTypeValueFn = nullptr;

   DynamicDownCastFn = nullptr;
   RuntimeFunctions.clear();

   auto* TypeInfoDecl = Sema.getTypeInfoDecl();
   auto* ECDecl = Sema.getExistentialContainerDecl();
   auto* ConfDecl = Sema.getProtocolConformanceDecl();
   //   auto *GenericEnvDecl = Sema.getGenericEnvironmentDecl();

   ILMod.addRecord(TypeInfoDecl);
   ILMod.addRecord(ECDecl);
   ILMod.addRecord(ConfDecl);
   //   ILMod.addRecord(GenericEnvDecl);

   for (auto Ty : ILMod.getRecords()) {
      if (StructTypeMap.find(Ty) != StructTypeMap.end())
         continue;

      // forward declare all types, they might be referenced when defining
      // others
      ForwardDeclareType(Ty);
   }

   TypeInfoTy = getStructTy(TypeInfoDecl);
   ExistentialContainerTy = getStructTy(ECDecl);
   ProtocolConformanceTy = getStructTy(ConfDecl);
   //   GenericEnvironmentTy = getStructTy(GenericEnvDecl);

   for (auto Ty : ILMod.getRecords()) {
      DeclareType(Ty);
   }

   for (const auto& F : ILMod.getFuncList()) {
      DeclareFunction(&F);
   }

   for (const auto& G : ILMod.getGlobalList())
      ForwardDeclareGlobal(&G);

   for (const auto& G : ILMod.getGlobalList())
      DeclareGlobal(&G);

   for (auto& F : ILMod.getFuncList())
      visitFunction(F);

   auto VoidFnTy = llvm::FunctionType::get(Builder.getVoidTy(), false);

   if (!GlobalInitFns.empty()) {
      auto ctorStructTy = llvm::StructType::get(
          Builder.getInt32Ty(), VoidFnTy->getPointerTo(), Int8PtrTy);

      llvm::SmallVector<llvm::Constant*, 4> Vals;
      for (auto& Fn : GlobalInitFns) {
         Vals.push_back(llvm::ConstantStruct::get(
             ctorStructTy, Builder.getInt32(Fn.second), Fn.first,
             llvm::ConstantPointerNull::get(Int8PtrTy)));
      }

      auto ctorArrayTy
          = llvm::ArrayType::get(ctorStructTy, GlobalInitFns.size());

      auto ctors = new llvm::GlobalVariable(
          *M, ctorArrayTy, true, llvm::GlobalVariable::AppendingLinkage,
          nullptr, "llvm.global_ctors");

      ctors->setInitializer(llvm::ConstantArray::get(ctorArrayTy, Vals));
   }

   if (!GlobalDeinitFns.empty()) {
      auto dtorStructTy = llvm::StructType::get(
          Builder.getInt32Ty(), VoidFnTy->getPointerTo(), Int8PtrTy);

      llvm::SmallVector<llvm::Constant*, 4> Vals;
      for (auto& Fn : GlobalDeinitFns) {
         Vals.push_back(llvm::ConstantStruct::get(
             dtorStructTy, Builder.getInt32(Fn.second), Fn.first,
             llvm::ConstantPointerNull::get(Int8PtrTy)));
      }

      auto dtorArrayTy
          = llvm::ArrayType::get(dtorStructTy, GlobalDeinitFns.size());

      auto dtors = new llvm::GlobalVariable(
          *M, dtorArrayTy, true, llvm::GlobalVariable::AppendingLinkage,
          nullptr, "llvm.global_dtors");

      dtors->setInitializer(llvm::ConstantArray::get(dtorArrayTy, Vals));
   }

   Intrinsics.clear();
   IntrinsicDecls.clear();
   GlobalInitFns.clear();
   GlobalDeinitFns.clear();
   ValueMap.clear();

   finalize(CI);

   if (DI) {
      endScope();

      delete DI;
      DI = nullptr;
   }

   DIFuncMap.clear();
   DIFileMap.clear();
   DITypeMap.clear();
   DIVarMap.clear();

   DI = nullptr;
   File = nullptr;
   this->CU = nullptr;

   assert(ScopeStack.empty() && "didn't pop scope!");
}

/// If given, IL will be emitted without debug info even if it is created.
static llvm::cl::opt<bool>
    AssumeSingleThread("fassume-single-threaded",
                       llvm::cl::desc("use non-atomic reference counting"));

llvm::Constant* IRGen::getMallocFn()
{
   if (!MallocFn) {
      MallocFn = M->getOrInsertFunction("_cdot_Malloc", Int8PtrTy, WordTy);
   }

   return MallocFn;
}

llvm::Constant* IRGen::getFreeFn()
{
   if (!FreeFn)
      FreeFn = M->getOrInsertFunction("_cdot_Free", VoidTy, Int8PtrTy);

   return FreeFn;
}

llvm::Constant* IRGen::getThrowFn()
{
   if (!ThrowFn)
      ThrowFn = M->getOrInsertFunction("__cdot_throw", VoidTy, Int8PtrTy);

   return ThrowFn;
}

llvm::Constant* IRGen::getAllocExcnFn()
{
   if (!AllocExcFn)
      AllocExcFn
          = M->getOrInsertFunction("__cdot_allocate_exception", Int8PtrTy,
                                   WordTy, Int8PtrTy, Int8PtrTy, Int8PtrTy);

   return AllocExcFn;
}

llvm::Constant* IRGen::getRetainFn()
{
   if (!RetainFn) {
      if (AssumeSingleThread) {
         RetainFn = M->getOrInsertFunction("_cdot_Retain", VoidTy, Int8PtrTy);
      }
      else {
         RetainFn
             = M->getOrInsertFunction("_cdot_AtomicRetain", VoidTy, Int8PtrTy);
      }
   }

   return RetainFn;
}

llvm::Constant* IRGen::getReleaseFn()
{
   if (!ReleaseFn) {
      if (AssumeSingleThread) {
         ReleaseFn = M->getOrInsertFunction("_cdot_Release", VoidTy, Int8PtrTy);
      }
      else {
         ReleaseFn
             = M->getOrInsertFunction("_cdot_AtomicRelease", VoidTy, Int8PtrTy);
      }
   }

   return ReleaseFn;
}

llvm::Constant* IRGen::getRetainLambdaFn()
{
   if (!RetainLambdaFn) {
      if (AssumeSingleThread) {
         RetainLambdaFn
             = M->getOrInsertFunction("_cdot_RetainLambda", VoidTy, Int8PtrTy);
      }
      else {
         RetainLambdaFn = M->getOrInsertFunction("_cdot_AtomicRetainLambda",
                                                 VoidTy, Int8PtrTy);
      }
   }

   return RetainLambdaFn;
}

llvm::Constant* IRGen::getReleaseLambdaFn()
{
   if (!ReleaseLambdaFn) {
      if (AssumeSingleThread) {
         ReleaseLambdaFn
             = M->getOrInsertFunction("_cdot_ReleaseLambda", VoidTy, Int8PtrTy);
      }
      else {
         ReleaseLambdaFn = M->getOrInsertFunction("_cdot_AtomicReleaseLambda",
                                                  VoidTy, Int8PtrTy);
      }
   }

   return ReleaseLambdaFn;
}

llvm::Constant* IRGen::getRetainBoxFn()
{
   if (!RetainBoxFn) {
      if (AssumeSingleThread) {
         RetainBoxFn
             = M->getOrInsertFunction("_cdot_RetainBox", VoidTy, Int8PtrTy);
      }
      else {
         RetainBoxFn = M->getOrInsertFunction("_cdot_AtomicRetainBox", VoidTy,
                                              Int8PtrTy);
      }
   }

   return RetainBoxFn;
}

llvm::Constant* IRGen::getReleaseBoxFn()
{
   if (!ReleaseBoxFn) {
      if (AssumeSingleThread) {
         ReleaseBoxFn
             = M->getOrInsertFunction("_cdot_ReleaseBox", VoidTy, Int8PtrTy);
      }
      else {
         ReleaseBoxFn = M->getOrInsertFunction("_cdot_AtomicReleaseBox", VoidTy,
                                               Int8PtrTy);
      }
   }

   return ReleaseBoxFn;
}

llvm::Constant* IRGen::getTypeInfoCmpFn()
{
   if (!TypeInfoCmpFn)
      TypeInfoCmpFn = M->getOrInsertFunction(
          "_cdot_TypeInfoCmp", Builder.getInt1Ty(), Int8PtrTy, Int8PtrTy);

   return TypeInfoCmpFn;
}

llvm::Constant* IRGen::getExitFn()
{
   if (!ExitFn)
      ExitFn = M->getOrInsertFunction("exit", VoidTy, Builder.getInt32Ty());

   return ExitFn;
}

llvm::Constant* IRGen::getPrintExceptionFn()
{
   if (!PrintExceptionFn)
      PrintExceptionFn
          = M->getOrInsertFunction("_cdot_PrintException", VoidTy, Int8PtrTy);

   return PrintExceptionFn;
}

llvm::Constant* IRGen::getCleanupExceptionFn()
{
   if (!CleanupExceptionFn)
      CleanupExceptionFn
          = M->getOrInsertFunction("_cdot_CleanupException", VoidTy, Int8PtrTy);

   return CleanupExceptionFn;
}

llvm::Constant* IRGen::getPrintfFn()
{
   if (!PrintfFn)
      PrintfFn = M->getOrInsertFunction(
          "printf",
          llvm::FunctionType::get(Builder.getInt32Ty(), {Int8PtrTy}, true));

   return PrintfFn;
}

llvm::Constant* IRGen::getMemCmpFn()
{
   if (!MemCmpFn)
      MemCmpFn = M->getOrInsertFunction(
          "memcmp",
          llvm::FunctionType::get(Builder.getInt32Ty(),
                                  {Int8PtrTy, Int8PtrTy, WordTy}, false));

   return MemCmpFn;
}

llvm::Constant* IRGen::getInitializeExistentialFn()
{
   if (!InitializeExistentialFn) {
      InitializeExistentialFn = M->getOrInsertFunction(
          "_cdot_InitializeExistential",
          llvm::FunctionType::get(
              VoidTy, {Int8PtrTy, Int8PtrTy, Int8PtrTy, Int8PtrTy}, false));
   }

   return InitializeExistentialFn;
}

llvm::Constant* IRGen::getDeinitializeExistentialFn()
{
   if (!DeinitializeExistentialFn) {
      DeinitializeExistentialFn = M->getOrInsertFunction(
          "_cdot_DeinitializeExistential",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy}, false));
   }

   return DeinitializeExistentialFn;
}

llvm::Constant* IRGen::getCopyExistentialFn()
{
   if (!CopyExistentialFn) {
      CopyExistentialFn = M->getOrInsertFunction(
          "_cdot_CopyExistential",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy}, false));
   }

   return CopyExistentialFn;
}

llvm::Constant* IRGen::getCastExistentialFn()
{
   if (!CastExistentialFn) {
      CastExistentialFn = M->getOrInsertFunction(
          "_cdot_ExistentialCast",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy, Int8PtrTy},
                                  false));
   }

   return CastExistentialFn;
}

llvm::Constant* IRGen::getCastExistentialFallibleFn()
{
   if (!CastExistentialFallibleFn) {
      CastExistentialFallibleFn = M->getOrInsertFunction(
          "_cdot_ExistentialCastFallible",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy, Int8PtrTy},
                                  false));
   }

   return CastExistentialFallibleFn;
}

llvm::Constant* IRGen::getUnwrapExistentialFn()
{
   if (!UnwrapExistentialFn) {
      UnwrapExistentialFn = M->getOrInsertFunction(
          "_cdot_ExistentialUnwrap",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy, Int8PtrTy},
                                  false));
   }

   return UnwrapExistentialFn;
}

llvm::Constant* IRGen::getCopyClassFn()
{
   if (!CopyClassFn) {
      CopyClassFn = M->getOrInsertFunction(
          "_cdot_CopyClass",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy}, false));
   }

   return CopyClassFn;
}

llvm::Constant* IRGen::getGetConformanceFn()
{
   if (!GetConformanceFn) {
      GetConformanceFn = M->getOrInsertFunction(
          "_cdot_GetConformance",
          llvm::FunctionType::get(Int8PtrTy, {Int8PtrTy, Int8PtrTy}, false));
   }

   return GetConformanceFn;
}

llvm::Constant* IRGen::getGetProtocolVTableFn()
{
   if (!GetProtocolVTableFn) {
      GetProtocolVTableFn = M->getOrInsertFunction(
          "_cdot_GetProtocolVTable",
          llvm::FunctionType::get(Int8PtrTy, {Int8PtrTy, Int8PtrTy}, false));
   }

   return GetProtocolVTableFn;
}

llvm::Constant* IRGen::getGetGenericArgumentFn()
{
   if (!GetGenericArgumentFn) {
      GetGenericArgumentFn = M->getOrInsertFunction(
          "_cdot_GetGenericArgument",
          llvm::FunctionType::get(Int8PtrTy, {Int8PtrTy, WordTy, WordTy},
                                  false));
   }

   return GetGenericArgumentFn;
}

llvm::Constant* IRGen::getGetTemplateParamTypeValueFn()
{
   if (!GetTemplateParamTypeValueFn) {
      GetTemplateParamTypeValueFn = M->getOrInsertFunction(
          "_cdot_GetTemplateParamTypeValue",
          llvm::FunctionType::get(Int8PtrTy, {Int8PtrTy}, false));
   }

   return GetTemplateParamTypeValueFn;
}

llvm::Constant* IRGen::getDynamicDownCastFn()
{
   if (!DynamicDownCastFn) {
      DynamicDownCastFn = M->getOrInsertFunction(
          "_cdot_DynamicDownCast",
          llvm::FunctionType::get(VoidTy, {Int8PtrTy, Int8PtrTy, Int8PtrTy},
                                  false));
   }

   return DynamicDownCastFn;
}

llvm::Constant* IRGen::getIntPowFn(QualType IntTy)
{
   auto llvmTy = getStorageType(IntTy);
   llvm::StringRef funcName;

   switch (cast<BuiltinType>(IntTy)->getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, Unsigned)                                   \
   case BuiltinType::Name:                                                     \
      funcName = "_cdot_intpow_" #Name;                                        \
      break;
#include "cdotc/Basic/BuiltinTypes.def"
   default:
      llvm_unreachable("not an integer type!");
   }

   return M->getOrInsertFunction(
       funcName, llvm::FunctionType::get(llvmTy, {llvmTy, llvmTy}, false));
}

llvm::CallInst* IRGen::CallRuntimeFunction(StringRef FuncName,
                                           ArrayRef<llvm::Value*> Args,
                                           llvm::Type* RetTy)
{
   auto It = RuntimeFunctions.find(FuncName);
   if (It != RuntimeFunctions.end())
      return Builder.CreateCall(It->getSecond(), Args);

   SmallVector<llvm::Type*, 2> ParamTys;
   for (auto* V : Args)
      ParamTys.push_back(V->getType());

   auto* Fn = M->getOrInsertFunction(
       FuncName,
       llvm::FunctionType::get(RetTy ? RetTy : VoidTy, ParamTys, false));

   RuntimeFunctions[FuncName] = Fn;
   return Builder.CreateCall(Fn, Args);
}

void IRGen::addMappedValue(const il::Value* ILVal, llvm::Value* LLVMVal)
{
   ValueMap[ILVal] = LLVMVal;
}

bool IRGen::IsSmallStruct(cdot::CanType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::ArrayTypeID:
   case Type::TupleTypeID:
      return !NeedsStructReturn(Ty);
   case Type::RecordTypeID: {
      auto* R = Ty->getRecord();
      switch (R->getKind()) {
      case Decl::StructDeclID:
         return !NeedsStructReturn(Ty);
      default:
         return false;
      }
   }
   default:
      return false;
   }
}

bool IRGen::NeedsStructReturn(CanType Ty)
{
   return Sema.NeedsStructReturn(Ty->getDesugaredType());
}

llvm::StructType* IRGen::getEnumCaseTy(ast::EnumCaseDecl* Decl)
{
   auto it = EnumCaseTys.find(Decl);
   if (it != EnumCaseTys.end())
      return it->getSecond();

   unsigned Size = 0;
   llvm::SmallVector<llvm::Type*, 4> ContainedTys;

   for (auto& Val : Decl->getArgs()) {
      Size += TI.getSizeOfType(Val->getType());
      ContainedTys.push_back(getStorageType(Val->getType()));
   }

   // add padding to get to the same size as the Enum type
   auto NeededCaseValSize
       = TI.getSizeOfType(cast<EnumDecl>(Decl->getRecord())->getType())
         - TI.getSizeOfType(cast<EnumDecl>(Decl->getRecord())->getRawType());

   if (Size < NeededCaseValSize) {
      ContainedTys.push_back(
          llvm::ArrayType::get(Builder.getInt8Ty(), NeededCaseValSize - Size));
   }

   auto llvmTy = llvm::StructType::get(Ctx, ContainedTys);
   EnumCaseTys[Decl] = llvmTy;

   return llvmTy;
}

llvm::StructType* IRGen::getStructTy(QualType Ty)
{
   return getStructTy(Ty->getRecord());
}

llvm::StructType* IRGen::getStructTy(RecordDecl* R)
{
   if (isa<ProtocolDecl>(R))
      return ExistentialContainerTy;

   auto Ty = StructTypeMap[R];
   if (Ty)
      return Ty;

   ForwardDeclareType(R);
   DeclareType(R);

   return StructTypeMap[R];
}

llvm::Type* IRGen::getLlvmTypeImpl(CanType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
      switch (Ty->asBuiltinType()->getKind()) {
      case BuiltinType::Void:
         return VoidTy;
#define CDOT_BUILTIN_INT(Name, BW, isUnsigned)                                 \
   case BuiltinType::Name:                                                     \
      return Builder.getIntNTy(BW);
#include "cdotc/Basic/BuiltinTypes.def"

      case BuiltinType::f16:
         return Builder.getHalfTy();
      case BuiltinType::f32:
         return Builder.getFloatTy();
      case BuiltinType::f64:
         return Builder.getDoubleTy();
      case BuiltinType::f80:
         return llvm::Type::getX86_FP80Ty(Ctx);
      case BuiltinType::f128:
         return llvm::Type::getFP128Ty(Ctx);
      default:
         llvm_unreachable("bad builtin type");
      }
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID: {
      auto pointee = Ty->getPointeeType();
      if (pointee->isVoidType())
         return Int8PtrTy;

      return getStorageType(pointee)->getPointerTo();
   }
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID: {
      QualType referenced = Ty->getReferencedType();
      return getStorageType(referenced)->getPointerTo();
   }
   case Type::BoxTypeID:
      return BoxTy;
   case Type::TokenTypeID:
      return llvm::Type::getTokenTy(Ctx);
   case Type::ArrayTypeID: {
      ArrayType* ArrTy = Ty->asArrayType();
      return llvm::ArrayType::get(getStorageType(ArrTy->getElementType()),
                                  ArrTy->getNumElements());
   }
   case Type::LambdaTypeID:
      return LambdaTy->getPointerTo();
   case Type::FunctionTypeID: {
      FunctionType* FuncTy = Ty->asFunctionType();

      QualType RetType = FuncTy->getReturnType();
      bool sret = NeedsStructReturn(RetType);

      llvm::Type* LLVMRetTy;
      if (RetType->isEmptyTupleType() || sret) {
         LLVMRetTy = VoidTy;
      }
      else {
         LLVMRetTy = getStorageType(RetType);
      }

      SmallVector<llvm::Type*, 4> argTypes;
      if (FuncTy->throws()) {
         argTypes.push_back(Int8PtrTy->getPointerTo());
      }
      if (sret) {
         argTypes.push_back(getParameterType(RetType));
      }

      for (const auto& arg : FuncTy->getParamTypes())
         argTypes.push_back(getParameterType(arg));

      auto* FnTy = llvm::FunctionType::get(LLVMRetTy, argTypes,
                                           FuncTy->isCStyleVararg());

      return FnTy->getPointerTo();
   }
   case Type::TupleTypeID: {
      TupleType* TupleTy = Ty->asTupleType();
      if (!TupleTy->getArity())
         return EmptyTupleTy;

      SmallVector<llvm::Type*, 4> argTypes;
      for (const auto& cont : TupleTy->getContainedTypes())
         argTypes.push_back(getStorageType(cont));

      return llvm::StructType::get(Ctx, argTypes);
   }
   case Type::RecordTypeID: {
      if (Ty->isClass()) {
         return getStructTy(Ty)->getPointerTo();
      }
      if (Ty->isRawEnum()) {
         return getLlvmTypeImpl(
             cast<ast::EnumDecl>(Ty->getRecord())->getRawType());
      }

      return getStructTy(Ty);
   }
   case Type::ExistentialTypeID:
      return ExistentialContainerTy;
   case Type::MetaTypeID:
      return TypeInfoTy;
   case Type::TemplateParamTypeID:
   case Type::AssociatedTypeID:
      return getLlvmTypeImpl(Ty->getDesugaredType());
   default:
      llvm_unreachable("type should note be possible here");
   }
}

llvm::Type* IRGen::getStorageType(CanType Ty)
{
   auto it = TypeMap.find(Ty);
   if (it != TypeMap.end())
      return it->getSecond();

   auto llvmTy = getLlvmTypeImpl(Ty);
   TypeMap[Ty] = llvmTy;

   return llvmTy;
}

llvm::Type* IRGen::getParameterType(CanType Ty)
{
   auto llvmTy = getStorageType(Ty);
   if (NeedsStructReturn(Ty)) {
      llvmTy = llvmTy->getPointerTo();
   }

   return llvmTy;
}

llvm::Type* IRGen::getGlobalType(CanType Ty) { return getStorageType(Ty); }

void IRGen::DeclareFunction(il::Function const* F)
{
   IRGenPrettyStackTraceEntry PSE(*F);
   auto funcTy = F->getType()->asFunctionType();

   SmallVector<llvm::Type*, 8> argTypes;
   llvm::Type* retType;

   bool sret = false;
   unsigned sretIdx = 0;

   if (F->mightThrow()) {
      argTypes.push_back(Int8PtrTy->getPointerTo());
      ++sretIdx;
   }

   if (F->isLambda()) {
      auto L = cast<Lambda>(F);

      llvm::SmallVector<llvm::Type*, 4> CapturedTypes;
      for (auto& Capt : L->getCaptures()) {
         (void)Capt;
         CapturedTypes.push_back(BoxTy->getPointerTo());
      }

      llvm::StructType* Ty = llvm::StructType::get(Ctx, CapturedTypes);
      argTypes.push_back(Ty->getPointerTo());

      ++sretIdx;
   }

   QualType FnRetTy = funcTy->getReturnType();
   if (F->isAsync()) {
      retType = Int8PtrTy;
   }
   else if (F->hasStructReturn()) {
      auto sretTy = getParameterType(FnRetTy);
      sret = true;

      argTypes.push_back(sretTy);
      retType = VoidTy;
   }
   else if (FnRetTy->isEmptyTupleType()) {
      retType = VoidTy;
   }
   else {
      retType = getStorageType(FnRetTy);
   }

   SmallVector<unsigned, 4> ParamIndices;
   for (auto& Arg : F->getEntryBlock()->getArgs()) {
      QualType ParamTy = Arg.getType();
      ParamIndices.push_back(argTypes.size());

      if (Arg.getType()->isEmptyTupleType()) {
         continue;
      }
      else {
         argTypes.push_back(getParameterType(ParamTy));
      }
   }

   auto linkage = F->isGlobalInitFn()
                      ? llvm::Function::InternalLinkage
                      : (llvm::Function::LinkageTypes)F->getLinkage();

   auto llvmTy
       = llvm::FunctionType::get(retType, argTypes, funcTy->isCStyleVararg());

   auto fn = llvm::Function::Create(llvmTy, linkage, F->getName(), M);
   fn->setUnnamedAddr((llvm::Function::UnnamedAddr)F->getUnnamedAddr());
   fn->setVisibility((llvm::Function::VisibilityTypes)F->getVisibility());

   llvm::AttributeList AttrList;

   if (F->isGlobalInitFn() || F->isGlobalCtor()) {
      GlobalInitFns.emplace_back(fn, F->getPriority());
   }

   if (F->isGlobalDtor()) {
      GlobalDeinitFns.emplace_back(fn, F->getPriority());
   }

   if (DI && !F->isDeclared())
      emitFunctionDI(*F, fn);

   auto& ILGen = CI.getILGen();
   if (auto Decl = dyn_cast_or_null<CallableDecl>(ILGen.getDeclForValue(F))) {
      if (auto Attr = Decl->getAttribute<InlineAttr>()) {
         switch (Attr->getLevel()) {
         case InlineAttr::never:
            AttrList
                = AttrList.addAttribute(Ctx, llvm::AttributeList::FunctionIndex,
                                        llvm::Attribute::NoInline);

            break;
         case InlineAttr::hint:
            AttrList
                = AttrList.addAttribute(Ctx, llvm::AttributeList::FunctionIndex,
                                        llvm::Attribute::InlineHint);

            break;
         case InlineAttr::always:
            AttrList
                = AttrList.addAttribute(Ctx, llvm::AttributeList::FunctionIndex,
                                        llvm::Attribute::AlwaysInline);
            break;
         }
      }
   }

   if (F->mightThrow()) {
      AttrList
          = AttrList.addParamAttribute(Ctx, 0, llvm::Attribute::SwiftError);
   }

   if (sret) {
      AttrList = AttrList.addParamAttribute(Ctx, sretIdx,
                                            llvm::Attribute::StructRet);
   }

   fn->setAttributes(AttrList);

   auto il_arg_it = F->getEntryBlock()->arg_begin();
   auto llvm_arg_it = fn->arg_begin();

   unsigned i = 0;
   for (auto Idx : ParamIndices) {
      while (i < Idx) {
         ++i;
         ++llvm_arg_it;
      }

      assert(il_arg_it != F->getEntryBlock()->arg_end());
      assert(llvm_arg_it != fn->arg_end());

      addMappedValue(&*il_arg_it++, &*llvm_arg_it);
   }

   addMappedValue(F, fn);
}

llvm::Value* IRGen::getCurrentSRetValue()
{
   auto fn = Builder.GetInsertBlock()->getParent();
   assert(!fn->arg_empty());

   auto it = fn->arg_begin();

   // lambda environment and error value come before sret value
   while (!it->hasAttribute(llvm::Attribute::StructRet)) {
      ++it;
      assert(it != fn->arg_end() && "no sret value!");
   }

   return &*it;
}

llvm::Value* IRGen::getCurrentErrorValue()
{
   auto fn = Builder.GetInsertBlock()->getParent();
   assert(!fn->arg_empty());

   auto it = fn->arg_begin();
   assert(it->hasSwiftErrorAttr());

   return &*it;
}

void IRGen::ForwardDeclareType(ast::RecordDecl* R)
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

void IRGen::DeclareType(ast::RecordDecl* R)
{
   if (isa<ProtocolDecl>(R))
      return;

   auto Ty = StructTypeMap[R];
   if (Ty->getStructNumElements())
      return;

   SmallVector<llvm::Type*, 8> ContainedTypes;

   if (auto S = dyn_cast<StructDecl>(R)) {
      if (isa<ClassDecl>(R)) {
         ContainedTypes.push_back(Builder.getInt64Ty());   // strong refcount
         ContainedTypes.push_back(Builder.getInt64Ty());   // weak refcount
         ContainedTypes.push_back(Builder.getInt8PtrTy()); // typeinfo
      }

      for (auto F : S->getFields()) {
         auto fieldTy = getStorageType(F->getType()->getCanonicalType());
         ContainedTypes.push_back(fieldTy);
      }
   }
   else if (auto E = dyn_cast<EnumDecl>(R)) {
      ContainedTypes.push_back(getStorageType(E->getRawType()));

      auto CaseSize
          = TI.getSizeOfType(E->getType()) - TI.getSizeOfType(E->getRawType());

      auto* ArrTy = llvm::ArrayType::get(Builder.getInt8Ty(), CaseSize);
      ContainedTypes.push_back(ArrTy);
   }

   if (ContainedTypes.empty())
      ContainedTypes.push_back(Int8PtrTy);

   Ty->setBody(ContainedTypes, true);
}

void IRGen::ForwardDeclareGlobal(il::GlobalVariable const* G)
{
   llvm::Type* globalTy = getGlobalType(G->getType()->getReferencedType());

   auto GV = new llvm::GlobalVariable(
       *M, globalTy, G->isConstant() && !G->isLazilyInitialized(),
       (llvm::GlobalVariable::LinkageTypes)G->getLinkage(), nullptr,
       G->getName());

   GV->setAlignment(TI.getAllocAlignOfType(G->getType()->getReferencedType()));
   GV->setUnnamedAddr((llvm::GlobalVariable::UnnamedAddr)G->getUnnamedAddr());
   GV->setVisibility((llvm::GlobalVariable::VisibilityTypes)G->getVisibility());

   addMappedValue(G, GV);
}

void IRGen::DeclareGlobal(il::GlobalVariable const* G)
{
   auto glob = cast<llvm::GlobalVariable>(ValueMap[G]);
   if (auto Init = G->getInitializer()) {
      auto InitVal = getConstantVal(Init);

      // this can happen with ConstantEnums that can't exactly match the
      // required type
      if (glob->getValueType() != InitVal->getType()) {
         auto NewGlobal = new llvm::GlobalVariable(
             *M, InitVal->getType(), glob->isConstant(), glob->getLinkage(),
             InitVal, glob->getName());

         glob->replaceAllUsesWith(NewGlobal);
         glob->eraseFromParent();

         addMappedValue(G, NewGlobal);
      }
      else {
         glob->setInitializer(InitVal);
      }
   }
   else if (!G->isDeclared()) {
      glob->setInitializer(llvm::ConstantAggregateZero::get(
          glob->getType()->getPointerElementType()));
   }
}

void IRGen::visitFunction(Function& F)
{
   if (F.isDeclared())
      return;

   IRGenPrettyStackTraceEntry PSE((F));

   auto func = cast<llvm::Function>(ValueMap[&F]);
   assert(func && "func not declared?");

   if (DI) {
      auto Sub = DIFuncMap.find((uintptr_t)&F);
      assert(Sub != DIFuncMap.end());

      beginScope(Sub->getSecond());
      Builder.SetCurrentDebugLocation(llvm::DebugLoc());
   }

   llvm::BasicBlock* AllocaBB = nullptr;
   if (!F.isAsync()) {
      AllocaBB = llvm::BasicBlock::Create(Ctx, "alloca_block", func);
   }

   // Create PHIs for basic block arguments.
   for (auto& BB : F.getBasicBlocks()) {
      if (BB.hasNoPredecessors())
         continue;

      auto B = llvm::BasicBlock::Create(Ctx, BB.getName(), func);
      Builder.SetInsertPoint(B);

      if (!BB.isEntryBlock() && !BB.getArgs().empty()) {
         for (const auto& arg : BB.getArgs()) {
            auto ty = getParameterType(arg.getType());
            addMappedValue(&arg, Builder.CreatePHI(ty, 0));
         }
      }

      addMappedValue(&BB, B);
   }

   if (!AllocaBB) {
      AllocaBB = getBasicBlock(F.getEntryBlock());
   }

   AllocaIt = AllocaBB->end();
   this->AllocaBB = AllocaBB;

   // Allocate structs that were passed destructured.
   Builder.SetInsertPoint(AllocaBB);

   for (auto& B : F.getBasicBlocks()) {
      if (B.hasNoPredecessors())
         continue;

      visitBasicBlock(B);
   }

   for (auto& B : F.getBasicBlocks()) {
      if (!B.isEntryBlock() && !B.getArgs().empty()) {
         for (size_t i = 0; i < B.getArgs().size(); ++i) {
            auto llvmBB = getBasicBlock(&B);

            auto val = &llvmBB->getInstList().front();
            for (size_t j = 0; j < i; ++j)
               val = val->getNextNode();

            auto phi = cast<llvm::PHINode>(val);
            for (BasicBlock* Pred : getPredecessors(B)) {
               auto Term = Pred->getTerminator();
               assert(Term && "no terminator for basic block");
               llvm::Value* PassedVal;

               if (auto Br = dyn_cast<BrInst>(Term)) {
                  PassedVal = getLlvmValue(&B == Br->getTargetBranch()
                                               ? Br->getTargetArgs()[i]
                                               : Br->getElseArgs()[i]);
               }
               else if (auto Inv = dyn_cast<InvokeInst>(Term)) {
                  continue;
               }
               else if (auto Yield = dyn_cast<YieldInst>(Term)) {
                  PassedVal = getLlvmValue(Yield->getResumeArgs()[i]);
                  PassedVal = Builder.CreateBitCast(PassedVal, phi->getType());
               }
               else {
                  llvm_unreachable("bad terminator kind");
               }

               phi->addIncoming(PassedVal, getBasicBlock(Pred->getName()));
            }

            if (!phi->getNumIncomingValues()) {
               assert(B.hasNoPredecessors()
                      && "branch didn't provide value "
                         "for block argument!");
               llvmBB->eraseFromParent();
            }
         }
      }
   }

   Builder.SetCurrentDebugLocation(llvm::DebugLoc());

   if (!F.isAsync()) {
      Builder.SetInsertPoint(AllocaBB);
      Builder.CreateBr(getBasicBlock(F.getEntryBlock()));
   }

   if (DI) {
      emitArgumentDI(F, func);
      endScope();
   }

   AllocaIt = llvm::BasicBlock::iterator();
   this->AllocaBB = nullptr;
}

llvm::GlobalVariable* IRGen::getOrCreateInitializedFlag(const il::Value* ForVal)
{
   auto It = InitializedFlagMap.find(ForVal);
   if (It != InitializedFlagMap.end())
      return It->getSecond();

   auto Flag = new llvm::GlobalVariable(*M, Builder.getInt1Ty(), false,
                                        llvm::GlobalVariable::PrivateLinkage,
                                        Builder.getFalse());

   InitializedFlagMap[ForVal] = Flag;
   return Flag;
}

llvm::Value* IRGen::visitDebugLocInst(const DebugLocInst& I)
{
   if (DI) {
      Builder.SetCurrentDebugLocation(
          llvm::DebugLoc::get(I.getLine(), I.getCol(), ScopeStack.top()));
   }

   return nullptr;
}

llvm::Value* IRGen::visitDebugLocalInst(const DebugLocalInst& I)
{
   if (DI)
      emitLocalVarDI(I);

   return nullptr;
}

void IRGen::visitBasicBlock(BasicBlock& B)
{
   Builder.SetInsertPoint(getBasicBlock(&B));

   for (auto const& I : B.getInstructions()) {
      auto val = visit(I);
      if (I.getType()->isEmptyTupleType()) {
         addMappedValue(&I, EmptyTuple);
         continue;
      }

      addMappedValue(&I, val);
      if (dyn_cast_or_null<llvm::TerminatorInst>(val))
         break;
   }

   assert(Builder.GetInsertBlock()->getTerminator()
          && "basic block not terminated!");
}

llvm::BasicBlock* IRGen::getBasicBlock(llvm::StringRef name)
{
   assert(Builder.GetInsertBlock());
   auto func = Builder.GetInsertBlock()->getParent();

   for (auto& BB : func->getBasicBlockList())
      if (BB.getName() == name)
         return &BB;

   return nullptr;
}

llvm::BasicBlock* IRGen::getBasicBlock(BasicBlock* BB)
{
   return cast<llvm::BasicBlock>(ValueMap[BB]);
}

llvm::Function* IRGen::getFunction(il::Function* F)
{
   return cast<llvm::Function>(ValueMap[F]);
}

llvm::ConstantInt* IRGen::wordSizedInt(uint64_t val)
{
   return llvm::ConstantInt::get(WordTy, val);
}

llvm::Value* IRGen::toInt8Ptr(llvm::Value* V)
{
   return Builder.CreateBitCast(V, Int8PtrTy);
}

llvm::Constant* IRGen::toInt8Ptr(llvm::Constant* V)
{
   return llvm::ConstantExpr::getBitCast(V, Int8PtrTy);
}

llvm::Value* IRGen::CreateSizeOf(llvm::Type* Ty)
{
   return Builder.CreatePtrToInt(
       Builder.CreateInBoundsGEP(
           llvm::ConstantPointerNull::get(Ty->getPointerTo()),
           {Builder.getInt32(1)}),
       Builder.getInt64Ty());
}

llvm::Value* IRGen::CreateCall(il::Function* F,
                               llvm::SmallVector<llvm::Value*, 8>& args)
{
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
                                      unsigned int alignment)
{
   auto IP = Builder.saveIP();
   auto Loc = Builder.getCurrentDebugLocation();
   Builder.SetCurrentDebugLocation(nullptr);

   Builder.SetInsertPoint(AllocaBB, AllocaIt);

   if (!alignment)
      alignment = TI.getAlignOfType(AllocatedType);

   auto A
       = CreateAlloca(getStorageType(AllocatedType), allocatedSize, alignment);

   Builder.restoreIP(IP);
   Builder.SetCurrentDebugLocation(Loc);

   AllocaIt = A->getIterator();
   return A;
}

llvm::AllocaInst* IRGen::CreateAlloca(QualType AllocatedType,
                                      il::Value* allocatedSize,
                                      unsigned int alignment)
{
   auto IP = Builder.saveIP();

   auto Loc = Builder.getCurrentDebugLocation();
   Builder.SetCurrentDebugLocation(nullptr);

   if (!allocatedSize || isa<Constant>(allocatedSize))
      Builder.SetInsertPoint(AllocaBB, AllocaIt);

   if (!alignment)
      alignment = TI.getAlignOfType(AllocatedType);

   auto A
       = CreateAlloca(getStorageType(AllocatedType), allocatedSize, alignment);

   Builder.restoreIP(IP);
   Builder.SetCurrentDebugLocation(Loc);

   if (!allocatedSize || isa<Constant>(allocatedSize))
      AllocaIt = A->getIterator();

   return A;
}

llvm::AllocaInst* IRGen::CreateAlloca(llvm::Type* AllocatedType,
                                      il::Value* allocatedSize,
                                      unsigned int alignment)
{
   auto IP = Builder.saveIP();

   auto Loc = Builder.getCurrentDebugLocation();
   Builder.SetCurrentDebugLocation(nullptr);

   if (!allocatedSize || isa<Constant>(allocatedSize))
      Builder.SetInsertPoint(AllocaBB, AllocaIt);

   llvm::AllocaInst* alloca;
   if (allocatedSize != nullptr)
      alloca = Builder.CreateAlloca(AllocatedType, getLlvmValue(allocatedSize));
   else
      alloca = Builder.CreateAlloca(AllocatedType);

   alloca->setAlignment(alignment);

   Builder.restoreIP(IP);
   Builder.SetCurrentDebugLocation(Loc);

   if (!allocatedSize || isa<Constant>(allocatedSize))
      AllocaIt = alloca->getIterator();

   return alloca;
}

llvm::AllocaInst* IRGen::CreateAlloca(llvm::Type* AllocatedType,
                                      size_t allocatedSize, unsigned alignment)
{
   auto IP = Builder.saveIP();

   auto Loc = Builder.getCurrentDebugLocation();
   Builder.SetCurrentDebugLocation(nullptr);

   Builder.SetInsertPoint(AllocaBB, AllocaIt);

   llvm::AllocaInst* alloca;
   if (allocatedSize > 1)
      alloca = Builder.CreateAlloca(AllocatedType, wordSizedInt(allocatedSize));
   else
      alloca = Builder.CreateAlloca(AllocatedType);

   alloca->setAlignment(alignment);

   Builder.restoreIP(IP);
   Builder.SetCurrentDebugLocation(Loc);

   AllocaIt = alloca->getIterator();
   return alloca;
}

llvm::Value* IRGen::CreateCopy(il::Value* Val)
{
   return CreateCopy(Val->getType(), getLlvmValue(Val));
}

llvm::Value* IRGen::CreateCopy(QualType Ty, llvm::Value* Val)
{
   llvm_unreachable("TODO!");
}

unsigned IRGen::getFieldOffset(StructDecl* S, const DeclarationName& FieldName)
{
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

QualType IRGen::getFieldType(ast::StructDecl* S,
                             const DeclarationName& FieldName)
{
   for (auto F : S->getFields()) {
      if (F->getDeclName() == FieldName)
         return F->getType();
   }

   llvm_unreachable("bad field name");
}

llvm::Value* IRGen::AccessField(ast::StructDecl* S, Value* Val,
                                const DeclarationName& FieldName, bool load)
{
   // Ignore a previous load of the value.
   if (auto* ld = dyn_cast<LoadInst>(Val)) {
      Val = ld->getTarget();
   }

   auto offset = getFieldOffset(S, FieldName);
   auto* StructVal = getLlvmValue(Val);

   if (StructVal->getType()->isStructTy()) {
      auto* Val = Builder.CreateExtractValue(StructVal, offset);
      auto* Alloc = Builder.CreateAlloca(Val->getType());
      Builder.CreateStore(Val, Alloc);

      return Alloc;
   }

   auto Ptr = Builder.CreateStructGEP(getStructTy(S), StructVal, offset);
   return load ? (llvm::Value*)Builder.CreateLoad(Ptr) : Ptr;
}

llvm::Value* IRGen::getLlvmValue(il::Value const* V)
{
   auto val = getPotentiallyBoxedValue(V);
   //   if (isa<AllocBoxInst>(V)) {
   //      val = unboxValue(val, V->getType());
   //   }

   return val;
}

llvm::Value* IRGen::getPotentiallyBoxedValue(const il::Value* V)
{
   if (auto C = dyn_cast<Constant>(V))
      return getConstantVal(C);

   auto it = ValueMap.find(V);
   assert(it != ValueMap.end());

   return it->second;
}

llvm::Value* IRGen::unboxValue(llvm::Value* V, QualType Ty)
{
   auto Unboxed = Builder.CreateStructGEP(
       BoxTy, Builder.CreateBitCast(V, BoxTy->getPointerTo()), BoxType::ObjPtr);

   return Builder.CreateBitCast(Unboxed, getStorageType(Ty)->getPointerTo());
}

void IRGen::buildConstantClass(llvm::SmallVectorImpl<llvm::Constant*>& Vec,
                               const ConstantClass* Class)
{
   if (auto Base = Class->getBase())
      buildConstantClass(Vec, Base);

   for (const auto& el : Class->getElements())
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

      addMappedValue(Int, V);
      return V;
   }

   if (auto F = dyn_cast<ConstantFloat>(C)) {
      if (F->getType()->isFloatTy())
         return llvm::ConstantFP::get(Builder.getFloatTy(), F->getFloatVal());

      auto it = ValueMap.find(F);
      if (it != ValueMap.end())
         return cast<llvm::ConstantFP>(it->getSecond());

      auto V = llvm::ConstantFP::get(Builder.getDoubleTy(), F->getDoubleVal());
      addMappedValue(F, V);

      return V;
   }

   if (auto S = dyn_cast<ConstantString>(C)) {
      auto it = ValueMap.find(S);
      if (it != ValueMap.end()) {
         auto* BC = cast<llvm::ConstantExpr>(it->getSecond());
         if (cast<llvm::GlobalVariable>(BC->getOperand(0))->getParent() == M)
            return BC;
      }

      auto StrConstant = llvm::ConstantDataArray::getString(Ctx, S->getValue());
      auto GV = new llvm::GlobalVariable(*M, StrConstant->getType(), true,
                                         llvm::GlobalValue::PrivateLinkage,
                                         StrConstant);

      GV->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);

      auto V = llvm::ConstantExpr::getBitCast(GV, Int8PtrTy);
      addMappedValue(S, V);

      return V;
   }

   if (auto A = dyn_cast<ConstantArray>(C)) {
      auto it = ValueMap.find(A);
      if (it != ValueMap.end())
         return cast<llvm::Constant>(it->getSecond());

      llvm::Constant* V;
      if (A->isAllZerosValue()) {
         V = llvm::ConstantAggregateZero::get(getStorageType(A->getType()));
      }
      else {
         bool NeedStructTy = false;
         llvm::Type* ElementTy = getStorageType(A->getElementType());

         SmallVector<llvm::Constant*, 8> Els;
         for (const auto& el : A->getVec()) {
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

         if (NeedStructTy) {
            V = llvm::ConstantStruct::getAnon(Els, true);
         }
         else {
            auto ArrTy = llvm::ArrayType::get(ElementTy, Els.size());
            V = llvm::ConstantArray::get(ArrTy, Els);
         }
      }

      addMappedValue(A, V);
      return V;
   }

   if (auto S = dyn_cast<ConstantStruct>(C)) {
      auto it = ValueMap.find(S);
      if (it != ValueMap.end())
         return cast<llvm::Constant>(it->getSecond());

      llvm::Constant* V;
      if (S->isAllZerosValue()) {
         V = llvm::ConstantAggregateZero::get(getStorageType(S->getType()));
      }
      else {
         bool NeedsCustomType = false;
         SmallVector<llvm::Constant*, 8> Vals;
         for (const auto& el : S->getElements()) {
            if (el->containsConstantEnum()) {
               NeedsCustomType = true;
            }

            Vals.push_back(getConstantVal(el));
         }

         if (NeedsCustomType) {
            V = llvm::ConstantStruct::getAnon(Vals, true);
         }
         else {
            V = llvm::ConstantStruct::get(
                cast<llvm::StructType>(getStorageType(S->getType())), Vals);
         }
      }

      addMappedValue(S, V);
      return V;
   }

   if (auto Tup = dyn_cast<ConstantTuple>(C)) {
      auto it = ValueMap.find(Tup);
      if (it != ValueMap.end())
         return cast<llvm::Constant>(it->getSecond());

      llvm::Constant* V;
      if (Tup->isAllZerosValue()) {
         V = llvm::ConstantAggregateZero::get(getStorageType(Tup->getType()));
      }
      else {
         SmallVector<llvm::Constant*, 8> Vals;
         for (const auto& el : Tup->getVec()) {
            Vals.push_back(getConstantVal(el));
         }

         V = llvm::ConstantStruct::getAnon(Ctx, Vals);
      }

      addMappedValue(Tup, V);
      return V;
   }

   if (auto Class = dyn_cast<ConstantClass>(C)) {
      auto it = ValueMap.find(Class);
      if (it != ValueMap.end()) {
         auto* GV = cast<llvm::GlobalVariable>(it->getSecond());
         if (GV->getParent() == M)
            return llvm::ConstantExpr::getBitCast(
                GV, getParameterType(C->getType()));
      }

      auto* TI = Class->getTypeInfo();

      // FIXME ConstantClass should not be bound to a module!
      if (TI->getParent() != ILMod) {
         TI = TI->getDeclarationIn(ILMod);

         ForwardDeclareGlobal(TI);
         DeclareGlobal(TI);
      }

      llvm::SmallVector<llvm::Constant*, 8> Vals{
          WordOne,                      // strong refcount
          WordZero,                     // weak refcount
          toInt8Ptr(getConstantVal(TI)) // typeinfo
      };

      buildConstantClass(Vals, Class);

      auto V = llvm::ConstantStruct::getAnon(Ctx, Vals);
      auto GV = new llvm::GlobalVariable(
          *M, V->getType(), true, llvm::GlobalVariable::InternalLinkage, V);

      addMappedValue(Class, GV);
      return llvm::ConstantExpr::getBitCast(GV, getParameterType(C->getType()));
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

      addMappedValue(U, GV);
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
      for (auto& Val : E->getCaseValues()) {
         CaseSize += TI.getSizeOfType(Val->getType());
         CaseVals.push_back(getConstantVal(Val));
      }

      // add padding if necessary
      if (CaseSize < TypeSize) {
         auto PaddingTy
             = llvm::ArrayType::get(Builder.getInt8Ty(), TypeSize - CaseSize);
         CaseVals.push_back(llvm::ConstantAggregateZero::get(PaddingTy));
      }

      llvm::Constant* Vals = llvm::ConstantStruct::getAnon(Ctx, CaseVals);
      if (E->getCase()->isIndirect()) {
         Vals = new llvm::GlobalVariable(*M, Vals->getType(), true,
                                         llvm::GlobalVariable::InternalLinkage,
                                         Vals);
      }

      llvm::Constant* Val = llvm::ConstantStruct::getAnon(
          Ctx, {getConstantVal(E->getCase()->getILValue()), Vals}, true);

      addMappedValue(E, Val);
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
      return llvm::UndefValue::get(getParameterType(Undef->getType()));
   }

   if (auto G = dyn_cast<GlobalVariable>(C)) {
      auto* Ty = getParameterType(G->getType());
      if (!Ty->isPointerTy()) {
         Ty = Ty->getPointerTo();
      }

      return llvm::ConstantExpr::getBitCast(
          cast<llvm::GlobalVariable>(ValueMap[G]), Ty);
   }

   if (auto Magic = dyn_cast<MagicConstant>(C)) {
      switch (Magic->getMagicConstantKind()) {
      case MagicConstant::__ctfe:
         return llvm::ConstantInt::getFalse(Ctx);
      }
   }

   if (isa<ConstantTokenNone>(C))
      return llvm::ConstantTokenNone::get(Ctx);

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
      case OPC::And:
         return llvm::ConstantExpr::getAdd(lhs, rhs);
      case OPC::Or:
         return llvm::ConstantExpr::getOr(lhs, rhs);
      case OPC::Xor:
         return llvm::ConstantExpr::getXor(lhs, rhs);
      case OPC::LShr:
         return llvm::ConstantExpr::getLShr(lhs, rhs);
      case OPC::AShr:
         return llvm::ConstantExpr::getAShr(lhs, rhs);
      case OPC::Shl:
         return llvm::ConstantExpr::getShl(lhs, rhs);
#define INT_OR_FP_OP(Name)                                                     \
   case OPC::Name:                                                             \
      if (ty->isIntegerType()) {                                               \
         if (ty->isUnsigned())                                                 \
            return llvm::ConstantExpr::getNUW##Name(lhs, rhs);                 \
                                                                               \
         return llvm::ConstantExpr::getNSW##Name(lhs, rhs);                    \
      }                                                                        \
                                                                               \
      return llvm::ConstantExpr::getF##Name(lhs, rhs);
         INT_OR_FP_OP(Add)
         INT_OR_FP_OP(Sub)
         INT_OR_FP_OP(Mul)

#undef INT_OR_FP_OP
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

      if (GEP->getTarget()->getType()->removeReference()->isRecordType()) {
         auto R = GEP->getTarget()->getType()->removeReference()->getRecord();

         unsigned idx = cast<ConstantInt>(GEP->getIdx())->getU32();
         if (isa<ClassDecl>(R))
            idx += 3;

         return llvm::ConstantExpr::getInBoundsGetElementPtr(
             val->getType(), val,
             llvm::ArrayRef<llvm::Constant*>{WordZero,
                                             getConstantVal(GEP->getIdx())});
      }
      if (val->getType()->getPointerElementType()->isArrayTy()) {
         return llvm::ConstantExpr::getInBoundsGetElementPtr(
             val->getType(), val,
             llvm::ArrayRef<llvm::Constant*>{WordZero,
                                             getConstantVal(GEP->getIdx())});
      }

      return llvm::ConstantExpr::getInBoundsGetElementPtr(
          val->getType(), val,
          llvm::ArrayRef<llvm::Constant*>{getConstantVal(GEP->getIdx())});
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

llvm::Value* IRGen::visitAllocaInst(AllocaInst const& I)
{
   if (I.isTagged()) {
      (void)getOrCreateInitializedFlag(&I);
   }

   llvm::Value* V = nullptr;
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

      llvm::Type* allocatedType
          = getStorageType(I.getType()->getReferencedType());

      if (I.isHeapAlloca()) {
         auto TypeSize
             = TI.getAllocSizeOfType(I.getType()->getReferencedType());

         llvm::Value* size = wordSizedInt(TypeSize);
         if (I.getAllocSize()) {
            size = Builder.CreateMul(size, getLlvmValue(I.getAllocSize()));
         }

         auto buff = Builder.CreateCall(getMallocFn(), {size});
         V = Builder.CreateBitCast(buff, allocatedType->getPointerTo());
      }
      else {
         V = CreateAlloca(allocatedType, I.getAllocSize(), alignment);
      }
   }

   return V;
}

llvm::Value* IRGen::visitAllocBoxInst(const il::AllocBoxInst& I)
{
   QualType Ty = I.getType()->getReferencedType();

   auto TypeSize = TI.getSizeOfType(I.getType()->getReferencedType());
   auto size = wordSizedInt(TypeSize + 3 * TI.getPointerSizeInBytes());

   llvm::Value* Mem = Builder.CreateCall(getMallocFn(), {size});
   Mem = Builder.CreateBitCast(Mem, BoxTy->getPointerTo());

   auto strongRefcountGEP = Builder.CreateStructGEP(BoxTy, Mem, 0);
   Builder.CreateStore(wordSizedInt(1), strongRefcountGEP);

   auto weakRefcountGEP = Builder.CreateStructGEP(BoxTy, Mem, 1);
   Builder.CreateStore(wordSizedInt(0), weakRefcountGEP);

   auto deinitGEP = Builder.CreateStructGEP(BoxTy, Mem, 2);

   llvm::Constant* Deinit = nullptr;
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

llvm::Value* IRGen::visitAssignInst(const AssignInst&)
{
   llvm_unreachable("didn't replace assign with store or init!");
}

static unsigned short getMemCpyAlign(QualType Ty) { return 1; }

llvm::Value* IRGen::visitStoreInst(StoreInst const& I)
{
   auto Dst = getLlvmValue(I.getDst());
   auto Src = getLlvmValue(I.getSrc());

   if (I.getDst()->isTagged()) {
      Builder.CreateStore(Builder.getTrue(),
                          getOrCreateInitializedFlag(I.getDst()));
   }

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
         assert(ElidedDebugLocalInst != nullptr && "no debug local inst!");

         emitLocalVarDI(*ElidedDebugLocalInst, Src);
         ElidedDebugLocalInst = nullptr;
      }

      addMappedValue(I.getDst(), Src);
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

   if (DI)
      emitDebugValue(I.getDst(), Src);

   if (NeedsStructReturn(Ty)) {
      return Builder.CreateMemCpy(Dst, Src, TI.getAllocSizeOfType(Ty),
                                  getMemCpyAlign(Ty));
   }

   auto Store = Builder.CreateStore(Src, Dst);
   Store->setAlignment(alignment);

   if (I.getMemoryOrder() != MemoryOrder::NotAtomic) {
      Store->setAtomic((llvm::AtomicOrdering)I.getMemoryOrder());
   }

   return Store;
}

llvm::Value* IRGen::visitInitInst(const InitInst& I)
{
   auto Dst = getLlvmValue(I.getDst());
   auto Src = getLlvmValue(I.getSrc());

   if (I.getDst()->isTagged()) {
      Builder.CreateStore(Builder.getTrue(),
                          getOrCreateInitializedFlag(I.getDst()));
   }

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

      addMappedValue(I.getDst(), Src);
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

   if (DI)
      emitDebugValue(I.getDst(), Src);

   if (NeedsStructReturn(Ty)) {
      return Builder.CreateMemCpy(Dst, Src, TI.getAllocSizeOfType(Ty),
                                  getMemCpyAlign(Ty));
   }

   auto Store = Builder.CreateStore(Src, Dst);
   Store->setAlignment(alignment);

   if (I.getMemoryOrder() != MemoryOrder::NotAtomic) {
      Store->setAtomic((llvm::AtomicOrdering)I.getMemoryOrder());
   }

   return Store;
}

llvm::Value* IRGen::visitStrongRetainInst(const il::StrongRetainInst& I)
{
   auto Val = toInt8Ptr(getPotentiallyBoxedValue(I.getOperand(0)));
   if (I.getOperand(0)->getType()->isBoxType()) {
      return Builder.CreateCall(getRetainBoxFn(), {Val});
   }

   if (I.getOperand(0)->getType()->isLambdaType()) {
      return Builder.CreateCall(getRetainLambdaFn(), {Val});
   }

   return Builder.CreateCall(getRetainFn(), {Val});
}

llvm::Value* IRGen::visitStrongReleaseInst(const il::StrongReleaseInst& I)
{
   auto Val = toInt8Ptr(getPotentiallyBoxedValue(I.getOperand(0)));
   if (I.getOperand(0)->getType()->isBoxType()) {
      return Builder.CreateCall(getReleaseBoxFn(), {Val});
   }

   if (I.getOperand(0)->getType()->isLambdaType()) {
      return Builder.CreateCall(getReleaseLambdaFn(), {Val});
   }

   return Builder.CreateCall(getReleaseFn(), {Val});
}

llvm::Value* IRGen::visitWeakRetainInst(const il::WeakRetainInst& I)
{
   llvm_unreachable("unimplemented");
}

llvm::Value* IRGen::visitWeakReleaseInst(const il::WeakReleaseInst& I)
{
   llvm_unreachable("unimplemented");
}

llvm::Value* IRGen::visitMoveInst(const il::MoveInst& I)
{
   // update the flag indicating this value was moved from
   llvm::GlobalVariable* Flag = getOrCreateInitializedFlag(I.getOperand(0));
   Builder.CreateStore(Builder.getFalse(), Flag);

   auto It = InitializedFlagMap.find(&I);
   if (It != InitializedFlagMap.end()) {
      Builder.CreateStore(Builder.getTrue(), It->getSecond());
   }

   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitBeginBorrowInst(const il::BeginBorrowInst& I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitEndBorrowInst(const il::EndBorrowInst& I)
{
   return nullptr;
}

llvm::Value* IRGen::visitGEPInst(GEPInst const& I)
{
   auto val = getLlvmValue(I.getVal());

   if (I.getVal()->getType()->removeReference()->isRecordType()) {
      // Ignore a previous load of the value.
      if (auto* ld = dyn_cast<llvm::LoadInst>(val)) {
         val = ld->getOperand(0);
      }

      auto R = I.getVal()->getType()->removeReference()->getRecord();
      assert(isa<ConstantInt>(I.getIndex()));

      unsigned idx = cast<ConstantInt>(I.getIndex())->getU32();
      if (isa<ClassDecl>(R)) {
         idx += 3;
      }

      if (val->getType()->isStructTy()) {
         auto* Val = Builder.CreateExtractValue(val, idx);
         auto* Alloc = Builder.CreateAlloca(Val->getType());
         Builder.CreateStore(Val, Alloc);

         return Alloc;
      }

      return Builder.CreateStructGEP(getStructTy(R), val, idx);
   }

   if (val->getType()->getPointerElementType()->isArrayTy()) {
      // Ignore a previous load of the value.
      if (auto* ld = dyn_cast<llvm::LoadInst>(val)) {
         val = ld->getOperand(0);
      }

      if (val->getType()->isArrayTy()) {
         auto* Val
             = Builder.CreateExtractElement(val, getLlvmValue(I.getIndex()));
         auto* Alloc = Builder.CreateAlloca(Val->getType());
         Builder.CreateStore(Val, Alloc);

         return Alloc;
      }

      return Builder.CreateInBoundsGEP(
          val, {wordSizedInt(0), getLlvmValue(I.getIndex())});
   }

   return Builder.CreateInBoundsGEP(val, getLlvmValue(I.getIndex()));
}

llvm::Value* IRGen::visitCaptureExtractInst(const CaptureExtractInst& I)
{
   auto F = Builder.GetInsertBlock()->getParent();
   auto env = F->arg_begin();

   auto idx = I.getIdx()->getU32();
   auto Val = Builder.CreateStructGEP(env->getType()->getPointerElementType(),
                                      env, idx);

   return Builder.CreateLoad(Val);
}

llvm::Value* IRGen::visitFieldRefInst(FieldRefInst const& I)
{
   return AccessField(I.getAccessedType(), I.getOperand(0), I.getFieldName());
}

llvm::Value* IRGen::visitTupleExtractInst(TupleExtractInst const& I)
{
   auto tup = getLlvmValue(I.getOperand(0));
   if (tup->getType()->isStructTy()) {
      auto* Val = Builder.CreateExtractValue(tup, I.getIdx()->getU32());
      auto* Alloc = Builder.CreateAlloca(Val->getType());
      Builder.CreateStore(Val, Alloc);

      return Alloc;
   }

   return Builder.CreateStructGEP(tup->getType()->getPointerElementType(), tup,
                                  I.getIdx()->getU32());
}

llvm::Value* IRGen::visitEnumExtractInst(const EnumExtractInst& I)
{
   auto Case = I.getCase();
   auto E = getLlvmValue(I.getOperand(0));

   // Ignore a previous load of the value.
   if (auto* ld = dyn_cast<llvm::LoadInst>(E)) {
      E = ld->getOperand(0);
   }

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
   if (E->getType()->isIntegerTy()) {
      return E;
   }

   if (E->getType()->isStructTy()) {
      return Builder.CreateExtractValue(E, 0);
   }

   // Ignore a previous load of the value.
   if (auto* ld = dyn_cast<llvm::LoadInst>(E)) {
      E = ld->getOperand(0);
   }

   auto Val
       = Builder.CreateStructGEP(E->getType()->getPointerElementType(), E, 0);

   if (!I.getType()->isReferenceType()) {
      // Handle builtin integer raw types
      auto *Ld = Builder.CreateLoad(Val);
      if (Ld->getType()->isStructTy()) {
         return Builder.CreateExtractValue(Ld, 0);
      }

      return Ld;
   }

   return Val;
}

bool shouldReallyLoad(SemaPass& Sema, const LoadInst& I)
{
   if (Sema.NeedsStructReturn(I.getType())) {
      return false;
   }

   return true;
   //
   //   auto *singleUser = I.getSingleUser();
   //   if (!singleUser) {
   //      return true;
   //   }
   //
   //   if (isa<FieldRefInst>(singleUser)) {
   //      return false;
   //   }
   //   if (auto *GEP = dyn_cast<GEPInst>(singleUser)) {
   //      return !GEP->getVal()->getType()->isRecordType();
   //   }
   //
   //   if (auto *call = dyn_cast<IntrinsicCallInst>(singleUser)) {
   //      switch (call->getCalledIntrinsic()) {
   //      case Intrinsic::memcpy:
   //      case Intrinsic::memset:
   //      case Intrinsic::memcmp:
   //      case Intrinsic::typeinfo_cmp:
   //         return false;
   //      default:
   //         return true;
   //      }
   //   }
   //
   //   return true;
}

llvm::Value* IRGen::visitLoadInst(LoadInst const& I)
{
   auto val = getLlvmValue(I.getTarget());
   if (!shouldReallyLoad(Sema, I)) {
      return val;
   }

   unsigned alignment = 0;
   if (auto Alloca = dyn_cast<llvm::AllocaInst>(val)) {
      alignment = Alloca->getAlignment();
   }
   else {
      alignment
          = TI.getAlignOfType(I.getTarget()->getType()->getReferencedType());
   }

   auto Ld = Builder.CreateLoad(val);
   Ld->setAlignment(alignment);

   if (I.getMemoryOrder() != MemoryOrder::NotAtomic) {
      Ld->setAtomic((llvm::AtomicOrdering)I.getMemoryOrder());
   }

   return Ld;
}

llvm::Value* IRGen::visitAddrOfInst(AddrOfInst const& I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitPtrToLvalueInst(const PtrToLvalueInst& I)
{
   return getLlvmValue(I.getOperand(0));
}

llvm::Value* IRGen::visitRetInst(RetInst const& I)
{
   if (I.canUseSRetValue()) {
      return Builder.CreateRetVoid();
   }

   if (auto V = I.getReturnedValue()) {
      auto retVal = getLlvmValue(V);
      if (retVal->getType()->isVoidTy() || V->getType()->isEmptyTupleType()) {
         return Builder.CreateRetVoid();
      }

      auto func = I.getParent()->getParent();
      if (func->hasStructReturn()) {
         auto argIt = Builder.GetInsertBlock()->getParent()->arg_begin();
         if (I.getParent()->getParent()->isLambda())
            ++argIt;

         auto sretVal = &*argIt;
         Builder.CreateMemCpy(sretVal, retVal, TI.getSizeOfType(V->getType()),
                              getMemCpyAlign(V->getType()));
      }
      else {
         return Builder.CreateRet(retVal);
      }
   }

   return Builder.CreateRetVoid();
}

llvm::Value* IRGen::visitYieldInst(const il::YieldInst& I)
{
   auto* CoroPromise
       = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::coro_promise);

   auto* CurrentFn = Builder.GetInsertBlock()->getParent();

   auto* Hdl = CoroHandleMap[CurrentFn];
   auto* Align = Builder.getInt32(
       TI.getAllocAlignOfType(I.getYieldedValue()->getType()));

   auto* Promise
       = Builder.CreateCall(CoroPromise, {Hdl, Align, Builder.getFalse()});

   Builder.CreateMemCpy(Promise, getLlvmValue(I.getYieldedValue()),
                        TI.getAllocSizeOfType(I.getYieldedValue()->getType()),
                        (unsigned)Align->getZExtValue());

   auto* CoroSuspend
       = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::coro_suspend);

   auto* Final = I.isFinalYield() ? Builder.getTrue() : Builder.getFalse();
   auto Next = Builder.CreateCall(CoroSuspend,
                                  {llvm::ConstantTokenNone::get(Ctx), Final});

   auto* Switch = Builder.CreateSwitch(Next, CoroSuspendMap[CurrentFn]);

   auto* Zero
       = cast<llvm::ConstantInt>(llvm::ConstantInt::get(Next->getType(), 0));
   auto* One
       = cast<llvm::ConstantInt>(llvm::ConstantInt::get(Next->getType(), 1));

   llvm::BasicBlock* ResumeDst;
   if (I.isFinalYield()) {
      ResumeDst = llvm::BasicBlock::Create(Ctx, "coro.unreachable");

      auto IP = Builder.saveIP();

      Builder.SetInsertPoint(ResumeDst);
      Builder.CreateUnreachable();

      Builder.restoreIP(IP);
   }
   else {
      ResumeDst = getBasicBlock(I.getResumeDst());
   }

   Switch->addCase(Zero, ResumeDst);
   Switch->addCase(One, CoroCleanupMap[CurrentFn]);

   return nullptr;
}

llvm::Value* IRGen::visitThrowInst(ThrowInst const& I)
{
   QualType ThrownTy = I.getThrownValue()->getType();
   unsigned TypeSize = TI.getAllocSizeOfType(ThrownTy);

   auto thrownVal = getLlvmValue(I.getThrownValue());
   auto ErrVal = getCurrentErrorValue();

   auto AllocSize = wordSizedInt(TypeSize + 2 * TI.getPointerSizeInBytes());
   llvm::Value* Alloc = Builder.CreateCall(getMallocFn(), AllocSize);
   Alloc = Builder.CreateBitCast(Alloc, ErrorTy->getPointerTo());

   auto* TypeInfoPtr = Builder.CreateStructGEP(ErrorTy, Alloc, 0);
   Builder.CreateStore(toInt8Ptr(getLlvmValue(I.getTypeInfo())), TypeInfoPtr);

   auto* CleanupFnPtr = Builder.CreateStructGEP(ErrorTy, Alloc, 1);
   if (auto Fn = I.getCleanupFn()) {
      Builder.CreateStore(toInt8Ptr(getLlvmValue(Fn)), CleanupFnPtr);
   }
   else {
      Builder.CreateStore(llvm::ConstantPointerNull::get(Int8PtrTy),
                          CleanupFnPtr);
   }

   auto* ObjPtr = Builder.CreateStructGEP(ErrorTy, Alloc, 2);
   if (NeedsStructReturn(ThrownTy)) {
      Builder.CreateMemCpy(ObjPtr, thrownVal, TypeSize,
                           getMemCpyAlign(ThrownTy));
   }
   else {
      Builder.CreateStore(
          thrownVal,
          Builder.CreateBitCast(ObjPtr, thrownVal->getType()->getPointerTo()));
   }

   Builder.CreateStore(toInt8Ptr(Alloc), ErrVal);

   auto fn = Builder.GetInsertBlock()->getParent();
   if (fn->getReturnType()->isVoidTy())
      return Builder.CreateRetVoid();

   return Builder.CreateRet(llvm::UndefValue::get(fn->getReturnType()));
}

llvm::Value* IRGen::visitRethrowInst(const il::RethrowInst& I)
{
   auto thrownVal = getLlvmValue(I.getThrownValue());
   auto ErrVal = getCurrentErrorValue();

   Builder.CreateStore(thrownVal, ErrVal);

   auto fn = Builder.GetInsertBlock()->getParent();
   if (fn->getReturnType()->isVoidTy())
      return Builder.CreateRetVoid();

   return Builder.CreateRet(llvm::UndefValue::get(fn->getReturnType()));
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
   llvm::BasicBlock* DefaultBB = nullptr;
   if (auto BB = I.getDefault()) {
      DefaultBB = getBasicBlock(BB);
   }
   else {
      auto IP = Builder.saveIP();
      auto unreachableBB = llvm::BasicBlock::Create(
          Ctx, "", Builder.GetInsertBlock()->getParent());

      Builder.SetInsertPoint(unreachableBB);
      Builder.CreateUnreachable();

      Builder.restoreIP(IP);
      DefaultBB = unreachableBB;
   }

   auto SwitchInst
       = Builder.CreateSwitch(getLlvmValue(I.getSwitchVal()), DefaultBB,
                              unsigned(I.getCases().size()));

   for (const auto& C : I.getCases()) {
      if (!C.first)
         continue;

      SwitchInst->addCase(cast<llvm::ConstantInt>(getLlvmValue(C.first)),
                          getBasicBlock(C.second));
   }

   return SwitchInst;
}

llvm::Value* IRGen::visitInvokeInst(InvokeInst const& I)
{
   auto ErrAlloc = Builder.CreateAlloca(Int8PtrTy);
   ErrAlloc->setSwiftError(true);

   Builder.CreateStore(llvm::ConstantPointerNull::get(Int8PtrTy), ErrAlloc);

   llvm::SmallVector<llvm::Value*, 8> args{ErrAlloc};
   PrepareCallArgs(args, I.getArgs(), isa<Method>(I.getCalledFunction()));

   auto* Call = Builder.CreateCall(getFunction(I.getCalledFunction()), args);
   Call->addParamAttr(0, llvm::Attribute::SwiftError);

   llvm::Value* RetVal;
   if (I.getCalledFunction()->hasStructReturn()) {
      RetVal = Call->getArgOperand(1);
   }
   else {
      RetVal = Call;
   }

   auto ErrLd = Builder.CreateLoad(ErrAlloc);
   auto IsNull = Builder.CreateIsNull(ErrLd);

   llvm::BasicBlock* NormalBB = getBasicBlock(I.getNormalContinuation());
   llvm::BasicBlock* LPadBB = getBasicBlock(I.getLandingPad());

   if (!RetVal->getType()->isVoidTy()) {
      auto* ResultPhi = cast<llvm::PHINode>(&NormalBB->getInstList().front());
      ResultPhi->addIncoming(Call, Builder.GetInsertBlock());
   }

   auto* ErrPhi = cast<llvm::PHINode>(&LPadBB->getInstList().front());
   ErrPhi->addIncoming(ErrLd, Builder.GetInsertBlock());

   Builder.CreateCondBr(IsNull, NormalBB, LPadBB);
   return PrepareReturnedValue(&I, RetVal);
}

llvm::Value* IRGen::visitVirtualInvokeInst(const VirtualInvokeInst& I)
{
   auto ErrAlloc = Builder.CreateAlloca(Int8PtrTy);
   ErrAlloc->setSwiftError(true);

   Builder.CreateStore(llvm::ConstantPointerNull::get(Int8PtrTy), ErrAlloc);

   llvm::SmallVector<llvm::Value*, 8> args{ErrAlloc, nullptr};
   PrepareCallArgs(args, I.getArgs(), true);

   auto* Callee = getLlvmValue(I.getCallee());

   llvm::Value* VTableRef;
   if (I.getCallee()->getType()->isClass()) {
      args[1] = Callee;
      VTableRef = Builder.CreateLoad(getVTable(Callee));
   }
   else {
      //      auto *Val = Builder.CreateStructGEP(ExistentialContainerTy,
      //      Callee, 0); Val = Builder.CreateBitCast(
      //         Val,
      //         getParameterType(I.getFunctionType()->getParamTypes().front())
      //            ->getPointerTo());
      //
      //      args[1] = Builder.CreateLoad(Val);
      args[1] = Callee;

      VTableRef = Builder.CreateCall(
          getGetProtocolVTableFn(),
          {// The existential container.
           toInt8Ptr(Callee),

           // The conformance we're looking for.
           toInt8Ptr(getLlvmValue(I.getProtocolTypeInfo()))});
   }

   auto VTableTy = llvm::ArrayType::get(Int8PtrTy, 0);
   auto VTable = Builder.CreateBitCast(VTableRef, VTableTy->getPointerTo());

   auto FuncPtr = Builder.CreateInBoundsGEP(
       VTable, {Builder.getInt32(0), Builder.getInt32(I.getOffset())});

   auto TypedFnPtr = Builder.CreateBitCast(
       Builder.CreateLoad(FuncPtr),
       getStorageType(I.getFunctionType()->getCanonicalType()));

   auto* Call = Builder.CreateCall(TypedFnPtr, args);
   Call->addParamAttr(0, llvm::Attribute::SwiftError);

   llvm::Value* RetVal;
   if (NeedsStructReturn(I.getFunctionType()->getReturnType())) {
      RetVal = Call->getArgOperand(1);
   }
   else {
      RetVal = Call;
   }

   auto ErrLd = Builder.CreateLoad(ErrAlloc);
   auto IsNull = Builder.CreateIsNull(ErrLd);

   llvm::BasicBlock* NormalBB = getBasicBlock(I.getNormalContinuation());
   llvm::BasicBlock* LPadBB = getBasicBlock(I.getLandingPad());

   if (!RetVal->getType()->isVoidTy()) {
      auto* ResultPhi = cast<llvm::PHINode>(&NormalBB->getInstList().front());
      ResultPhi->addIncoming(Call, Builder.GetInsertBlock());
   }

   auto* ErrPhi = cast<llvm::PHINode>(&LPadBB->getInstList().front());
   ErrPhi->addIncoming(ErrLd, Builder.GetInsertBlock());

   Builder.CreateCondBr(IsNull, NormalBB, LPadBB);
   return PrepareReturnedValue(&I, RetVal);
}

llvm::Value* IRGen::visitLandingPadInst(LandingPadInst const& I)
{
   llvm_unreachable("NOT YET");
}

llvm::Value* IRGen::getVTable(llvm::Value* llvmVal)
{
   auto TypeInfo = Builder.CreateStructGEP(
       llvmVal->getType()->getPointerElementType(), llvmVal, 2);

   llvm::Value* Ld = Builder.CreateLoad(TypeInfo);
   Ld = Builder.CreateBitCast(Ld, TypeInfoTy->getPointerTo());

   return Builder.CreateStructGEP(TypeInfoTy, Ld, MetaType::VTable);
}

llvm::Value* IRGen::getTypeInfo(llvm::Value* llvmVal)
{
   return Builder.CreateStructGEP(llvmVal->getType()->getPointerElementType(),
                                  llvmVal, 2);
}

llvm::Value* IRGen::getVirtualMethod(il::Value* CalleeVal, il::Value* Offset)
{
   llvm::Value* VTableRef;
   llvm::Value* Callee = getLlvmValue(CalleeVal);

   QualType CalleeType = CalleeVal->getType();
   if (CalleeType->isMetaType()) {
      llvm::Value* Conf = Builder.CreateCall(
          getGetConformanceFn(),
          {toInt8Ptr(Callee),
           toInt8Ptr(getLlvmValue(CI.getILGen().GetOrCreateTypeInfo(
               CalleeType->removeMetaType())))});

      Conf = Builder.CreateBitCast(Conf, ProtocolConformanceTy->getPointerTo());
      VTableRef = Builder.CreateLoad(
          Builder.CreateStructGEP(ProtocolConformanceTy, Conf, 1));
   }
   else if (CalleeType->isClass()) {
      VTableRef = Builder.CreateLoad(getVTable(Callee));
   }
   else {
      auto* Conformance = Builder.CreateLoad(
          Builder.CreateStructGEP(ExistentialContainerTy, Callee, 2));

      VTableRef = Builder.CreateLoad(Builder.CreateStructGEP(
          Conformance->getType()->getPointerElementType(), Conformance, 1));
   }

   auto VTableTy = llvm::ArrayType::get(Int8PtrTy, 0);
   auto VTable = Builder.CreateBitCast(VTableRef, VTableTy->getPointerTo());

   return Builder.CreateInBoundsGEP(
       VTable, {Builder.getInt32(0), getLlvmValue(Offset)});
}

llvm::Value* IRGen::visitIntrinsicCallInst(IntrinsicCallInst const& I)
{
   switch (I.getCalledIntrinsic()) {
   case Intrinsic::__ctfe_stacktrace:
   case Intrinsic::begin_unsafe:
   case Intrinsic::end_unsafe:
      return nullptr;
   case Intrinsic::memcpy:
   case Intrinsic::memset:
   case Intrinsic::memcmp: {
      llvm::Value* Args[]
          = {getLlvmValue(I.getOperand(0)), getLlvmValue(I.getOperand(1)),
             getLlvmValue(I.getOperand(2))};

      if (I.getCalledIntrinsic() == Intrinsic::memcpy) {
         return Builder.CreateMemCpy(Args[0], Args[1], Args[2], 1);
      }

      if (I.getCalledIntrinsic() == Intrinsic::memcmp) {
         return Builder.CreateCall(
             getMemCmpFn(), {toInt8Ptr(Args[0]), toInt8Ptr(Args[1]), Args[2]});
      }

      return Builder.CreateMemSet(Args[0], Args[1], Args[2], 1);
   }
   case Intrinsic::likely: {
      auto* Val = I.getArgs().front();
      auto* LLVMVal = getLlvmValue(Val);

      return Builder.CreateCall(
          getIntrinsic(llvm::Intrinsic::ID::expect, Builder.getInt1Ty()),
          {LLVMVal, Builder.getTrue()});
   }
   case Intrinsic::unlikely: {
      auto* Val = I.getArgs().front();
      auto* LLVMVal = getLlvmValue(Val);

      return Builder.CreateCall(
          getIntrinsic(llvm::Intrinsic::ID::expect, Builder.getInt1Ty()),
          {LLVMVal, Builder.getFalse()});
   }
   case Intrinsic::lifetime_begin: {
      auto* Val = I.getArgs().front();
      auto* LLVMVal = getLlvmValue(Val);
      QualType Ty = Val->getType()->removeReference();

      if (Ty->isClass()) {
         LLVMVal = Builder.CreateLoad(LLVMVal);
      }

      return Builder.CreateLifetimeStart(
          LLVMVal, Builder.getInt64(TI.getAllocSizeOfType(Ty)));
   }
   case Intrinsic::lifetime_end: {
      auto* Val = I.getArgs().front();
      auto* LLVMVal = getLlvmValue(Val);
      QualType Ty = Val->getType()->removeReference();

      if (Ty->isClass()) {
         LLVMVal = Builder.CreateLoad(LLVMVal);
      }

      return Builder.CreateLifetimeEnd(
          LLVMVal, Builder.getInt64(TI.getAllocSizeOfType(Ty)));
   }
   case Intrinsic::get_lambda_env: {
      auto lambda = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(LambdaTy, lambda, 1);
   }
   case Intrinsic::get_lambda_funcptr: {
      auto lambda = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(LambdaTy, lambda, 0);
   }
   case Intrinsic::generic_value: {
      auto* Val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(Val->getType()->getPointerElementType(),
                                     Val, 0);
   }
   case Intrinsic::generic_environment: {
      auto* Val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(Val->getType()->getPointerElementType(),
                                     Val, 1);
   }
   case Intrinsic::generic_argument_ref: {
      auto* Ptr = toInt8Ptr(getLlvmValue(I.getArgs()[0]));
      auto* Depth = getLlvmValue(I.getArgs()[1]);
      auto* Idx = getLlvmValue(I.getArgs()[2]);

      return Builder.CreateCall(getGetGenericArgumentFn(), {Ptr, Depth, Idx});
   }
   case Intrinsic::generic_type_value: {
      return Builder.CreateCall(getGetTemplateParamTypeValueFn(),
                                getLlvmValue(I.getArgs()[0]));
   }
   case Intrinsic::existential_ref: {
      auto* Val = getLlvmValue(I.getArgs()[0]);
      return Builder.CreateStructGEP(Val->getType()->getPointerElementType(),
                                     Val, 0);
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
   case Intrinsic::virtual_method: {
      return Builder.CreateLoad(
          getVirtualMethod(I.getArgs()[0], I.getArgs()[1]));
   }
   case Intrinsic::typeinfo_ref: {
      return getTypeInfo(getLlvmValue(I.getArgs().front()));
   }
   case Intrinsic::indirect_case_ref: {
      auto Val = getLlvmValue(I.getArgs().front());
      return Builder.CreateInBoundsGEP(
          Builder.CreateBitCast(Val, Int8PtrTy->getPointerTo()), {WordOne});
   }
   case Intrinsic::unbox: {
      auto Val = getLlvmValue(I.getArgs().front());
      return unboxValue(Val, I.getArgs().front()->getType()->getBoxedType());
   }
   case Intrinsic::typeinfo_cmp:
      return Builder.CreateCall(getTypeInfoCmpFn(),
                                {toInt8Ptr(getLlvmValue(I.getArgs()[0])),
                                 toInt8Ptr(getLlvmValue(I.getArgs()[1]))});
   case Intrinsic::excn_typeinfo_ref: {
      llvm::Value* Val = getLlvmValue(I.getArgs()[0]);
      Val = Builder.CreateBitCast(Val, ErrorTy->getPointerTo());

      return Builder.CreateStructGEP(ErrorTy, Val, 0);
   }
   case Intrinsic::excn_object_ref: {
      llvm::Value* Val = getLlvmValue(I.getArgs()[0]);
      Val = Builder.CreateBitCast(Val, ErrorTy->getPointerTo());

      return Builder.CreateStructGEP(ErrorTy, Val, 2);
   }
   case Intrinsic::print_exception:
      return Builder.CreateCall(getPrintExceptionFn(),
                                toInt8Ptr(getLlvmValue(I.getArgs()[0])));
   case Intrinsic::cleanup_exception:
      return Builder.CreateCall(getCleanupExceptionFn(),
                                toInt8Ptr(getLlvmValue(I.getArgs()[0])));

   case Intrinsic::deinit_existential: {
      return Builder.CreateCall(getDeinitializeExistentialFn(),
                                toInt8Ptr(getLlvmValue(I.getArgs()[0])));
   }
   case Intrinsic::terminate:
      return Builder.CreateCall(getExitFn(), Builder.getInt32(1));
   case Intrinsic::print_runtime_error: {
      auto ID = static_cast<il::IntrinsicCallInst::FatalErrorKind>(
          cast<ConstantInt>(I.getArgs().front())->getU32());

      SourceLocation Loc = I.getSourceLoc();
      assert(Loc && "error intrinsic without source location");

      fs::FileManager& FileMgr = CI.getFileMgr();
      llvm::StringRef FileName = FileMgr.getFileName(Loc);
      LineColPair LineAndCol = FileMgr.getLineAndCol(Loc);

      switch (ID) {
      case IntrinsicCallInst::UnexpectedThrownError: {
         debugPrint("fatal error: 'try!' expression unexpectedly threw an "
                    "error (file: '%s', line: %d)",
                    Builder.CreateGlobalString(FileName),
                    Builder.getInt32(LineAndCol.line));

         break;
      }
      }

      return Builder.CreateCall(getExitFn(), Builder.getInt32(1));
   }
   case Intrinsic::atomic_cmpxchg: {
      auto* Ptr = getLlvmValue(I.getArgs()[0]);
      auto* Cmp = getLlvmValue(I.getArgs()[1]);
      auto* NewVal = getLlvmValue(I.getArgs()[2]);
      auto Success = static_cast<llvm::AtomicOrdering>(
          cast<ConstantInt>(I.getArgs()[3])->getU32());
      auto Failure = static_cast<llvm::AtomicOrdering>(
          cast<ConstantInt>(I.getArgs()[4])->getU32());

      return Builder.CreateAtomicCmpXchg(Ptr, Cmp, NewVal, Success, Failure);
   }
   case Intrinsic::atomic_rmw: {
      ConstantInt* OpVal;
      if (auto* CI = dyn_cast<ConstantInt>(I.getArgs()[0])) {
         OpVal = CI;
      }
      else {
         OpVal = cast<ConstantInt>(
             cast<EnumInitInst>(I.getArgs()[0])->getCase()->getILValue());
      }

      auto Op = static_cast<llvm::AtomicRMWInst::BinOp>(OpVal->getU32());
      auto* Ptr = getLlvmValue(I.getArgs()[1]);
      auto* Val = getLlvmValue(I.getArgs()[2]);
      auto Order = static_cast<llvm::AtomicOrdering>(
          cast<ConstantInt>(I.getArgs()[3])->getU32());

      return Builder.CreateAtomicRMW(Op, Ptr, Val, Order);
   }
   case Intrinsic::coro_id: {
      auto* coro_id = getIntrinsic(llvm::Intrinsic::coro_id);
      return Builder.CreateCall(
          coro_id,
          {
              Builder.getInt32(0),                       // alignment
              toInt8Ptr(getLlvmValue(I.getArgs()[0])),   // promise
              llvm::ConstantPointerNull::get(Int8PtrTy), // coroaddr (keep null)
              llvm::ConstantPointerNull::get(Int8PtrTy)  // fnaddr (keep null)
          });
   }
   case Intrinsic::coro_size: {
      auto* coro_size = llvm::Intrinsic::getDeclaration(
          M, llvm::Intrinsic::coro_size, WordTy);

      return Builder.CreateCall(coro_size);
   }
   case Intrinsic::coro_suspend:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_suspend),
                                {
                                    getLlvmValue(I.getArgs().front()), // handle
                                    getLlvmValue(I.getArgs()[1]),      // final
                                });
   case Intrinsic::coro_begin:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_begin),
                                {
                                    getLlvmValue(I.getArgs().front()), // ID
                                    getLlvmValue(I.getArgs()[1]),      // mem
                                });
   case Intrinsic::coro_end:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_end),
                                {
                                    getLlvmValue(I.getArgs().front()), // handle
                                    Builder.getFalse()                 // unwind
                                });
   case Intrinsic::coro_resume:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_resume),
                                {
                                    getLlvmValue(I.getArgs().front()), // handle
                                });
   case Intrinsic::coro_destroy:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_destroy),
                                {
                                    getLlvmValue(I.getArgs().front()), // handle
                                });
   case Intrinsic::coro_save:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_save),
                                {
                                    getLlvmValue(I.getArgs().front()), // handle
                                });
   case Intrinsic::coro_alloc:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_alloc),
                                {
                                    getLlvmValue(I.getArgs().front()), // ID
                                });
   case Intrinsic::coro_free:
      return Builder.CreateCall(getIntrinsic(llvm::Intrinsic::coro_free),
                                {
                                    getLlvmValue(I.getArgs().front()), // ID
                                    getLlvmValue(I.getArgs()[1]),      // handle
                                });
   case Intrinsic::coro_return:
      return Builder.CreateRet(getLlvmValue(I.getArgs().front()));
   }
}

llvm::Function* IRGen::getIntrinsic(llvm::Intrinsic::ID ID,
                                    llvm::ArrayRef<llvm::Type*> Tys)
{
   auto It = IntrinsicDecls.find(ID);
   if (It != IntrinsicDecls.end())
      return It->getSecond();

   auto* D = llvm::Intrinsic::getDeclaration(M, ID, Tys);
   IntrinsicDecls[ID] = D;

   return D;
}

llvm::Value* IRGen::visitLLVMIntrinsicCallInst(const LLVMIntrinsicCallInst& I)
{
   auto& Fn = Intrinsics[I.getIntrinsicName()];
   if (!Fn) {
      SmallVector<llvm::Type*, 4> ParamTys;
      for (auto& Arg : I.getArgs()) {
         ParamTys.push_back(getStorageType(Arg->getType()));
      }

      auto* FnTy = llvm::FunctionType::get(getStorageType(I.getType()),
                                           ParamTys, false);

      Fn = M->getOrInsertFunction(I.getIntrinsicName()->getIdentifier(), FnTy);
   }

   llvm::SmallVector<llvm::Value*, 8> args;
   for (const auto& arg : I.getArgs())
      args.push_back(getLlvmValue(arg));

   return Builder.CreateCall(Fn, args);
}

static il::Value* LookThroughLoad(il::Value* V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getTarget();

   return V;
}

void IRGen::PrepareCallArgs(SmallVectorImpl<llvm::Value*>& Result,
                            ArrayRef<il::Value*> Args, bool IsMethod)
{
   for (auto* Arg : Args) {
      Result.push_back(getLlvmValue(Arg));
   }
}

llvm::Value* IRGen::PrepareReturnedValue(const il::Value* ILVal,
                                         llvm::Value* RetVal)
{
   if (ILVal->getType()->isEmptyTupleType()) {
      return EmptyTuple;
   }

   return RetVal;
}

llvm::Value* IRGen::visitCallInst(CallInst const& I)
{
   llvm::BasicBlock* MergeBB = nullptr;
   if (I.isTaggedDeinit()) {
      // check the flag to see if it was moved
      auto* Val = LookThroughLoad(I.getOperand(0));
      llvm::GlobalVariable* Flag = getOrCreateInitializedFlag(Val);
      auto FlagLd = Builder.CreateLoad(Flag);

      auto* InitializedBB = llvm::BasicBlock::Create(
          Ctx, "deinit.initialized", Builder.GetInsertBlock()->getParent());
      MergeBB = llvm::BasicBlock::Create(Ctx, "deinit.merge",
                                         Builder.GetInsertBlock()->getParent());

      Builder.CreateCondBr(FlagLd, InitializedBB, MergeBB);
      Builder.SetInsertPoint(InitializedBB);
   }

   SmallVector<llvm::Value*, 8> args;
   PrepareCallArgs(args, I.getArgs(), isa<Method>(I.getCallee()));

   llvm::Value* Call;
   if (auto* Fn = I.getCalledFunction()) {
      Call = CreateCall(Fn, args);
   }
   else {
      Call = Builder.CreateCall(getLlvmValue(I.getCallee()), args);
   }

   if (MergeBB) {
      Builder.CreateBr(MergeBB);
      Builder.SetInsertPoint(MergeBB);
   }

   auto* FnTy = I.getCallee()->getType()->asFunctionType();
   if (FnTy->isAsync()) {
      auto* CoroPromise
          = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::coro_promise);

      QualType RetTy = FnTy->getReturnType();
      auto* Align = Builder.getInt32(TI.getAllocAlignOfType(RetTy));

      auto* Promise
          = Builder.CreateCall(CoroPromise, {Call, Align, Builder.getFalse()});

      Call = Builder.CreateBitCast(Promise,
                                   getStorageType(RetTy)->getPointerTo());

      if (!NeedsStructReturn(RetTy))
         Call = Builder.CreateLoad(Call);
   }

   return PrepareReturnedValue(&I, Call);
}

llvm::Value* IRGen::visitVirtualCallInst(VirtualCallInst const& I)
{
   unsigned i = 0;
   SmallVector<llvm::Value*, 8> args{nullptr};

   QualType RetTy = I.getFunctionType()->getReturnType();
   bool sret = NeedsStructReturn(RetTy);
   if (sret) {
      args[i++] = Builder.CreateAlloca(getStorageType(RetTy));
      args.push_back(nullptr);
   }

   PrepareCallArgs(args, I.getArgs(), true);

   auto* Callee = getLlvmValue(I.getCallee());

   llvm::Value* VTableRef;

   QualType CalleeType = I.getCallee()->getType();
   if (CalleeType->isMetaType()) {
      args[i] = Callee;
      llvm::Value* Conf = Builder.CreateCall(
          getGetConformanceFn(),
          {toInt8Ptr(Callee),
           toInt8Ptr(getLlvmValue(CI.getILGen().GetOrCreateTypeInfo(
               CalleeType->removeMetaType())))});

      Conf = Builder.CreateBitCast(Conf, ProtocolConformanceTy->getPointerTo());
      VTableRef = Builder.CreateLoad(
          Builder.CreateStructGEP(ProtocolConformanceTy, Conf, 1));
   }
   else if (CalleeType->isClass()) {
      args[i] = Callee;
      VTableRef = Builder.CreateLoad(getVTable(Callee));
   }
   else {
      //      auto *Val = Builder.CreateStructGEP(ExistentialContainerTy,
      //      Callee, 0); Val = Builder.CreateBitCast(
      //         Val,
      //         getParameterType(I.getFunctionType()->getParamTypes().front())
      //                 ->getPointerTo());
      //
      //      args[i] = Builder.CreateLoad(Val);

      args[i] = Callee;

      VTableRef = Builder.CreateCall(
          getGetProtocolVTableFn(),
          {// The existential container.
           toInt8Ptr(Callee),

           // The conformance we're looking for.
           toInt8Ptr(getLlvmValue(I.getProtocolTypeInfo()))});
   }

   auto VTableTy = llvm::ArrayType::get(Int8PtrTy, 0);
   auto VTable = Builder.CreateBitCast(VTableRef, VTableTy->getPointerTo());

   auto FuncPtr = Builder.CreateInBoundsGEP(
       VTable, {Builder.getInt32(0), Builder.getInt32(I.getOffset())});

   auto TypedFnPtr = Builder.CreateBitCast(
       Builder.CreateLoad(FuncPtr),
       getStorageType(I.getFunctionType()->getCanonicalType()));

   auto* RetVal
       = PrepareReturnedValue(&I, Builder.CreateCall(TypedFnPtr, args));

   if (I.getFunctionType()->isAsync()) {
      auto* CoroPromise
          = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::coro_promise);

      QualType RetTy = I.getCalledFunction()->getReturnType();
      auto* Align = Builder.getInt32(TI.getAllocAlignOfType(RetTy));

      auto* Promise = Builder.CreateCall(CoroPromise,
                                         {RetVal, Align, Builder.getFalse()});

      RetVal = Builder.CreateBitCast(Promise,
                                     getStorageType(RetTy)->getPointerTo());

      if (!NeedsStructReturn(RetTy)) {
         RetVal = Builder.CreateLoad(RetVal);
      }
   }

   RetVal = PrepareReturnedValue(&I, RetVal);
   if (sret) {
      return args[0];
   }

   return RetVal;
}

llvm::FunctionType* IRGen::getLambdaType(FunctionType* FTy)
{
   llvm::SmallVector<llvm::Type*, 8> ArgTypes{
       BoxTy->getPointerTo()->getPointerTo()};

   llvm::Type* retType;
   if (NeedsStructReturn(FTy->getReturnType())) {
      retType = VoidTy;
      ArgTypes.push_back(getStorageType(FTy->getReturnType())->getPointerTo());
   }
   else if (FTy->getReturnType()->isEmptyTupleType()) {
      retType = VoidTy;
   }
   else {
      retType = getStorageType(FTy->getReturnType());
   }

   for (const auto& arg : FTy->getParamTypes()) {
      auto argTy = getStorageType(arg);
      if (argTy->isStructTy())
         argTy = argTy->getPointerTo();

      ArgTypes.push_back(argTy);
   }

   return llvm::FunctionType::get(retType, ArgTypes, false);
}

llvm::Value* IRGen::visitLambdaCallInst(LambdaCallInst const& I)
{
   auto Lambda = getLlvmValue(I.getLambda());

   auto Env = Builder.CreateStructGEP(LambdaTy, Lambda, 2);
   llvm::Value* Fun
       = Builder.CreateLoad(Builder.CreateStructGEP(LambdaTy, Lambda, 0));

   FunctionType* funTy = I.getLambda()->getType()->asFunctionType();
   llvm::SmallVector<llvm::Value*, 8> args{Env};

   bool sret = NeedsStructReturn(funTy->getReturnType());
   if (sret) {
      auto alloca = CreateAlloca(funTy->getReturnType());
      args.push_back(alloca);
   }

   PrepareCallArgs(args, I.getArgs());

   Fun = Builder.CreateBitCast(Fun, getLambdaType(funTy)->getPointerTo());
   llvm::Value* ret = Builder.CreateCall(Fun, args);

   if (sret)
      ret = args[1];

   return ret;
}

llvm::Value* IRGen::visitStructInitInst(StructInitInst const& I)
{
   llvm::Value* alloca;
   if (I.isFallible()) {
      auto* Opt = cast<EnumDecl>(I.getType()->getRecord());
      auto* Some = Opt->getSomeCase();
      QualType ValueTy = Some->getArgs().front()->getType();

      llvm::Value* ValAlloc;
      if (ValueTy->isClass()) {
         auto TypeSize = TI.getAllocSizeOfType(ValueTy);

         llvm::Value* size = wordSizedInt(TypeSize);
         auto buff = Builder.CreateCall(getMallocFn(), {size});

         ValAlloc = Builder.CreateBitCast(buff, getStorageType(ValueTy));
      }
      else {
         ValAlloc = CreateAlloca(ValueTy);
      }

      alloca = InitEnum(Opt, Some, {ValAlloc});
   }
   else if (I.canUseSRetValue()) {
      alloca = getCurrentSRetValue();
   }
   else if (I.isHeapAllocated()) {
      alloca = Builder.CreateCall(
          getMallocFn(), wordSizedInt(TI.getAllocSizeOfType(I.getType())));

      alloca = Builder.CreateBitCast(
          alloca, getStructTy(I.getInitializedType())->getPointerTo());
   }
   else if (I.getType()->isClass()) {
      alloca = CreateAlloca(getStructTy(I.getInitializedType()));
   }
   else {
      alloca = CreateAlloca(I.getType());
   }

   SmallVector<llvm::Value*, 8> args{alloca};
   PrepareCallArgs(args, I.getArgs(), true);

   Builder.CreateCall(getFunction(I.getInit()), args);

   if (IsSmallStruct(I.getType())) {
      return Builder.CreateLoad(alloca);
   }

   return alloca;
}

llvm::Value* IRGen::visitLambdaInitInst(LambdaInitInst const& I)
{
   auto fun = getLlvmValue(I.getFunction());

   llvm::Value* alloca;
   if (I.getNumOperands() != 0) {
      llvm::Value* Size = CreateSizeOf(LambdaTy);

      size_t EnvSize = TI.getPointerSizeInBytes();
      for (unsigned i = 0, numOps = I.getNumOperands(); i != numOps; ++i) {
         EnvSize += TI.getPointerSizeInBytes();
      }

      llvm::Value* TotalSize = Builder.CreateAdd(Size, wordSizedInt(EnvSize));

      alloca = Builder.CreateCall(getMallocFn(), TotalSize);
      alloca = Builder.CreateBitCast(alloca, LambdaTy->getPointerTo());

      auto ptr = Builder.CreateStructGEP(LambdaTy, alloca, 2);

      unsigned i = 0;
      for (const auto& capture : I.getOperands()) {
         auto val = getPotentiallyBoxedValue(capture);
         auto gep = Builder.CreateInBoundsGEP(ptr, {wordSizedInt(i)});

         Builder.CreateStore(
             val, Builder.CreateBitCast(gep, val->getType()->getPointerTo()));

         ++i;
      }

      Builder.CreateStore(llvm::ConstantPointerNull::get(BoxTy->getPointerTo()),
                          Builder.CreateInBoundsGEP(ptr, {wordSizedInt(i)}));
   }
   else {
      alloca = Builder.CreateCall(getMallocFn(), {CreateSizeOf(LambdaTy)});
      alloca = Builder.CreateBitCast(alloca, LambdaTy->getPointerTo());

      auto envPtr = Builder.CreateStructGEP(LambdaTy, alloca, 2);
      Builder.CreateStore(llvm::ConstantPointerNull::get(BoxTy->getPointerTo()),
                          envPtr);
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
   llvm::Value* alloca;
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

llvm::Value* IRGen::InitEnum(ast::EnumDecl* EnumTy, ast::EnumCaseDecl* Case,
                             llvm::ArrayRef<llvm::Value*> CaseVals,
                             bool CanUseSRetValue)
{
   if (EnumTy->getMaxAssociatedValues() == 0) {
      auto rawTy = getStorageType(EnumTy->getRawType());
      return llvm::ConstantInt::get(
          rawTy, cast<ConstantInt>(Case->getILValue())->getZExtValue());
   }

   auto LlvmTy = getStructTy(EnumTy);

   llvm::Value* alloca;
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

   llvm::Value* StoreDst;
   llvm::Value* IndirectStorage = nullptr;
   llvm::Value* ValuePtr = Builder.CreateStructGEP(LlvmTy, alloca, 1);

   // if the case is indirect, we need to heap allocate the storage for it
   // and store a pointer to that storage
   if (Case->isIndirect()) {
      StoreDst = Builder.CreateCall(getMallocFn(), {CreateSizeOf(CaseTy)});
      IndirectStorage = StoreDst;
   }
   else {
      StoreDst = ValuePtr;
   }

   StoreDst = Builder.CreateBitCast(StoreDst, CaseTy->getPointerTo());

   assert(Case->getArgs().size() == CaseVals.size() && "bad argument count!");

   unsigned i = 0;
   for (const auto& Arg : Case->getArgs()) {
      auto GEP = Builder.CreateStructGEP(CaseTy, StoreDst, i);

      auto val = CaseVals[i];
      if (NeedsStructReturn(Arg->getType())) {
         Builder.CreateMemCpy(GEP, val, TI.getSizeOfType(Arg->getType()),
                              getMemCpyAlign(Arg->getType()));
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
   for (auto& Arg : I.getArgs())
      CaseVals.push_back(getLlvmValue(Arg));

   return InitEnum(I.getEnumTy(), I.getCase(), CaseVals, I.canUseSRetValue());
}

llvm::Value* IRGen::visitDeallocInst(const DeallocInst& I)
{
   if (I.isHeap())
      Builder.CreateCall(getFreeFn(), toInt8Ptr(getLlvmValue(I.getValue())));

   return nullptr;
}

llvm::Value* IRGen::visitDeallocBoxInst(const DeallocBoxInst& I)
{
   return nullptr;
}

llvm::Value* IRGen::applyBinaryOp(unsigned OpCode, QualType ty,
                                  llvm::Value* lhs, llvm::Value* rhs)
{
   using OPC = BinaryOperatorInst::OpCode;
   switch ((OPC)OpCode) {
   case OPC::And:
      return Builder.CreateAnd(lhs, rhs);
   case OPC::Or:
      return Builder.CreateOr(lhs, rhs);
   case OPC::Xor:
      return Builder.CreateXor(lhs, rhs);
   case OPC::LShr:
      return Builder.CreateLShr(lhs, rhs);
   case OPC::AShr:
      return Builder.CreateAShr(lhs, rhs);
   case OPC::Shl:
      return Builder.CreateShl(lhs, rhs);
#define INT_OR_FP_OP(Name)                                                     \
   case OPC::Name:                                                             \
      if (ty->isIntegerType()) {                                               \
         if (ty->isUnsigned())                                                 \
            return Builder.CreateNUW##Name(lhs, rhs);                          \
                                                                               \
         return Builder.CreateNSW##Name(lhs, rhs);                             \
      }                                                                        \
                                                                               \
      return Builder.CreateF##Name(lhs, rhs);
      INT_OR_FP_OP(Add)
      INT_OR_FP_OP(Sub)
      INT_OR_FP_OP(Mul)

#undef INT_OR_FP_OP
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
      llvm::Constant* powFn;
      if (lhs->getType()->isIntegerTy()) {
         powFn = getIntPowFn(ty);
      }
      else if (rhs->getType()->isIntegerTy()) {
         powFn = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::ID::powi,
                                                 {lhs->getType()});
      }
      else {
         powFn = llvm::Intrinsic::getDeclaration(M, llvm::Intrinsic::ID::pow,
                                                 {lhs->getType()});
      }

      return Builder.CreateCall(powFn, {lhs, rhs});
   }
   }

   llvm_unreachable("bad opcode");
}

llvm::Value* IRGen::visitBinaryOperatorInst(const BinaryOperatorInst& I)
{
   auto ty = I.getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   return applyBinaryOp(I.getOpCode(), ty, lhs, rhs);
}

llvm::Value* IRGen::visitCompInst(const CompInst& I)
{
   using OPC = CompInst::OpCode;

   auto ty = I.getLhs()->getType();
   auto lhs = getLlvmValue(I.getOperand(0));
   auto rhs = getLlvmValue(I.getOperand(1));

   switch (I.getOpCode()) {
   case OPC::CompEQ:
      if (ty->isIntegerType() || ty->isPointerType()
          || ty->isThinFunctionTy()) {
         return Builder.CreateICmpEQ(lhs, rhs);
      }

      return Builder.CreateFCmpOEQ(lhs, rhs);
   case OPC::CompNE:
      if (ty->isIntegerType() || ty->isPointerType()
          || ty->isThinFunctionTy()) {
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

llvm::Value* IRGen::visitUnaryOperatorInst(const UnaryOperatorInst& I)
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

      return Builder.CreateFNeg(target);
   }
   }

   llvm_unreachable("bad unary inst");
}

llvm::Value* IRGen::visitBitCastInst(BitCastInst const& I)
{
   return Builder.CreateBitCast(getLlvmValue(I.getOperand(0)),
                                getParameterType(I.getType()));
}

llvm::Value* IRGen::visitIntegerCastInst(IntegerCastInst const& I)
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
   case CastKind::IntToEnum:
      return val;
   default:
      llvm_unreachable("not an integer cast!");
   }
}

llvm::Value* IRGen::visitFPCastInst(FPCastInst const& I)
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

llvm::Value* IRGen::visitUnionCastInst(UnionCastInst const& I)
{
   auto U = getLlvmValue(I.getOperand(0));
   auto toTy = getStorageType(I.getType());

   return Builder.CreateBitCast(U, toTy);
}

llvm::Value* IRGen::visitExistentialInitInst(ExistentialInitInst const& I)
{
   llvm::Value* Val = getLlvmValue(I.getOperand(0));
   llvm::Value* ValPtr;

   if (!Val->getType()->isPointerTy()) {
      auto* Alloc = Builder.CreateAlloca(Val->getType());
      Builder.CreateStore(Val, Alloc);

      ValPtr = toInt8Ptr(Alloc);
   }
   else {
      ValPtr = toInt8Ptr(Val);
   }

   // Get the type info.
   auto* valueTypeInfo = getLlvmValue(I.getValueTypeInfo());

   // Get the protocol type info.
   auto* protoTypeInfo = getConstantVal(I.getProtocolTypeInfo());

   // Initialize the existential container.
   auto alloca = CreateAlloca(ExistentialContainerTy);
   if (I.isPreallocated()) {
      CallRuntimeFunction("_cdot_InitializeExistentialPreallocated",
                          {ValPtr, toInt8Ptr(valueTypeInfo),
                           toInt8Ptr(protoTypeInfo), toInt8Ptr(alloca)});
   }
   else {
      Builder.CreateCall(getInitializeExistentialFn(),
                         {ValPtr, toInt8Ptr(valueTypeInfo),
                          toInt8Ptr(protoTypeInfo), toInt8Ptr(alloca)});
   }

   return alloca;
}

llvm::Value* IRGen::visitGenericInitInst(const GenericInitInst& I)
{
   auto Val = getLlvmValue(I.getOperand(0));
   auto Env = getLlvmValue(I.getOperand(1));

   // A generic container is a tuple (Value, GenericEnvironment)
   auto* Ty = getStorageType(I.getType());
   auto* Alloc = CreateAlloca(Ty);
   auto* ValRef = Builder.CreateStructGEP(Ty, Alloc, 0);
   auto* EnvRef = Builder.CreateStructGEP(Ty, Alloc, 1);

   QualType ValTy = I.getOperand(0)->getType();
   if (NeedsStructReturn(ValTy)) {
      Builder.CreateMemCpy(ValRef, Val, TI.getAllocSizeOfType(ValTy),
                           TI.getAllocAlignOfType(ValTy));
   }
   else {
      Builder.CreateStore(Val, ValRef);
   }

   QualType EnvTy = I.getOperand(1)->getType();
   Builder.CreateMemCpy(EnvRef, Env, TI.getAllocSizeOfType(EnvTy),
                        TI.getAllocAlignOfType(EnvTy));

   return Alloc;
}

llvm::Value* IRGen::visitExceptionCastInst(ExceptionCastInst const& I)
{
   auto Exc = getLlvmValue(I.getOperand(0));
   auto toTy = getStorageType(I.getType());

   return Builder.CreateBitCast(Exc, toTy);
}

llvm::Value* IRGen::visitDynamicCastInst(const DynamicCastInst& I)
{
   auto* ResultAlloc = CreateAlloca(I.getType());
   auto* TypeInfo = getLlvmValue(I.getTargetTypeInfo());
   auto* Val = getLlvmValue(I.getOperand(0));

   Builder.CreateCall(getDynamicDownCastFn(), {Val, TypeInfo, ResultAlloc});
   return ResultAlloc;
}

llvm::Value* IRGen::visitExistentialCastInst(const il::ExistentialCastInst& I)
{
   auto* Val = toInt8Ptr(getLlvmValue(I.getOperand(0)));
   auto* TargetTI = toInt8Ptr(getLlvmValue(I.getTargetTypeInfo()));
   auto* ResultAlloc = Builder.CreateAlloca(getStorageType(I.getType()));

   auto* AllocPtr = toInt8Ptr(ResultAlloc);
   switch (I.getKind()) {
   case CastKind::ExistentialCast:
      CallRuntimeFunction("_cdot_ExistentialCast", {Val, TargetTI, AllocPtr});
      break;
   case CastKind::ExistentialCastFallible: {
      CallRuntimeFunction("_cdot_ExistentialCastFallible",
                          {Val, TargetTI, AllocPtr});
      break;
   }
   case CastKind::ExistentialUnwrap:
      CallRuntimeFunction("_cdot_ExistentialUnwrap", {Val, TargetTI, AllocPtr});
      break;
   case CastKind::ExistentialUnwrapFallible:
      CallRuntimeFunction("_cdot_ExistentialUnwrapFallible",
                          {Val, TargetTI, AllocPtr});
      break;
   default:
      llvm_unreachable("not an existential cast!");
   }

   if (!NeedsStructReturn(I.getType())) {
      return Builder.CreateLoad(ResultAlloc);
   }

   return ResultAlloc;
}

} // namespace il
} // namespace cdot