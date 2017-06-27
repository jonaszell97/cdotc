//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>
#include "../Context.h"

enum class VisitorFlag {
    NONE,
    LINK_TREE
};

class AstNode;
class InputStmt;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class WhileStmt;
class ArrayLiteral;
class LiteralExpr;
class ObjectLiteral;
class ObjectPropExpr;
class ArrayAccessExpr;
class CallExpr;
class MethodCallExpr;
class MemberRefExpr;
class BinaryOperator;
class ExplicitCastExpr;
class TertiaryOperator;
class UnaryOperator;
class BreakStmt;
class ContinueStmt;
class IfStmt;
class FuncArgDecl;
class ReturnStmt;
class InputStmt;
class OutputStmt;
class Expression;
class Statement;
class InstantiationExpr;
class ClassDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;

class Visitor {
public:
    Visitor();
    Visitor(const Visitor&);

    virtual Variant visit(FunctionDecl*);
    virtual Variant visit(CompoundStmt*);
    virtual Variant visit(IdentifierRefExpr*);
    virtual Variant visit(DeclStmt*);
    virtual Variant visit(ForStmt*);
    virtual Variant visit(WhileStmt*);

    virtual Variant visit(ArrayLiteral*);
    virtual Variant visit(LiteralExpr*);
    virtual Variant visit(ObjectLiteral*);
    virtual Variant visit(ObjectPropExpr*);
    virtual Variant visit(ArrayAccessExpr*);
    virtual Variant visit(CallExpr*);
    virtual Variant visit(MemberRefExpr*);
    virtual Variant visit(MethodCallExpr*);
    virtual Variant visit(BinaryOperator*);
    virtual Variant visit(ExplicitCastExpr*);
    virtual Variant visit(TertiaryOperator*);
    virtual Variant visit(UnaryOperator*);
    virtual Variant visit(BreakStmt*);
    virtual Variant visit(ContinueStmt*);
    virtual Variant visit(IfStmt*);
    virtual Variant visit(FuncArgDecl*);
    virtual Variant visit(ReturnStmt*);
    virtual Variant visit(InputStmt*);
    virtual Variant visit(OutputStmt*);
    virtual Variant visit(Expression*);
    virtual Variant visit(InstantiationExpr*);
    virtual Variant visit(ClassDecl*);
    virtual Variant visit(MethodDecl*);
    virtual Variant visit(FieldDecl*);
    virtual Variant visit(ConstrDecl*);
};


#endif //CDOT_VISITOR_H
