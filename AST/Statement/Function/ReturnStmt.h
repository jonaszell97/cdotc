//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_RETURNSTATEMENT_H
#define MATHPARSER_RETURNSTATEMENT_H


#include "../Statement.h"
#include "../../Expression/Expression.h"

class ReturnStmt : public Statement {
public:
    ReturnStmt(Expression::SharedPtr);
    VariantPtr evaluate(VariantPtr = {});

    typedef std::shared_ptr<ReturnStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);
protected:
    Expression::SharedPtr _return_val;
    std::string __class_name = "ReturnStmt";
};


#endif //MATHPARSER_RETURNSTATEMENT_H
