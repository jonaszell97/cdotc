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

} // namespace llvm;

namespace cdot {
namespace ast {
   class RecordDecl;
   class StructDecl;
}

namespace il {

class ConstantClass;

class IRGen: public InstructionVisitor<IRGen, llvm::Value*> {
public:
   explicit IRGen(CompilationUnit &CU,
                  llvm::LLVMContext &Ctx,
                  llvm::Module *M,
                  bool emitDebugInfo);
   ~IRGen();

   void visitCompilationUnit(CompilationUnit &CU);
   void visitModule(Module &M);
   void visitFunction(Function &F);
   void visitBasicBlock(BasicBlock &B);

   static void linkAndEmit(CompilationUnit &CU);

#  define CDOT_INSTRUCTION(Name) \
   llvm::Value *visit##Name(Name const& I);
#  include "IL/Instructions.def"

private:
   void DeclareFunction(il::Function const* F);

   void ForwardDeclareGlobal(il::GlobalVariable const* G);
   void DeclareGlobal(il::GlobalVariable const* G);

   void ForwardDeclareType(ast::RecordDecl *R);
   void DeclareType(ast::RecordDecl *R);

   void finalize(const CompilationUnit &CU);

   llvm::StructType *getStructTy(QualType Ty);
   llvm::StructType *getStructTy(ast::RecordDecl *R);

   llvm::Type *getLlvmTypeImpl(QualType Ty);

   llvm::Type *getGlobalType(QualType Ty);
   llvm::Type *getStorageType(QualType Ty);
   llvm::Type *getParameterType(QualType Ty);

   llvm::Value *unboxValue(llvm::Value *V, QualType Ty);
   llvm::Value *getLlvmValue(il::Value const* V);
   llvm::Value *getPotentiallyBoxedValue(il::Value const* V);

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

   llvm::Constant *getPrintfFn();
   llvm::Constant *getMemCmpFn();
   llvm::Constant *getIntPowFn(QualType IntTy);

   llvm::StructType *getEnumCaseTy(ast::EnumCaseDecl *Decl);

   void debugPrint(const llvm::Twine &str);

   CompilationUnit &CI;
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

   const il::DebugLocalInst *ElidedDebugLocalInst = nullptr;

   llvm::DenseMap<ast::EnumCaseDecl*, llvm::StructType*> EnumCaseTys;

   llvm::SmallVector<llvm::Function*, 4> GlobalInitFns;
   llvm::SmallVector<llvm::Function*, 4> GlobalDeinitFns;

   llvm::LLVMContext &Ctx;
   llvm::Module *M;
   llvm::IRBuilder<> Builder;

   llvm::IntegerType *WordTy;
   llvm::IntegerType *Int1Ty;

   llvm::PointerType *Int8PtrTy;
   llvm::PointerType *EmptyArrayPtrTy;
   llvm::Type *VoidTy;

   llvm::StructType *ProtocolTy;
   llvm::StructType *TypeInfoTy;
   llvm::StructType *BoxTy;
   llvm::StructType *LambdaTy;

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
   llvm::Constant *IntPowFn;

   llvm::Constant *WordOne;
   llvm::Constant *WordZero;

   llvm::DenseMap<il::Value const*, llvm::Value*> ValueMap;

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
