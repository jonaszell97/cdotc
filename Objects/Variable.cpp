//
// Created by Jonas Zell on 19.06.17.
//

#include "Variable.h"
#include "../Util.h"

Variable::Variable(Variant val, bool any, std::string name) :
    _value(val),
    _name(name)
{
    _type = any ? ANY_T : val.type;
}

Variable::Variable(const Variable &v) : Variable(v._value, v._type == ANY_T, v._name) {

}

Variant Variable::get() {
    return _value;
}

void Variable::reassign(Variant val) {
    if (_type != ANY_T && _type != val.type) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[val.type]
                                                 + " to object property of type " + util::types[_type]);
    }

    _value = val;
}

void Variable::is_any() {
    _type = ANY_T;
}