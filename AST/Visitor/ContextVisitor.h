//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_CONTEXTVISITOR_H
#define CDOT_CONTEXTVISITOR_H


#include "Visitor.h"

class ContextVisitor : public Visitor {
public:
    ContextVisitor();
    ContextVisitor(Context::SharedPtr);
    ContextVisitor(const ContextVisitor&);

    inline virtual Context::SharedPtr get_context() {
        return context;
    }

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

protected:
    Context::SharedPtr context;
    Statement *root_stmt;
    std::string root_type;
    CompoundStmt* func_context;
    std::vector<std::string> declared_identifiers;
    std::vector<std::string> referenced_identifiers;
};


#endif //CDOT_CONTEXTVISITOR_H
