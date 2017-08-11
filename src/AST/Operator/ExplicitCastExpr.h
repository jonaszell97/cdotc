//
// Created by Jonas Zell on 23.06.17.
//

#ifndef CDOT_EXPLICITCASTEXPR_H
#define CDOT_EXPLICITCASTEXPR_H


#include "Operator.h"

class ExplicitCastExpr : public Operator {
public:
    ExplicitCastExpr(TypeSpecifier);
    ExplicitCastExpr(TypeSpecifier, Expression::SharedPtr);

    inline std::string get_operator() {
        return _operator;
    }
    inline void set_child(Expression::SharedPtr child) {
        _child = child;
    }

    typedef std::shared_ptr<ExplicitCastExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::EXPLICIT_CAST_EXPR;
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
    TypeSpecifier from;
    TypeSpecifier to;
    Expression::SharedPtr _child;
};


#endif //CDOT_EXPLICITCASTEXPR_H
