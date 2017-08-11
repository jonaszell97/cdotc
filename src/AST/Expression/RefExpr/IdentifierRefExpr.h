//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H


#include <string>
#include "../Expression.h"
#include "RefExpr.h"
#include "../../Context.h"

class MemberRefExpr;

class IdentifierRefExpr : public Expression {
public:
    IdentifierRefExpr(std::string);
    IdentifierRefExpr(Variant);

    typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::IDENTIFIER_EXPR;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string _ident;

    // codegen
    TypeSpecifier target_type;
    bool is_class = false;
    bool is_this = false;

    bool is_captured_var = false;
    bool is_ns = false;
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
