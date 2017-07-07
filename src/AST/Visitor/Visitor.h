//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_VISITOR_H
#define CDOT_VISITOR_H

#include <vector>
#include "../Context.h"


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
class FunctionCallExpr;
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
class ModuleDecl;
class ImportStmt;
class ExportStmt;
class StructDecl;

class Visitor {
public:
    virtual Variant visit(ModuleDecl*) = 0;

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
    virtual Variant visit(ObjectLiteral*) = 0;
    virtual Variant visit(ObjectPropExpr*) = 0;
    virtual Variant visit(ArrayAccessExpr*) = 0;
    virtual Variant visit(CallExpr*) = 0;
    virtual Variant visit(MemberRefExpr*) = 0;
    virtual Variant visit(MethodCallExpr*) = 0;
    virtual Variant visit(FunctionCallExpr*) = 0;
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
    virtual Variant visit(InputStmt*) = 0;
    virtual Variant visit(OutputStmt*) = 0;
    virtual Variant visit(Expression*) = 0;
    virtual Variant visit(ClassDecl*) = 0;
    virtual Variant visit(MethodDecl*) = 0;
    virtual Variant visit(FieldDecl*) = 0;
    virtual Variant visit(ConstrDecl*) = 0;
    virtual Variant visit(LambdaExpr*) = 0;
    virtual Variant visit(OperatorDecl*) = 0;
    virtual Variant visit(StructDecl*) = 0;
};


#endif //CDOT_VISITOR_H
