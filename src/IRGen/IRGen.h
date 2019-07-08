//
// Created by Jonas Zell on 10.12.17.
//

#ifndef CDOT_IRGEN_H
#define CDOT_IRGEN_H

#include "Basic/TargetInfo.h"
#include "IL/Passes/InstructionVisitor.h"

#include <llvm/BinaryFormat/Dwarf.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <stack>

namespace llvm {

class DIBuilder;
class TargetMachine;

} // namespace llvm;

namespace cdot {
namespace ast {
   class RecordDecl;
   class SemaPass;
   class StructDecl;
}

namespace il {

class ConstantClass;

class IRGen final: public InstructionVisitor<IRGen, llvm::Value*> {
public:
   explicit IRGen(CompilerInstance &CU,
                  llvm::LLVMContext &Ctx,
                  bool emitDebugInfo);

   ~IRGen();

   void visitCompilationUnit(CompilerInstance &CU);
   void visitModule(Module &M);
   void visitFunction(Function &F);
   void visitBasicBlock(BasicBlock &B);

   llvm::Module *linkModules(CompilerInstance &CI);

   void prepareModuleForEmission(llvm::Module *Mod);
   void linkAndEmit(CompilerInstance &CU);

   void emitObjectFile(llvm::StringRef OutFile, llvm::Module *Module,
                       bool KeepOpen = false, int *FD = nullptr,
                       size_t *FileSize = nullptr);

   void emitObjectFile(llvm::raw_ostream &OS, llvm::Module *Module);
   void emitAsmFile(llvm::raw_ostream &OS, llvm::Module *Module);
   void emitExecutable(StringRef OutFile, llvm::Module *Module,
                       ArrayRef<StringRef> AdditionalFilesToLink = {});

   void emitStaticLibrary(llvm::StringRef OutFile, llvm::Module *Module);
   void emitDynamicLibrary(llvm::StringRef OutFile, llvm::Module *Module);

   std::string createLinkedModuleTmpFile(StringRef Str);

#  define CDOT_INSTRUCTION(Name) \
   llvm::Value *visit##Name(Name const& I);
#  include "IL/Instructions.def"

private:
   void DeclareFunction(il::Function const* F);

   void ForwardDeclareGlobal(il::GlobalVariable const* G);
   void DeclareGlobal(il::GlobalVariable const* G);

   void ForwardDeclareType(ast::RecordDecl *R);
   void DeclareType(ast::RecordDecl *R);

   void finalize(const CompilerInstance &CU);
   void runMandatoryPasses(llvm::Module *M);

   bool NeedsStructReturn(CanType Ty);
   bool IsSmallStruct(CanType Ty);
   bool PassStructDirectly(CanType Ty);

   void addMappedValue(const il::Value *ILVal, llvm::Value *LLVMVal);

   llvm::StructType *getStructTy(QualType Ty);
   llvm::StructType *getStructTy(ast::RecordDecl *R);

   llvm::Type *getLlvmTypeImpl(CanType Ty);

   llvm::Type *getGlobalType(CanType Ty);
   llvm::Type *getStorageType(CanType Ty);
   llvm::Type *getParameterType(CanType Ty);

   llvm::Value *unboxValue(llvm::Value *V, QualType Ty);
   llvm::Value *getLlvmValue(il::Value const* V);
   llvm::Value *getPotentiallyBoxedValue(il::Value const* V);

   void PrepareCallArgs(SmallVectorImpl<llvm::Value*> &Result,
                        ArrayRef<il::Value*> Args,
                        bool IsMethod = false);

   llvm::Value *PrepareReturnedValue(const il::Value *ILVal,
                                     llvm::Value *RetVal);

   llvm::Value *applyBinaryOp(unsigned OpCode, QualType ty, llvm::Value *LHS,
                              llvm::Value *RHS);

   void buildConstantClass(llvm::SmallVectorImpl<llvm::Constant*> &Vec,
                           const ConstantClass *Class);

   llvm::Constant *getConstantVal(il::Constant const* C);

   llvm::BasicBlock *getBasicBlock(llvm::StringRef name);
   llvm::BasicBlock *getBasicBlock(BasicBlock *BB);

   llvm::Value *CreateCopy(il::Value *Val);
   llvm::Value *CreateCopy(QualType Ty, llvm::Value *Val);

   llvm::Function *getFunction(il::Function *F);
   llvm::Value *getCurrentSRetValue();
   llvm::Value *getCurrentErrorValue();

   /// Return the VTable for a protocol conformance given a protocol type
   /// info and an existential container.
   llvm::Value *getVTableForConformance(llvm::Value *Existential,
                                        llvm::Value *ProtocolTypeInfo);

   llvm::ConstantInt *wordSizedInt(uint64_t val);
   llvm::Value *toInt8Ptr(llvm::Value *V);
   llvm::Constant *toInt8Ptr(llvm::Constant *V);

   llvm::Value *CreateSizeOf(llvm::Type *Ty);

   llvm::Value *CreateCall(il::Function *F,
                           llvm::SmallVector<llvm::Value*, 8> &args);

   llvm::AllocaInst *CreateAlloca(llvm::Type *AllocatedType,
                                  size_t allocatedSize = 1,
                                  unsigned alignment = 0);

   llvm::AllocaInst *CreateAlloca(QualType AllocatedType,
                                  size_t allocatedSize = 1,
                                  unsigned alignment = 0);

   llvm::AllocaInst *CreateAlloca(llvm::Type *AllocatedType,
                                  il::Value *allocatedSize,
                                  unsigned alignment = 0);

   llvm::AllocaInst *CreateAlloca(QualType AllocatedType,
                                  il::Value *allocatedSize,
                                  unsigned alignment = 0);

   llvm::Value *InitEnum(ast::EnumDecl *E,
                         ast::EnumCaseDecl *Case,
                         llvm::ArrayRef<llvm::Value*> CaseVals,
                         bool CanUseSRetValue = false);

   unsigned getFieldOffset(ast::StructDecl *S,
                           const DeclarationName &FieldName);
   QualType getFieldType(ast::StructDecl *S, const DeclarationName &FieldName);

   llvm::Value *AccessField(ast::StructDecl *S,
                            Value *Val,
                            const DeclarationName &FieldName,
                            bool load = false);

   llvm::Value *getVTable(llvm::Value *llvmVal);
   llvm::Value *getTypeInfo(llvm::Value *llvmVal);
   llvm::Value *getVirtualMethod(il::Value *Callee, il::Value *Offset);

   llvm::FunctionType *getLambdaType(FunctionType *FTy);

   llvm::Constant *getMallocFn();
   llvm::Constant *getFreeFn();
   llvm::Constant *getThrowFn();
   llvm::Constant *getAllocExcnFn();

   llvm::Constant *getReleaseFn();
   llvm::Constant *getRetainFn();
   llvm::Constant *getReleaseLambdaFn();
   llvm::Constant *getRetainLambdaFn();
   llvm::Constant *getReleaseBoxFn();
   llvm::Constant *getRetainBoxFn();
   llvm::Constant *getTypeInfoCmpFn();
   llvm::Constant *getPrintExceptionFn();
   llvm::Constant *getCleanupExceptionFn();
   llvm::Constant *getExitFn();

   llvm::Constant *getInitializeExistentialFn();
   llvm::Constant *getDeinitializeExistentialFn();
   llvm::Constant *getCopyExistentialFn();
   llvm::Constant *getCastExistentialFn();
   llvm::Constant *getCastExistentialFallibleFn();
   llvm::Constant *getUnwrapExistentialFn();
   llvm::Constant *getCopyClassFn();
   llvm::Constant *getGetProtocolVTableFn();
   llvm::Constant *getGetConformanceFn();

   llvm::Constant *getGetGenericArgumentFn();
   llvm::Constant *getGetGenericTypeValueFn();

   llvm::Constant *getDynamicDownCastFn();

   llvm::Constant *getPrintfFn();
   llvm::Constant *getMemCmpFn();
   llvm::Constant *getIntPowFn(QualType IntTy);

   llvm::StructType *getEnumCaseTy(ast::EnumCaseDecl *Decl);

   llvm::Function *getIntrinsic(llvm::Intrinsic::ID ID);

   template<class ...Args>
   void debugPrint(const llvm::Twine &str, Args&&... args)
   {
      Builder.CreateCall(getPrintfFn(),
                         { toInt8Ptr(Builder.CreateGlobalString(str.str())),
                            std::forward<Args&&>(args)... });
   }

   llvm::CallInst *CallRuntimeFunction(StringRef FuncName,
                                       ArrayRef<llvm::Value*> Args,
                                       llvm::Type *RetTy = nullptr);

   llvm::GlobalVariable *getOrCreateInitializedFlag(const il::Value *ForVal);

   CompilerInstance &CI;
   ast::SemaPass &Sema;
   const TargetInfo &TI;
   il::Module *ILMod;

   bool emitDebugInfo : 1;

   llvm::DIBuilder *DI;
   llvm::DIFile *File;
   llvm::DICompileUnit *CU;
   llvm::DenseMap<size_t, llvm::DIFile*> DIFileMap;
   llvm::DenseMap<QualType, llvm::DIType*> DITypeMap;
   llvm::DenseMap<uintptr_t, llvm::DISubprogram*> DIFuncMap;
   std::stack<llvm::DIScope*> ScopeStack;

   llvm::DenseMap<ast::RecordDecl*, llvm::StructType*> StructTypeMap;
   llvm::DenseMap<QualType, llvm::Type*> TypeMap;

   llvm::DenseMap<StringRef, llvm::Constant*> RuntimeFunctions;

   const il::DebugLocalInst *ElidedDebugLocalInst = nullptr;

   llvm::BasicBlock *AllocaBB = nullptr;
   llvm::BasicBlock::iterator AllocaIt;

   llvm::DenseMap<ast::EnumCaseDecl*, llvm::StructType*> EnumCaseTys;
   llvm::DenseMap<IdentifierInfo*, llvm::Constant*> Intrinsics;

   using FunctionPriorityPair = std::pair<llvm::Function*,unsigned short>;

   llvm::SmallVector<FunctionPriorityPair, 4> GlobalInitFns;
   llvm::SmallVector<FunctionPriorityPair, 4> GlobalDeinitFns;

   llvm::DenseMap<const il::Value*, llvm::GlobalVariable*> InitializedFlagMap;

   llvm::LLVMContext &Ctx;
   llvm::Module *M;
   llvm::IRBuilder<> Builder;

   SmallPtrSet<void*, 16> LinkedModuleLibs;
   llvm::Module *LinkedModule = nullptr;
   llvm::TargetMachine *TargetMachine = nullptr;
   llvm::SmallPtrSet<llvm::Module*, 16> FinalizedModules;

   llvm::IntegerType *WordTy;
   llvm::IntegerType *Int1Ty;

   llvm::PointerType *Int8PtrTy;
   llvm::PointerType *EmptyArrayPtrTy;
   llvm::Type *VoidTy;

   llvm::StructType *ExistentialContainerTy = nullptr;
   llvm::StructType *GenericEnvironmentTy = nullptr;
   llvm::StructType *ProtocolConformanceTy = nullptr;

   llvm::StructType *TypeInfoTy = nullptr;
   llvm::StructType *ErrorTy;
   llvm::StructType *BoxTy;
   llvm::StructType *LambdaTy;
   llvm::Type *EmptyTupleTy;

   llvm::FunctionType *DeinitializerTy;

   llvm::Constant *MallocFn;
   llvm::Constant *FreeFn;
   llvm::Constant *ThrowFn;
   llvm::Constant *AllocExcFn;
   llvm::Constant *RetainFn;
   llvm::Constant *ReleaseFn;
   llvm::Constant *RetainLambdaFn;
   llvm::Constant *ReleaseLambdaFn;
   llvm::Constant *RetainBoxFn;
   llvm::Constant *ReleaseBoxFn;
   llvm::Constant *PrintfFn;
   llvm::Constant *MemCmpFn;
   llvm::Constant *TypeInfoCmpFn;
   llvm::Constant *PrintExceptionFn;
   llvm::Constant *CleanupExceptionFn;
   llvm::Constant *ExitFn;

   llvm::Constant *InitializeExistentialFn;
   llvm::Constant *DeinitializeExistentialFn;
   llvm::Constant *CopyExistentialFn;
   llvm::Constant *CopyClassFn;
   llvm::Constant *CastExistentialFn;
   llvm::Constant *CastExistentialFallibleFn;
   llvm::Constant *UnwrapExistentialFn;
   llvm::Constant *GetProtocolVTableFn;
   llvm::Constant *GetConformanceFn;

   llvm::Constant *GetGenericArgumentFn;
   llvm::Constant *GetGenericTypeValueFn;

   llvm::Constant *DynamicDownCastFn;

   llvm::Constant *WordOne;
   llvm::Constant *WordZero;
   llvm::Constant *EmptyTuple;

   llvm::DenseMap<il::Value const*, llvm::Value*> ValueMap;
   llvm::DenseMap<il::Value const*, llvm::DILocalVariable*> DIVarMap;

   llvm::DenseMap<unsigned, llvm::Function*> IntrinsicDecls;

   llvm::DenseMap<llvm::Function*, llvm::Value*> CoroHandleMap;
   llvm::DenseMap<llvm::Function*, llvm::BasicBlock*> CoroCleanupMap;
   llvm::DenseMap<llvm::Function*, llvm::BasicBlock*> CoroSuspendMap;

   llvm::MDNode *emitModuleDI();

   llvm::DIFile *getFileDI(size_t fileID, llvm::StringRef fileName,
                           llvm::StringRef path);

   llvm::DIFile *getFileDI(SourceLocation loc);

   llvm::DIType *getTypeDI(QualType ty);
   llvm::DIType *getRecordDI(QualType ty);

   llvm::dwarf::Tag getTagForRecord(ast::RecordDecl *R);

   void beginLexicalScope(const SourceLocation &loc);

   void beginScope(llvm::DIScope *scope);
   void endScope();

   void emitLocalVarDI(const il::DebugLocalInst &Inst,
                       llvm::Value *Val = nullptr);

   void emitDebugValue(il::Value *Val, llvm::Value *LLVMVal);

   llvm::MDNode *emitGlobalVarDI(GlobalVariable const& G,
                                 llvm::GlobalVariable *var);

   llvm::MDNode *emitFunctionDI(il::Function const& F,
                                llvm::Function *func);

   void emitArgumentDI(il::Function const& F,
                       llvm::Function *func);
};

} // namespace il
} // namespace cdot

#endif //CDOT_IRGEN_H
