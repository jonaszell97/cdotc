//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/RefExpr/IdentifierRefExpr.h"

class DeclStmt : public Statement {
public:
    DeclStmt();

    void add_declaration(std::string, TypeSpecifier, Expression::SharedPtr = {});
    inline size_t size() {
        return decl_identifiers.size();
    }

    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    std::vector<std::string> decl_identifiers;
    std::vector<std::pair<TypeSpecifier, Expression::SharedPtr>> declarations;
};


#endif //CDOT_DECLSTATEMENT_H
