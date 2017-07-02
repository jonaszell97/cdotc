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

Class::Class(std::string name, Function constructor, AccessModifier am) :
    _class_name(name),
    access_modifier(am),
    constructor(constructor),
    properties(std::map<std::string, Variant::SharedPtr>()),
    property_types{},
    methods(std::map<std::string, Function>()),
    static_properties(std::map<std::string, Variant::SharedPtr>()),
    static_property_types{},
    static_methods(std::map<std::string, Function>()),
    unary_operators(std::unordered_map<std::string, Function>()),
    binary_operators(std::unordered_map<std::string, Function>()),
    _access_modifiers(std::map<std::string, AccessModifier>()),
    _non_nullable_fields{},
    _dynamic_methods(std::map<std::string, std::function<Variant(Object*, std::vector<Variant>)>>()),
    _builtin_methods({
        {"toString", cdot::lib::obj::toString}
    }),
    _builtin_static_methods({
        {"hashCode", cdot::lib::obj::hashCode}
    })
{

}

/**
 * Defines a method on the class prototype that can be dynamically called with different 'this' values
 * @param func_name
 * @param func
 * @param am
 */
void Class::add_method(std::string func_name, Function func, AccessModifier am) {
    methods.insert(std::pair<std::string, Function>(func_name, func));
    _access_modifiers.emplace(func_name, am);
}

/**
 * Adds a property to the class prototype that is inherited by every instance
 * @param prop_name
 * @param prop
 * @param am
 */
void Class::add_property(std::string prop_name, TypeSpecifier type, Variant::SharedPtr default_val, AccessModifier am) {
    if (!type.nullable) {
        _non_nullable_fields.push_back(prop_name);
    }

    property_types.emplace(prop_name, type);
    properties.emplace(prop_name, default_val);
    _access_modifiers.emplace(prop_name, am);
}

/**
 * Adds a static method to the class
 * @param func_name
 * @param func
 * @param am
 */
void Class::add_static_method(std::string func_name, Function func, AccessModifier am) {
    static_methods.insert(std::pair<std::string, Function>(func_name, func));
    _access_modifiers.emplace(func_name, am);
}

/**
 * Adds a static property to the class
 * @param prop_name
 * @param prop
 * @param am
 */
void Class::add_static_property(std::string prop_name, TypeSpecifier type, Variant::SharedPtr prop, AccessModifier am) {
    static_property_types.emplace(prop_name, type);
    static_properties.insert(std::pair<std::string, Variant::SharedPtr>(prop_name, prop));
    _access_modifiers.emplace(prop_name, am);
}

/**
 * Adds a unary operator
 * @param op
 * @param fun
 * @param am
 */
void Class::add_unary_operator(std::string op, Function fun, AccessModifier am) {
    unary_operators.emplace(op, fun);
    _access_modifiers.emplace("unary_"+op, am);
}

/**
 * Adds a binary operator
 * @param op
 * @param fun
 * @param am
 */
void Class::add_binary_operator(std::string op, Function fun, AccessModifier am) {
    binary_operators.emplace(op, fun);
    _access_modifiers.emplace("binary_"+op, am);
}

/**
 * Returns a static property, if it exists
 * @param prop_name
 * @return
 */
Variant Class::access_static_property(std::string prop_name, std::string class_context) {
    if (static_properties.find(prop_name) != static_properties.end()) {
        if (!is_accessible(prop_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access private property " + prop_name + " of class " + _class_name);
        }

        return { static_properties[prop_name] };
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + class_name());
}

/**
 * Calls an instance method with the given this value
 * @param method_name
 * @param this_arg
 * @param args
 * @return
 */
Variant Class::call_method(std::string method_name, Object::SharedPtr this_arg, std::vector<Variant> args,
       std::string class_context)
{
    if (methods.find(method_name) != methods.end()) {
        if (!is_accessible(method_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " + _class_name);
        }

        return methods.at(method_name).call(args, this_arg);
    }

    if (_builtin_methods.find(method_name) != _builtin_methods.end()) {
        if (!is_accessible(method_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " + _class_name);
        }

        return _builtin_methods.at(method_name)(this_arg.get(), args);
    }

    if (_dynamic_methods.find(method_name) != _dynamic_methods.end()) {
        if (!is_accessible(method_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " + _class_name);
        }

        return _dynamic_methods.at(method_name)(this_arg.get(), args);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Method " + method_name + " does not exist on Object of type " + class_name());
}

/**
 * Calls a unary operator method
 * @param op
 * @param this_arg
 * @param args
 * @param class_context
 * @return
 */
Variant Class::call_unary_operator(std::string op, Object::SharedPtr this_arg, std::string
    class_context)
{
    if (unary_operators.find(op) != unary_operators.end()) {
        if (!is_accessible("unary_" + op, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private operator " + op + " of class " +
                    _class_name);
        }

        return unary_operators.at(op).call(std::vector<Variant>(), this_arg);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Unary operator " + op + " is not defined in class " + _class_name);
}

/**
 * Calls a binary operator method
 * @param op
 * @param this_arg
 * @param args
 * @param class_context
 * @return
 */
Variant Class::call_binary_operator(std::string op, Object::SharedPtr this_arg, Variant arg, std::string
    class_context)
{
    if (binary_operators.find(op) != binary_operators.end()) {
        if (!is_accessible("binary_"+op, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private operator " + op + " of class " +
                    _class_name);
        }

        return binary_operators.at(op).call(std::vector<Variant>{arg}, this_arg);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Binary operator " + op + " is not defined in class " + _class_name);
}

/**
 * Returns whether or not the class defines the given operator
 * @param op
 * @return
 */
bool Class::has_unary_operator(std::string op) {
    return unary_operators.find(op) != unary_operators.end();
}

/**
 * Returns whether or not the class defines the given operator
 * @param op
 * @return
 */
bool Class::has_binary_operator(std::string op) {
    return binary_operators.find(op) != binary_operators.end();
}

/**
 * Calls a static method
 * @param method_name
 * @param args
 * @return
 */
Variant Class::call_static_method(std::string method_name, std::vector<Variant> args, std::string class_context) {
    if (static_methods.find(method_name) != static_methods.end()) {
        if (!is_accessible(method_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " + _class_name);
        }

        return static_methods.at(method_name).call(args);
    }

    if (_builtin_static_methods.find(method_name) != _builtin_static_methods.end()) {
        if (!is_accessible(method_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " + _class_name);
        }

        return _builtin_static_methods.at(method_name)(args);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Static method " + method_name + " does not exist on " + class_name());
}

/**
 * Returns a new instance of the class with the given constructor args
 * @param args
 * @return
 */
Object::SharedPtr Class::instantiate(std::vector<Variant> args) {
    Object::SharedPtr obj = std::make_shared<Object>(this);
    for (auto prop : properties) {
        prop.second->set_type(property_types[prop.first]);
        prop.second->is_initialized(false);
        obj->_properties.emplace(prop.first, prop.second);
    }

    constructor.call(args, obj);

    if (obj->has_uninitialized_fields()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + class_name() + " has non-nullable fields that have to be defined" +
            " in the constructor");
    }

    return obj;
}

namespace cdot {
namespace lib {

    void assert_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name) {
        if (args.size() != size) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "Wrong number of arguments supplied for call to function " + name);
        }

        for (int i = 0; i < args.size(); ++i) {
            if (!val::is_compatible(args[i].get_type().type, types[i].type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "No matching call found for function " + name);
            }
        }
    }

    void assert_constr_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name) {
        if (args.size() != size) {
            RuntimeError::raise(ERR_WRONG_NUM_ARGS, "Wrong number of arguments supplied for call to constructor " + name);
        }

        for (int i = 0; i < args.size(); ++i) {
            if (!val::is_compatible(args[i].get_type().type, types[i].type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "No matching call found for constructor " + name);
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

    Variant construct(Object *this_arg, std::vector<Variant> args) {
        return {};
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