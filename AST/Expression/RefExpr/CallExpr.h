//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"
#include "RefExpr.h"

class CallExpr : public RefExpr {
public:
    CallExpr();
    CallExpr(const CallExpr& cp);
    virtual AstNode::SharedPtr clone() const;
    void add_argument(Expression::SharedPtr);

    typedef std::shared_ptr<CallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;
    friend class MethodCallExpr;
protected:
    std::vector<Expression::SharedPtr> _arguments;
};


#endif //CDOT_CALLEXPR_H
