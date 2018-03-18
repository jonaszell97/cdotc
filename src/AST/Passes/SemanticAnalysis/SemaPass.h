//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_SEMA_H
#define CDOT_SEMA_H

#include "ActionResult.h"

#include "AST/Passes/SemanticAnalysis/Template.h"
#include "AST/Passes/SemanticAnalysis/CandidateSet.h"
#include "AST/Passes/SemanticAnalysis/Scope/Scope.h"
#include "AST/Passes/SemanticAnalysis/ConversionSequence.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/ASTVisitor.h"
#include "AST/ASTContext.h"
#include "AST/Attribute/Attribute.h"

#include "Basic/CastKind.h"
#include "Basic/DependencyGraph.h"
#include "Basic/Mangle.h"
#include "Basic/Precedence.h"
#include "BuiltinCandidateBuilder.h"

#include "Compiler.h"
#include "CTFE/StaticEvaluator.h"

#include "Message/DiagnosticsEngine.h"
#include "Support/Casting.h"

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
class SemaDiagConsumer;

class SemaPass: public ASTVisitor<ExprResult, StmtResult, DeclResult> {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;
   using MultiLevelTemplateArgList = sema::MultiLevelTemplateArgList;

   explicit SemaPass(CompilationUnit &compilationUnit);
   ~SemaPass();

   Type *getBuiltinType(llvm::StringRef typeName);
   ObjectType *getObjectTy(llvm::StringRef name) const;
   ObjectType *getObjectTy(Type::BoxedPrimitive kind) const;

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

   void addDeclToContext(DeclContext &Ctx, llvm::StringRef name,
                         NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, Decl *D);

   void copyStatusFlags(Expression *From, Statement *To)
   {
      To->copyStatusFlags(From);
      if (From->isVariadicArgPackExpansion())
         To->setContainsUnexpandedParameterPack(false);
   }

   [[nodiscard]]
   ExprResult visitExpr(Statement *DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      copyStatusFlags(E, DependentStmt);

      return res;
   }

   [[nodiscard]]
   ExprResult visitExpr(Decl *DependentDecl, Expression *E)
   {
      auto res = visit(E, true);
      DependentDecl->copyStatusFlags(E);

      return res;
   }

   [[nodiscard]]
   ExprResult getRValue(Statement *DependentStmt, Expression *E)
   {
      auto res = visit(E, true);
      copyStatusFlags(E, DependentStmt);

      if (!res)
         return ExprError();

      auto Cast = castToRValue(res.get());
      assert(Cast->getExprType() && "lvalue to rvalue cast introduced error");

      return Cast;
   }

   [[nodiscard]]
   ExprResult getRValue(Decl *DependentDecl, Expression *E)
   {
      auto res = visit(E, true);
      DependentDecl->copyStatusFlags(E);

      if (!res)
         return ExprError();

      auto Cast = castToRValue(res.get());
      assert(Cast->getExprType() && "lvalue to rvalue cast introduced error");

      return Cast;
   }

   [[nodiscard]]
   ExprResult getAsOrCast(Statement *DependentStmt,
                          Expression *E,
                          QualType expectedType) {
      E->setContextualType(expectedType);

      auto res = visit(E, true);
      copyStatusFlags(E, DependentStmt);

      if (!res)
         return ExprError();

      // this will return a usable expression even if it fails
      return implicitCastIfNecessary(res.get(), expectedType);
   }

   [[nodiscard]]
   ExprResult getAsOrCast(Decl *DependentDecl,
                          Expression *E,
                          QualType expectedType) {
      E->setContextualType(expectedType);

      auto res = visit(E, true);
      DependentDecl->copyStatusFlags(E);

      if (!res)
         return ExprError();

      // this will return a usable expression even if it fails
      return implicitCastIfNecessary(res.get(), expectedType);
   }

   [[nodiscard]]
   ExprResult visitExpr(Statement *DependentStmt,
                        Expression *E,
                        QualType contextualType) {
      E->setContextualType(contextualType);

      auto res = visit(E, true);
      copyStatusFlags(E, DependentStmt);

      return res;
   }

   [[nodiscard]]
   StmtResult visitStmt(Statement *DependentStmt, Statement *Stmt)
   {
      auto res = visit(Stmt, true);
      DependentStmt->copyStatusFlags(Stmt);

      return res;
   }

   [[nodiscard]]
   DeclResult visitStmt(Statement *DependentStmt, Decl *D)
   {
      auto res = visit(D, true);
      DependentStmt->copyStatusFlags(D);

      return res;
   }

   [[nodiscard]]
   StmtResult visitStmt(Decl *DependentDecl, Statement *Stmt)
   {
      auto res = visit(Stmt, true);
      DependentDecl->copyStatusFlags(Stmt);

      return res;
   }

   [[nodiscard]]
   DeclResult visitStmt(Decl *DependentDecl, Decl *D)
   {
      auto res = visit(D, true);
      DependentDecl->copyStatusFlags(D);

      return res;
   }

   [[nodiscard]]
   DeclResult declareStmt(Statement *DependentStmt, Decl *Decl)
   {
      auto res = declare(Decl, true);
      DependentStmt->copyStatusFlags(Decl);

      return res;
   }

   [[nodiscard]]
   DeclResult declareStmt(Decl *DependentDecl, Decl *Decl)
   {
      auto res = declare(Decl, true);
      DependentDecl->copyStatusFlags(Decl);

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
   
   // Declaration pass

   bool declareDeclContext(DeclContext *Ctx);
   void transferDecls(DeclContext *From, DeclContext *To);

   StmtResult declareImportStmt(ImportStmt *node);
   DeclResult declareCompoundDecl(CompoundDecl *Compound);
   DeclResult declareTranslationUnit(TranslationUnit *node);

   DeclResult declareNamespaceDecl(NamespaceDecl *NS);
   StmtResult declareUsingStmt(UsingStmt *Stmt);

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

#  define CDOT_DECL_ATTR(Name, Spelling)              \
   void check##Name##Attr(Decl *D, Name##Attr *A);

#  define CDOT_STMT_ATTR(Name, Spelling)              \
   void check##Name##Attr(Statement *S, Name##Attr *A);

#  define CDOT_EXPR_ATTR(Name, Spelling)              \
   void check##Name##Attr(Expression *E, Name##Attr *A);

#  define CDOT_TYPE_ATTR(Name, Spelling)              \
   void check##Name##Attr(SourceType Ty, Name##Attr *A);

#  include "AST/Attribute/Attributes.def"

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

   ExprResult visitDictionaryLiteral(DictionaryLiteral *Dict);
   ExprResult visitArrayLiteral(ArrayLiteral *Expr);

   ExprResult visitNoneLiteral(NoneLiteral *Expr);
   ExprResult visitStringLiteral(StringLiteral *Expr);
   ExprResult visitStringInterpolation(StringInterpolation *Expr);
   ExprResult visitTupleLiteral(TupleLiteral *Expr);

   ExprResult visitExprSequence(ExprSequence *Expr);
   ExprResult visitTypePredicateExpr(TypePredicateExpr *Pred);
   ExprResult visitBinaryOperator(BinaryOperator *BinOp);
   ExprResult visitUnaryOperator(UnaryOperator *node);

   ExprResult visitCastExpr(CastExpr *Cast);

   StmtResult visitTryStmt(TryStmt *Stmt);
   StmtResult visitThrowStmt(ThrowStmt *Stmt);

   DeclResult visitFuncArgDecl(FuncArgDecl *Decl);

   ExprResult visitLambdaExpr(LambdaExpr *Expr);
   ExprResult visitImplicitCastExpr(ImplicitCastExpr *node);

   ExprResult visitFunctionTypeExpr(FunctionTypeExpr *Expr);
   ExprResult visitTupleTypeExpr(TupleTypeExpr *Expr);
   ExprResult visitArrayTypeExpr(ArrayTypeExpr *Expr);
   ExprResult visitDeclTypeExpr(DeclTypeExpr *Expr);
   ExprResult visitPointerTypeExpr(PointerTypeExpr *Expr);
   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr *Expr);
   ExprResult visitOptionTypeExpr(OptionTypeExpr *Expr);

   StmtResult visitNullStmt(NullStmt *stmt);
   StmtResult visitModuleStmt(ModuleStmt *stmt);
   StmtResult visitImportStmt(ImportStmt *stmt);
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
   size_t checkConstraints(Statement *DependentStmt,
                           NamedDecl *ConstrainedDecl,
                           const sema::TemplateArgList &templateArgs);

   size_t checkConstraints(Statement *DependentStmt,
                           NamedDecl *ConstrainedDecl);

   bool checkConstraint(Statement *DependentStmt,
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

   IdentifierRefExpr *wouldBeValidIdentifier(llvm::StringRef maybeIdent);
   GlobalVarDecl *getGlobalVariable(llvm::StringRef maybeGlobal);

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

   QualType resolveDependencies(QualType Ty, RecordDecl *R,
                                Statement *PointOfInstantiation);

   template<class ...Args>
   void diagnose(Statement *Stmt, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         Stmt->setHadError(true);
      }

      diagnose(Stmt->getSourceRange(), msg, std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(Decl *D, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         D->setIsInvalid(true);
      }

      diagnose(D->getSourceRange(), msg, std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(SourceLocation loc, diag::MessageKind msg, Args const&...args)
   {
      return diagnose(SourceRange(loc), msg,
                      std::forward<Args const&>(args)...);
   }

   template<class ...Args>
   void diagnose(SourceRange range, diag::MessageKind msg, Args const&...args)
   {
      if (diag::isError(msg)) {
         EncounteredError = true;

         if (currentScope)
            currentScope->setHadError(true);
      }

      {
         diag::DiagnosticBuilder Builder(Diags, msg);
         addDiagnosticArgs(Builder, range, std::forward<Args const &>(args)...);
      }

      if (!diag::isNote(msg))
         noteInstantiationContext();
   }

   void noteInstantiationContext();

   void diagnoseTemplateArgErrors(NamedDecl *Template,
                                  Statement *ErrorStmt,
                                  TemplateArgList &list,
                                  sema::TemplateArgListResult &Cand);

   void visitRecordInstantiation(Statement *DependentStmt, RecordDecl *Inst);
   void declareRecordInstantiation(Statement *DependentStmt, RecordDecl *Inst);

   void visitFunctionInstantiation(Statement *DependentStmt,
                                   CallableDecl *Inst);

   void registerDelayedInstantiation(NamedDecl *Inst, Statement *POI);
   void registerDelayedFunctionDecl(CallableDecl *C);
   void registerTemplateParamWithDefaultVal(TemplateParamDecl *TD);

   void visitDelayedDecl(NamedDecl *ND);

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
   StructDecl *getStruct(llvm::StringRef name) const;
   ClassDecl *getClass(llvm::StringRef name) const;
   EnumDecl *getEnum(llvm::StringRef name) const;
   UnionDecl *getUnion(llvm::StringRef name) const;
   ProtocolDecl *getProtocol(llvm::StringRef name) const;
   NamespaceDecl *getNamespace(llvm::StringRef name) const;

   FunctionDecl *getAnyFn(llvm::StringRef name) const;
   GlobalVarDecl *getVariable(llvm::StringRef name) const;
   TypedefDecl *getTypedef(llvm::StringRef name) const;
   AliasDecl* getAlias(llvm::StringRef name) const;

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

   struct DeclScopeRAII {
      DeclScopeRAII(SemaPass &SP, DeclContext *Ctx)
         : SP(SP), declContext(SP.DeclCtx)
      {
         SP.DeclCtx = Ctx;
      }

      ~DeclScopeRAII()
      {
         SP.DeclCtx = declContext;
      }

   private:
      SemaPass &SP;
      DeclContext *declContext;
   };

   struct ScopeResetRAII {
      explicit ScopeResetRAII(SemaPass &S)
         : S(S), scope(S.currentScope)
      {
         S.currentScope = nullptr;
      }

      ~ScopeResetRAII()
      {
         S.currentScope = scope;
      }

   private:
      SemaPass &S;
      Scope *scope;
   };

   struct DeclContextRAII {
      DeclContextRAII(SemaPass &SP, DeclContext *Ctx) : SP(SP)
      {
         Ctx->setParentCtx(SP.DeclCtx);
         SP.pushDeclContext(Ctx);
      }

      ~DeclContextRAII()
      {
         SP.popDeclContext();
      }

   private:
      SemaPass &SP;
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

   bool hasDefaultValue(QualType type) const;

   void issueDiagnostics();

   size_t getNumDiags() const;
   void resizeDiags(size_t toSize);

   bool encounteredError() const { return EncounteredError; }
   size_t getNumGlobals() const { return numGlobals; }

private:
   CompilationUnit *compilationUnit; // can't be a reference because it's a
                                     // movable type

   std::unique_ptr<SemaDiagConsumer> DiagConsumer;
   DiagnosticsEngine Diags;

   ASTContext &Context;
   DeclContext *DeclCtx;

   Stage stage = Stage::Declaration;

   Scope *currentScope = nullptr;

   llvm::SmallPtrSet<NamedDecl*, 16> DelayedDecls;
   llvm::SmallVector<std::pair<Statement*, NamedDecl*>, 0>
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

   bool fatalError        : 1;
   bool fatalErrorInScope : 1;
   bool EncounteredError  : 1;
   bool InCTFE            : 1;

   QualType UnknownAnyTy;

   RecordDecl *ArrayDecl      = nullptr;
   RecordDecl *DictionaryDecl = nullptr;
   RecordDecl *StringDecl     = nullptr;
   RecordDecl *OptionDecl     = nullptr;

public:
   RecordDecl *getArrayDecl();
   RecordDecl *getDictionaryDecl();
   RecordDecl *getStringDecl();
   RecordDecl *getOptionDecl();

   ExprResult visit(Expression *Expr, bool);
   StmtResult visit(Statement *node, bool);
   DeclResult visit(Decl *D, bool);

   DeclResult declare(Decl *Decl, bool);

   RecordDecl *getCurrentRecordCtx();

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
   QualType getStaticForValue(llvm::StringRef name) const;

   bool ensureSizeKnown(QualType Ty, SourceLocation loc);
   bool ensureSizeKnown(RecordDecl *R, SourceLocation loc);
   bool ensureDeclared(Decl *D);
   bool prepareFunctionForCtfe(CallableDecl *Fn);

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
                 llvm::StringRef elementName,
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
                  llvm::StringRef name,
                  llvm::ArrayRef<Expression*> args,
                  llvm::ArrayRef<Expression*> templateArgs = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false,
                  bool includesSelf = false);

   CandidateSet
   lookupFunction(llvm::StringRef name,
                  llvm::ArrayRef<Expression*> args,
                  llvm::ArrayRef<Expression*> templateArgs = {},
                  Statement *Caller = nullptr,
                  bool suppressDiags = false,
                  bool includesSelf = false);

   CandidateSet
   lookupMethod(llvm::StringRef name,
                Expression *SelfExpr,
                llvm::ArrayRef<Expression*> args,
                llvm::ArrayRef<Expression*> templateArgs = {},
                Statement *Caller = nullptr,
                bool suppressDiags = false);

   CandidateSet getCandidates(llvm::StringRef name,
                              Expression *SelfExpr);

   CandidateSet
   lookupCase(llvm::StringRef name,
              EnumDecl *E,
              llvm::ArrayRef<Expression*> args,
              llvm::ArrayRef<Expression*> templateArgs = {},
              Statement *Caller = nullptr,
              bool suppressDiags = false);

   void lookupFunction(CandidateSet &CandSet,
                       llvm::StringRef name,
                       llvm::ArrayRef<Expression*> args,
                       llvm::ArrayRef<Expression*> templateArgs = {},
                       Statement *Caller = nullptr,
                       bool suppressDiags = false);

   CandidateSet checkAnonymousCall(FunctionType *FTy,
                                   llvm::ArrayRef<Expression*> args,
                                   Statement *Caller = nullptr);

   void maybeInstantiate(CandidateSet &CandSet, Statement *Caller);
   void maybeInstantiateMemberFunction(MethodDecl *M, Statement *Caller);

   void declareMemberwiseInitializer(StructDecl *S);
   void declareDefaultInitializer(StructDecl *S);
   void declareDefaultDeinitializer(StructDecl *S);

   TemplateParamDecl* getTemplateParam(llvm::StringRef name);
   AssociatedTypeDecl* getAssociatedType(llvm::StringRef name);

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
                                                    = diag::err_type_mismatch);

   Expression* forceCast(Expression* Expr, QualType destTy);

   Expression *castToRValue(Expression *Expr);
   void toRValue(Expression *Expr);

private:
   // IdentifierRef

   CallableDecl* checkFunctionReference(Expression *E,
                                        DeclContext *Ctx,
                                        llvm::StringRef funcName,
                                       llvm::ArrayRef<Expression*>templateArgs);

   struct AliasResult {
      explicit AliasResult(AliasDecl *Alias)
         : TypeDependent(false), ValueDependent(false), HadError(false),
           Result(Alias)
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

      operator bool() const
      {
         return !HadError && !TypeDependent && !ValueDependent;
      }

   private:
      bool TypeDependent  : 1;
      bool ValueDependent : 1;
      bool HadError       : 1;

      AliasDecl *Result;
   };

   AliasResult checkAlias(LookupResult &lookupResult,
                          llvm::ArrayRef<Expression*> templateArgs,
                          SourceLocation loc);

   bool checkAlias(AliasDecl *alias,
                   std::vector<diag::DiagnosticBuilder> &Diagnostics);

   // MemberRef

   QualType HandleStaticTypeMember(MemberRefExpr *node, Type *Ty);

   ExprResult HandleFieldAccess(IdentifierRefExpr *Ident, FieldDecl *F);
   ExprResult HandlePropAccess(IdentifierRefExpr *Ident, PropDecl *P);

   ExprResult HandleEnumCase(IdentifierRefExpr *node, EnumDecl *E);

   // CallExpr

   void HandleFunctionCall(CallExpr* node,
                           DeclContext *Ctx,
                           llvm::StringRef funcName);

   void HandleBuiltinCall(CallExpr*);

   ExprResult HandleMethodCall(CallExpr *Call, Expression *ParentExpr);
   void HandleStaticTypeCall(CallExpr *node, Type *Ty);

   void HandleConstructorCall(CallExpr*, RecordDecl *R);
   void HandleCallOperator(CallExpr *node, Expression *ParentExpr);
   void HandleAnonCall(CallExpr *node, Expression *ParentExpr);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               Statement *Subject,
                               llvm::StringRef memberName,
                               diag::MessageKind msg
                                 = diag::err_undeclared_identifer);

   // BinaryOperator

   void checkClassAccessibility(RecordDecl* cl, Expression* cause);

   void checkMemberAccessibility(RecordDecl* record,
                                 const string& memberName,
                                 const AccessModifier& access,
                                 Expression* cause);

   void ApplyCasts(std::vector<Expression* > &args,
                   CandidateSet &CandSet);

   void PrepareCallArgs(std::vector<Expression* >& args,
                        FunctionType *FuncTy);

   void PrepareCallArgs(std::vector<Expression* >& args,
                        CallableDecl *C);

   void calculateRecordSize(RecordDecl *R);
   void calculateRecordSizes();
};

} // namespace ast
} // namespace cdot


#endif //CDOT_SEMA_H
