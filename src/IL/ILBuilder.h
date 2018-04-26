//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_BUILDER_H
#define CDOT_BUILDER_H

#include "AST/Type.h"
#include "Basic/CastKind.h"
#include "BasicBlock.h"

#include <llvm/ADT/ArrayRef.h>

namespace llvm {
   class APSInt;
   class APFloat;
}

namespace cdot {

enum class ConstructorKind : unsigned char;
class DeclarationName;
class CompilationUnit;
class IdentifierInfo;

namespace fs {
   class FileManager;
} // namespace f

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
   class EnumCaseDecl;
} // namespace ast

namespace il {

#define CDOT_ALL(Name) class Name;
#include "Instructions.def"

class Instruction;
class ConstantExpr;

class CastInst;
class OperatorInst;

class Module;
class Context;
enum class Intrinsic : unsigned char;

class ILBuilder {
public:
   using iterator = BasicBlock::iterator;

   explicit ILBuilder(ast::ASTContext &ASTCtx, Context &Ctx,
                      fs::FileManager &FileMgr);

   explicit ILBuilder(ast::ASTContext &ASTCtx, fs::FileManager &FileMgr,
                      Module *M);

   ~ILBuilder();

   void SetModule(Module *M)
   {
      this->M = M;
   }

   void SetInsertPoint(iterator it);
   void SetInsertPoint(BasicBlock *bb, bool KeepDebugLoc = false);

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
      BasicBlock *BB;
      iterator insertPoint;
      SourceLocation DebugLoc;
   };

   InsertPoint saveIP()
   {
      return { InsertBlock, insertPoint, LastDebugLoc };
   }

   void restoreIP(InsertPoint &IP)
   {
      InsertBlock = IP.BB;
      insertPoint = IP.insertPoint;
      LastDebugLoc = IP.DebugLoc;
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

   ConstantStruct *GetConstantStruct(ast::StructDecl *S,
                                     llvm::ArrayRef<Constant*> elements);

   ConstantClass *GetConstantClass(ast::ClassDecl *S,
                                   GlobalVariable *TI,
                                   llvm::ArrayRef<Constant*> elements,
                                   ConstantClass *Base = nullptr);

   ConstantClass *GetConstantClass(ConstantStruct *S,
                                   GlobalVariable *TI,
                                   ConstantClass *Base = nullptr);

   ConstantClass *ForwardDeclareConstantClass(ast::ClassDecl *S,
                                              GlobalVariable *TI);

   ConstantClass *ReplaceForwardDecl(ConstantClass *ForwardDecl,
                                     llvm::ArrayRef<Constant*> elements,
                                     ConstantClass *Base = nullptr);

   ConstantClass *ReplaceForwardDecl(ConstantClass *ForwardDecl,
                                     ConstantStruct *StructVal,
                                     ConstantClass *Base = nullptr);

   ConstantUnion *GetConstantUnion(ast::UnionDecl *U, Constant *InitVal);
   ConstantEnum *GetConstantEnum(ast::EnumCaseDecl *Case,
                                 llvm::ArrayRef<Constant*> CaseVals);

   ConstantArray *GetConstantArray(QualType ArrTy,
                                   llvm::ArrayRef<Constant *> Arr);

   ConstantArray *GetConstantArray(llvm::ArrayRef<Constant *> Arr);
   ConstantArray *GetConstantArray(QualType ty, size_t numElements);

   ConstantTuple *GetConstantTuple(QualType TupleTy,
                                   llvm::ArrayRef<Constant *> Arr);

   ConstantTuple *GetConstantTuple(llvm::ArrayRef<Constant *> Arr);

   Constant *GetConstantPtr(QualType ty, uintptr_t val);
   Constant *GetConstantNull(QualType ty)
   {
      return GetConstantPtr(ty, 0);
   }

   UndefValue *GetUndefValue(QualType Ty);
   MagicConstant *GetMagicConstant(unsigned char Kind);

   Argument *CreateArgument(QualType type,
                            unsigned Convention = 0,
                            BasicBlock *parent = nullptr,
                            llvm::StringRef name = "",
                            SourceLocation loc = {});

   Function *CreateFunction(llvm::StringRef name,
                            QualType returnType,
                            llvm::ArrayRef<Argument *> args,
                            bool mightThrow,
                            bool vararg,
                            SourceLocation loc = {});

   Lambda *CreateLambda(QualType returnType,
                        llvm::ArrayRef<Argument *> args,
                        bool mightThrow,
                        SourceLocation loc = {});

   Method *CreateMethod(llvm::StringRef methodName,
                        QualType returnType,
                        llvm::ArrayRef<Argument *> args,
                        bool isStatic,
                        bool isVirtual,
                        bool mightThrow,
                        bool vararg,
                        SourceLocation loc = {});

   Initializer *CreateInitializer(llvm::StringRef methodName,
                                  llvm::ArrayRef<Argument *> args,
                                  bool mightThrow,
                                  bool vararg,
                                  ConstructorKind Kind,
                                  SourceLocation loc = {});

   GlobalVariable *CreateGlobalVariable(QualType type,
                                        bool isConst = false,
                                        Constant *initializer = nullptr,
                                        llvm::StringRef name = "",
                                        SourceLocation loc = {});

   GlobalVariable *CreateGlobalVariable(Constant *initializer,
                                        bool isConst = false,
                                        llvm::StringRef name = "",
                                        SourceLocation loc = {});

   VTable *CreateVTable(llvm::ArrayRef<il::Function*> Entries,
                        ast::ClassDecl *ClassTy);

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
                            bool IsLet = false,
                            llvm::StringRef name = "");

   AllocaInst *CreateAlloca(QualType ofType,
                            size_t size,
                            unsigned align = 0,
                            bool heap = false,
                            bool IsLet = false,
                            llvm::StringRef name = "");

   AllocBoxInst *CreateAllocBox(QualType Ty,
                                Function *Deinitializer = nullptr,
                                llvm::StringRef name = "");

   DeallocInst *CreateDealloc(Value *V,
                              bool Heap = false,
                              llvm::StringRef name = "");

   DeallocBoxInst *CreateDeallocBox(Value *V,
                                    llvm::StringRef name = "");

   Instruction *AllocUninitialized(size_t size,
                                   unsigned align = 0,
                                   bool heap = false,
                                   llvm::StringRef name = "");

   StoreInst *CreateStore(Value *val,
                          Value *ptr,
                          bool IsInit = false,
                          llvm::StringRef name = "");

   FieldRefInst *CreateFieldRef(Value *val,
                                const DeclarationName &fieldName,
                                bool IsLet = false,
                                llvm::StringRef name = "");

   GEPInst *CreateStructGEP(Value *val,
                            size_t idx,
                            bool IsLet = false,
                            llvm::StringRef name = "");

   GEPInst *CreateGEP(Value *val,
                      size_t idx,
                      bool IsLet = false,
                      llvm::StringRef name = "");

   Instruction *CreateExtractValue(Value *val,
                                   size_t idx,
                                   bool IsLet = false,
                                   llvm::StringRef name = "");

   GEPInst *CreateGEP(Value *val, Value *idx, bool IsLet = false,
                      llvm::StringRef name = "");

   CaptureExtractInst *CreateCaptureExtract(size_t idx,
                                            llvm::StringRef name = "");

   TupleExtractInst *CreateTupleExtract(Value *val, size_t idx,
                                        bool IsLet = false,
                                        llvm::StringRef name = "");

   EnumRawValueInst *CreateEnumRawValue(Value *Val,
                                        llvm::StringRef name = "");

   EnumExtractInst *CreateEnumExtract(Value *Val,
                                      ast::EnumCaseDecl *Case,
                                      size_t caseVal,
                                      bool IsLet = false,
                                      llvm::StringRef name = "");

   EnumExtractInst *CreateEnumExtract(Value *Val,
                                      const IdentifierInfo *CaseName,
                                      size_t caseVal,
                                      bool IsLet = false,
                                      llvm::StringRef name = "");

   LoadInst *CreateLoad(Value *val,
                        llvm::StringRef name = "");

   AddrOfInst *CreateAddrOf(Value *target,
                            llvm::StringRef name = "");

   PtrToLvalueInst *CreatePtrToLvalue(Value *target,
                                      llvm::StringRef name = "");

   InitInst *CreateInit(ast::StructDecl *InitializedType,
                        Method *Init,
                        llvm::ArrayRef<Value *> args,
                        llvm::StringRef name = "");

   UnionInitInst *CreateUnionInit(ast::UnionDecl *UnionTy,
                                  Value *InitializerVal,
                                  llvm::StringRef name = "");

   EnumInitInst *CreateEnumInit(ast::EnumDecl *EnumTy,
                                ast::EnumCaseDecl *Case,
                                llvm::ArrayRef<Value *> args,
                                llvm::StringRef name = "");

   LambdaInitInst *CreateLambdaInit(Function *Function,
                                    QualType LambdaTy,
                                    llvm::ArrayRef<Value*> Captures,
                                    llvm::StringRef name = "");

   DeinitializeLocalInst *CreateDeinitializeLocal(Value *RefcountedVal,
                                                  llvm::StringRef name = "");
   DeinitializeLocalInst *CreateDeinitializeLocal(Function *DeinitFn,
                                                  Value *ValueToDeinit,
                                                  llvm::StringRef name = "");

   DeinitializeTemporaryInst *CreateDeinitializeTemp(Value *RefcountedVal,
                                                     llvm::StringRef name = "");
   DeinitializeTemporaryInst *CreateDeinitializeTemp(Function *DeinitFn,
                                                     Value *ValueToDeinit,
                                                     llvm::StringRef name = "");

   UnionCastInst *CreateUnionCast(Value *target,
                                  ast::UnionDecl *UnionTy,
                                  const IdentifierInfo *fieldName,
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

   BinaryOperatorInst *CreateBinOp(unsigned char OpCode,
                                   Value *lhs, Value *rhs,
                                   llvm::StringRef name = "",
                                   SourceLocation loc = {});

   UnaryOperatorInst *CreateUnaryOp(unsigned char OpCode,
                                    Value *target,
                                    llvm::StringRef name = "",
                                    SourceLocation loc = {});

   CompInst *CreateComp(unsigned char OpCode,
                        Value *lhs, Value *rhs,
                        llvm::StringRef name = "",
                        SourceLocation loc = {});

#  define CDOT_BINARY_OP(Name, OP)                                      \
   BinaryOperatorInst *Create##Name(Value *lhs, Value *rhs,             \
                                    llvm::StringRef name = "",          \
                                    SourceLocation loc = {});

#  define CDOT_COMP_OP(Name, OP)                                     \
   CompInst *Create##Name(Value *lhs, Value *rhs,                    \
                          llvm::StringRef name = "",                 \
                          SourceLocation loc = {});

#  define CDOT_UNARY_OP(Name, OP)                                    \
   UnaryOperatorInst *Create##Name(Value *target,                    \
                                   llvm::StringRef name = "");

#  include "Instructions.def"

#define CDOT_BUILDER_CAST(Name) \
   Name##Inst *Create##Name(Value *val, Type *toType, \
                            llvm::StringRef name = "");

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
                              QualType toType,
                              llvm::StringRef name = "");

   DynamicCastInst* CreateDynamicCast(il::Value *val,
                                      ast::ClassDecl *TargetTy,
                                      QualType Type,
                                      llvm::StringRef name = "");

   IntToEnumInst *CreateIntToEnum(Value *target,
                                  Type *toType,
                                  llvm::StringRef name = "");

   Value *CreateIsX(Value *V, uint64_t val);
   Value *CreateIsZero(Value *V);
   Value *CreateIsNotZero(Value *V);
   Value *CreateIsNull(Value *V);
   Value *CreateIsOne(Value *V);

   void SetDebugLoc(SourceLocation debugLoc);

   il::DebugLocInst *CreateDebugLoc(SourceLocation Loc);
   il::DebugLocalInst *CreateDebugLocal(const IdentifierInfo *II, Value *Val);

protected:
   ast::ASTContext &ASTCtx;
   Context &Ctx;
   Module *M;

   SourceLocation LastDebugLoc;
   fs::FileManager &FileMgr;

   BasicBlock *InsertBlock = nullptr;
   iterator insertPoint;

   void insertInstruction(Instruction *inst,
                          llvm::StringRef name = "");
   BasicBlock *getInsertBlock();
};

} // namespace il
} // namespace cdot

#endif //CDOT_BUILDER_H
