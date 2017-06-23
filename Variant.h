//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cmath>
#include "Exceptions.h"

class Function;
class FuncArgument;
class Object;
class ObjectProperty;
class Array;

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
    OBJECT_PROP_T,
    ANY_T,
    FUNCTION_T,
    FUNC_ARG_T,
    ARRAY_T,
    VOID_T,
    AUTO_T
    //REF_T
};

struct Variant;
typedef std::shared_ptr<Variant> VariantPtr;

struct Variant {

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
        std::shared_ptr<ObjectProperty> op_val;
        std::shared_ptr<Function> fun_val;
        std::shared_ptr<FuncArgument> fa_val;
        std::shared_ptr<Array> arr_val;
    };

    std::string s_val;

    void check_numeric();
    void destroy();

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
    Variant(std::shared_ptr<ObjectProperty>);
    Variant(std::shared_ptr<Function>);
    Variant(std::shared_ptr<FuncArgument>);
    Variant(std::shared_ptr<Array>);
    Variant(std::string);


    static VariantPtr from(double);
    static VariantPtr from(int);
    static VariantPtr from(long);
    static VariantPtr from(float);
    static VariantPtr from(char);
    static VariantPtr from(bool);
    static VariantPtr from(std::string);
    static VariantPtr from(Variant);

    ValueType type;
    bool is_numeric = false;

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
    extern Variant empty();
    extern ValueType strtotype(std::string);

    extern bool is_compatible(ValueType , ValueType);
}

#endif //VALUE_H
