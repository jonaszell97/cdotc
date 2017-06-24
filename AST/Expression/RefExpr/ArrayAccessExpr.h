//
// Created by Jonas Zell on 21.06.17.
//

#ifndef MATHPARSER_ARRAYACCESSEXPR_H
#define MATHPARSER_ARRAYACCESSEXPR_H


#include "../../../Variant/Variant.h"
#include "../Expression.h"
#include "RefExpr.h"

class ArrayAccessExpr : public RefExpr {
public:
    ArrayAccessExpr(int);
    ArrayAccessExpr(Expression::SharedPtr);
    Variant evaluate(Variant = {});
    void return_ref(bool);

    typedef std::shared_ptr<ArrayAccessExpr> SharedPtr;
    void set_member_expr(std::shared_ptr<RefExpr>);
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

protected:
    Expression::SharedPtr _index;
    std::shared_ptr<RefExpr> _member_expr;
    bool _return_ref = false;
};


#endif //MATHPARSER_ARRAYACCESSEXPR_H
