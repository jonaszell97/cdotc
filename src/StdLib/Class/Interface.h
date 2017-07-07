//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_INTERFACE_H
#define CDOT_INTERFACE_H


#include "Class.h"

class Interface {
public:
    Interface(std::string);

    void declare_field(std::string, TypeSpecifier, bool);
    void declare_method(std::string, std::vector<TypeSpecifier>, TypeSpecifier, bool);

    inline virtual bool is_instantiable() {
        return false;
    }
    inline std::string get_name() {
        return name;
    }

    typedef std::unique_ptr<Interface> UniquePtr;

    friend class Class;

protected:
    std::string name;

    /** Properties and methods */
    std::vector<std::pair<std::string, TypeSpecifier>> properties;
    std::vector<std::pair<std::string, TypeSpecifier>> methods;
    std::vector<std::vector<TypeSpecifier>> method_args;

    /** Static properties and methods */
    std::vector<std::pair<std::string, TypeSpecifier>> static_properties;
    std::vector<std::pair<std::string, TypeSpecifier>> static_methods;
    std::vector<std::vector<TypeSpecifier>> static_method_args;
};


#endif //CDOT_INTERFACE_H
