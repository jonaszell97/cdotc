//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_TERTIARYOPERATOR_H
#define MATHPARSER_TERTIARYOPERATOR_H

#include "Operator.h"
#include "../Statement/CompoundStmt.h"

class TertiaryOperator : public Operator {
public:
    TertiaryOperator(std::string);

protected:
    Expression::SharedPtr _condition;
    Expression::SharedPtr _if_branch;
    Expression::SharedPtr _else_branch;
    std::string __class_name = "TertiaryOperator";
};


#endif //MATHPARSER_TERTIARYOPERATOR_H
