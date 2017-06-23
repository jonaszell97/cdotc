//
// Created by Jonas Zell on 14.06.17.
//

#include <regex>
#include "Variant.h"
#include "Objects/Object.h"
#include "Util.h"
#include "Objects/Variable.h"
#include "Objects/Array.h"

Variant::~Variant() {
    destroy();
}

Variant::Variant() : o_val{} {
    type = VOID_T;
}

Variant::Variant(bool b) : Variant() {
    b_val = b;
    is_numeric = true;
    type = BOOL_T;
}

Variant::Variant(double d) : Variant() {
    is_numeric = true;
    d_val = d;
    type = DOUBLE_T;
}

Variant::Variant(int i) : Variant() {
    int_val = i;
    is_numeric = true;
    type = INT_T;
}

Variant::Variant(float f) : Variant() {
    float_val = f;
    is_numeric = true;
    type = FLOAT_T;
}

Variant::Variant(long l) : Variant() {
    is_numeric = true;
    if (l > INT32_MAX) {
        long_val = l;
        type = LONG_T;
    }
    else {
        int_val = int(l);
        type = INT_T;
    }
}

Variant::Variant(char c) : Variant() {
    c_val = c;
    type = CHAR_T;
}

Variant::Variant(std::string s) : Variant() {
    s_val = s;
    type = STRING_T;
}

Variant::Variant(std::shared_ptr<Object> o) : Variant() {
    o_val = o;
    type = OBJECT_T;
}

Variant::Variant(std::shared_ptr<ObjectProperty> o): Variant() {
    op_val = o;
    type = OBJECT_PROP_T;
}

Variant::Variant(std::shared_ptr<Function> fun): Variant() {
    fun_val = fun;
    type = FUNCTION_T;
}

Variant::Variant(std::shared_ptr<FuncArgument> fa): Variant() {
    fa_val = fa;
    type = FUNC_ARG_T;
}

/*Variant::Variant(std::shared_ptr<Variant> r): Variant() {
    ref = r;
    type = REF_T;
}*/

Variant::Variant(std::shared_ptr<Array> arr): Variant() {
    arr_val = arr;
    type = ARRAY_T;
}

VariantPtr Variant::from(double d) {
    return std::make_shared<Variant>(d);
}

VariantPtr Variant::from(int i) {
    return std::make_shared<Variant>(i);
}

VariantPtr Variant::from(long l) {
    return std::make_shared<Variant>(l);
}

VariantPtr Variant::from(float f) {
    return std::make_shared<Variant>(f);
}

VariantPtr Variant::from(char c) {
    return std::make_shared<Variant>(c);
}

VariantPtr Variant::from(bool b) {
    return std::make_shared<Variant>(b);
}

VariantPtr Variant::from(Variant v) {
    return std::make_shared<Variant>(v);
}

VariantPtr Variant::from(std::string s) {
    return std::make_shared<Variant>(s);
}

void Variant::destroy() {
    switch (type) {
        case OBJECT_T:
            o_val.~shared_ptr(); break;
        case OBJECT_PROP_T:
            op_val.~shared_ptr(); break;
        case FUNC_ARG_T:
            fa_val.~shared_ptr(); break;
        case ARRAY_T:
            arr_val.~shared_ptr(); break;
        /*case REF_T:
            ref.~shared_ptr(); break;*/
        case FUNCTION_T:
            fun_val.~shared_ptr(); break;
        default:
            break;
    }

    new (&o_val) std::shared_ptr<Object>();
    type = VOID_T;
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
char Variant::get<char>() {
    if (type != CHAR_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value cannot be converted to char");
    }

    return c_val;
}

template <>
std::string Variant::get<std::string>() {
    if (type != STRING_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a string");
    }

    return s_val;
}

template <>
Object::SharedPtr Variant::get<Object::SharedPtr>() {
    if (type != OBJECT_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an object");
    }

    return o_val;
}

template <>
ObjectProperty::SharedPtr Variant::get<ObjectProperty::SharedPtr>() {
    if (type != OBJECT_PROP_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an object property");
    }

    return op_val;
}

template <>
Function::SharedPtr Variant::get<Function::SharedPtr>() {
    if (type != FUNCTION_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a function");
    }

    return fun_val;
}

template <>
FuncArgument::SharedPtr Variant::get<FuncArgument::SharedPtr>() {
    if (type != FUNC_ARG_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a function argument");
    }

    return fa_val;
}

/*template <>
VariantPtr Variant::get<VariantPtr>() {
    if (type != REF_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a reference");
    }

    return ref;
}*/

template <>
Array::SharedPtr Variant::get<Array::SharedPtr>() {
    if (type != OBJECT_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an array");
    }

    return arr_val;
}

template <>
Object& Variant::get<Object&>() {
    if (type != OBJECT_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an object");
    }

    return *o_val;
}

template <>
ObjectProperty& Variant::get<ObjectProperty&>() {
    if (type != OBJECT_PROP_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an object property");
    }

    return *op_val;
}

template <>
Function& Variant::get<Function&>() {
    if (type != FUNCTION_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a function");
    }

    return *fun_val;
}

template <>
FuncArgument& Variant::get<FuncArgument&>() {
    if (type != FUNC_ARG_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a function argument");
    }

    return *fa_val;
}

/*template <>
Variant& Variant::get<Variant&>() {
    if (type != REF_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not a reference");
    }

    return *ref;
}*/

template <>
Array& Variant::get<Array&>() {
    if (type != OBJECT_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Value is not an array");
    }

    return *arr_val;
}

void Variant::check_numeric() {
    switch(type) {
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
    switch (cp.type) {
        case STRING_T:
            s_val = cp.s_val; break;
        case CHAR_T:
            c_val = cp.c_val; break;
        case BOOL_T:
            b_val = cp.b_val; break;
        case OBJECT_T:
            o_val = cp.o_val; break;
        case OBJECT_PROP_T:
            op_val = cp.op_val; break;
        case FUNC_ARG_T:
            fa_val = cp.fa_val; break;
        case ARRAY_T:
            arr_val = cp.arr_val; break;
        /*case REF_T:
            ref = cp.ref; break;*/
        case FUNCTION_T:
            fun_val = cp.fun_val; break;
        case INT_T:
            int_val = cp.int_val; break;
        case LONG_T:
            long_val = cp.long_val; break;
        case DOUBLE_T:
            d_val = cp.d_val; break;
        case FLOAT_T:
            float_val = cp.float_val; break;
        default:
            d_val = cp.d_val;
            break;
    }

    type = cp.type;
    check_numeric();
}

std::string Variant::to_string(bool escape) {
    switch (type) {
        case STRING_T: {
            return escape ? util::str_escape(s_val) : s_val;
        }
        case CHAR_T:
            return escape ? util::str_escape(std::string(1,c_val)) : std::string(1,c_val);
        case BOOL_T:
            return b_val ? "true" : "false";
        case OBJECT_T:
            return o_val->print();
        case OBJECT_PROP_T:
            return op_val->get()->to_string(escape);
        /*case REF_T:
            return ref->to_string(escape);*/
        case ARRAY_T:
            return arr_val->print();
        case FUNCTION_T:
            return fun_val->print();
        case INT_T:
            return std::to_string(int_val);
        case LONG_T:
            return std::to_string(long_val);
        case DOUBLE_T:
            return std::to_string(d_val);
        case FLOAT_T:
            return std::to_string(float_val);
        default:
            return std::to_string(d_val);
    }
}

Variant Variant::cast_to(ValueType target_type) {
    if (type == target_type || target_type == ANY_T) {
        return *this;
    }
    if (type == OBJECT_PROP_T) {
        return op_val->get()->cast_to(target_type);
    }

    switch (type) {
        case STRING_T: {
            if (target_type == CHAR_T) {
                if (s_val.length() > 1) {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + s_val + "' to char");
                }

                c_val = s_val.at(0);
            }
            else if (target_type == INT_T) {
                try {
                    int_val = std::stoi(s_val);
                } catch(std::invalid_argument) {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + s_val + "' to number");
                }
            }
            else if (target_type == LONG_T) {
                try {
                    long_val = std::stol(s_val);
                } catch(std::invalid_argument) {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + s_val + "' to number");
                }
            }
            else if (target_type == FLOAT_T) {
                try {
                    float_val = std::stof(s_val);
                } catch(std::invalid_argument) {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + s_val + "' to number");
                }
            }
            else if (target_type == DOUBLE_T) {
                try {
                    d_val = std::stod(s_val);
                } catch(std::invalid_argument) {
                    RuntimeError::raise(ERR_BAD_CAST, "Cannot cast string '" + s_val + "' to number");
                }
            }
            else if (target_type == BOOL_T) {
                b_val = s_val != "false" && s_val != "0";
            }
            else {
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                                  + " to " + util::types[target_type]);
            }

            break;
        }
        case CHAR_T: {
            if (target_type == STRING_T) {
                s_val = std::string(1, c_val);
            }
            else if (target_type == INT_T) {
                int_val = c_val;
            }
            else if (target_type == LONG_T) {
                long_val = c_val;
            }
            else if (target_type == FLOAT_T) {
                float_val = c_val;
            }
            else if (target_type == DOUBLE_T) {
                d_val = c_val;
            }
            else if (target_type == BOOL_T) {
                b_val = c_val != '0';
            }
            else {
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                                  + " to " + util::types[target_type]);
            }

            break;
        }
        case INT_T:
        case LONG_T: {
            if (target_type == STRING_T) {
                s_val = std::to_string(type == INT_T ? int_val : long_val);
            }
            else if (target_type == CHAR_T) {
                if ((type == INT_T ? int_val : long_val) > CHAR_MAX) {
                    RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value " + std::to_string(type == INT_T ? int_val : long_val)
                                                           + " is too large for type char");
                }

                c_val = char(type == INT_T ? int_val : long_val);
            }
            else if (target_type == BOOL_T) {
                b_val = (type == INT_T ? int_val : long_val) != 0;
            }
            else if (target_type == FLOAT_T) {
                float_val = (type == INT_T ? int_val : long_val);
            }
            else if (target_type == DOUBLE_T) {
                d_val = (type == INT_T ? int_val : long_val);
            }
            else {
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                                  + " to " + util::types[target_type]);
            }

            break;
        }
        case FLOAT_T:
        case DOUBLE_T:
            if (target_type == STRING_T) {
                s_val = std::to_string(type == FLOAT_T ? float_val : d_val);
            }
            else if (target_type == CHAR_T) {
                if ((type == FLOAT_T ? float_val : d_val) > CHAR_MAX) {
                    RuntimeError::raise(ERR_VAL_TOO_LARGE, "Value " + std::to_string(type == FLOAT_T ? float_val : d_val)
                                                           + " is too large for type char");
                }

                c_val = char(type == FLOAT_T ? float_val : d_val);
            }
            else if (target_type == BOOL_T) {
                b_val = (type == FLOAT_T ? float_val : d_val) != 0;
            }
            else if (target_type == FLOAT_T) {
                float_val = (type == FLOAT_T ? float_val : float(d_val));
            }
            else if (target_type == DOUBLE_T) {
                d_val = (type == FLOAT_T ? float_val : d_val);
            }
            else {
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                                  + " to " + util::types[target_type]);
            }
            break;
        case BOOL_T: {
            if (target_type == STRING_T) {
                s_val = (b_val ? "true" : "false");
            }
            else if( target_type == CHAR_T) {
                c_val = b_val ? '1' : '0';
            }
            else if (target_type == NUMBER_T) {
                d_val = b_val;
            }
            else {
                RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                                  + " to " + util::types[target_type]);
            }

            break;
        }
        default:
            RuntimeError::raise(ERR_BAD_CAST, "Cannot cast value of type " + util::types[type]
                                              + " to " + util::types[target_type]);
    }

    destroy();
    type = target_type;
    check_numeric();

    return *this;
}

Variant Variant::operator+(Variant v1) {
    if (type == OBJECT_PROP_T) {
        return op_val->get()->operator+(v1);
    }
    if (v1.type == OBJECT_PROP_T) {
        v1 = *v1.op_val->get();
    }

    if (type != v1.type) {
        if (type == STRING_T) {
            return s_val + v1.to_string();
        }
        else if (v1.type == STRING_T) {
            return to_string() + v1.s_val;
        }

        RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + util::types[type]
                                          + " and " + util::types[v1.type]);
    }

    switch(type) {
        case STRING_T:
            s_val = s_val + v1.s_val;
            break;
        case CHAR_T:
            s_val = std::string(1, c_val) + std::string(1, v1.c_val);
            break;
        case NUMBER_T:
            d_val = d_val + v1.d_val;
            break;
        case INT_T:
            int_val = int_val + v1.int_val;
            break;
        case LONG_T:
            long_val = long_val + v1.long_val;
            break;
        case DOUBLE_T:
            d_val = d_val + v1.d_val;
            break;
        case FLOAT_T:
            float_val = float_val + v1.float_val;
            break;
        default:
            RuntimeError::raise(ERR_BAD_CAST, "Cannot add values of type " + util::types[type]
                                              + " and " + util::types[v1.type]);
    }

    return *this;
};

Variant Variant::operator-(Variant v1) {
    cast_to(NUMBER_T);
    v1.cast_to(NUMBER_T);

    d_val = d_val - v1.d_val;
    return *this;
};

Variant Variant::operator-() {
    cast_to(NUMBER_T);

    d_val = -d_val;
    return *this;
}

Variant Variant::operator=(const Variant &v) {
    destroy();
    switch (v.type) {
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
        case OBJECT_T:
            o_val = v.o_val;
            break;
        case OBJECT_PROP_T:
            op_val = v.op_val;
            break;
        case FUNC_ARG_T:
            fa_val = v.fa_val;
            break;
        case FUNCTION_T:
            fun_val = v.fun_val;
            break;
        case ARRAY_T:
            if (type == ARRAY_T && !arr_val->is_var_length() && v.arr_val->get_length() > arr_val->get_length()) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign array of capacity "
                                                    + std::to_string(v.arr_val->get_length())
                + " to array of capacity " + std::to_string(arr_val->get_length()));
            }
            if (type == ARRAY_T && arr_val->get_type() != ANY_T && (v.arr_val->get_type() != arr_val->get_type())) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign array of type " + util::types[v.arr_val->get_type()]
                                                    + " to array of type " + util::types[arr_val->get_type()]);
            }

            arr_val = v.arr_val;
            break;
        default:
            break;
    }

    type = v.type;
    check_numeric();

    return *this;
}

Variant Variant::strict_equals(const Variant &v) {
    if (type == OBJECT_PROP_T) {
        return op_val->get()->operator=(v);
    }

    if (!val::is_compatible(type, v.type) && !any_type) {
        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[v.type]
                                          + " to variable of type " + util::types[type]);
    }

    return operator=(v);
}

Variant Variant::operator*(Variant v1) {
    cast_to(NUMBER_T);
    v1.cast_to(NUMBER_T);

    d_val = d_val * v1.d_val;
    return *this;
};

Variant Variant::operator/(Variant v1) {
    cast_to(NUMBER_T);
    v1.cast_to(NUMBER_T);

    d_val = d_val / v1.d_val;
    return *this;
};

Variant Variant::operator%(Variant v1) {
    cast_to(NUMBER_T);
    v1.cast_to(NUMBER_T);

    d_val = int(d_val) % int(v1.d_val);
    return *this;
};

Variant Variant::operator!() {
    cast_to(BOOL_T);

    b_val = !b_val;
    return *this;
}

Variant Variant::operator==(Variant v1) {
    if (type != v1.type) {
        return { false };
    }

    bool equal;
    switch (v1.type) {
        case STRING_T:
            equal = s_val == v1.s_val; break;
        case CHAR_T:
            equal = c_val == v1.c_val; break;
        case BOOL_T:
            equal = b_val == v1.b_val; break;
        case OBJECT_T:
            equal = o_val == v1.o_val; break;
        case OBJECT_PROP_T:
            equal = op_val == v1.op_val; break;
        case FUNC_ARG_T:
            equal = fa_val == v1.fa_val; break;
        case ARRAY_T:
            equal = arr_val == v1.arr_val; break;
        /*case REF_T:
            return ref->operator==(v1);*/
        case NUMBER_T:
        default:
            equal = d_val == v1.d_val; break;
    }

    return { equal };
}

Variant Variant::operator!=(Variant v1) {
    return !operator==(v1);
}

Variant Variant::operator<(Variant v1) {
    if (type != v1.type) {
        RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot compare values of type " + util::types[type]
                                              + " and " + util::types[v1.type]);
    }

    switch(type) {
        case CHAR_T:
            return { c_val < v1.c_val };
        case NUMBER_T:
            return { d_val < v1.d_val };
        default:
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot compare values of type " + util::types[type]
                                                  + " and " + util::types[v1.type]);
            return {};
    }
}

Variant Variant::operator&&(Variant v1) {
    if (type != BOOL_T || v1.type != BOOL_T) {
        RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply logical and to values of type " + util::types[type]
                                              + " and " + util::types[v1.type]);
        return {};
    }

    return { b_val && v1.b_val };
}

Variant Variant::operator||(Variant v1) {
    if (type != BOOL_T || v1.type != BOOL_T) {
        RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply logical or to values of type " + util::types[type]
                                              + " and " + util::types[v1.type]);
        return {};
    }

    return { b_val || v1.b_val };
}

Variant Variant::operator&(Variant v1) {
    return { bool(int(cast_to(NUMBER_T).d_val) & int(v1.cast_to(NUMBER_T).d_val)) };
}

Variant Variant::operator|(Variant v1) {
    return { bool(int(cast_to(NUMBER_T).d_val) | int(v1.cast_to(NUMBER_T).d_val)) };
}

Variant Variant::operator~() {
    return { bool(~int(cast_to(NUMBER_T).d_val)) };
}

Variant Variant::operator>(Variant v1) {
    return !operator<(v1) && !operator==(v1);
}

Variant Variant::operator<=(Variant v1) {
    return !operator>(v1);
}

Variant Variant::operator>=(Variant v1) {
    return !operator<(v1);
}

Variant Variant::operator>>(Variant v1) {
    return { double(long(cast_to(NUMBER_T).d_val) >> long(v1.cast_to(NUMBER_T).d_val)) };
}

Variant Variant::operator<<(Variant v1) {
    return { double(long(cast_to(NUMBER_T).d_val) << long(v1.cast_to(NUMBER_T).d_val)) };
}

Variant Variant::operator^(Variant v1) {
    return { long(cast_to(NUMBER_T).d_val) ^ long(v1.cast_to(NUMBER_T).d_val) };
}

namespace val {
    ValueType strtotype(std::string type) {
        return util::typemap[type];
    }

    bool is_compatible(ValueType v1, ValueType v2) {
        return (v1 == ANY_T || v2 == ANY_T || (v1 == v2));
    }
};