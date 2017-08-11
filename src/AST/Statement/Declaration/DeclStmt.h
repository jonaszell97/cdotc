//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/RefExpr/IdentifierRefExpr.h"

class DeclStmt : public Statement {
public:
    DeclStmt(string, TypeRef::SharedPtr, bool, bool, Expression::SharedPtr = nullptr);

    std::vector<AstNode::SharedPtr> get_children() override;

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int) override;

    inline virtual NodeType get_type() override {
        return NodeType::DECLARATION;
    }
    virtual inline Variant accept(Visitor& v) override {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    string identifier;
    TypeRef::SharedPtr type;
    Expression::SharedPtr value;
    bool is_const;
    bool is_global;
};


#endif //CDOT_DECLSTATEMENT_H
