//
// Created by Jonas Zell on 25.06.17.
//

#include "Class.h"

//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Class.h"
#include "../Objects/Function.h"
#include "../../Util.h"
#include "Interface.h"
#include "../GlobalContext.h"

Class::Class(std::string name, Function constructor, AccessModifier am, bool is_abstract, bool is_struct) :
    _class_name(name),
    access_modifier(am),
    constructor(constructor),
    is_abstract(is_abstract),
    is_struct(is_struct),
    properties(std::unordered_map<std::string, Variant::SharedPtr>()),
    property_types{},
    methods(std::unordered_map<std::string, Function>()),
    static_properties(std::unordered_map<std::string, Variant::SharedPtr>()),
    static_property_types{},
    static_methods(std::unordered_map<std::string, Function>()),
    unary_operators(std::unordered_map<std::string, Function>()),
    binary_operators(std::unordered_map<std::string, Function>()),
    _access_modifiers(std::unordered_map<std::string, AccessModifier>()),
    _non_nullable_fields{},
    _builtin_methods({
        {"toString", cdot::lib::obj::toString}
    }),
    _builtin_static_methods({
        {"hashCode", cdot::lib::obj::hashCode}
    })
{

}

Class::Class(std::string name, AccessModifier am, bool is_abstract, bool is_struct) : Class(name,
        Function("void"), am, is_abstract, is_struct) {

}

void Class::extend(std::string class_name) {
    parent_class = GlobalContext::get_class(class_name);
    for (int i = 0; i < parent_class->implements.size(); ++i) {
        implements.push_back(parent_class->implements[i]);
    }
}

void Class::implement(std::string interface) {
    implements.push_back(GlobalContext::get_interface(interface));
}

void Class::finalize() {
    check_interface_conformity();
}

void Class::check_interface_conformity() {
    if (is_abstract) {
        return;
    }

    for (auto interface : implements) {
        std::string not_implemented;

        for (auto prop : interface->properties) {
            if (!has_property(prop.first)) {
                not_implemented = prop.first;
                break;
            }
            else if (!val::is_compatible(get_property(prop.first)->get_type(), prop.second)) {
                not_implemented = prop.first;
                break;
            }
        }

        for (auto prop : interface->static_properties) {
            if (!has_static_property(prop.first)) {
                not_implemented = prop.first;
                break;
            }
            else if (!val::is_compatible(get_static_property(prop.first)->get_type(), prop.second)) {
                not_implemented = prop.first;
                break;
            }
        }

        for (int i = 0; i < interface->methods.size(); ++i) {
            std::string symbol_name = util::method_to_symbol(interface->methods[i].first, interface->method_args[i],
                    false);

            if (!has_method(symbol_name)) {
                not_implemented = interface->methods[i].first;
                break;
            }

            Function& func = get_method(symbol_name);
            if (func.get_return_type() != interface->methods[i].second || func.is_abstract()) {
                not_implemented = interface->methods[i].first;
                break;
            }
        }

        for (int i = 0; i < interface->static_methods.size(); ++i) {
            std::string symbol_name = util::method_to_symbol(interface->static_methods[i].first,
                    interface->static_method_args[i], false);

            if (!has_static_method(symbol_name)) {
                not_implemented = interface->static_methods[i].first;
                break;
            }

            Function& func = get_static_method(symbol_name);
            if (func.get_return_type() != interface->static_methods[i].second || func.is_abstract()) {
                not_implemented = interface->static_methods[i].first;
                break;
            }
        }

        if (not_implemented != "") {
            RuntimeError::raise(ERR_TYPE_ERROR, "Interface " + interface->get_name() + " is not correctly "
                    "implemented, field " + not_implemented + " is not defined in class " + class_name());
        }
    }
}

/**
 * Defines a method on the class prototype that can be dynamically called with different 'this' values
 * @param func_name
 * @param func
 * @param am
 */
void Class::add_method(std::string func_name, Function func, AccessModifier am) {
    std::string symbol_name = util::method_to_symbol(func_name, func.get_arg_types(), false);

    methods.emplace(symbol_name, func);
    _access_modifiers.emplace(symbol_name, am);
}

/**
 * Returns whether or not a method is defined. 'symbol' must already be internal mangled name
 * @param symbol
 * @return
 */
bool Class::has_method(std::string symbol) {
    return methods.find(symbol) != methods.end() || (parent_class && parent_class->has_method(symbol));
}

/**
 * Returns a method of the current or parent class. DOES NOT CHECK FOR EXISTANCE OF PARENT CLASS
 * @return
 */
Function& Class::get_method(std::string symbol) {
    return methods.find(symbol) != methods.end() ? methods.at(symbol) : parent_class->get_method(symbol);
}

/**
 * Adds a property to the class prototype that is inherited by every instance
 * @param prop_name
 * @param prop
 * @param am
 */
void Class::add_property(std::string prop_name, TypeSpecifier type, Variant::SharedPtr default_val,
        AccessModifier am) {
    if (!type.nullable) {
        _non_nullable_fields.push_back(prop_name);
    }

    property_types.emplace(prop_name, type);
    properties.emplace(prop_name, default_val);
    _access_modifiers.emplace(prop_name, am);
}

/**
 * Returns whether or not the class has a certain property. Name must be mangled
 * @param symbol
 * @return
 */
bool Class::has_property(std::string symbol) {
    return properties.find(symbol) != properties.end() || (parent_class && parent_class->has_property(symbol));
}

/**
 * Returns a property
 * @return
 */
Variant::SharedPtr& Class::get_property(std::string symbol) {
    return properties.find(symbol) != properties.end() ? properties.at(symbol) : parent_class->get_property(symbol);
}

/**
 * Adds a static method to the class
 * @param func_name
 * @param func
 * @param am
 */
void Class::add_static_method(std::string func_name, Function func, AccessModifier am) {
    std::string symbol_name = util::method_to_symbol(func_name, func.get_arg_types(), true);

    static_methods.emplace(symbol_name, func);
    _access_modifiers.emplace(symbol_name, am);
}

/**
 * Returns whether or not a static method is defined. 'symbol' must already be internal mangled name
 * @param symbol
 * @return
 */
bool Class::has_static_method(std::string symbol) {
    return static_methods.find(symbol) != static_methods.end() ||
            (parent_class && parent_class->has_static_method(symbol));
}

/**
 * Returns a static method
 * @return
 */
Function& Class::get_static_method(std::string symbol) {
    return static_methods.find(symbol) != static_methods.end() ? static_methods.at(symbol) :
            parent_class->get_static_method(symbol);
}

/**
 * Adds a static property to the class
 * @param prop_name
 * @param prop
 * @param am
 */
void Class::add_static_property(std::string prop_name, TypeSpecifier type, Variant::SharedPtr prop,
        AccessModifier am) {
    static_property_types.emplace(prop_name, type);
    static_properties.emplace(prop_name, prop);
    _access_modifiers.emplace(prop_name, am);
}

/**
 * Returns whether or not the class has a certain static property. Name must be mangled
 * @param symbol
 * @return
 */
bool Class::has_static_property(std::string symbol) {
    return static_properties.find(symbol) != static_properties.end() ||
            (parent_class && parent_class->has_static_property(symbol));
}

/**
 * Returns a static property
 * @param symbol
 * @return
 */
Variant::SharedPtr& Class::get_static_property(std::string symbol) {
    return static_properties.find(symbol) != static_properties.end() ? static_properties.at(symbol) :
           parent_class->get_static_property(symbol);
}

/**
 * Adds a unary operator
 * @param op
 * @param fun
 * @param am
 */
void Class::add_unary_operator(std::string op, Function fun, AccessModifier am) {
    std::string symbol_name = util::method_to_symbol(op, fun.get_arg_types(), false);
    unary_operators.emplace(symbol_name, fun);
    _access_modifiers.emplace(symbol_name, am);
}

/**
 * Adds a binary operator
 * @param op
 * @param fun
 * @param am
 */
void Class::add_binary_operator(std::string op, Function fun, AccessModifier am) {
    std::string symbol_name = util::method_to_symbol(op, fun.get_arg_types(), false);
    binary_operators.emplace(symbol_name, fun);
    _access_modifiers.emplace(symbol_name, am);
}

/**
 * Returns a static property, if it exists
 * @param prop_name
 * @return
 */
Variant Class::access_static_property(std::string prop_name, std::string class_context) {
    if (static_properties.find(prop_name) != static_properties.end()) {
        if (!is_accessible(prop_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access private property " + prop_name + " of class " +
                    _class_name);
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
    std::vector<TypeSpecifier> arg_types;
    for (auto arg : args) {
        arg_types.push_back(arg.get_type());
    }
    std::string symbol_name = util::method_to_symbol(method_name, arg_types, false);

    if (methods.find(symbol_name) != methods.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " +
                    _class_name);
        }

        return methods.at(symbol_name).call(args, this_arg);
    }

    if (_builtin_methods.find(symbol_name) != _builtin_methods.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " +
                    _class_name);
        }

        return _builtin_methods.at(symbol_name)(this_arg.get(), args);
    }

    if (_dynamic_methods.find(symbol_name) != _dynamic_methods.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " +
                    _class_name);
        }

        return _dynamic_methods.at(symbol_name)(this_arg.get(), args);
    }

    if (parent_class) {
        return parent_class->call_method(method_name, this_arg, args, class_context == _class_name ?
             parent_class->_class_name : class_context);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Method " + method_name + " does not exist on Object of type " +
            class_name());
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
    std::string symbol_name = util::method_to_symbol(op, std::vector<TypeSpecifier>{}, false);
    if (unary_operators.find(symbol_name) != unary_operators.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private operator " + op + " of class " +
                    _class_name);
        }

        return unary_operators.at(symbol_name).call(std::vector<Variant>(), this_arg);
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
    std::string symbol_name = util::method_to_symbol(op, std::vector<TypeSpecifier>{arg.get_type()}, false);
    if (binary_operators.find(symbol_name) != binary_operators.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private operator " + op + " of class " +
                    _class_name);
        }

        return binary_operators.at(symbol_name).call(std::vector<Variant>{arg}, this_arg);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Binary operator " + op + " is not defined in class " + _class_name);
}

/**
 * Returns whether or not the class defines the given operator
 * @param op
 * @return
 */
bool Class::has_unary_operator(std::string op) {
    std::string symbol_name = util::method_to_symbol(op, std::vector<TypeSpecifier>{}, false);
    return unary_operators.find(symbol_name) != unary_operators.end();
}

/**
 * Returns whether or not the class defines the given operator
 * @param op
 * @return
 */
bool Class::has_binary_operator(std::string op, Variant arg) {
    std::string symbol_name = util::method_to_symbol(op, std::vector<TypeSpecifier>{arg.get_type()}, false);
    return binary_operators.find(symbol_name) != binary_operators.end();
}

/**
 * Calls a static method
 * @param method_name
 * @param args
 * @return
 */
Variant Class::call_static_method(std::string method_name, std::vector<Variant> args, std::string class_context) {
    std::vector<TypeSpecifier> arg_types;
    for (auto arg : args) {
        arg_types.push_back(arg.get_type());
    }
    std::string symbol_name = util::method_to_symbol(method_name, arg_types, true);

    if (static_methods.find(symbol_name) != static_methods.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " +
                    class_name());
        }

        return static_methods.at(symbol_name).call(args);
    }

    if (_builtin_static_methods.find(symbol_name) != _builtin_static_methods.end()) {
        if (!is_accessible(symbol_name, class_context)) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call private method " + method_name + " of class " +
                    class_name());
        }

        return _builtin_static_methods.at(symbol_name)(args);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Static method " + method_name + " does not exist on " + class_name());
}

/**
 * Returns a new instance of the class with the given constructor args
 * @param args
 * @return
 */
Object::SharedPtr Class::instantiate(std::vector<Variant> args) {
    if (!is_instantiable()) {
        RuntimeError::raise(ERR_TYPE_ERROR, class_name() + " is not instantiable");
    }

    Object::SharedPtr obj = std::make_shared<Object>(this);
    Class* current = this;
    while (current) {
        for (auto prop : current->properties) {
            Variant::SharedPtr v = std::make_shared<Variant>(*prop.second);
            v->set_type(property_types[prop.first]);
            v->set_default();
            if (v->is_null()) {
                v->is_initialized(false);
            }

            obj->_properties.emplace(prop.first, v);
        }

        current = current->parent_class;
    }

    constructor.call(args, obj);

    std::string uninit = obj->has_uninitialized_fields();
    if (uninit != "") {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + class_name() + " has non-nullable fields that have to be "
                "defined in the constructor (" + uninit + ")");
    }

    return obj;
}