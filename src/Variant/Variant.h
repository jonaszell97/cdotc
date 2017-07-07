//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cmath>
#include <vector>
#include "../Message/Exceptions.h"

class Function;
class Object;
class Array;
class Expression;
class Class;
class Interface;

namespace cdot {
namespace var {
    class Converter;
    class Arithmetic;
}
}

class Variant;

enum ValueType : unsigned int {
    INT_T,
    LONG_T,
    FLOAT_T,
    DOUBLE_T,
    NUMBER_T,
    STRING_T,
    BOOL_T,
    CHAR_T,
    OBJECT_T,
    ANY_T,
    VOID_T,
    AUTO_T,
    CLASS_T,
    INTERFACE_T,
    REF_T
};

struct TypeSpecifier;

namespace val {
    extern ValueType strtotype(std::string);
    extern std::string typetostr(ValueType);
    extern std::string typetostr(TypeSpecifier);
    extern std::string base_class(ValueType);
    extern std::string type_name(Variant);
    extern bool is_compatible(ValueType , ValueType);
    extern bool is_compatible(TypeSpecifier , TypeSpecifier);
    extern bool is_castable(TypeSpecifier, TypeSpecifier);
    extern bool implicitly_castable(ValueType, ValueType);

    extern ValueType simple_arithmetic_return_type(TypeSpecifier, TypeSpecifier);
    extern ValueType division_return_type(TypeSpecifier, TypeSpecifier);
}

struct TypeSpecifier {
    TypeSpecifier() {
        type = VOID_T;
        is_numeric = false;
    }

    TypeSpecifier(ValueType t) {
        type = t;
        switch (type) {
            case INT_T:
            case FLOAT_T:
            case LONG_T:
            case DOUBLE_T:
                is_numeric = true;
                is_primitive = true;
                break;
            case CHAR_T:
            case BOOL_T:
                is_primitive = true;
                break;
            default:
                is_primitive = false;
                break;
        }
    }

    bool operator==(TypeSpecifier ts) {
        return type == ts.type && class_name == ts.class_name;
    }
    bool operator!=(TypeSpecifier ts) {
        return !operator==(ts);
    }
    bool operator==(ValueType v) {
        return type == v;
    }
    bool operator!=(ValueType v) {
        return type != v;
    }

    ValueType type = VOID_T;
    std::string class_name;
    std::shared_ptr<Expression> length;
    bool is_array = false;
    bool is_var_length = false;
    bool is_primitive = true;
    bool is_numeric = false;
    bool nullable = false;
    bool is_function = false;
    std::vector<TypeSpecifier> args = std::vector<TypeSpecifier>();
    TypeSpecifier* return_type = nullptr;

    std::string to_string() {
        std::string _nullable = nullable ? "?" : "";
        if (is_array) {
            return type == OBJECT_T ? class_name : val::typetostr(type) + "[]" + _nullable;
        }
        if (type == OBJECT_T && !is_function) {
            return class_name + _nullable;
        }
        if (type == CLASS_T) return "Class";
        if (type == INTERFACE_T) return "Interface";

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
};

struct Variant {
protected:
    union {
        int int_val;
        long long_val;
        float float_val;
        double d_val;
        bool b_val;
        char c_val;
    };

    bool any_type = false;

    union {
        std::shared_ptr<Object> o_val;
        std::shared_ptr<Variant> ref;
        Class* class_val;
        Interface* interface_val;
    };

    std::string s_val;
    TypeSpecifier type;
    bool _is_null = false;
    bool is_numeric = false;
    bool initialized = true;
    void check_numeric();
    void destroy();

public:
    typedef std::shared_ptr<Variant> SharedPtr;
    typedef std::unique_ptr<Variant> UniquePtr;

    friend class cdot::var::Converter;
    friend class cdot::var::Arithmetic;

    ~Variant();
    Variant();
    Variant(TypeSpecifier);
    Variant(const Variant&);
    Variant(double);
    Variant(int);
    Variant(long);
    Variant(float);
    Variant(char);
    Variant(bool);
    Variant(std::shared_ptr<Object>);
    Variant(std::shared_ptr<Function>);
    Variant(std::shared_ptr<Variant>);
    Variant(Class*);
    Variant(Interface*);
    Variant(std::string);

    TypeSpecifier get_type() const;
    void is_any_type(bool = true);
    bool is_ref();
    void set_default();
    void is_initialized(bool init) {
        initialized = init;
    }
    inline bool is_initialized() const {
        if (type.type == REF_T) {
            return ref->is_initialized();
        }

        return initialized;
    }

    inline void is_nullable(bool nullable) {
        type.nullable = nullable;
    }
    inline bool is_null() const {
        if (type.type == REF_T) {
            return ref->is_null();
        }

        return _is_null;
    }
    inline void is_null(bool is_null) {
        _is_null = is_null;
    }
    inline void set_type(TypeSpecifier ts) {
        type = ts;
    }

    std::string to_string(bool = false);
    long hash();

    template <class T>
    T get();

    Variant cast_to(ValueType target_type);

    // binary math operators
    Variant operator+(Variant v1);
    Variant operator-(Variant v1);
    Variant operator*(Variant v1);
    Variant operator/(Variant v1);
    Variant operator%(Variant v1);
    Variant pow(Variant v1);

    // equality operators
    Variant operator==(Variant v1);
    Variant operator!=(Variant v1);
    Variant operator<=(Variant v1);
    Variant operator>=(Variant v1);
    Variant operator<(Variant v1);
    Variant operator>(Variant v1);

    // logical operators
    Variant operator&&(Variant v1);
    Variant operator||(Variant v1);

    // bitwise operators
    Variant operator&(Variant v1);
    Variant operator|(Variant v1);
    Variant operator^(Variant v1);
    Variant operator<<(Variant v1);
    Variant operator>>(Variant v1);

    // dereference operator
    Variant operator*();

    // assignment operator
    Variant operator=(const Variant& v);
    Variant strict_equals(const Variant& v);

    // unary operators
    Variant operator!();
    Variant operator-();
    Variant operator~();

    friend std::ostream& operator<<(std::ostream& str, Variant& v) {
        return str << v.to_string(true);
    }
};

#endif //VALUE_H
