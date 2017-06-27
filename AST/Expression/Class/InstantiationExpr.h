//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_INSTANTIATIONEXPR_H
#define CDOT_INSTANTIATIONEXPR_H


#include "../../../Variant/Variant.h"
#include "../../AstNode.h"
#include "../Expression.h"

class InstantiationExpr : public Expression {
public:
    InstantiationExpr(std::string, std::vector<Expression::SharedPtr>);
    InstantiationExpr(const InstantiationExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<InstantiationExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string class_name;
    std::vector<Expression::SharedPtr> constr_args;
};


#endif //CDOT_INSTANTIATIONEXPR_H
