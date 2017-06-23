//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_WHILESTATEMENT_H
#define MATHPARSER_WHILESTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"

class WhileStmt : public Statement {
public:
    WhileStmt(Expression::SharedPtr, CompoundStmt::SharedPtr);

protected:
    Expression::SharedPtr _condition;
    CompoundStmt::SharedPtr _while_branch;
    std::string __class_name = "WhileStmt";
};


#endif //MATHPARSER_WHILESTATEMENT_H
