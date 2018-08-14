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
#include "ConformanceChecker.h"
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
#include <llvm/ADT/SmallPtrSet.h>

#include <queue>

namespace cdot {

namespace sema {
   class ConformanceCheckerImpl;
} // namespace sema

namespace ast {

class ILGenPass;
class OverloadResolver;
class DeclContextLookupResult;
class ExprResolverImpl;
class ReflectionBuilder;

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

   bool implicitlyCastableTo(QualType from, QualType to);
   ConversionSequenceBuilder getConversionSequence(QualType from, QualType to);
   void getConversionSequence(ConversionSequenceBuilder &Seq,
                              QualType from, QualType to);

   // -1 indicates compatible signatures, positive values are error codes to
   // be used in diagnostics
   int signaturesCompatible(CallableDecl *C1, CallableDecl *C2);

   bool doDeclarations();
   bool doSema();
   bool doILGen();

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

   void makeDeclAvailable(DeclContext &Dst, NamedDecl *Decl,
                          bool IgnoreRedecl = false);
   void makeDeclAvailable(DeclContext &Dst,
                          DeclarationName Name,
                          NamedDecl *Decl,
                          bool IgnoreRedecl = false);

   void makeDeclsAvailableIn(DeclContext &Dst, DeclContext &Src,
                             bool IgnoreRedecl = false);

   MultiLevelLookupResult MultiLevelLookup(DeclContext &Ctx,
                                           DeclarationName Name,
                                           bool ExternalLookup = true,
                                           bool LocalLookup = true,
                                           bool LookInExtensions = true);

   SingleLevelLookupResult Lookup(DeclContext &Ctx,
                                  DeclarationName Name,
                                  bool ExternalLookup = true,
                                  bool LookInExtensions = true);

   DeclContextLookupResult LookupOwn(DeclContext &Ctx,
                                     DeclarationName Name,
                                     bool ExternalLookup = true,
                                     bool LookInExtensions = true);

   template <class T>
   T *LookupSingle(DeclContext &Ctx,
                   DeclarationName Name,
                   bool ExternalLookup = true) {
      auto Result = MultiLevelLookup(Ctx, Name, ExternalLookup);
      if (Result.empty() || Result.front().size() != 1)
         return nullptr;

      return support::dyn_cast<T>(Result.front().front());
   }

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
   void ActOnDecl(DeclContext *Ctx, Decl *D, bool Global = false);
   void ActOnImportDecl(ImportDecl *D, bool Global = false);
   void ActOnUsingDecl(UsingDecl *D, bool Global = false);

   void ActOnRecordDecl(RecordDecl* R);
   void ActOnStructDecl(StructDecl *S);
   void ActOnProtoDecl(ProtocolDecl *P);
   void ActOnEnumDecl(EnumDecl *E);
   void ActOnUnionDecl(UnionDecl *U);
   void ActOnExtensionDecl(ExtensionDecl* Ext);

   void ActOnOperatorDecl(OperatorDecl* Op);
   void ActOnPrecedenceGroupDecl(PrecedenceGroupDecl* PG);

   void ActOnFunctionDecl(FunctionDecl *F);
   void ActOnTypedefDecl(TypedefDecl *TD);
   void ActOnAliasDecl(AliasDecl *Alias);

   void ActOnFieldDecl(FieldDecl *F);
   void ActOnMethodDecl(MethodDecl *M);
   void ActOnInitDecl(InitDecl *I);
   void ActOnDeinitDecl(DeinitDecl *D);

   void ActOnTemplateParamDecl(DeclContext &Ctx, TemplateParamDecl *P);

   void ActOnMacroExpansionDecl(MacroExpansionDecl *Decl, bool Global = false);
   void ActOnStaticIfDecl(StaticIfDecl *D, bool Global = false);
   void ActOnStaticForDecl(StaticForDecl *D, bool Global = false);
   void ActOnMixinDecl(MixinDecl *D, bool Global = false);
   
   // Declaration pass

   bool declareDeclContext(DeclContext *Ctx);
   void declareImmediateDecls(RecordDecl *R,
                              DependencyGraph<NamedDecl*> &LayoutDependency);

   void visitDeclContext(DeclContext *Ctx);

   void transferDecls(DeclContext *From, DeclContext *To);

   DeclResult declareDebugDecl(DebugDecl *D);

   DeclResult declareImportDecl(ImportDecl *Decl);
   DeclResult declareModuleDecl(ModuleDecl *Decl);

   DeclResult declareCompoundDecl(CompoundDecl *Compound);

   DeclResult declareMacroDecl(MacroDecl *Decl) { return Decl; }
   DeclResult declareMacroExpansionDecl(MacroExpansionDecl *Decl);

   DeclResult declarePrecedenceGroupDecl(PrecedenceGroupDecl *Decl);
   DeclResult declareOperatorDecl(OperatorDecl *Decl);

   DeclResult declareNamespaceDecl(NamespaceDecl *NS);
   DeclResult declareUnittestDecl(UnittestDecl *D);
   DeclResult declareUsingDecl(UsingDecl *UD);

   DeclResult declareTemplateParamDecl(TemplateParamDecl *decl);

   DeclResult declareCallableDecl(CallableDecl *C);
   DeclResult declareFunctionDecl(FunctionDecl *F);
   DeclResult declareFuncArgDecl(FuncArgDecl *Decl);

   DeclResult declareLocalVarDecl(LocalVarDecl *Decl) { return Decl; }
   DeclResult declareGlobalVarDecl(GlobalVarDecl *Decl);
   DeclResult declareDestructuringDecl(DestructuringDecl *Decl) { return Decl; }

   DeclResult declareRecordDecl(RecordDecl *node);

   DeclResult declareStructDecl(StructDecl *node);
   DeclResult declareClassDecl(ClassDecl *node);
   DeclResult declareProtocolDecl(ProtocolDecl *node);
   DeclResult declareExtensionDecl(ExtensionDecl *Ext);
   DeclResult declareUnionDecl(UnionDecl *node);
   DeclResult declareEnumDecl(EnumDecl *node);

   DeclResult declareFieldDecl(FieldDecl *F);
   DeclResult declareFieldDeclImmediate(
      FieldDecl *F,
      DependencyGraph<NamedDecl*> *LayoutDependency = nullptr);

   DeclResult declarePropDecl(PropDecl *Decl);
   DeclResult declareSubscriptDecl(SubscriptDecl *Decl);
   DeclResult declareAssociatedTypeDecl(AssociatedTypeDecl *Decl);

   DeclResult declareEnumCaseDecl(EnumCaseDecl *node);
   DeclResult declareEnumCaseDeclImmediate(
      EnumCaseDecl *node,
      DependencyGraph<NamedDecl*> &LayoutDependency);

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

   DeclResult visitDebugDecl(DebugDecl *D);

   DeclResult visitNamespaceDecl(NamespaceDecl *node);
   DeclResult visitUnittestDecl(UnittestDecl *D);

   StmtResult visitCompoundStmt(CompoundStmt *Stmt);

   StmtResult visitAttributedStmt(AttributedStmt *Stmt);
   ExprResult visitAttributedExpr(AttributedExpr *Expr);

   StmtResult visitDeclStmt(DeclStmt *Stmt);
   DeclResult visitUsingDecl(UsingDecl *UD);

   DeclResult visitCompoundDecl(CompoundDecl *D);

   bool visitVarDecl(VarDecl *Decl);
   DeclResult visitLocalVarDecl(LocalVarDecl *Decl);
   DeclResult visitGlobalVarDecl(GlobalVarDecl *Decl);

   DeclResult visitDestructuringDecl(DestructuringDecl *D);

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
   DeclResult visitSubscriptDecl(SubscriptDecl *SD);
   DeclResult visitAssociatedTypeDecl(AssociatedTypeDecl *ATDecl);

   DeclResult visitMethodDecl(MethodDecl *M);
   DeclResult visitInitDecl(InitDecl *Init);
   DeclResult visitDeinitDecl(DeinitDecl *Deinit);

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr *Ident,
                                   TemplateArgListExpr *TemplateArgs = nullptr);

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

   StmtResult visitNullStmt(NullStmt *stmt);
   DeclResult visitModuleDecl(ModuleDecl *Mod);
   DeclResult visitImportDecl(ImportDecl *stmt);
   DeclResult visitEnumCaseDecl(EnumCaseDecl *stmt);
   DeclResult visitTypedefDecl(TypedefDecl *stmt);
   DeclResult visitTemplateParamDecl(TemplateParamDecl *stmt);

   StmtResult visitDebugStmt(DebugStmt *Stmt);

   DeclResult visitAliasDecl(AliasDecl *Alias);

   StmtResult visitStaticIfStmt(StaticIfStmt *Stmt);
   StmtResult visitStaticForStmt(StaticForStmt *Stmt);

   DeclResult visitStaticAssertStmt(StaticAssertStmt *Decl);
   DeclResult visitStaticPrintStmt(StaticPrintStmt *Decl);

   ExprResult visitStaticExpr(StaticExpr *Expr);
   ExprResult visitTraitsExpr(TraitsExpr *Expr);

   ExprResult visitMixinExpr(MixinExpr *Expr);
   StmtResult visitMixinStmt(MixinStmt *Stmt);
   DeclResult visitMixinDecl(MixinDecl *Decl);

   ExprResult visitMacroExpansionExpr(MacroExpansionExpr *Expr);
   StmtResult visitMacroExpansionStmt(MacroExpansionStmt *Stmt);

   TypeResult visitSourceType(Decl *D, const SourceType &Ty,
                              bool WantMeta = false);
   TypeResult visitSourceType(Statement *S, const SourceType &Ty,
                              bool WantMeta = false);
   TypeResult visitSourceType(const SourceType &Ty, bool WantMeta = false);

   using ConformanceSet = SmallPtrSetImpl<ProtocolDecl*>;

   void registerExplicitConformances(
      RecordDecl *Rec,
      ConformanceSet *AddedConformances = nullptr);

   void registerExplicitConformances(
      RecordDecl *Rec,
      ArrayRef<SourceType> ConfTypes,
      ConformanceSet *AddedConformances = nullptr);

   void registerImplicitAndInheritedConformances(
      RecordDecl *Rec,
      ConformanceSet *AddedConformances = nullptr);

   void registerImplicitAndInheritedConformances(
      RecordDecl *Rec,
      ArrayRef<SourceType> ConfTypes,
      ConformanceSet *AddedConformances = nullptr);

   // disallow passing an rvalue as second parameter
   template<class T>
   TypeResult visitSourceType(T *D, SourceType &&Ty) = delete;

   template<class T>
   TypeResult visitSourceType(SourceType &&Ty) = delete;

   struct StaticExprResult {
      explicit StaticExprResult(Expression *Expr, il::Constant *V)
         : Expr(Expr), Value(V), HadError(false)
      {}

      StaticExprResult(Expression *Expr = nullptr)
         : Expr(Expr), HadError(true)
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

      operator bool() const { return !hadError(); }

   private:
      Expression *Expr = nullptr;
      il::Constant *Value = nullptr;
      bool HadError;
   };

   StaticExprResult evalStaticExpr(StmtOrDecl DependentStmt, Expression *expr);
   StaticExprResult evaluateAs(StmtOrDecl DependentStmt, Expression *expr,
                               QualType Ty);
   StaticExprResult evaluateAsBool(StmtOrDecl DependentStmt, Expression *expr);


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

   bool resolvePrecedenceGroups();
   bool visitDelayedDeclsAfterParsing();
   bool visitDelayedDeclsAfterDeclaration();
   bool visitProtocolImplementations();
   bool visitDelayedInstantiations();

   bool calculateRecordSizes(ArrayRef<RecordDecl*> Decls);
   MethodDecl *addImplicitConformance(RecordDecl *R,
                                      ImplicitConformanceKind kind);

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

   QualType resolveDependencies(QualType Ty,
                                const MultiLevelTemplateArgList& TAs,
                                StmtOrDecl PointOfInstantiation);

   QualType resolveDependencies(QualType Ty,
                                const sema::MultiLevelFinalTemplateArgList& TAs,
                                StmtOrDecl PointOfInstantiation);

   QualType resolveNestedNameSpecToType(NestedNameSpecifierWithLoc *Name,
                                        bool Diagnose = true,
                                        StmtOrDecl SOD = {});

   NamedDecl *resolveNestedNameSpecToDecl(NestedNameSpecifierWithLoc *Name,
                                          bool Diagnose = true,
                                          StmtOrDecl SOD = {});

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

   void visitRecordInstantiation(StmtOrDecl DependentStmt, RecordDecl *Inst);
   void declareRecordInstantiation(StmtOrDecl DependentStmt, RecordDecl *Inst);
   void finalizeRecordInstantiation(RecordDecl *Inst);

   void visitFunctionInstantiation(StmtOrDecl DependentStmt,
                                   CallableDecl *Inst);

   void registerDelayedInstantiation(NamedDecl *Inst, StmtOrDecl POI);
   void registerTemplateParamWithDefaultVal(TemplateParamDecl *TD);

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

private:
   /// Pointer to the compiler instance this Sema object belongs to.
   CompilerInstance *compilationUnit;

   /// Current diagnostic consumer. Default behaviour is to store the
   /// diagnostics and emit them when Sema is destructed.
   std::unique_ptr<DiagnosticConsumer> DiagConsumer;

   /// Diagnostics Engine for this Sema object.
   DiagnosticsEngine Diags;

   /// Reference to the compilation's AST context.
   ASTContext &Context;

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

public:
   /// Dependency graph that tracks struct / enum / tuple layouts and finds
   /// circular dependencies.
   DependencyGraph<NamedDecl*> LayoutDependency;

   /// Dependency graph for protocol conformances. Detects circular
   /// conformances.
   DependencyGraph<ProtocolDecl*> ConformanceDependency;

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
   struct TypeMeta {
      llvm::Optional<bool> Persistable;
      llvm::Optional<bool> MoveOnly;
      llvm::Optional<bool> ImplicitlyCopyable;
      llvm::Optional<bool> SelfEquatable;
      llvm::Optional<bool> NeedsRetainOrRelease;
      llvm::Optional<bool> NeedsDeinitilization;
      llvm::Optional<bool> NeedsStructReturn;
      llvm::Optional<bool> ContainsAssociatedTypeConstraint;
   };

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
   /// Additional information about types that is lazily built.
   llvm::DenseMap<QualType, TypeMeta> TypeMetaMap;

   /// Information about a particular type's coroutine implementation.
   llvm::DenseMap<QualType, CoroutineInfo> CoroutineInfoMap;

   /// Stack of current instantiations.
   std::vector<NamedDecl*> InstantiationStack;

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

   /// True if we need to track declarations per file (i.e. if we're doing
   /// incremential compilation
   bool TrackDeclsPerFile : 1;

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

   Module *StdModule     = nullptr;
   Module *PreludeModule = nullptr;
   Module *BuiltinModule = nullptr;
   Module *ReflectModule = nullptr;
   Module *SysModule     = nullptr;
   Module *RuntimeModule = nullptr;
   Module *AsyncModule   = nullptr;
   Module *TestModule    = nullptr;

   FunctionDecl *PureVirtual = nullptr;

   StructDecl  *ArrayDecl     = nullptr;
   StructDecl *ArrayViewDecl  = nullptr;
   ClassDecl  *DictionaryDecl = nullptr;
   StructDecl  *StringDecl    = nullptr;
   StructDecl *StringViewDecl = nullptr;
   EnumDecl   *OptionDecl     = nullptr;
   StructDecl *BoxDecl        = nullptr;
   ClassDecl *PromiseDecl     = nullptr;
   ClassDecl *FutureDecl      = nullptr;
   StructDecl *CoroHandleDecl = nullptr;

   StructDecl *TypeInfoDecl             = nullptr;
   StructDecl *ValueWitnessTableDecl    = nullptr;
   StructDecl *ProtocolConformanceDecl  = nullptr;
   StructDecl *ExistentialContainerDecl = nullptr;

   ProtocolDecl *AnyDecl                 = nullptr;
   ProtocolDecl *EquatableDecl           = nullptr;
   ProtocolDecl *HashableDecl            = nullptr;
   ProtocolDecl *CopyableDecl            = nullptr;
   ProtocolDecl *MoveOnlyDecl            = nullptr;
   ProtocolDecl *ImplicitlyCopyableDecl  = nullptr;
   ProtocolDecl *StringRepresentableDecl = nullptr;
   ProtocolDecl *PersistableDecl         = nullptr;
   ProtocolDecl *AwaiterDecl             = nullptr;
   ProtocolDecl *AwaitableDecl           = nullptr;

   enum class InitializableByKind {
      Integer, Float, Char, Bool, String, Array,
   };

   ProtocolDecl *InitializableBy[6] = { nullptr };

   PrecedenceGroupDecl *DefaultPrecedenceGroup = nullptr;

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

   StructDecl *getTypeInfoDecl();
   StructDecl *getValueWitnessTableDecl();
   StructDecl *getProtocolConformanceDecl();
   StructDecl *getExistentialContainerDecl();

   ProtocolDecl *getAnyDecl();
   ProtocolDecl *getEquatableDecl();
   ProtocolDecl *getHashableDecl();
   ProtocolDecl *getCopyableDecl();
   ProtocolDecl *getMoveOnlyDecl();
   ProtocolDecl *getImplicitlyCopyableDecl();
   ProtocolDecl *getStringRepresentableDecl();
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

   bool trackDeclsPerFile() const { return TrackDeclsPerFile; }
   void setTrackDeclsPerFile(bool V) { TrackDeclsPerFile = V; }

   ExprResult visit(Expression *Expr, bool);
   StmtResult visit(Statement *node, bool);
   DeclResult visit(Decl *D, bool);

   DeclResult declare(Decl *Decl, bool);

   RecordDecl *getCurrentRecordCtx();
   bool inTemplate();
   bool inUnboundedTemplate();
   bool isInDependentContext();

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
   NamedDecl *getCurrentInstantiationScope();

   QualType getStaticForValue(IdentifierInfo *name) const;

   //===-------------------------------------------------------===//
   // CTFE
   //===-------------------------------------------------------===//

   bool ensureSizeKnown(QualType Ty, SourceLocation loc);
   bool ensureSizeKnown(RecordDecl *R, SourceLocation loc);

   bool ensureDeclared(Decl *D);
   bool ensureContextDeclared(DeclContext *DC);
   bool ensureVisited(Decl *D);

   bool ensureDeclared(Module *M);
   bool ensureVisited(Module *M);

   bool prepareGlobalForCtfe(VarDecl *Decl);

   DeclResult declareAndVisit(Decl *D);

   void registerExtension(ExtensionDecl *D);
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
                  ArrayRef<Expression*> args,
                  ArrayRef<Expression*> templateArgs = {},
                  ArrayRef<IdentifierInfo*> labels = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false);

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
                  llvm::ArrayRef<Expression*> args,
                  llvm::ArrayRef<Expression*> templateArgs = {},
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
   int ExprCanReturn(Expression *E, QualType Ty);
   QualType ResolveContextualLambdaExpr(LambdaExpr *E, QualType Ty);
   QualType GetDefaultExprType(Expression *E);

   void visitTypeDependentContextualExpr(Expression *E);

   DeclResult doDestructure(DestructuringDecl *D,
                            QualType DestructuredTy);

   void maybeInstantiate(CandidateSet &CandSet, Statement *Caller);
   bool maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                               const TemplateArgList &templateArgs,
                               Statement *Caller);

   MethodDecl *maybeInstantiateMemberFunction(MethodDecl *M, StmtOrDecl Caller);

   MethodDecl *InstantiateMethod(RecordDecl *R, StringRef Name, StmtOrDecl SOD);
   MethodDecl *InstantiateProperty(RecordDecl *R, StringRef Name,
                                   bool Getter, StmtOrDecl SOD);

   void declareMemberwiseInitializer(StructDecl *S,
                                     InitDecl *ExplicitDecl = nullptr);

   void declareDefaultInitializer(StructDecl *S);
   void declareDefaultDeinitializer(RecordDecl *R);

   bool inGlobalDeclContext() const;

   void checkDuplicateFunctionDeclaration(CallableDecl *C,
                                          llvm::StringRef fnKind);

   bool alreadyVisited(Statement *stmt)
   {
      bool prev = stmt->isSemanticallyChecked();
      stmt->setSemanticallyChecked(true);

      return prev;
   }

   bool alreadyVisited(Decl *D)
   {
      bool prev = D->isSemanticallyChecked();
      D->setSemanticallyChecked(true);

      return prev;
   }

   bool alreadyDeclared(Decl *D)
   {
      bool prev = D->wasDeclared();
      D->setDeclared(true);

      return prev;
   }

   bool isDeclared(Decl *D) { return D->wasDeclared(); }
   bool isVisited(Decl *D) { return D->isSemanticallyChecked(); }

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

public:
   CallableDecl*checkFunctionReference(Expression *E,
                                       DeclarationName funcName,
                                       MultiLevelLookupResult &MultiLevelResult,
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

   AliasResult checkAlias(MultiLevelLookupResult &MultiLevelResult,
                          llvm::ArrayRef<Expression*> templateArgs,
                          Expression *E);

   bool checkAlias(AliasDecl *alias, CandidateSet::Candidate &Cand);

   // checks whether the parent expression of the given expression refers to
   // a namespace rather than a value and adjusts the expression appropriately
   NestedNameSpecifier *checkNamespaceRef(Expression *Expr);
   bool refersToNamespace(Expression *E);

   ExprResult checkNamespaceRef(MacroExpansionExpr *Expr);
   StmtResult checkNamespaceRef(MacroExpansionStmt *Stmt);
   DeclResult checkNamespaceRef(MacroExpansionDecl *D);

private:
   ExprResult HandleBuiltinTypeMember(IdentifierRefExpr *Expr, QualType Ty);
   ExprResult HandleStaticTypeMember(IdentifierRefExpr *Expr, QualType Ty);

   ExprResult HandleFieldAccess(IdentifierRefExpr *Ident,
                                MutableArrayRef<Expression*> TemplateArgs,
                                FieldDecl *F);

   ExprResult HandlePropAccess(IdentifierRefExpr *Ident, PropDecl *P);

   ExprResult HandleEnumCase(IdentifierRefExpr *node, EnumDecl *E);

   // CallExpr

   ExprResult HandleStaticTypeCall(CallExpr *Call,
                                   MutableArrayRef<Expression*> TemplateArgs,
                                   Type *Ty);
   ExprResult HandleConstructorCall(CallExpr *Call,
                                    MutableArrayRef<Expression*> TemplateArgs,
                                    RecordDecl *R);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               StmtOrDecl Subject,
                               DeclarationName memberName,
                               diag::MessageKind msg
                                 = diag::err_member_not_found,
                               SourceRange SR = SourceRange());

   bool isAccessible(NamedDecl *ND);
   void checkAccessibility(NamedDecl *ND, StmtOrDecl SOD);

   StmtOrDecl checkMacroCommon(StmtOrDecl SOD,
                               DeclarationName MacroName,
                               DeclContext &Ctx,
                               MacroDecl::Delimiter Delim,
                               llvm::ArrayRef<lex::Token> Tokens,
                               unsigned Kind);


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

   friend class ReflectionBuilder;
};

} // namespace ast
} // namespace cdot


#endif //CDOT_SEMA_H
