//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILGENPASS_H
#define CDOT_ILGENPASS_H

#include <stack>
#include <llvm/ADT/DenseSet.h>
#include <unordered_map>
#include <Basic/Precedence.h>

#include "AST/ASTVisitor.h"

#include "IL/ILBuilder.h"
#include "Basic/CastKind.h"

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

class ILGenPass: public ASTVisitor<il::Value*, void> {
public:
   explicit ILGenPass(il::Context &Ctx, SemaPass &SP);
   bool run();

   void outputIL();

   il::Value *visit(Expression *expr);
   il::Value *evaluateAsConstant(Expression *expr);

   void visit(Statement *node);
   void visit(Decl *D);

   bool prepareFunctionForCtfe(CallableDecl *C);
   ctfe::CTFEResult evaluateStaticExpr(Expression *expr);

   void ForwardDeclareRecord(RecordDecl* R);
   void GenerateTypeInfo(RecordDecl *R, bool innerDecls = false);

   void visitCompoundStmt(CompoundStmt *node);
   void visitNamespaceDecl(NamespaceDecl *node);

   void visitCompoundDecl(CompoundDecl *D);

   void visitDeclStmt(DeclStmt *Stmt);
   void visitLocalVarDecl(LocalVarDecl *Decl);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitLocalDestructuringDecl(LocalDestructuringDecl *node);
   void visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   void visitFunctionDecl(FunctionDecl *node);
   void visitCallableDecl(CallableDecl *node);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);
   void visitClassDecl(ClassDecl *node);
   void visitStructDecl(StructDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
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
   il::Value *visitMemberRefExpr(MemberRefExpr *Expr);
   il::Value *visitTupleMemberExpr(TupleMemberExpr *node);
   il::Value *visitEnumCaseExpr(EnumCaseExpr *node);

   void visitForStmt(ForStmt *node);
   void visitForInStmt(ForInStmt *Stmt);
   void visitWhileStmt(WhileStmt *node);
   void visitIfStmt(IfStmt *node);
   void visitLabelStmt(LabelStmt *node);
   void visitGotoStmt(GotoStmt *node);

   void visitMatchStmt(MatchStmt *node);
   void HandleSwitch(MatchStmt *node);
   void HandleEqualitySwitch(MatchStmt *node);
   void HandleIntegralSwitch(MatchStmt *node,
                             llvm::SmallVectorImpl<il::Value*> &values);

   void HandlePatternSwitch(MatchStmt *node);

   void visitCaseStmt(CaseStmt *node);

   il::Value *visitExpressionPattern(ExpressionPattern *node);
   il::Value *visitCasePattern(CasePattern *node);
   il::Value *visitIsPattern(IsPattern *node);

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

   void visitTryStmt(TryStmt *node);
   void visitThrowStmt(ThrowStmt *node);

   il::Value *visitLambdaExpr(LambdaExpr *Expr);
   il::Value *visitImplicitCastExpr(ImplicitCastExpr *node);

   void visitDebugStmt(DebugStmt *node);

   il::Value* visitStaticExpr(StaticExpr *node);
   il::Value *visitTraitsExpr(TraitsExpr *node);

   il::Function* DeclareFunction(CallableDecl *C);

   void DeclareDeclContext(DeclContext *Ctx);
   void DeclareRecord(RecordDecl *R);
   void declareRecordInstantiation(RecordDecl *Inst);

   void DeclareGlobalVariable(GlobalVarDecl *decl);
   void DeclareGlobalVariable(GlobalDestructuringDecl *decl);

   struct ModuleRAII {
      ModuleRAII(ILGenPass &ILGen, CallableDecl *C);
      ModuleRAII(ILGenPass &ILGen, il::Module *M);

      ~ModuleRAII();

   private:
      ILGenPass &ILGen;
      il::Module *savedModule;
   };

   void addDeclValuePair(NamedDecl *Decl, il::Value *Val)
   {
      DeclMap[Decl] = Val;
      ReverseDeclMap[Val] = Decl;
   }

   il::Value *getValueForDecl(NamedDecl *Stmt)
   {
      auto It = DeclMap.find(Stmt);
      if (It == DeclMap.end())
         return nullptr;

      return It->second;
   }

   NamedDecl *getDeclForValue(il::Value *V)
   {
      auto It = ReverseDeclMap.find(V);
      if (It == ReverseDeclMap.end())
         return nullptr;

      return It->second;
   }

   CallableDecl *getCallableDecl(il::Function const* F);

   struct InsertPointRAII {
      InsertPointRAII(ILGenPass &ILGen)
         : ILGen(ILGen), IP(ILGen.Builder.saveIP()),
           temporaries(std::move(ILGen.temporaries)),
           locals(std::move(ILGen.locals))
      {
         ILGen.temporaries.clear();
         while (!ILGen.locals.empty()) ILGen.locals.pop();
      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock *IB)
         : ILGen(ILGen), IP(ILGen.Builder.saveIP()),
           temporaries(std::move(ILGen.temporaries)),
           locals(std::move(ILGen.locals))
      {
         ILGen.Builder.SetInsertPoint(IB);
         ILGen.temporaries.clear();
         while (!ILGen.locals.empty()) ILGen.locals.pop();
      }

      InsertPointRAII(ILGenPass &ILGen, il::BasicBlock::iterator IB)
         : ILGen(ILGen), IP(ILGen.Builder.saveIP()),
           temporaries(std::move(ILGen.temporaries)),
           locals(std::move(ILGen.locals))
      {
         ILGen.Builder.SetInsertPoint(IB);
         ILGen.temporaries.clear();
         while (!ILGen.locals.empty()) ILGen.locals.pop();
      }

      ~InsertPointRAII()
      {
         ILGen.Builder.restoreIP(IP);
         ILGen.temporaries = std::move(temporaries);
         ILGen.locals = std::move(locals);
      }

   private:
      ILGenPass &ILGen;
      il::ILBuilder::InsertPoint IP;
      llvm::SmallPtrSet<il::Value*, 8> temporaries;
      std::stack<std::vector<il::Value*>> locals;
   };

   struct TerminatorRAII {
      TerminatorRAII(ILGenPass &ILGen);
      ~TerminatorRAII();

   private:
      il::ILBuilder &Builder;
      il::TerminatorInst *Term;
   };

   bool hasFunctionDefinition(CallableDecl *C) const;
   il::Function *getFunc(CallableDecl *C) const;
   il::Method *getFunc(MethodDecl *M) const;

   SemaPass &getSema() const
   {
      return SP;
   }

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

   void DefineFunction(il::Function *F,
                       CallableDecl* CD);

   void DefineGlobal(il::GlobalVariable *G,
                     Expression* const &defaultVal,
                     size_t ordering);

   void FinalizeGlobalInitFn();

   void doDestructure(DestructuringDecl *node);

public:
   il::Context &getContext();
   il::Module *getModule();
   il::Function *getCurrentFn();

   il::Function *getPrintf();

   il::Function *wrapNonLambdaFunction(il::Function *F);
   il::Function *getPartiallyAppliedLambda(il::Method *M, il::Value *Self);

   il::Value *getDefaultValue(QualType Ty);
   il::Value *getTuple(TupleType *Ty, llvm::ArrayRef<il::Value*> Vals);

   llvm::SmallVector<il::Argument*, 4> makeArgVec(
      llvm::ArrayRef<QualType> from);

public:
   il::Value *castTo(il::Value *V, QualType to);
   il::Value *HandleCast(const ConversionSequence &res,
                         il::Value *Val,
                         bool forced = false);

   il::Value *DoPointerArith(op::OperatorKind op,
                             il::Value *lhs, il::Value *rhs);
   il::Value *CreateLogicalAnd(il::Value *lhs, Expression *rhsNode);
   il::Value *CreateLogicalOr(il::Value *lhs, Expression *rhsNode);

   il::Instruction *CreateCall(CallableDecl *C,
                               llvm::ArrayRef<il::Value*> args,
                               Expression *Caller = nullptr);

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

   void deinitializeValue(il::Value *Val);

   void AppendDefaultDeinitializer(il::Method *M);
   void DefineDefaultInitializer(StructDecl *S);
   void DefineMemberwiseInitializer(StructDecl *S);
   void DefineImplicitEquatableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitHashableConformance(MethodDecl *M, RecordDecl *R);
   void DefineImplicitStringRepresentableConformance(MethodDecl *M,
                                                     RecordDecl *R);

   void SynthesizeGetterAndSetter(FieldDecl *F);

   void visitTemplateInstantiations();

   il::Value *HandleUnsafeTupleGet(il::Value *tup, il::Value *idx,
                                   TupleType *Ty);

   il::Function *getBuiltin(llvm::StringRef name);

   il::TypeInfo *CreateTypeInfo(QualType ty);
   il::GlobalVariable *GetTypeInfo(QualType ty);

   void deinitializeTemporaries();
   void deinitializeLocals();
   void declareLocal(il::Value *V);

   il::StoreInst *CreateStore(il::Value *src, il::Value *dst,
                              bool IsInitialization = false);

   const TargetInfo &getTargetInfo() const;

   void setEmitDebugInfo(bool Emit) { emitDI = Emit; }

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

   const IdentifierInfo *SelfII;

   llvm::StringMap<llvm::StringRef> BuiltinFns;

   struct BreakContinueScope {
      il::BasicBlock *BreakTarget;
      il::BasicBlock *ContinueTarget;
   };

   std::stack<BreakContinueScope> BreakContinueStack;

   struct UnresolvedGoto {
      il::BrInst *Inst;
      llvm::StringRef labelName;
   };

   std::stack<llvm::SmallVector<UnresolvedGoto, 2>> UnresolvedGotos;
   llvm::StringMap<il::BasicBlock*> Labels;

   llvm::SmallPtrSet<il::Value*, 8> temporaries;
   std::stack<std::vector<il::Value*>> locals;

   std::unordered_map<NamedDecl*, il::Value*> DeclMap;
   std::unordered_map<il::Value*, NamedDecl*> ReverseDeclMap;

   std::unordered_map<size_t, il::BasicBlock*> GlobalInitBBs;

public:
   il::ILBuilder Builder;

private:
   struct EHScope {
      il::LandingPadInst *LandingPad;
   };

   bool emitDI;

   std::stack<EHScope> EHStack;
   llvm::DenseSet<uintptr_t> VisitedDecls;
   llvm::SmallDenseMap<size_t, il::Module*> Modules;

   template<class T>
   bool alreadyVisited(T *ptr)
   {
      return !VisitedDecls.insert((uintptr_t)ptr).second;
   }

   struct CtfeScope {
      CtfeScope(CallableDecl *CurrentFn) : HadError(false), CurrentFn(CurrentFn)
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

      ~EnterCtfeScope()
      {
         ILGen.CtfeScopeStack.pop_back();
      }

   private:
      ILGenPass &ILGen;
   };

   std::vector<CtfeScope> CtfeScopeStack;

   void registerCalledFunction(CallableDecl *C, il::Function *F,
                               Expression *Caller);

   bool inCTFE() const { return !CtfeScopeStack.empty(); }
};

} // namespace ast
} // namespace cdot


#endif //CDOT_ILGENPASS_H
