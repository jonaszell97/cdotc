//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BUILDER_H
#define CDOT_BUILDER_H

#include <llvm/ADT/ArrayRef.h>
#include <Basic/CastKind.h>
#include <IL/Value/Instruction/Operator/OperatorInst.h>
#include "Value/Function/BasicBlock.h"

#include "AST/Type.h"

namespace llvm {
   class APSInt;
   class APFloat;
}

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

   BasicBlock *CreateBasicBlock(llvm::StringRef name = "");

   BasicBlock *CreateBasicBlock(Function *func = nullptr,
                                bool setInsertPoint = false,
                                llvm::StringRef name = "");

   ConstantInt *GetConstantInt(QualType ty, uint64_t value);
   ConstantInt *GetConstantInt(QualType ty, const llvm::APSInt &Val);
   ConstantInt *GetConstantInt(QualType ty, llvm::APSInt &&Val);

   ConstantInt *GetTrue();
   ConstantInt *GetFalse();
   ConstantInt *GetChar(char c);

   ConstantFloat *GetConstantFP(QualType ty, double d);
   ConstantFloat *GetConstantFP(QualType ty, const llvm::APFloat &Val);
   ConstantFloat *GetConstantFP(QualType ty, llvm::APFloat &&Val);

   ConstantFloat *GetConstantFloat(float f);
   ConstantFloat *GetConstantDouble(double d);

   ConstantString *GetConstantString(llvm::StringRef str);

   ConstantStruct *GetConstantStruct(AggregateType *ty,
                                     llvm::ArrayRef<Constant *> elements);

   ConstantArray *GetConstantArray(llvm::SmallVector<Constant *, 4> &&Arr);
   ConstantArray *GetConstantArray(llvm::ArrayRef<Constant *> Arr);
   ConstantArray *GetConstantArray(QualType ty, size_t numElements);

   ConstantPointer *GetConstantPtr(QualType ty, uintptr_t val);
   ConstantPointer *GetConstantNull(QualType ty)
   {
      return GetConstantPtr(ty, 0);
   }

   Argument *CreateArgument(QualType type,
                            bool vararg,
                            BasicBlock *parent = nullptr,
                            llvm::StringRef name = "",
                            const SourceLocation &loc = {});

   ClassType *CreateClass(ast::ClassDecl *C,
                          llvm::StringRef name,
                          const SourceLocation &loc = { });

   StructType *CreateStruct(ast::RecordDecl *S,
                            llvm::StringRef name,
                            const SourceLocation &loc = { });

   EnumType *CreateEnum(ast::EnumDecl *E,
                        llvm::StringRef name,
                        const SourceLocation &loc = { });

   UnionType *CreateUnion(ast::UnionDecl *U,
                          llvm::StringRef name,
                          const SourceLocation &loc = { });

   ProtocolType *CreateProtocol(ast::ProtocolDecl *P,
                                llvm::StringRef name,
                                const SourceLocation &loc = { });

   Function *CreateFunction(llvm::StringRef name,
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

   GlobalVariable *CreateGlobalVariable(QualType type,
                                        bool isConst = false,
                                        Constant *initializer = nullptr,
                                        llvm::StringRef name = "",
                                        const SourceLocation &loc = {});

   GlobalVariable *CreateGlobalVariable(Constant *initializer,
                                        bool isConst = false,
                                        llvm::StringRef name = "",
                                        const SourceLocation &loc = {});

   CallInst *CreateCall(Function *F,
                        llvm::ArrayRef<Value*> args,
                        llvm::StringRef name = "");

   ProtocolCallInst *CreateProtocolCall(Method *M,
                                       llvm::ArrayRef<Value*> args,
                                       llvm::StringRef name = "");

   VirtualCallInst *CreateVirtualCall(Method *M,
                                      llvm::ArrayRef<Value*> args,
                                      llvm::StringRef name = "");

   IntrinsicCallInst *CreateIntrinsic(Intrinsic id,
                                      llvm::ArrayRef<Value*> args,
                                      llvm::StringRef name = "");

   Instruction *GetStrongRefcount(Value *V, llvm::StringRef name = "");
   Instruction *GetWeakRefcount(Value *V, llvm::StringRef name = "");
   Instruction *GetVTable(Value *V, llvm::StringRef name = "");
   Instruction *GetTypeInfo(Value *V, llvm::StringRef name = "");

   Instruction *CreateRetain(Value *V, llvm::StringRef name = "");
   Instruction *CreateRelease(Value *V, llvm::StringRef name = "");
   Instruction *CreateLifetimeBegin(Value *V, llvm::StringRef name = "");
   Instruction *CreateLifetimeEnd(Value *V, llvm::StringRef name = "");

   IndirectCallInst *CreateIndirectCall(Value *Func,
                                        llvm::ArrayRef<Value*> args,
                                        llvm::StringRef name = "");

   LambdaCallInst *CreateLambdaCall(Value *Func,
                                    llvm::ArrayRef<Value*> args,
                                    llvm::StringRef name = "");

   InvokeInst *CreateInvoke(Function *F,
                            llvm::ArrayRef<Value*> args,
                            BasicBlock *NormalCont,
                            BasicBlock *LandingPad,
                            llvm::StringRef name = "");

   ProtocolInvokeInst *CreateProtocolInvoke(Method *M,
                                            llvm::ArrayRef<Value*> args,
                                            BasicBlock *NormalCont,
                                            BasicBlock *LandingPad,
                                            llvm::StringRef name = "");

   VirtualInvokeInst *CreateVirtualInvoke(Method *M,
                                          llvm::ArrayRef<Value*> args,
                                          BasicBlock *NormalCont,
                                          BasicBlock *LandingPad,
                                          llvm::StringRef name = "");

   AllocaInst *CreateAlloca(QualType ofType,
                            unsigned align = 0,
                            bool heap = false,
                            llvm::StringRef name = "");

   AllocaInst *CreateAlloca(QualType ofType,
                            size_t size,
                            unsigned align = 0,
                            bool heap = false,
                            llvm::StringRef name = "");

   Instruction *AllocUninitialized(size_t size,
                                   unsigned align = 0,
                                   bool heap = false,
                                   llvm::StringRef name = "");

   StoreInst *CreateStore(Value *val,
                          Value *ptr,
                          llvm::StringRef name = "");

   FieldRefInst *CreateFieldRef(Value *val,
                                StructType *ty,
                                llvm::StringRef fieldName,
                                llvm::StringRef name = "");

   GEPInst *CreateGEP(Value *val,
                      size_t idx,
                      llvm::StringRef name = "");

   GEPInst *CreateGEP(Value *val,
                      int idx,
                      llvm::StringRef name = "") {
      return CreateGEP(val, size_t(idx), name);
   }

   Instruction *CreateExtractValue(Value *val,
                                   size_t idx,
                                   llvm::StringRef name = "");

   GEPInst *CreateGEP(Value *val,
                      Value *idx,
                      llvm::StringRef name = "");

   GEPInst *CreateStructGEP(AggregateType *Ty,
                            Value *val,
                            size_t idx,
                            llvm::StringRef name = "");

   CaptureExtractInst *CreateCaptureExtract(size_t idx,
                                            llvm::StringRef name = "");

   TupleExtractInst *CreateTupleExtract(Value *val, size_t idx,
                                        llvm::StringRef name = "");

   EnumRawValueInst *CreateEnumRawValue(Value *Val,
                                        llvm::StringRef name = "");

   EnumExtractInst *CreateEnumExtract(Value *Val,
                                      llvm::StringRef caseName,
                                      size_t caseVal,
                                      llvm::StringRef name = "");

   LoadInst *CreateLoad(Value *val,
                        llvm::StringRef name = "");

   AddrOfInst *CreateAddrOf(Value *target,
                            llvm::StringRef name = "");

   PtrToLvalueInst *CreatePtrToLvalue(Value *target,
                                      llvm::StringRef name = "");

   InitInst *CreateInit(StructType *InitializedType,
                        Method *Init,
                        llvm::ArrayRef<Value *> args,
                        llvm::StringRef name = "");

   UnionInitInst *CreateUnionInit(UnionType *UnionTy,
                                  Value *InitializerVal,
                                  llvm::StringRef name = "");

   EnumInitInst *CreateEnumInit(EnumType *EnumTy,
                                std::string const& caseName,
                                llvm::ArrayRef<Value *> args,
                                llvm::StringRef name = "");

   LambdaInitInst *CreateLambdaInit(Function *Function,
                                    QualType LambdaTy,
                                    llvm::ArrayRef<Value*> Captures,
                                    llvm::StringRef name = "");

   UnionCastInst *CreateUnionCast(Value *target,
                                  UnionType *UnionTy,
                                  std::string const& fieldName,
                                  llvm::StringRef name = "");

   ExceptionCastInst *CreateExceptionCast(Value *Lpad,
                                          QualType toType,
                                          llvm::StringRef name = "");

   RetInst *CreateRet(Value *Val,
                      llvm::StringRef name = "");

   RetInst *CreateRetVoid(llvm::StringRef name = "");

   ThrowInst *CreateThrow(Value *thrownVal,
                          GlobalVariable *typeInfo,
                          llvm::StringRef name = "");

   UnreachableInst *CreateUnreachable(llvm::StringRef name = "");

   BrInst *CreateBr(BasicBlock *target,
                    llvm::ArrayRef<Value*> BlockArgs = {},
                    llvm::StringRef name = "");

   BrInst *CreateUnresolvedBr(llvm::StringRef name = "");

   BrInst *CreateCondBr(Value *Condition,
                        BasicBlock *IfBranch,
                        BasicBlock *ElseBranch,
                        llvm::ArrayRef<Value*> TargetArgs = {},
                        llvm::ArrayRef<Value*> ElseArgs = {},
                        llvm::StringRef name = "");

   SwitchInst *CreateSwitch(Value *SwitchVal,
                            BasicBlock *DefaultDst = nullptr,
                            llvm::StringRef name = "");

   LandingPadInst *CreateLandingPad(llvm::StringRef name = "");

   BinaryOperatorInst *CreateBinOp(BinaryOperatorInst::OpCode opc,
                                   Value *lhs, Value *rhs,
                                   llvm::StringRef name = "",
                                   SourceLocation loc = {});

   UnaryOperatorInst *CreateUnaryOp(UnaryOperatorInst::OpCode opc,
                                    Value *target,
                                    llvm::StringRef name = "",
                                    SourceLocation loc = {});

   CompInst *CreateComp(CompInst::OpCode opc,
                        Value *lhs, Value *rhs,
                        llvm::StringRef name = "",
                        SourceLocation loc = {});

#  define CDOT_BINARY_OP(Name, OP)                                      \
   BinaryOperatorInst *Create##Name(Value *lhs, Value *rhs,             \
                                    llvm::StringRef name = "",          \
                                    const SourceLocation &loc = {});

#  define CDOT_COMP_OP(Name, OP)                                     \
   CompInst *Create##Name(Value *lhs, Value *rhs,                    \
                          llvm::StringRef name = "",                 \
                          const SourceLocation &loc = {});

#  define CDOT_UNARY_OP(name, OP)                                    \
   UnaryOperatorInst *Create##name(Value *target,                    \
                                   llvm::StringRef name = "");

#  include "IL/Value/Instructions.def"

#define CDOT_BUILDER_CAST(name) \
   name##Inst *Create##name(Value *val, Type *toType, \
                            llvm::StringRef name = "");

   CDOT_BUILDER_CAST(DynamicCast)
   CDOT_BUILDER_CAST(ProtoCast)

#undef CDOT_BUILDER_CAST

   IntegerCastInst *CreateIntegerCast(CastKind kind, Value *val,
                                      QualType toType,
                                      llvm::StringRef name = "");

#  define CDOT_INT_CAST(Kind)                                        \
   IntegerCastInst *Create##Kind(Value *val,                         \
                                 QualType toType,                    \
                                 llvm::StringRef name = "") {        \
      return CreateIntegerCast(CastKind::Kind, val, toType, name);   \
   }

   CDOT_INT_CAST(IntToPtr)
   CDOT_INT_CAST(PtrToInt)
   CDOT_INT_CAST(Ext)
   CDOT_INT_CAST(Trunc)
   CDOT_INT_CAST(SignFlip)
   CDOT_INT_CAST(FPToInt)
   CDOT_INT_CAST(IntToFP)
   CDOT_INT_CAST(IntToEnum)
   CDOT_INT_CAST(EnumToInt)

#  define CDOT_FP_CAST(Kind)                                    \
   FPCastInst *Create##Kind(Value *val,                         \
                            QualType toType,                    \
                            llvm::StringRef name = "") {        \
      return CreateFPCast(CastKind::Kind, val, toType, name);   \
   }

   CDOT_FP_CAST(FPExt)
   CDOT_FP_CAST(FPTrunc)

#  undef CDOT_INT_CAST
#  undef CDOT_FP_CAST

   FPCastInst *CreateFPCast(CastKind kind, Value *val,
                            QualType toType,
                            llvm::StringRef name = "");

   BitCastInst *CreateBitCast(CastKind kind, Value *val,
                              Type *toType,
                              llvm::StringRef name = "");

   IntToEnumInst *CreateIntToEnum(Value *target,
                                  Type *toType,
                                  llvm::StringRef name = "");

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
                          llvm::StringRef name = "");
   BasicBlock *getInsertBlock();

   void importType(QualType Ty);
};

} // namespace il
} // namespace cdot

#endif //CDOT_BUILDER_H
