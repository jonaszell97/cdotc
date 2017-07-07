//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H


#include "../../AstNode.h"
#include "../Expression.h"

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(TypeSpecifier = {});

    inline void is_var_length(bool var) {
        _var_length = var;
        _length = -1;
    }
    inline void set_type(TypeSpecifier _type) {
        type = _type;
    }
    inline void set_length(int len) {
        _length = len;
    }
    inline void set_length_expr(Expression::SharedPtr len) {
        _length_expr = len;
    }
    void add_element(Expression::SharedPtr);

    typedef std::shared_ptr<ArrayLiteral> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    std::vector<Expression::SharedPtr> _elements;
    Expression::SharedPtr _length_expr;
    TypeSpecifier type;
    int _length;
    bool _var_length = false;
};


#endif //CDOT_ARRAYLITERAL_H
