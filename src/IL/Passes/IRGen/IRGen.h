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
   explicit IRGen(llvm::LLVMContext &Ctx);
   ~IRGen();

   void visitCompilationUnit(CompilationUnit &CU);
   void visitModule(Module &M);
   void visitFunction(Function &F);
   void visitBasicBlock(BasicBlock &B);

   static void linkAndEmit(llvm::LLVMContext &Ctx,
                           std::vector<CompilationUnit> &CUs);

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

   llvm::Function *getMallocFn();
   llvm::Function *getFreeFn();
   llvm::Function *getThrowFn();
   llvm::Function *getAllocExcnFn();
   llvm::Function *getReleaseFn();
   llvm::Function *getRetainFn();
   llvm::Function *getPrintfFn();
   llvm::Function *getMemCmpFn();
   llvm::Function *getIntPowFn();

   void debugPrint(const llvm::Twine &str);

   il::Module *ILMod;

   llvm::DIBuilder *DI;
   llvm::DIFile *File;
   llvm::SmallDenseMap<size_t, llvm::DIFile*> DIFileMap;
   llvm::SmallDenseMap<uintptr_t, llvm::DIType*> DITypeMap;
   llvm::SmallDenseMap<uintptr_t, llvm::DISubprogram*> DIFuncMap;
   std::stack<llvm::DIScope*> ScopeStack;

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

   llvm::Function *MallocFn;
   llvm::Function *FreeFn;
   llvm::Function *ThrowFn;
   llvm::Function *AllocExcFn;
   llvm::Function *RetainFn;
   llvm::Function *ReleaseFn;
   llvm::Function *PrintfFn;
   llvm::Function *MemCmpFn;
   llvm::Function *IntPowFn;

   llvm::StringMap<llvm::Value*> ValueMap;

   llvm::MDNode *emitModuleDI();

   llvm::DIFile *getFileDI(size_t fileID, llvm::StringRef fileName,
                           llvm::StringRef path);

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
