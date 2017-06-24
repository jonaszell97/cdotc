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
    FUNCTION_T,
    ARRAY_T,
    VOID_T,
    AUTO_T,
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
        std::shared_ptr<Function> fun_val;
        std::shared_ptr<Variant> ref;
        std::shared_ptr<Array> arr_val;
    };

    std::string s_val;
    ValueType type;
    bool is_numeric = false;

    void check_numeric();
    void destroy();

public:
    typedef std::shared_ptr<Variant> SharedPtr;
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
    Variant(std::string);

    ValueType get_type() const;
    void is_any_type(bool = true);
    bool is_ref();
    Variant dereference();

    std::string to_string(bool = false);
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
    extern bool is_compatible(ValueType , ValueType);
}

#endif //VALUE_H
