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
        _properties(std::map<std::string, Variant::SharedPtr>()),
        uninitialized_fields(class_prototype->_non_nullable_fields)
{

}

void Object::operator=(const Object &o) {
    if (class_prototype->class_name() != o.class_prototype->class_name()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign object of class " + o.class_prototype->class_name() + " to"
                " object of class " + class_prototype->class_name());
    }

    _properties = std::map<std::string, Variant::SharedPtr>();
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
    if (_properties.find(prop_name) != _properties.end()) {
        if (class_prototype->_access_modifiers[prop_name] != AccessModifier::PUBLIC
            && class_context != class_prototype->class_name() && !force_access)
        {
            RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access private property " + prop_name + " of class " +
                class_prototype->class_name());
        }

        return _properties[prop_name];
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on object of type "
                                        + class_prototype->class_name());
}

Variant Object::call_method(std::string method_name, std::vector<Variant> args, std::string class_context, bool force) {
    return class_prototype->call_method(method_name, shared_from_this(), args, class_context);
}

bool Object::has_uninitialized_fields() {
    for (auto field : uninitialized_fields) {
        if (_properties[field]->get_type() == VOID_T) {
            return true;
        }
    }

    return false;
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