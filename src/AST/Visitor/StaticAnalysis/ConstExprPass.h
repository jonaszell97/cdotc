//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_CONSTEXPRVISITOR_H
#define CDOT_CONSTEXPRVISITOR_H

#include <stack>
#include <vector>
#include <string>
#include <unordered_map>
#include "../Visitor.h"

namespace cdot {
   struct Variant;
}

using namespace cdot;
using std::stack;
using std::unordered_map;
using std::string;

class ConstExprPass {
public:
   ConstExprPass();

   virtual Variant visit(NamespaceDecl*);

   virtual Variant visit(FunctionDecl*);
   virtual Variant visit(CompoundStmt*);
   virtual Variant visit(IdentifierRefExpr*);
   virtual Variant visit(DeclStmt*);
   virtual Variant visit(ForStmt*);
   virtual Variant visit(ForInStmt*);
   virtual Variant visit(WhileStmt*);

   virtual Variant visit(CollectionLiteral*);
   virtual Variant visit(NumericLiteral*);
   virtual Variant visit(NoneLiteral*);
   virtual Variant visit(StringLiteral*);
   virtual Variant visit(SubscriptExpr*);
   virtual Variant visit(CallExpr*);
   virtual Variant visit(MemberRefExpr*);
   virtual Variant visit(BinaryOperator*);
   virtual Variant visit(TertiaryOperator*);
   virtual Variant visit(UnaryOperator*);
   virtual Variant visit(BreakStmt*);
   virtual Variant visit(ContinueStmt*);
   virtual Variant visit(IfStmt*);
   virtual Variant visit(MatchStmt*);
   virtual Variant visit(CaseStmt*);
   virtual Variant visit(LabelStmt*);
   virtual Variant visit(GotoStmt*);
   virtual Variant visit(FuncArgDecl*);
   virtual Variant visit(ReturnStmt*);
   virtual Variant visit(Expression*);
   virtual Variant visit(ClassDecl*);
   virtual Variant visit(MethodDecl*);
   virtual Variant visit(FieldDecl*);
   virtual Variant visit(ConstrDecl*);
   virtual Variant visit(DestrDecl*);
   virtual Variant visit(LambdaExpr*);
   virtual Variant visit(ImplicitCastExpr*);
   virtual Variant visit(ExtendStmt*);
   virtual Variant visit(TypedefDecl*);
   virtual Variant visit(TypeRef*);
   virtual Variant visit(DeclareStmt*);
   virtual Variant visit(LvalueToRvalue*);
   virtual Variant visit(DebugStmt*);
   virtual Variant visit(TupleLiteral*);
   virtual Variant visit(EnumDecl*);

   virtual Variant visit(Statement*);

protected:
   static size_t latestScope;

   void pushScope();
   void popScope();

   long hasVariable(string& varName);
   Variant& getVariable(string& varName);
   void setVariable(string& varName, Variant& val);

   std::vector<string> Scopes;
   unordered_map<string, Variant> Variables;
};


#endif //CDOT_CONSTEXPRVISITOR_H
