//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "../AstNode.h"
#include "../Statement/Statement.h"

class Expression : public Statement {
public:
    typedef std::shared_ptr<Expression> SharedPtr;

    Expression();
    Expression(Expression::SharedPtr);

    virtual void set_child(Expression::SharedPtr);

    virtual inline void return_ref(bool ref) {
        _return_ref = ref;
    }
    virtual inline void implicit_ref(bool implicit) {
        _implicit_ref = implicit;
    }
    virtual inline void set_member_expr(std::shared_ptr<Expression> ref_expr) {
        _member_expr = ref_expr;
    }

    std::vector<AstNode::SharedPtr> get_children();

    virtual void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    Expression::SharedPtr _child;
    bool _implicit_ref = false;
    Expression::SharedPtr _member_expr;
    bool _return_ref = false;
};


#endif //EXPRESSION_H
