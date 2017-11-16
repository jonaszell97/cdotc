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

class SymbolTable;
class Expression;

using std::string;
using std::pair;

namespace cdot {

class BuiltinType;
class BinaryOperator;
class MetaType;
struct Typedef;
enum class BinaryOperatorType : unsigned int;

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

   string currentSelf;
   pair<string, string> currentFunction;

   bool inProtocol = false;
   bool isFunctionRoot = false;
   bool isLambdaRoot = false;

   bool continuable = false;
   bool breakable = false;
   bool continued = false;
   bool broken = false;

   bool unsafe = false;
   bool mutableSelf = false;

   Type declaredReturnType;
   bool returnable = false;
   bool inLambda = false;
   int branches = 1;
   int returned = 0;

   Scope* enclosingScope = nullptr;
   std::vector<string>* uninitializedFields = nullptr;
   std::vector<pair<string, Type>>* captures = nullptr;
};

} // namespace cdot

using namespace cdot;

class SemaPass: public AbstractPass {
public:
   explicit SemaPass();
   void run(std::vector<std::shared_ptr<CompoundStmt>> &roots) override;

   void doInitialPass(const std::shared_ptr<Statement>& statement);
   void doInitialPass(
      const std::vector<std::shared_ptr<Statement>>& statements);

   static void connectTree(AstNode*);

   void visit(NamespaceDecl *node) override;
   void visit(UsingStmt *node) override;
   void visit(CompoundStmt *node) override;

   void visit(DeclStmt *node) override;
   void visit(FunctionDecl *node) override;
   void visit(CallableDecl *node) override;
   void visit(DeclareStmt *node) override;

   void visit(ClassDecl *node) override;
   void visit(ExtensionDecl *node) override;
   void visit(EnumDecl *node) override;
   void visit(UnionDecl *node) override;

   void visit(FieldDecl *node) override;
   void visit(PropDecl *node) override;

   void visit(MethodDecl *node) override;
   void visit(ConstrDecl *node) override;
   void visit(DestrDecl *node) override;

   void visit(RecordTemplateDecl *node) override;
   void visit(CallableTemplateDecl *node) override;
   void visit(MethodTemplateDecl *node) override;

   void visit(IdentifierRefExpr *node) override;
   void visit(SubscriptExpr *node) override;
   void visit(CallExpr *node) override;
   void visit(MemberRefExpr *node) override;

   void visit(ForStmt *node) override;
   void visit(ForInStmt *node) override;
   void visit(WhileStmt *node) override;
   void visit(IfStmt *node) override;
   void visit(MatchStmt *node) override;
   void visit(CaseStmt *node) override;
   void visit(LabelStmt *node) override;
   void visit(GotoStmt *node) override;

   void visit(ReturnStmt *node) override;
   void visit(BreakStmt *node) override;
   void visit(ContinueStmt *node) override;

   void visit(IntegerLiteral *node) override;
   void visit(FPLiteral *node) override;
   void visit(BoolLiteral *node) override;
   void visit(CharLiteral *node) override;

   void visit(CollectionLiteral *node) override;

   void visit(NoneLiteral *node) override;
   void visit(StringLiteral *node) override;
   void visit(StringInterpolation *node) override;
   void visit(TupleLiteral *node) override;

   void visit(BinaryOperator *node) override;
   void visit(TertiaryOperator *node) override;
   void visit(UnaryOperator *node) override;

   void visit(TryStmt *node) override;
   void visit(ThrowStmt *node) override;

   void visit(FuncArgDecl *node) override;
   void visit(Expression *node) override;
   void visit(LambdaExpr *node) override;
   void visit(ImplicitCastExpr *node) override;
   void visit(TypedefDecl *node) override;
   void visit(TypeRef *node) override;
   void visit(LvalueToRvalue *node) override;

   void visit(EndOfFileStmt *node) override;
   void visit(DebugStmt *node) override;

   void visit(Statement *node) override;

protected:
   std::unordered_map<string, DeclStmt*> declarations;
   std::stack<Scope> Scopes;
   std::stack<pair<string, string>> Cleanups;

   std::stack<Type> Results;

   std::function<Type(Expression*)> resolverFn;
   std::function<BuiltinType*
      (TypeRef*, const std::vector<TemplateArg>&,
       const std::vector<TemplateConstraint>&)> TypeResolverFn;

   Type getResult();
   Type getResult(AstNode *node);
   Type getResult(std::shared_ptr<AstNode> node);

   Type getAmbiguousResult(AstNode *node);
   Type getAmbiguousResult(std::shared_ptr<AstNode> node);

   void returnResult(Type t);
   void returnResult(BuiltinType *t);

   std::stack<string> classScopeStack;
   string& currentClass() {
      return classScopeStack.top();
   }

   Scope* latestScope = nullptr;

   size_t lastScopeID = 0;
   void pushScope();
   void pushFunctionScope(
      Type returnType,
      string methodName,
      string mangledName,
      bool isLambda = false
   );

   void pushMethodScope(
      cl::Method*
   );

   void pushLoopScope(bool continuable = true, bool breakable = true);
   void popScope();

   void pushClassScope(cl::Record* cl);
   void popClassScope();

   string declareVariable(
      const string &name,
      const Type &type,
      bool isGlobal = false,
      AstNode *cause = nullptr
   );

   cl::Record *getRecord(
      string &name,
      TemplateArgList *argList
   );

   void pushTy(const Type&);
   Type popTy();

   inline void resolve(Type*, AstNode *node = nullptr);

   pair<pair<Variable, string>, bool> getVariable(string &name,
                                                  AstNode *cause = nullptr);

   CallCompatability getFunction(
      const string& funcName,
      std::vector<Argument>& args,
      std::vector<TemplateArg> const& templateArgs = {}
   );

   CallCompatability getMethod(
      cl::Record *rec,
      const string& methodName,
      const std::vector<Argument>& args = {},
      std::vector<TemplateArg> const& templateArgs = {}
   );

   CallCompatability getCase(
      cl::Enum *en,
      const string &caseName,
      std::vector<Argument> const& args = {}
   );

   bool hasVariable(string name);

   MetaType *getMetaType(BuiltinType *forType);

   void wrapImplicitCast(
      std::shared_ptr<Expression>& target,
      const Type &originTy,
      const Type &destTy
   );

   void lvalueToRvalue(std::shared_ptr<Expression>& target);
   void toRvalueIfNecessary(Type &ty, std::shared_ptr<Expression> &target,
                            bool preCond = true);

   void CopyScopeProps(
      Scope* src,
      Scope* dst
   );

   bool warnCast(Type &lhs, Type &rhs);
   void raiseTypeError(Type &lhs, Type &rhs, AstNode* cause);

   std::stack<Type> typeStack;

   std::vector<string> labels = {};

   inline bool has_label(string label) {
      if (std::find(labels.begin(), labels.end(), label) != labels.end()) {
         return true;
      }

      return false;
   }

   void ReturnMemberExpr(Expression*, Type);

   void return_(Type& ret_type, AstNode *cause = nullptr);

   void continue_(ContinueStmt* continueStmt);
   void break_(BreakStmt* breakStmt);

   std::vector<Attribute> attributes = {};

   bool hasAttribute(Attr kind) {
      for (const auto& attr : attributes) {
         if (attr.kind == kind) {
            return true;
         }
      }

      return false;
   }

   Callable *currentCallable = nullptr;

   static std::vector<string> currentNamespace;
   static std::vector<string> importedNamespaces;

   static std::vector<std::vector<TemplateConstraint>*> GenericsStack;

   static void pushNamespace(const string &ns);
   static void popNamespace();

   static inline string ns_prefix() {
      return currentNamespace.back().empty() ? ""
                                             : currentNamespace.back() + ".";
   }

   Type tryFunctionReference(IdentifierRefExpr *node);

   pair<Type, std::vector<Type>> unify(
      std::vector<std::shared_ptr<Expression>>&);


   void DefineClass(ClassDecl*, cdot::cl::Class*);
   void DefineField(FieldDecl*, cdot::cl::Class*);
   void DefineMethod(MethodDecl*, cdot::cl::Class*);
   void DefineConstr(ConstrDecl*, cdot::cl::Class*);
   void DefineDestr(DestrDecl*, cdot::cl::Class*);

   BuiltinType* HandleBuiltinIdentifier(IdentifierRefExpr *node);

   Type HandleBinaryOperator(Type &lhs, Type &rhs, BinaryOperatorType,
                             BinaryOperator *node);
   Type HandleCastOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleAssignmentOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleArithmeticOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleBitwiseOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleLogicalOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleEqualityOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleComparisonOp(Type &lhs, Type &rhs, BinaryOperator *node);
   Type HandleOtherOp(Type &lhs, Type &rhs, BinaryOperator *node);

   Type tryBinaryOperatorMethod(
      Type& fst,
      Type& snd,
      BinaryOperator *node,
      string &opName,
      bool isAssignment
   );
   Type tryFreeStandingBinaryOp(
      Type& fst,
      Type& snd,
      BinaryOperator *node,
      string &opName,
      bool isAssignment
   );
   Type tryFreeStandingUnaryOp(
      Type& lhs,
      UnaryOperator *node,
      string &opName
   );

   void HandleEnumComp(Type& fst, Type& snd, BinaryOperator *node);
   void HandleTupleComp(Type& fst, Type& snd, BinaryOperator *node);

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
      std::vector<Type>& resolvedArgs
   );

   void ApplyCasts(
      CallExpr *node,
      std::vector<Type> &givenArgs,
      std::vector<Argument> &declaredArgs
   );

   void CopyNodeProperties(Expression *src, Expression *dst);
   void CheckUnionAccess(MemberRefExpr *node);

   bool matchableAgainst(
      Type& matchVal,
      std::shared_ptr<CaseStmt> const& caseVal
   );

   Type HandleDictionaryLiteral(CollectionLiteral *node);

   cl::Method *setterMethod = nullptr;
   void HandleFieldAccess(MemberRefExpr *node, cl::Class *cl);
   void HandlePropAccess(MemberRefExpr *node, cl::Record *rec);
};


#endif //CDOT_TYPECHECKVISITOR_H
