//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_INPUTSTATEMENT_H
#define MATHPARSER_INPUTSTATEMENT_H


#include "../Statement.h"
#include "../DeclStmt.h"
#include "../../Expression/Literal/LiteralExpr.h"

class InputStmt : public Statement {
public:
    InputStmt(std::string, ValueType = ANY_T);
    InputStmt(Variant, ValueType = ANY_T);
    Variant evaluate(Variant = {});

    void __dump(int);
protected:
    std::string _ident;
    ValueType _type;
    DeclStmt::SharedPtr _decl;
    LiteralExpr::SharedPtr _literal;
    std::string __class_name = "InputStmt";
};


#endif //MATHPARSER_INPUTSTATEMENT_H
