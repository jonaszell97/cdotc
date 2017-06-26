//
// Created by Jonas Zell on 23.06.17.
//

#ifndef CDOT_EXPLICITCASTEXPR_H
#define CDOT_EXPLICITCASTEXPR_H


#include "Operator.h"

class ExplicitCastExpr : public Operator {
public:
    ExplicitCastExpr(std::string);
    ExplicitCastExpr(const ExplicitCastExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    Variant evaluate(Variant = {});
    inline std::string get_operator() {
        return _operator;
    }
    inline void set_child(Expression::SharedPtr child) {
        _child = child;
    }

    typedef std::shared_ptr<ExplicitCastExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _operator;
    Expression::SharedPtr _child;
};


#endif //CDOT_EXPLICITCASTEXPR_H
