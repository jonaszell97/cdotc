//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_LITERALEXPRESSION_H
#define MATHPARSER_LITERALEXPRESSION_H


#include "../Expression.h"

class LiteralExpr : public Expression {
public:
    LiteralExpr();
    LiteralExpr(Variant);
    LiteralExpr(Variant, ValueType);
    void set_val(Variant);
    virtual Variant evaluate(Variant = {});

    typedef std::shared_ptr<LiteralExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

protected:
    Variant _value;
    ValueType _type;
    std::string __class_name = "LiteralExpr";
};


#endif //MATHPARSER_LITERALEXPRESSION_H
