#ifndef CDOT_SEMA_H
#define CDOT_SEMA_H

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/ASTVisitor.h"
#include "cdotc/AST/Attr.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/EmptyASTVisitor.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/Basic/CastKind.h"
#include "cdotc/Basic/Mangle.h"
#include "cdotc/Basic/Precedence.h"
#include "cdotc/CTFE/StaticEvaluator.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Diagnostics/DiagnosticsEngine.h"
#include "cdotc/Sema/ActionResult.h"
#include "cdotc/Sema/CandidateSet.h"
#include "cdotc/Sema/ConversionSequence.h"
#include "cdotc/Sema/Lookup.h"
#include "cdotc/Sema/Scope/Scope.h"
#include "cdotc/Sema/Template.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/Optional.h"

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallPtrSet.h>

#include <queue>

namespace cdot {

class ConformanceResolver;
class QueryContext;

namespace ast {

class DeclContextLookupResult;
class ILGenPass;
class ReflectionBuilder;
class TemplateInstantiator;

/// This class takes care of the bulk of the semantic analysis of the program.
class SemaPass : public EmptyASTVisitor<ExprResult, StmtResult, DeclResult> {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;
   using MultiLevelTemplateArgList = sema::MultiLevelTemplateArgList;

   explicit SemaPass(CompilerInstance& compilationUnit);
   ~SemaPass();

   /// \return The diagnostics engine of this compilation.
   DiagnosticsEngine& getDiags() { return Diags; }

   /// \return The active compiler instance.
   CompilerInstance& getCompilerInstance() const { return *compilerInstance; }

   /// \return The AST context.
   ASTContext& getContext() const { return Context; }

   /// \return The declaration context that is currently being checked.
   DeclContext& getDeclContext() const { return *DeclCtx; }

   /// \return The symbol mangler instance.
   const SymbolMangler& getMangler() const { return mangle; }

   /// \return The template instantiator instance.
   TemplateInstantiator& getInstantiator() const;

   /// \return The conformance resolver instance.
   ConformanceResolver& getConformanceResolver();

   /// \return The ILGen instance.
   ILGenPass& getILGen() { return *ILGen.get(); }

   /// \return Whether or not this record is currently being resolved by the
   /// conformance resolver. If true, this record is not yet ready for full
   /// lookup or instantiation.
   bool IsBeingResolved(RecordDecl *R);

   /// \return Whether or not any conformance of R to P exists,
   /// ignoring constraints.
   bool UncheckedConformanceExists(RecordDecl *R, ProtocolDecl *P);

   /// Enter a new declaration context.
   void pushDeclContext(DeclContext* Ctx) { this->DeclCtx = Ctx; }

   /// Exit a declaration context.
   void popDeclContext() { DeclCtx = DeclCtx->getParentCtx(); }

   /// \return True iff there is an implicit conversion from `from` to `to`.
   bool implicitlyConvertibleTo(CanType from, CanType to);

   /// Conversion options.
   enum ConversionOpts : uint8_t {
      CO_None = 0x0,
      CO_IsClangParameterValue = 0x1,
      CO_IsSelfValue = 0x2,
   };

   /// \return A possible conversion sequence from `from` to `to`, or an invalid
   /// one if there is no conversion.
   ConversionSequenceBuilder getConversionSequence(CanType from, CanType to,
                                                   ConversionOpts options = CO_None);

   /// Apply standard conversions for a C-style vararg function argument.
   Expression* convertCStyleVarargParam(Expression* Expr);

   /// Whether or not two function signatures are compatible. Used for checking
   /// of overrides of virtual methods.
   /// \return -1 indicates compatible signatures, positive values
   /// are error codes to use in diagnostics.
   int signaturesCompatible(CallableDecl* C1, CallableDecl* C2);

   /// Diagnose an illegal redeclaration.
   void diagnoseRedeclaration(DeclContext& Ctx,
                              DeclContext::AddDeclResultKind ResKind,
                              DeclarationName Name, NamedDecl* Decl);

   /// Make a declaration visible in a context under a specified name.
   void addDeclToContext(DeclContext& Ctx, DeclarationName Name,
                         NamedDecl* Decl);

   /// Make a declaration visible in a context.
   void addDeclToContext(DeclContext& Ctx, NamedDecl* Decl);

   /// Add a (potentially named) decl to a context.
   void addDeclToContext(DeclContext& Ctx, Decl* D);

   /// Make a declaration visible in a context.
   void makeDeclAvailable(DeclContext& Dst, NamedDecl* Decl,
                          bool IgnoreRedecl = false,
                          bool IsImport = false);

   /// Make a declaration visible in a context under a specified name.
   void makeDeclAvailable(DeclContext& Dst, DeclarationName Name,
                          NamedDecl* Decl, bool IgnoreRedecl = false,
                          bool IsImport = false);

   /// Make all declarations of `Src` visible in `Dst`.
   void makeDeclsAvailableIn(DeclContext& Dst, DeclContext& Src,
                             bool IgnoreRedecl = false);

private:
   /// Semantically check a type-checked expression.
   ExprResult visit(Expression* Expr, bool);

   /// Semantically check a statement.
   StmtResult visit(Statement* node, bool);

public:
   /// Semantically check a type-checked expression.
   [[nodiscard]] ExprResult visitExpr(StmtOrDecl DependentStmt, Expression* E)
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

   /// Semantically check a type-checked expression, converting it to an R-value
   /// if necessary.
   [[nodiscard]] ExprResult getRValue(StmtOrDecl DependentStmt, Expression* E)
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

   /// Semantically check a type-checked expression, converting it to a
   /// specific type if necessary.
   [[nodiscard]] ExprResult getAsOrCast(StmtOrDecl DependentStmt, Expression* E,
                                        QualType expectedType)
   {
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

   [[nodiscard]] ExprResult visitExpr(StmtOrDecl DependentStmt, Expression* E,
                                      QualType contextualType)
   {
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

   /// Semantically check a statement.
   [[nodiscard]] StmtResult visitStmt(StmtOrDecl DependentStmt, Statement* Stmt)
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

    /// Semantically check a type-checked expression.
   [[nodiscard]] ExprResult visitExpr(Expression* E) { return visit(E, true); }

   /// Semantically check a statement.
   [[nodiscard]] StmtResult visitStmt(Statement* Stmt) { return visit(Stmt, true); }

   /// -------
   /// The various 'ActOn' methods are called by the parser to do some work we
   /// can do without type information, like making named declarations visible.
   /// -------

   void ActOnDecl(DeclContext* DC, Decl* D);
   void ActOnImportDecl(DeclContext* DC, ImportDecl* D);

   void ActOnRecordDecl(DeclContext* DC, RecordDecl* R);
   void ActOnStructDecl(DeclContext* DC, StructDecl* S);
   void ActOnProtoDecl(DeclContext* DC, ProtocolDecl* P);
   void ActOnEnumDecl(DeclContext* DC, EnumDecl* E);
   void ActOnUnionDecl(DeclContext* DC, UnionDecl* U);
   void ActOnExtensionDecl(DeclContext* DC, ExtensionDecl* Ext);

   void ActOnOperatorDecl(DeclContext* DC, OperatorDecl* Op);
   void ActOnPrecedenceGroupDecl(DeclContext* DC, PrecedenceGroupDecl* PG);

   void ActOnFunctionDecl(DeclContext* DC, FunctionDecl* F);
   void ActOnTypedefDecl(DeclContext* DC, TypedefDecl* TD);
   void ActOnAliasDecl(DeclContext* DC, AliasDecl* Alias);

   void ActOnFieldDecl(DeclContext* DC, FieldDecl* F);
   void ActOnPropDecl(DeclContext* DC, PropDecl* P);
   void ActOnMethodDecl(DeclContext* DC, MethodDecl* M);
   void ActOnInitDecl(DeclContext* DC, InitDecl* I);
   void ActOnDeinitDecl(DeclContext* DC, DeinitDecl* D);
   void ActOnSubscriptDecl(DeclContext* DC, SubscriptDecl* D);

   void ActOnTemplateParamDecl(DeclContext* DC, TemplateParamDecl* P);

   void ActOnMacroExpansionDecl(DeclContext* DC, MacroExpansionDecl* Decl);
   void ActOnStaticIfDecl(DeclContext* DC, StaticIfDecl* D);
   void ActOnStaticForDecl(DeclContext* DC, StaticForDecl* D);
   void ActOnMixinDecl(DeclContext* DC, MixinDecl* D);

   /// -------
   /// These functions are used for semantically checking the various
   /// attributes.
   /// -------

   void checkDeclAttrs(Decl* D, Attr::VisitationPoint VP);

#define CDOT_DECL_ATTR(Name, Spelling)                                         \
   void check##Name##Attr(Decl* D, Name##Attr* A);

#define CDOT_STMT_ATTR(Name, Spelling)                                         \
   void check##Name##Attr(Statement* S, Name##Attr* A);

#define CDOT_EXPR_ATTR(Name, Spelling)                                         \
   void check##Name##Attr(Expression* E, Name##Attr* A);

#define CDOT_TYPE_ATTR(Name, Spelling)                                         \
   void check##Name##Attr(Expression* E, Name##Attr* A);

#include "cdotc/AST/Attributes.def"

   /// Typecheck an expression (potentially using a constraint system). This
   /// does not guarantee that the expression is semantically valid, the various
   /// 'visit' functions are used after this to complete semantically checking
   /// an expression.
   ExprResult typecheckExpr(Expression* Expr, SourceType RequiredType,
                            Statement* DependentStmt,
                            bool isHardRequirement = true)
   {
      auto Result = typecheckExpr(Expr, RequiredType, isHardRequirement);
      if (Result) {
         DependentStmt->copyStatusFlags(Result.get());
      }
      else {
         DependentStmt->copyStatusFlags(Expr);
      }

      return Result;
   }

   /// See above.
   ExprResult typecheckExpr(Expression* Expr, SourceType RequiredType,
                            Decl* DependentDecl, bool isHardRequirement = true)
   {
      auto Result = typecheckExpr(Expr, RequiredType, isHardRequirement);
      if (Result) {
         DependentDecl->copyStatusFlags(Result.get());
      }
      else {
         DependentDecl->copyStatusFlags(Expr);
      }

      return Result;
   }

   /// See above.
   ExprResult typecheckExpr(Expression* Expr, SourceType RequiredType,
                            StmtOrDecl DependentDecl,
                            bool isHardRequirement = true)
   {
      auto Result = typecheckExpr(Expr, RequiredType, isHardRequirement);
      if (Result) {
         DependentDecl.copyStatusFlags(Result.get());
      }
      else {
         DependentDecl.copyStatusFlags(Expr);
      }

      return Result;
   }

   /// See above.
   ExprResult typecheckExpr(Expression* Expr,
                            SourceType RequiredType = ast::SourceType(),
                            bool isHardRequirement = true);

   /// -------
   /// These functions handle semantic checking of expressions and statements
   /// after their types have been assigned via constraint solving.
   /// -------

   StmtResult visitCompoundStmt(CompoundStmt* Stmt);

   StmtResult visitAttributedStmt(AttributedStmt* Stmt);
   ExprResult visitAttributedExpr(AttributedExpr* Expr);

   StmtResult visitDeclStmt(DeclStmt* Stmt);
   bool visitVarDecl(VarDecl* Decl);
   DeclResult visitLocalVarDecl(LocalVarDecl* Decl);
   DeclResult visitGlobalVarDecl(GlobalVarDecl* Decl);

   DeclResult visitDestructuringDecl(DestructuringDecl* D);

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr* Ident,
                                     TemplateArgListExpr* TemplateArgs
                                     = nullptr);

   ExprResult visitDeclRefExpr(DeclRefExpr* Expr);
   ExprResult visitMemberRefExpr(MemberRefExpr* Expr);
   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* Expr);

   ExprResult visitParenExpr(ParenExpr* Expr);
   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr* node);
   ExprResult visitSelfExpr(SelfExpr* Expr);
   ExprResult visitSuperExpr(SuperExpr* Expr);

   ExprResult visitBuiltinExpr(BuiltinExpr* node);
   ExprResult visitSubscriptExpr(SubscriptExpr* Expr);
   ExprResult visitCallExpr(CallExpr* Call,
                            TemplateArgListExpr* TemplateArgs = nullptr);
   ExprResult visitAnonymousCallExpr(AnonymousCallExpr* Call);
   ExprResult visitTemplateArgListExpr(TemplateArgListExpr* Expr);

   ExprResult visitTupleMemberExpr(TupleMemberExpr* Expr);

   StmtResult visitForStmt(ForStmt* Stmt);
   StmtResult visitForInStmt(ForInStmt* Stmt);
   StmtResult visitWhileStmt(WhileStmt* Stmt);

   void visitIfConditions(Statement* Stmt,
                          MutableArrayRef<IfCondition> Conditions);

   StmtResult visitIfStmt(IfStmt* Stmt);

   StmtResult visitMatchStmt(MatchStmt* Stmt);
   StmtResult visitCaseStmt(CaseStmt* Stmt, MatchStmt* Match = nullptr);

   PatternExpr* visitPatternExpr(Statement* DependentStmt, PatternExpr* E,
                                 Expression* MatchVal);

   ExprResult visitExpressionPattern(ExpressionPattern* node,
                                     Expression* MatchVal = nullptr);
   ExprResult visitCasePattern(CasePattern* Expr,
                               Expression* MatchVal = nullptr);
   ExprResult visitIsPattern(IsPattern* node, Expression* MatchVal = nullptr);

   StmtResult visitDiscardAssignStmt(DiscardAssignStmt* Stmt);

   StmtResult visitReturnStmt(ReturnStmt* Stmt);
   StmtResult visitBreakStmt(BreakStmt* Stmt);
   StmtResult visitContinueStmt(ContinueStmt* Stmt);

   ExprResult visitIntegerLiteral(IntegerLiteral* Expr);
   ExprResult visitFPLiteral(FPLiteral* Expr);
   ExprResult visitBoolLiteral(BoolLiteral* Expr);
   ExprResult visitCharLiteral(CharLiteral* Expr);

   ExprResult visitDictionaryLiteral(DictionaryLiteral* Expr);
   ExprResult visitArrayLiteral(ArrayLiteral* Expr);

   ExprResult visitNoneLiteral(NoneLiteral* Expr);
   ExprResult visitStringLiteral(StringLiteral* Expr);
   ExprResult visitStringInterpolation(StringInterpolation* Expr);
   ExprResult visitTupleLiteral(TupleLiteral* Expr);

   ExprResult visitExprSequence(ExprSequence* Expr);
   ExprResult visitTypePredicateExpr(TypePredicateExpr* Pred);
   ExprResult visitBinaryOperator(BinaryOperator* BinOp);
   ExprResult visitAssignExpr(AssignExpr* Expr);
   ExprResult visitUnaryOperator(UnaryOperator* UnOp);
   ExprResult visitIfExpr(IfExpr* Expr);

   ExprResult visitCastExpr(CastExpr* Cast);
   ExprResult visitAddrOfExpr(AddrOfExpr* Expr);

   StmtResult visitDoStmt(DoStmt* Stmt);
   StmtResult visitThrowStmt(ThrowStmt* Stmt);
   ExprResult visitTryExpr(TryExpr* Expr);
   ExprResult visitAwaitExpr(AwaitExpr* Expr);

   DeclResult visitFuncArgDecl(FuncArgDecl* Decl);

   ExprResult visitLambdaExpr(LambdaExpr* Expr);
   ExprResult visitImplicitCastExpr(ImplicitCastExpr* node);

   TypeResult resolveArrayTypeExpr(Statement* DependentExpr,
                                   const SourceType& ElementType,
                                   StaticExpr* SizeExpr);

   ExprResult visitTypeExpr(TypeExpr* E);
   ExprResult visitFunctionTypeExpr(FunctionTypeExpr* Expr);
   ExprResult visitTupleTypeExpr(TupleTypeExpr* Expr);
   ExprResult visitArrayTypeExpr(ArrayTypeExpr* Expr);
   ExprResult visitDeclTypeExpr(DeclTypeExpr* Expr);
   ExprResult visitPointerTypeExpr(PointerTypeExpr* Expr);
   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr* Expr);
   ExprResult visitOptionTypeExpr(OptionTypeExpr* Expr);
   ExprResult visitExistentialTypeExpr(ExistentialTypeExpr* Expr);

   StmtResult visitNullStmt(NullStmt* stmt);

   StmtResult visitDebugStmt(DebugStmt* Stmt);

   StmtResult visitStaticIfStmt(StaticIfStmt* Stmt);

   ExprResult visitVariadicExpansionExpr(VariadicExpansionExpr* Expr);
   StmtResult visitVariadicForStmt(StaticForStmt* Stmt);
   StmtResult visitStaticForStmt(StaticForStmt* Stmt);

   DeclResult visitStaticAssertDecl(StaticAssertDecl* Decl);
   DeclResult visitStaticPrintDecl(StaticPrintDecl* Decl);
   DeclResult visitVariadicForDecl(StaticForDecl* Decl);

   ExprResult visitStaticExpr(StaticExpr* Expr);
   ExprResult visitTraitsExpr(TraitsExpr* Expr);

   ExprResult visitMixinExpr(MixinExpr* Expr);
   StmtResult visitMixinStmt(MixinStmt* Stmt);

   Expression* resolveMacroExpansionExpr(MacroExpansionExpr* Expr);
   ExprResult visitMacroExpansionExpr(MacroExpansionExpr* Expr);
   StmtResult visitMacroExpansionStmt(MacroExpansionStmt* Stmt);

   /// Resolve a type as it appeared in the source to an actual type.
   TypeResult visitSourceType(Decl* D, const SourceType& Ty,
                              bool WantMeta = false);

   /// Resolve a type as it appeared in the source to an actual type.
   TypeResult visitSourceType(Statement* S, const SourceType& Ty,
                              bool WantMeta = false);

   /// Resolve a type as it appeared in the source to an actual type.
   TypeResult visitSourceType(const SourceType& Ty, bool WantMeta = false);

   /// Disallow passing an rvalue as second parameter.
   template<class T> TypeResult visitSourceType(T* D, SourceType&& Ty) = delete;

   /// Disallow passing an rvalue as second parameter.
   TypeResult visitSourceType(SourceType&& Ty) = delete;

   /// The result of a compile-time expression evaluation.
   struct StaticExprResult {
      explicit StaticExprResult(Expression* Expr, il::Constant* V)
          : Expr(Expr), Value(V), HadError(false)
      {
      }

      /* implicit */
      StaticExprResult(Expression* Expr = nullptr, bool typeDependent = false)
          : Expr(Expr), HadError(!typeDependent)
      {
      }

      il::Constant* getValue()
      {
         assert(!hadError());
         return Value;
      }

      [[nodiscard]] Expression* getExpr() const { return Expr; }
      [[nodiscard]] bool hadError() const { return HadError; }
      [[nodiscard]] bool isDependent() const { return !Value && !HadError; }

      operator bool() const { return !hadError() && !isDependent(); }

   private:
      Expression* Expr = nullptr;
      il::Constant* Value = nullptr;
      bool HadError;
   };

   /// Try to evaluate a compile-time expression.
   StaticExprResult evalStaticExpr(StmtOrDecl DependentStmt, Expression* expr);

   /// Try to evaluate a compile-time expression as a value of a specific type.
   StaticExprResult evaluateAs(StmtOrDecl DependentStmt, Expression* expr,
                               QualType Ty);

   /// Try to evaluate a compile-time expression as a boolean value.
   Optional<bool> evaluateAsBool(StmtOrDecl DependentStmt, Expression* expr);

   /// Print a constraint to an output stream.
   void printConstraint(llvm::raw_ostream& OS, QualType ConstrainedType,
                        DeclConstraint* C, QualType Self = QualType());

   /// \return the string representation of a constant value.
   bool getStringValue(Expression* Expr, il::Constant* V, llvm::StringRef& Str);

   /// \return the bool value of a constant value.
   bool getBoolValue(Expression* Expr, il::Constant* V, bool& Val);

   /// Check if a type is valid as the type of declaration (variable, property
   /// or field). This excludes types like void or protocols with associated
   /// types.
   void checkIfTypeUsableAsDecl(SourceType Ty, StmtOrDecl DependentDecl);

   /// Convert an abstract associated type to a concrete type.
   QualType CreateConcreteTypeFromAssociatedType(AssociatedType *AT,
                                                 QualType Outer,
                                                 QualType Original);

   /// Resolve a nested associated type to a concrete type.
   QualType ResolveNestedAssociatedType(QualType AT, CanType Self);

   /// Resolve a nested associated type to a concrete type.
   QualType ResolveNestedAssociatedType(
       QualType AT, DeclContext *DC = nullptr, QualType Self = QualType(),
       sema::FinalTemplateArgumentList *TemplateArgs = nullptr);

   /// \return The declaration represented by a type.
   NamedDecl* getTypeDecl(QualType Ty);

   /// \return The type of a declaration.
   QualType getTypeForDecl(NamedDecl* ND);

   /// \return Whether or not a warning should be issued if the result of the
   /// expression is unused.
   bool warnOnUnusedResult(Expression* E) const;

   /// Helper function for diagnosing errors.
   template<class... Args>
   void diagnose(Statement* Stmt, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         Stmt->setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   /// Helper function for diagnosing errors.
   template<class... Args>
   void diagnose(Decl* D, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         D->setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   /// Helper function for diagnosing errors.
   template<class... Args>
   void diagnose(StmtOrDecl SOD, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         SOD.setIsInvalid(true);
      }

      diagnose(msg, std::forward<Args const&>(args)...);
   }

   /// Helper function for diagnosing errors.
   template<class... Args>
   void diagnose(diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         if (currentScope) {
            currentScope->setHadError(true);
         }

         EncounteredError = true;
      }

      {
         diag::DiagnosticBuilder Builder(Diags, msg);
         addDiagnosticArgs(Builder, std::forward<Args const&>(args)...);
      }

      if (!diag::isNote(msg)) {
         noteInstantiationContext();
      }
   }

   /// Append a note of the current instantiation context to an
   /// issued diagnostic.
   void noteInstantiationContext();

   /// Diagnose an invalid template argument list.
   void diagnoseTemplateArgErrors(NamedDecl* Template, Statement* ErrorStmt,
                                  TemplateArgList& list,
                                  llvm::ArrayRef<Expression*> OriginalArgs,
                                  sema::TemplateArgListResult& Cand);

   /// Whether or not two template parameters are effectively equal.
   bool equivalent(TemplateParamDecl* p1, TemplateParamDecl* p2);

   /// Register the scope in which an instantiation was requested.
   void registerInstantiation(NamedDecl* Inst, NamedDecl* Scope)
   {
      InstScopeMap[Inst] = Scope;
   }

   /// \return The scope in which an instantiation was requested.
   NamedDecl* getInstantiationScope(NamedDecl* Inst);

   /// Validate that an 'override' method actually overrides a virtual method.
   void checkVirtualOrOverrideMethod(MethodDecl* M);

   /// Verify that a class overrides every abstract method of its base class.
   bool checkIfAbstractMethodsOverridden(ClassDecl* R);

   /// Diagnose a global variable that somehow references itself in its
   /// initializer.
   void diagnoseCircularlyDependentGlobalVariables(Expression* Expr,
                                                   NamedDecl* globalVar);

   /// \return The current function scope.
   CallableDecl* getCurrentFun() const;

   /// Helper function to create an identifier from a static string.
   template<std::size_t StrLen>
   IdentifierInfo* getIdentifier(const char (&Str)[StrLen])
   {
      return &Context.getIdentifiers().get(Str);
   }

   /// Helper struct that holds some info about the current state.
   /// This is useful because Sema has to jump around a lot, so packing this in
   /// a struct makes it easier to save and restore the values.
   struct SemaState {
      explicit SemaState(SemaPass& SP)
          : StateBits(SP.StateUnion),
            EvaluatingGlobalVar(SP.EvaluatingGlobalVar), S(SP.currentScope),
            DoScopeStack(move(SP.DoScopeStack)),
            TryScopeStack(move(SP.TryScopeStack)),
            ReferencedATs(SP.ReferencedATs)
      {
      }

      uint8_t StateBits;
      GlobalVarDecl* EvaluatingGlobalVar;
      Scope* S;

      std::vector<bool> DoScopeStack;
      std::vector<bool> TryScopeStack;

      SmallVectorImpl<AssociatedTypeDecl*>* ReferencedATs;
   };

   /// \return The current sema state.
   SemaState getSemaState() { return SemaState(*this); }

   /// Reset to a previous state.
   void resetState(SemaState& State)
   {
      StateUnion = State.StateBits;
      EvaluatingGlobalVar = State.EvaluatingGlobalVar;
      currentScope = State.S;
      DoScopeStack = move(State.DoScopeStack);
      TryScopeStack = move(State.TryScopeStack);
      ReferencedATs = State.ReferencedATs;
   }

   /// Clear the sema state.
   void clearState()
   {
      StateUnion = 0;
      EvaluatingGlobalVar = nullptr;
      currentScope = nullptr;

      DoScopeStack.clear();
      TryScopeStack.clear();
      ReferencedATs = nullptr;
   }

   /// Enter a new diagnostic scope, effectively ignoring any previous
   /// diagnostics.
   struct DiagnosticScopeRAII {
      explicit DiagnosticScopeRAII(SemaPass& SP, bool Disabled = false)
          : SP(SP), fatalError(SP.fatalError),
            fatalErrorInScope(SP.fatalErrorInScope),
            encounteredError(SP.EncounteredError), Disabled(Disabled),
            State(SP.Diags.saveState()), numDiags(SP.getNumDiags())
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
      SemaPass& SP;
      bool fatalError : 1;
      bool fatalErrorInScope : 1;
      bool encounteredError : 1;
      bool Disabled : 1;

      DiagnosticsEngine::DiagState State;
      size_t numDiags;
   };

   /// Register a new diagnostic consumer.
   struct DiagConsumerRAII {
      DiagConsumerRAII(SemaPass& SP, DiagnosticConsumer* PrevConsumer);
      ~DiagConsumerRAII();

   protected:
      SemaPass& SP;
      DiagnosticConsumer* PrevConsumer;
   };

   /// Register a void diagnostic consumer.
   struct IgnoreDiagsRAII : DiagConsumerRAII {
      explicit IgnoreDiagsRAII(SemaPass& SP, bool Enabled = true);
      ~IgnoreDiagsRAII();

   private:
      static std::unique_ptr<DiagnosticConsumer> NullConsumer;
   };

   /// Temporarily enter a different scope.
   struct DeclScopeRAII {
      DeclScopeRAII(SemaPass& SP, DeclContext* Ctx);

      ~DeclScopeRAII()
      {
         SP.DeclCtx = declContext;
         SP.resetState(State);
         SP.typeSubstitutions = std::move(typeSubstitutions);
      }

   private:
      SemaPass& SP;
      DeclContext* declContext;
      SemaState State;
      llvm::DenseMap<QualType, QualType> typeSubstitutions;
   };

   enum SetParentCtxDiscrim { SetParentContext };

   /// Enter a new scope that is nested in the previous scope.
   struct DeclContextRAII {
      DeclContextRAII(SemaPass& SP, DeclContext* Ctx);
      DeclContextRAII(SemaPass& SP, DeclContext* Ctx, SetParentCtxDiscrim);

      ~DeclContextRAII() { SP.DeclCtx = Prev; }

   private:
      SemaPass& SP;
      DeclContext* Prev;
   };

   /// Create a new scope for the evaluation of a global variable.
   struct EnterGlobalVarScope {
      EnterGlobalVarScope(SemaPass& SP, GlobalVarDecl* V)
          : SP(SP), Prev(SP.EvaluatingGlobalVar)
      {
         SP.EvaluatingGlobalVar = V;
      }

      ~EnterGlobalVarScope() { SP.EvaluatingGlobalVar = Prev; }

   private:
      SemaPass& SP;
      GlobalVarDecl* Prev;
   };

   /// Create a new scope for the evaluation of a compile-time expression.
   struct EnterCtfeScope {
      explicit EnterCtfeScope(SemaPass& SP) : SP(SP), previous(SP.Bits.InCTFE)
      {
         SP.Bits.InCTFE = true;
      }

      ~EnterCtfeScope() { SP.Bits.InCTFE = previous; }

   private:
      SemaPass& SP;
      bool previous;
   };

   /// \return The current sema scope.
   Scope* getScope() const { return currentScope; }

   /// \return Whether or not we're currently evaluating a compile-time
   /// expression.
   bool inCTFE() const { return Bits.InCTFE; }

   /// \return Whether or not incomplete template types are allowed to be
   /// referenced.
   bool allowIncompleteTemplateTypes() const
   {
      return Bits.AllowIncompleteTemplateTypes;
   }

   /// \return True iff the given type has a default value.
   bool hasDefaultValue(CanType type) const;

   /// Check whether or not two argument conventions are compatible.
   bool AreConventionsCompatible(ArgumentConvention Needed,
                                 ArgumentConvention Given) const;

   /// Issue all diagnostics encountered so far.
   void issueDiagnostics();

   /// The number of total issued diagnostics.
   size_t getNumDiags() const;

   /// Resize the stored diagnostics, effectively deleting the last ones after
   /// `toSize`.
   void resizeDiags(size_t toSize);

   /// True iff an error was encountered during compilation until now.
   bool encounteredError() const { return EncounteredError; }

   /// Set whether or not an error was encountered.
   void setEncounteredError(bool b) { EncounteredError = b; }

   /// Register the implementation of an associated type requirement for a record.
   void registerAssociatedTypeImpl(RecordDecl *R, AssociatedTypeDecl *AT,
                                   AliasDecl *Impl);

   /// Register the implementation of an associated type requirement for a record.
   void registerAssociatedTypeImpl(RecordDecl *R, AliasDecl *Impl);

   /// \return the implementation of an associated type requirement for a record.
   AliasDecl *getAssociatedTypeImpl(RecordDecl *R, DeclarationName Name);

   /// \return the implementation of an associated type requirement for a record.
   template<std::size_t StrLen>
   AliasDecl *getAssociatedTypeImpl(RecordDecl *R, const char (&Str)[StrLen])
   {
      return getAssociatedTypeImpl(R, getIdentifier(Str));
   }

   /// Apply the capabilities provided by constraints in the current context
   /// to a type.
   QualType ApplyCapabilities(QualType T, DeclContext* DeclCtx = nullptr,
                              bool force = false);

   /// \return The constraint set for a declaration.
   ConstraintSet* getDeclConstraints(NamedDecl* ND);

   /// \return All conformances of a record.
   ArrayRef<Conformance*> getAllConformances(RecordDecl* R);

   /// \return All conformances of a type.
   ArrayRef<Conformance*> getAllConformances(CanType T);

   // For setting some quick access flags to compilation options.
   friend CompilerInstance;

private:
   /// Pointer to the compiler instance this Sema object belongs to.
   CompilerInstance* compilerInstance;

   /// Current diagnostic consumer. Default behaviour is to store the
   /// diagnostics and emit them when Sema is destructed.
   std::unique_ptr<DiagnosticConsumer> DiagConsumer;

   /// Diagnostics Engine for this Sema object.
   DiagnosticsEngine Diags;

public:
   /// Convenience reference to the global query context.
   QueryContext& QC;

   /// Reference to the compilation's AST context.
   ASTContext& Context;

private:
   using AssociatedTypeImplMapType = llvm::DenseMap<
       RecordDecl*, llvm::DenseMap<DeclarationName, AliasDecl*>>;

   /// Pointer to the current declaration context we're in.
   DeclContext* DeclCtx;

   /// Pointer to the current compound scope we're in.
   Scope* currentScope = nullptr;

   /// Next scope ID to assign.
   unsigned NextScopeID = 1;

   /// The lookup level for each decl context.
   llvm::DenseMap<DeclContext*, LookupLevel> lookupLevels;

   /// Set of method declarations that fulfill protocol requirements.
   llvm::SmallVector<MethodDecl*, 4> ProtocolImplementations;

   /// Map of associated type implementations.
   AssociatedTypeImplMapType AssociatedTypeDeclMap;

   /// True iff runtime generics are enabled. Will be set by the
   /// CompilerInstance.
   bool RuntimeGenerics = false;

public:
   /// Functions whose body is queued for instantiation.
   llvm::SetVector<CallableDecl*> QueuedInstantiations;

private:
   /// Symbol mangler instance.
   SymbolMangler mangle;

   /// Static expression evaluator instance.
   StaticEvaluator Evaluator;

   /// Instantiator instance.
   std::unique_ptr<TemplateInstantiator> Instantiator;

   /// Stack of do / catch scopes.
   std::vector<bool> DoScopeStack;

   /// Stack of try scopes.
   std::vector<bool> TryScopeStack;

   /// Stack of await scopes.
   std::vector<bool> AwaitScopeStack;

   /// The conformance resolver instance.
   ConformanceResolver *ConfResolver;

public:
   /// Enter a new 'try' scope for exception handling.
   struct TryScopeRAII {
      explicit TryScopeRAII(SemaPass& SP) : SP(SP)
      {
         SP.TryScopeStack.emplace_back(false);
      }

      ~TryScopeRAII() { SP.TryScopeStack.pop_back(); }

      bool containsThrowingCall() const { return SP.TryScopeStack.back(); }

   private:
      SemaPass& SP;
   };

   /// Enter a new 'await' scope for async handling.
   struct AwaitScopeRAII {
      explicit AwaitScopeRAII(SemaPass& SP) : SP(SP)
      {
         SP.AwaitScopeStack.emplace_back(false);
      }

      ~AwaitScopeRAII() { SP.AwaitScopeStack.pop_back(); }

      bool containsAsyncCall() const { return SP.AwaitScopeStack.back(); }

   private:
      SemaPass& SP;
   };

   /// Enter a new 'do' scope for exception handling.
   struct DoScopeRAII {
      explicit DoScopeRAII(SemaPass& SP, bool Exhaustive) : SP(SP)
      {
         SP.DoScopeStack.emplace_back(Exhaustive);
      }

      ~DoScopeRAII() { SP.DoScopeStack.pop_back(); }

   private:
      SemaPass& SP;
   };

   /// Enter a unittest scope.
   struct UnittestRAII {
      explicit UnittestRAII(SemaPass& SP) : SP(SP)
      {
         SP.Bits.InUnitTest = true;
      }

      ~UnittestRAII() { SP.Bits.InUnitTest = false; }

   private:
      SemaPass& SP;
   };

   /// Info that is needed for a coroutine function.
   struct CoroutineInfo {
      /// The promise / awaitable type.
      QualType AwaitableType;

      /// The awaiter type.
      QualType AwaiterType;

      /// The awaited type.
      QualType AwaitedType;

      /// Default initializer for the awaitable.
      InitDecl* AwaitableInit = nullptr;

      /// The method to obtain the awaiter from the awaitable.
      MethodDecl* AwaitableGetAwaiter = nullptr;

      /// The method to resolve the awaitable with a value.
      MethodDecl* AwaitableResolve = nullptr;

      /// The awaiter 'awaitSuspend' method.
      MethodDecl* AwaitSuspend = nullptr;

      /// The awaiter 'awaitResume' method.
      MethodDecl* AwaitResume = nullptr;

      /// The awaiter 'ready' method.
      MethodDecl* AwaitReady = nullptr;

      /// The coroutine handle initializer.
      InitDecl* CoroHandleInit = nullptr;
   };

   /// \return The coroutine info for a function.
   const CoroutineInfo& getCoroutineInfo(CallableDecl* C);

   /// \return The coroutine info for a type.
   const CoroutineInfo& getCoroutineInfo(QualType Ty)
   {
      return CoroutineInfoMap[Ty];
   }

   /// Collect the coroutine info for a type.
   void collectCoroutineInfo(QualType Ty, StmtOrDecl D);

private:
   /// Information about a particular type's coroutine implementation.
   llvm::DenseMap<QualType, CoroutineInfo> CoroutineInfoMap;

   /// ILGen instance, owned by Sema.
   std::unique_ptr<ILGenPass> ILGen;

   /// True when we encountered a fatal error.
   bool fatalError : 1;

   /// True when we encountered a fatal error in the current scope.
   bool fatalErrorInScope : 1;

   /// True when we encountered any error.
   bool EncounteredError : 1;

   struct StateBits {
      bool InCTFE : 1;
      bool AllowIncompleteTemplateTypes : 1;
      bool InDefaultArgumentValue : 1;
      bool InUnitTest : 1;
      bool DontApplyCapabilities : 1;
      bool InStaticContext : 1;
   };

   union {
      uint8_t StateUnion = 0;
      StateBits Bits;
   };

   /// Referenced associated types in the current declaration constraint.
   SmallVectorImpl<AssociatedTypeDecl*>* ReferencedATs = nullptr;

   /// Type substitutions in this context introduced by constraints.
   llvm::DenseMap<QualType, QualType> typeSubstitutions;

   /// Global variable we're currently evaluating.
   GlobalVarDecl* EvaluatingGlobalVar = nullptr;

   /// Builtin declarations.
   EnumDecl* GenericArgumentValueDecl = nullptr;
   StructDecl* GenericArgumentDecl = nullptr;
   StructDecl* GenericEnvironmentDecl = nullptr;

public:
#define BITS_RAII(NAME, NEWVAL)                                                \
   struct NAME##RAII {                                                         \
      NAME##RAII(SemaPass& Sema, bool val = NEWVAL)                            \
          : Sema(Sema), prevVal(Sema.Bits.NAME)                                \
      {                                                                        \
         Sema.Bits.NAME = val;                                                 \
      }                                                                        \
                                                                               \
      ~NAME##RAII() { Sema.Bits.NAME = prevVal; }                              \
                                                                               \
   private:                                                                    \
      SemaPass& Sema;                                                          \
      bool prevVal;                                                            \
   }

   BITS_RAII(DontApplyCapabilities, true);
   BITS_RAII(InDefaultArgumentValue, true);
   BITS_RAII(InStaticContext, true);

   /// Types of 'InitializableBy' protocols.
   enum class InitializableByKind {
      Integer = 0,
      Float,
      GraphemeCluster,
      ASCII,
      CodePoint,
      Bool,
      String,
      Array,
      Dictionary,
      None,
      _Last
   };

private:
   /// Cache for looked up InitializableBy protocols.
   ProtocolDecl* InitializableBy[(int)InitializableByKind::_Last] = {nullptr};

   /// The string initializer function used for synthesized strings.
   InitDecl* StringInit = nullptr;

   /// The string += functions used for string interpolation.
   MethodDecl* StringPlusEqualsString = nullptr;

   /// Reflection builder instance, lazily initialized.
   ReflectionBuilder* ReflBuilder = nullptr;

   /// Map from instantiations to the scope in which they were instantiated
   llvm::DenseMap<NamedDecl*, NamedDecl*> InstScopeMap;

   /// Cache for transformed imported clang types.
   llvm::DenseMap<CanType, CanType> ClangTypeMap;

public:
   /// Map from instantiated decl contexts to their template for lookup.
   llvm::DenseMap<DeclContext*, DeclContext*> LookupContextMap;

   /// The unknown any type, here for convenience.
   QualType UnknownAnyTy;

   /// The error type, here for convenience.
   QualType ErrorTy;

   /// ------
   /// Methods for finding known declarations from the standard libarary.
   /// ------

   Module* getStdModule();
   Module* getPreludeModule();
   Module* getBuiltinModule();
   Module* getReflectModule();
   Module* getSysModule();
   Module* getRuntimeModule();
   Module* getAsyncModule();
   Module* getTestModule();

   FunctionDecl* getPureVirtualDecl();
   FunctionDecl* getCopyClassDecl();
   FunctionDecl* getAtomicReleaseDecl();

   StructDecl* getArrayDecl();
   StructDecl* getArrayViewDecl();
   ClassDecl* getDictionaryDecl();
   StructDecl* getStringDecl();
   StructDecl* getStringBufferDecl();
   StructDecl* getStringStorageDecl();
   StructDecl* getStringViewDecl();
   EnumDecl* getOptionDecl();
   StructDecl* getBoxDecl();
   ClassDecl* getPromiseDecl();
   ClassDecl* getFutureDecl();
   StructDecl* getCoroutineHandleDecl();

   StructDecl* getIntDecl();
   StructDecl* getUIntDecl();

   StructDecl* getInt8Decl();
   StructDecl* getUInt8Decl();
   StructDecl* getInt16Decl();
   StructDecl* getUInt16Decl();
   StructDecl* getInt32Decl();
   StructDecl* getUInt32Decl();
   StructDecl* getInt64Decl();
   StructDecl* getUInt64Decl();
   StructDecl* getInt128Decl();
   StructDecl* getUInt128Decl();

   StructDecl* getFloatDecl();
   StructDecl* getDoubleDecl();

   StructDecl* getUnsafePtrDecl();
   StructDecl* getUnsafeMutablePtrDecl();

   StructDecl* getUnsafeRawPtrDecl();
   StructDecl* getUnsafeMutableRawPtrDecl();

   StructDecl* getUnsafeBufferPtrDecl();
   StructDecl* getUnsafeMutableBufferPtrDecl();

   StructDecl* getBoolDecl();
   StructDecl* getCharacterDecl();

   StructDecl* getTypeInfoDecl();
   StructDecl* getValueWitnessTableDecl();
   StructDecl* getProtocolConformanceDecl();
   StructDecl* getExistentialContainerDecl();

   EnumDecl* getGenericArgumentValueDecl();
   StructDecl* getGenericArgumentDecl();
   StructDecl* getGenericEnvironmentDecl();

   ProtocolDecl* getAnyDecl();
   ProtocolDecl* getEquatableDecl();
   ProtocolDecl* getHashableDecl();
   ProtocolDecl* getCopyableDecl();
   ProtocolDecl* getMoveOnlyDecl();
   ProtocolDecl* getImplicitlyCopyableDecl();
   ProtocolDecl* getStringRepresentableDecl();
   ProtocolDecl* getTruthValueDecl();
   ProtocolDecl* getRawRepresentableDecl();
   ProtocolDecl* getPersistableDecl();
   ProtocolDecl* getAwaiterDecl();
   ProtocolDecl* getAwaitableDecl();

   ProtocolDecl* getInitializableByDecl(InitializableByKind Kind);

   PrecedenceGroupDecl* getDefaultPrecedenceGroup();

   InitDecl* getStringInit();
   MethodDecl* getStringPlusEqualsString();

   /// Whether or not the 'class' keyword can be used - only false if the user
   /// explicitly requested no standard library, or if the standard library
   /// could not be found.
   bool canUseClass(SourceLocation Loc);

   /// True iff `D` is in the std.reflect module.
   bool isInReflectModule(Decl* D);

   /// True iff `D` is in the builtin module.
   bool isInBuiltinModule(Decl* D);

   /// True iff `D` is a declaration in the standard library.
   bool isInStdModule(Decl* D);

   /// Get an Option<T> of the type `Ty`.
   QualType getOptionOf(QualType Ty, StmtOrDecl DependentStmt);

   /// Get an Atomic<T> of the type `Ty`.
   QualType getAtomicOf(QualType Ty);

   /// The current record context, or null if not in a record.
   RecordDecl* getCurrentRecordCtx();

   /// The current extension context, or null if not in an extension.
   ExtensionDecl* getCurrentExtensionCtx();

   /// The current extension context, or null if not in an extension.
   ExtensionDecl* getExtensionCtx(DeclContext *CurCtx);

   /// The current named decl we're checking.
   NamedDecl* getCurrentDecl() const;

   /// True iff we're evaluating a static context (i.e. a static method or
   /// property).
   bool inStaticContext() const { return Bits.InStaticContext; }

   /// Check that every declaration in a protocol extensions marked as 'default'
   /// actually overrides a protocol requirement.
   void checkProtocolExtension(ExtensionDecl* Ext, ProtocolDecl* P);

   /// -----
   /// Type-related query functions.
   /// -----

   bool IsEquatableType(QualType Ty);
   bool IsCopyableType(QualType Ty);
   bool IsMoveOnlyType(QualType Ty);
   bool IsNoOpCopyableType(QualType Ty);
   bool IsImplicitlyCopyableType(QualType Ty);
   bool IsPersistableType(QualType Ty);
   bool NeedsRetainOrRelease(QualType Ty);
   bool NeedsDeinitilization(QualType Ty);
   bool NeedsStructReturn(QualType Ty);
   bool ShouldPassByValue(QualType Ty);
   bool ContainsAssociatedTypeConstraint(QualType Ty);

   bool ConformsTo(CanType T, CanType Existential, bool anyConformance = false);
   bool ConformsTo(CanType T, ProtocolDecl* Proto,
                   bool AllowConditional = false);

   bool ConformsTo(CanType T, ProtocolDecl* Proto, ExtensionDecl *InDeclCtx,
                   bool AllowConditional = false);

   bool IsSubClassOf(ClassDecl* C, ClassDecl* Base, bool errorVal = true);

   /// Infer the types of a lambda expression from context.
   int inferLambdaArgumentTypes(LambdaExpr* LE, QualType fromTy);

   /// Transform a parameter type from an imported clang function into the
   /// appropriate standard library type.
   CanType TransformImportedCType(CanType T);

   //===-------------------------------------------------------===//
   // CTFE
   //===-------------------------------------------------------===//

   bool prepareGlobalForCtfe(VarDecl* Decl);
   void makeExtensionVisible(ExtensionDecl* D);

   void addProtocolImplementation(MethodDecl* Impl)
   {
      ProtocolImplementations.push_back(Impl);
   }

   //===-------------------------------------------------------===//
   // Lookup
   //===-------------------------------------------------------===//

   /// Prepare a DeclContext for complete name lookup.
   bool PrepareNameLookup(DeclContext *DC);
   bool FindDependencies(RecordDecl *Inst);

   /// Query lookup status of a DeclContext.
   LookupLevel getLookupLevel(DeclContext *DC) const;
   void updateLookupLevel(DeclContext *DC, LookupLevel newLevel);
   bool hasLookupLevel(DeclContext *DC, LookupLevel level) const;

private:
   template<class T, class... Args>
   void addDiagnosticArgs(diag::DiagnosticBuilder& diag, T const& first,
                          Args const&... args)
   {
      diag << first;
      addDiagnosticArgs(diag, args...);
   }

   void addDiagnosticArgs(diag::DiagnosticBuilder& diag) {}

public:
   struct ScopeGuard {
      enum Status { Enabled, Disabled };

      explicit ScopeGuard(SemaPass& S, Status st = Enabled,
                          CompoundStmt* CS = nullptr);

      ScopeGuard(SemaPass& S, CallableDecl* F,
                 bool InferrableReturnType = false)
          : S(S)
      {
         S.currentScope = new (S.getContext())
             FunctionScope(F, InferrableReturnType, S.currentScope);
      }

      ScopeGuard(SemaPass& S, MethodDecl* M, bool InferrableReturnType = false)
          : S(S)
      {
         S.currentScope = new (S.getContext())
             MethodScope(M, InferrableReturnType, S.currentScope);
      }

      ScopeGuard(SemaPass& S, LambdaExpr* L) : S(S)
      {
         S.currentScope = new (S.getContext()) LambdaScope(L, S.currentScope);
      }

      ScopeGuard(SemaPass& S, IdentifierInfo* elementName, QualType elementTy)
          : S(S)
      {
         S.currentScope = new (S.getContext())
             StaticForScope(elementName, elementTy, S.currentScope);
      }

      ScopeGuard(SemaPass& S, bool continuable, bool breakable,
                 bool isLastCase = false, bool nextCaseHasArgs = false)
          : S(S)
      {
         S.currentScope = new (S.getContext())
             LoopScope(continuable, breakable, isLastCase, nextCaseHasArgs,
                       S.currentScope);
      }

      ~ScopeGuard()
      {
         if (enabled)
            S.currentScope = S.currentScope->getEnclosingScope();
      }

   private:
      SemaPass& S;
      bool enabled = true;
   };

   template<class T> T* getSpecificScope() const
   {
      for (auto S = currentScope; S; S = S->getEnclosingScope())
         if (auto t = support::dyn_cast<T>(S))
            return t;

      return nullptr;
   }

   Scope* getCurrentScope() const { return currentScope; }

   FunctionScope* getFuncScope() const
   {
      return getSpecificScope<FunctionScope>();
   }

   MethodScope* getMethodScope() const
   {
      return getSpecificScope<MethodScope>();
   }

   LambdaScope* getLambdaScope() const
   {
      return getSpecificScope<LambdaScope>();
   }

   LoopScope* getLoopScope() const { return getSpecificScope<LoopScope>(); }
   BlockScope* getBlockScope() const { return getSpecificScope<BlockScope>(); }

   DeclResult doDestructure(DestructuringDecl* D, QualType DestructuredTy);

   MethodDecl* getEquivalentMethod(MethodDecl* Orig, RecordDecl* Inst);

   void maybeInstantiate(CandidateSet& CandSet, Statement* Caller);
   void maybeInstantiate(CandidateSet::Candidate& Cand, Statement* Caller);

   bool maybeInstantiateRecord(CandidateSet::Candidate& Cand,
                               const TemplateArgList& templateArgs,
                               Statement* Caller);

   template<class T>
   T* maybeInstantiateTemplateMember(DeclContext* DC, T* Member)
   {
      return support::cast<T>(maybeInstantiateTemplateMemberImpl(DC, Member));
   }

   NamedDecl* maybeInstantiateTemplateMemberImpl(DeclContext* LookupCtx,
                                                 NamedDecl* Member);

   CallableDecl*
   maybeInstantiateMemberFunction(CallableDecl* Fn, StmtOrDecl Caller,
                                  bool NeedImmediateInstantiation = false);

   RecordDecl* InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                 sema::FinalTemplateArgumentList* TemplateArgs);

   RecordDecl* InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                 const sema::TemplateArgList& TemplateArgs);

   AliasDecl* InstantiateAlias(SourceLocation POI, AliasDecl* td,
                               sema::FinalTemplateArgumentList* TemplateArgs);

   void checkDuplicateFunctionDeclaration(CallableDecl* C,
                                          llvm::StringRef fnKind);

   Expression* implicitCastIfNecessary(Expression* Expr, QualType destTy,
                                       bool ignoreError = false,
                                       ConversionOpts opts = CO_None,
                                       diag::MessageKind msg
                                       = diag::err_type_mismatch,
                                       SourceLocation DiagLoc = {},
                                       SourceRange DiagRange = {},
                                       bool* hadError = nullptr);

   void checkDeclaredVsGivenType(Decl* DependentDecl, Expression*& val,
                                 const SourceType& ST, QualType DeclaredType,
                                 QualType GivenType, bool IsLet,
                                 SourceLocation EqualsLoc);

   // don't allow accidentally passing two QualTypes
   Expression* implicitCastIfNecessary(Expression*, QualType, QualType,
                                       diag::MessageKind
                                       = diag::err_type_mismatch)
       = delete;

   Expression* forceCast(Expression* Expr, QualType destTy);
   Expression* castToRValue(Expression* Expr);

public:
   CallableDecl* checkFunctionReference(IdentifierRefExpr* E, CallableDecl* CD,
                                        ArrayRef<Expression*> templateArgs);

   RecordDecl *checkRecordReference(IdentifierRefExpr* E, RecordDecl* R,
                                    ArrayRef<Expression*> templateArgs);

   AliasDecl* checkAliasReference(IdentifierRefExpr* E, AliasDecl* Alias,
                                  ArrayRef<Expression*> templateArgs);

   // Checks whether the parent expression of the given expression refers to
   // a namespace rather than a value and adjusts the expression appropriately.
   bool refersToNamespace(Expression* E);

   ExprResult checkNamespaceRef(MacroExpansionExpr* Expr);
   StmtResult checkNamespaceRef(MacroExpansionStmt* Stmt);
   DeclResult checkNamespaceRef(MacroExpansionDecl* D);

   QualType getParentType(Expression* ParentExpr);

private:
   QualType HandleFieldAccess(MemberRefExpr* Expr, FieldDecl* F);
   QualType HandlePropAccess(MemberRefExpr* Expr, PropDecl* P);

   Expression* checkCustomDeref(Expression* E, QualType T);

public:
   CallExpr* CreateCall(CallableDecl* C, ArrayRef<Expression*> Args,
                        SourceLocation Loc);
   CallExpr* CreateCall(CallableDecl* C, ASTVector<Expression*>&& Args,
                        SourceLocation Loc);

   void diagnoseMemberNotFound(DeclContext* Ctx, StmtOrDecl Subject,
                               DeclarationName memberName,
                               diag::MessageKind msg
                               = diag::err_member_not_found,
                               SourceRange SR = SourceRange());

   void checkAccessibility(NamedDecl* ND, StmtOrDecl SOD);

   SubscriptDecl *getSubscriptDecl(MethodDecl *AccessorFn);

private:
   StmtOrDecl checkMacroCommon(StmtOrDecl SOD, DeclarationName MacroName,
                               DeclContext& Ctx, MacroDecl::Delimiter Delim,
                               llvm::ArrayRef<lex::Token> Tokens,
                               unsigned Kind);

public:
   FuncArgDecl* MakeSelfArg(SourceLocation Loc);

   void addDependency(Decl* ReferencedDecl);
   void addDependency(NamedDecl* DependentDecl, Decl* ReferencedDecl);

   unsigned getSerializationFile(Decl* D);

   void checkDefaultAccessibility(NamedDecl* ND);

   llvm::DenseMap<DeclarationName, NamedDecl*> BuiltinDecls;

   template<std::size_t StrLen>
   NamedDecl* getBuiltinDecl(const char (&Str)[StrLen])
   {
      auto It = BuiltinDecls.find(getIdentifier(Str));
      if (It == BuiltinDecls.end())
         return nullptr;

      return It->getSecond();
   }

   template<class T, std::size_t StrLen>
   T* getBuiltinDecl(const char (&Str)[StrLen])
   {
      return support::cast_or_null<T>(getBuiltinDecl(Str));
   }

private:
   // A namespace to which the compiler can add builtin declarations without
   // conflicting with user-defined types.
   NamespaceDecl* PrivateNamespace = nullptr;

   // Builtin namespace
   const IdentifierInfo* BuiltinIdents[64];
   bool BuiltinIdentsInitialized = false;

   Optional<bool> canUseClassVal;

   void initBuiltinIdents();
   ExprResult HandleBuiltinCall(CallExpr* C);

   // static reflection
   const IdentifierInfo* ReflectionIdents[64];
   bool ReflectionIdentsInitialized = false;

   llvm::DenseMap<AliasDecl*, Expression*> ReflectionValues;

   void initReflectionIdents();

public:
   ExprResult HandleReflectionAlias(AliasDecl* Al, Expression* Expr);

   ExprResult HandleBuiltinAlias(AliasDecl* Al, Expression* Expr,
                                 ArrayRef<Expression*> DependentTemplateArgs = {});

   void SetBuiltinAliasType(AliasDecl* A);

   NamespaceDecl* getPrivateNamespace();

   friend class ReflectionBuilder;
};

/// Helper functions for conversion options.
FLAG_ENUM(SemaPass::ConversionOpts)

} // namespace ast
} // namespace cdot

#endif // CDOT_SEMA_H
