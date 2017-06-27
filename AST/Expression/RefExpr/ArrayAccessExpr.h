//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYACCESSEXPR_H
#define CDOT_ARRAYACCESSEXPR_H


#include "../../../Variant/Variant.h"
#include "../Expression.h"
#include "RefExpr.h"

class ArrayAccessExpr : public RefExpr {
public:
    ArrayAccessExpr(int);
    ArrayAccessExpr(Expression::SharedPtr);
    ArrayAccessExpr(const ArrayAccessExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<ArrayAccessExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    Expression::SharedPtr _index;
};


#endif //CDOT_ARRAYACCESSEXPR_H
