//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "Array.h"
#include "../../Util.h"

Array::Array(TypeSpecifier type, int length) :
    _type(type),
    _var_length(length == -1),
    _length(length),
    _elements(std::vector<Variant::SharedPtr>())
{

}

void Array::operator=(const Object &o) {
    try {
        const Array &arr = dynamic_cast<const Array &>(o);

        if (!val::is_compatible(arr._type.type, _type.type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to assign array of type " + val::typetostr(_type)
                    + " to array of type " + val::typetostr(_type));
        }
        if (!_var_length && arr._length != _length) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to assign array of " + (arr._var_length ? "variable length" :
                    "length " + std::to_string(arr._length)) + " to array of length " + std::to_string(_length));
        }

        _elements = std::vector<Variant::SharedPtr>();
        for (auto el : arr._elements) {
            _elements.push_back(std::make_shared<Variant>(*el));
        }

    } catch(std::bad_cast& e) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type object to array");
    }
}

Variant::SharedPtr Array::at(int index) {
    if (index < 0 || (!_var_length && index >= _length) || (_var_length && index >= _elements.size())) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Index " + std::to_string(index) + " does not exist on array " + print());
    }

    return _elements[index];
}

void Array::push(Variant::SharedPtr el) {
    if (!_var_length && _elements.size() == _length) {
       RuntimeError::raise(ERR_BAD_ACCESS, "Array capacity reached");
    }
    if (!val::is_compatible(el->get_type(), _type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to push value of type " + val::typetostr(el->get_type())
                                                + " to array of type " + val::typetostr(_type));
    }

    _elements.push_back(el);
}

void Array::push(Variant v) {
    push(std::make_shared<Variant>(v));
}

Variant::SharedPtr Array::operator[](size_t index) {
    return at(index);
}

void Array::set(int index, Variant::SharedPtr value) {
    if (!val::is_compatible(value->get_type(), _type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to push value of type " + val::typetostr(value->get_type())
                                                + " to array of type " + val::typetostr(_type));
    }

    _elements[index] = value;
}

Variant::SharedPtr Array::pop() {
    if (_elements.size() == 0) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot pop from an empty Array");
    }

    Variant::SharedPtr _return = _elements[_elements.size() - 1];
    _elements.pop_back();

    return _return;
}

std::string Array::print() {
    std::string str = val::typetostr(_type) + "[" + (_var_length ? "" : std::to_string(_length)) + "]";
    str += " [";

    auto _el = _elements;

    std::string delimiter = "";
    for (int i = 0; i < _el.size(); ++i) {
        delimiter = _el[i]->get_type() == STRING_T ? "\"" : (_el[i]->get_type() == CHAR_T ? "'" : "");
        str += delimiter + _el[i]->to_string(true) + delimiter;
        if (i < _el.size() - 1) {
            str += ", ";
        }
    }

    return str + "]";
}

