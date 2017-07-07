//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <unordered_map>
#include <vector>
#include "../../Variant/Variant.h"
#include "../Objects/Function.h"

class Interface;

enum class AccessModifier {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

namespace util {
    std::string method_to_symbol(std::string, std::vector<TypeSpecifier>, bool);
}

typedef Variant (*InstanceFunction)(Object*, std::vector<Variant>);
typedef Variant (*StaticFunction)(std::vector<Variant>);

class Class {
public:
    Class(std::string, AccessModifier, bool = false, bool = false);
    Class(std::string, Function, AccessModifier, bool = false, bool = false);
    virtual ~Class() {}

    /** Instance Properties */
    void add_property(std::string, TypeSpecifier, Variant::SharedPtr = {}, AccessModifier = AccessModifier::PUBLIC);
    void add_method(std::string, Function, AccessModifier = AccessModifier::PUBLIC);

    Variant call_method(std::string, Object::SharedPtr, std::vector<Variant>, std::string);

    /** Static Properties */
    void add_static_property(std::string, TypeSpecifier, Variant::SharedPtr = {}, AccessModifier = AccessModifier::PUBLIC);
    void add_static_method(std::string, Function, AccessModifier = AccessModifier::PUBLIC);

    Variant access_static_property(std::string, std::string);
    Variant call_static_method(std::string, std::vector<Variant>, std::string);

    /** Operators */
    void add_unary_operator(std::string, Function, AccessModifier = AccessModifier::PUBLIC);
    void add_binary_operator(std::string, Function, AccessModifier = AccessModifier::PUBLIC);
    Variant call_unary_operator(std::string, Object::SharedPtr, std::string);
    Variant call_binary_operator(std::string, Object::SharedPtr, Variant, std::string);
    bool has_unary_operator(std::string);
    bool has_binary_operator(std::string, Variant arg);

    void extend(std::string);
    void implement(std::string);
    void finalize();

    inline bool is_accessible(std::string ident, std::string class_context) {
        return (_access_modifiers.find(ident) != _access_modifiers.end() &&
            _access_modifiers[ident] == AccessModifier::PUBLIC) || class_context == _class_name;
    }

    virtual std::shared_ptr<Object> instantiate(std::vector<Variant>);

    inline virtual std::string class_name() {
        return _class_name;
    }

    inline void _add_builtin(std::string name, InstanceFunction func, AccessModifier am, std::vector<TypeSpecifier>
            args) {
        std::string symbol_name = util::method_to_symbol(name, args, false);
        _builtin_methods.emplace(symbol_name, func);
        _access_modifiers.emplace(symbol_name, am);
    }

    inline void _add_static_builtin(std::string name, StaticFunction func, AccessModifier am,
            std::vector<TypeSpecifier> args) {
        std::string symbol_name = util::method_to_symbol(name, args, true);
        _builtin_static_methods.emplace(symbol_name, func);
        _access_modifiers.emplace(symbol_name, am);
    }

    inline void _add_dynamic_method(std::string name, std::function<Variant(Object*, std::vector<Variant>)> method,
            std::vector<TypeSpecifier> args) {
        std::string symbol_name = util::method_to_symbol(name, args, false);
        _dynamic_methods.emplace(symbol_name, method);
        _access_modifiers.emplace(symbol_name, AccessModifier::PUBLIC);
    }

    inline void add_non_nullable_field(std::string field_name) {
        _non_nullable_fields.push_back(field_name);
    }

    inline Variant::SharedPtr get_default_val(std::string prop_name) {
        return properties[prop_name];
    }

    inline void declare_type(std::string field, TypeSpecifier type) {
        property_types.emplace(field, type);
    }

    inline AccessModifier get_access_modifier() {
        return access_modifier;
    }

    inline virtual bool is_instantiable() {
        return !is_abstract;
    }

    typedef std::unique_ptr<Class> UniquePtr;

    friend struct Variant;
    friend class Object;

protected:
    /** Inheritance and implementation */
    void check_interface_conformity();

    bool has_static_property(std::string);
    Variant::SharedPtr& get_static_property(std::string);
    bool has_static_method(std::string);
    Function& get_static_method(std::string);

    bool has_property(std::string);
    Variant::SharedPtr& get_property(std::string);
    bool has_method(std::string);
    Function& get_method(std::string);

    /** General info */
    std::string _class_name;
    AccessModifier access_modifier;
    Function constructor;
    bool is_abstract;
    bool is_struct;

    /** Inheritance */
    Class* parent_class = nullptr;
    std::vector<Interface*> implements;

    std::unordered_map<std::string, AccessModifier> _access_modifiers;

    /** Properties and methods */
    std::unordered_map<std::string, Variant::SharedPtr> properties;
    std::unordered_map<std::string, TypeSpecifier> property_types;
    std::unordered_map<std::string, Function> methods;

    /** Static properties and methods */
    std::unordered_map<std::string, Variant::SharedPtr> static_properties;
    std::unordered_map<std::string, TypeSpecifier> static_property_types;
    std::unordered_map<std::string, Function> static_methods;
    std::vector<std::string> _non_nullable_fields;

    /** Operators */
    std::unordered_map<std::string, Function> unary_operators;
    std::unordered_map<std::string, Function> binary_operators;

    /** Builtin */
    std::unordered_map<std::string, InstanceFunction> _builtin_methods;
    std::unordered_map<std::string, std::function<Variant(Object*, std::vector<Variant>)>> _dynamic_methods;
    std::unordered_map<std::string, StaticFunction> _builtin_static_methods;
};


#endif //CDOT_CLASS_H
