//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "Array.h"
#include "../Util.h"

Array::Array(ValueType type, int length) :
    _type(type),
    _var_length(length == -1),
    _length(length),
    _elements(std::vector<VariantPtr>())
{

}

VariantPtr Array::at(int index) {
    if (index < 0 || (!_var_length && index > _length)|| (_var_length && index > _elements.size())) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Index " + std::to_string(index) + " does not exist on array " + print());
    }

    return _elements[index];
}

void Array::push(VariantPtr el) {
    if (!_var_length && _elements.size() == _length) {
       RuntimeError::raise(ERR_BAD_ACCESS, "Array capacity reached.");
    }
    if (!val::is_compatible(el->type, _type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to push value of type " + util::types[el->type]
                                                + " to array of type " + util::types[_type]);
    }

    _elements.push_back(el);
}

void Array::push(Variant el) {
    push(std::make_shared<Variant>(el));
}

VariantPtr Array::operator[](size_t index) {
    return at(index);
}

void Array::set(int index, Variant value) {
    if (!val::is_compatible(value.type, _type)) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to push value of type " + util::types[value.type]
                                                + " to array of type " + util::types[_type]);
    }

    _elements[index] = std::make_shared<Variant>(value);
}

VariantPtr Array::pop() {
    if (_elements.size() == 0) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot pop from an empty Array");
    }

    VariantPtr _return = _elements[_elements.size() - 1];
    _elements.pop_back();

    return _return;
}

std::string Array::print() {
    std::string str = "array";

    if (!_var_length) {
        str += "(" + std::to_string(_length) + ")";
    }
    str += " [";

    auto _el = _elements;

    std::string delimiter = "";
    for (int i = 0; i < _el.size(); ++i) {
        delimiter = _el[i]->type == STRING_T ? "\"" : (_el[i]->type == CHAR_T ? "'" : "");
        str += delimiter + _el[i]->to_string(true) + delimiter;
        if (i < _el.size() - 1) {
            str += ", ";
        }
    }

    return str + "]";
}

