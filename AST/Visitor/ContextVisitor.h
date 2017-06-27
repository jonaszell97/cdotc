//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_CONTEXTVISITOR_H
#define CDOT_CONTEXTVISITOR_H


#include "Visitor.h"

class ContextVisitor : public Visitor {
public:
    ContextVisitor(bool = false);
    ContextVisitor(const ContextVisitor&);

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

protected:
    bool is_progr_root;
    Statement *root_stmt;
    std::string root_type;
    CompoundStmt* root;
    std::vector<std::string> declared_identifiers;
    std::vector<std::string> referenced_identifiers;
};


#endif //CDOT_CONTEXTVISITOR_H
