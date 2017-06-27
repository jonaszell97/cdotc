//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/RefExpr/IdentifierRefExpr.h"

class DeclStmt : public Statement {
public:
    DeclStmt(std::string, Expression::SharedPtr, TypeSpecifier);
    DeclStmt(std::string, TypeSpecifier);
    DeclStmt(const DeclStmt& cp);
    virtual AstNode::SharedPtr clone() const;

    std::vector<AstNode::SharedPtr> get_children();
    Expression::SharedPtr get_expr();

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _ident;
    Expression::SharedPtr _val;
    TypeSpecifier _type;
    std::string __class_name = "DeclStmt";
};


#endif //CDOT_DECLSTATEMENT_H
