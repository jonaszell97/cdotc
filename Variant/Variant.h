//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cmath>
#include "../Exceptions.h"

class Function;
class Object;
class Array;
class Expression;
class Class;

namespace cdot {
namespace var {
    class Converter;
    class Arithmetic;
}
}

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
    REF_T
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
    };

    std::string s_val;
    ValueType type;
    bool is_numeric = false;
    bool nullable = true;

    void check_numeric();
    void destroy();

public:
    typedef std::shared_ptr<Variant> SharedPtr;
    typedef std::unique_ptr<Variant> UniquePtr;

    friend class cdot::var::Converter;
    friend class cdot::var::Arithmetic;

    ~Variant();
    Variant();
    Variant(const Variant&);
    Variant(double);
    Variant(int);
    Variant(long);
    Variant(float);
    Variant(char);
    Variant(bool);
    Variant(std::shared_ptr<Object>);
    Variant(std::shared_ptr<Function>);
    Variant(std::shared_ptr<Array>);
    Variant(std::shared_ptr<Variant>);
    Variant(Class*);
    Variant(std::string);

    ValueType get_type() const;
    void is_any_type(bool = true);
    bool is_ref();

    inline void is_nullable(bool null) {
        nullable = null;
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

namespace val {
    extern ValueType strtotype(std::string);
    extern std::string typetostr(ValueType);
    extern std::string base_class(ValueType);
    extern std::string type_name(Variant);
    extern bool is_compatible(ValueType , ValueType);
}

struct TypeSpecifier {
    ValueType type;
    std::string class_name;
    std::shared_ptr<Expression> length;
    bool is_array = false;
    bool is_var_length = false;
    bool is_primitive = true;
    bool nullable = false;

    std::string to_string() {
        if (type == OBJECT_T) {
            return class_name;
        }

        return val::typetostr(type);
    }
};

#endif //VALUE_H
