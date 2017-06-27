//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_TERTIARYOPERATOR_H
#define CDOT_TERTIARYOPERATOR_H

#include "Operator.h"
#include "../Statement/CompoundStmt.h"

class TertiaryOperator : public Operator {
public:
    TertiaryOperator(Expression::SharedPtr, Expression::SharedPtr, Expression::SharedPtr);
    TertiaryOperator(const TertiaryOperator& cp);
    inline std::string get_operator() {
        return "?:";
    }
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<TertiaryOperator> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;
protected:
    Expression::SharedPtr _condition;
    Expression::SharedPtr _if_branch;
    Expression::SharedPtr _else_branch;
    std::string __class_name = "TertiaryOperator";
};


#endif //CDOT_TERTIARYOPERATOR_H
