//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILGENPASS_H
#define CDOT_ILGENPASS_H

#include "AST/Decl.h"
#include "AST/EmptyASTVisitor.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/StmtOrDecl.h"
#include "Basic/CastKind.h"
#include "Basic/Precedence.h"
#include "IL/ILBuilder.h"
#include "IL/Passes/PassManager.h"
#include "ILGen/Cleanup.h"

#include <stack>
#include <unordered_map>
#include <queue>

#include <llvm/ADT/DenseSet.h>

namespace cdot {

class TargetInfo;
class ConversionSequence;

namespace ctfe {
   struct CTFEResult;
} // namespace ctfe

namespace ast {

class SemaPass;
class VarDecl;
class Decl;
class TypeExpr;

class ILGenPass: public EmptyASTVisitor<il::Value*, void> {
public:
   explicit ILGenPass(il::Context &Ctx, SemaPass &SP);
   bool run();

   il::Value *visit(Expression *expr);
   il::Value *evaluateAsConstant(Expression *expr);

   void visit(Statement *node);
   void visit(Decl *D);

   bool prepareFunctionForCtfe(CallableDecl *C, StmtOrDecl Caller,
                               bool NeedsCompileTimeAttr = true);
   ctfe::CTFEResult evaluateStaticExpr(Expression *expr);

   void ForwardDeclareRecord(RecordDecl* R);
   void GenerateTypeInfo(RecordDecl *R, bool innerDecls = false);

   void visitDeclContext(DeclContext *Ctx);

   void visitCompoundStmt(CompoundStmt *node);
   void visitNamespaceDecl(NamespaceDecl *node);

   void visitUnittestDecl(UnittestDecl *D);

   void visitCompoundDecl(CompoundDecl *D);

   void visitDeclStmt(DeclStmt *Stmt);
   void visitModuleDecl(ModuleDecl *Decl);

   void visitLocalVarDecl(LocalVarDecl *Decl);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitDestructuringDecl(DestructuringDecl *D);
   void doDestructure(DestructuringDecl *D, il::Value *V);

   void visitFunctionDecl(FunctionDecl *node);
   void visitCallableDecl(CallableDecl *node);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);
   void visitClassDecl(ClassDecl *node);
   void visitStructDecl(StructDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitProtocolDecl(ProtocolDecl *node);
   void visitEnumDecl(EnumDecl *node);
   void visitUnionDecl(UnionDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitInitDecl(InitDecl *node);
   void visitDeinitDecl(DeinitDecl *node);

   void visitFuncArgDecl(FuncArgDecl *node);

   void visitAttributedStmt(AttributedStmt *Stmt);
   il::Value *visitAttributedExpr(AttributedExpr *Expr);

   il::Value *visitIdentifierRefExpr(IdentifierRefExpr *Expr);
   il::Value *visitBuiltinIdentExpr(BuiltinIdentExpr *node);
   il::Value *visitSelfExpr(SelfExpr *Expr);
   il::Value * visitSuperExpr(SuperExpr *Expr);

   il::Value *visitParenExpr(ParenExpr *node);

   il::Value *visitSubscriptExpr(SubscriptExpr *node);
   il::Value *visitCallExpr(CallExpr *Expr);
   il::Value *visitAnonymousCallExpr(AnonymousCallExpr *Expr);
   il::Value *visitTupleMemberExpr(TupleMemberExpr *node);
   il::Value *visitEnumCaseExpr(EnumCaseExpr *node);
   il::Value *visitTemplateArgListExpr(TemplateArgListExpr *Expr);

   il::BasicBlock* visitIfConditions(ArrayRef<IfCondition> Conditions,
                                     il::BasicBlock *TrueBB,
                                     il::BasicBlock *FalseBB);

   void visitForStmt(ForStmt *node);
   void visitForInStmt(ForInStmt *Stmt);
   void visitWhileStmt(WhileStmt *node);
   void visitIfStmt(IfStmt *node);

   void visitMatchStmt(MatchStmt *node);
   void HandleEqualitySwitch(MatchStmt *Stmt);
   void HandleIntegralSwitch(MatchStmt *node);

   void HandlePatternSwitch(MatchStmt *node);

   void visitCaseStmt(CaseStmt *node);

   void visitPatternExpr(PatternExpr *E,
                         il::Value *MatchVal,
                         il::BasicBlock *MatchBB,
                         il::BasicBlock *NoMatchBB,
                         ArrayRef<il::Value*> MatchArgs = {},
                         ArrayRef<il::Value*> NoMatchArgs = {});

   il::Value *visitExpressionPattern(ExpressionPattern *node,
                                     il::Value *MatchVal = nullptr,
                                     il::BasicBlock *MatchBB = nullptr,
                                     il::BasicBlock *NoMatchBB = nullptr,
                                     ArrayRef<il::Value*> MatchArgs = {},
                                     ArrayRef<il::Value*> NoMatchArgs = {});

   il::Value *visitCasePattern(CasePattern *node,
                               il::Value *MatchVal = nullptr,
                               il::BasicBlock *MatchBB = nullptr,
                               il::BasicBlock *NoMatchBB = nullptr,
                               ArrayRef<il::Value*> MatchArgs = {},
                               ArrayRef<il::Value*> NoMatchArgs = {});

   il::Value *visitIsPattern(IsPattern *node,
                             il::Value *MatchVal = nullptr,
                             il::BasicBlock *MatchBB = nullptr,
                             il::BasicBlock *NoMatchBB = nullptr,
                             ArrayRef<il::Value*> MatchArgs = {},
                             ArrayRef<il::Value*> NoMatchArgs = {});

   void visitDiscardAssignStmt(DiscardAssignStmt *Stmt);
   void visitReturnStmt(ReturnStmt *Stmt);
   void visitBreakStmt(BreakStmt *node);
   void visitContinueStmt(ContinueStmt *node);

   il::Value *visitIntegerLiteral(IntegerLiteral *node);
   il::Value *visitFPLiteral(FPLiteral *node);
   il::Value *visitBoolLiteral(BoolLiteral *node);
   il::Value *visitCharLiteral(CharLiteral *node);

   il::Value *visitDictionaryLiteral(DictionaryLiteral *Expr);
   il::Value *visitArrayLiteral(ArrayLiteral *Arr);

   il::Value *visitNoneLiteral(NoneLiteral *node);
   il::Value *visitStringLiteral(StringLiteral *S);
   il::Value *visitStringInterpolation(StringInterpolation *node);
   il::Value *visitTupleLiteral(TupleLiteral *node);

   il::Value *visitExprSequence(ExprSequence *node);
   il::Value *visitBinaryOperator(BinaryOperator *BinOp);
   il::Value *visitAssignExpr(AssignExpr *Expr);
   il::Value *visitIfExpr(IfExpr *node);
   il::Value *visitUnaryOperator(UnaryOperator *UnOp);

   il::Value *visitCastExpr(CastExpr *Cast);
   il::Value *visitTypePredicateExpr(TypePredicateExpr *Pred);

   il::Value *visitTypeExpr(TypeExpr *Expr);
   il::Value *visitFunctionTypeExpr(FunctionTypeExpr *Expr);
   il::Value *visitTupleTypeExpr(TupleTypeExpr *Expr);
   il::Value *visitArrayTypeExpr(ArrayTypeExpr *Expr);
   il::Value *visitDeclTypeExpr(DeclTypeExpr *Expr);
   il::Value *visitPointerTypeExpr(PointerTypeExpr *Expr);
   il::Value *visitReferenceTypeExpr(ReferenceTypeExpr *Expr);
   il::Value *visitOptionTypeExpr(OptionTypeExpr *Expr);

   void visitDoStmt(DoStmt *node);
   il::Value *visitTryExpr(TryExpr *Expr);
   void visitThrowStmt(ThrowStmt *Stmt);

   il::Value *visitAwaitExpr(AwaitExpr *Expr);

   il::Value *visitLambdaExpr(LambdaExpr *Expr);
   il::Value *visitImplicitCastExpr(ImplicitCastExpr *node);

   void visitDebugStmt(DebugStmt *node);

   il::Value* visitStaticExpr(StaticExpr *node);
   il::Value *visitTraitsExpr(TraitsExpr *node);

   il::Function* DeclareFunction(CallableDecl *C);

   void VerifyFunction(il::Function *F);
   void CanonicalizeFunction(il::Function *F);
   void OptimizeFunction(il::PassManager &PM, il::Function *F);

   void notifyFunctionCalledInTemplate(CallableDecl *C);

   void DeclareDeclContext(DeclContext *Ctx);
   void DeclareRecord(RecordDecl *R);
   void declareRecordInstantiation(RecordDecl *Inst);

   void DeclareGlobalVariable(GlobalVarDecl *decl);

   struct StringInfo {
      /// The String type.
      StructDecl *String = nullptr;

      /// The String Buffer type.
      StructDecl *StringBuffer = nullptr;

      /// The String Storage type.
      StructDecl *StringStorage = nullptr;

      /// The String Storage enum.
      EnumDecl *StringStorageStorage = nullptr;

      /// The Atomic<UInt32> type.
      StructDecl *AtomicIntDecl = nullptr;

      /// The String Storage direct case.
      EnumCaseDecl *StringStorageDirectCase = nullptr;
   };

   void initStringInfo();
   il::Constant *MakeStdString(llvm::StringRef Str);

   struct ModuleRAII {
      ModuleRAII(ILGenPass &ILGen, CallableDecl *C);
      ModuleRAII(ILGenPass &ILGen, il::Module *M);

      ~ModuleRAII();

   private:
      ILGenPass &ILGen;
      il::Module *savedModule;
   };

   void addDeclValuePair(NamedDecl *Decl, il::Value *Val);
   il::Value *getValueForDecl(const NamedDecl *D);

   NamedDecl *getDeclForValue(const il::Value *V)
   {
      return getDeclForValue(const_cast<il::Value*>(V));
   }

   NamedDecl *getDeclForValue(il::Value *V);

   CallableDecl *getCallableDecl(il::Function const* F);

   struct InsertPointRAII {
      InsertPointRAII(ILGenPass &ILGen)
         : ILGen(ILGen), MR(ILGen, ILGen.Builder.getModule()),
           Cleanups(std::move(ILGen.Cleanups)),
           SavedLastCleanupScope(ILGen.LastCleanupScope),
           IP(ILGen.Builder.saveIP())
      {
         new(&ILGen.Cleanups) CleanupStack(ILGen);
         ILGen.LastCleanupScope = nullptr;
      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock *IB,
                      bool KeepDebugLoc = false)
         : ILGen(ILGen), MR(ILGen, IB->getParent()->getParent()),
           Cleanups(std::move(ILGen.Cleanups)),
           SavedLastCleanupScope(ILGen.LastCleanupScope),
           IP(ILGen.Builder.saveIP())
      {
         ILGen.Builder.SetInsertPoint(IB, KeepDebugLoc);
         new(&ILGen.Cleanups) CleanupStack(ILGen);
         ILGen.LastCleanupScope = nullptr;
      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock::iterator IB)
         : ILGen(ILGen), MR(ILGen, IB->getParent()->getParent()->getParent()),
           Cleanups(std::move(ILGen.Cleanups)),
           SavedLastCleanupScope(ILGen.LastCleanupScope),
           IP(ILGen.Builder.saveIP())
      {
         ILGen.Builder.SetInsertPoint(IB);
         new(&ILGen.Cleanups) CleanupStack(ILGen);
         ILGen.LastCleanupScope = nullptr;
      }

      ~InsertPointRAII()
      {
         ILGen.Builder.restoreIP(IP);
         ILGen.Cleanups = std::move(Cleanups);
         ILGen.LastCleanupScope = SavedLastCleanupScope;
      }

   private:
      ILGenPass &ILGen;
      ModuleRAII MR;
      CleanupStack Cleanups;
      CleanupScope *SavedLastCleanupScope;
      il::ILBuilder::InsertPoint IP;
   };

   struct TerminatorRAII {
      TerminatorRAII(ILGenPass &ILGen);
      ~TerminatorRAII();

      bool hasTerminator() const { return Term != nullptr; }
      il::TerminatorInst *getTerminator() const { return Term; }

   private:
      il::ILBuilder &Builder;
      il::TerminatorInst *Term;
   };

   il::Function *getFunc(CallableDecl *C);
   il::Method *getFunc(MethodDecl *M);

   SemaPass &getSema() const { return SP; }

   friend class SemaPass;

protected:
   SemaPass &SP;

   il::ValueType makeValueType(QualType ty);

   void DeclareClassOrStruct(StructDecl *S);
   void DeclareEnum(EnumDecl *E);
   void DeclareUnion(UnionDecl *U);
   void DeclareProtocol(ProtocolDecl *cl);

   void DeclareField(FieldDecl *field);

   void DeclareProperty(PropDecl *P);
   void DefineProperty(PropDecl *P);

   void DefineFunction(CallableDecl* CD);

   void DefineLazyGlobal(il::GlobalVariable *G,
                         Expression *defaultVal);

   void FinalizeGlobalInitFn();
   void FinalizeGlobalDeinitFn();
   void VisitPotentiallyLazyGlobals();
   void VisitImportedInstantiations();

public:
   il::Context &getContext();
   il::Module *getModule();
   il::Function *getCurrentFn();

   il::Function *getPrintf();
   il::Module *getCtfeModule();
   il::Module *getUnittestModule();

   il::Module *getModuleFor(NamedDecl *ND);

   il::Function *wrapNonLambdaFunction(il::Function *F);
   il::Function *wrapNonLambdaFunction(il::Value *F);
   il::Function *getPartiallyAppliedLambda(il::Method *M, il::Value *Self);

   il::Value *getDefaultValue(QualType Ty);
   il::Value *getTuple(TupleType *Ty, llvm::ArrayRef<il::Value*> Vals);

   llvm::SmallVector<il::Argument*, 4> makeArgVec(
      llvm::ArrayRef<QualType> from);

   il::Value *LookThroughLoad(il::Value *V);

public:
   il::Value *HandleCast(const ConversionSequence &res,
                         il::Value *Val,
                         bool forced = false);

   il::Value *DoPointerArith(op::OperatorKind op,
                             il::Value *lhs, il::Value *rhs);
   il::Value *CreateLogicalAnd(il::Value *lhs, Expression *rhsNode);
   il::Value *CreateLogicalOr(il::Value *lhs, Expression *rhsNode);

   il::Value *CreateCall(CallableDecl *C,
                         llvm::ArrayRef<il::Value*> args,
                         Expression *Caller = nullptr,
                         bool DirectCall = false);

   il::Value *CreateCopy(il::Value *Val);
   il::Value *Forward(il::Value *Val);

   il::Instruction *CreateAllocBox(QualType Ty);

   il::BasicBlock *makeUnreachableBB();

   il::Value *HandleIntrinsic(CallExpr *node);

   void retainIfNecessary(il::Value *V);
   void releaseIfNecessary(il::Value *V);

   void GenerateVTable(ClassDecl *C);
   void GeneratePTable(RecordDecl *R);

   il::Value *stringify(il::Value *Val);
   il::Value *getString(const llvm::Twine &str);
   il::Constant *getConstantVal(QualType Ty, const cdot::Variant &V);

   il::Value *getCStyleArray(Type *Ty,
                             llvm::ArrayRef<il::Value*> elements,
                             size_t minCapacity = 0,
                             bool onHeap = false,
                             bool allConstant = false);

   il::Function *getGlobalInitFn();

   il::Value *CreateEqualityComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateTupleComp(il::Value *lhs, il::Value *rhs);
   il::Value *CreateEnumComp(il::Value *lhs, il::Value *rhs);

   void AppendDefaultDeinitializer(il::Method *M);
   void DefineDefaultInitializer(StructDecl *S);
   void DefineAbstractMethod(MethodDecl *M);
   void DefineMemberwiseInitializer(StructDecl *S, bool IsComplete = true);
   void DefineImplicitEquatableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitHashableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitCopyableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitStringRepresentableConformance(MethodDecl *M,
                                                     RecordDecl *R);

   void EmitCoroutinePrelude(CallableDecl *C, il::Function &F);
   il::Value *EmitCoroutineAwait(il::Value *Awaitable);
   il::Value *EmitCoroutineReturn(il::Value *Value);

   void SynthesizeGetterAndSetter(FieldDecl *F);

   void VisitTemplateInstantiations();

   il::Value *HandleUnsafeTupleGet(il::Value *tup, il::Value *idx,
                                   TupleType *Ty);

   il::Function *getBuiltin(llvm::StringRef name);

   il::TypeInfo *CreateTypeInfo(QualType ty);

   void SetTypeInfo(QualType Ty, il::GlobalVariable *GV);
   il::GlobalVariable *GetOrCreateTypeInfo(QualType ty);
   il::GlobalVariable *GetTypeInfo(QualType ty);

   il::Instruction *CreateStore(il::Value *src, il::Value *dst,
                                bool IsInitialization = false);

   const TargetInfo &getTargetInfo() const;

   void setEmitDebugInfo(bool Emit)
   {
      emitDI = Emit;
      Builder.SetEmitDebugInfo(Emit);
   }

   il::Function *CreateUnittestFun();

private:
   llvm::SmallDenseMap<QualType, il::GlobalVariable*> TypeInfoMap;
   llvm::SmallDenseMap<il::Value*, std::pair<il::Value*, size_t>> CaptureMap;

   Type *VoidTy;
   PointerType *Int8PtrTy;
   PointerType *UInt8PtrTy;
   Type *BoolTy;
   Type *DeinitializerTy;
   Type *WordTy;
   Type *USizeTy;

   il::Constant *UWordZero;
   il::Constant *UWordOne;

   il::Constant *WordZero;
   il::Constant *WordOne;

   il::Module *CTFEModule = nullptr;
   il::Module *UnittestModule = nullptr;

   StringInfo StrInfo;

   const IdentifierInfo *SelfII;

   llvm::StringMap<llvm::StringRef> BuiltinFns;

   struct BreakContinueScope {
      BreakContinueScope(il::BasicBlock *BreakTarget,
                         il::BasicBlock *ContinueTarget = nullptr,
                         const CleanupsDepth &CleanupUntil = CleanupsDepth(),
                         IdentifierInfo *Label = nullptr)
         : BreakTarget(BreakTarget), ContinueTarget(ContinueTarget),
           CleanupUntil(CleanupUntil), Label(Label)
      { }

      il::BasicBlock *BreakTarget;
      il::BasicBlock *ContinueTarget;
      CleanupsDepth CleanupUntil;
      IdentifierInfo *Label;
   };

   struct BreakContinueRAII {
      BreakContinueRAII(ILGenPass &ILGen,
                        il::BasicBlock *BreakTarget,
                        il::BasicBlock *ContinueTarget = nullptr,
                        const CleanupsDepth &CleanupUntil = CleanupsDepth(),
                        IdentifierInfo *Label = nullptr);

      ~BreakContinueRAII();

   private:
      ILGenPass &ILGen;
   };

   std::vector<BreakContinueScope> BreakContinueStack;

   std::unordered_map<NamedDecl*, il::Value*> DeclMap;
   std::unordered_map<il::Value*, NamedDecl*> ReverseDeclMap;

   std::unordered_map<VarDecl*, il::Value*> LocalDeclMap;
   std::unordered_map<il::Value*, VarDecl*> ReverseLocalDeclMap;

   std::stack<CompoundStmt*> CompoundStmtStack;

   CleanupStack Cleanups;
   bool emitDI;

   llvm::SmallPtrSet<il::GlobalVariable*, 16> NonTrivialGlobals;
   llvm::SmallPtrSet<ast::GlobalVarDecl*, 16> PotentiallyLazyGlobals;
   llvm::SmallPtrSet<MethodDecl*, 4> InstantiatedImportedMethods;

   struct CoroutineInfo {
      /// The Promise value (not the LLVM notion of promise!)
      il::Value *Awaitable = nullptr;

      /// The coroutine ID token.
      il::Value *ID = nullptr;

      /// The coroutine handle.
      il::Value *Handle = nullptr;

      /// The coroutine end basic block.
      il::BasicBlock *EndBB = nullptr;

      /// The coroutine cleanup basic block (non-final).
      il::BasicBlock *CleanupBB = nullptr;
   };

   llvm::DenseMap<il::Function*, CoroutineInfo> CoroInfoMap;

   il::PassManager MandatoryPassManager;

   /// Map from unit test declarations to their respective function.
   SmallVector<UnittestDecl*, 0> Unittests;

public:
   il::ILBuilder Builder;

   il::PassManager &getMandatoryPassManager();

   void registerInstantiatedImportedMethod(MethodDecl *M)
   {
      InstantiatedImportedMethods.insert(M);
   }

   void CreateEndCleanupBlocks(CoroutineInfo &Info);

private:
   struct EHScope {
      EHScope(il::BasicBlock *LandingPad, bool EmitCleanups = true)
         : LandingPad(LandingPad), EmitCleanups(EmitCleanups)
      { }

      il::BasicBlock *LandingPad;
      bool EmitCleanups;
   };

   std::vector<EHScope> EHStack;
   llvm::DenseSet<uintptr_t> VisitedDecls;

   struct EHScopeRAII {
      EHScopeRAII(ILGenPass &ILGen, il::BasicBlock *LP,
                  bool EmitCleanups = true)
         : ILGen(ILGen)
      {
         ILGen.EHStack.emplace_back(LP, EmitCleanups);
      }

      ~EHScopeRAII()
      {
         ILGen.EHStack.pop_back();
      }

   private:
      ILGenPass &ILGen;
   };

   template<class T>
   bool alreadyVisited(T *ptr)
   {
      return !VisitedDecls.insert((uintptr_t)ptr).second;
   }

   struct CtfeScope {
      explicit CtfeScope(CallableDecl *CurrentFn)
         : HadError(false), CurrentFn(CurrentFn)
      {}

      bool HadError;
      CallableDecl *CurrentFn;
   };

   struct EnterCtfeScope {
      EnterCtfeScope(ILGenPass &ILGen, CallableDecl *CalledFn)
         : ILGen(ILGen)
      {
         ILGen.CtfeScopeStack.emplace_back(CalledFn);
      }

      ~EnterCtfeScope();

   private:
      ILGenPass &ILGen;
   };

   CleanupScope *LastCleanupScope = nullptr;

public:
   struct CleanupRAII: public CleanupScope {
      CleanupRAII(ILGenPass &ILGen)
         : CleanupScope(ILGen.Cleanups),
           ILGen(ILGen), SavedCleanupScope(ILGen.LastCleanupScope)
      {
         ILGen.LastCleanupScope = this;
      }

      CleanupScope *getOuterCleanupScope()
      {
         return SavedCleanupScope;
      }

      ~CleanupRAII()
      {
         ILGen.LastCleanupScope = SavedCleanupScope;
      }

      CleanupsDepth getDepth() const { return depth; }

   private:
      ILGenPass &ILGen;
      CleanupScope *SavedCleanupScope;
   };

   struct ExprCleanupRAII: public ExprCleanupScope {
      ExprCleanupRAII(ILGenPass &ILGen)
         : ExprCleanupScope(ILGen.Cleanups),
           ILGen(ILGen), SavedCleanupScope(ILGen.LastCleanupScope)
      {
         ILGen.LastCleanupScope = this;
      }

      ~ExprCleanupRAII()
      {
         ILGen.LastCleanupScope = SavedCleanupScope;
      }

   private:
      ILGenPass &ILGen;
      CleanupScope *SavedCleanupScope;
   };

private:
   struct CompoundStmtRAII {
      CompoundStmtRAII(ILGenPass &ILGen, CompoundStmt *S) : ILGen(ILGen)
      {
         ILGen.CompoundStmtStack.push(S);
      }

      ~CompoundStmtRAII()
      {
         ILGen.CompoundStmtStack.pop();
      }

   private:
      ILGenPass &ILGen;
   };

   bool eraseTemporaryCleanup(il::Value *Tmp)
   {
      assert(LastCleanupScope && "no cleanup scope!");
      return LastCleanupScope->ignoreValue(Tmp);
   }

   void pushDefaultCleanup(il::Value *Val)
   {
      Cleanups.pushCleanup<DefaultCleanup>(Val);
   }

   std::vector<CtfeScope> CtfeScopeStack;

   bool PushToCtfeQueue = false;
   std::queue<ast::CallableDecl*> CtfeQueue;

   bool CanSynthesizeFunction(CallableDecl *C);

   void registerReferencedGlobal(VarDecl *Decl,
                                 il::GlobalVariable *GV,
                                 Expression *RefExpr);
public:
   bool registerCalledFunction(CallableDecl *C, StmtOrDecl Caller);
   bool inCTFE() const { return !CtfeScopeStack.empty(); }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ILGENPASS_H
