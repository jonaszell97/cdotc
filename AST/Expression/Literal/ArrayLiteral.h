//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H


#include "../../AstNode.h"
#include "../Expression.h"

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(ValueType = ANY_T);
    ArrayLiteral(ValueType, Expression::SharedPtr);
    ArrayLiteral(const ArrayLiteral& cp);
    virtual AstNode::SharedPtr clone() const;
    inline void is_var_length(bool var) {
        _var_length = var;
        _length = -1;
    }
    inline void set_type(ValueType type) {
        _type = type;
    }
    inline void set_length(Expression::SharedPtr length_expr) {
        _length_expr = length_expr;
    }
    inline void set_length(int len) {
        _length = len;
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
    friend class ContextVisitor;

protected:
    std::vector<Expression::SharedPtr> _elements;
    ValueType _type;
    Expression::SharedPtr _length_expr;
    int _length;
    bool _var_length = false;
};


#endif //CDOT_ARRAYLITERAL_H
