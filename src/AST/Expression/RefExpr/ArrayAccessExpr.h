//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYACCESSEXPR_H
#define CDOT_ARRAYACCESSEXPR_H


#include "../../../Variant/Variant.h"
#include "../Expression.h"
#include "RefExpr.h"
#include "CallExpr.h"

class ArrayAccessExpr : public Expression {
public:
    ArrayAccessExpr(int);
    ArrayAccessExpr(Expression::SharedPtr);

    typedef std::shared_ptr<ArrayAccessExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::ARRAY_ACCESS_EXPR;
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
    Expression::SharedPtr _index;

    // codegen
    TypeSpecifier arr_type;
    bool is_subscript_op = false;
    CallExpr::SharedPtr overriden_call;
};


#endif //CDOT_ARRAYACCESSEXPR_H
