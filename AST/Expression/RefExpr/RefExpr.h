//
// Created by Jonas Zell on 21.06.17.
//

#ifndef MATHPARSER_REFEXPR_H
#define MATHPARSER_REFEXPR_H


#include "../../../Variant.h"
#include "../../AstNode.h"
#include "../Expression.h"

class RefExpr : public Expression {
public:
    virtual VariantPtr evaluate(VariantPtr = {}) = 0;
    virtual void set_member_expr(std::shared_ptr<RefExpr>) = 0;
    virtual void return_ref(bool) = 0;

    typedef std::shared_ptr<RefExpr> SharedPtr;
    virtual std::vector<AstNode::SharedPtr> get_children() = 0;
    virtual void __dump(int) = 0;
};


#endif //MATHPARSER_REFEXPR_H
