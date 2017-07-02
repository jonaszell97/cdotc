//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_LITERALEXPRVISITOR_H
#define CDOT_LITERALEXPRVISITOR_H


#include "Visitor.h"
#include "../Expression/Literal/ObjectPropExpr.h"
#include "../Statement/Declaration/FuncArgDecl.h"
#include "../Statement/ControlFlow/LabelStmt.h"

class EvaluatingVisitor : public Visitor {
public:
    EvaluatingVisitor();
    EvaluatingVisitor(std::shared_ptr<Context> ctx);
    EvaluatingVisitor(const EvaluatingVisitor&);

    virtual Variant evaluate(Statement*);
    virtual Variant evaluate(CompoundStmt*);

    virtual Variant visit(FunctionDecl*);
    virtual Variant visit(CompoundStmt*);
    virtual Variant visit(IdentifierRefExpr*);
    virtual Variant visit(DeclStmt*);
    virtual Variant visit(ForStmt*);
    virtual Variant visit(WhileStmt*);

    virtual Variant visit(ArrayLiteral*);
    virtual Variant visit(LiteralExpr*);
    virtual Variant visit(StringLiteral*);
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
    virtual Variant visit(SwitchStmt*);
    virtual Variant visit(CaseStmt*);
    virtual Variant visit(LabelStmt*);
    virtual Variant visit(GotoStmt*);
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
    virtual Variant visit(LambdaExpr*);
    virtual Variant visit(OperatorDecl*);

    inline Context* get_ctx() {
        return context.get();
    }
    inline void catch_undeclared_identifiers() {
        catch_undeclared = true;
        undeclared_identifiers = std::vector<std::string>();
    }
protected:

    /** Context */
    std::shared_ptr<Context> context;
    bool catch_undeclared = false;
    std::vector<std::string> undeclared_identifiers;

    /** Identifier evaluation */
    Variant::SharedPtr current_obj_ref;

    /** Object and function declarations */
    ObjectProp current_prop;
    FuncArg current_arg;
    Function* current_constr;
    Class::UniquePtr current_class;

    /** Goto label */
    LabelStmt* current_label;
    int start_index = 0;
};


#endif //CDOT_LITERALEXPRVISITOR_H
