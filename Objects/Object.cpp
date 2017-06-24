//
// Created by Jonas Zell on 18.06.17.
//

#include "Object.h"
#include "../Util.h"
#include "Function.h"

Object::Object(std::string name, AccessModifier am) :
        _class_name(name),
        _access_modifier(am),
        _properties(std::map<std::string, Variant::SharedPtr>()),
        _methods(std::map<std::string, Function::SharedPtr>())
{

}

Variant Object::access_property(std::string prop_name) {
    if (_properties.find(prop_name) == _properties.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + print());
    }

    return { _properties[prop_name] };
}

void Object::reassign_property(std::string prop_name, Variant::SharedPtr value) {
    if (_properties.find(prop_name) == _properties.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + print());
    }
    if(!val::is_compatible(value->get_type(), _properties[prop_name]->get_type())) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[value->get_type()]
                                          + " to object property of type " + util::types[_properties[prop_name]->get_type()]);
    }

    _properties[prop_name] = value;
}

void Object::add_method(std::string func_name, Function::SharedPtr func) {
    _methods.insert(std::pair<std::string, Function::SharedPtr>(func_name, func));
}

void Object::add_property(std::string prop_name, Variant::SharedPtr prop) {
    _properties.insert(std::pair<std::string, Variant::SharedPtr>(prop_name, prop));
}

Variant Object::call_method(std::string method_name, std::vector<Variant> args) {
    if (_methods.find(method_name) == _methods.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Method " + method_name + " does not exist on type " + print());
    }

    return _methods.at(method_name)->call(args);
}

std::string Object::print() {
    std::string _str = _class_name + " [";
    int i = 0;

    for (auto& prop : _properties) {
        _str += prop.first + " = ";
        Variant::SharedPtr _val = prop.second;
        if (_val->get_type() == STRING_T) {
            _str += "\"" + _val->to_string() + "\"";
        }
        else if (_val->get_type() == CHAR_T) {
            _str += "'" + _val->to_string() + "'";
        }
        else {
            _str += _val->to_string();
        }
        if (++i < _properties.size()) {
            _str += ", ";
        }
    }

    return _str + "]";
}