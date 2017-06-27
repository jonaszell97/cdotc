//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "Operator.h"
#include "../Expression/Expression.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"

class UnaryOperator : public Operator {
public:
    UnaryOperator(std::string, std::string);
    UnaryOperator(Variant, std::string);
    UnaryOperator(const UnaryOperator& cp);
    virtual AstNode::SharedPtr clone() const;

    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<UnaryOperator> SharedPtr;
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _operator;
    std::string __class_name = "UnaryOperator";
    bool prefix;
};


#endif //CDOT_UNARYOPERATOR_H
