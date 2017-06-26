//
// Created by Jonas Zell on 18.06.17.
//

#include <iostream>
#include "Object.h"
#include "../../Util.h"
#include "Function.h"

Object::Object() {
    class_prototype = GlobalContext::get_class("Object");
    generic_obj = true;
}

Object::Object(Class* class_prototype) :
        class_prototype(class_prototype),
        _properties(std::map<std::string, Variant::SharedPtr>())
{

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
}

Variant Object::access_property(std::string prop_name) {
    if (_properties.find(prop_name) != _properties.end()) {
        return { _properties[prop_name] };
    }

    RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on object of type "
                                        + class_prototype->class_name());
}

Variant Object::call_method(std::string method_name, std::vector<Variant> args) {
    return class_prototype->call_method(method_name, this, args);
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