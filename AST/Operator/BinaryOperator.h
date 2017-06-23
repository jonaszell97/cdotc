//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_BINARYOPERATOR_H
#define MATHPARSER_BINARYOPERATOR_H


#include "../AstNode.h"
#include <string>
#include "Operator.h"
#include "../Expression/Expression.h"

class BinaryOperator : public Operator {
public:
    BinaryOperator(std::string);
    VariantPtr evaluate(VariantPtr = {});
    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();
    void set_fst_child(Expression::SharedPtr);
    void set_snd_child(Expression::SharedPtr);

    typedef std::shared_ptr<BinaryOperator> SharedPtr;
    void __dump(int);

protected:
    Expression::SharedPtr _first_child;
    Expression::SharedPtr _second_child;
    std::string _operator;
    std::string __class_name = "BinaryOperator";
};


#endif //MATHPARSER_BINARYOPERATOR_H
