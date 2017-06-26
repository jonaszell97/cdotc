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
    Variant evaluate(Variant = {});

    typedef std::shared_ptr<ArrayAccessExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    Expression::SharedPtr _index;
};


#endif //CDOT_ARRAYACCESSEXPR_H
