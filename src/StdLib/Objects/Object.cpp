//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Object.h"
#include "../../Util.h"
#include "Function.h"
#include "../GlobalContext.h"

Object::Object(bool internal) {
    if (!internal) {
        class_prototype = GlobalContext::get_class("Object");
        generic_obj = true;
    }
}

Object::Object(Class* class_prototype) :
        class_prototype(class_prototype),
        _properties(std::unordered_map<std::string, Variant::SharedPtr>()),
        uninitialized_fields(class_prototype->_non_nullable_fields)
{

}

void Object::operator=(const Object &o) {
    if (class_prototype->class_name() != o.class_prototype->class_name()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign object of class " + o.class_prototype->class_name() + " to"
                " object of class " + class_prototype->class_name());
    }

    _properties = std::unordered_map<std::string, Variant::SharedPtr>();
    for (auto prop : o._properties) {
        _properties.emplace(prop.first, prop.second);
    }
}

void Object::set_property(std::string prop_name, Variant::SharedPtr prop) {
    if (!generic_obj) {
        if (_properties.find(prop_name) == _properties.end()) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on object of type "
                                                + class_prototype->class_name());
        }

        _properties[prop_name] = prop;
    }
    else {
        if (_properties.find(prop_name) != _properties.end()) {
            RuntimeError::raise(ERR_BAD_ACCESS, "Re-declaration of property " + prop_name);
        }

        _properties.insert(std::pair<std::string, Variant::SharedPtr>(prop_name, prop));
    }

    auto index = std::find(uninitialized_fields.begin(), uninitialized_fields.end(), prop_name);
    if (index != uninitialized_fields.end()) {
        uninitialized_fields.erase(index);
    }
}

void Object::add_property(std::string prop_name, Variant::SharedPtr value) {
    if (_properties.find(prop_name) != _properties.end()) {
        _properties[prop_name] = value;
    }
    else {
        _properties.emplace(prop_name, value);
    }
}

Variant::SharedPtr Object::access_property(std::string prop_name, std::string class_context, bool force_access) {
    if (get_properties().find(prop_name) != get_properties().end()) {
        if (get_class()->_access_modifiers[prop_name] != AccessModifier::PUBLIC
            && class_context != get_class()->class_name() && !force_access)
        {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access private property " + prop_name + " of class " +
                get_class()->class_name());
        }

        return get_properties().at(prop_name);
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on object of type "
                                        + get_class()->class_name());
}

Variant Object::call_method(std::string method_name, std::vector<Variant> args, std::string class_context, bool force) {
    return get_class()->call_method(method_name, shared_from_this(), args, class_context);
}

std::string Object::has_uninitialized_fields() {
    for (auto field : uninitialized_fields) {
        if (_properties[field]->is_null()) {
            return field;
        }
    }

    return "";
}

std::string Object::print() {
    std::string _str = class_prototype->class_name() + " {";
    int i = 0;

    for (auto& prop : _properties) {
        _str += prop.first + " = ";
        Variant::SharedPtr _val = prop.second;
        if (_val->get_type() == STRING_T) {
            _str += "\"" + _val->to_string(true) + "\"";
        }
        else if (_val->get_type() == CHAR_T) {
            _str += "'" + _val->to_string(true) + "'";
        }
        else {
            _str += _val->to_string(true);
        }
        if (++i < _properties.size()) {
            _str += ", ";
        }
    }

    return _str + "}";
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