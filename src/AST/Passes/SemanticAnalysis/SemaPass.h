//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_TYPECHECKVISITOR_H
#define CDOT_TYPECHECKVISITOR_H

#include <unordered_map>
#include <stack>
#include <llvm/ADT/DenseSet.h>

#include "Template.h"

#include "../AbstractPass.h"
#include "../../Attribute/Attribute.h"
#include "../../../Util.h"
#include "../../../Variant/Type/ObjectType.h"

#include "../../../Variant/Type/Generic.h"
#include "../../../Basic/CastKind.h"
#include "../../../Basic/Precedence.h"
#include "../../../Message/Diagnostics.h"

class SymbolTable;

using std::string;
using std::pair;

namespace cdot {

struct Namespace;
struct Alias;

class Type;
class MetaType;
struct Typedef;

struct Variable;

class Callable;
struct FunctionTemplateInstantiation;

namespace cl {

struct Template;

struct EnumCase;
class Class;
struct Method;
class Struct;
class Protocol;
class Union;
class Enum;
struct Field;
class Property;

} // namespace cl

struct Scope {
   Scope()
      : isLambdaRoot(false), returnable(false),
        nonContinuableMatchStmt(false), isLastCase(false)
   {}

   size_t id;

   union {
      Callable *function = nullptr;
      cl::Method *method;
   };

   bool isLambdaRoot : 1;
   bool returnable   : 1;
   bool nonContinuableMatchStmt : 1;
   bool isLastCase : 1;

   Scope* enclosingScope = nullptr;
   std::set<ast::Statement*>* captures = nullptr;
};

} // namespace cdot


namespace cdot {

namespace sema {

class ConformanceCheckerImpl;

} // namespace sema

namespace ast {

class ILGenPass;
class DeclPass;

enum class FailureReason : unsigned char;

class ExprResolverImpl;
enum class BinaryOperatorType : unsigned int;
enum class OperatorKind : unsigned;
struct CallResult;

struct CallCompatability;

class SemaPass: public AbstractPass<SemaPass, QualType>,
                public diag::DiagnosticIssuer {
public:
   using TemplateArgList = cdot::sema::TemplateArgList;

   SemaPass();
   ~SemaPass();

   void run(std::vector<CompilationUnit> &CUs);

   QualType visit(Expression *node);
   void visit(Statement *node)
   {
      return AbstractPass::visit(node);
   }

   template<class T>
   auto visit(const std::shared_ptr<T> &node)
      -> decltype(visit(node.get()))
   {
      return visit(node.get());
   }

   DeclPass* getDeclPass() const
   {
      return declPass.get();
   }

   std::vector<cl::Record*> const& getSelfStack() const;

   struct ExprResult {
   public:
      ExprResult(QualType type, bool had_error, bool type_dependant,
                 bool value_dependant = false)
         : type(type), had_error(had_error), type_dependant(type_dependant),
           value_dependant(value_dependant)
      { }

      QualType getType() const
      {
         return type;
      }

      bool hadError() const
      {
         return (had_error | type_dependant) != 0;
      }

      operator bool() const
      {
         return hadError();
      }

   private:
      QualType type;
      bool had_error : 1;
      bool type_dependant : 1;
      bool value_dependant : 1;
   };

   ExprResult visitExpr(Statement *DependantStmt, Expression *E)
   {
      auto ty = visit(E);
      DependantStmt->setIsTypeDependent(
         E->isTypeDependant() | DependantStmt->isTypeDependant());

      DependantStmt->setIsValueDependent(
         E->isValueDependant() | DependantStmt->isValueDependant());

      DependantStmt->setHadError(
         E->hadError() | DependantStmt->hadError());

      return ExprResult(ty, E->hadError(), E->isTypeDependant(),
                        E->isValueDependant());
   }

   ExprResult visitExpr(Statement *DependantStmt,
                        std::shared_ptr<Expression> const& E) {
      return visitExpr(DependantStmt, E.get());
   }

   bool visitStmt(Statement *DependantStmt, Statement *Stmt)
   {
      visit(Stmt);
      DependantStmt->setIsTypeDependent(
         Stmt->isTypeDependant() | DependantStmt->isTypeDependant());

      DependantStmt->setIsValueDependent(
         Stmt->isValueDependant() | DependantStmt->isValueDependant());

      DependantStmt->setHadError(
         Stmt->hadError() | DependantStmt->hadError());

      return !stopEvaluating(Stmt);
   }

   bool visitStmt(Statement *DependantStmt,
                  std::shared_ptr<Statement> const& Stmt) {
      return visitStmt(DependantStmt, Stmt.get());
   }

   void visitNamespaceDecl(NamespaceDecl *node);
   void visitUsingStmt(UsingStmt *node);
   void visitCompoundStmt(CompoundStmt *node);

   bool visitVarDecl(VarDecl *node);
   void visitLocalVarDecl(LocalVarDecl *node);
   void visitGlobalVarDecl(GlobalVarDecl *node);

   void visitFunctionDecl(FunctionDecl *node);
   void visitCallableDecl(CallableDecl *node);
   void visitDeclareStmt(DeclareStmt *node);

   void visitRecordCommon(RecordDecl *node);
   void visitRecordDecl(RecordDecl *node);

   void visitClassDecl(ClassDecl *node);
   void visitExtensionDecl(ExtensionDecl *node);
   void visitEnumDecl(EnumDecl *node);
   void visitUnionDecl(UnionDecl *node);
   void visitProtocolDecl(ProtocolDecl *node);

   void visitFieldDecl(FieldDecl *node);
   void visitPropDecl(PropDecl *node);
   void visitAssociatedTypeDecl(AssociatedTypeDecl *node);

   void visitMethodDecl(MethodDecl *node);
   void visitConstrDecl(ConstrDecl *node);
   void visitDestrDecl(DestrDecl *node);

   QualType visitIdentifierRefExpr(IdentifierRefExpr *node);
   QualType visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node);
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
   QualType visitLambdaExpr(LambdaExpr *node);
   QualType visitImplicitCastExpr(ImplicitCastExpr *node);
   QualType visitTypeRef(TypeRef *node);

   void visitDebugStmt(DebugStmt *node);

   void visitAliasDecl(AliasDecl *node);

   void visitStaticIfStmt(StaticIfStmt *node);
   void visitStaticAssertStmt(StaticAssertStmt *node);
   void visitStaticForStmt(StaticForStmt *node);
   void visitStaticPrintStmt(StaticPrintStmt *node);

   QualType visitStaticExpr(StaticExpr *node);
   QualType visitTraitsExpr(TraitsExpr *node);

   Variant evalStaticExpr(StaticExpr *node,
                          TemplateArgList const& templateArgs = {},
                          bool useSavedResult = true);

   bool wouldBeValidIdentifier(llvm::StringRef maybeIdent);
   Variable *getGlobalVariable(llvm::StringRef maybeGlobal);

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
   Type* resolveDependencies(Type *Ty,
                            llvm::ArrayRef<cl::AssociatedType> AssociatedTys);

   template<class ...Args>
   void diagnose(Statement *Stmt, diag::MessageKind msg, Args const&... args)
   {
      Stmt->setHadError(diag::isError(msg));

      encounteredError |= Stmt->hadError();

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

   void declareRecordInstantiation(cl::Record *Inst);

   void visitRecordInstantiation(cl::Record *R);
   void visitFunctionInstantiation(Callable *C);

   void addDelayedInstantiation(cl::Record *R);

   enum class Stage {
      Declaration = 0,
      Sema,
      ILGen,
   };

   Stage getStage() const
   {
      return stage;
   }

   friend class DeclPass;
   friend class ExprResolverImpl;
   friend class StaticExprEvaluator;
   friend class EvaluatorImpl;
   friend class OverloadResolver;
   friend class GlobalDependencyImpl;
   friend class sema::TemplateArgListImpl;

protected:
   Stage stage = Stage::Declaration;

   std::unordered_map<string, LocalVarDecl*> declarations;
   std::stack<Scope> Scopes;

   llvm::SmallPtrSet<cl::Record*, 16> DelayedInstantiations;
   llvm::StringMap<GotoStmt*> UnresolvedGotos;

   cl::Record *currentClass();

   Scope* currentScope()
   {
      return &Scopes.top();
   }

   size_t lastScopeID = 0;

   TemplateParameter const* hasTemplateParam(llvm::StringRef name);
   cl::AssociatedType const* hasAssociatedType(llvm::StringRef name);

   void pushScope();
   void popScope();

   void pushFunctionScope(Callable *func);
   void pushMethodScope(cl::Method *m);

   void popFunctionScope();

   void pushLoopScope(bool continuable = true, bool breakable = true);
   void popLoopScope(bool continuable = true, bool breakable = true);

   void pushRecordScope(cl::Record *cl);
   void popRecordScope();

   bool stopEvaluating(Statement *Stmt);
   bool stopEvaluating(std::shared_ptr<Statement> const &Stmt);

   template<class T, class ...Args>
   void addDiagnosticArgs(diag::DiagnosticBuilder &diag,
                          T const &first,
                          Args const &... args) {
      diag << first;
      addDiagnosticArgs(diag, args...);
   }

   void addDiagnosticArgs(diag::DiagnosticBuilder &diag) {}

   void issueDiagnostics();

   struct ScopeGuard {
      enum Kind {
         Normal, Method, Function, Loop, Record
      };

      explicit ScopeGuard(SemaPass &S)
         : S(S), kind(Normal), continuable(false), breakable(false)
      {
         S.pushScope();
      }

      ScopeGuard(SemaPass &S, Callable *func)
         : S(S), kind(Function), continuable(false), breakable(false)
      {
         S.pushFunctionScope(func);
      }

      ScopeGuard(SemaPass &S, cl::Method *m)
         : S(S), kind(Function), continuable(false), breakable(false)
      {
         S.pushMethodScope(m);
      }

      ScopeGuard(SemaPass &S, bool continuable, bool breakable)
         : S(S), kind(Loop), continuable(continuable), breakable(breakable)
      {
         S.pushLoopScope(continuable, breakable);
      }

      ScopeGuard(SemaPass &S, cl::Record *R)
         : S(S), kind(Record), continuable(false), breakable(false)
      {
         S.pushRecordScope(R);
      }

      ~ScopeGuard() {
         switch (kind) {
            case Normal: S.popScope(); break;
            case Function:
            case Method:
               S.popFunctionScope(); break;
            case Loop:
               S.popLoopScope(continuable, breakable); break;
            case Record:S.popRecordScope(); break;
         }
      }

   private:
      SemaPass &S;
      Kind kind;
      bool continuable : 1;
      bool breakable : 1;
   };

   string declareVariable(const string &name,
                          const QualType &type,
                          bool isGlobal,
                          bool typeDependant,
                          const SourceLocation &loc);

   cl::Record *getRecord(llvm::StringRef name,
                         std::vector<TemplateArg> const& templateArgs,
                         const SourceLocation &loc);

   struct VarResult {
      Variable *V;
      bool escapesLambdaScope;
   };

   bool isPrivateNamespaceAccessible(size_t id);

   VarResult getVariable(llvm::StringRef name,
                         AstNode *cause = nullptr);

   class CallCandidate {
   public:
      CallCandidate(CallCompatability &Comp,
                    Callable *C);

      CallCandidate(CallCompatability &Comp,
                    cl::EnumCase *Case);

      CallCandidate(CallCompatability &Comp,
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

      Function *getFunction() const;
      cl::Method *getMethod() const;
      Callable *getCallable() const;

      FunctionType *getFunctionType() const;
      cl::EnumCase *getCase() const;
      TemplateArgList& getTemplateArgs();
      std::vector<QualType> const& getResolvedGivenArgs() const;

      std::vector<CallCandidate>& getFailedCandidates();

   protected:
      bool compatible;

      explicit CallResult(bool compatible)
         : compatible(compatible)
      {}
   };

   class CompatibleCallResult: public CallResult {
   public:
      CompatibleCallResult(Callable *C,
                           std::vector<QualType> &&resolvedGivenArgs,
                           TemplateArgList &&list,
                           TemplateArgList &&initializerTemplateArgs = {})
         : CallResult(true), C(C), resolvedGivenArgs(move(resolvedGivenArgs)),
           templateArgs(std::move(list)),
           initializerTemplateArgs(std::move(initializerTemplateArgs))
      {}

      explicit CompatibleCallResult(cl::EnumCase *Case,
                                    std::vector<QualType> &&resolvedGivenArgs)
         : CallResult(true), Case(Case),
           resolvedGivenArgs(move(resolvedGivenArgs))
      {}

      explicit CompatibleCallResult(FunctionType *FTy,
                                    std::vector<QualType> &&resolvedGivenArgs)
         : CallResult(true), functionType(FTy),
           resolvedGivenArgs(move(resolvedGivenArgs))
      {}

      Callable *getCallable() const
      {
         return C;
      }

      Function *getFunction() const
      {
         return F;
      }

      cl::Method *getMethod() const
      {
         return M;
      }

      cl::EnumCase *getCase() const
      {
         return Case;
      }

      void setFunction(Function *F)
      {
         CompatibleCallResult::F = F;
      }

      void setMethod(cl::Method *M)
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

      const std::vector<QualType> &getResolvedGivenArgs() const
      {
         return resolvedGivenArgs;
      }

      friend class CallResult;

   private:
      union {
         Callable *C;
         Function *F;
         cl::Method *M;
         cl::EnumCase *Case;
         FunctionType *functionType;
      };

      std::vector<QualType> resolvedGivenArgs;
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

   FunctionResult getUFCS(const string& funcName,
                          std::vector<Argument>& args,
                          std::vector<TemplateArg> const& templateArgs = {},
                          Statement *caller = nullptr);

   FunctionResult getFunction(const string& funcName,
                              std::vector<Argument>& args,
                              std::vector<TemplateArg> const& templateArgs = {},
                              Statement *caller = nullptr);

   FunctionResult getMethod(cdot::cl::Record *rec,
                            const string& methodName,
                            const std::vector<Argument>& args = {},
                            std::vector<TemplateArg> const& templateArgs = {},
                            Statement *caller = nullptr,
                            bool isStatic = false);

   struct ConstraintResult {
   public:
      ConstraintResult(bool hadError, bool typeDependant,
                       bool ctfeDependant,
                       llvm::SmallVector<diag::DiagnosticBuilder,
                          4> &&diagnostics)
         : error(hadError), typeDependant(typeDependant),
           ctfeDependant(ctfeDependant), satisfied(false),
           diagnostics(std::move(diagnostics))
      { }

      ConstraintResult()
         : error(false), typeDependant(false), ctfeDependant(false),
           satisfied(true)
      {}

      bool hadError() const
      {
         return error;
      }

      bool isTypeDependant() const
      {
         return typeDependant;
      }

      bool isCtfeDependant() const
      {
         return ctfeDependant;
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
      bool typeDependant;
      bool ctfeDependant;

      bool satisfied;
      llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
   };

   ConstraintResult checkConstraint(
                                 std::shared_ptr<StaticExpr> const& constraint,
                                 TemplateArgList &TAs);

   FunctionResult checkAnonymousCall(FunctionType *FTy,
                                     const std::vector<Argument>& args,
                                     SourceLocation const& loc);

   void maybeInstantiateMemberFunction(cl::Method *M,
                                       const SourceLocation &callerLoc);

   cl::Record *InstantiateRecord(cl::Record *R,
                                 TemplateArgList &&TAs,
                                 SourceLocation loc = {});

   FunctionResult getCase(cdot::cl::Enum *en,
                          const string &caseName,
                          std::vector<Argument> const& args = {});

   bool hasVariable(string name);

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
   bool implicitCastIfNecessary(std::shared_ptr<Expression> &target,
                                const QualType &originTy,
                                const QualType &destTy,
                                bool preCondition = true,
                                bool ignoreError = false);

   void forceCast(std::shared_ptr<Expression> &target,
                  const QualType &originTy,
                  const QualType &destTy);

   void lvalueToRvalue(std::shared_ptr<Expression>& target);
   void toRvalueIfNecessary(QualType &ty, std::shared_ptr<Expression> &target,
                            bool preCond = true);

   void CopyScopeProps(
      Scope* src,
      Scope* dst
   );

   std::set<string> labels = {};

   QualType VisitSubExpr(Expression *, QualType);

   llvm::DenseSet<uintptr_t> VisitedDecls;

   bool alreadyVisited(Statement *stmt)
   {
      return !VisitedDecls.insert((uintptr_t)stmt).second;
   }

   size_t BreakStack = 0;
   size_t ContinueStack = 0;
   std::stack<QualType> ReturnTypeStack;

   std::unique_ptr<DeclPass> declPass;
   std::unique_ptr<ILGenPass> ILGen;

   llvm::SmallVector<TemplateArgList const*, 2> templateArgStack;

   bool fatalError : 1;
   bool fatalErrorInScope : 1;
   bool encounteredError : 1;

   void pushNamespace(const string &ns);
   void popNamespace();

   void clearCurrentNamespace();

   void importNamespace(const string &ns);

   void pushTemplateParams(std::vector<TemplateParameter>* params);
   void popTemplateParams();

   llvm::ArrayRef<size_t> importedNamespaces();

   Callable *getCurrentFunction();

   void visitTypeInferredGlobals();
   void checkDeclTypeReturnType(Callable *C);

   Type *getPlaceholderType(Expression *expr);

   string ns_prefix();

   bool tryDestructure(QualType givenTy, VarDecl *node);

   ExprResult unify(Statement *Stmt,
                    std::vector<std::shared_ptr<Expression>> const&exprs);

   Type *resolveNamespaceReference(IdentifierRefExpr *node);

   // IdentifierRef

   QualType HandleBuiltinIdentifier(IdentifierRefExpr *node);
   QualType tryFunctionReference(IdentifierRefExpr *node);
   QualType trySuper(IdentifierRefExpr *node);
   QualType trySelf(IdentifierRefExpr *node, bool guaranteedSelf);
   QualType tryFunctionArg(IdentifierRefExpr *node);
   Variant tryAlias(llvm::StringRef ident,
                    llvm::ArrayRef<TemplateArg> templateArgs,
                    Expression *expr);

   // MemberRef

   QualType HandleStaticMember(MemberRefExpr *node, cl::Record *R);
   QualType HandleStaticTypeMember(MemberRefExpr *node, Type *Ty);

   QualType HandleEnumCase(MemberRefExpr *node, cl::Enum *E);

   QualType HandleFieldAccess(MemberRefExpr *node, cl::Record *R,
                              cl::Field *F = nullptr);

   QualType HandlePropAccess(MemberRefExpr *node, cl::Record *rec,
                             cl::Property *P = nullptr);

   QualType HandleEnumMember(MemberRefExpr *node, cl::Enum *E);
   QualType HandleTupleMember(MemberRefExpr *node, TupleType *ty);
   QualType HandleUnionMember(MemberRefExpr *node, cl::Union *U);

   QualType HandleTypeMember(MemberRefExpr *node, Type *Ty);
   QualType HandleNamespaceMember(MemberRefExpr *node, Namespace *NS);

   QualType HandleMethodReference(MemberRefExpr *node, cl::Record *R);

   // CallExpr

   void HandleFunctionCall(CallExpr*, llvm::StringRef ident);
   void HandleBuiltinCall(CallExpr*);

   void HandleMethodCall(CallExpr *node, QualType ty);
   void HandleNamespaceMember(CallExpr *node, Namespace *NS);

   void HandleStaticTypeCall(CallExpr *node, Type *Ty);
   void HandleOtherTypeCall(CallExpr *node, Type *ty);

   void HandleConstructorCall(CallExpr*, llvm::StringRef ident);
   void HandleCallOperator(CallExpr *node, QualType ty);
   void HandleAnonCall(CallExpr *node, QualType ty);

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
                                    const string &opName,
                                    bool isAssignment);

   QualType tryFreeStandingBinaryOp(QualType& lhs, QualType& rhs,
                                    BinaryOperator *node,
                                    const string &opName,
                                    bool isAssignment);

   QualType tryFreeStandingUnaryOp(QualType& lhs,
                                   UnaryOperator *node,
                                   const string &opName);

   void checkClassAccessibility(cdot::cl::Record* cl, Expression* cause);

   void checkMemberAccessibility(cdot::cl::Record* record,
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

   void diagnoseAmbiguousCall(llvm::ArrayRef<Callable*> perfectMatches,
                              Statement *Caller);

   void PrepareCallArgs(std::vector<std::shared_ptr<Expression>>& args,
                        std::vector<Argument>& givenArgs,
                        std::vector<Argument> const& declaredArgs);

   void ApplyCasts(std::vector<std::shared_ptr<Expression>> &args,
                   std::vector<QualType> const& givenArgs,
                   const std::vector<Argument> &declaredArgs);

   void CopyNodeProperties(Expression *src, Expression *dst);

   cl::Method *setterMethod = nullptr;

   cl::Record *getRecord(llvm::StringRef name);
   cl::Struct *getStruct(llvm::StringRef name);
   cl::Enum *getEnum(llvm::StringRef name);
   cl::Union *getUnion(llvm::StringRef name);

   Namespace *getNamespace(llvm::StringRef name);
   llvm::ArrayRef<Alias*> getAliases(llvm::StringRef name);
   Function *getAnyFn(llvm::StringRef name);
   Typedef *getTypedef(llvm::StringRef name);
   llvm::ArrayRef<Function*> getFunctionOverloads(llvm::StringRef name);

   void calculateRecordSizes();

   struct DeclScopeRAII {
      DeclScopeRAII(SemaPass &S, DeclPass &DP);
      ~DeclScopeRAII();

   private:
      SemaPass &S;
      DeclPass *savedPass;
      std::stack<Scope> Scopes;
   };

   struct ScopeStackRAII {
      ScopeStackRAII(SemaPass &S)
         : S(S), Scopes(std::move(S.Scopes))
      {
         new (&S.Scopes) std::stack<Scope>();
         S.Scopes.push(Scope{});
      }

      ~ScopeStackRAII()
      {
         new (&S.Scopes) std::stack<Scope>(std::move(Scopes));
      }

   private:
      SemaPass &S;
      std::stack<Scope> Scopes;
   };
};

} // namespace ast
} // namespace cdot


#endif //CDOT_TYPECHECKVISITOR_H
