//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BUILDER_H
#define CDOT_BUILDER_H

#include <llvm/ADT/ArrayRef.h>
#include "Value/Function/BasicBlock.h"
#include "../Variant/Type/QualType.h"

namespace cdot {

struct CompilationUnit;
struct Argument;
class Function;
class Callable;

namespace cl {

class Record;
struct Method;

} // namespace cl

namespace ast {

class Function;

} // namespace ast

namespace il {

#define CDOT_VALUE_DECLARE
#include "Value/ValueIncludes.def"

class Module;
class Context;

class ILBuilder {
public:
   ILBuilder(Context &Ctx);
   ILBuilder(Module *M);
   ~ILBuilder();

   void SetModule(Module *M)
   {
      this->M = M;
   }

   void SetSourceLoc(const SourceLocation &loc)
   {
      currentSourceLoc = loc;
   }

   void SetInsertPoint(llvm::SmallVector<Instruction*, 8>::iterator it);
   void SetInsertPoint(BasicBlock *bb);

   llvm::SmallVector<Instruction*, 8>::iterator &GetInsertPoint()
   {
      return insertPoint;
   }

   BasicBlock *GetInsertBlock() const
   {
      return InsertBlock;
   }

   Module *getModule()
   {
      return M;
   }

   Context &getContext()
   {
      return Ctx;
   }

   class InsertPointGuard {
   public:
      explicit InsertPointGuard(ILBuilder &Builder)
         : Builder(Builder), BB(Builder.GetInsertBlock()),
           insertPoint(Builder.GetInsertPoint()) {}

      ~InsertPointGuard()
      {
         if (insertPoint != BB->getInstructions().end()) {
            Builder.SetInsertPoint(insertPoint);
         }
         else {
            Builder.SetInsertPoint(BB);
         }
      }

   private:
      ILBuilder &Builder;
      BasicBlock *BB;
      BasicBlock::iterator insertPoint;
   };

   InsertPointGuard MakeInsertPointGuard()
   {
      return InsertPointGuard(*this);
   }

   struct InsertPoint {
   public:
      InsertPoint(BasicBlock *BB, BasicBlock::iterator IP)
         : BB(BB), insertPoint(IP)
      {

      }

      BasicBlock *BB;
      BasicBlock::iterator insertPoint;
   };

   InsertPoint saveIP()
   {
      return { InsertBlock, insertPoint };
   }

   void restoreIP(InsertPoint &IP)
   {
      InsertBlock = IP.BB;
      insertPoint = IP.insertPoint;
   }

   BasicBlock *CreateBasicBlock(const std::string &name = "",
                                const SourceLocation &loc = {});

   BasicBlock *CreateBasicBlock(Function *func = nullptr,
                                bool setInsertPoint = false,
                                const std::string &name = "",
                                const SourceLocation &loc = {});

   ConstantInt *CreateConstantInt(Type *ty, uint64_t value);
   ConstantInt *CreateTrue();
   ConstantInt *CreateFalse();
   ConstantInt *CreateChar(char c);

   ConstantFloat *CreateConstantFloat(float f);
   ConstantFloat *CreateConstantDouble(double d);
   ConstantString *CreateConstantString(const std::string &str);
   ConstantStruct *CreateConstantStruct(AggregateType *ty,
                                        llvm::ArrayRef<Constant*> elements);

   ConstantArray *CreateConstantArray(llvm::SmallVector<Constant*, 4> &&Arr);
   ConstantArray *CreateConstantArray(llvm::ArrayRef<Constant*> Arr);
   ConstantArray *CreateConstantArray(Type *ty, size_t numElements);

   ConstantBitCastInst *CreateConstantBitCast(Constant *Val,
                                              Type *toType,
                                              const std::string &name = "",
                                              const SourceLocation &loc = {});

   Argument *CreateArgument(Type *type,
                            bool vararg,
                            Function *parent,
                            const std::string &name = "",
                            const SourceLocation &loc = {});

   ClassType *DeclareClass(const std::string &name,
                           llvm::StringRef parentClass,
                           const SourceLocation &loc = {});

   StructType *DeclareStruct(const std::string &name,
                             const SourceLocation &loc = {});

   EnumType *DeclareEnum(const std::string &name, Type *rawType,
                         const SourceLocation &loc = {});

   UnionType *DeclareUnion(const std::string &name,
                           const SourceLocation &loc = {});

   ProtocolType *DeclareProtocol(const std::string &name,
                                 const SourceLocation &loc = {});

   Function *CreateFunction(Callable *F);

   Function *CreateFunction(const std::string &name,
                            QualType returnType,
                            llvm::ArrayRef<cdot::Argument> args,
                            bool mightThrow,
                            const SourceLocation &loc = {});

   Method *CreateMethod(cl::Method *M);
   Method *CreateMethod(AggregateType *forType,
                        const std::string &methodName,
                        QualType returnType,
                        llvm::ArrayRef<cdot::Argument> args,
                        bool isStatic,
                        bool mightThrow,
                        const SourceLocation &loc = {});

   Initializer *CreateInitializer(cl::Method *method);
   Initializer *CreateInitializer(AggregateType *forType,
                                  const std::string &methodName,
                                  llvm::ArrayRef<cdot::Argument> args,
                                  bool mightThrow,
                                  const SourceLocation &loc = {});

   GlobalVariable *CreateGlobalVariable(Type *type,
                                        bool isConst = false,
                                        Constant *initializer = nullptr,
                                        const std::string &name = "",
                                        const SourceLocation &loc = {});

   GlobalVariable *CreateGlobalVariable(Constant *initializer,
                                        bool isConst = false,
                                        const std::string &name = "",
                                        const SourceLocation &loc = {});

   CallInst *CreateCall(Function *F,
                        llvm::ArrayRef<Value*> args,
                        const std::string &name = "",
                        const SourceLocation &loc = {});

   InvokeInst *CreateInvoke(Function *F,
                            llvm::ArrayRef<Value*> args,
                            BasicBlock *NormalCont,
                            BasicBlock *LandingPad,
                            const std::string &name = "",
                            const SourceLocation &loc = {});

   CallInst *CreateMethodCall(Method *M,
                              Value *Self,
                              llvm::ArrayRef<Value*> args,
                              const std::string &name = "",
                              const SourceLocation &loc = {});

   CallInst *CreateMethodInvoke(Method *M,
                                Value *Self,
                                llvm::ArrayRef<Value*> args,
                                BasicBlock *NormalCont,
                                BasicBlock *LandingPad,
                                const std::string &name = "",
                                const SourceLocation &loc = {});
   
   AllocaInst *CreateAlloca(Type *ofType,
                            unsigned align = 0,
                            bool heap = false,
                            const std::string &name = "",
                            const SourceLocation &loc = {});

   StoreInst *CreateStore(Value *val,
                          Value *ptr,
                          const std::string &name = "",
                          const SourceLocation &loc = {});

   GEPInst *CreateGEP(Value *val,
                      size_t idx,
                      const std::string &name = "",
                      const SourceLocation &loc = {});

   GEPInst *CreateStructGEP(AggregateType *Ty,
                            Value *val,
                            size_t idx,
                            const std::string &name = "",
                            const SourceLocation &loc = { });

   TupleExtractInst *CreateTupleExtract(Value *val, size_t idx,
                                        const string &name = "",
                                        const SourceLocation &loc = {});

   LoadInst *CreateLoad(Value *val,
                        const std::string &name = "",
                        const SourceLocation &loc = {});

   InitInst *CreateInit(StructType *InitializedType,
                        Method *Init,
                        llvm::ArrayRef<Value *> args,
                        const std::string &name = "",
                        const SourceLocation &loc = { });

   UnionInitInst *CreateUnionInit(UnionType *UnionTy,
                                  Value *InitializerVal,
                                  const std::string &name = "",
                                  const SourceLocation &loc = { });

   EnumInitInst *CreateEnumInit(EnumType *EnumTy,
                                llvm::StringRef caseName,
                                llvm::ArrayRef<Value *> args,
                                const std::string &name = "",
                                const SourceLocation &loc = { });

   LambdaInitInst *CreateLambda(Constant *Function,
                                llvm::SmallVector<Value*, 4> &&Captures,
                                const std::string &name = "",
                                const SourceLocation &loc = {});

   UnionCastInst *CreateUnionCast(Value *target,
                                  UnionType *UnionTy,
                                  llvm::StringRef fieldName,
                                  const std::string &name = "",
                                  const SourceLocation &loc = {});

   ExceptionCastInst *CreateExceptionCast(Value *Lpad,
                                          Type *toType,
                                          const std::string &name = "",
                                          const SourceLocation &loc = {});

   RetInst *CreateRet(Value *Val,
                      const std::string &name = "",
                      const SourceLocation &loc = { });

   RetInst *CreateRetVoid(const std::string &name = "",
                          const SourceLocation &loc = { });

   ThrowInst *CreateThrow(Value *thrownVal,
                          const std::string &name = "",
                          const SourceLocation &loc = { });

   UnreachableInst *CreateUnreachable(const std::string &name = "",
                                      const SourceLocation &loc = { });

   BrInst *CreateBr(BasicBlock *target,
                    llvm::SmallVector<Value*, 4> &&BlockArgs = {},
                    const std::string &name = "",
                    const SourceLocation &loc = { });

   BrInst *CreateUnresolvedBr(const std::string &name = "",
                              const SourceLocation &loc = { });

   BrInst *CreateCondBr(Value *Condition,
                        BasicBlock *IfBranch,
                        BasicBlock *ElseBranch,
                        llvm::SmallVector<Value*, 4> &&TargetArgs = {},
                        llvm::SmallVector<Value*, 4> &&ElseArgs = {},
                        const std::string &name = "",
                        const SourceLocation &loc = { });

   SwitchInst *CreateSwitch(Value *SwitchVal,
                            const std::string &name = "",
                            const SourceLocation &loc = { });

   LandingPadInst *CreateLandingPad(const std::string &name = "",
                                    const SourceLocation &loc = { });

#define CDOT_BUILDER_OP(Name) \
   Name##Inst *Create##Name(Value *lhs, Value *rhs, \
                            const std::string &name = "", \
                            const SourceLocation &loc = {});

   CDOT_BUILDER_OP(Add)
   CDOT_BUILDER_OP(Sub)
   CDOT_BUILDER_OP(Mul)
   CDOT_BUILDER_OP(Div)
   CDOT_BUILDER_OP(Mod)
   CDOT_BUILDER_OP(Exp)

   CDOT_BUILDER_OP(And)
   CDOT_BUILDER_OP(Or)
   CDOT_BUILDER_OP(Xor)

   CDOT_BUILDER_OP(AShr)
   CDOT_BUILDER_OP(LShr)
   CDOT_BUILDER_OP(Shl)

   CDOT_BUILDER_OP(CompEQ)
   CDOT_BUILDER_OP(CompNE)
   CDOT_BUILDER_OP(CompLT)
   CDOT_BUILDER_OP(CompGT)
   CDOT_BUILDER_OP(CompLE)
   CDOT_BUILDER_OP(CompGE)

#undef CDOT_BUILDER_OP
#define CDOT_BUILDER_OP(name) \
   name##Inst *Create##name(Value *target, \
                            const std::string &name = "", \
                            const SourceLocation &loc = {});

   CDOT_BUILDER_OP(Min)
   CDOT_BUILDER_OP(Neg)

#undef CDOT_BUILDER_OP

#define CDOT_BUILDER_CAST(name) \
   name##Inst *Create##name(Value *val, Type *toType, \
                            const std::string &name = "", \
                            const SourceLocation &loc = {});

   CDOT_BUILDER_CAST(BitCast)
   CDOT_BUILDER_CAST(IntegerCast)
   CDOT_BUILDER_CAST(FPCast)
   CDOT_BUILDER_CAST(DynamicCast)
   CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

protected:
   Context &Ctx;
   Module *M;
   SourceLocation currentSourceLoc;

   BasicBlock *InsertBlock = nullptr;
   llvm::SmallVector<Instruction*, 8>::iterator insertPoint;

   void insertInstruction(Instruction *inst);
   BasicBlock *getInsertBlock();
};

} // namespace il
} // namespace cdot

#endif //CDOT_BUILDER_H
