//
// Created by Jonas Zell on 14.06.17.
//

#include <regex>
#include "Variant.h"
#include "../Util.h"
#include "Conversion.h"
#include "Arithmetic.h"
#include <sstream>
#include <iostream>
#include "../AST/Visitor/StaticAnalysis/Class.h"
#include "../AST/Namespace.h"

Variant::Variant() : type{} {
    type.type = VOID_T;
    _is_null = true;
}

Variant::Variant(bool b) : Variant() {
    b_val = b;
    is_numeric = true;
    type.class_name = "Bool";
    type.type = BOOL_T;
    type.static_const = true;
    _is_null = false;
}

Variant::Variant(double d) : Variant() {
    is_numeric = true;
    d_val = d;
    type.class_name = "Double";
    type.type = DOUBLE_T;
    type.static_const = true;
    _is_null = false;
}

Variant::Variant(int i) : Variant() {
    int_val = i;
    is_numeric = true;
    type.class_name = "Int";
    type.static_const = true;
    type.type = INT_T;
    _is_null = false;
}

Variant::Variant(float f) : Variant() {
    float_val = f;
    is_numeric = true;
    type.class_name = "Float";
    type.type = FLOAT_T;
    type.static_const = true;
    _is_null = false;
}

Variant::Variant(long l) : Variant() {
    is_numeric = true;
    _is_null = false;
    long_val = l;
    type.static_const = true;
    type.type = LONG_T;
}

Variant::Variant(char c) : Variant() {
    c_val = c;
    type.type = CHAR_T;
    type.class_name = "Char";
    type.static_const = true;
    _is_null = false;
}

Variant::Variant(std::string s) : Variant() {
    s_val = s;
    type.type = OBJECT_T;
    type.class_name = "String";
    _is_null = false;
}

Variant::Variant(TypeSpecifier ts) {
    type = ts;
    _is_null = true;
}

TypeSpecifier& Variant::get_type() {
    return type;
}

void Variant::set_default() {
    switch (type.type) {
        case INT_T:
            int_val = 0;
            break;
        case LONG_T:
            long_val = 0l;
            break;
        case FLOAT_T:
            float_val = 0.0f;
            break;
        case DOUBLE_T:
            d_val = 0.0;
            break;
        case BOOL_T:
            b_val = false;
            break;
        case CHAR_T:
            c_val = '\0';
            break;
        case STRING_T:
            s_val = "";
            break;
        default:
            return;
    }

    _is_null = false;
}

void Variant::is_any_type(bool any) {
    any_type = any;
}

template <>
double Variant::get<double>() {
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to double");
    }

    return d_val;
}

template <>
int Variant::get<int>() {
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to int");
    }

    return int_val;
}

template <>
long Variant::get<long>() {
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to long");
    }

    return long_val;
}

template <>
float Variant::get<float>() {
    if (!is_numeric) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to float");
    }

    return float_val;
}

template <>
bool Variant::get<bool>() {
    if (type.type != BOOL_T) {
        return cast_to(BOOL_T).get<bool>();
    }

    return b_val;
}

template <>
char Variant::get<char>() {
    if (type.type != CHAR_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to char");
    }

    return c_val;
}

template <>
std::string Variant::get<std::string>() {
    if (type.class_name != "String") {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a string");
    }

    return s_val;
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
    switch (cp.type.type) {
        case STRING_T:
            type.computed_length = cp.type.computed_length;
            s_val = cp.s_val; break;
        case CHAR_T:
            c_val = cp.c_val; break;
        case BOOL_T:
            b_val = cp.b_val; break;
        case OBJECT_T:
            type.class_name = cp.type.class_name;
        case INT_T:
            int_val = cp.int_val; break;
        case LONG_T:
            long_val = cp.long_val; break;
        case DOUBLE_T:
            d_val = cp.d_val; break;
        case FLOAT_T:
            float_val = cp.float_val; break;
        case VOID_T:
            break;
        default:
            d_val = cp.d_val;
            break;
    }

    if (cp.type.class_name == "String") {
        s_val = cp.s_val;
    }

    _is_null = cp._is_null;
    type = cp.type;
    check_numeric();
}

std::string Variant::to_string(bool escape) {
    if (_is_null) {
        return "null";
    }

    if (type.class_name == "String") {
        return escape ? util::str_escape(s_val) : s_val;
    }

    switch (type.type) {
        case CHAR_T:
            return escape ? util::str_escape(std::string(1,c_val)) : std::string(1,c_val);
        case BOOL_T:
            return b_val ? "true" : "false";
        case INT_T:
            return std::to_string(int_val);
        case LONG_T:
            return std::to_string(long_val);
        case DOUBLE_T:
            return std::to_string(d_val);
        case FLOAT_T:
            return std::to_string(float_val);
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

Variant& Variant::operator=(const Variant &v) {

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
            type.computed_length = v.type.computed_length;
            s_val = v.s_val;
            break;
        case BOOL_T:
            b_val = v.b_val;
        case CHAR_T:
            c_val = v.c_val;
            break;
        case VOID_T:
            _is_null = true;
            type.type = VOID_T;
            return *this;
        default:
            break;
    }

    if (v.type.class_name == "String") {
        s_val = v.s_val;
    }

    initialized = true;
    _is_null = v._is_null;
    type = v.type;
    check_numeric();

    return *this;
}

Variant Variant::strict_equals(const Variant &v) {
    if (get_type().type != v.type.type && !any_type  && initialized && !(type.nullable && v.is_null())) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + val::typetostr(v.type)
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

TypeSpecifier& TypeSpecifier::resolve(Namespace* ns) {
    if (resolved) {
        return *this;
    }

    ns->resolve_typedef(*this);
    if (return_type != nullptr) {
        ns->resolve_typedef(*return_type);
        invalid_ns_ref = return_type->invalid_ns_ref;
    }

    resolved = true;
    return *this;
}

void TypeSpecifier::resolveGeneric(TypeSpecifier &ts, unordered_map<string, TypeSpecifier> &concrete_generics) {
    if (concrete_generics.empty() || !ts.is_generic) {
        return;
    }

    ts.concrete_generic_types = concrete_generics;

    if (!ts.generic_class_name.empty()) {
        ts.operator=(concrete_generics[ts.generic_class_name]);
    }

    if (ts.element_type != nullptr) {
        resolveGeneric(*ts.element_type, concrete_generics);
    }

    for (auto& arg : ts.args) {
        resolveGeneric(arg, concrete_generics);
    }
}

std::string TypeSpecifier::to_string() const {
    std::string _nullable = nullable ? "?" : "";
    if (raw_array) {
        return "["  + element_type->to_string() + "]";
    }
    if (type == OBJECT_T && !is_function) {
        return class_name + _nullable;
    }
    if (type == CLASS_T) return class_name;
    if (type == INTERFACE_T) return class_name;

    if (is_function) {
        std::string str = "(";
        for (int i = 0; i < args.size(); ++i) {
            str += args.at(i).to_string();
            if (i < args.size() - 1) {
                str += ", ";
            }
        }

        return str + ") -> " + return_type->to_string();
    }

    return val::typetostr(type) + _nullable;
}

//TypeSpecifier& TypeSpecifier::operator=(const TypeSpecifier &other) {
//    type = other.type;
//    class_name = other.class_name;
//    arr_length = other.arr_length;
//    invalid_ns_ref = other.invalid_ns_ref;
//    raw_array = other.raw_array;
//    cstring = other.cstring;
//    is_primitive = other.is_primitive;
//    computed_length = other.computed_length;
//    bitwidth = other.bitwidth;
//    is_unsigned = other.is_unsigned;
//    is_generic = other.is_generic;
//    generic_class_name = other.generic_class_name;
//    is_vararg = other.is_vararg;
//    concrete_generic_types.clear();
//    for (const auto& gen : other.concrete_generic_types) {
//        concrete_generic_types.emplace(gen.first, gen.second);
//    }
//    nullable = other.nullable;
//    static_const = other.static_const;
//    is_const = other.is_const;
//    is_function = other.is_function;
//    is_pointer = other.is_pointer;
//    is_lambda = other.is_lambda;
//    resolved = other.resolved;
//    lambda_id = other.lambda_id;
//    ns_name = other.ns_name;
//    args = other.args;
//    element_type = other.element_type;
//}

bool TypeSpecifier::operator==(TypeSpecifier& ts) {
    if (type == INT_T && ts.type == INT_T) {
        return bitwidth == ts.bitwidth && is_unsigned == ts.is_unsigned;
    }

    if (type == ts.type && type != OBJECT_T) {
        return true;
    }

    if (type != ts.type) {
        return false;
    }

    if (raw_array != ts.raw_array) {
        return false;
    }

    if (raw_array) {
        return *element_type == *ts.element_type;
    }

    if (cstring != ts.cstring) {
        return false;
    }

    return class_name == ts.class_name;
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
        return type.to_string();
    }

    std::string base_class(ValueType v) {
        if (util::classmap.find(v) != util::classmap.end()) {
            return util::classmap[v];
        }

        return "";
    }

    std::string type_name(Variant v) {
        return typetostr(v.get_type().type);
    }

    bool is_compatible(ValueType v1, ValueType v2) {
        return (v1 == ANY_T || v2 == ANY_T || (v1 == v2)) || util::in_vector<ValueType>(util::implicit_type_conversions[v1], v2);
    }

    bool is_compatible(TypeSpecifier& given, TypeSpecifier& needed) {

        // nullable
        if ((given.nullable && needed.type == VOID_T) || needed.nullable && given.type == VOID_T) {
            return true;
        }
        if (given.type == VOID_T && needed.type == VOID_T) {
            return true;
        }
        if (given.type == VOID_T && needed.type != VOID_T) {
            return false;
        }

        // function type
        if (given.is_function || needed.is_function) {
            return given.to_string() == needed.to_string();
        }

        // raw arrays
        if (given.raw_array != needed.raw_array) {
            return false;
        }
        if (given.raw_array && needed.raw_array) {
            return val::is_compatible(*given.element_type, *needed.element_type);
        }

        // class implements interface
        if (needed.type == INTERFACE_T) {
            if (given.type == INTERFACE_T) {
                return needed.class_name == given.class_name;
            }

            return Namespace::latest()->get_class(given.class_name)->implements(needed.class_name);
        }

        // autoboxing
        if (given.type != OBJECT_T && needed.type == OBJECT_T) {
            return Namespace::global()->get_class(needed.class_name)->is_base_class_of(util::types[given.type]);
        }

        // two primitive types
        if (given.type != OBJECT_T && needed.type == OBJECT_T) {
            return given.type == needed.type;
        }

        // base class
        if (given.type == OBJECT_T && needed.type == OBJECT_T) {
            if (given.class_name == needed.class_name) {
                return true;
            }

            return Namespace::latest()->get_class(needed.class_name)->is_base_class_of(given.class_name);
        }

        return given.type == needed.type || implicitly_castable(given.type, needed.type);
    }

    bool is_castable(TypeSpecifier& t1, TypeSpecifier& t2) {
        if (t1 == t2) {
            return true;
        }
        if (t1.type == OBJECT_T && t2.type == OBJECT_T) {
            if (t1.class_name == t2.class_name) {
                return true;
            }

            return Namespace::latest()->get_class(t1.class_name)->is_base_class_of(t2.class_name);
        }
        if (t1.is_function || t2.is_function) {
            return false;
        }

        auto conv = util::explicit_type_conversions[t1.type];

        return std::find(conv.begin(), conv.end(), t2.type) != conv.end();
    }

    bool implicitly_castable(ValueType v1, ValueType v2) {
        if (v1 == OBJECT_T || v1 == CLASS_T || v1 == INTERFACE_T || v1 == VOID_T) {
            return false;
        }
        if (v1 == v2) {
            return true;
        }

        auto conv = util::implicit_type_conversions[v1];
        return std::find(conv.begin(), conv.end(), v2) != conv.end();
    }

    bool has_default(TypeSpecifier t) {
        if (t.is_primitive) {
            return true;
        }

        if (t.raw_array) {
            return true;
        }

        if (t.cstring) {
            return true;
        }

        if (t.type == OBJECT_T) {
            return Namespace::latest()->get_class(t.class_name)->has_method("init", {}) != "";
        }

        return false;
    }

    TypeSpecifier simple_arithmetic_return_type(TypeSpecifier& t1, TypeSpecifier& t2) {
        if (t1.class_name == "String" || t2.class_name == "String") {
            TypeSpecifier ts(OBJECT_T);
            ts.class_name = "String";

            return ts;
        }

        if (t1.raw_array && t2.raw_array) {
            if (!val::is_compatible(*t1.element_type, *t2.element_type)) {
                return VOID_T;
            }

            return t1;
        }

        if (!t1.is_primitive || !t2.is_primitive) {
            return VOID_T;
        }

        if (t1.type == INT_T && t2.type == INT_T) {
            int bitwidth = t1.bitwidth >= t2.bitwidth ? t1.bitwidth : t2.bitwidth;
            bool is_unsigned = t1.is_unsigned && t2.is_unsigned;
            TypeSpecifier ret(INT_T);
            ret.bitwidth = bitwidth;
            ret.is_unsigned = is_unsigned;

            return ret;
        }

        if (t1.type == t2.type) {
            return t1.type;
        }

        switch (t1.type) {
            case INT_T:
                switch (t2.type) {
                    case DOUBLE_T:
                        return DOUBLE_T;
                    case FLOAT_T:
                        return FLOAT_T;
                    case BOOL_T:
                    case CHAR_T:
                        return INT_T;
                    default:
                        return VOID_T;
                }
                break;
            case LONG_T:
                switch (t2.type) {
                    case INT_T:
                    case BOOL_T:
                    case CHAR_T:
                        return LONG_T;
                    case DOUBLE_T:
                        return DOUBLE_T;
                    case FLOAT_T:
                        return FLOAT_T;
                    default:
                        return VOID_T;
                }
                break;
            case FLOAT_T:
                switch (t2.type) {
                    case INT_T:
                    case LONG_T:
                    case BOOL_T:
                    case CHAR_T:
                        return FLOAT_T;
                    case DOUBLE_T:
                        return DOUBLE_T;
                    default:
                        return VOID_T;
                }
                break;
            case DOUBLE_T:
                return DOUBLE_T;
            case CHAR_T:
            case BOOL_T:
                return simple_arithmetic_return_type(t2, t1);
            default:
                return VOID_T;
        }
    }

    ValueType division_return_type(TypeSpecifier& t1, TypeSpecifier& t2) {
        if (!t1.is_primitive || !t2.is_primitive) {
            return VOID_T;
        }

        if (t1.type == DOUBLE_T || t2.type == DOUBLE_T) {
            return DOUBLE_T;
        }

        return FLOAT_T;
    }
};