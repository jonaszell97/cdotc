//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "Statement.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"

class DeclStmt : public Statement {
public:
    DeclStmt(std::string, Expression::SharedPtr, TypeSpecifier);
    DeclStmt(std::string, TypeSpecifier);
    DeclStmt(const DeclStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    Variant evaluate(Variant = {});
    std::vector<AstNode::SharedPtr> get_children();
    Expression::SharedPtr get_expr();

    inline void set_context(Context::SharedPtr ctx) {
        context = ctx;
    }

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    Context::SharedPtr context;
    std::string _ident;
    Expression::SharedPtr _val;
    TypeSpecifier _type;
    std::string __class_name = "DeclStmt";
};


#endif //CDOT_DECLSTATEMENT_H
