//
// Created by Jonas Zell on 10.12.17.
//

#ifndef CDOT_IRGEN_H
#define CDOT_IRGEN_H

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/Dwarf.h>
#include <stack>
#include "../InstructionVisitor.h"

namespace llvm {

class DIBuilder;

} // namespace llvm;

namespace cdot {
namespace il {

class IRGen: public InstructionVisitor<IRGen, llvm::Value*> {
public:
   explicit IRGen(llvm::LLVMContext &Ctx,
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
#  include "../../Value/Instructions.def"

private:
   void DeclareFunction(il::Function const* F);

   void ForwardDeclareGlobal(il::GlobalVariable const* G);
   void DeclareGlobal(il::GlobalVariable const* G);

   void ForwardDeclareType(il::AggregateType const* Ty);
   void DeclareType(il::AggregateType const* Ty);

   void outputIR(const CompilationUnit &CU);
   void finalize(const CompilationUnit &CU);

   llvm::StructType *getStructTy(Type *Ty);
   llvm::StructType *getStructTy(llvm::StringRef name);
   llvm::StructType *getStructTy(AggregateType *Ty);

   llvm::Type *getLlvmType(QualType Ty);
   llvm::Type *getFieldType(QualType Ty);
   llvm::Type *getLlvmType(Type *Ty);

   llvm::Value *getLlvmValue(il::Value const* V);

   llvm::Constant *getConstantVal(il::Constant const* C);
   llvm::Value *getBlockArg(il::Argument const* A);

   llvm::BasicBlock *getBasicBlock(llvm::StringRef name);
   llvm::BasicBlock *getBasicBlock(BasicBlock *BB);

   llvm::Function *getFunction(il::Function *F);

   llvm::ConstantInt *wordSizedInt(uint64_t val);
   llvm::Value *toInt8Ptr(llvm::Value *V);

   llvm::Value *CreateCall(il::Function *F,
                           llvm::SmallVector<llvm::Value*, 8> &args);

   llvm::Value *CreateAlloca(llvm::Type *AllocatedType,
                             size_t allocatedSize = 1);

   llvm::Value *AccessField(StructType *Ty,
                            Value *Val,
                            llvm::StringRef fieldName,
                            bool load = false);

   llvm::FunctionType *getLambdaType(FunctionType *FTy);

   llvm::Constant *getMallocFn();
   llvm::Constant *getFreeFn();
   llvm::Constant *getThrowFn();
   llvm::Constant *getAllocExcnFn();
   llvm::Constant *getReleaseFn();
   llvm::Constant *getRetainFn();
   llvm::Constant *getPrintfFn();
   llvm::Constant *getMemCmpFn();
   llvm::Constant *getIntPowFn();

   void debugPrint(const llvm::Twine &str);

   il::Module *ILMod;

   bool emitDebugInfo : 1;

   llvm::DIBuilder *DI;
   llvm::DIFile *File;
   llvm::DICompileUnit *CU;
   llvm::SmallDenseMap<size_t, llvm::DIFile*> DIFileMap;
   llvm::SmallDenseMap<uintptr_t, llvm::DIType*> DITypeMap;
   llvm::SmallDenseMap<uintptr_t, llvm::DISubprogram*> DIFuncMap;
   std::stack<llvm::DIScope*> ScopeStack;

   llvm::SmallDenseMap<uintptr_t, llvm::StructType*> TypeMap;

   llvm::LLVMContext &Ctx;
   llvm::Module *M;
   llvm::IRBuilder<> Builder;

   llvm::IntegerType *WordTy;
   llvm::IntegerType *Int1Ty;

   llvm::PointerType *Int8PtrTy;
   llvm::Type *VoidTy;

   llvm::StructType *ProtocolTy;
   llvm::StructType *TypeInfoTy;
   llvm::StructType *ClassInfoTy;
   llvm::StructType *RefcountedTy;
   llvm::StructType *LambdaTy;

   llvm::FunctionType *DeinitializerTy;

   llvm::Constant *MallocFn;
   llvm::Constant *FreeFn;
   llvm::Constant *ThrowFn;
   llvm::Constant *AllocExcFn;
   llvm::Constant *RetainFn;
   llvm::Constant *ReleaseFn;
   llvm::Constant *PrintfFn;
   llvm::Constant *MemCmpFn;
   llvm::Constant *IntPowFn;

   llvm::StringMap<llvm::Value*> ValueMap;

   llvm::MDNode *emitModuleDI();

   llvm::DIFile *getFileDI(size_t fileID, llvm::StringRef fileName,
                           llvm::StringRef path);

   llvm::DIFile *getFileDI(SourceLocation loc);

   llvm::DIType *getTypeDI(Type *ty);
   llvm::DIType *getRecordDI(Type *ty);

   llvm::dwarf::Tag getTagForRecord(il::AggregateType *Ty);

   void beginLexicalScope(const SourceLocation &loc);

   void beginScope(llvm::DIScope *scope);
   void endScope();

   void emitLocalVarDI(Instruction const &I,
                       llvm::Value *inst);

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
