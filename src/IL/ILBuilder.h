//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BUILDER_H
#define CDOT_BUILDER_H

#include <llvm/ADT/ArrayRef.h>
#include "Value/Function/BasicBlock.h"

#include "../Variant/Type/Type.h"

namespace cdot {

enum class CastKind : unsigned char;
class CompilationUnit;

namespace ast {
   class FunctionDecl;
   class CallableDecl;
   class RecordDecl;
   class StructDecl;
   class ClassDecl;
   class EnumDecl;
   class UnionDecl;
   class ProtocolDecl;
   class ASTContext;
} // namespace ast

namespace il {

#define CDOT_VALUE_DECLARE
#include "Value/ValueIncludes.def"

class Module;
class Context;
enum class Intrinsic : unsigned char;

class ILBuilder {
public:
   using iterator = BasicBlock::iterator;

   explicit ILBuilder(ast::ASTContext &ASTCtx, Context &Ctx);
   explicit ILBuilder(ast::ASTContext &ASTCtx, Module *M);
   ~ILBuilder();

   void SetModule(Module *M)
   {
      this->M = M;
   }

   void SetSourceLoc(const SourceLocation &loc)
   {
      currentSourceLoc = loc;
   }

   void SetInsertPoint(iterator it);
   void SetInsertPoint(BasicBlock *bb);

   iterator &GetInsertPoint()
   {
      return insertPoint;
   }

   BasicBlock *GetInsertBlock() const
   {
      return InsertBlock;
   }

   void ClearInsertPoint()
   {
      InsertBlock = nullptr;
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
         if (!BB && !insertPoint.getNodePtr())
            Builder.SetInsertPoint(nullptr);
         else if (insertPoint != BB->getInstructions().end())
            Builder.SetInsertPoint(insertPoint);
         else
            Builder.SetInsertPoint(BB);
      }

   private:
      ILBuilder &Builder;
      BasicBlock *BB;
      iterator insertPoint;
   };

   InsertPointGuard MakeInsertPointGuard()
   {
      return InsertPointGuard(*this);
   }

   struct InsertPoint {
   public:
      InsertPoint(BasicBlock *BB, iterator IP)
         : BB(BB), insertPoint(IP)
      {

      }

      BasicBlock *BB;
      iterator insertPoint;
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

   BasicBlock *CreateBasicBlock(const std::string &name = "");

   BasicBlock *CreateBasicBlock(Function *func = nullptr,
                                bool setInsertPoint = false,
                                const std::string &name = "");

   ConstantInt *CreateConstantInt(Type *ty, uint64_t value);
   ConstantInt *CreateTrue();
   ConstantInt *CreateFalse();
   ConstantInt *CreateChar(char c);

   ConstantFloat *CreateConstantFP(Type *ty, double d);
   ConstantFloat *CreateConstantFloat(float f);
   ConstantFloat *CreateConstantDouble(double d);
   ConstantString *CreateConstantString(const std::string &str);
   ConstantStruct *CreateConstantStruct(AggregateType *ty,
                                        llvm::ArrayRef<Constant*> elements);

   ConstantArray *CreateConstantArray(llvm::SmallVector<Constant*, 4> &&Arr);
   ConstantArray *CreateConstantArray(llvm::ArrayRef<Constant*> Arr);
   ConstantArray *CreateConstantArray(QualType ty, size_t numElements);

   Argument *CreateArgument(QualType type,
                            bool vararg,
                            BasicBlock *parent = nullptr,
                            const std::string &name = "",
                            const SourceLocation &loc = {});

   ClassType *DeclareClass(ast::ClassDecl *C,
                           const std::string &name,
                           const SourceLocation &loc = {});

   StructType *DeclareStruct(ast::RecordDecl *S,
                             const std::string &name,
                             const SourceLocation &loc = {});

   EnumType *DeclareEnum(ast::EnumDecl *E,
                         const std::string &name,
                         const SourceLocation &loc = {});

   UnionType *DeclareUnion(ast::UnionDecl *U,
                           const std::string &name,
                           const SourceLocation &loc = {});

   ProtocolType *DeclareProtocol(ast::ProtocolDecl *P,
                                 const std::string &name,
                                 const SourceLocation &loc = {});

   Function *CreateFunction(const std::string &name,
                            QualType returnType,
                            llvm::ArrayRef<Argument *> args,
                            bool mightThrow,
                            bool vararg,
                            bool isExternC = false,
                            const SourceLocation &loc = {});

   Lambda *CreateLambda(QualType returnType,
                        llvm::ArrayRef<Argument *> args,
                        bool mightThrow,
                        const SourceLocation &loc = {});

   Method *CreateMethod(AggregateType *forType,
                        const std::string &methodName,
                        QualType returnType,
                        llvm::ArrayRef<Argument *> args,
                        bool isStatic,
                        bool isVirtual,
                        bool isProperty,
                        bool isOperator,
                        bool isConversionOp,
                        bool mightThrow,
                        bool vararg,
                        const SourceLocation &loc = {},
                        bool addSelfArg = true);

   Initializer *CreateInitializer(AggregateType *forType,
                                  const std::string &methodName,
                                  llvm::ArrayRef<Argument *> args,
                                  bool mightThrow,
                                  bool vararg,
                                  const SourceLocation &loc = {},
                                  bool addSelfArg = true);

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
                        const std::string &name = "");

   ProtocolCallInst *CreateProtocolCall(Method *M,
                                       llvm::ArrayRef<Value*> args,
                                       const std::string &name = "");

   VirtualCallInst *CreateVirtualCall(Method *M,
                                      llvm::ArrayRef<Value*> args,
                                      const std::string &name = "");

   IntrinsicCallInst *CreateIntrinsic(Intrinsic id,
                                      llvm::ArrayRef<Value*> args,
                                      const std::string &name = "");

   IndirectCallInst *CreateIndirectCall(Value *Func,
                                        llvm::ArrayRef<Value*> args,
                                        const std::string &name = "");

   LambdaCallInst *CreateLambdaCall(Value *Func,
                                    llvm::ArrayRef<Value*> args,
                                    const std::string &name = "");

   InvokeInst *CreateInvoke(Function *F,
                            llvm::ArrayRef<Value*> args,
                            BasicBlock *NormalCont,
                            BasicBlock *LandingPad,
                            const std::string &name = "");

   ProtocolInvokeInst *CreateProtocolInvoke(Method *M,
                                            llvm::ArrayRef<Value*> args,
                                            BasicBlock *NormalCont,
                                            BasicBlock *LandingPad,
                                            const std::string &name = "");

   VirtualInvokeInst *CreateVirtualInvoke(Method *M,
                                          llvm::ArrayRef<Value*> args,
                                          BasicBlock *NormalCont,
                                          BasicBlock *LandingPad,
                                          const std::string &name = "");

   AllocaInst *CreateAlloca(Type *ofType,
                            unsigned align = 0,
                            bool heap = false,
                            const std::string &name = "");

   AllocaInst *CreateAlloca(Type *ofType,
                            size_t size,
                            unsigned align = 0,
                            bool heap = false,
                            const std::string &name = "");

   StoreInst *CreateStore(Value *val,
                          Value *ptr,
                          const std::string &name = "");

   FieldRefInst *CreateFieldRef(Value *val,
                                StructType *ty,
                                llvm::StringRef fieldName,
                                const std::string &name = "");

   GEPInst *CreateGEP(Value *val,
                      int idx,
                      const std::string &name = "");

   GEPInst *CreateGEP(Value *val,
                      Value *idx,
                      const std::string &name = "");

   GEPInst *CreateStructGEP(AggregateType *Ty,
                            Value *val,
                            size_t idx,
                            const std::string &name = "");

   CaptureExtractInst *CreateCaptureExtract(size_t idx,
                                            const std::string &name = "");

   TupleExtractInst *CreateTupleExtract(Value *val, size_t idx,
                                        const std::string &name = "");

   EnumRawValueInst *CreateEnumRawValue(Value *Val,
                                        const std::string &name = "");

   EnumExtractInst *CreateEnumExtract(Value *Val,
                                      llvm::StringRef caseName,
                                      size_t caseVal,
                                      const std::string &name = "");

   LoadInst *CreateLoad(Value *val,
                        const std::string &name = "");

   AddrOfInst *CreateAddrOf(Value *target,
                            const std::string &name = "");

   PtrToLvalueInst *CreatePtrToLvalue(Value *target,
                                      const std::string &name = "");

   InitInst *CreateInit(StructType *InitializedType,
                        Method *Init,
                        llvm::ArrayRef<Value *> args,
                        const std::string &name = "");

   UnionInitInst *CreateUnionInit(UnionType *UnionTy,
                                  Value *InitializerVal,
                                  const std::string &name = "");

   EnumInitInst *CreateEnumInit(EnumType *EnumTy,
                                std::string const& caseName,
                                llvm::ArrayRef<Value *> args,
                                const std::string &name = "");

   LambdaInitInst *CreateLambdaInit(Function *Function,
                                    QualType LambdaTy,
                                    llvm::ArrayRef<Value*> Captures,
                                    const std::string &name = "");

   UnionCastInst *CreateUnionCast(Value *target,
                                  UnionType *UnionTy,
                                  std::string const& fieldName,
                                  const std::string &name = "");

   ExceptionCastInst *CreateExceptionCast(Value *Lpad,
                                          Type *toType,
                                          const std::string &name = "");

   RetInst *CreateRet(Value *Val,
                      const std::string &name = "");

   RetInst *CreateRetVoid(const std::string &name = "");

   ThrowInst *CreateThrow(Value *thrownVal,
                          GlobalVariable *typeInfo,
                          const std::string &name = "");

   UnreachableInst *CreateUnreachable(const std::string &name = "");

   BrInst *CreateBr(BasicBlock *target,
                    llvm::ArrayRef<Value*> BlockArgs = {},
                    const std::string &name = "");

   BrInst *CreateUnresolvedBr(const std::string &name = "");

   BrInst *CreateCondBr(Value *Condition,
                        BasicBlock *IfBranch,
                        BasicBlock *ElseBranch,
                        llvm::ArrayRef<Value*> TargetArgs = {},
                        llvm::ArrayRef<Value*> ElseArgs = {},
                        const std::string &name = "");

   SwitchInst *CreateSwitch(Value *SwitchVal,
                            const std::string &name = "");

   LandingPadInst *CreateLandingPad(const std::string &name = "");

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
                            const std::string &name = "");

   CDOT_BUILDER_OP(Min)
   CDOT_BUILDER_OP(Neg)

#undef CDOT_BUILDER_OP

#define CDOT_BUILDER_CAST(name) \
   name##Inst *Create##name(Value *val, Type *toType, \
                            const std::string &name = "");

   CDOT_BUILDER_CAST(DynamicCast)
   CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

   IntegerCastInst *CreateIntegerCast(CastKind kind, Value *val,
                                      Type *toType,
                                      const std::string &name = "");

   IntegerCastInst *CreateIntegerCast(CastKind kind, Value *val,
                                      QualType toType,
                                      const std::string &name = "");

   FPCastInst *CreateFPCast(CastKind kind, Value *val,
                            Type *toType,
                            const std::string &name = "");

   FPCastInst *CreateFPCast(CastKind kind, Value *val,
                            QualType toType,
                            const std::string &name = "");

   BitCastInst *CreateBitCast(CastKind kind, Value *val,
                              Type *toType,
                              const std::string &name = "");

   IntToEnumInst *CreateIntToEnum(Value *target,
                                  Type *toType,
                                  const std::string &name = "");

   Value *CreateIsX(Value *V, uint64_t val);
   Value *CreateIsZero(Value *V);
   Value *CreateIsNotZero(Value *V);
   Value *CreateIsNull(Value *V);
   Value *CreateIsOne(Value *V);

   const SourceLocation &getDebugLoc() const
   {
      return debugLoc;
   }

   void setDebugLoc(const SourceLocation &debugLoc)
   {
      ILBuilder::debugLoc = debugLoc;
   }

protected:
   ast::ASTContext &ASTCtx;
   Context &Ctx;
   Module *M;
   SourceLocation currentSourceLoc;

   SourceLocation debugLoc;

   BasicBlock *InsertBlock = nullptr;
   iterator insertPoint;

   void insertInstruction(Instruction *inst,
                          const std::string &name = "");
   BasicBlock *getInsertBlock();

   void importType(Type *Ty);
};

} // namespace il
} // namespace cdot

#endif //CDOT_BUILDER_H
