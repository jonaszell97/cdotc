//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_FUNCTIONCALLEXPR_H
#define CDOT_FUNCTIONCALLEXPR_H


#include "../../../Variant/Variant.h"
#include "../../Expression/Expression.h"

class FunctionCallExpr : public Expression {
public:
    FunctionCallExpr(std::string, std::vector<Expression::SharedPtr>);

    typedef std::shared_ptr<FunctionCallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string _ident;
    std::vector<Expression::SharedPtr> args;
};


#endif //CDOT_FUNCTIONCALLEXPR_H
