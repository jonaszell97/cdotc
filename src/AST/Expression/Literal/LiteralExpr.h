//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H


#include "../Expression.h"

class LiteralExpr : public Expression {
public:
    LiteralExpr();
    LiteralExpr(Variant);
    LiteralExpr(Variant, TypeSpecifier);

    void set_val(Variant);

    typedef std::shared_ptr<LiteralExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    Variant _value;
    TypeSpecifier _type;
};


#endif //CDOT_LITERALEXPRESSION_H
