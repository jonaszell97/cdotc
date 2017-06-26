//
// Created by Jonas Zell on 25.06.17.
//

#include "Class.h"

//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Class.h"
#include "Objects/Function.h"

Class::Class(std::string name, Method constructor, AccessModifier am) :
    _class_name(name),
    access_modifier(am),
    constructor(constructor),
    properties(std::vector<std::string>),
    methods(std::map<std::string, Function::SharedPtr>()),
    static_properties(std::map<std::string, Variant::SharedPtr>()),
    static_methods(std::map<std::string, Function::SharedPtr>()),
    _access_modifiers(std::map<std::string, AccessModifier>()),
    _builtin_methods({
        {"toString", cdot::lib::obj::toString}
    }),
    _builtin_static_methods({
        {"hashCode", cdot::lib::obj::hashCode}
    })
{

}

Class::Class(const Class &cp) {

}

void Class::add_method(std::string func_name, Function::SharedPtr func, AccessModifier am) {
    methods.insert(std::pair<std::string, Function::SharedPtr>(func_name, func));
    _access_modifiers.emplace(func_name, am);
}

void Class::add_property(std::string prop_name, Variant::SharedPtr prop, AccessModifier am) {
    properties.insert(std::pair<std::string, Variant::SharedPtr>(prop_name, prop));
    _access_modifiers.emplace(prop_name, am);
}

void Class::add_static_method(std::string func_name, Function::SharedPtr func, AccessModifier am) {
    static_methods.insert(std::pair<std::string, Function::SharedPtr>(func_name, func));
    _access_modifiers.emplace(func_name, am);
}

void Class::add_static_property(std::string prop_name, Variant::SharedPtr prop, AccessModifier am) {
    static_properties.insert(std::pair<std::string, Variant::SharedPtr>(prop_name, prop));
    _access_modifiers.emplace(prop_name, am);
}

Variant Class::access_static_property(std::string prop_name) {
    if (static_properties.find(prop_name) != static_properties.end()) {
        return { static_properties[prop_name] };
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + class_name());
}

Variant Class::call_method(std::string method_name, Object *this_arg, std::vector<Variant> args) {
    if (methods.find(method_name) != methods.end()) {
        return methods.at(method_name)->call(args);
    }

    if (_builtin_methods.find(method_name) != _builtin_methods.end()) {
        return _builtin_methods.at(method_name)(this_arg, args);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Method " + method_name + " does not exist on Object of type " + class_name());
}

Variant Class::call_static_method(std::string method_name, std::vector<Variant> args) {
    if (static_methods.find(method_name) != static_methods.end()) {
        return static_methods.at(method_name)->call(args);
    }

    if (_builtin_static_methods.find(method_name) != _builtin_static_methods.end()) {
        return _builtin_static_methods.at(method_name)(args);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Static method " + method_name + " does not exist on " + class_name());
}

Object::SharedPtr Class::instantiate(std::vector<Variant> args) {
    Object::SharedPtr obj = std::make_shared<Object>(this);
    for (auto prop : properties) {
        obj->_properties.emplace(prop, std::make_shared<Variant>());
    }

    constructor.call(obj, args);
    
    return obj;
}

namespace cdot {
namespace lib {

    void assert_args(int size, std::vector<ValueType> types, std::vector<Variant> args, std::string name) {
        if (args.size() != size) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "Wrong number of arguments supplied for call to function " + name);
        }

        for (int i = 0; i < args.size(); ++i) {
            if (!val::is_compatible(args[i].get_type(), types[i])) {
                RuntimeError::raise(ERR_TYPE_ERROR, "No matching call found for function " + name);
            }
        }
    }

namespace obj {

    /******************************************/
    /*                                        */
    /*           INSTANCE METHODS             */
    /*                                        */
    /******************************************/

    Variant toString(Object *this_arg, std::vector<Variant> args) {
        assert_args(0, {}, args, "toString");

        return this_arg->print();
    }

    /******************************************/
    /*                                        */
    /*             STATIC METHODS             */
    /*                                        */
    /******************************************/

    Variant hashCode(std::vector<Variant> args) {
        assert_args(1, { ANY_T }, args, "hashCode");

        return args[0].hash();
    }

} // namespace obj
} // namespace lib
} // namespace cdot