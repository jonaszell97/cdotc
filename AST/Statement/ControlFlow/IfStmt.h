//
// Created by Jonas Zell on 19.06.17.
//

#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"
#include "../CompoundStmt.h"

class IfStmt : public Statement {
public:
    IfStmt(Expression::SharedPtr, CompoundStmt::SharedPtr, CompoundStmt::SharedPtr);

protected:
    Expression::SharedPtr _condition;
    CompoundStmt::SharedPtr _if_branch;
    CompoundStmt::SharedPtr _else_branch;
    std::string __class_name = "IfStmt";
};


#endif //IFSTATEMENT_H
