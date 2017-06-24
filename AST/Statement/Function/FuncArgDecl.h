//
// Created by Jonas Zell on 20.06.17.
//

#ifndef MATHPARSER_FUNCARGDECL_H
#define MATHPARSER_FUNCARGDECL_H


#include "../../../Objects/Function.h"
#include "../../Expression/Expression.h"

struct FuncArg {
    std::string name;
    ValueType type;
    Variant default_val = {};
};

class FuncArgDecl : public Expression {
public:
    FuncArgDecl();
    FuncArgDecl(std::string, ValueType, Expression::SharedPtr = {});
    void set_name(std::string);
    void set_type(ValueType);
    void set_default(Expression::SharedPtr);
    Variant evaluate(Variant = {});

    FuncArg specific_eval();

    typedef std::shared_ptr<FuncArgDecl> SharedPtr;
    void __dump(int);

protected:
    std::string _arg_name;
    ValueType _arg_type;
    Expression::SharedPtr _default_val;
};


#endif //MATHPARSER_FUNCARGDECL_H
