//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H


#include "../Expression.h"

class LiteralExpr : public Expression {
public:
    LiteralExpr();
    LiteralExpr(Variant);
    LiteralExpr(Variant, ValueType);
    LiteralExpr(const LiteralExpr& cp);
    virtual AstNode::SharedPtr clone() const;
    void set_val(Variant);
    virtual Variant evaluate(Variant = {});

    typedef std::shared_ptr<LiteralExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    Variant _value;
    ValueType _type;
    std::string __class_name = "LiteralExpr";
};


#endif //CDOT_LITERALEXPRESSION_H
