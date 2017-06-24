//
// Created by Jonas Zell on 18.06.17.
//

#ifndef FUNCTION_H
#define FUNCTION_H


#include <string>
#include <vector>
#include "../Variant/Variant.h"
#include "Object.h"

class CompoundStmt;

class Function : public Object {
public:
    Function(std::string, ValueType = VOID_T);

    void add_argument(std::string, ValueType, Variant = {});
    void set_return_type(ValueType);
    void set_body(std::shared_ptr<CompoundStmt>);

    std::string get_name();
    Variant call(std::vector<Variant>);
    std::string print();

    typedef std::shared_ptr<Function> SharedPtr;

protected:
    std::string function_name;
    ValueType return_type;
    std::shared_ptr<CompoundStmt> func_body;

    int num_args;
    std::vector<std::string> arg_names;
    std::vector<ValueType> arg_types;
    std::map<int, Variant> arg_defaults;
};


#endif //FUNCTION_H
