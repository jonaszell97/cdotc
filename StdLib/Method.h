//
// Created by Jonas Zell on 26.06.17.
//

#ifndef CDOT_METHOD_H
#define CDOT_METHOD_H

#include <string>
#include <vector>
#include "../Variant/Variant.h"
#include "Objects/Object.h"

class CompoundStmt;
class Class;

class Method {
public:
    Method(std::string, std::shared_ptr<CompoundStmt>, std::vector<std::pair<std::string, ValueType>>);
    Method(std::string, Variant (*)(Object*, std::vector<Variant>));
    virtual Variant call(Object::SharedPtr this_arg, std::vector<Variant> args);

protected:
    std::string class_name;
    std::shared_ptr<CompoundStmt> body;
    Variant (*_internal_body)(Object*, std::vector<Variant>);
    bool _is_lib_method = false;
    std::vector<std::pair<std::string, ValueType>> signature;
};


#endif //CDOT_METHOD_H
