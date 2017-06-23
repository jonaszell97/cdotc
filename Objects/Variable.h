//
// Created by Jonas Zell on 19.06.17.
//

#ifndef VARIABLE_H
#define VARIABLE_H


#include <string>
#include "../Variant.h"

class Variable {
public:
    Variable(Variant, bool = false, std::string = "");
    Variable(const Variable&);
    Variant get();
    void reassign(Variant);
    void is_any();

    typedef std::shared_ptr<Variable> SharedPtr;
protected:
    Variant _value;
    std::string _name;
    ValueType _type;
};


#endif //VARIABLE_H
