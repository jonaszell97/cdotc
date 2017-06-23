//
// Created by Jonas Zell on 18.06.17.
//

#include "Object.h"
#include "../Util.h"

Object::Object(std::string name, AccessModifier am) :
        _class_name(name),
        _access_modifier(am)
{
    _properties = std::map<std::string, ObjectProperty::SharedPtr>();
    _methods = std::map<std::string, Function>();
}

VariantPtr Object::access_property(std::string prop_name) {
    if (_properties.find(prop_name) == _properties.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + print());
    }

    return std::make_shared<Variant>(_properties.at(prop_name));
}

void Object::reassign_property(std::string prop_name, VariantPtr value) {
    if (_properties.find(prop_name) == _properties.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Property " + prop_name + " does not exist on type " + print());
    }

    ObjectProperty::SharedPtr prop = _properties[prop_name];
    if (prop->_type != ANY_T && prop->_type != value->type) {
        RuntimeError::raise(ERR_BAD_CAST, "Incompatible types: " + util::types[prop->_type]
                                                + " and " + util::types[value->type]);
    }

    _properties.at(prop_name)->_value = value;
}

void Object::add_method(Function func) {
    _methods.insert(std::pair<std::string, Function>(func.get_name(), func));
}

void Object::add_property(ObjectProperty::SharedPtr prop) {
    _properties.insert(std::pair<std::string, ObjectProperty::SharedPtr>(prop->get_name(), prop));
}

VariantPtr Object::call_method(std::string method_name, std::vector<VariantPtr> args) {
    if (_methods.find(method_name) == _methods.end()) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Method " + method_name + " does not exist on type " + print());
    }

    return _methods.at(method_name).call(args);
}

std::string Object::print() {
    std::string _str = _class_name + " [";
    int i = 0;
    for (auto &prop : _properties) {
        _str += prop.first + " = ";
        Variant _val = prop.second;
        if (_val.type == STRING_T) {
            _str += "\"" + _val.to_string() + "\"";
        }
        else if (_val.type == CHAR_T) {
            _str += "'" + _val.to_string() + "'";
        }
        else {
            _str += _val.to_string();
        }
        if (++i < _properties.size()) {
            _str += ", ";
        }
    }

    return _str + "]";
}

ObjectProperty::ObjectProperty(std::string prop_name, VariantPtr val, ValueType type, AccessModifier am) :
        _property_name(prop_name),
        _value(val),
        _type(type),
        _access_modifier(am)
{
    if (type == AUTO_T) {
        _type = val->type;
    }
    else if(!val::is_compatible(val->type, type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[val->type]
            + " to object property of type " + util::types[type]);
    }
}

std::string ObjectProperty::get_name() {
    return _property_name;
}

ObjectProperty ObjectProperty::set_object(Object *obj) {
    _object = obj;

    return *this;
}

void ObjectProperty::set(VariantPtr val) {
    if(!val::is_compatible(val->type, _type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[val->type]
                                          + " to object property of type " + util::types[_type]);
    }

    _value = val;
}

void ObjectProperty::set(Variant v) {
    set(std::make_shared<Variant>(v));
}

VariantPtr ObjectProperty::get() {
    return _value;
}