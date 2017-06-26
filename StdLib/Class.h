//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <unordered_map>
#include <vector>
#include "../Variant/Variant.h"
#include "Objects/Function.h"
#include "Method.h"

enum class AccessModifier {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

typedef Variant (*InstanceFunction)(Object*, std::vector<Variant>);
typedef Variant (*StaticFunction)(std::vector<Variant>);

namespace cdot {
namespace lib {

    void assert_args(int size, std::vector<ValueType> types, std::vector<Variant> args, std::string name);
    void assert_constr_args(int size, std::vector<ValueType> types, std::vector<Variant> args, std::string name);

namespace obj {

    /******************************************/
    /*                                        */
    /*           INSTANCE METHODS             */
    /*                                        */
    /******************************************/

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
    Class(std::string, Method, AccessModifier);
    Class(const Class&);

    /** Instance Properties */
    void add_property(std::string, Variant::SharedPtr = {}, AccessModifier = AccessModifier::PUBLIC);
    void add_method(std::string, std::shared_ptr<Function>, AccessModifier = AccessModifier::PUBLIC);
    Variant call_method(std::string, Object*, std::vector<Variant>);

    /** Static Properties */
    void add_static_property(std::string, Variant::SharedPtr = {}, AccessModifier = AccessModifier::PUBLIC);
    void add_static_method(std::string, std::shared_ptr<Function>, AccessModifier = AccessModifier::PUBLIC);

    Variant access_static_property(std::string);
    Variant call_static_method(std::string, std::vector<Variant>);

    inline bool is_accessible(std::string ident) {
        return _access_modifiers.find(ident) != _access_modifiers.end() &&
            _access_modifiers[ident] == AccessModifier::PUBLIC;
    }

    std::shared_ptr<Object> instantiate(std::vector<Variant>);

    inline virtual std::string class_name() {
        return _class_name;
    }

    inline void _add_builtin(std::string name, InstanceFunction func) {
        _builtin_methods.emplace(name, func);
    }

    inline void _add_static_builtin(std::string name, StaticFunction func) {
        _builtin_static_methods.emplace(name, func);
    }

    typedef std::unique_ptr<Class> UniquePtr;

    friend struct Variant;

protected:
    std::string _class_name;
    AccessModifier access_modifier;
    Method constructor;

    std::map<std::string, AccessModifier> _access_modifiers;

    std::vector<std::string> properties;
    std::map<std::string, std::shared_ptr<Function>> methods;

    std::map<std::string, Variant::SharedPtr> static_properties;
    std::map<std::string, std::shared_ptr<Function>> static_methods;

    std::map<std::string, InstanceFunction> _builtin_methods;
    std::map<std::string, StaticFunction> _builtin_static_methods;
};


#endif //CDOT_CLASS_H
