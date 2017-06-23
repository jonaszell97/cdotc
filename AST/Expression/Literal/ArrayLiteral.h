//
// Created by Jonas Zell on 21.06.17.
//

#ifndef MATHPARSER_ARRAYLITERAL_H
#define MATHPARSER_ARRAYLITERAL_H


#include "../../AstNode.h"
#include "../Expression.h"

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(ValueType = ANY_T);
    ArrayLiteral(ValueType, Expression::SharedPtr);
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
    VariantPtr evaluate(VariantPtr = {});

    typedef std::shared_ptr<ArrayLiteral> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

protected:
    std::vector<Expression::SharedPtr> _elements;
    ValueType _type;
    Expression::SharedPtr _length_expr;
    int _length;
    bool _var_length = false;
};


#endif //MATHPARSER_ARRAYLITERAL_H
