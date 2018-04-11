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
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/StmtOrDecl.h"
#include "Basic/CastKind.h"
#include "Basic/DependencyGraph.h"
#include "Basic/Mangle.h"
#include "Basic/Precedence.h"
#include "BuiltinCandidateBuilder.h"
#include "Compiler.h"
#include "CTFE/StaticEvaluator.h"
#include "Message/DiagnosticsEngine.h"
#include "Sema/CandidateSet.h"
#include "Sema/Scope/Scope.h"
#include "Sema/ConversionSequence.h"
#include "Support/Casting.h"
#include "Template.h"
#include "TemplateInstantiator.h"

#include <cstdint>

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallPtrSet.h>

namespace cdot {

namespace sema {
   class ConformanceCheckerImpl;
} // namespace sema

namespace ast {

class ILGenPass;
class OverloadResolver;
class LookupResult;
class ExprResolverImpl;

class SemaPass: public ASTVisitor<ExprResult, StmtResult, DeclResult> {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;
   using MultiLevelTemplateArgList = sema::MultiLevelTemplateArgList;

   explicit SemaPass(CompilationUnit &compilationUnit);
   ~SemaPass();

   Type *getBuiltinType(DeclarationName typeName);

   DiagnosticsEngine &getDiags() { return Diags; }

   CompilationUnit &getCompilationUnit() const { return *compilationUnit; }
   void setCompilationUnit(CompilationUnit &CU) { compilationUnit = &CU; }

   ASTContext& getContext() const { return compilationUnit->getContext(); }

   DeclContext& getDeclContext() const { return *DeclCtx; }
   void setDeclContext(DeclContext &Ctx) { DeclCtx = &Ctx; }

   const SymbolMangler &getMangler() const { return mangle; }
   TemplateInstantiator &getInstantiator() const { return Instantiator; }

   module::Module *getModuleBeingCompiled();

   void pushDeclContext(DeclContext *Ctx);
   void popDeclContext();

   bool implicitlyCastableTo(QualType from, QualType to) const;
   ConversionSequence getConversionSequence(QualType from, QualType to) const;

   // -1 indicates compatible signatures, positive values are error codes to
   // be used in diagnostics
   int signaturesCompatible(CallableDecl *C1, CallableDecl *C2);

   bool doDeclarations();
   bool doSema();
   bool doILGen();

   StmtResult visitScoped(Statement *Stmt);
   DeclResult visitScoped(Decl *D);
   DeclResult declareScoped(Decl *D);

   ILGenPass &getILGen() { return *ILGen.get(); }

   Statement *getParent(Statement *Child) const;
   void updateParent(Statement *Child, Statement *Parent) const;
   void createParentMap(Statement *Stmt) const;

   void updateParentMapForTemplateInstantiation(Statement *Template,
                                                Statement *Inst) const;

   void diagnoseRedeclaration(DeclContext &Ctx,
                              DeclContext::AddDeclResultKind ResKind,
                              DeclarationName Name,
                              NamedDecl *Decl);

   void addDeclToContext(DeclContext &Ctx, DeclarationName Name,
                         NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, Decl *D);

   void makeDeclAvailable(DeclContext &Dst, NamedDecl *Decl);
   void makeDeclAvailable(DeclContext &Dst,
                          DeclarationName Name,
                          NamedDecl *Decl);

   void makeDeclsAvailableIn(DeclContext &Dst, DeclContext &Src);

//   void copyStatusFlags(Expression *From, Statement *To)
//   {
//      To->copyStatusFlags(From);
//      if (From->isVariadicArgPackExpansion())
//         To->setContainsUnexpandedParameterPack(false);
//   }

   [[nodiscard]]
   ExprResult visitExpr(StmtOrDecl DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      DependentStmt.copyStatusFlags(E);

      return res;
   }

   [[nodiscard]]
   ExprResult getRValue(StmtOrDecl DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      DependentStmt.copyStatusFlags(E);

      if (!res)
         return ExprError();

      auto Cast = castToRValue(res.get());
      assert(Cast->getExprType() && "lvalue to rvalue cast introduced error");

      return Cast;
   }

   [[nodiscard]]
   ExprResult getAsOrCast(StmtOrDecl DependentStmt,
                          Expression *E,
                          QualType expectedType) {
      E->setContextualType(expectedType);

      auto res = visit(E, true);
      DependentStmt.copyStatusFlags(E);

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
      DependentStmt.copyStatusFlags(E);

      return res;
   }

   [[nodiscard]]
   StmtResult visitStmt(StmtOrDecl DependentStmt, Statement *Stmt)
   {
      auto res = visit(Stmt, true);
      DependentStmt.copyStatusFlags(Stmt);

      return res;
   }

   [[nodiscard]]
   DeclResult visitStmt(StmtOrDecl DependentStmt, Decl *D)
   {
      auto res = visit(D, true);
      DependentStmt.copyStatusFlags(D);

      return res;
   }

   [[nodiscard]]
   DeclResult declareStmt(StmtOrDecl DependentStmt, Decl *Decl)
   {
      auto res = declare(Decl, true);
      DependentStmt.copyStatusFlags(Decl);

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

   [[nodiscard]]
   DeclResult visitDecl(Decl *D)
   {
      return visit(D, true);
   }

   [[nodiscard]]
   DeclResult declareStmt(Decl *Decl)
   {
      return declare(Decl, true);
   }

   // Initial Declarations during parsing

   void ActOnRecordDecl(RecordDecl* R);
   void ActOnStructDecl(StructDecl *S);
   void ActOnProtoDecl(ProtocolDecl *P);
   void ActOnEnumDecl(EnumDecl *E);
   void ActOnUnionDecl(UnionDecl *U);
   void ActOnExtensionDecl(ExtensionDecl* Ext);

   void ActOnFunctionDecl(FunctionDecl *F);
   void ActOnTypedefDecl(TypedefDecl *TD);
   void ActOnAliasDecl(AliasDecl *Alias);

   void ActOnFieldDecl(FieldDecl *F);
   void ActOnMethodDecl(MethodDecl *M);
   void ActOnInitDecl(InitDecl *I);
   void ActOnDeinitDecl(DeinitDecl *D);

   void ActOnTemplateParamDecl(DeclContext &Ctx, TemplateParamDecl *P);
   
   // Declaration pass

   bool declareDeclContext(DeclContext *Ctx);
   void transferDecls(DeclContext *From, DeclContext *To);

   DeclResult declareImportDecl(ImportDecl *Decl);
   DeclResult declareModuleDecl(ModuleDecl *Decl);

   DeclResult declareCompoundDecl(CompoundDecl *Compound);
   DeclResult declareTranslationUnit(TranslationUnit *node);

   DeclResult declareNamespaceDecl(NamespaceDecl *NS);
   DeclResult declareUsingDecl(UsingDecl *UD);

   DeclResult declareTemplateParamDecl(TemplateParamDecl *decl);

   DeclResult declareCallableDecl(CallableDecl *C);
   DeclResult declareFunctionDecl(FunctionDecl *F);
   DeclResult declareFuncArgDecl(FuncArgDecl *Decl);

   DeclResult declareLocalVarDecl(LocalVarDecl *Decl) { return {}; }

   DeclResult declareGlobalVarDecl(GlobalVarDecl *Decl);
   StmtResult declareGlobalDestructuringDecl(GlobalDestructuringDecl *Decl);

   DeclResult declareRecordDecl(RecordDecl *node);

   DeclResult declareStructDecl(StructDecl *node);
   DeclResult declareClassDecl(ClassDecl *node);
   DeclResult declareProtocolDecl(ProtocolDecl *node);
   DeclResult declareExtensionDecl(ExtensionDecl *Ext);
   DeclResult declareUnionDecl(UnionDecl *node);
   DeclResult declareEnumDecl(EnumDecl *node);

   DeclResult declareFieldDecl(FieldDecl *F);
   DeclResult declarePropDecl(PropDecl *Decl);
   DeclResult declareAssociatedTypeDecl(AssociatedTypeDecl *Decl);
   DeclResult declareEnumCaseDecl(EnumCaseDecl *node);

   DeclResult declareMethodDecl(MethodDecl *M);
   DeclResult declareInitDecl(InitDecl *Init);
   DeclResult declareDeinitDecl(DeinitDecl *Deinit);

   DeclResult declareTypedefDecl(TypedefDecl *Decl);
   DeclResult declareAliasDecl(AliasDecl *Decl);

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

   DeclResult declareMixinDecl(MixinDecl *Decl);
   void parseMixinExpr(Expression *E, llvm::StringRef Str,
                       llvm::SmallVectorImpl<Statement*> &Stmts);

   DeclResult declareStaticIfDecl(StaticIfDecl *Stmt);
   DeclResult declareStaticAssertStmt(StaticAssertStmt *Stmt);
   DeclResult declareStaticPrintStmt(StaticPrintStmt *Stmt);
   DeclResult declareStaticForDecl(StaticForDecl *node);
   
   // Sema pass

   DeclResult visitNamespaceDecl(NamespaceDecl *node);
   StmtResult visitCompoundStmt(CompoundStmt *compound);

   StmtResult visitAttributedStmt(AttributedStmt *Stmt);
   ExprResult visitAttributedExpr(AttributedExpr *Expr);

   StmtResult visitDeclStmt(DeclStmt *Stmt);

   DeclResult visitCompoundDecl(CompoundDecl *D);

   bool visitVarDecl(VarDecl *Decl);
   DeclResult visitLocalVarDecl(LocalVarDecl *Decl);
   DeclResult visitGlobalVarDecl(GlobalVarDecl *Decl);

   bool visitDestructuringDecl(DestructuringDecl *node);
   StmtResult visitLocalDestructuringDecl(LocalDestructuringDecl *node);
   StmtResult visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   DeclResult visitFunctionDecl(FunctionDecl *F);
   DeclResult visitCallableDecl(CallableDecl *CD);

   DeclResult visitRecordCommon(RecordDecl *node);
   DeclResult visitRecordDecl(RecordDecl *R);

   DeclResult visitClassDecl(ClassDecl *C);
   DeclResult visitStructDecl(StructDecl *S);
   DeclResult visitExtensionDecl(ExtensionDecl *Ext);
   DeclResult visitEnumDecl(EnumDecl *E);
   DeclResult visitUnionDecl(UnionDecl *U);
   DeclResult visitProtocolDecl(ProtocolDecl *P);

   DeclResult visitFieldDecl(FieldDecl *FD);
   DeclResult visitPropDecl(PropDecl *PD);
   DeclResult visitAssociatedTypeDecl(AssociatedTypeDecl *ATDecl);

   DeclResult visitMethodDecl(MethodDecl *M);
   DeclResult visitInitDecl(InitDecl *Init);
   DeclResult visitDeinitDecl(DeinitDecl *Deinit);

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr *Ident);
   ExprResult visitParenExpr(ParenExpr *Expr);
   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr *node);
   ExprResult visitSelfExpr(SelfExpr *Expr);
   ExprResult visitSuperExpr(SuperExpr *Expr);

   ExprResult visitBuiltinExpr(BuiltinExpr *node);
   ExprResult visitSubscriptExpr(SubscriptExpr *Expr);
   ExprResult visitCallExpr(CallExpr *Call);
   ExprResult visitMemberRefExpr(MemberRefExpr *Expr);
   ExprResult visitEnumCaseExpr(EnumCaseExpr *Expr);

   ExprResult visitTupleMemberExpr(TupleMemberExpr *Expr);

   StmtResult visitForStmt(ForStmt *Stmt);
   StmtResult visitForInStmt(ForInStmt *Stmt);
   StmtResult visitWhileStmt(WhileStmt *Stmt);
   StmtResult visitIfStmt(IfStmt *Stmt);
   StmtResult visitLabelStmt(LabelStmt *Stmt);
   StmtResult visitGotoStmt(GotoStmt *Stmt);

   StmtResult visitMatchStmt(MatchStmt *Stmt);
   StmtResult visitCaseStmt(CaseStmt *Stmt, MatchStmt *Match = nullptr);
   ExprResult visitExpressionPattern(ExpressionPattern *node);
   ExprResult visitCasePattern(CasePattern *Expr);
   ExprResult visitIsPattern(IsPattern *node);

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
   ExprResult visitUnaryOperator(UnaryOperator *UnOp);
   ExprResult visitIfExpr(IfExpr *Expr);

   ExprResult visitCastExpr(CastExpr *Cast);

   StmtResult visitTryStmt(TryStmt *Stmt);
   StmtResult visitThrowStmt(ThrowStmt *Stmt);

   DeclResult visitFuncArgDecl(FuncArgDecl *Decl);

   ExprResult visitLambdaExpr(LambdaExpr *Expr);
   ExprResult visitImplicitCastExpr(ImplicitCastExpr *node);

   TypeResult resolveArrayTypeExpr(Statement *DependentExpr,
                                   const SourceType &ElementType,
                                   StaticExpr *SizeExpr);

   ExprResult visitFunctionTypeExpr(FunctionTypeExpr *Expr);
   ExprResult visitTupleTypeExpr(TupleTypeExpr *Expr);
   ExprResult visitArrayTypeExpr(ArrayTypeExpr *Expr);
   ExprResult visitDeclTypeExpr(DeclTypeExpr *Expr);
   ExprResult visitPointerTypeExpr(PointerTypeExpr *Expr);
   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr *Expr);
   ExprResult visitOptionTypeExpr(OptionTypeExpr *Expr);

   StmtResult visitNullStmt(NullStmt *stmt);
   DeclResult visitModuleDecl(ModuleDecl *stmt);
   DeclResult visitImportDecl(ImportDecl *stmt);
   DeclResult visitEnumCaseDecl(EnumCaseDecl *stmt);
   DeclResult visitTypedefDecl(TypedefDecl *stmt);
   DeclResult visitTemplateParamDecl(TemplateParamDecl *stmt);
   DeclResult visitTranslationUnit(TranslationUnit *stmt);

   StmtResult visitDebugStmt(DebugStmt *Stmt);

   DeclResult visitAliasDecl(AliasDecl *Alias);

   StmtResult visitStaticIfStmt(StaticIfStmt *Stmt);
   StmtResult visitStaticForStmt(StaticForStmt *Decl);

   DeclResult visitStaticAssertStmt(StaticAssertStmt *Decl);
   DeclResult visitStaticPrintStmt(StaticPrintStmt *Decl);

   ExprResult visitStaticExpr(StaticExpr *Expr);
   ExprResult visitTraitsExpr(TraitsExpr *Expr);

   ExprResult visitMixinExpr(MixinExpr *Expr);
   StmtResult visitMixinStmt(MixinStmt *Stmt);
   DeclResult visitMixinDecl(MixinDecl *Decl);

   TypeResult visitSourceType(Decl *D, const SourceType &Ty);
   TypeResult visitSourceType(Statement *S, const SourceType &Ty);
   TypeResult visitSourceType(const SourceType &Ty);

   // disallow passing an rvalue as second parameter
   template<class T>
   TypeResult visitSourceType(T *D, SourceType &&Ty) = delete;

   template<class T>
   TypeResult visitSourceType(SourceType &&Ty) = delete;

   struct StaticExprResult {
      explicit StaticExprResult(Expression *Expr, Variant &&V)
         : Expr(Expr), Value(std::move(V)), HadError(false)
      {}

      StaticExprResult() : HadError(true)
      {}

      Variant &getValue()
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

      operator bool() const { return !hadError(); }

   private:
      Expression *Expr = nullptr;
      Variant Value;
      bool HadError;
   };

   StaticExprResult evalStaticExpr(Expression *expr);
   StaticExprResult evalStaticExpr(Statement *DependentStmt, Expression *expr);
   StaticExprResult evalStaticExpr(Decl *DependentDecl, Expression *expr);

   StaticExprResult evalStaticExpr(Expression *expr,
                                   TemplateArgList const& templateArgs);

   StaticExprResult evaluateAs(Expression *expr, QualType Ty);
   StaticExprResult evaluateAsBool(Expression *expr);


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

   ConstraintResult checkConstraints(SourceLocation Loc,
                                     NamedDecl *ConstrainedDecl,
                                     const sema::MultiLevelTemplateArgList
                                                               &templateArgs);

   ConstraintResult checkConstraints(SourceLocation Loc,
                           NamedDecl *ConstrainedDecl);

   bool checkConstraint(SourceLocation Loc,
                        Expression *Constraint);

   bool getStringValue(Expression *Expr,
                       Variant const &V,
                       llvm::StringRef &Str);

   bool getBoolValue(Expression *Expr,
                     Variant const &V,
                     bool &Val);

   void visitDelayedDeclsAfterParsing();
   void visitDelayedDeclsAfterDeclaration();
   void visitDelayedInstantiations();

   void addImplicitConformance(RecordDecl *R, ImplicitConformanceKind kind);

   IdentifierRefExpr *wouldBeValidIdentifier(SourceLocation Loc,
                                             IdentifierInfo *maybeIdent,
                                             bool LHSOfAssignment = false);

   void noteConstantDecl(Expression *DeclRef);

   template<class T, class ...Args>
   T *makeStmt(Args&& ...args)
   {
      return new (getContext()) T(std::forward<Args&&>(args)...);
   }

   QualType resolveDependencies(QualType Ty,
                                MultiLevelTemplateArgList const& TAs,
                                Statement *PointOfInstantiation);

   QualType resolveDependencies(QualType Ty,
                                MultiLevelTemplateArgList const& TAs,
                                Statement *PointOfInstantiation,
                                size_t variadicIx);

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
   void diagnose(diag::MessageKind msg, Args const&...args)
   {
      if (diag::isError(msg)) {
         EncounteredError = true;

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
                                  sema::TemplateArgListResult &Cand);

   void visitRecordInstantiation(StmtOrDecl DependentStmt, RecordDecl *Inst);
   void declareRecordInstantiation(StmtOrDecl DependentStmt, RecordDecl *Inst);
   void finalizeRecordInstantiation(RecordDecl *Inst);

   void visitFunctionInstantiation(StmtOrDecl DependentStmt,
                                   CallableDecl *Inst);

   void registerDelayedInstantiation(NamedDecl *Inst, StmtOrDecl POI);
   void registerTemplateParamWithDefaultVal(TemplateParamDecl *TD);

   void visitDelayedDecl(NamedDecl *ND);
   void checkVirtualOrOverrideMethod(MethodDecl *M);

   void diagnoseCircularlyDependentGlobalVariables(Expression *Expr,
                                                   NamedDecl *globalVar);

   enum class Stage {
      Declaration = 0,
      Sema,
      ILGen,
   };

   Stage getStage() const
   {
      return stage;
   }

   template<class T>
   T *lookup(llvm::StringRef name, unsigned lvl = 2) const
   {
      return support::dyn_cast_or_null<T>(lookup(name, lvl));
   }

   ast::NamedDecl *lookup(llvm::StringRef name, unsigned lvl = 2) const;

   RecordDecl *getRecord(llvm::StringRef name) const;

   DeclContext *getNearestDeclContext(Statement *Stmt) const;
   CallableDecl *getCurrentFun() const;

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

   private:
      SemaPass &SP;
      DiagnosticConsumer *PrevConsumer;
   };

   struct DeclScopeRAII {
      DeclScopeRAII(SemaPass &SP, DeclContext *Ctx)
         : SP(SP), declContext(SP.DeclCtx),
           AllowUnexpanded(SP.AllowUnexpandedParameterPack),
           AllowIncompleteTys(SP.AllowIncompleteTemplateTypes)
      {
         SP.DeclCtx = Ctx;
         SP.AllowUnexpandedParameterPack = false;
         SP.AllowIncompleteTemplateTypes = false;
      }

      ~DeclScopeRAII()
      {
         SP.DeclCtx = declContext;
         SP.AllowUnexpandedParameterPack = AllowUnexpanded;
         SP.AllowIncompleteTemplateTypes = AllowIncompleteTys;
      }

   private:
      SemaPass &SP;
      DeclContext *declContext;
      bool AllowUnexpanded    : 1;
      bool AllowIncompleteTys : 1;
   };

   struct ScopeResetRAII {
      explicit ScopeResetRAII(SemaPass &S)
         : S(S), scope(S.currentScope),
           AllowUnexpanded(S.AllowUnexpandedParameterPack),
           AllowIncompleteTys(S.AllowIncompleteTemplateTypes)
      {
         S.currentScope = nullptr;
         S.AllowUnexpandedParameterPack = false;
         S.AllowIncompleteTemplateTypes = false;
      }

      ~ScopeResetRAII()
      {
         S.currentScope = scope;
         S.AllowUnexpandedParameterPack = AllowUnexpanded;
         S.AllowIncompleteTemplateTypes = AllowIncompleteTys;
      }

   private:
      SemaPass &S;
      Scope *scope;
      bool AllowUnexpanded    : 1;
      bool AllowIncompleteTys : 1;
   };

   enum SetParentCtxDiscrim {
      SetParentContext
   };

   struct DeclContextRAII {
      DeclContextRAII(SemaPass &SP, DeclContext *Ctx);
      DeclContextRAII(SemaPass &SP, DeclContext *Ctx, SetParentCtxDiscrim);

      ~DeclContextRAII()
      {
         SP.popDeclContext();
      }

   private:
      SemaPass &SP;
   };

   struct ArgPackExpansionRAII {
      ArgPackExpansionRAII(SemaPass &SP, bool IsExpansion)
         : SP(SP), Previous(SP.AllowUnexpandedParameterPack)
      {
         SP.AllowUnexpandedParameterPack |= IsExpansion;
      }

      ~ArgPackExpansionRAII()
      {
         SP.AllowUnexpandedParameterPack = Previous;
      }

   private:
      SemaPass &SP;
      bool Previous;
   };

   struct AllowIncompleteTemplateTypeRAII {
      AllowIncompleteTemplateTypeRAII(SemaPass &SP)
         : SP(SP), Previous(SP.AllowIncompleteTemplateTypes)
      {
         SP.AllowIncompleteTemplateTypes = true;
      }

      ~AllowIncompleteTemplateTypeRAII()
      {
         SP.AllowIncompleteTemplateTypes = Previous;
      }

   private:
      SemaPass &SP;
      bool Previous;
   };

   struct AssociatedTypeSubstRAII {
      AssociatedTypeSubstRAII(SemaPass &SP, RecordDecl *R)
         : SP(SP), Prev(SP.AssociatedTypeSubst)
      {
         SP.AssociatedTypeSubst = R;
      }

      ~AssociatedTypeSubstRAII()
      {
         SP.AssociatedTypeSubst = Prev;
      }

   private:
      SemaPass &SP;
      RecordDecl *Prev;
   };

   struct InstantiationRAII {
      InstantiationRAII(SemaPass &SP,
                        DeclContext *Ctx,
                        NamedDecl *Template)
         : SP(SP), DeclRAII(SP, Ctx), ScopeRAII(SP)
      {
         SP.pushInstantiationContext(Template);
      }

      ~InstantiationRAII()
      {
         SP.popInstantiationContext();
      }

   private:
      SemaPass &SP;
      DeclScopeRAII DeclRAII;
      ScopeResetRAII ScopeRAII;
   };

   struct EnterCtfeScope {
      explicit EnterCtfeScope(SemaPass &SP) : SP(SP), previous(SP.InCTFE)
      {
         SP.InCTFE = true;
      }

      ~EnterCtfeScope()
      {
         SP.InCTFE = previous;
      }

   private:
      SemaPass &SP;
      bool previous;
   };

   Scope *getScope() const { return currentScope; }
   bool inCTFE()     const { return InCTFE; }
   bool allowIncompleteTemplateTypes() const
   { return AllowIncompleteTemplateTypes; }

   bool hasDefaultValue(QualType type) const;

   void issueDiagnostics();

   size_t getNumDiags() const;
   void resizeDiags(size_t toSize);

   bool encounteredError() const { return EncounteredError; }
   size_t getNumGlobals() const { return numGlobals; }

private:
   CompilationUnit *compilationUnit; // can't be a reference because it's a
                                     // movable type

   std::unique_ptr<DiagnosticConsumer> DiagConsumer;
   DiagnosticsEngine Diags;

   ASTContext &Context;
   DeclContext *DeclCtx;

   Stage stage = Stage::Declaration;

   Scope *currentScope = nullptr;

   llvm::SmallVector<NamedDecl*, 16> DelayedDecls;
   llvm::SmallVector<std::pair<StmtOrDecl, NamedDecl*>, 0>
      DelayedInstantiations;

   DependencyGraph<RecordDecl*> LayoutDependency;
   llvm::StringMap<GotoStmt*> UnresolvedGotos;
   
   SymbolMangler mangle;
   StaticEvaluator Evaluator;
   BuiltinCandidateBuilder CandBuilder;
   mutable TemplateInstantiator Instantiator;

   llvm::StringSet<> labels;
   llvm::DenseSet<uintptr_t> DeclaredDecls;
   llvm::DenseSet<uintptr_t> VisitedDecls;

   std::vector<NamedDecl*> InstantiationStack;

   llvm::SmallPtrSet<Decl*, 8> VisitedGlobalDecls;
   size_t numGlobals = 0;

   std::unique_ptr<ILGenPass> ILGen;

   bool fatalError                   : 1;
   bool fatalErrorInScope            : 1;
   bool EncounteredError             : 1;
   bool InCTFE                       : 1;
   bool AllowUnexpandedParameterPack : 1;
   bool AllowIncompleteTemplateTypes : 1;

   RecordDecl *AssociatedTypeSubst = nullptr;

   QualType UnknownAnyTy;

   ClassDecl *ArrayDecl      = nullptr;
   ClassDecl *DictionaryDecl = nullptr;
   ClassDecl *StringDecl     = nullptr;
   StructDecl *StringViewDecl = nullptr;
   EnumDecl *OptionDecl     = nullptr;
   StructDecl *TypeInfoDecl   = nullptr;

   ProtocolDecl *AnyDecl                 = nullptr;
   ProtocolDecl *EquatableDecl           = nullptr;
   ProtocolDecl *HashableDecl            = nullptr;
   ProtocolDecl *StringRepresentableDecl = nullptr;

public:
   ClassDecl *getArrayDecl();
   ClassDecl *getDictionaryDecl();
   ClassDecl *getStringDecl();
   StructDecl *getStringViewDecl();
   EnumDecl *getOptionDecl();
   StructDecl *getTypeInfoDecl();

   ProtocolDecl *getAnyDecl();
   ProtocolDecl *getEquatableDecl();
   ProtocolDecl *getHashableDecl();
   ProtocolDecl *getStringRepresentableDecl();

   ExprResult visit(Expression *Expr, bool);
   StmtResult visit(Statement *node, bool);
   DeclResult visit(Decl *D, bool);

   DeclResult declare(Decl *Decl, bool);

   RecordDecl *getCurrentRecordCtx();
   bool inTemplate();

   void pushInstantiationContext(NamedDecl *Decl)
   {
      InstantiationStack.push_back(Decl);
   }

   void popInstantiationContext()
   {
      InstantiationStack.pop_back();
   }

   bool stopEvaluating(Expression *Expr);
   bool stopEvaluating(Statement *Stmt);

   void checkConformances();
   void checkConformances(DeclContext *Ctx);
   void checkProtocolConformance(RecordDecl *R);

   int inferLambdaArgumentTypes(LambdaExpr *LE, QualType fromTy);

   NamedDecl *getCurrentDecl() const;
   QualType getStaticForValue(IdentifierInfo *name) const;

   bool ensureSizeKnown(QualType Ty, SourceLocation loc);
   bool ensureSizeKnown(RecordDecl *R, SourceLocation loc);
   bool ensureDeclared(Decl *D);
   bool ensureVisited(Decl *D);
   bool prepareFunctionForCtfe(CallableDecl *Fn);

   DeclResult declareAndVisit(Decl *D);

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

      explicit ScopeGuard(SemaPass &S, Status st = Enabled)
         : S(S), enabled(st == Enabled)
      {
         if (enabled)
            S.currentScope = new (S.getContext()) BlockScope(S.currentScope);
      }

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
         S.currentScope = new (S.getContext()) LambdaScope(L, S.currentScope);
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
                  llvm::ArrayRef<Expression*> args,
                  llvm::ArrayRef<Expression*> templateArgs = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false,
                  bool includesSelf = false);

   CandidateSet
   lookupFunction(DeclarationName name,
                  llvm::ArrayRef<Expression*> args,
                  llvm::ArrayRef<Expression*> templateArgs = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false,
                  bool includesSelf = false);

   CandidateSet
   lookupMethod(DeclarationName name,
                Expression *SelfExpr,
                llvm::ArrayRef<Expression*> args,
                llvm::ArrayRef<Expression*> templateArgs = {},
                Statement *Caller = nullptr,
                bool suppressDiags = false);

   CandidateSet getCandidates(DeclarationName name,
                              Expression *SelfExpr);

   CandidateSet
   lookupCase(DeclarationName name,
              EnumDecl *E,
              llvm::ArrayRef<Expression*> args,
              llvm::ArrayRef<Expression*> templateArgs = {},
              Statement *Caller = nullptr,
              bool suppressDiags = false);

   void lookupFunction(CandidateSet &CandSet,
                       DeclarationName name,
                       llvm::ArrayRef<Expression*> args,
                       llvm::ArrayRef<Expression*> templateArgs = {},
                       Statement *Caller = nullptr,
                       bool suppressDiags = false);

   CandidateSet checkAnonymousCall(FunctionType *FTy,
                                   llvm::ArrayRef<Expression*> args,
                                   Statement *Caller = nullptr);

   bool ExprCanReturn(Expression *E, QualType Ty);

   void maybeInstantiate(CandidateSet &CandSet, Statement *Caller);
   bool maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                               TemplateArgList &&templateArgs,
                               Statement *Caller);

   void maybeInstantiateMemberFunction(MethodDecl *M, StmtOrDecl Caller);

   void declareMemberwiseInitializer(StructDecl *S);
   void declareDefaultInitializer(StructDecl *S);
   void declareDefaultDeinitializer(RecordDecl *R);

   TemplateParamDecl* getTemplateParam(DeclarationName name);
   AssociatedTypeDecl* getAssociatedType(DeclarationName name);

   bool inGlobalDeclContext() const;

   void checkDuplicateFunctionDeclaration(CallableDecl *C,
                                          llvm::StringRef fnKind);

   bool alreadyVisited(Statement *stmt)
   {
      return !VisitedDecls.insert((uintptr_t)stmt).second;
   }

   bool alreadyVisited(Decl *D)
   {
      return !VisitedDecls.insert((uintptr_t)D).second;
   }

   bool alreadyDeclared(Decl *D)
   {
      return !DeclaredDecls.insert((uintptr_t)D).second;
   }

   bool isDeclared(Decl *D)
   {
      return DeclaredDecls.find((uintptr_t)D) != DeclaredDecls.end();
   }

   bool isVisited(Decl *D)
   {
      return VisitedDecls.find((uintptr_t)D) != VisitedDecls.end();
   }

   Expression *implicitCastIfNecessary(Expression* Expr,
                                       QualType destTy,
                                       bool ignoreError = false,
                                       diag::MessageKind msg
                                                    =
                                       diag::err_type_mismatch,
                                       SourceLocation DiagLoc = {},
                                       SourceRange DiagRange = {});

   // don't allow accidentally passing two QualTypes
   Expression *implicitCastIfNecessary(Expression*, QualType, QualType,
                                       diag::MessageKind
                                          = diag::err_type_mismatch) = delete;

   Expression* forceCast(Expression* Expr, QualType destTy);

   Expression *castToRValue(Expression *Expr);
   void toRValue(Expression *Expr);

private:
   // IdentifierRef

   CallableDecl* checkFunctionReference(Expression *E,
                                        DeclContext *Ctx,
                                        DeclarationName funcName,
                                       llvm::ArrayRef<Expression*>templateArgs);

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

      AliasResult(bool typeDependent, bool valueDependent)
         : TypeDependent(typeDependent), ValueDependent(valueDependent),
           HadError(true)
      {}

      bool isTypeDependent() const
      {
         return TypeDependent;
      }

      bool isValueDependent() const
      {
         return ValueDependent;
      }

      AliasDecl *getAlias()
      {
         assert(!HadError && !TypeDependent && !ValueDependent);
         return Result;
      }

      CandidateSet &getCandSet()
      {
         return CandSet;
      }

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

   AliasResult checkAlias(LookupResult &lookupResult,
                          llvm::ArrayRef<Expression*> templateArgs,
                          SourceLocation loc);

   bool checkAlias(AliasDecl *alias, CandidateSet::Candidate &Cand);

   // checks whether the parent expression of the given expression refers to
   // a namespace rather than a value and adjusts the expression appropriately
   ExprResult checkNamespaceRef(Expression *Expr);

   // MemberRef

   ExprResult HandleBuiltinTypeMember(MemberRefExpr *Expr, QualType Ty);
   ExprResult HandleStaticTypeMember(MemberRefExpr *Expr, QualType Ty);

   ExprResult HandleFieldAccess(IdentifierRefExpr *Ident, FieldDecl *F);
   ExprResult HandlePropAccess(IdentifierRefExpr *Ident, PropDecl *P);

   ExprResult HandleEnumCase(IdentifierRefExpr *node, EnumDecl *E);

   // CallExpr

   ExprResult HandleFunctionCall(CallExpr* Call,
                                 DeclContext *Ctx,
                                 DeclarationName funcName);

   ExprResult HandleBuiltinCall(CallExpr*);

   ExprResult HandleMethodCall(CallExpr *Call, Expression *ParentExpr);
   ExprResult HandleStaticTypeCall(CallExpr *Call, Type *Ty);

   ExprResult HandleConstructorCall(CallExpr*, RecordDecl *R);
   ExprResult HandleCallOperator(CallExpr *node, Expression *ParentExpr);
   ExprResult HandleAnonCall(CallExpr *node, Expression *ParentExpr);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               Statement *Subject,
                               DeclarationName memberName,
                               diag::MessageKind msg
                                 = diag::err_undeclared_identifer);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               Decl *Subject,
                               DeclarationName memberName,
                               diag::MessageKind msg
                               = diag::err_undeclared_identifer);

   void checkDefaultAccessibility(NamedDecl *ND);
   void checkAccessibility(NamedDecl *ND, Expression* Expr);

public:
   void ApplyCasts(llvm::MutableArrayRef<Expression*> args,
                   Expression *DependentExpr,
                   CandidateSet &CandSet);

   void PrepareCallArgs(std::vector<Expression*>& args,
                        CallableDecl *C);

   void calculateRecordSize(RecordDecl *R);
   void finalizeRecordDecls();

private:
   // static reflection
   const IdentifierInfo *ReflectionIdents[10];
   bool ReflectionIdentsInitialized = false;

   void initReflectionIdents();

   ExprResult HandleReflectionAlias(AliasDecl *Al, Expression *Expr);
   ExprResult HandleReflectionCall(CallableDecl *C);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_SEMA_H
