//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_METHOD_H
#define CDOT_METHOD_H

#include "../AST/Statement/CompoundStmt.h"

class Class;

class Method {
public:
    Method(std::string class_name, CompoundStmt::SharedPtr body, std::vector<std::pair<std::string, ValueType>>);
    Variant call(Object::SharedPtr this_arg, std::vector<Variant> args);

protected:
    std::string class_name;
    CompoundStmt::SharedPtr body;
    std::vector<std::pair<std::string, ValueType>> signature;
};


#endif //CDOT_METHOD_H
