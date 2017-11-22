//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_TYPECHECKVISITOR_H
#define CDOT_TYPECHECKVISITOR_H

#include <unordered_map>
#include <stack>
#include "../AbstractPass.h"
#include "Function.h"
#include "../../../Util.h"
#include "../../Attribute/Attribute.h"
#include "../../../Variant/Type/ObjectType.h"
#include "Builtin.h"

#include "../../../Variant/Type/Generic.h"
#include "../../Statement/Declaration/DeclStmt.h"

class SymbolTable;

using std::string;
using std::pair;

namespace cdot {

class Type;
class MetaType;
struct Typedef;

struct Variable;

class Callable;
struct FunctionTemplateInstantiation;

namespace cl {

struct Template;
class Class;
class Method;
class Enum;

} // namespace cl

struct Scope {
   size_t id;

   union {
      Callable *function;
      cl::Method *method;
   };

   bool isLambdaRoot = false;
   bool returnable = false;

   Scope* enclosingScope = nullptr;
   std::set<std::string>* captures = nullptr;
};

} // namespace cdot


namespace cdot {
namespace ast {

enum class BinaryOperatorType : unsigned int;

class SemaPass: public AbstractPass {
public:
   explicit SemaPass();
   void run(std::vector<CompilationUnit> &CUs);

   void doInitialPass(const std::shared_ptr<Statement>& statement);
   void doInitialPass(
      const std::vector<std::shared_ptr<Statement>>& statements);

   void visit(AstNode *node) {}
   void visit(NamespaceDecl *node);
   void visit(UsingStmt *node);
   void visit(CompoundStmt *node);

   void visit(DeclStmt *node);
   void visit(FunctionDecl *node);
   void visit(CallableDecl *node);
   void visit(DeclareStmt *node);

   void visit(ClassDecl *node);
   void visit(ExtensionDecl *node);
   void visit(EnumDecl *node);
   void visit(UnionDecl *node);

   void visit(FieldDecl *node);
   void visit(PropDecl *node);

   void visit(MethodDecl *node);
   void visit(ConstrDecl *node);
   void visit(DestrDecl *node);

   void visit(RecordTemplateDecl *node);
   void visit(CallableTemplateDecl *node);
   void visit(MethodTemplateDecl *node);

   void visit(IdentifierRefExpr *node);
   void visit(SubscriptExpr *node);
   void visit(CallExpr *node);
   void visit(MemberRefExpr *node);

   void visit(ForStmt *node);
   void visit(ForInStmt *node);
   void visit(WhileStmt *node);
   void visit(IfStmt *node);
   void visit(MatchStmt *node);
   void visit(CaseStmt *node);
   void visit(LabelStmt *node);
   void visit(GotoStmt *node);

   void visit(ReturnStmt *node);
   void visit(BreakStmt *node);
   void visit(ContinueStmt *node);

   void visit(IntegerLiteral *node);
   void visit(FPLiteral *node);
   void visit(BoolLiteral *node);
   void visit(CharLiteral *node);

   void visit(CollectionLiteral *node);

   void visit(NoneLiteral *node);
   void visit(StringLiteral *node);
   void visit(StringInterpolation *node);
   void visit(TupleLiteral *node);

   void visit(BinaryOperator *node);
   void visit(TertiaryOperator *node);
   void visit(UnaryOperator *node);

   void visit(TryStmt *node);
   void visit(ThrowStmt *node);

   void visit(FuncArgDecl *node);
   void visit(Expression *node);
   void visit(LambdaExpr *node);
   void visit(ImplicitCastExpr *node);
   void visit(TypedefDecl *node);
   void visit(TypeRef *node);
   void visit(LvalueToRvalue *node);

   void visit(EndOfFileStmt *node);
   void visit(DebugStmt *node);

   void visit(Statement *node);

protected:
   std::unordered_map<string, DeclStmt*> declarations;
   std::stack<Scope> Scopes;
   std::stack<pair<string, string>> Cleanups;

   llvm::StringMap<GotoStmt*> UnresolvedGotos;
   std::stack<QualType> Results;

   std::function<QualType(Expression*)> resolverFn;
   std::function<Type*
      (TypeRef*, const std::vector<TemplateArg>&,
       const std::vector<TemplateConstraint>&)> TypeResolverFn;

   QualType pop();
   QualType VisitNode(AstNode *node);
   QualType VisitNode(std::shared_ptr<AstNode> node);

   QualType getAmbiguousResult(AstNode *node);
   QualType getAmbiguousResult(std::shared_ptr<AstNode> node);

   void returnResult(QualType t);
   void returnResult(Type *t);

   std::stack<string> classScopeStack;
   string& currentClass() {
      return classScopeStack.top();
   }

   Scope* latestScope = nullptr;

   size_t lastScopeID = 0;

   void pushScope();
   void popScope();

   void pushFunctionScope(QualType returnType,
                          Callable *func);

   void pushMethodScope(cl::Method*);

   void popFunctionScope();

   void pushLoopScope(bool continuable = true, bool breakable = true);
   void popLoopScope(bool continuable = true, bool breakable = true);

   void pushClassScope(cl::Record* cl);
   void popClassScope();

   string declareVariable(
      const string &name,
      const QualType &type,
      bool isGlobal = false,
      AstNode *cause = nullptr
   );

   cl::Record *getRecord(
      string &name,
      TemplateArgList *argList
   );

   void pushTy(const QualType&);
   QualType popTy();

   struct VarResult {
      Variable *V;
      std::string scope;
      bool escapesLambdaScope;
   };

   VarResult getVariable(string &name,
                         AstNode *cause = nullptr);

   CallCompatability getFunction(
      const string& funcName,
      std::vector<Argument>& args,
      std::vector<TemplateArg> const& templateArgs = {}
   );

   CallCompatability getMethod(
      cdot::cl::Record *rec,
      const string& methodName,
      const std::vector<Argument>& args = {},
      std::vector<TemplateArg> const& templateArgs = {}
   );

   CallCompatability getCase(
      cdot::cl::Enum *en,
      const string &caseName,
      std::vector<Argument> const& args = {}
   );

   bool hasVariable(string name);

   MetaType *getMetaType(Type *forType);

   void wrapImplicitCast(
      std::shared_ptr<Expression>& target,
      const QualType &originTy,
      const QualType &destTy
   );

   void lvalueToRvalue(std::shared_ptr<Expression>& target);
   void toRvalueIfNecessary(QualType &ty, std::shared_ptr<Expression> &target,
                            bool preCond = true);

   void CopyScopeProps(
      Scope* src,
      Scope* dst
   );

   bool warnCast(QualType &lhs, QualType &rhs);
   void raiseTypeError(QualType &lhs, QualType &rhs, AstNode* cause);

   std::stack<QualType> typeStack;

   std::set<string> labels = {};

   void ReturnMemberExpr(Expression*, QualType);

   std::vector<Attribute> attributes = {};

   bool hasAttribute(Attr kind) {
      for (const auto& attr : attributes) {
         if (attr.kind == kind) {
            return true;
         }
      }

      return false;
   }

   size_t BreakStack = 0;
   size_t ContinueStack = 0;
   std::stack<QualType> ReturnTypeStack;

   static std::vector<string> currentNamespace;
   static std::vector<string> importedNamespaces;

   static std::vector<std::vector<TemplateConstraint>*> GenericsStack;

   static void pushNamespace(const string &ns);
   static void popNamespace();

   static inline string ns_prefix() {
      return currentNamespace.back().empty() ? ""
                                             : currentNamespace.back() + ".";
   }

   QualType tryFunctionReference(IdentifierRefExpr *node);

   pair<QualType, std::vector<QualType>> unify(
      std::vector<std::shared_ptr<Expression>>&);


   void DefineClass(ClassDecl*, cdot::cl::Class*);
   void DefineField(FieldDecl*, cdot::cl::Class*);
   void DefineMethod(MethodDecl*, cdot::cl::Class*);
   void DefineConstr(ConstrDecl*, cdot::cl::Class*);
   void DefineDestr(DestrDecl*, cdot::cl::Class*);

   Type* HandleBuiltinIdentifier(IdentifierRefExpr *node);

   QualType HandleBinaryOperator(QualType &lhs, QualType &rhs, BinaryOperatorType,
                             BinaryOperator *node);
   QualType HandleCastOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleAssignmentOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleArithmeticOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleBitwiseOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleLogicalOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleEqualityOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleComparisonOp(QualType &lhs, QualType &rhs, BinaryOperator *node);
   QualType HandleOtherOp(QualType &lhs, QualType &rhs, BinaryOperator *node);

   QualType tryBinaryOperatorMethod(
      QualType& fst,
      QualType& snd,
      BinaryOperator *node,
      const string &opName,
      bool isAssignment
   );
   QualType tryFreeStandingBinaryOp(
      QualType& fst,
      QualType& snd,
      BinaryOperator *node,
      const string &opName,
      bool isAssignment
   );
   QualType tryFreeStandingUnaryOp(
      QualType& lhs,
      UnaryOperator *node,
      const string &opName
   );

   void HandleEnumComp(QualType& fst, QualType& snd, BinaryOperator *node);
   void HandleTupleComp(QualType& fst, QualType& snd, BinaryOperator *node);

   void HandleFunctionCall(CallExpr*);
   void HandleBuiltinCall(CallExpr*);

   void HandleMethodCall(CallExpr*);
   void HandleEnumCase(CallExpr *node);

   void throwMethodNotFound(
      CallCompatability& res,
      CallExpr *node,
      cdot::cl::Class *cl
   );

   void HandleConstructorCall(CallExpr*);
   void HandleCallOperator(CallExpr*);
   void HandleAnonCall(CallExpr*);

   void checkClassAccessibility(
      cdot::cl::Class*& cl,
      Expression* cause
   );

   void checkMemberAccessibility(
      cdot::cl::Class*& cl,
      const string& memberName,
      const AccessModifier& access,
      Expression* cause
   );

   void PrepareCallArgs(
      std::vector<pair<string, std::shared_ptr<Expression>>>& args,
      std::vector<Argument>& givenArgs,
      CallCompatability& comp
   );

   void PrepareCallArgs(
      std::vector<pair<string, std::shared_ptr<Expression>>>& args,
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& declaredArgs,
      std::vector<QualType>& resolvedArgs
   );

   void ApplyCasts(
      CallExpr *node,
      std::vector<QualType> &givenArgs,
      std::vector<Argument> &declaredArgs
   );

   void CopyNodeProperties(Expression *src, Expression *dst);
   void CheckUnionAccess(MemberRefExpr *node);

   bool matchableAgainst(
      QualType& matchVal,
      std::shared_ptr<CaseStmt> const& caseVal
   );

   QualType HandleDictionaryLiteral(CollectionLiteral *node);

   cl::Method *setterMethod = nullptr;
   void HandleFieldAccess(MemberRefExpr *node, cl::Class *cl);
   void HandlePropAccess(MemberRefExpr *node, cl::Record *rec);
};

} // namespace ast
} // namespace cdot


#endif //CDOT_TYPECHECKVISITOR_H
