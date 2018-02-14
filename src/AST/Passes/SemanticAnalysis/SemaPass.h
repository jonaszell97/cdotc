//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_SEMA_H
#define CDOT_SEMA_H

#include "AST/Passes/SemanticAnalysis/Template.h"
#include "AST/Passes/SemanticAnalysis/Scope/Scope.h"
#include "AST/Passes/NullASTVisitor.h"

#include "AST/ASTContext.h"
#include "AST/Attribute/Attribute.h"

#include "Basic/CastKind.h"
#include "Basic/Precedence.h"

#include "CTFE/StaticEvaluator.h"

#include "Message/Diagnostics.h"
#include "Support/Casting.h"

#include "Compiler.h"

#include <unordered_map>
#include <stack>
#include <llvm/ADT/DenseSet.h>
#include <cstdint>
#include <llvm/ADT/SmallPtrSet.h>

using std::string;
using std::pair;

namespace cdot {

namespace sema {
   class ConformanceCheckerImpl;
} // namespace sema

namespace ast {

class ILGenPass;
class DeclPass;
class OverloadResolver;
class LookupResult;

enum class ImplicitConformanceKind : unsigned char;
enum class FailureReason : unsigned char;

class ExprResolverImpl;
enum class BinaryOperatorType : unsigned int;
enum class OperatorKind : unsigned;
struct CallResult;

struct CallCompatability;

class SemaPass: public diag::DiagnosticIssuer,
                public NullASTVisitor<QualType> {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;

   explicit SemaPass(CompilationUnit &compilationUnit);
   ~SemaPass();

   Type *getBuiltinType(llvm::StringRef typeName);
   ObjectType *getObjectTy(llvm::StringRef name) const;
   ObjectType *getObjectTy(Type::BoxedPrimitive kind) const;

   CompilationUnit &getCompilationUnit() const
   {
      return *compilationUnit;
   }

   void updateCompilationUnit(CompilationUnit &CU)
   {
      compilationUnit = &CU;
   }

   ASTContext& getContext() const
   {
      return compilationUnit->getContext();
   }

   bool implicitlyCastableTo(QualType from, QualType to) const;

   void doDeclarations();
   bool doSema();
   void doILGen();

   QualType visit(Expression *node);
   void visit(Statement *node);

   void visitScoped(Statement *node);

   DeclPass* getDeclPass() const
   {
      return declPass.get();
   }

   ILGenPass &getILGen()
   {
      return *ILGen.get();
   }

   Statement *getParent(Statement *Child) const;
   void updateParent(Statement *Child, Statement *Parent) const;
   void createParentMap(Statement *Stmt) const;

   void updateParentMapForTemplateInstantiation(Statement *Template,
                                                Statement *Inst) const;

   void addDeclToContext(DeclContext &Ctx, llvm::StringRef name,
                         NamedDecl *Decl);
   void addDeclToContext(DeclContext &Ctx, NamedDecl *Decl);

   struct ExprResult {
   public:
      ExprResult(QualType type, bool had_error, bool type_dependent,
                 bool value_dependent = false)
         : type(type), had_error(had_error), type_dependent(type_dependent),
           value_dependent(value_dependent)
      { }

      QualType getType() const
      {
         return type;
      }

      bool hadError() const
      {
         return (had_error | type_dependent) != 0;
      }

      bool isTypeDependent() const
      {
         return type_dependent;
      }

      bool isValueDependent() const
      {
         return value_dependent;
      }

      operator bool() const
      {
         return hadError();
      }

   private:
      QualType type;
      bool had_error : 1;
      bool type_dependent : 1;
      bool value_dependent : 1;
   };

   ExprResult visitExpr(Statement *DependantStmt, Expression *E)
   {
      auto ty = visit(E);
      DependantStmt->setIsTypeDependent(
         E->isTypeDependent() | DependantStmt->isTypeDependent());

      DependantStmt->setIsValueDependent(
         E->isValueDependent() | DependantStmt->isValueDependent());

      DependantStmt->setHadError(
         E->hadError() | DependantStmt->hadError() | ty.isUnknownAny());

      return ExprResult(ty, DependantStmt->hadError(),
                        DependantStmt->isTypeDependent(),
                        DependantStmt->isValueDependent());
   }

   bool visitStmt(Statement *DependantStmt, Statement *Stmt)
   {
      visit(Stmt);
      DependantStmt->setIsTypeDependent(
         Stmt->isTypeDependent() | DependantStmt->isTypeDependent());

      DependantStmt->setIsValueDependent(
         Stmt->isValueDependent() | DependantStmt->isValueDependent());

      DependantStmt->setHadError(
         Stmt->hadError() | DependantStmt->hadError());

      return !stopEvaluating(Stmt);
   }

   void visitNamespaceDecl(NamespaceDecl *node);
   void visitUsingStmt(UsingStmt *node);
   void visitCompoundStmt(CompoundStmt *node);

   bool visitVarDecl(VarDecl *node);
   void visitLocalVarDecl(LocalVarDecl *node);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   bool visitDestructuringDecl(DestructuringDecl *node);
   void visitLocalDestructuringDecl(LocalDestructuringDecl *node);
   void visitGlobalDestructuringDecl(GlobalDestructuringDecl *node);

   void visitFunctionDecl(FunctionDecl *F);
   void visitCallableDecl(CallableDecl *CD);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);

   void visitClassDecl(ClassDecl *C);
   void visitStructDecl(StructDecl *S);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitEnumDecl(EnumDecl *node);
   void visitUnionDecl(UnionDecl *node);
   void visitProtocolDecl(ProtocolDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);
   void visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitInitDecl(InitDecl *node);
   void visitDeinitDecl(DeinitDecl *node);

   QualType visitIdentifierRefExpr(IdentifierRefExpr *node);
   QualType visitBuiltinExpr(BuiltinExpr *node);
   QualType visitSubscriptExpr(SubscriptExpr *node, QualType ty = {});
   QualType visitCallExpr(CallExpr *node, QualType ty = {});
   QualType visitMemberRefExpr(MemberRefExpr *node, QualType ty = {});
   QualType visitEnumCaseExpr(EnumCaseExpr *node);

   void visitForStmt(ForStmt *node);
   void visitForInStmt(ForInStmt *node);
   void visitWhileStmt(WhileStmt *node);
   void visitIfStmt(IfStmt *node);
   void visitLabelStmt(LabelStmt *node);
   void visitGotoStmt(GotoStmt *node);

   void visitMatchStmt(MatchStmt *node);
   void visitCaseStmt(CaseStmt *node);
   QualType visitExpressionPattern(ExpressionPattern *node);
   QualType visitCasePattern(CasePattern *node);
   QualType visitIsPattern(IsPattern *node);

   void visitReturnStmt(ReturnStmt *node);
   void visitBreakStmt(BreakStmt *node);
   void visitContinueStmt(ContinueStmt *node);

   QualType visitIntegerLiteral(IntegerLiteral *node);
   QualType visitFPLiteral(FPLiteral *node);
   QualType visitBoolLiteral(BoolLiteral *node);
   QualType visitCharLiteral(CharLiteral *node);

   QualType visitDictionaryLiteral(DictionaryLiteral *node);
   QualType visitArrayLiteral(ArrayLiteral *node);

   QualType visitNoneLiteral(NoneLiteral *node);
   QualType visitStringLiteral(StringLiteral *node);
   QualType visitStringInterpolation(StringInterpolation *node);
   QualType visitTupleLiteral(TupleLiteral *node);

   QualType visitExprSequence(ExprSequence *node);
   QualType visitBinaryOperator(BinaryOperator *node);
   QualType visitTertiaryOperator(TertiaryOperator *node);
   QualType visitUnaryOperator(UnaryOperator *node);

   void visitTryStmt(TryStmt *node);
   void visitThrowStmt(ThrowStmt *node);

   void visitFuncArgDecl(FuncArgDecl *node);
   QualType visitLambdaExpr(LambdaExpr *LE);
   QualType visitImplicitCastExpr(ImplicitCastExpr *node);
   QualType visitTypeRef(TypeRef *node);

   QualType visitLvalueToRvalue(LvalueToRvalue *node) { return QualType(); }
   QualType visitTemplateArgExpr(TemplateArgExpr *node) { return QualType(); }
   QualType visitConstraintExpr(ConstraintExpr *expr) { return QualType(); }

   void visitNullStmt(NullStmt *stmt) { }
   void visitModuleStmt(ModuleStmt *stmt) { }
   void visitImportStmt(ImportStmt *stmt) { }
   void visitEnumCaseDecl(EnumCaseDecl *stmt) { }
   void visitTypedefDecl(TypedefDecl *stmt) { }
   void visitTemplateParamDecl(TemplateParamDecl *stmt) { }
   void visitTranslationUnit(TranslationUnit *stmt) { }

   void visitDebugStmt(DebugStmt *node);

   void visitAliasDecl(AliasDecl *Alias);

   void visitStaticIfStmt(StaticIfStmt *node);
   void visitStaticAssertStmt(StaticAssertStmt *node);
   void visitStaticForStmt(StaticForStmt *node);
   void visitStaticPrintStmt(StaticPrintStmt *node);

   QualType visitStaticExpr(StaticExpr *node);
   QualType visitTraitsExpr(TraitsExpr *node);

   struct StaticExprResult {
      explicit StaticExprResult(Variant &&V)
         : Value(std::move(V)), HadError(false)
      {}

      StaticExprResult() : HadError(true)
      {}

      Variant &getValue()
      {
         assert(!hadError());
         return Value;
      }

      bool hadError() const
      {
         return HadError;
      }

      operator bool() const { return !hadError(); }

   private:
      Variant Value;
      bool HadError;
   };

   StaticExprResult evalStaticExpr(StaticExpr *expr);
   StaticExprResult evalStaticExpr(StaticExpr *expr,
                                   TemplateArgList const& templateArgs);

   StaticExprResult evaluateAs(StaticExpr *expr, QualType Ty);
   StaticExprResult evaluateAsBool(StaticExpr *expr);

   void addImplicitConformance(RecordDecl *R, ImplicitConformanceKind kind);

   IdentifierRefExpr *wouldBeValidIdentifier(llvm::StringRef maybeIdent);
   GlobalVarDecl *getGlobalVariable(llvm::StringRef maybeGlobal);

   template<class T, class ...Args>
   T *makeStmt(Args&& ...args)
   {
      return new (getContext()) T(std::forward<Args&&>(args)...);
   }

   PrecedenceGroup getPrecedence(QualType lhsType, OperatorKind op);
   PrecedenceGroup getPrecedence(QualType lhsType, llvm::StringRef op);

   struct BinaryOperatorResult {
      Type *resultType = nullptr;
      Type *castOperandsTo = nullptr;
   };

   BinaryOperatorResult getBinaryOperatorResult(const QualType &lhs,
                                                const QualType &rhs,
                                                OperatorKind op);

   BinaryOperatorResult getPointerArithmeticResult(const QualType &lhs,
                                                   const QualType &rhs,
                                                   OperatorKind op);

   Type* resolveDependencies(Type *Ty, TemplateArgList const& TAs);
   Type* resolveDependencies(Type *Ty, RecordDecl *R);

   template<class ...Args>
   void diagnose(Statement *Stmt, diag::MessageKind msg, Args const&... args)
   {
      if (diag::isError(msg)) {
         Stmt->setHadError(true);
         encounteredError = true;

         if (currentScope)
            currentScope->setHadError(true);
      }

      diagnostics.emplace_back(msg);
      addDiagnosticArgs(diagnostics.back(), Stmt->getSourceLoc(), args...);
   };

   void issueDiagnostics(
      llvm::MutableArrayRef<diag::DiagnosticBuilder> &diags) const
   {
      for (auto &diag : diags)
         diag << diag::cont;
   }

   template <class T>
   void issueDiagnostics(T &t)
   {
      for (auto &diag : t.getDiagnostics())
         diag << diag::cont;
   }

   void declareRecordInstantiation(RecordDecl *Inst);

   void visitRecordInstantiation(RecordDecl *R);
   void visitFunctionInstantiation(CallableDecl *C);

   void registerDelayedInstantiation(RecordDecl *R);
   void registerDelayedFunctionDecl(CallableDecl *C);

   void visitDelayedDecls();
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

   RecordDecl *getRecord(llvm::StringRef name) const;
   StructDecl *getStruct(llvm::StringRef name) const;
   ClassDecl *getClass(llvm::StringRef name) const;
   EnumDecl *getEnum(llvm::StringRef name) const;
   UnionDecl *getUnion(llvm::StringRef name) const;
   ProtocolDecl *getProtocol(llvm::StringRef name) const;
   NamespaceDecl *getNamespace(llvm::StringRef name) const;

   FunctionDecl *getAnyFn(llvm::StringRef name) const;
   TypedefDecl *getTypedef(llvm::StringRef name) const;

   DeclContext *getNearestDeclContext(Statement *Stmt) const;
   CallableDecl *getCurrentFun() const;

   struct DiagnosticScopeRAII {
      DiagnosticScopeRAII(SemaPass &SP)
         : SP(SP), fatalError(SP.fatalError),
           fatalErrorInScope(SP.fatalErrorInScope),
           encounteredError(SP.encounteredError),
           diagnostics(move(SP.diagnostics))
      {
         SP.fatalError = false;
         SP.fatalErrorInScope = false;
         SP.encounteredError = false;
         SP.diagnostics.clear();
      }

      llvm::ArrayRef<diag::DiagnosticBuilder> getAddedDiagnostics() const
      {
         return SP.diagnostics;
      }

      ~DiagnosticScopeRAII()
      {
         SP.fatalError = fatalError;
         SP.fatalErrorInScope = fatalErrorInScope;
         SP.encounteredError = encounteredError;
         SP.diagnostics = move(diagnostics);
      }

   private:
      SemaPass &SP;
      bool fatalError : 1;
      bool fatalErrorInScope : 1;
      bool encounteredError : 1;
      llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
   };

   struct DiagnosticCounter {
      DiagnosticCounter(SemaPass &SP)
         : SP(SP), initialSize(SP.diagnostics.size())
      {}

      bool hadError() const
      {
         return SP.getDiagnostics().size() > initialSize;
      }

   private:
      SemaPass &SP;
      size_t initialSize;
   };

   QualType getBoxedType(QualType type) const;
   QualType getUnboxedType(QualType type) const;
   bool hasDefaultValue(QualType type) const;

   void issueDiagnostics();

   void registerBuiltinOp(QualType opTy,
                          OperatorKind kind,
                          OperatorInfo opInfo);

   friend class DeclPass;
   friend class ILGenPass;
   friend class ExprResolverImpl;
   friend class StaticExprEvaluator;
   friend class EvaluatorImpl;
   friend class OverloadResolver;
   friend class GlobalDependencyImpl;
   friend class sema::TemplateArgListImpl;

protected:
   CompilationUnit *compilationUnit; // can't be a reference because it's a
                                     // movable type

   ASTContext &Context;
   Stage stage = Stage::Declaration;

   std::unordered_map<string, LocalVarDecl*> declarations;
   Scope *currentScope = nullptr;

   llvm::SmallPtrSet<NamedDecl*, 16> DelayedDecls;
   llvm::StringMap<GotoStmt*> UnresolvedGotos;

   StaticEvaluator Evaluator;

   RecordDecl *getCurrentRecord();

   TemplateParamDecl const* hasTemplateParam(llvm::StringRef name);
   AssociatedTypeDecl const* hasAssociatedType(llvm::StringRef name);

   bool stopEvaluating(Statement *Stmt);

   void checkConformances();
   void checkProtocolConformance(RecordDecl *R);

   template<class T, class ...Args>
   void addDiagnosticArgs(diag::DiagnosticBuilder &diag,
                          T const &first,
                          Args const &... args) {
      diag << first;
      addDiagnosticArgs(diag, args...);
   }

   void addDiagnosticArgs(diag::DiagnosticBuilder &diag) {}

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

      ScopeGuard(SemaPass &S, CallableDecl *F)
         : S(S)
      {
         S.currentScope = new (S.getContext()) FunctionScope(F, S.currentScope);
      }

      ScopeGuard(SemaPass &S, MethodDecl *M)
         : S(S)
      {
         S.currentScope = new (S.getContext()) MethodScope(M, S.currentScope);
      }

      ScopeGuard(SemaPass &S, LambdaExpr *L)
         : S(S)
      {
         S.currentScope = new (S.getContext()) LambdaScope(L, S.currentScope);
      }

      ScopeGuard(SemaPass &S, RecordDecl *R)
         : S(S)
      {
         S.currentScope = new (S.getContext()) RecordScope(R, S.currentScope);
      }

      ScopeGuard(SemaPass &S, llvm::ArrayRef<TemplateParamDecl*> params)
         : S(S)
      {
         S.currentScope = new (S.getContext()) TemplateScope(params,
                                                             S.currentScope);
      }

      ScopeGuard(SemaPass &S, bool continuable, bool breakable,
                 bool isLastCase = false, bool nextCaseHasArgs = false)
         : S(S)
      {
         S.currentScope = new (S.getContext()) LoopScope(continuable, breakable,
                                                         isLastCase,
                                                         nextCaseHasArgs);
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

   class CallCandidate {
   public:
      CallCandidate(CallCompatability &Comp,
                    llvm::ArrayRef<Expression*> resolvedArgs,
                    CallableDecl *C);

      CallCandidate(CallCompatability &Comp,
                    llvm::ArrayRef<Expression*> resolvedArgs,
                    EnumCaseDecl *Case);

      CallCandidate(CallCompatability &Comp,
                    llvm::ArrayRef<Expression*> resolvedArgs,
                    FunctionType *FTy,
                    SourceLocation declLoc);

      diag::DiagnosticBuilder &getDiagnostic()
      {
         return diagnostic;
      }

      const SourceLocation &getLoc() const
      {
         return loc;
      }

   private:
      SourceLocation loc;
      diag::DiagnosticBuilder diagnostic;
   };

   class CallResult {
   public:
      bool isCompatible() const { return compatible; }
      ~CallResult();

      FunctionDecl *getFunction() const;
      MethodDecl *getMethod() const;
      CallableDecl *getCallable() const;

      FunctionType *getFunctionType() const;
      EnumCaseDecl *getCase() const;
      TemplateArgList& getTemplateArgs();

      std::vector<CallCandidate>& getFailedCandidates();

   protected:
      bool compatible;

      explicit CallResult(bool compatible)
         : compatible(compatible)
      {}
   };

   class CompatibleCallResult: public CallResult {
   public:
      CompatibleCallResult(CallableDecl *C,
                           TemplateArgList &&list,
                           TemplateArgList &&initializerTemplateArgs = {})
         : CallResult(true), C(C),
           templateArgs(std::move(list)),
           initializerTemplateArgs(std::move(initializerTemplateArgs))
      {}

      explicit CompatibleCallResult(EnumCaseDecl *Case)
         : CallResult(true), Case(Case)
      {}

      explicit CompatibleCallResult(FunctionType *FTy)
         : CallResult(true), functionType(FTy)
      {}

      CallableDecl *getCallable() const
      {
         return C;
      }

      FunctionDecl *getFunction() const
      {
         return F;
      }

      MethodDecl *getMethod() const
      {
         return M;
      }

      EnumCaseDecl *getCase() const
      {
         return Case;
      }

      void setFunction(FunctionDecl *F)
      {
         CompatibleCallResult::F = F;
      }

      void setMethod(MethodDecl *M)
      {
         CompatibleCallResult::M = M;
      }

      FunctionType *getFunctionType() const
      {
         return functionType;
      }

      TemplateArgList& getTemplateArgs()
      {
         return templateArgs;
      }

      void setTemplateArgs(TemplateArgList &&list)
      {
         templateArgs = std::move(list);
      }

      TemplateArgList &getInitializerTemplateArgs()
      {
         return initializerTemplateArgs;
      }

      friend class CallResult;

   private:
      union {
         CallableDecl *C;
         FunctionDecl *F;
         MethodDecl *M;
         EnumCaseDecl *Case;
         FunctionType *functionType;
      };

      TemplateArgList templateArgs;
      TemplateArgList initializerTemplateArgs;

      void destroyValue();
   };

   class IncompatibleCallResult: public CallResult {
   public:
      explicit IncompatibleCallResult(
         std::vector<CallCandidate> &&failedCandidates = {})
         : CallResult(false), failedCandidates(move(failedCandidates))
      {}

      std::vector<CallCandidate> &getFailedCandidates()
      {
         return failedCandidates;
      }

      friend class CallResult;

   private:
      std::vector<CallCandidate> failedCandidates;

      void destroyValue();
   };

   using FunctionResult = std::unique_ptr<CallResult>;

   FunctionResult getUFCS(llvm::StringRef funcName,
                          llvm::ArrayRef<Expression*> args,
                          std::vector<TemplateArgExpr*> const& templateArgs = {},
                          Statement *caller = nullptr);

   FunctionResult getFunction(llvm::StringRef funcName,
                              llvm::ArrayRef<Expression*> args,
                              std::vector<TemplateArgExpr*> const& templateArgs = {},
                              Statement *caller = nullptr);

   FunctionResult getFunction(llvm::ArrayRef<FunctionDecl*> overloads,
                              llvm::ArrayRef<Expression*> args,
                              std::vector<TemplateArgExpr*> const& templateArgs = {},
                              Statement *caller = nullptr);

   FunctionResult getMethod(RecordDecl *rec,
                            llvm::StringRef methodName,
                            llvm::ArrayRef<Expression*> args = {},
                            std::vector<TemplateArgExpr*> const& templateArgs = {},
                            Statement *caller = nullptr,
                            bool isStatic = false);

   FunctionResult getMethod(RecordDecl *rec,
                            LookupResult &lookupResult,
                            llvm::ArrayRef<Expression*> args = {},
                            std::vector<TemplateArgExpr*> const& templateArgs = {},
                            Statement *caller = nullptr,
                            bool isStatic = false);

   FunctionResult getMethod(RecordDecl *rec,
                            llvm::ArrayRef<MethodDecl*> overloads,
                            llvm::ArrayRef<Expression*> args = {},
                            std::vector<TemplateArgExpr*> const& templateArgs = {},
                            Statement *caller = nullptr,
                            bool isStatic = false);

   void registerBuiltinOperators(OperatorKind opKind);

   struct ConstraintResult {
   public:
      ConstraintResult(bool hadError, bool typeDependant,
                       bool ctfeDependant,
                       llvm::SmallVector<diag::DiagnosticBuilder,
                          4> &&diagnostics)
         : error(hadError), typeDependent(typeDependant),
           satisfied(false),
           diagnostics(std::move(diagnostics))
      { }

      ConstraintResult()
         : error(false), typeDependent(false),
           satisfied(true)
      {}

      bool hadError() const
      {
         return error;
      }

      bool isTypeDependant() const
      {
         return typeDependent;
      }

      bool isSatisfied() const
      {
         return satisfied;
      }

      llvm::SmallVector<diag::DiagnosticBuilder, 4> &getDiagnostics()
      {
         return diagnostics;
      }

   private:
      bool error;
      bool typeDependent;

      bool satisfied;
      llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
   };

   ConstraintResult checkConstraint(StaticExpr* const& constraint,
                                    TemplateArgList &TAs);

   FunctionResult checkAnonymousCall(FunctionType *FTy,
                                     llvm::ArrayRef<Expression*> args,
                                     Statement *Caller = nullptr);

   void maybeInstantiateMemberFunction(MethodDecl *M,
                                       const SourceLocation &callerLoc);

   RecordDecl* InstantiateRecord(RecordDecl *R,
                                 TemplateArgList &&TAs,
                                 SourceLocation loc = {});

   FunctionResult getCase(EnumDecl *E,
                          const string &caseName,
                          llvm::ArrayRef<Expression*> args = {},
                          Statement *Caller = nullptr);

   MetaType *getMetaType(Type *forType);

   void pushTemplateArgs(TemplateArgList const& list)
   {
      templateArgStack.push_back(&list);
   }

   void popTemplateArgs()
   {
      templateArgStack.pop_back();
   }

   struct TemplateArgRAII {
      TemplateArgRAII(SemaPass &S, TemplateArgList const& list) : S(S)
      {
         S.pushTemplateArgs(list);
      }

      ~TemplateArgRAII()
      {
         S.popTemplateArgs();
      }

   private:
      SemaPass &S;
   };

   sema::ResolvedTemplateArg const* getTemplateArg(llvm::StringRef name)
   {
      for (auto &list : templateArgStack)
         if (auto TA = list->getNamedArg(name))
            return TA;

      return nullptr;
   }

private:
   bool implicitCastIfNecessary(Expression* target,
                                const QualType &originTy,
                                const QualType &destTy,
                                bool preCondition = true,
                                bool ignoreError = false);

   void forceCast(Expression* target,
                  const QualType &originTy,
                  const QualType &destTy);

   void lvalueToRvalue(Expression* & target);
   void toRvalueIfNecessary(QualType &ty, Expression* &target,
                            bool preCond = true);

   QualType VisitSubExpr(Expression *, QualType);

   llvm::StringSet<> labels;
   llvm::SmallPtrSet<Statement*, 8> VisitedGlobalDecls;
   llvm::DenseSet<uintptr_t> VisitedDecls;

   bool alreadyVisited(Statement *stmt)
   {
      return !VisitedDecls.insert((uintptr_t)stmt).second;
   }

   size_t numGlobals = 0;

   std::unique_ptr<DeclPass> declPass;
   std::unique_ptr<ILGenPass> ILGen;

   llvm::SmallVector<TemplateArgList const*, 2> templateArgStack;

   bool fatalError : 1;
   bool fatalErrorInScope : 1;
   bool encounteredError : 1;

   QualType UnknownAnyTy;

   struct BuiltinOperator {
      BuiltinOperator(FunctionType *FuncTy, const OperatorInfo &operatorInfo)
         : FuncTy(FuncTy), operatorInfo(operatorInfo)
      { }

      FunctionType *getFuncTy() const
      {
         return FuncTy;
      }

      const OperatorInfo &getOperatorInfo() const
      {
         return operatorInfo;
      }

   private:
      FunctionType *FuncTy;
      OperatorInfo operatorInfo;
   };

   std::unordered_map<OperatorKind, BuiltinOperator> BuiltinBinaryOperators;
   std::unordered_map<OperatorKind, BuiltinOperator> BuiltinUnaryOperators;

   void checkDeclTypeReturnType(CallableDecl *C);

   ExprResult unify(Statement *Stmt,
                    std::vector<Expression* > const&exprs);

   bool checkPossibleTemplateArgList(
                                 Expression *Expr,
                                 std::vector<TemplateArgExpr*> &templateArgs);

   // IdentifierRef

   QualType HandleBuiltinIdentifier(IdentifierRefExpr *node);
   QualType checkFunctionReference(IdentifierRefExpr *node, CallableDecl *F);
   QualType checkSuper(IdentifierRefExpr *node);
   QualType checkSelf(IdentifierRefExpr *node);

   struct AliasResult {
      explicit AliasResult(AliasDecl *Alias)
         : TypeDependent(false), ValueDependent(false), HadError(false),
           Result(Alias)
      {}

      explicit AliasResult(std::vector<diag::DiagnosticBuilder> &&Candidates)
         : TypeDependent(false), ValueDependent(false), HadError(true),
           FailedCandidates(std::move(Candidates))
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

      bool hadError() const
      {
         return HadError;
      }

      AliasDecl *getAlias()
      {
         assert(!HadError && !TypeDependent && !ValueDependent);
         return Result;
      }

      const std::vector<diag::DiagnosticBuilder> &getDiagnostics() const
      {
         assert(HadError);
         return FailedCandidates;
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
      std::vector<diag::DiagnosticBuilder> FailedCandidates;
   };

   AliasResult checkAlias(LookupResult &lookupResult,
                          llvm::ArrayRef<TemplateArgExpr*> templateArgs,
                          SourceLocation loc);

   bool checkAlias(AliasDecl *alias,
                   std::vector<diag::DiagnosticBuilder> &Diagnostics);

   // MemberRef

   QualType HandleStaticMember(MemberRefExpr *node, RecordDecl *R);
   QualType HandleStaticTypeMember(MemberRefExpr *node, Type *Ty);

   QualType HandleEnumCase(MemberRefExpr *node, EnumDecl *E);

   QualType HandleFieldAccess(MemberRefExpr *node, RecordDecl *R,
                              FieldDecl *F = nullptr);

   QualType HandlePropAccess(MemberRefExpr *node, RecordDecl *rec,
                             PropDecl *P = nullptr);

   QualType HandleEnumMember(MemberRefExpr *node, EnumDecl *E);
   QualType HandleTupleMember(MemberRefExpr *node, TupleType *ty);
   QualType HandleUnionMember(MemberRefExpr *node, UnionDecl *U);

   QualType HandleTypeMember(MemberRefExpr *node, Type *Ty);
   QualType HandleNamespaceMember(MemberRefExpr *node, NamespaceDecl *NS);

   QualType HandleMethodReference(MemberRefExpr *node, RecordDecl *R);

   // CallExpr

   void HandleFunctionCall(CallExpr* node,
                           llvm::ArrayRef<FunctionDecl*> overloads);
   bool TryTypedef(CallExpr *node, TypedefDecl *td = nullptr);

   void HandleBuiltinCall(CallExpr*);

   void HandleMethodCall(CallExpr *node, QualType ty);
   void HandleDeclContextMember(CallExpr *node, DeclContext *Ctx);

   void HandleStaticTypeCall(CallExpr *node, Type *Ty);
   void HandleOtherTypeCall(CallExpr *node, Type *ty);

   void HandleConstructorCall(CallExpr*, RecordDecl *R);
   void HandleCallOperator(CallExpr *node, QualType ty);
   void HandleAnonCall(CallExpr *node, QualType ty);

   void diagnoseMemberNotFound(DeclContext *Ctx,
                               Statement *Subject,
                               llvm::StringRef memberName,
                               llvm::StringRef hint = "");

   // BinaryOperator

   QualType HandleBinaryOperator(QualType &lhs, QualType &rhs,
                                 BinaryOperatorType kind,
                                 BinaryOperator *node);

   QualType HandleTypePredicate(QualType &lhs,
                                BinaryOperator *node);

   QualType HandleCastOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleAssignmentOp(QualType &lhs, QualType &rhs,
                               BinaryOperator *node);

   QualType tryBinaryOperatorMethod(QualType& lhs,
                                    QualType& rhs,
                                    BinaryOperator *node,
                                    const string &opName);

   QualType tryFreeStandingBinaryOp(QualType& lhs, QualType& rhs,
                                    BinaryOperator *node,
                                    const string &opName);

   QualType tryFreeStandingUnaryOp(QualType& lhs,
                                   UnaryOperator *node,
                                   const string &opName);

   void checkClassAccessibility(RecordDecl* cl, Expression* cause);

   void checkMemberAccessibility(RecordDecl* record,
                                 const string& memberName,
                                 const AccessModifier& access,
                                 Expression* cause);

   enum class NoMatchingCallKind : unsigned char {
      Function,
      Method,
      Case,
      AnonymousCall
   };

   void diagnoseNoMatchingCall(CallResult *Res,
                               NoMatchingCallKind kind,
                               llvm::StringRef name,
                               CallExpr *expr);

   void diagnoseAmbiguousCall(llvm::ArrayRef<CallableDecl*> perfectMatches,
                              Statement *Caller);

   void ApplyCasts(std::vector<Expression* > &args,
                   FunctionType *FuncTy);

   void PrepareCallArgs(std::vector<Expression* >& args,
                        FunctionType *FuncTy);

   void PrepareCallArgs(std::vector<Expression* >& args,
                        CallableDecl *C);

   void calculateRecordSizes();

   bool checkFunctionCompatibility(
                           NamedDecl *C,
                           OverloadResolver &Resolver,
                           std::unique_ptr<CompatibleCallResult> &Res,
                           llvm::SmallVectorImpl<CallableDecl*> &perfectCalls,
                           std::vector<CallCandidate> &failedCandidates,
                           size_t &bestMatch,
                           size_t &maxSatisfiedConstraints);

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
};

} // namespace ast
} // namespace cdot


#endif //CDOT_SEMA_H
