//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>
#include "../../Variant/Variant.h"

class AstNode;
class FunctionDecl;
class CompoundStmt;
class IdentifierRefExpr;
class DeclStmt;
class ForStmt;
class WhileStmt;
class ArrayLiteral;
class LiteralExpr;
class ArrayAccessExpr;
class CallExpr;
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
class Expression;
class Statement;
class ClassDecl;
class FieldDecl;
class MethodDecl;
class ConstrDecl;
class SwitchStmt;
class CaseStmt;
class LabelStmt;
class GotoStmt;
class StringLiteral;
class LambdaExpr;
class OperatorDecl;
class NamespaceDecl;
class ImportStmt;
class ExportStmt;
class InterfaceDecl;
class ImplicitCastExpr;
class ExtendStmt;
class TypedefDecl;
class TypeRef;

class Visitor {
public:
    virtual Variant visit(NamespaceDecl*) = 0;

    virtual Variant visit(FunctionDecl*) = 0;
    virtual Variant visit(CompoundStmt*) = 0;
    virtual Variant visit(IdentifierRefExpr*) = 0;
    virtual Variant visit(DeclStmt*) = 0;
    virtual Variant visit(ForStmt*) = 0;
    virtual Variant visit(WhileStmt*) = 0;
    virtual Variant visit(ImportStmt*) = 0;
    virtual Variant visit(ExportStmt*) = 0;

    virtual Variant visit(ArrayLiteral*) = 0;
    virtual Variant visit(LiteralExpr*) = 0;
    virtual Variant visit(StringLiteral*) = 0;
    virtual Variant visit(ArrayAccessExpr*) = 0;
    virtual Variant visit(CallExpr*) = 0;
    virtual Variant visit(MemberRefExpr*) = 0;
    virtual Variant visit(BinaryOperator*) = 0;
    virtual Variant visit(ExplicitCastExpr*) = 0;
    virtual Variant visit(TertiaryOperator*) = 0;
    virtual Variant visit(UnaryOperator*) = 0;
    virtual Variant visit(BreakStmt*) = 0;
    virtual Variant visit(ContinueStmt*) = 0;
    virtual Variant visit(IfStmt*) = 0;
    virtual Variant visit(SwitchStmt*) = 0;
    virtual Variant visit(CaseStmt*) = 0;
    virtual Variant visit(LabelStmt*) = 0;
    virtual Variant visit(GotoStmt*) = 0;
    virtual Variant visit(FuncArgDecl*) = 0;
    virtual Variant visit(ReturnStmt*) = 0;
    virtual Variant visit(Expression*) = 0;
    virtual Variant visit(ClassDecl*) = 0;
    virtual Variant visit(MethodDecl*) = 0;
    virtual Variant visit(FieldDecl*) = 0;
    virtual Variant visit(ConstrDecl*) = 0;
    virtual Variant visit(LambdaExpr*) = 0;
    virtual Variant visit(OperatorDecl*) = 0;
    virtual Variant visit(InterfaceDecl*) = 0;
    virtual Variant visit(ImplicitCastExpr*) = 0;
    virtual Variant visit(ExtendStmt*) = 0;
    virtual Variant visit(TypedefDecl*) = 0;
    virtual Variant visit(TypeRef*) = 0;
};


#endif //CDOT_VISITOR_H
