//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#define ADD_FRIEND_PASSES \
   friend class ::DeclPass; \
   friend class ::TypeCheckPass; \
   friend class ::CodeGen; \
   friend class ::ConstExprPass; \
   friend class ::HeaderGen;

class AstNode;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class ForInStmt;
class WhileStmt;
class CollectionLiteral;
class NumericLiteral;
class SubscriptExpr;
class CallExpr;
class MemberRefExpr;
class TertiaryOperator;
class UnaryOperator;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class FuncArgDecl;
class ReturnStmt;
class Expression;
class Statement;
class ClassDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;
class DestrDecl;
class MatchStmt;
class CaseStmt;
class LabelStmt;
class GotoStmt;
class StringLiteral;
class LambdaExpr;
class NamespaceDecl;
class UsingStmt;
class EndOfFileStmt;
class ImplicitCastExpr;
class TypedefDecl;
class TypeRef;
class DeclareStmt;
class LvalueToRvalue;
class DebugStmt;
class TupleLiteral;
class EnumDecl;
class EnumCaseDecl;
class Statement;
class NoneLiteral;
class StringInterpolation;

namespace cdot {
   class BinaryOperator;
}

using cdot::BinaryOperator;

class AbstractPass {
public:
   virtual ~AbstractPass() = default;
   virtual void finalize() {}

   virtual void visit(NamespaceDecl *node);
   virtual void visit(UsingStmt *node);
   virtual void visit(CompoundStmt *node);

   virtual void visit(DeclStmt *node);
   virtual void visit(FunctionDecl *node);
   virtual void visit(DeclareStmt *node);

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
   virtual void visit(Expression *node);
   virtual void visit(LambdaExpr *node);
   virtual void visit(ImplicitCastExpr *node);
   virtual void visit(TypedefDecl *node);
   virtual void visit(TypeRef *node);
   virtual void visit(LvalueToRvalue *node);

   virtual void visit(EndOfFileStmt *node);
   virtual void visit(DebugStmt *node);

   virtual void visit(Statement *node);

protected:
   AbstractPass() = default;
};

#endif //CDOT_VISITOR_H
