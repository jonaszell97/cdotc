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

class SymbolTable;
class Expression;

using std::string;
using std::pair;

namespace cdot {
   class Type;
   class BinaryOperator;
   enum class BinaryOperatorType : unsigned int;

   namespace cl {
      class Class;
      class Method;
      class Enum;
   }

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

      Type* declaredReturnType = nullptr;
      bool returnable = false;
      bool inLambda = false;
      int branches = 1;
      int returned = 0;

      Scope* enclosingScope = nullptr;
      std::vector<string>* uninitializedFields = nullptr;
      std::vector<pair<string, Type*>>* captures = nullptr;
   };
}

using namespace cdot;

class TypeCheckPass {
public:
   TypeCheckPass();

   void doInitialPass(std::vector<std::shared_ptr<Statement>>& statements);

   static void connectTree(AstNode*);

   virtual Type* visit(NamespaceDecl *node);
   virtual Type* visit(UsingStmt *node);
   virtual Type* visit(CompoundStmt *node);

   virtual Type* visit(DeclStmt *node);
   virtual Type* visit(FunctionDecl *node);
   virtual Type* visit(DeclareStmt *node);

   virtual Type* visit(ClassDecl *node);
   virtual Type* visit(MethodDecl *node);
   virtual Type* visit(FieldDecl *node);
   virtual Type* visit(ConstrDecl *node);
   virtual Type* visit(DestrDecl *node);
   virtual Type* visit(EnumDecl *node);

   virtual Type* visit(IdentifierRefExpr *node);
   virtual Type* visit(SubscriptExpr *node);
   virtual Type* visit(CallExpr *node);
   virtual Type* visit(MemberRefExpr *node);

   virtual Type* visit(ForStmt *node);
   virtual Type* visit(ForInStmt *node);
   virtual Type* visit(WhileStmt *node);
   virtual Type* visit(IfStmt *node);
   virtual Type* visit(MatchStmt *node);
   virtual Type* visit(CaseStmt *node);
   virtual Type* visit(LabelStmt *node);
   virtual Type* visit(GotoStmt *node);

   virtual Type* visit(ReturnStmt *node);
   virtual Type* visit(BreakStmt *node);
   virtual Type* visit(ContinueStmt *node);

   virtual Type* visit(CollectionLiteral *node);
   virtual Type* visit(NumericLiteral *node);
   virtual Type* visit(NoneLiteral *node);
   virtual Type* visit(StringLiteral *node);
   virtual Type* visit(StringInterpolation *node);
   virtual Type* visit(TupleLiteral *node);

   virtual Type* visit(BinaryOperator *node);
   virtual Type* visit(TertiaryOperator *node);
   virtual Type* visit(UnaryOperator *node);


   virtual Type* visit(FuncArgDecl *node);
   virtual Type* visit(Expression *node);
   virtual Type* visit(LambdaExpr *node);
   virtual Type* visit(ImplicitCastExpr *node);
   virtual Type* visit(TypedefDecl *node);
   virtual Type* visit(TypeRef *node);
   virtual Type* visit(LvalueToRvalue *node);

   virtual Type* visit(EndOfFileStmt *node);
   virtual Type* visit(DebugStmt *node);
   
   virtual Type* visit(Statement *node);

protected:
   std::unordered_map<string, DeclStmt*> declarations = {};
   std::stack<Scope> Scopes;
   std::stack<pair<string, string>> Cleanups;

   std::stack<string> classScopeStack;
   string& currentClass() {
      return classScopeStack.top();
   }

   Scope* latestScope = nullptr;

   size_t lastScopeID = 0;
   void pushScope();
   void pushFunctionScope(
      Type* returnType,
      string methodName,
      string mangledName,
      bool isLambda = false
   );

   void pushMethodScope(
      cl::Method*
   );

   void pushLoopScope(bool continuable = true, bool breakable = true);
   void popScope();

   void pushClassScope(cl::Class* cl, string& className);
   void popClassScope();

   string declareVariable(string &name, Type *type, bool isGlobal = false, AstNode *cause = nullptr);

   void pushTy(Type *);
   Type* popTy();

   inline void resolve(Type**, AstNode *node = nullptr);

   pair<pair<Type*, string>, bool> getVariable(string &name, AstNode *cause = nullptr);

   CallCompatability getFunction(
      string& funcName,
      std::vector<Argument>& args,
      std::vector<Type*> generics = {}
   );

   bool hasVariable(string name);

   void wrapImplicitCast(std::shared_ptr<Expression>& target, Type*& originTy, Type* destTy);
   void lvalueToRvalue(std::shared_ptr<Expression>& target);
   void toRvalueIfNecessary(Type *&ty, std::shared_ptr<Expression> &target, bool preCond = true);

   bool castGenericIfNecessary(
      Expression *node,
      unordered_map<string, Type *> &concreteGenerics,
      Type *&ty,
      bool preCond = true
   );

   std::stack<Type*> typeStack;

   std::vector<string> labels = {};

   inline bool has_label(string label) {
      if (std::find(labels.begin(), labels.end(), label) != labels.end()) {
         return true;
      }

      return false;
   }

   inline Type* ReturnMemberExpr(Expression*, Type*);

   void return_(Type* ret_type, AstNode *cause = nullptr);

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

   static std::vector<string> currentNamespace;
   static std::vector<string> importedNamespaces;

   static std::stack<std::vector<ObjectType*>*> GenericsStack;

   static inline void pushNamespace(string &ns);
   static inline void popNamespace();

   bool checkLambdaCompatibility(LambdaExpr*, Type*);

   static inline string ns_prefix() {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   pair<Type*, std::vector<Type*>> unify(std::vector<std::shared_ptr<Expression>>&);


   void DefineClass(ClassDecl*, cdot::cl::Class*);
   void DefineField(FieldDecl*, cdot::cl::Class*);
   void DefineMethod(MethodDecl*, cdot::cl::Class*);
   void DefineConstr(ConstrDecl*, cdot::cl::Class*);
   void DefineDestr(DestrDecl*, cdot::cl::Class*);

   Type* HandleBuiltinIdentifier(IdentifierRefExpr *node);

   Type* HandleBinaryOperator(Type*, Type*, BinaryOperatorType, BinaryOperator *node);
   Type* HandleCastOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleAssignmentOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleArithmeticOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleBitwiseOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleLogicalOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleEqualityOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleComparisonOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleOtherOp(Type *fst, Type *snd, BinaryOperator *node);

   Type* tryBinaryOperatorMethod(
      Type *fst,
      Type *snd,
      BinaryOperator *node,
      string &opName,
      bool isAssignment
   );
   Type* tryFreeStandingBinaryOp(
      Type *fst,
      Type *snd,
      BinaryOperator *node,
      string &opName,
      bool isAssignment
   );
   Type* tryFreeStandingUnaryOp(
      Type *lhs,
      UnaryOperator *node,
      string &opName
   );

   void HandleEnumComp(Type *fst, Type *snd, BinaryOperator *node);
   void HandleTupleComp(Type *fst, Type *snd, BinaryOperator *node);

   std::vector<std::vector<Argument>*> GetMethodOverloadArgs(
      cl::Class* cl,
      string& methodName
   );

   std::vector<std::vector<Argument>*> GetFunctionOverloadArgs(
      string& functionName
   );

   void GetCallArguments(
      std::vector<std::vector<Argument>*> overloads,
      CallExpr *&node
   );

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
      string& memberName,
      AccessModifier& access,
      Expression* cause
   );

   void PrepareCallArgs(
      std::vector<pair<string, std::shared_ptr<Expression>>>& args,
      std::vector<Argument>& givenArgs,
      CallCompatability& comp
   );

   void ApplyCasts(
      std::vector<pair<string, std::shared_ptr<Expression>>> &args,
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &declaredArgs,
      std::vector<size_t> &casts
   );

   void CopyNodeProperties(Expression *src, Expression *dst);

   bool matchableAgainst(
      Type*& matchVal,
      std::shared_ptr<CaseStmt> const& caseVal
   );

   Type* HandleDictionaryLiteral(CollectionLiteral *node);
};


#endif //CDOT_TYPECHECKVISITOR_H
