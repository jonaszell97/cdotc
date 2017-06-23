//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_UNARYOPERATOR_H
#define MATHPARSER_UNARYOPERATOR_H

#include "Operator.h"
#include "../Expression/Expression.h"

class UnaryOperator : public Operator {
public:
    UnaryOperator(std::string, std::string);
    UnaryOperator(Variant, std::string);
    VariantPtr evaluate(VariantPtr = {});
    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<UnaryOperator> SharedPtr;
    void __dump(int);

protected:
    std::string _operator;
    std::string __class_name = "UnaryOperator";
    bool prefix;
};


#endif //MATHPARSER_UNARYOPERATOR_H
