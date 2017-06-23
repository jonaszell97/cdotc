//
// Created by Jonas Zell on 18.06.17.
//

#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>
#include <vector>
#include "../Variant.h"

class CompoundStmt;

class FuncArgument {
public:
    FuncArgument(std::string, ValueType, VariantPtr = std::make_shared<Variant>());
    VariantPtr get_default();
    ValueType get_type();
    std::string get_name();
    bool has_default();

    typedef std::shared_ptr<FuncArgument> SharedPtr;
protected:
    std::string _arg_name;
    VariantPtr _default;
    ValueType _type;
    bool _has_default;
};

class Function {
public:
    Function(std::string, ValueType = VOID_T);
    Function(const Function&);
    void add_argument(FuncArgument);
    void set_return_type(ValueType);
    void set_body(std::shared_ptr<CompoundStmt>);
    std::string get_name();
    VariantPtr call(std::vector<VariantPtr>);
    std::string print();

    typedef std::shared_ptr<Function> SharedPtr;
protected:
    std::string _function_name;
    std::vector<FuncArgument> _arguments;
    ValueType _return_type;
    std::shared_ptr<CompoundStmt> _func_body;
};


#endif //FUNCTION_H
