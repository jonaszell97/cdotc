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
#include "../AST/SymbolTable.h"
#include "Type/IntegerType.h"
#include "Type/FPType.h"
#include "Type/ObjectType.h"
#include "Type/VoidType.h"

Variant::Variant() : _type{} {
    _type.type = VOID_T;
    type = new VoidType;
}

Variant::Variant(double d) : Variant() {
    float_val = d;
    type = FPType::getDoubleTy();
}

Variant::Variant(int i) : Variant() {
    int_val = i;
    type = IntegerType::get(sizeof(int*) * 8);
}

Variant::Variant(float f) : Variant() {
    float_val = f;
    type = FPType::getFloatTy();
}

Variant::Variant(long l) : Variant() {
    int_val = l;
    type = IntegerType::get(64);
}

Variant::Variant(char c) : Variant() {
    int_val = c;
    type = IntegerType::get(8);
}

Variant::Variant(bool b) : Variant() {
    int_val = b;
    type = IntegerType::get(1);
}

Variant::Variant(std::string s) : Variant() {
    s_val = s;
    type = ObjectType::get("String");
}

void Variant::isUnsigned(bool uns, Lexer* lexer) {
    if (!isa<IntegerType>(type)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "'u' suffix is only valid on integer literals", lexer);
    }

    cast<IntegerType>(type)->isUnsigned(uns);
}

std::string Variant::to_string(bool escape) {
    switch (type->getTypeID()) {
        case TypeID::IntegerTypeID:
            return std::to_string(int_val);
        case TypeID::FPTypeID:
            return std::to_string(float_val);
        default:
            return "Fuck";
    }
}

//
//Variant Variant::operator+(Variant v1) {
//    return cdot::var::Arithmetic::add(*this, v1);
//};
//
//Variant Variant::pow(Variant v1) {
//    return cdot::var::Arithmetic::pow(*this, v1);
//}
//
//Variant Variant::operator-(Variant v1) {
//    return cdot::var::Arithmetic::minus(*this, v1);
//};
//
//Variant Variant::operator-() {
//    return cdot::var::Arithmetic::unary_minus(*this);
//}

//Variant& Variant::operator=(const Variant &v) {
//
//    switch (v._type.type) {
//        case INT_T:
//            int_val = v.int_val; break;
//        case LONG_T:
//            long_val = v.long_val; break;
//        case DOUBLE_T:
//            d_val = v.d_val; break;
//        case FLOAT_T:
//            float_val = v.float_val; break;
//        case STRING_T:
//            _type.computed_length = v._type.computed_length;
//            s_val = v.s_val;
//            break;
//        case BOOL_T:
//            b_val = v.b_val;
//        case CHAR_T:
//            c_val = v.c_val;
//            break;
//        case VOID_T:
//            _is_null = true;
//            _type.type = VOID_T;
//            return *this;
//        default:
//            break;
//    }
//
//    if (v._type.class_name == "String") {
//        s_val = v.s_val;
//    }
//
//    initialized = true;
//    _is_null = v._is_null;
//    _type = v._type;
//    check_numeric();
//
//    return *this;
//}
//
//Variant Variant::strict_equals(const Variant &v) {
//    if (get_type().type != v._type.type && !any_type  && initialized && !(_type.nullable && v.is_null())) {
//        RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + val::typetostr(v._type)
//             + " to variable of type " + val::typetostr(get_type()));
//    }
//
//    return operator=(v);
//}
//
//Variant Variant::operator*(Variant v1) {
//    return cdot::var::Arithmetic::multiply(*this, v1);
//};
//
//Variant Variant::operator/(Variant v1) {
//    return cdot::var::Arithmetic::divide(*this, v1);
//};
//
//Variant Variant::operator%(Variant v1) {
//    return cdot::var::Arithmetic::modulo(*this, v1);
//};
//
//Variant Variant::operator!() {
//     return cdot::var::Arithmetic::unary_not(*this);
//}
//
//Variant Variant::operator==(Variant v1) {
//    return cdot::var::Arithmetic::equal(*this, v1);
//}
//
//Variant Variant::operator!=(Variant v1) {
//    return cdot::var::Arithmetic::not_equal(*this, v1);
//}
//
//Variant Variant::operator&&(Variant v1) {
//    return cdot::var::Arithmetic::logical_and(*this, v1);
//}
//
//Variant Variant::operator||(Variant v1) {
//    return cdot::var::Arithmetic::logical_or(*this, v1);
//}
//
//Variant Variant::operator&(Variant v1) {
//    return cdot::var::Arithmetic::bitwise_and(*this, v1);
//}
//
//Variant Variant::operator|(Variant v1) {
//    return cdot::var::Arithmetic::bitwise_or(*this, v1);
//}
//
//Variant Variant::operator~() {
//    return cdot::var::Arithmetic::bitwise_negate(*this);
//}
//
//Variant Variant::operator<(Variant v1) {
//    return cdot::var::Arithmetic::lower(*this, v1);
//}
//
//Variant Variant::operator>(Variant v1) {
//    return cdot::var::Arithmetic::greater(*this, v1);
//}
//
//Variant Variant::operator<=(Variant v1) {
//    return cdot::var::Arithmetic::lower_equal(*this, v1);
//}
//
//Variant Variant::operator>=(Variant v1) {
//    return cdot::var::Arithmetic::greater_equal(*this, v1);
//}
//
//Variant Variant::operator>>(Variant v1) {
//    return cdot::var::Arithmetic::bitshift_right(*this, v1);
//}
//
//Variant Variant::operator<<(Variant v1) {
//    return cdot::var::Arithmetic::bitshift_left(*this, v1);
//}
//
//Variant Variant::operator^(Variant v1) {
//    return cdot::var::Arithmetic::bitwise_xor(*this, v1);
//}

TypeSpecifier& TypeSpecifier::resolve(std::vector<string>& namespaces) {
    return *this;
}

void TypeSpecifier::resolveGeneric(TypeSpecifier &ts, unordered_map<string, TypeSpecifier> &concrete_generics) {
    if (concrete_generics.empty() || !ts.is_generic) {
        return;
    }

    // generic types can themselves be generic
    for (auto& conc : ts.concrete_generic_types) {
        resolveGeneric(conc.second, concrete_generics);
    }

    if (!ts.generic_class_name.empty() && concrete_generics.find(ts.generic_class_name) != concrete_generics.end()) {
        auto gen = concrete_generics[ts.generic_class_name];
        ts.class_name = gen.class_name;
        ts.type = gen.type;
        ts.is_function = gen.is_function;
        ts.concrete_generic_types = gen.concrete_generic_types;
    }

    if (ts.type == CARRAY_T && ts.element_type != nullptr) {
        resolveGeneric(*ts.element_type, concrete_generics);
    }

    for (auto& arg : ts.args) {
        resolveGeneric(arg, concrete_generics);
    }
}

std::string TypeSpecifier::to_string() const {
    std::string _nullable = nullable ? "?" : "";
    if (type == CARRAY_T) {
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

TypeSpecifier& TypeSpecifier::mostGeneral(TypeSpecifier &fst, TypeSpecifier &snd) {
    if (fst.type == OBJECT_T && snd.type != OBJECT_T) {
        return fst;
    }

    if (snd.type == OBJECT_T && fst.type != OBJECT_T) {
        return snd;
    }

    if (fst.type == OBJECT_T) {
        auto cl1 = SymbolTable::getClass(fst.class_name);

        if (cl1->is_base_class_of(snd.class_name)) {
            return fst;
        }

        return snd;
    }

    auto fst_pos = util::type_hierarchy[fst.type];
    auto snd_pos = util::type_hierarchy[snd.type];

    return fst_pos >= snd_pos ? fst : snd;
}

TypeSpecifier::TypeSpecifier(const TypeSpecifier &other) {
    type = other.type;
    class_name = other.class_name;
    arr_length = other.arr_length;
    invalid_ns_ref = other.invalid_ns_ref;

    cstring = other.cstring;
    is_primitive = other.is_primitive;
    computed_length = other.computed_length;
    bitwidth = other.bitwidth;
    is_unsigned = other.is_unsigned;
    is_generic = other.is_generic;
    generic_class_name = other.generic_class_name;
    is_vararg = other.is_vararg;
    concrete_generic_types.clear();
    for (const auto& gen : other.concrete_generic_types) {
        concrete_generic_types.emplace(gen.first, gen.second);
    }
    nullable = other.nullable;
    static_const = other.static_const;
    is_const = other.is_const;
    is_function = other.is_function;
    is_pointer = other.is_pointer;
    is_lambda = other.is_lambda;
    is_assignable = other.is_assignable;
    resolved = other.resolved;
    lambda_id = other.lambda_id;

    cstyle_vararg = other.cstyle_vararg;

    args = other.args;
    if (other.element_type != nullptr) {
        element_type = new TypeSpecifier(*other.element_type);
    }
}

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

    if (type == CARRAY_T) {
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

    bool is_compatible(ValueType v1, ValueType v2) {
        return (v1 == ANY_T || v2 == ANY_T || (v1 == v2)) || util::in_vector<ValueType>(util::implicit_type_conversions[v1], v2);
    }

    bool is_compatible(TypeSpecifier& given, TypeSpecifier& needed) {

        // nullable
        if ((given.nullable && needed.type == VOID_T) || (needed.nullable && given.type == VOID_T)) {
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

        // cstring
        if (given.cstring && needed.is_pointer && needed.bitwidth == 8) {
            return true;
        }

        // raw arrays
        if (given.type == CARRAY_T && needed.is_pointer) {
            return val::is_compatible(*given.element_type, needed);
        }
        if ((given.type == CARRAY_T) != (needed.type == CARRAY_T)) {
            return false;
        }
        if (given.type == CARRAY_T && needed.type == CARRAY_T) {
            return val::is_compatible(*given.element_type, *needed.element_type);
        }

        // class implements interface
        if (needed.type == INTERFACE_T) {
            if (given.type == INTERFACE_T) {
                return needed.class_name == given.class_name;
            }

            return SymbolTable::getClass(given.class_name)->conformsTo(needed.class_name);
        }

        // autoboxing
        if (given.type != OBJECT_T && needed.type == OBJECT_T) {
            return SymbolTable::getClass(needed.class_name)->is_base_class_of(util::types[given.type]);
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

            return SymbolTable::getClass(needed.class_name)->is_base_class_of(given.class_name);
        }

        if ((given.bitwidth == 8 && needed.type == CHAR_T) || (given.type == CHAR_T && needed.bitwidth == 8)) {
            return true;
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

            return SymbolTable::getClass(t1.class_name)->is_base_class_of(t2.class_name);
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

        if (t.type == CARRAY_T) {
            return true;
        }

        if (t.cstring) {
            return true;
        }

        if (t.type == OBJECT_T) {
            return SymbolTable::getClass(t.class_name)->hasMethod("init", {}).compatibility ==
                CompatibilityType::COMPATIBLE;
        }

        return false;
    }

    TypeSpecifier simple_arithmetic_return_type(TypeSpecifier& t1, TypeSpecifier& t2) {
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