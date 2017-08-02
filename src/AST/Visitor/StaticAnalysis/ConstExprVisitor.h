//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_CONSTEXPRVISITOR_H
#define CDOT_CONSTEXPRVISITOR_H

#include "../Visitor.h"

class ConstExprVisitor : public Visitor {
    virtual Variant visit(ModuleDecl*);
    virtual Variant visit(ImportStmt*);
    virtual Variant visit(ExportStmt*);

    virtual Variant visit(FunctionDecl*);
    virtual Variant visit(CompoundStmt*);
    virtual Variant visit(IdentifierRefExpr*);
    virtual Variant visit(DeclStmt*);
    virtual Variant visit(ForStmt*);
    virtual Variant visit(WhileStmt*);

    virtual Variant visit(ArrayLiteral*);
    virtual Variant visit(LiteralExpr*);
    virtual Variant visit(StringLiteral*);
    virtual Variant visit(ArrayAccessExpr*);
    virtual Variant visit(CallExpr*);
    virtual Variant visit(MemberRefExpr*);
    virtual Variant visit(MethodCallExpr*);
    virtual Variant visit(FunctionCallExpr*);
    virtual Variant visit(BinaryOperator*);
    virtual Variant visit(ExplicitCastExpr*);
    virtual Variant visit(TertiaryOperator*);
    virtual Variant visit(UnaryOperator*);
    virtual Variant visit(BreakStmt*);
    virtual Variant visit(ContinueStmt*);
    virtual Variant visit(IfStmt*);
    virtual Variant visit(SwitchStmt*);
    virtual Variant visit(CaseStmt*);
    virtual Variant visit(LabelStmt*);
    virtual Variant visit(GotoStmt*);
    virtual Variant visit(FuncArgDecl*);
    virtual Variant visit(ReturnStmt*);
    virtual Variant visit(InputStmt*);
    virtual Variant visit(OutputStmt*);
    virtual Variant visit(Expression*);
    virtual Variant visit(ClassDecl*);
    virtual Variant visit(MethodDecl*);
    virtual Variant visit(FieldDecl*);
    virtual Variant visit(ConstrDecl*);
    virtual Variant visit(LambdaExpr*);
    virtual Variant visit(OperatorDecl*);
    virtual Variant visit(StructDecl*);
};


#endif //CDOT_CONSTEXPRVISITOR_H
