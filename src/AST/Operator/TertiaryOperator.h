//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_TERTIARYOPERATOR_H
#define CDOT_TERTIARYOPERATOR_H

#include "Operator.h"
#include "../Statement/Block/CompoundStmt.h"

class TertiaryOperator : public Operator {
public:
    TertiaryOperator(Expression::SharedPtr, Expression::SharedPtr, Expression::SharedPtr);

    inline std::string get_operator() {
        return "?:";
    }

    typedef std::shared_ptr<TertiaryOperator> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::TERTIARY_OPERATOR;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
protected:
    Expression::SharedPtr condition;
    Expression::SharedPtr lhs;
    Expression::SharedPtr rhs;

    // codegen
    TypeSpecifier result_type;
};


#endif //CDOT_TERTIARYOPERATOR_H
