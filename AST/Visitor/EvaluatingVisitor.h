//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_LITERALEXPRVISITOR_H
#define CDOT_LITERALEXPRVISITOR_H


#include "Visitor.h"
#include "../Expression/Literal/ObjectPropExpr.h"
#include "../Statement/Function/FuncArgDecl.h"

enum class EvaluationFlag {
    NONE,
    RETURN_NAME,
    RETURN_PROP
};

class EvaluatingVisitor : public Visitor {
public:
    EvaluatingVisitor();
    EvaluatingVisitor(const EvaluatingVisitor&);

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
    virtual Variant visit(MethodCallExpr*);
    virtual Variant visit(CallExpr*);
    virtual Variant visit(MemberRefExpr*);
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

protected:
    /** Control flow */
    bool return_reached = false;
    bool loop_broken = false;
    bool loop_continued = false;
    bool current_context_returnable = false;
    bool current_context_continuable = false;
    bool current_context_breakable = false;

    /** Function return value */
    Variant return_val = {};

    /** Identifier evaluation */
    Variant current_obj_ref = {};

    /** Flag visits */
    EvaluationFlag flag = EvaluationFlag::NONE;

    /** Object and function declarations */
    ObjectProp current_prop;
    FuncArg current_arg;
};


#endif //CDOT_LITERALEXPRVISITOR_H
