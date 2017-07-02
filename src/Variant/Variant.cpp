//
// Created by Jonas Zell on 14.06.17.
//

#include <regex>
#include "Variant.h"
#include "../StdLib/Objects/Object.h"
#include "../Util.h"
#include "../StdLib/Objects/Array.h"
#include "../StdLib/Objects/Function.h"
#include "Conversion.h"
#include "Arithmetic.h"
#include <sstream>
#include <iostream>

Variant::~Variant() {
    destroy();
}

Variant::Variant() : o_val{}, type{} {
    type.type = VOID_T;
    _is_null = true;
}

Variant::Variant(bool b) : Variant() {
    b_val = b;
    is_numeric = true;
    type.type = BOOL_T;
    _is_null = false;
}

Variant::Variant(double d) : Variant() {
    is_numeric = true;
    d_val = d;
    type.type = DOUBLE_T;
    _is_null = false;
}

Variant::Variant(int i) : Variant() {
    int_val = i;
    is_numeric = true;
    type.type = INT_T;
    _is_null = false;
}

Variant::Variant(float f) : Variant() {
    float_val = f;
    is_numeric = true;
    type.type = FLOAT_T;
    _is_null = false;
}

Variant::Variant(long l) : Variant() {
    is_numeric = true;
    _is_null = false;
    long_val = l;
    type.type = LONG_T;
}

Variant::Variant(char c) : Variant() {
    c_val = c;
    type.type = CHAR_T;
    _is_null = false;
}

Variant::Variant(Class* c) : Variant() {
    class_val = c;
    type.type = CLASS_T;
    _is_null = false;
}

Variant::Variant(std::string s) : Variant() {
    s_val = s;
    type.type = STRING_T;
    _is_null = false;
}

Variant::Variant(std::shared_ptr<Object> o) : Variant() {
    o_val = o;
    type.type = OBJECT_T;
    _is_null = false;
}

Variant::Variant(std::shared_ptr<Variant> r): Variant() {
    ref = r;
    type.type = REF_T;
    _is_null = false;
}

void Variant::destroy() {
    switch (type.type) {
        case OBJECT_T:
            o_val.~shared_ptr(); break;
        case REF_T:
            ref.~shared_ptr(); break;
        default:
            break;
    }

    new (&o_val) std::shared_ptr<Object>();
    type.type = VOID_T;
}

TypeSpecifier Variant::get_type() const {
    if (type.type == REF_T) {
        return ref->type;
    }

    return type;
}

bool Variant::is_ref() {
    return type.type == REF_T;
}

Variant Variant::operator*() {
    if (type.type != REF_T) {
        return Variant(*this);
    }

    return Variant(*ref);
}

void Variant::is_any_type(bool any) {
    any_type = any;
}

template <>
double Variant::get<double>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->get<double>();
    }
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to double");
    }

    return d_val;
}

template <>
int Variant::get<int>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->ref->get<int>();
    }
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to int");
    }

    return int_val;
}

template <>
long Variant::get<long>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->ref->get<long>();
    }
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to long");
    }

    return long_val;
}

template <>
float Variant::get<float>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->ref->get<float>();
    }
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to float");
    }

    return float_val;
}

template <>
bool Variant::get<bool>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->ref->get<bool>();
    }
    if (type.type != BOOL_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not boolean");
    }

    return b_val;
}

template <>
char Variant::get<char>() {
    if (type.type == REF_T) {
        if (ref->type.type != REF_T) return ref->ref->get<char>();
    }
    if (type.type != CHAR_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to char");
    }

    return c_val;
}

template <>
std::string Variant::get<std::string>() {
    if (type.type != STRING_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a string");
    }

    return s_val;
}

template <>
Object::SharedPtr Variant::get<Object::SharedPtr>() {
    if (type.type == REF_T) {
        return ref->get<Object::SharedPtr>();
    }
    if (type.type != OBJECT_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an object");
    }

    return o_val;
}

template <>
Variant::SharedPtr Variant::get<Variant::SharedPtr>() {
    if (type.type != REF_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a reference");
    }

    return ref;
}

template <>
Class* Variant::get<Class*>() {
    if (type.type != CLASS_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a class");
    }

    return class_val;
}

void Variant::check_numeric() {
    switch(type.type) {
        case INT_T:
        case FLOAT_T:
        case DOUBLE_T:
        case LONG_T:
        case BOOL_T:
        case CHAR_T:
            is_numeric = true;
            break;
        default:
            is_numeric = false;
            break;
    }
}

Variant::Variant(const Variant &cp) {
    new (&o_val) std::shared_ptr<Object>();
    switch (cp.type.type) {
        case STRING_T:
            s_val = cp.s_val; break;
        case CHAR_T:
            c_val = cp.c_val; break;
        case BOOL_T:
            b_val = cp.b_val; break;
        case OBJECT_T:
            o_val = cp.o_val; break;
        case REF_T:
            ref = cp.ref; break;
        case INT_T:
            int_val = cp.int_val; break;
        case LONG_T:
            long_val = cp.long_val; break;
        case DOUBLE_T:
            d_val = cp.d_val; break;
        case FLOAT_T:
            float_val = cp.float_val; break;
        case CLASS_T:
            class_val = cp.class_val; break;
        case VOID_T:
            break;
        default:
            d_val = cp.d_val;
            break;
    }

    _is_null = cp._is_null;
    type.nullable = cp.type.nullable;
    type.type = cp.type.type;
    check_numeric();
}

std::string Variant::to_string(bool escape) {
    if (_is_null) {
        return "null";
    }

    switch (type.type) {
        case STRING_T: {
            return escape ? util::str_escape(s_val) : s_val;
        }
        case CHAR_T:
            return escape ? util::str_escape(std::string(1,c_val)) : std::string(1,c_val);
        case BOOL_T:
            return b_val ? "true" : "false";
        case OBJECT_T:
            return o_val->print();
        case REF_T: {
            std::ostringstream ss;
            ss << &(*ref);

            return ss.str();
        }
        case INT_T:
            return std::to_string(int_val);
        case LONG_T:
            return std::to_string(long_val);
        case DOUBLE_T:
            return std::to_string(d_val);
        case FLOAT_T:
            return std::to_string(float_val);
        case CLASS_T:
            return class_val->class_name();
        case VOID_T:
            return "null";
        default:
            return std::to_string(d_val);
    }
}

long Variant::hash() {
    switch (type.type) {
        case STRING_T: {
            return { long(std::hash<std::string>()(s_val)) };
        }
        case CHAR_T:
            return { long(std::hash<char>()(c_val)) };
        case BOOL_T:
            return { long(std::hash<bool>()(b_val)) };
        case INT_T:
            return { long(std::hash<int>()(int_val)) };
        case LONG_T:
            return { long(std::hash<long>()(long_val)) };
        case DOUBLE_T:
            return { long(std::hash<double>()(d_val)) };
        case FLOAT_T:
            return { long(std::hash<float>()(float_val)) };
        default:
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot hash value of type " + val::type_name(*this));
    }
}

Variant Variant::cast_to(ValueType target_type) {
    return cdot::var::Converter::cast(*this, target_type);
}

Variant Variant::operator+(Variant v1) {
    return cdot::var::Arithmetic::add(*this, v1);
};

Variant Variant::pow(Variant v1) {
    return cdot::var::Arithmetic::pow(*this, v1);
}

Variant Variant::operator-(Variant v1) {
    return cdot::var::Arithmetic::minus(*this, v1);
};

Variant Variant::operator-() {
    return cdot::var::Arithmetic::unary_minus(*this);
}

Variant Variant::operator=(const Variant &v) {

    switch (v.type.type) {
        case INT_T:
            int_val = v.int_val; break;
        case LONG_T:
            long_val = v.long_val; break;
        case DOUBLE_T:
            d_val = v.d_val; break;
        case FLOAT_T:
            float_val = v.float_val; break;
        case STRING_T:
            s_val = v.s_val;
            break;
        case BOOL_T:
            b_val = v.b_val;
        case CHAR_T:
            c_val = v.c_val;
            break;
        case REF_T:
            ref = v.ref;
            break;
        case OBJECT_T:
            if (o_val == nullptr) {
                o_val = v.o_val;
            }
            else {
                o_val->operator=(*v.o_val);
            }

            break;
        case CLASS_T:
            class_val = v.class_val;
        case VOID_T:
            _is_null = true;
            type.type = VOID_T;
            return *this;
        default:
            break;
    }

    initialized = true;
    _is_null = v._is_null;
    type.nullable = v.type.nullable;
    type.type = v.type.type;
    check_numeric();

    return *this;
}

Variant Variant::strict_equals(const Variant &v) {
    if (get_type().type != v.get_type().type && !any_type  && initialized && !(type.nullable && v.is_null())) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + val::typetostr(v.get_type())
             + " to variable of type " + val::typetostr(get_type()));
    }

    return operator=(v);
}

Variant Variant::operator*(Variant v1) {
    return cdot::var::Arithmetic::multiply(*this, v1);
};

Variant Variant::operator/(Variant v1) {
    return cdot::var::Arithmetic::divide(*this, v1);
};

Variant Variant::operator%(Variant v1) {
    return cdot::var::Arithmetic::modulo(*this, v1);
};

Variant Variant::operator!() {
     return cdot::var::Arithmetic::unary_not(*this);
}

Variant Variant::operator==(Variant v1) {
    return cdot::var::Arithmetic::equal(*this, v1);
}

Variant Variant::operator!=(Variant v1) {
    return cdot::var::Arithmetic::not_equal(*this, v1);
}

Variant Variant::operator&&(Variant v1) {
    return cdot::var::Arithmetic::logical_and(*this, v1);
}

Variant Variant::operator||(Variant v1) {
    return cdot::var::Arithmetic::logical_or(*this, v1);
}

Variant Variant::operator&(Variant v1) {
    return cdot::var::Arithmetic::bitwise_and(*this, v1);
}

Variant Variant::operator|(Variant v1) {
    return cdot::var::Arithmetic::bitwise_or(*this, v1);
}

Variant Variant::operator~() {
    return cdot::var::Arithmetic::bitwise_negate(*this);
}

Variant Variant::operator<(Variant v1) {
    return cdot::var::Arithmetic::lower(*this, v1);
}

Variant Variant::operator>(Variant v1) {
    return cdot::var::Arithmetic::greater(*this, v1);
}

Variant Variant::operator<=(Variant v1) {
    return cdot::var::Arithmetic::lower_equal(*this, v1);
}

Variant Variant::operator>=(Variant v1) {
    return cdot::var::Arithmetic::greater_equal(*this, v1);
}

Variant Variant::operator>>(Variant v1) {
    return cdot::var::Arithmetic::bitshift_right(*this, v1);
}

Variant Variant::operator<<(Variant v1) {
    return cdot::var::Arithmetic::bitshift_left(*this, v1);
}

Variant Variant::operator^(Variant v1) {
    return cdot::var::Arithmetic::bitwise_xor(*this, v1);
}

namespace val {
    ValueType strtotype(std::string type) {
        return util::typemap[type];
    }

    std::string typetostr(ValueType type) {
        if (type == AUTO_T) {
            return "auto";
        }

        return util::types[type];
    }

    std::string typetostr(TypeSpecifier type) {
        return typetostr(type.type);
    }

    std::string base_class(ValueType v) {
        if (util::classmap.find(v) != util::classmap.end()) {
            return util::classmap[v];
        }

        return "";
    }

    std::string type_name(Variant v) {
        if (v.get_type().type == OBJECT_T) {
            return v.get<Object::SharedPtr>()->get_class()->class_name();
        }

        return typetostr(v.get_type().type);
    }

    bool is_compatible(ValueType v1, ValueType v2) {
        return (v1 == ANY_T || v2 == ANY_T || (v1 == v2)) || util::in_vector<ValueType>(util::type_conversions[v1], v2);
    }

    bool is_compatible(TypeSpecifier t1, TypeSpecifier t2) {
        return is_compatible(t1.type, t2.type) || (t1.nullable && t2.type == VOID_T) || (t2.nullable && t1.type ==
                VOID_T);
    }
};