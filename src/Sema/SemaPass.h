//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_SEMA_H
#define CDOT_SEMA_H

#include "ActionResult.h"
#include "AST/ASTContext.h"
#include "AST/ASTVisitor.h"
#include "AST/Attr.h"
#include "AST/Decl.h"
#include "AST/EmptyASTVisitor.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/StmtOrDecl.h"
#include "Basic/CastKind.h"
#include "Basic/DependencyGraph.h"
#include "Basic/Mangle.h"
#include "Basic/Precedence.h"
#include "BuiltinCandidateBuilder.h"
#include "Driver/Compiler.h"
#include "CTFE/StaticEvaluator.h"
#include "Lookup.h"
#include "Message/DiagnosticsEngine.h"
#include "Sema/CandidateSet.h"
#include "Sema/Scope/Scope.h"
#include "Sema/ConversionSequence.h"
#include "Support/Casting.h"
#include "Template.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallPtrSet.h>

#include <queue>

namespace cdot {

class QueryContext;

namespace sema {
   class ConformanceCheckerImpl;
} // namespace sema

namespace ast {

class ILGenPass;
class OverloadResolver;
class DeclContextLookupResult;
class ExprResolverImpl;
class ReflectionBuilder;
class NameBinder;

class SemaPass: public EmptyASTVisitor<ExprResult, StmtResult, DeclResult> {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;
   using MultiLevelTemplateArgList = sema::MultiLevelTemplateArgList;

   explicit SemaPass(CompilerInstance &compilationUnit);
   ~SemaPass();

   Type *getBuiltinType(DeclarationName typeName);

   DiagnosticsEngine &getDiags() { return Diags; }

   CompilerInstance &getCompilationUnit() const { return *compilationUnit; }
   void setCompilationUnit(CompilerInstance &CU) { compilationUnit = &CU; }

   ASTContext& getContext() const { return compilationUnit->getContext(); }

   DeclContext& getDeclContext() const { return *DeclCtx; }
   void setDeclContext(DeclContext &Ctx) { DeclCtx = &Ctx; }

   const SymbolMangler &getMangler() const { return mangle; }
   TemplateInstantiator &getInstantiator() const { return Instantiator; }

   void pushDeclContext(DeclContext *Ctx);
   void popDeclContext();

   bool implicitlyCastableTo(CanType from, CanType to);
   ConversionSequenceBuilder getConversionSequence(CanType from, CanType to);
   void getConversionSequence(ConversionSequenceBuilder &Seq,
                              CanType from, CanType to);

   // -1 indicates compatible signatures, positive values are error codes to
   // be used in diagnostics
   int signaturesCompatible(CallableDecl *C1, CallableDecl *C2);

   ILGenPass &getILGen() { return *ILGen.get(); }

   Statement *getParent(Statement *Child) const;
   void updateParent(Statement *Child, Statement *Parent) const;
   void createParentMap(Statement *Stmt) const;

   void diagnoseRedeclaration(DeclContext &Ctx,
                              DeclContext::AddDeclResultKind ResKind,
                              DeclarationName Name,
                              NamedDecl *Decl);

   void addDeclToContext(DeclContext &Ctx, DeclarationName Name,
                         NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, Decl *D);

   void makeDeclAvailable(DeclContext &Dst, NamedDecl *Decl,
                          bool IgnoreRedecl = false);
   void makeDeclAvailable(DeclContext &Dst,
                          DeclarationName Name,
                          NamedDecl *Decl,
                          bool IgnoreRedecl = false);

   void makeDeclsAvailableIn(DeclContext &Dst, DeclContext &Src,
                             bool IgnoreRedecl = false);

   [[nodiscard]]
   ExprResult visitExpr(StmtOrDecl DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      if (res) {
         DependentStmt.copyStatusFlags(res.get());
      }
      else {
         DependentStmt.copyStatusFlags(E);
      }

      return res;
   }

   [[nodiscard]]
   ExprResult getRValue(StmtOrDecl DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      if (res) {
         DependentStmt.copyStatusFlags(res.get());
      }
      else {
         DependentStmt.copyStatusFlags(E);
      }

      if (!res)
         return ExprError();

      return castToRValue(res.get());
   }

   [[nodiscard]]
   ExprResult getAsOrCast(StmtOrDecl DependentStmt,
                          Expression *E,
                          QualType expectedType) {
      E->setContextualType(expectedType);

      auto res = visit(E, true);
      if (res) {
         DependentStmt.copyStatusFlags(res.get());
      }
      else {
         DependentStmt.copyStatusFlags(E);
      }

      if (!res)
         return ExprError();

      // this will return a usable expression even if it fails
      return implicitCastIfNecessary(res.get(), expectedType);
   }

   [[nodiscard]]
   ExprResult visitExpr(StmtOrDecl DependentStmt,
                        Expression *E,
                        QualType contextualType) {
      E->setContextualType(contextualType);

      auto res = visit(E, true);
      if (res) {
         DependentStmt.copyStatusFlags(res.get());
      }
      else {
         DependentStmt.copyStatusFlags(E);
      }

      return res;
   }

   [[nodiscard]]
   StmtResult visitStmt(StmtOrDecl DependentStmt, Statement *Stmt)
   {
      auto res = visit(Stmt, true);
      if (res) {
         DependentStmt.copyStatusFlags(res.get());
      }
      else {
         DependentStmt.copyStatusFlags(Stmt);
      }

      return res;
   }

   [[nodiscard]]
   ExprResult visitExpr(Expression *E)
   {
      return visit(E, true);
   }

   [[nodiscard]]
   StmtResult visitStmt(Statement *Stmt)
   {
      return visit(Stmt, true);
   }

   // Initial Declarations during parsing
   void ActOnDecl(DeclContext *DC, Decl *D);
   void ActOnImportDecl(DeclContext *DC, ImportDecl *D);
   void ActOnUsingDecl(DeclContext *DC, UsingDecl *D);

   void ActOnRecordDecl(DeclContext *DC, RecordDecl* R);
   void ActOnStructDecl(DeclContext *DC, StructDecl *S);
   void ActOnProtoDecl(DeclContext *DC, ProtocolDecl *P);
   void ActOnEnumDecl(DeclContext *DC, EnumDecl *E);
   void ActOnUnionDecl(DeclContext *DC, UnionDecl *U);
   void ActOnExtensionDecl(DeclContext *DC, ExtensionDecl* Ext);

   void ActOnOperatorDecl(DeclContext *DC, OperatorDecl* Op);
   void ActOnPrecedenceGroupDecl(DeclContext *DC, PrecedenceGroupDecl* PG);

   void ActOnFunctionDecl(DeclContext *DC, FunctionDecl *F);
   void ActOnTypedefDecl(DeclContext *DC, TypedefDecl *TD);
   void ActOnAliasDecl(DeclContext *DC, AliasDecl *Alias);

   void ActOnFieldDecl(DeclContext *DC, FieldDecl *F);
   void ActOnMethodDecl(DeclContext *DC, MethodDecl *M);
   void ActOnInitDecl(DeclContext *DC, InitDecl *I);
   void ActOnDeinitDecl(DeclContext *DC, DeinitDecl *D);
   void ActOnSubscriptDecl(DeclContext *DC, SubscriptDecl *D);

   void ActOnTemplateParamDecl(DeclContext *DC,  TemplateParamDecl *P);

   void ActOnMacroExpansionDecl(DeclContext *DC, MacroExpansionDecl *Decl);
   void ActOnStaticIfDecl(DeclContext *DC, StaticIfDecl *D);
   void ActOnStaticForDecl(DeclContext *DC, StaticForDecl *D);
   void ActOnMixinDecl(DeclContext *DC, MixinDecl *D);

   StmtResult declareDebugStmt(DebugStmt *Stmt);

   void checkDeclAttrs(Decl *D, Attr::VisitationPoint VP);

#  define CDOT_DECL_ATTR(Name, Spelling)                       \
   void check##Name##Attr(Decl *D, Name##Attr *A);

#  define CDOT_STMT_ATTR(Name, Spelling)                       \
   void check##Name##Attr(Statement *S, Name##Attr *A);

#  define CDOT_EXPR_ATTR(Name, Spelling)                       \
   void check##Name##Attr(Expression *E, Name##Attr *A);

#  define CDOT_TYPE_ATTR(Name, Spelling)                       \
   void check##Name##Attr(Expression *E, Name##Attr *A);

#  include "AST/Attributes.def"

   ExprResult typecheckExpr(Expression *Expr,
                            SourceType RequiredType,
                            Statement *DependentStmt) {
      auto Result = typecheckExpr(Expr, RequiredType);
      if (Result) {
         DependentStmt->copyStatusFlags(Result.get());
      }
      else {
         DependentStmt->copyStatusFlags(Expr);
      }

      return Result;
   }

   ExprResult typecheckExpr(Expression *Expr,
                            SourceType RequiredType,
                            Decl *DependentDecl) {
      auto Result = typecheckExpr(Expr, RequiredType);
      if (Result) {
         DependentDecl->copyStatusFlags(Result.get());
      }
      else {
         DependentDecl->copyStatusFlags(Expr);
      }

      return Result;
   }

   ExprResult typecheckExpr(Expression *Expr,
                            SourceType RequiredType = ast::SourceType());

   StmtResult visitCompoundStmt(CompoundStmt *Stmt);

   StmtResult visitAttributedStmt(AttributedStmt *Stmt);
   ExprResult visitAttributedExpr(AttributedExpr *Expr);

   StmtResult visitDeclStmt(DeclStmt *Stmt);
   bool visitVarDecl(VarDecl *Decl);
   DeclResult visitLocalVarDecl(LocalVarDecl *Decl);
   DeclResult visitGlobalVarDecl(GlobalVarDecl *Decl);

   DeclResult visitDestructuringDecl(DestructuringDecl *D);

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr *Ident,
                                   TemplateArgListExpr *TemplateArgs = nullptr);

   ExprResult visitDeclRefExpr(DeclRefExpr *Expr);
   ExprResult visitMemberRefExpr(MemberRefExpr *Expr);
   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *Expr);

   ExprResult visitParenExpr(ParenExpr *Expr);
   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr *node);
   ExprResult visitSelfExpr(SelfExpr *Expr);
   ExprResult visitSuperExpr(SuperExpr *Expr);

   ExprResult visitBuiltinExpr(BuiltinExpr *node);
   ExprResult visitSubscriptExpr(SubscriptExpr *Expr);
   ExprResult visitCallExpr(CallExpr *Call,
                            TemplateArgListExpr *TemplateArgs = nullptr);
   ExprResult visitAnonymousCallExpr(AnonymousCallExpr *Call);
   ExprResult visitEnumCaseExpr(EnumCaseExpr *Expr);
   ExprResult visitTemplateArgListExpr(TemplateArgListExpr *Expr);

   ExprResult visitTupleMemberExpr(TupleMemberExpr *Expr);

   StmtResult visitForStmt(ForStmt *Stmt);
   StmtResult visitForInStmt(ForInStmt *Stmt);
   StmtResult visitWhileStmt(WhileStmt *Stmt);

   void visitIfConditions(Statement *Stmt,
                          MutableArrayRef<IfCondition> Conditions);
   StmtResult visitIfStmt(IfStmt *Stmt);

   StmtResult visitMatchStmt(MatchStmt *Stmt);
   StmtResult visitCaseStmt(CaseStmt *Stmt, MatchStmt *Match = nullptr);

   void visitPatternExpr(Statement *DependentStmt,
                         PatternExpr *E,
                         Expression *MatchVal);

   ExprResult visitExpressionPattern(ExpressionPattern *node,
                                     Expression *MatchVal = nullptr);
   ExprResult visitCasePattern(CasePattern *Expr,
                               Expression *MatchVal = nullptr);
   ExprResult visitIsPattern(IsPattern *node,
                             Expression *MatchVal = nullptr);

   StmtResult visitDiscardAssignStmt(DiscardAssignStmt *Stmt);

   StmtResult visitReturnStmt(ReturnStmt *Stmt);
   StmtResult visitBreakStmt(BreakStmt *Stmt);
   StmtResult visitContinueStmt(ContinueStmt *Stmt);

   ExprResult visitIntegerLiteral(IntegerLiteral *Expr);
   ExprResult visitFPLiteral(FPLiteral *Expr);
   ExprResult visitBoolLiteral(BoolLiteral *Expr);
   ExprResult visitCharLiteral(CharLiteral *Expr);

   ExprResult visitDictionaryLiteral(DictionaryLiteral *Expr);
   ExprResult visitArrayLiteral(ArrayLiteral *Expr);

   ExprResult visitNoneLiteral(NoneLiteral *Expr);
   ExprResult visitStringLiteral(StringLiteral *Expr);
   ExprResult visitStringInterpolation(StringInterpolation *Expr);
   ExprResult visitTupleLiteral(TupleLiteral *Expr);

   ExprResult visitExprSequence(ExprSequence *Expr);
   ExprResult visitTypePredicateExpr(TypePredicateExpr *Pred);
   ExprResult visitBinaryOperator(BinaryOperator *BinOp);
   ExprResult visitAssignExpr(AssignExpr *Expr);
   ExprResult visitUnaryOperator(UnaryOperator *UnOp);
   ExprResult visitIfExpr(IfExpr *Expr);

   ExprResult visitCastExpr(CastExpr *Cast);
   ExprResult visitAddrOfExpr(AddrOfExpr *Expr);

   StmtResult visitDoStmt(DoStmt *Stmt);
   StmtResult visitThrowStmt(ThrowStmt *Stmt);
   ExprResult visitTryExpr(TryExpr *Expr);
   ExprResult visitAwaitExpr(AwaitExpr *Expr);

   DeclResult visitFuncArgDecl(FuncArgDecl *Decl);

   ExprResult visitLambdaExpr(LambdaExpr *Expr);
   ExprResult visitImplicitCastExpr(ImplicitCastExpr *node);

   TypeResult resolveArrayTypeExpr(Statement *DependentExpr,
                                   const SourceType &ElementType,
                                   StaticExpr *SizeExpr);

   ExprResult visitTypeExpr(TypeExpr *E);
   ExprResult visitFunctionTypeExpr(FunctionTypeExpr *Expr);
   ExprResult visitTupleTypeExpr(TupleTypeExpr *Expr);
   ExprResult visitArrayTypeExpr(ArrayTypeExpr *Expr);
   ExprResult visitDeclTypeExpr(DeclTypeExpr *Expr);
   ExprResult visitPointerTypeExpr(PointerTypeExpr *Expr);
   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr *Expr);
   ExprResult visitOptionTypeExpr(OptionTypeExpr *Expr);
   ExprResult visitExistentialTypeExpr(ExistentialTypeExpr *Expr);

   StmtResult visitNullStmt(NullStmt *stmt);

   StmtResult visitDebugStmt(DebugStmt *Stmt);

   StmtResult visitStaticIfStmt(StaticIfStmt *Stmt);
   StmtResult visitStaticForStmt(StaticForStmt *Stmt);

   DeclResult visitStaticAssertDecl(StaticAssertDecl *Decl);
   DeclResult visitStaticPrintDecl(StaticPrintDecl *Decl);

   ExprResult visitStaticExpr(StaticExpr *Expr);
   ExprResult visitTraitsExpr(TraitsExpr *Expr);

   ExprResult visitMixinExpr(MixinExpr *Expr);
   StmtResult visitMixinStmt(MixinStmt *Stmt);

   Expression *resolveMacroExpansionExpr(MacroExpansionExpr *Expr);
   ExprResult visitMacroExpansionExpr(MacroExpansionExpr *Expr);
   StmtResult visitMacroExpansionStmt(MacroExpansionStmt *Stmt);

   TypeResult visitSourceType(Decl *D, const SourceType &Ty,
                              bool WantMeta = false);
   TypeResult visitSourceType(Statement *S, const SourceType &Ty,
                              bool WantMeta = false);
   TypeResult visitSourceType(const SourceType &Ty, bool WantMeta = false);

   // disallow passing an rvalue as second parameter
   template<class T>
   TypeResult visitSourceType(T *D, SourceType &&Ty) = delete;

   template<class T>
   TypeResult visitSourceType(SourceType &&Ty) = delete;

   struct StaticExprResult {
      explicit StaticExprResult(Expression *Expr, il::Constant *V)
         : Expr(Expr), Value(V), HadError(false)
      {}

      StaticExprResult(Expression *Expr = nullptr, bool typeDependent = false)
         : Expr(Expr), HadError(!typeDependent)
      {}

      il::Constant *getValue()
      {
         assert(!hadError());
         return Value;
      }

      Expression *getExpr() const
      {
         return Expr;
      }

      bool hadError() const
      {
         return HadError;
      }

      bool isDependent() const
      {
         return !Value && !HadError;
      }

      operator bool() const { return !hadError() && !isDependent(); }

   private:
      Expression *Expr = nullptr;
      il::Constant *Value = nullptr;
      bool HadError;
   };

   StaticExprResult evalStaticExpr(StmtOrDecl DependentStmt, Expression *expr);
   StaticExprResult evaluateAs(StmtOrDecl DependentStmt, Expression *expr,
                               QualType Ty);

   Optional<bool> evaluateAsBool(StmtOrDecl DependentStmt, Expression *expr);

   QualType getTypeForDecl(NamedDecl *ND);

   // returns the index of the failed constraint, or -1 if all are successful
   // or dependent

   struct ConstraintResult {
      enum Dependence : unsigned {
         NotDependent, TypeDependent, ValueDependent,
         TypeAndValueDependent,
      };

      ConstraintResult()
      {
         ValuePair.setPointer(nullptr);
         ValuePair.setInt(NotDependent);
      }

      ConstraintResult(StaticExpr *E)
      {
         ValuePair.setPointer(E);
      }

      ConstraintResult(bool TypeDep, bool ValueDep)
      {
         unsigned Dep = NotDependent;
         Dep |= TypeDep;
         Dep |= ValueDep << 1;

         ValuePair.setInt((Dependence)Dep);
      }

      StaticExpr *getFailedConstraint() const
      {
         return ValuePair.getPointer();
      }

      bool isTypeDependent() const
      {
         return (ValuePair.getInt() & TypeDependent) != 0;
      }

      bool isValueDependent() const
      {
         return (ValuePair.getInt() & ValueDependent) != 0;
      }

      bool isDependent() const
      {
         return ValuePair.getInt() != 0;
      }

   private:
      llvm::PointerIntPair<StaticExpr*, 2, Dependence> ValuePair;
   };

   void visitConstraints(NamedDecl *ConstrainedDecl);

   ConstraintResult checkConstraints(StmtOrDecl DependentDecl,
                                     NamedDecl *ConstrainedDecl,
                                     const sema::TemplateArgList &templateArgs,
                                     DeclContext *Ctx = nullptr);

   ConstraintResult checkConstraints(StmtOrDecl DependentDecl,
                                     NamedDecl *ConstrainedDecl,
                                     DeclContext *Ctx = nullptr);

   ConstraintResult checkConstraints(StmtOrDecl DependentDecl,
                                     NamedDecl *ConstrainedDecl,
                                     ArrayRef<StaticExpr*> Constraints,
                                     DeclContext *Ctx = nullptr);

   bool checkDeclConstraint(NamedDecl *ConstrainedDecl,
                            QualType ConstrainedType,
                            DeclConstraint *C);

   void printConstraint(llvm::raw_ostream &OS,
                        QualType ConstrainedType,
                        DeclConstraint *C);

   bool checkConstraint(StmtOrDecl DependentDecl,
                        Expression *Constraint);

   bool getStringValue(Expression *Expr,
                       il::Constant *V,
                       llvm::StringRef &Str);

   bool getBoolValue(Expression *Expr,
                     il::Constant *V,
                     bool &Val);

   void checkIfTypeUsableAsDecl(SourceType Ty, StmtOrDecl DependentDecl);

   IdentifierRefExpr *wouldBeValidIdentifier(SourceLocation Loc,
                                             IdentifierInfo *maybeIdent,
                                             bool LHSOfAssignment = false);

   template<class T, class ...Args>
   T *makeStmt(Args&& ...args)
   {
      return new (getContext()) T(std::forward<Args&&>(args)...);
   }

   std::pair<DeclContext*, bool> getAsContext(QualType Ty);
   NamedDecl *getTypeDecl(QualType Ty);

   bool warnOnUnusedResult(Expression *E) const;

   template<class ...Args>
   void diagnose(Statement *Stmt, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         Stmt->setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(Decl *D, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         D->setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(StmtOrDecl SOD, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         SOD.setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(diag::MessageKind msg, Args const&...args)
   {
      if (diag::isError(msg)) {
         if (currentScope)
            currentScope->setHadError(true);
      }

      {
         diag::DiagnosticBuilder Builder(Diags, msg);
         addDiagnosticArgs(Builder, std::forward<Args const &>(args)...);
      }

      if (!diag::isNote(msg))
         noteInstantiationContext();
   }

   void noteInstantiationContext();

   void diagnoseTemplateArgErrors(NamedDecl *Template,
                                  Statement *ErrorStmt,
                                  TemplateArgList &list,
                                  llvm::ArrayRef<Expression*> OriginalArgs,
                                  sema::TemplateArgListResult &Cand);

   bool equivalent(TemplateParamDecl *p1, TemplateParamDecl *p2);

   void registerInstantiation(NamedDecl *Inst, NamedDecl *Scope)
   {
      InstScopeMap[Inst] = Scope;
   }

   NamedDecl *getInstantiationScope(NamedDecl *Inst);

   void visitDelayedDecl(Decl *ND);
   void checkVirtualOrOverrideMethod(MethodDecl *M);
   void checkIfAbstractMethodsOverridden(ClassDecl *R);

   void diagnoseCircularlyDependentGlobalVariables(Expression *Expr,
                                                   NamedDecl *globalVar);

   enum class Stage {
      Parsing = 0,
      Declaration,
      Sema,
      Finalization,
      ILGen,
   };

   Stage getStage() const { return stage; }
   CallableDecl *getCurrentFun() const;

   template <std::size_t StrLen>
   IdentifierInfo *getIdentifier(const char (&Str)[StrLen])
   {
      return &Context.getIdentifiers().get(Str);
   }

   struct SemaState {
      explicit SemaState(SemaPass &SP)
         : StateBits(SP.StateUnion),
           EvaluatingGlobalVar(SP.EvaluatingGlobalVar),
           S(SP.currentScope), DoScopeStack(move(SP.DoScopeStack)),
           TryScopeStack(move(SP.TryScopeStack)),
           ReferencedATs(SP.ReferencedATs)
      {
      }

      uint8_t StateBits;
      GlobalVarDecl *EvaluatingGlobalVar;
      Scope *S;

      std::vector<bool> DoScopeStack;
      std::vector<bool> TryScopeStack;

      SmallVectorImpl<AssociatedTypeDecl*> *ReferencedATs;
   };

   SemaState getSemaState() { return SemaState(*this); }

   void resetState(SemaState &State)
   {
      StateUnion = State.StateBits;
      EvaluatingGlobalVar = State.EvaluatingGlobalVar;
      currentScope = State.S;
      DoScopeStack = move(State.DoScopeStack);
      TryScopeStack = move(State.TryScopeStack);
      ReferencedATs = State.ReferencedATs;
   }

   void clearState()
   {
      StateUnion = 0;
      EvaluatingGlobalVar = nullptr;
      currentScope = nullptr;

      DoScopeStack.clear();
      TryScopeStack.clear();
      ReferencedATs = nullptr;
   }

   struct DiagnosticScopeRAII {
      explicit DiagnosticScopeRAII(SemaPass &SP, bool Disabled = false)
         : SP(SP), fatalError(SP.fatalError),
           fatalErrorInScope(SP.fatalErrorInScope),
           encounteredError(SP.EncounteredError),
           Disabled(Disabled), State(SP.Diags.saveState()),
           numDiags(SP.getNumDiags())
      {
         SP.fatalError = false;
         SP.fatalErrorInScope = false;
         SP.EncounteredError = false;
      }

      void disable() { Disabled = true; }

      ~DiagnosticScopeRAII()
      {
         if (!Disabled) {
            SP.fatalError = fatalError;
            SP.fatalErrorInScope = fatalErrorInScope;
            SP.EncounteredError = encounteredError;
            SP.Diags.restoreState(State);
            SP.resizeDiags(numDiags);
         }
      }

   private:
      SemaPass &SP;
      bool fatalError : 1;
      bool fatalErrorInScope : 1;
      bool encounteredError : 1;
      bool Disabled : 1;

      DiagnosticsEngine::DiagState State;
      size_t numDiags;
   };

   struct DiagConsumerRAII {
      DiagConsumerRAII(SemaPass &SP, DiagnosticConsumer *PrevConsumer);
      ~DiagConsumerRAII();

   protected:
      SemaPass &SP;
      DiagnosticConsumer *PrevConsumer;
   };

   struct IgnoreDiagsRAII: DiagConsumerRAII {
      explicit IgnoreDiagsRAII(SemaPass &SP, bool Enabled = true);
      ~IgnoreDiagsRAII();

   private:
      bool Enabled;
   };

   struct DeclScopeRAII {
      DeclScopeRAII(SemaPass &SP, DeclContext *Ctx);

      ~DeclScopeRAII()
      {
         SP.DeclCtx = declContext;
         SP.resetState(State);
      }

   private:
      SemaPass &SP;
      DeclContext *declContext;
      SemaState State;
   };

   struct ScopeResetRAII {
      explicit ScopeResetRAII(SemaPass &SP, Scope *S = nullptr)
         : SP(SP), State(SP.getSemaState())
      {
         SP.clearState();
         SP.currentScope = S;
      }

      ~ScopeResetRAII()
      {
         SP.resetState(State);
      }

   private:
      SemaPass &SP;
      SemaState State;
   };

   enum SetParentCtxDiscrim {
      SetParentContext
   };

   struct DeclContextRAII {
      DeclContextRAII(SemaPass &SP, DeclContext *Ctx);
      DeclContextRAII(SemaPass &SP, DeclContext *Ctx, SetParentCtxDiscrim);

      ~DeclContextRAII()
      {
         SP.DeclCtx = Prev;
      }

   private:
      SemaPass &SP;
      DeclContext *Prev;
   };

   struct ArgPackExpansionRAII {
      ArgPackExpansionRAII(SemaPass &SP, bool IsExpansion)
         : SP(SP), Previous(SP.Bits.AllowUnexpandedParameterPack)
      {
         SP.Bits.AllowUnexpandedParameterPack |= IsExpansion;
      }

      ~ArgPackExpansionRAII()
      {
         SP.Bits.AllowUnexpandedParameterPack = Previous;
      }

   private:
      SemaPass &SP;
      bool Previous;
   };

   struct AllowIncompleteTemplateTypeRAII {
      AllowIncompleteTemplateTypeRAII(SemaPass &SP)
         : SP(SP), Previous(SP.Bits.AllowIncompleteTemplateTypes)
      {
         SP.Bits.AllowIncompleteTemplateTypes = true;
      }

      ~AllowIncompleteTemplateTypeRAII()
      {
         SP.Bits.AllowIncompleteTemplateTypes = Previous;
      }

   private:
      SemaPass &SP;
      bool Previous;
   };

   struct DefaultArgumentValueRAII {
      DefaultArgumentValueRAII(SemaPass &SP)
         : SP(SP), Previous(SP.Bits.InDefaultArgumentValue)
      {
         SP.Bits.InDefaultArgumentValue = true;
      }

      ~DefaultArgumentValueRAII()
      {
         SP.Bits.InDefaultArgumentValue = Previous;
      }

   private:
      SemaPass &SP;
      bool Previous;
   };

   struct EnterGlobalVarScope {
      EnterGlobalVarScope(SemaPass &SP, GlobalVarDecl *V)
         : SP(SP), Prev(SP.EvaluatingGlobalVar)
      {
         SP.EvaluatingGlobalVar = V;
      }

      ~EnterGlobalVarScope()
      {
         SP.EvaluatingGlobalVar = Prev;
      }

   private:
      SemaPass &SP;
      GlobalVarDecl *Prev;
   };

   struct EnterCtfeScope {
      explicit EnterCtfeScope(SemaPass &SP) : SP(SP), previous(SP.Bits.InCTFE)
      {
         SP.Bits.InCTFE = true;
      }

      ~EnterCtfeScope()
      {
         SP.Bits.InCTFE = previous;
      }

   private:
      SemaPass &SP;
      bool previous;
   };

   Scope *getScope() const { return currentScope; }
   bool inCTFE()     const { return Bits.InCTFE; }
   bool allowIncompleteTemplateTypes() const
   { return Bits.AllowIncompleteTemplateTypes; }

   bool hasDefaultValue(QualType type) const;

   void issueDiagnostics();

   size_t getNumDiags() const;
   void resizeDiags(size_t toSize);

   bool encounteredError() const { return EncounteredError; }
   void setEncounteredError(bool b) { EncounteredError = b; }

   size_t getNumGlobals() const { return numGlobals; }

   // For setting some quick access flags to compilation options.
   friend CompilerInstance;

private:
   /// Pointer to the compiler instance this Sema object belongs to.
   CompilerInstance *compilationUnit;

public:
   /// Convenience reference to the global query context.
   QueryContext &QC;

private:
   /// Current diagnostic consumer. Default behaviour is to store the
   /// diagnostics and emit them when Sema is destructed.
   std::unique_ptr<DiagnosticConsumer> DiagConsumer;

   /// Diagnostics Engine for this Sema object.
   DiagnosticsEngine Diags;

public:
   /// Reference to the compilation's AST context.
   ASTContext &Context;

private:
   /// Compilation stage.
   Stage stage = Stage::Parsing;

   /// Pointer to the current declaration context we're in.
   DeclContext *DeclCtx;

   /// Pointer to the current compound scope we're in.
   Scope *currentScope = nullptr;

   /// Next scope ID to assign.
   unsigned NextScopeID = 1;

   /// Declarations that could not be handled yet, or need special treatment
   /// at a later stage of compilation.
   SmallVector<Decl*, 0> DelayedDecls;

   /// Macro expansions that were encountered at the top level and need to be
   /// expanded after parsing.
   std::queue<MacroExpansionDecl*> TopLevelMacros;

   /// Instantiations that have been declared but not yet visited.
   SmallVector<std::pair<StmtOrDecl, NamedDecl*>, 0> DelayedInstantiations;

   /// Set of method declarations that fulfill protocol requirements.
   llvm::SmallVector<MethodDecl*, 4> ProtocolImplementations;

   /// True iff runtime generics are enabled. Will be set by the
   /// CompilerInstance.
   bool RuntimeGenerics = false;

public:
   /// Functions whose body is queued for instantiation.
   llvm::SetVector<CallableDecl*> QueuedInstantiations;

private:
   struct UnresolvedPredecenceGroup {
      const IdentifierInfo *Name;
      ModuleDecl *InModule;

      bool operator==(const UnresolvedPredecenceGroup &rhs) const
      {
         return Name == rhs.Name &&
                InModule == rhs.InModule;
      }

      bool operator!=(const UnresolvedPredecenceGroup &rhs) const
      {
         return !(rhs == *this);
      }
   };

   /// Dependency graph for Precedence groups. Detects circular higherThan /
   /// lowerThan relationships
   DependencyGraph<UnresolvedPredecenceGroup> PrecedenceDependency;

   /// Symbol mangler instance.
   SymbolMangler mangle;

   /// Static expression evaluator instance.
   StaticEvaluator Evaluator;

   /// Candidate builder instance.
   BuiltinCandidateBuilder CandBuilder;

   /// Instantiator instance.
   mutable TemplateInstantiator Instantiator;

   /// Function declarations marked extern C with a particular name.
   llvm::DenseMap<IdentifierInfo*, CallableDecl*> ExternCFuncs;

   /// Stack of do / catch scopes.
   std::vector<bool> DoScopeStack;

   /// Stack of try scopes.
   std::vector<bool> TryScopeStack;

public:
   struct TryScopeRAII {
      explicit TryScopeRAII(SemaPass &SP) : SP(SP)
      {
         SP.TryScopeStack.emplace_back(false);
      }

      ~TryScopeRAII()
      {
         SP.TryScopeStack.pop_back();
      }

      bool containsThrowingCall() const
      {
         return SP.TryScopeStack.back();
      }

   private:
      SemaPass &SP;
   };

   struct DoScopeRAII {
      explicit DoScopeRAII(SemaPass &SP, bool Exhaustive) : SP(SP)
      {
         SP.DoScopeStack.emplace_back(Exhaustive);
      }

      ~DoScopeRAII()
      {
         SP.DoScopeStack.pop_back();
      }

   private:
      SemaPass &SP;
   };

   struct EvaluatingRAII {
      EvaluatingRAII(Decl *D) : D(D)
      {
         D->setBeingEvaluated(true);
      }

      ~EvaluatingRAII()
      {
         D->setBeingEvaluated(false);
      }

   private:
      Decl *D;
   };

   struct UnittestRAII {
      explicit UnittestRAII(SemaPass &SP) : SP(SP)
      {
         SP.Bits.InUnitTest = true;
      }

      ~UnittestRAII()
      {
         SP.Bits.InUnitTest = false;
      }

   private:
      SemaPass &SP;
   };

   struct CoroutineInfo {
      /// The promise / awaitable type.
      QualType AwaitableType;

      /// The awaiter type.
      QualType AwaiterType;

      /// The awaited type.
      QualType AwaitedType;

      /// Default initializer for the awaitable.
      InitDecl *AwaitableInit = nullptr;

      /// The method to obtain the awaiter from the awaitable.
      MethodDecl *AwaitableGetAwaiter = nullptr;

      /// The method to resolve the awaitable with a value.
      MethodDecl *AwaitableResolve = nullptr;

      /// The awaiter 'awaitSuspend' method.
      MethodDecl *AwaitSuspend = nullptr;

      /// The awaiter 'awaitResume' method.
      MethodDecl *AwaitResume = nullptr;

      /// The awaiter 'ready' method.
      MethodDecl *AwaitReady = nullptr;

      /// The coroutine handle initializer.
      InitDecl *CoroHandleInit = nullptr;
   };

   const CoroutineInfo &getCoroutineInfo(CallableDecl *C);
   const CoroutineInfo &getCoroutineInfo(QualType Ty)
   {
      return CoroutineInfoMap[Ty];
   }

   void collectCoroutineInfo(QualType Ty, StmtOrDecl D);

private:
   /// Information about a particular type's coroutine implementation.
   llvm::DenseMap<QualType, CoroutineInfo> CoroutineInfoMap;

   /// Global declarations that we visited.
   llvm::SmallPtrSet<Decl*, 8> VisitedGlobalDecls;

   /// Number of global variables encountered.
   size_t numGlobals = 0;

   /// ILGen instance, owned by Sema.
   std::unique_ptr<ILGenPass> ILGen;

   /// True when we encountered a fatal error.
   bool fatalError         : 1;

   /// True when we encountered a fatal error in the current scope.
   bool fatalErrorInScope  : 1;

   /// True when we encountered any error.
   bool EncounteredError   : 1;

   struct StateBits {
      bool InCTFE                       : 1;
      bool AllowUnexpandedParameterPack : 1;
      bool AllowIncompleteTemplateTypes : 1;
      bool InDefaultArgumentValue       : 1;
      bool InUnitTest                   : 1;
   };

   union {
      uint8_t StateUnion = 0;
      StateBits Bits;
   };

   /// Current associated type substitution.
   RecordDecl *AssociatedTypeSubst = nullptr;

   /// Referenced associated types in the current declaration constraint.
   SmallVectorImpl<AssociatedTypeDecl*> *ReferencedATs = nullptr;

   /// Global variable we're currently evaluating.
   GlobalVarDecl *EvaluatingGlobalVar = nullptr;

   /// Builtin declarations.

   EnumDecl   *GenericArgumentValueDecl = nullptr;
   StructDecl *GenericArgumentDecl      = nullptr;
   StructDecl *GenericEnvironmentDecl   = nullptr;

public:
   enum class InitializableByKind {
      Integer = 0, Float, GraphemeCluster, ASCII, CodePoint,
      Bool, String, Array, Dictionary, None, _Last
   };

private:
   ProtocolDecl *InitializableBy[(int)InitializableByKind::_Last] = { nullptr };
   InitDecl *StringInit = nullptr;
   MethodDecl *StringPlusEqualsString = nullptr;

   /// Reflection builder instance, lazily initialized.
   ReflectionBuilder *ReflBuilder = nullptr;

   /// Map from instantiations to the scope in which they were instantiated
   llvm::DenseMap<NamedDecl*, NamedDecl*> InstScopeMap;

public:
   /// The unknown any type, here for convenience.
   QualType UnknownAnyTy;

   /// The error type, here for convenience.
   QualType ErrorTy;

   Module *getStdModule();
   Module *getPreludeModule();
   Module *getBuiltinModule();
   Module *getReflectModule();
   Module *getSysModule();
   Module *getRuntimeModule();
   Module *getAsyncModule();
   Module *getTestModule();

   FunctionDecl *getPureVirtualDecl();
   FunctionDecl *getCopyClassDecl();
   FunctionDecl *getAtomicReleaseDecl();

   StructDecl *getArrayDecl();
   StructDecl *getArrayViewDecl();
   ClassDecl *getDictionaryDecl();
   StructDecl *getStringDecl();
   StructDecl *getStringViewDecl();
   EnumDecl *getOptionDecl();
   StructDecl *getBoxDecl();
   ClassDecl *getPromiseDecl();
   ClassDecl *getFutureDecl();
   StructDecl *getCoroutineHandleDecl();

   StructDecl *getIntDecl();
   StructDecl *getUIntDecl();

   StructDecl *getInt8Decl();
   StructDecl *getUInt8Decl();
   StructDecl *getInt16Decl();
   StructDecl *getUInt16Decl();
   StructDecl *getInt32Decl();
   StructDecl *getUInt32Decl();
   StructDecl *getInt64Decl();
   StructDecl *getUInt64Decl();
   StructDecl *getInt128Decl();
   StructDecl *getUInt128Decl();

   StructDecl *getFloatDecl();
   StructDecl *getDoubleDecl();

   StructDecl *getUnsafePtrDecl();
   StructDecl *getUnsafeMutablePtrDecl();

   StructDecl *getUnsafeRawPtrDecl();
   StructDecl *getUnsafeMutableRawPtrDecl();

   StructDecl *getUnsafeBufferPtrDecl();
   StructDecl *getUnsafeMutableBufferPtrDecl();

   StructDecl *getBoolDecl();
   StructDecl *getCharacterDecl();

   StructDecl *getTypeInfoDecl();
   StructDecl *getValueWitnessTableDecl();
   StructDecl *getProtocolConformanceDecl();
   StructDecl *getExistentialContainerDecl();

   EnumDecl   *getGenericArgumentValueDecl();
   StructDecl *getGenericArgumentDecl();
   StructDecl *getGenericEnvironmentDecl();

   ProtocolDecl *getAnyDecl();
   ProtocolDecl *getEquatableDecl();
   ProtocolDecl *getHashableDecl();
   ProtocolDecl *getCopyableDecl();
   ProtocolDecl *getMoveOnlyDecl();
   ProtocolDecl *getImplicitlyCopyableDecl();
   ProtocolDecl *getStringRepresentableDecl();
   ProtocolDecl *getRawRepresentableDecl();
   ProtocolDecl *getPersistableDecl();
   ProtocolDecl *getAwaiterDecl();
   ProtocolDecl *getAwaitableDecl();

   ProtocolDecl *getInitializableByDecl(InitializableByKind Kind);

   PrecedenceGroupDecl *getDefaultPrecedenceGroup();

   InitDecl *getStringInit();
   MethodDecl *getStringPlusEqualsString();

   bool isInReflectModule(Decl *D);
   bool isInBuiltinModule(Decl *D);
   bool isInStdModule(Decl *D);

   QualType getOptionOf(QualType Ty, StmtOrDecl DependentStmt);

   ExprResult visit(Expression *Expr, bool);
   StmtResult visit(Statement *node, bool);

   RecordDecl *getCurrentRecordCtx();
   bool inTemplate();
   bool inUnboundedTemplate();
   bool isInDependentContext();

   void checkProtocolExtension(ExtensionDecl *Ext, ProtocolDecl *P);

   bool IsEquatableType(QualType Ty);
   bool IsCopyableType(QualType Ty);
   bool IsMoveOnlyType(QualType Ty);
   bool IsNoOpCopyableType(QualType Ty);
   bool IsImplicitlyCopyableType(QualType Ty);
   bool IsPersistableType(QualType Ty);
   bool NeedsRetainOrRelease(QualType Ty);
   bool NeedsDeinitilization(QualType Ty);
   bool NeedsStructReturn(QualType Ty);

   bool ContainsAssociatedTypeConstraint(QualType Ty);

   int inferLambdaArgumentTypes(LambdaExpr *LE, QualType fromTy);

   NamedDecl *getCurrentDecl() const;

   QualType getStaticForValue(IdentifierInfo *name) const;

   //===-------------------------------------------------------===//
   // CTFE
   //===-------------------------------------------------------===//

   bool prepareGlobalForCtfe(VarDecl *Decl);

   void makeExtensionVisible(ExtensionDecl *D);

   void addProtocolImplementation(MethodDecl *Impl)
   {
      ProtocolImplementations.push_back(Impl);
   }

private:
   template<class T, class ...Args>
   void addDiagnosticArgs(diag::DiagnosticBuilder &diag,
                          T const &first,
                          Args const &... args) {
      diag << first;
      addDiagnosticArgs(diag, args...);
   }

   void addDiagnosticArgs(diag::DiagnosticBuilder &diag) {}

public:
   struct ScopeGuard {
      enum Status {
         Enabled,
         Disabled
      };

      explicit ScopeGuard(SemaPass &S,
                          Status st = Enabled,
                          CompoundStmt *CS = nullptr);

      ScopeGuard(SemaPass &S,
                 CallableDecl *F,
                 bool InferrableReturnType = false) : S(S) {
         S.currentScope = new (S.getContext())
            FunctionScope(F, InferrableReturnType, S.currentScope);
      }

      ScopeGuard(SemaPass &S,
                 MethodDecl *M,
                 bool InferrableReturnType = false) : S(S) {
         S.currentScope = new (S.getContext())
            MethodScope(M, InferrableReturnType, S.currentScope);
      }

      ScopeGuard(SemaPass &S, LambdaExpr *L) : S(S)
      {
         S.currentScope = new (S.getContext())
            LambdaScope(L, S.currentScope);
      }

      ScopeGuard(SemaPass &S,
                 IdentifierInfo *elementName,
                 QualType elementTy) : S(S) {
         S.currentScope =
            new (S.getContext()) StaticForScope(elementName, elementTy,
                                                S.currentScope);
      }

      ScopeGuard(SemaPass &S, bool continuable, bool breakable,
                 bool isLastCase = false, bool nextCaseHasArgs = false) : S(S)
      {
         S.currentScope = new (S.getContext()) LoopScope(continuable, breakable,
                                                         isLastCase,
                                                         nextCaseHasArgs,
                                                         S.currentScope);
      }

      ~ScopeGuard()
      {
         if (enabled)
            S.currentScope = S.currentScope->getEnclosingScope();
      }

   private:
      SemaPass &S;
      bool enabled = true;
   };

   template<class T>
   T *getSpecificScope() const
   {
      for (auto S = currentScope; S; S = S->getEnclosingScope())
         if (auto t = support::dyn_cast<T>(S))
            return t;

      return nullptr;
   }

   Scope *getCurrentScope() const { return currentScope; }

   FunctionScope *getFuncScope() const
   {
      return getSpecificScope<FunctionScope>();
   }

   MethodScope *getMethodScope() const
   {
      return getSpecificScope<MethodScope>();
   }

   LambdaScope *getLambdaScope() const
   {
      return getSpecificScope<LambdaScope>();
   }

   LoopScope *getLoopScope() const
   {
      return getSpecificScope<LoopScope>();
   }

   BlockScope *getBlockScope() const
   {
      return getSpecificScope<BlockScope>();
   }

   CandidateSet
   lookupFunction(DeclContext *Ctx,
                  DeclarationName name,
                  Expression *SelfArg,
                  ArrayRef<Expression*> args,
                  ArrayRef<Expression*> templateArgs = {},
                  ArrayRef<IdentifierInfo*> labels = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false);

   CandidateSet
   lookupFunction(DeclarationName name,
                  Expression *SelfArg,
                  ArrayRef<Expression*> args,
                  ArrayRef<Expression*> templateArgs = {},
                  ArrayRef<IdentifierInfo*> labels = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false);

   CandidateSet getCandidates(DeclarationName name,
                              Expression *SelfExpr);

   CandidateSet lookupCase(DeclarationName name,
                           EnumDecl *E,
                           ArrayRef<Expression*> args,
                           ArrayRef<Expression*> templateArgs = {},
                           ArrayRef<IdentifierInfo*> labels = {},
                           Statement *Caller = nullptr,
                           bool suppressDiags = false);

   void lookupFunction(CandidateSet &CandSet,
                       DeclarationName name,
                       ArrayRef<Expression*> args,
                       ArrayRef<Expression*> templateArgs = {},
                       ArrayRef<IdentifierInfo*> labels = {},
                       Statement *Expr = nullptr,
                       bool suppressDiags = false);

   void lookupFunction(CandidateSet &CandSet,
                       DeclarationName name,
                       Expression *SelfArg,
                       ArrayRef<Expression*> args,
                       ArrayRef<Expression*> templateArgs = {},
                       ArrayRef<IdentifierInfo*> labels = {},
                       Statement *Expr = nullptr,
                       bool suppressDiags = false);

   CandidateSet checkAnonymousCall(FunctionType *FTy,
                                   ArrayRef<Expression*> args,
                                   ArrayRef<IdentifierInfo*> labels,
                                   Statement *Caller = nullptr);

   /// -1 indicates that the type cannot be returned, other values are the
   /// respective conversion penalty, if any
   int ExprCanReturn(Expression *E, CanType Ty);
   int ExprCanReturnImpl(Expression *E, CanType Ty);

   QualType ResolveContextualLambdaExpr(LambdaExpr *E, QualType Ty);
   QualType GetDefaultExprType(Expression *E);

   ExprResult visitTypeDependentContextualExpr(Expression *E);

   DeclResult doDestructure(DestructuringDecl *D,
                            QualType DestructuredTy);

   MethodDecl *getEquivalentMethod(MethodDecl *Orig,
                                   RecordDecl *Inst);

   void CheckReturnedSelfType(QualType ParentType, Expression *E);

   void maybeInstantiate(CandidateSet &CandSet, Statement *Caller);
   void maybeInstantiate(CandidateSet::Candidate &Cand,
                         Statement *Caller);

   bool maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                               const TemplateArgList &templateArgs,
                               Statement *Caller);

   template<class T>
   T *maybeInstantiateTemplateMember(DeclContext *DC, T* Member)
   {
      return support::cast<T>(maybeInstantiateTemplateMemberImpl(DC, Member));
   }

   NamedDecl *maybeInstantiateTemplateMemberImpl(DeclContext *LookupCtx,
                                                 NamedDecl *Member);

   CallableDecl *maybeInstantiateMemberFunction(CallableDecl *M,
                                                StmtOrDecl Caller,
                                                bool NeedImmediateInstantiation
                                                   = false);

   MethodDecl *InstantiateMethod(RecordDecl *R, StringRef Name, StmtOrDecl SOD);
   MethodDecl *InstantiateProperty(RecordDecl *R, StringRef Name,
                                   bool Getter, StmtOrDecl SOD);

   RecordDecl *InstantiateRecord(SourceLocation POI,
                                 RecordDecl *R,
                                 sema::FinalTemplateArgumentList *TemplateArgs);

   RecordDecl *InstantiateRecord(SourceLocation POI,
                                 RecordDecl *R,
                                 const sema::TemplateArgList &TemplateArgs);

   AliasDecl *InstantiateAlias(SourceLocation POI,
                               AliasDecl *td,
                               sema::FinalTemplateArgumentList *TemplateArgs);

   void checkDuplicateFunctionDeclaration(CallableDecl *C,
                                          llvm::StringRef fnKind);

   Expression *implicitCastIfNecessary(Expression* Expr,
                                       QualType destTy,
                                       bool ignoreError = false,
                                       diag::MessageKind msg =
                                          diag::err_type_mismatch,
                                       SourceLocation DiagLoc = {},
                                       SourceRange DiagRange = {},
                                       bool *hadError = nullptr);

   void checkDeclaredVsGivenType(Decl *DependentDecl,
                                 Expression *&val,
                                 const SourceType &ST,
                                 QualType DeclaredType,
                                 QualType GivenType,
                                 bool IsLet,
                                 SourceLocation EqualsLoc);

   // don't allow accidentally passing two QualTypes
   Expression *implicitCastIfNecessary(Expression*, QualType, QualType,
                                       diag::MessageKind
                                          = diag::err_type_mismatch) = delete;

   Expression* forceCast(Expression* Expr, QualType destTy);

   Expression *castToRValue(Expression *Expr);
   void toRValue(Expression *Expr);

public:
   CallableDecl* checkFunctionReference(
                                 Expression *E,
                                 DeclarationName funcName,
                                 const MultiLevelLookupResult &MultiLevelResult,
                                 llvm::ArrayRef<Expression*> templateArgs);

   CallableDecl* checkFunctionReference(Expression *E,
                                        CallableDecl *CD,
                                        ArrayRef<Expression*> templateArgs);

   RecordDecl* checkRecordReference(Expression *E,
                                    RecordDecl *R,
                                    ArrayRef<Expression*> templateArgs);

   AliasDecl* checkAliasReference(Expression *E,
                                  AliasDecl *Alias,
                                  ArrayRef<Expression*> templateArgs);

   struct AliasResult {
      explicit AliasResult(AliasDecl *Alias)
         : TypeDependent(false), ValueDependent(false), HadError(false),
           Result(Alias)
      {}

      AliasResult(CandidateSet &&CandSet)
         : TypeDependent(false), ValueDependent(false), HadError(true),
           CandSet(move(CandSet))
      {}

      AliasResult()
         : TypeDependent(false), ValueDependent(false), HadError(true)
      {}

      AliasResult(bool typeDependent, bool valueDependent,
                  AliasDecl *Alias = nullptr)
         : TypeDependent(typeDependent), ValueDependent(valueDependent),
           HadError(true), Result(Alias)
      {}

      bool isTypeDependent() const { return TypeDependent; }
      bool isValueDependent() const{ return ValueDependent; }

      AliasDecl *getAlias() { return Result; }
      CandidateSet &getCandSet() { return CandSet; }

      operator bool() const
      {
         return !HadError && !TypeDependent && !ValueDependent;
      }

   private:
      bool TypeDependent  : 1;
      bool ValueDependent : 1;
      bool HadError       : 1;

      CandidateSet CandSet;
      AliasDecl *Result;
   };

   AliasResult checkAlias(const MultiLevelLookupResult &MultiLevelResult,
                          llvm::ArrayRef<Expression*> templateArgs,
                          Expression *E);

   bool checkAlias(AliasDecl *alias, CandidateSet::Candidate &Cand);

   // checks whether the parent expression of the given expression refers to
   // a namespace rather than a value and adjusts the expression appropriately
   NestedNameSpecifierWithLoc *checkNamespaceRef(Expression *Expr);
   bool refersToNamespace(Expression *E);

   ExprResult checkNamespaceRef(MacroExpansionExpr *Expr);
   StmtResult checkNamespaceRef(MacroExpansionStmt *Stmt);
   DeclResult checkNamespaceRef(MacroExpansionDecl *D);

   QualType getParentType(Expression *ParentExpr);

private:
   ExprResult HandleBuiltinTypeMember(IdentifierRefExpr *Expr, QualType Ty);
   ExprResult HandleStaticTypeMember(IdentifierRefExpr *Expr, QualType Ty);

   QualType HandleFieldAccess(Expression *Expr, FieldDecl *F);
   QualType HandlePropAccess(Expression *Expr, PropDecl *P);
   ExprResult HandleEnumCase(Expression *Expr, EnumCaseDecl *Case);

   Expression *checkDeref(Expression *E, QualType T);

   // CallExpr

   ExprResult HandleStaticTypeCall(CallExpr *Call,
                                   TemplateArgListExpr *ArgExpr,
                                   CanType Ty,
                                   bool AllowProtocol = true);

   ExprResult HandleConstructorCall(CallExpr *Call,
                                    TemplateArgListExpr *ArgExpr,
                                    QualType T,
                                    const MultiLevelLookupResult *LookupRes =
                                       nullptr,
                                    TemplateParamDecl *Param = nullptr,
                                    AssociatedTypeDecl *AT = nullptr,
                                    bool AllowProtocol = true);

public:
   CallExpr *CreateCall(CallableDecl *C, ArrayRef<Expression*> Args,
                        SourceLocation Loc);
   CallExpr *CreateCall(CallableDecl *C, ASTVector<Expression*> &&Args,
                        SourceLocation Loc);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               StmtOrDecl Subject,
                               DeclarationName memberName,
                               diag::MessageKind msg
                                 = diag::err_member_not_found,
                               SourceRange SR = SourceRange());

   bool isAccessible(NamedDecl *ND);
   void checkAccessibility(NamedDecl *ND, StmtOrDecl SOD);

private:
   StmtOrDecl checkMacroCommon(StmtOrDecl SOD,
                               DeclarationName MacroName,
                               DeclContext &Ctx,
                               MacroDecl::Delimiter Delim,
                               llvm::ArrayRef<lex::Token> Tokens,
                               unsigned Kind);

   Expression *UnwrapExistential(QualType ParentType, Expression *Expr);

public:
   FuncArgDecl *MakeSelfArg(SourceLocation Loc);

   void addDependency(Decl *ReferencedDecl);
   void addDependency(NamedDecl *DependentDecl, Decl *ReferencedDecl);

   unsigned getSerializationFile(Decl *D);

   void calculateRecordSize(RecordDecl *R, bool CheckDependencies = true);
   bool finalizeRecordDecls();

   void checkDefaultAccessibility(NamedDecl *ND);

   llvm::DenseMap<DeclarationName, NamedDecl*> BuiltinDecls;

   template <std::size_t StrLen>
   NamedDecl *getBuiltinDecl(const char (&Str)[StrLen])
   {
      auto It = BuiltinDecls.find(getIdentifier(Str));
      if (It == BuiltinDecls.end())
         return nullptr;

      return It->getSecond();
   }

   template <class T, std::size_t StrLen>
   T *getBuiltinDecl(const char (&Str)[StrLen])
   {
      return support::cast_or_null<T>(getBuiltinDecl(Str));
   }

private:
   // A namespace to which the compiler can add builtin declarations without
   // conflicting with user-defined types.
   NamespaceDecl *PrivateNamespace = nullptr;

   // Builtin namespace
   const IdentifierInfo *BuiltinIdents[64];
   bool BuiltinIdentsInitialized = false;

   void initBuiltinIdents();
   ExprResult HandleBuiltinCall(CallExpr *C);

   // static reflection
   const IdentifierInfo *ReflectionIdents[64];
   bool ReflectionIdentsInitialized = false;

   llvm::DenseMap<AliasDecl*, Expression*> ReflectionValues;

   void initReflectionIdents();

public:
   ExprResult HandleReflectionAlias(AliasDecl *Al, Expression *Expr);
   ExprResult HandleReflectionCall(CallableDecl *C);

   ExprResult HandleBuiltinAlias(AliasDecl *Al, Expression *Expr);
   void SetBuiltinAliasType(AliasDecl *A);

   NamespaceDecl *getPrivateNamespace();

   friend class ReflectionBuilder;
   friend class NameBinder;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_SEMA_H
