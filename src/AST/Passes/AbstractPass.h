//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#define ADD_FRIEND_PASSES \
   friend class ::DeclPass; \
   friend class ::SemaPass; \
   friend class ::CodeGen; \
   friend class ::ConstExprPass; \
   friend class ::HeaderGen;

#include "../AstDeclarations.h"

namespace cdot {
   class BinaryOperator;
}

using cdot::BinaryOperator;

class AbstractPass {
public:
   virtual ~AbstractPass() = default;
   virtual void finalize() {}

   virtual void visit(AstNode *node);
   virtual void visit(Expression *node);
   virtual void visit(Statement *node);

   virtual void visit(NamespaceDecl *node);
   virtual void visit(UsingStmt *node);
   virtual void visit(CompoundStmt *node);

   virtual void visit(DeclStmt *node);
   virtual void visit(FunctionDecl *node);
   virtual void visit(DeclareStmt *node);

   virtual void visit(UnionDecl *node);
   virtual void visit(ClassDecl *node);
   virtual void visit(MethodDecl *node);
   virtual void visit(FieldDecl *node);
   virtual void visit(ConstrDecl *node);
   virtual void visit(DestrDecl *node);

   virtual void visit(EnumDecl *node);
   virtual void visit(EnumCaseDecl *node);

   virtual void visit(IdentifierRefExpr *node);
   virtual void visit(SubscriptExpr *node);
   virtual void visit(CallExpr *node);
   virtual void visit(MemberRefExpr *node);

   virtual void visit(ForStmt *node);
   virtual void visit(ForInStmt *node);
   virtual void visit(WhileStmt *node);
   virtual void visit(IfStmt *node);
   virtual void visit(MatchStmt *node);
   virtual void visit(CaseStmt *node);
   virtual void visit(LabelStmt *node);
   virtual void visit(GotoStmt *node);

   virtual void visit(ReturnStmt *node);
   virtual void visit(BreakStmt *node);
   virtual void visit(ContinueStmt *node);

   virtual void visit(CollectionLiteral *node);
   virtual void visit(NumericLiteral *node);
   virtual void visit(NoneLiteral *node);
   virtual void visit(StringLiteral *node);
   virtual void visit(StringInterpolation *node);
   virtual void visit(TupleLiteral *node);

   virtual void visit(UnaryOperator *node);
   virtual void visit(BinaryOperator *node);
   virtual void visit(TertiaryOperator *node);
   
   virtual void visit(FuncArgDecl *node);
   virtual void visit(LambdaExpr *node);
   virtual void visit(ImplicitCastExpr *node);
   virtual void visit(TypedefDecl *node);
   virtual void visit(TypeRef *node);
   virtual void visit(LvalueToRvalue *node);

   virtual void visit(EndOfFileStmt *node);
   virtual void visit(DebugStmt *node);

   virtual void visit(TryStmt *node);
   virtual void visit(ThrowStmt *node);

protected:
   AbstractPass() = default;
};

#endif //CDOT_VISITOR_H
