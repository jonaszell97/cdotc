//
// Created by Jonas Zell on 13.07.17.
//

#ifndef CDOT_IMPLICITCASTEXPR_H
#define CDOT_IMPLICITCASTEXPR_H

#include "../Expression/Expression.h"

class ImplicitCastExpr : public Expression {
public:
    ImplicitCastExpr(TypeSpecifier from, TypeSpecifier to, Expression::SharedPtr);

    typedef std::shared_ptr<ImplicitCastExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }

    virtual NodeType get_type() {
        return NodeType::IMPLICIT_CAST_EXPR;
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
    friend class ConstExprVisitor;

protected:
    TypeSpecifier from;
    TypeSpecifier to;
    Expression::SharedPtr target;
};


#endif //CDOT_IMPLICITCASTEXPR_H
