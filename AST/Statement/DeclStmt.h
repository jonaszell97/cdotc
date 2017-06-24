//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_DECLSTATEMENT_H
#define MATHPARSER_DECLSTATEMENT_H


#include "Statement.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"

class DeclStmt : public Statement {
public:
    DeclStmt(std::string, Expression::SharedPtr, ValueType = ANY_T);
    Variant evaluate(Variant = {});
    std::vector<AstNode::SharedPtr> get_children();
    Expression::SharedPtr get_expr();

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int);

protected:
    std::string _ident;
    Expression::SharedPtr _val;
    ValueType _type;
    std::string __class_name = "DeclStmt";
};


#endif //MATHPARSER_DECLSTATEMENT_H
