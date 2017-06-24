//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_OUTPUTSTATEMENT_H
#define MATHPARSER_OUTPUTSTATEMENT_H


#include <string>
#include "../../Expression/Expression.h"
#include "../Statement.h"

class OutputStmt : public Statement {
public:
    OutputStmt(Expression::SharedPtr, bool = false);
    Variant evaluate(Variant = {});

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);
protected:
    Expression::SharedPtr _value;
    bool _newline;
    std::string __class_name = "OutputStmt";
};


#endif //MATHPARSER_OUTPUTSTATEMENT_H
