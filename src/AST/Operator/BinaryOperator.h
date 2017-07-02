//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H


#include "../AstNode.h"
#include <string>
#include "Operator.h"
#include "../Expression/Expression.h"

class BinaryOperator : public Operator {
public:
    BinaryOperator(std::string);
    BinaryOperator(const BinaryOperator& cp);
    virtual AstNode::SharedPtr clone() const;

    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();
    void set_fst_child(Expression::SharedPtr);
    void set_snd_child(Expression::SharedPtr);

    typedef std::shared_ptr<BinaryOperator> SharedPtr;
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    Expression::SharedPtr _first_child;
    Expression::SharedPtr _second_child;
    std::string _operator;
    std::string __class_name = "BinaryOperator";
};


#endif //CDOT_BINARYOPERATOR_H
