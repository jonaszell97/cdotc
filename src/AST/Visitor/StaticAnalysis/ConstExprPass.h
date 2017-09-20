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

   virtual Variant visit(NamespaceDecl *node);
   virtual Variant visit(CompoundStmt *node);

   virtual Variant visit(DeclStmt *node);
   virtual Variant visit(FunctionDecl *node);
   virtual Variant visit(DeclareStmt *node);

   virtual Variant visit(ClassDecl *node);
   virtual Variant visit(MethodDecl *node);
   virtual Variant visit(FieldDecl *node);
   virtual Variant visit(ConstrDecl *node);
   virtual Variant visit(DestrDecl *node);
   virtual Variant visit(EnumDecl *node);

   virtual Variant visit(IdentifierRefExpr *node);
   virtual Variant visit(SubscriptExpr *node);
   virtual Variant visit(CallExpr *node);
   virtual Variant visit(MemberRefExpr *node);

   virtual Variant visit(ForStmt *node);
   virtual Variant visit(ForInStmt *node);
   virtual Variant visit(WhileStmt *node);
   virtual Variant visit(IfStmt *node);
   virtual Variant visit(MatchStmt *node);
   virtual Variant visit(CaseStmt *node);
   virtual Variant visit(LabelStmt *node);
   virtual Variant visit(GotoStmt *node);

   virtual Variant visit(ReturnStmt *node);
   virtual Variant visit(BreakStmt *node);
   virtual Variant visit(ContinueStmt *node);

   virtual Variant visit(CollectionLiteral *node);
   virtual Variant visit(NumericLiteral *node);
   virtual Variant visit(NoneLiteral *node);
   virtual Variant visit(StringLiteral *node);
   virtual Variant visit(StringInterpolation *node);
   virtual Variant visit(TupleLiteral *node);

   virtual Variant visit(BinaryOperator *node);
   virtual Variant visit(TertiaryOperator *node);
   virtual Variant visit(UnaryOperator *node);


   virtual Variant visit(FuncArgDecl *node);
   virtual Variant visit(Expression *node);
   virtual Variant visit(LambdaExpr *node);
   virtual Variant visit(ImplicitCastExpr *node);
   virtual Variant visit(TypedefDecl *node);
   virtual Variant visit(TypeRef *node);
   virtual Variant visit(LvalueToRvalue *node);

   virtual Variant visit(DebugStmt *node);

   virtual Variant visit(Statement *node);

protected:
   static size_t ConditionStack;

   long hasVariable(string& varName);
   Variant& getVariable(string& varName);
   void setVariable(string& varName, Variant& val);

   unordered_map<string, Variant> Variables;
};


#endif //CDOT_CONSTEXPRVISITOR_H
