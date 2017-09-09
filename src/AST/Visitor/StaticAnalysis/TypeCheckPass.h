//
// Created by Jonas Zell on 04.07.17.
//

#ifndef CDOT_TYPECHECKVISITOR_H
#define CDOT_TYPECHECKVISITOR_H

#include <unordered_map>
#include <stack>
#include "../Visitor.h"
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

   struct FunctionResult {
      CompatibilityType compatibility;
      Function* func = nullptr;

      std::unordered_map<size_t, pair<Type*, Type*>> neededCasts;
      std::vector<pair<string, std::shared_ptr<Expression>>> orderedArgs;
      string expectedType;
      string foundType;
      size_t incompArg = 0;
   };

   struct Scope {
      size_t id;

      string currentClass;
      string currentSelf;
      string currentFunction;

      bool inProtocol = false;
      bool isFunctionRoot = false;
      bool isLambdaRoot = false;

      bool continuable = false;
      bool breakable = false;
      bool continued = false;
      bool broken = false;

      bool unsafe = false;

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

   void dump();

   static void connectTree(AstNode*);

   virtual Type* visit(NamespaceDecl*);
   virtual Type* visit(UsingStmt*);
   virtual Type* visit(EndOfFileStmt*);

   virtual Type* visit(FunctionDecl*);
   virtual Type* visit(CompoundStmt*);
   virtual Type* visit(IdentifierRefExpr*);
   virtual Type* visit(DeclStmt*);
   virtual Type* visit(ForStmt*);
   virtual Type* visit(ForInStmt*);
   virtual Type* visit(WhileStmt*);

   virtual Type* visit(CollectionLiteral*);
   virtual Type* visit(NumericLiteral*);
   virtual Type* visit(NoneLiteral*);
   virtual Type* visit(StringLiteral*);
   virtual Type* visit(SubscriptExpr*);
   virtual Type* visit(CallExpr*);
   virtual Type* visit(MemberRefExpr*);
   virtual Type* visit(BinaryOperator*);
   virtual Type* visit(TertiaryOperator*);
   virtual Type* visit(UnaryOperator*);
   virtual Type* visit(BreakStmt*);
   virtual Type* visit(ContinueStmt*);
   virtual Type* visit(IfStmt*);
   virtual Type* visit(MatchStmt*);
   virtual Type* visit(CaseStmt*);
   virtual Type* visit(LabelStmt*);
   virtual Type* visit(GotoStmt*);
   virtual Type* visit(FuncArgDecl*);
   virtual Type* visit(ReturnStmt*);
   virtual Type* visit(Expression*);
   virtual Type* visit(ClassDecl*);
   virtual Type* visit(MethodDecl*);
   virtual Type* visit(FieldDecl*);
   virtual Type* visit(ConstrDecl*);
   virtual Type* visit(DestrDecl*);
   virtual Type* visit(LambdaExpr*);
   virtual Type* visit(ImplicitCastExpr*);
   virtual Type* visit(ExtendStmt*);
   virtual Type* visit(TypedefDecl*);
   virtual Type* visit(TypeRef*);
   virtual Type* visit(DeclareStmt*);
   virtual Type* visit(LvalueToRvalue*);
   virtual Type* visit(DebugStmt*);
   virtual Type* visit(TupleLiteral*);
   virtual Type* visit(EnumDecl*);

   virtual Type* visit(Statement*);

   void DeclareClasses(std::shared_ptr<CompoundStmt>);
   cdot::cl::Class* DeclareClass(ClassDecl*);
   cdot::cl::Class* DeclareClassMethods(ClassDecl*);

   cdot::cl::Enum* DeclareEnum(EnumDecl*);
   cdot::cl::Enum* DeclareEnumMethods(EnumDecl*);

protected:
   std::unordered_map<string, DeclStmt*> declarations = {};
   std::stack<Scope> Scopes;
   std::stack<pair<string, string>> Cleanups;

   Scope* latestScope = nullptr;

   size_t lastScopeID = 0;
   void pushScope();
   void pushFunctionScope(Type* returnType, bool isLambda = false);
   void pushMethodScope(Type* returnType, string& className);
   void pushLoopScope(bool continuable = true, bool breakable = true);
   void popScope();

   string declareVariable(string &name, Type *type, bool isGlobal = false, AstNode *cause = nullptr);
   Type*& declareFunction(Function::UniquePtr &&func, std::vector<ObjectType *> &generics, AstNode *decl = nullptr);

   void pushTy(Type *);
   Type* popTy();

   inline void resolve(Type**);
   inline void checkExistance(ObjectType*, AstNode*);

   pair<pair<Type*, string>, bool> getVariable(string &name, AstNode *cause = nullptr);
   FunctionResult getFunction(string &, std::vector<Type *> &, std::vector<Type *> &, std::vector<string> &,
      std::vector<pair<string, std::shared_ptr<Expression>>> &);

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

   static std::vector<ObjectType*>* currentClassGenerics;

   static inline void pushNamespace(string &ns);
   static inline void popNamespace();

   bool checkLambdaCompatibility(LambdaExpr*, Type*);

   static inline string ns_prefix() {
      return currentNamespace.back().empty() ? "" : currentNamespace.back() + ".";
   }

   pair<Type*, std::vector<Type*>> unify(std::vector<std::shared_ptr<Expression>>&);

   void DeclareFunction(FunctionDecl*);

   void DefineClass(ClassDecl*, cdot::cl::Class*);

   void DeclareField(FieldDecl*, cdot::cl::Class*);
   void DefineField(FieldDecl*, cdot::cl::Class*);

   void DeclareMethod(MethodDecl*, cdot::cl::Class*);
   void DefineMethod(MethodDecl*, cdot::cl::Class*);

   void DeclareConstr(ConstrDecl*, cdot::cl::Class*);
   void DefineConstr(ConstrDecl*, cdot::cl::Class*);
   void DefineDestr(DestrDecl*, cdot::cl::Class*);

   void PrepareCallArgs(std::vector<pair<string, std::shared_ptr<Expression>>>&, std::vector<Type*>&,
      std::vector<Type*>&, std::vector<std::shared_ptr<Expression>>&);
   void PrepareCallArgs(std::vector<pair<string, std::shared_ptr<Expression>>>&, std::vector<Type*>&, std::vector<Type*>&);

   Type* HandleBinaryOperator(Type*, Type*, BinaryOperatorType, BinaryOperator *node);
   Type* HandleCastOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleAssignmentOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleArithmeticOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleBitwiseOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleLogicalOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleEqualityOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleComparisonOp(Type *fst, Type *snd, BinaryOperator *node);
   Type* HandleOtherOp(Type *fst, Type *snd, BinaryOperator *node);

   Type* tryOperatorMethod(Type *fst, Type *snd, BinaryOperator *node, string& opName, bool isAssignment);
   Type* tryFreeStandingOp(Type *fst, Type *snd, BinaryOperator *node, string& opName, bool isAssignment);

   void HandleEnumComp(Type *fst, Type *snd, BinaryOperator *node);
   void HandleTupleComp(Type *fst, Type *snd, BinaryOperator *node);

   void HandleFunctionCall(CallExpr*);
   void HandleMethodCall(CallExpr*);
   void HandleConstructorCall(CallExpr*);
   void HandleCallOperator(CallExpr*);
   void HandleAnonCall(CallExpr*);

   void ApplyCasts(std::vector<pair<string, std::shared_ptr<Expression>>>&, std::vector<Type*>&,
      unordered_map<size_t, pair<Type*, Type*>>&);

   void CopyNodeProperties(Expression *src, Expression *dst);

   bool matchableAgainst(Type*& matchVal, std::shared_ptr<CaseStmt> const& caseVal);
};


#endif //CDOT_TYPECHECKVISITOR_H
