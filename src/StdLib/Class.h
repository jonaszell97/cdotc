//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <unordered_map>
#include <vector>
#include "../Variant/Variant.h"
#include "Objects/Function.h"

enum class AccessModifier {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

typedef Variant (*InstanceFunction)(Object*, std::vector<Variant>);
typedef Variant (*StaticFunction)(std::vector<Variant>);

namespace cdot {
namespace lib {

    void assert_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name);
    void assert_constr_args(int size, std::vector<TypeSpecifier> types, std::vector<Variant> args, std::string name);

namespace obj {

    /******************************************/
    /*                                        */
    /*           INSTANCE METHODS             */
    /*                                        */
    /******************************************/

    Variant construct(Object *this_arg, std::vector<Variant> args);
    Variant toString(Object *this_arg, std::vector<Variant> args);

    /******************************************/
    /*                                        */
    /*             STATIC METHODS             */
    /*                                        */
    /******************************************/

    Variant hashCode(std::vector<Variant> args);

} // namespace obj
} // namespace lib
} // namespace cdot

class Class {
public:
    Class(std::string, Function, AccessModifier);

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
    bool has_binary_operator(std::string);

    inline bool is_accessible(std::string ident, std::string class_context) {
        return (_access_modifiers.find(ident) != _access_modifiers.end() &&
            _access_modifiers[ident] == AccessModifier::PUBLIC) || class_context == _class_name;
    }

    std::shared_ptr<Object> instantiate(std::vector<Variant>);

    inline virtual std::string class_name() {
        return _class_name;
    }

    inline void _add_builtin(std::string name, InstanceFunction func, AccessModifier am = AccessModifier::PUBLIC) {
        _builtin_methods.emplace(name, func);
        _access_modifiers.emplace(name, am);
    }

    inline void _add_static_builtin(std::string name, StaticFunction func, AccessModifier am = AccessModifier::PUBLIC) {
        _builtin_static_methods.emplace(name, func);
        _access_modifiers.emplace(name, am);
    }

    inline void _add_dynamic_method(std::string name, std::function<Variant(Object*, std::vector<Variant>)> method) {
        _dynamic_methods.emplace(name, method);
        _access_modifiers.emplace(name, AccessModifier::PUBLIC);
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

    typedef std::unique_ptr<Class> UniquePtr;

    friend struct Variant;
    friend class Object;

protected:
    std::string _class_name;
    AccessModifier access_modifier;
    Function constructor;

    std::map<std::string, AccessModifier> _access_modifiers;

    std::map<std::string, Variant::SharedPtr> properties;
    std::map<std::string, TypeSpecifier> property_types;
    std::map<std::string, Function> methods;

    std::map<std::string, Variant::SharedPtr> static_properties;
    std::map<std::string, TypeSpecifier> static_property_types;
    std::map<std::string, Function> static_methods;
    std::vector<std::string> _non_nullable_fields;

    std::unordered_map<std::string, Function> unary_operators;
    std::unordered_map<std::string, Function> binary_operators;

    std::map<std::string, InstanceFunction> _builtin_methods;
    std::map<std::string, std::function<Variant(Object*, std::vector<Variant>)>> _dynamic_methods;
    std::map<std::string, StaticFunction> _builtin_static_methods;
};


#endif //CDOT_CLASS_H
