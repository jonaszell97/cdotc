//
// Created by Jonas Zell on 14.06.17.
//

#ifndef VALUE_H
#define VALUE_H

#include <string>
#include <cmath>
#include <vector>
#include <llvm/IR/Value.h>
#include <llvm/IR/DerivedTypes.h>
#include <unordered_map>
#include "../Message/Exceptions.h"
#include "../Util.h"

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

using std::string;
using std::pair;
using std::unordered_map;
class Variant;
class Namespace;

enum ValueType : unsigned int {
    INT_T,
    LONG_T,
    FLOAT_T,
    DOUBLE_T,
    STRING_T,
    BOOL_T,
    CHAR_T,
    OBJECT_T,
    ANY_T,
    VOID_T,
    AUTO_T,
    CLASS_T,
    INTERFACE_T
};

struct TypeSpecifier;

namespace val {
    extern ValueType strtotype(std::string);
    extern std::string typetostr(ValueType);
    extern std::string typetostr(TypeSpecifier);
    extern std::string base_class(ValueType);
    extern std::string type_name(Variant);
    extern bool is_compatible(ValueType , ValueType);
    extern bool is_compatible(TypeSpecifier& , TypeSpecifier&);
    extern bool is_castable(TypeSpecifier&, TypeSpecifier&);
    extern bool implicitly_castable(ValueType, ValueType);
    extern bool has_default(TypeSpecifier);

    extern TypeSpecifier simple_arithmetic_return_type(TypeSpecifier&, TypeSpecifier&);
    extern ValueType division_return_type(TypeSpecifier&, TypeSpecifier&);
}

struct TypeSpecifier {
    TypeSpecifier() {
        type = VOID_T;
    }

    TypeSpecifier(ValueType t) {
        type = t;
        switch (type) {
            case INT_T:
            case FLOAT_T:
            case LONG_T:
            case DOUBLE_T:
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

    TypeSpecifier& resolve(Namespace*);
    static void resolveGeneric(TypeSpecifier &ts, unordered_map<string, TypeSpecifier>&);

    bool operator==(TypeSpecifier& ts);

    bool operator!=(TypeSpecifier& ts) {
        return !operator==(ts);
    }
    bool operator==(ValueType v) {
        return type == v;
    }
    bool operator!=(ValueType v) {
        return type != v;
    }

    TypeSpecifier& operator=(const TypeSpecifier&) = default;

    ValueType type = VOID_T;
    std::string class_name;
    std::shared_ptr<Expression> arr_length = nullptr;

    bool invalid_ns_ref = false;

    bool raw_array = false;
    bool cstring = false;

    bool is_primitive = true;
    size_t computed_length = 0;

    int bitwidth = 32;
    bool is_unsigned = false;

    bool is_generic = false;
    string generic_class_name;

    bool is_vararg = false;

    std::unordered_map<string, TypeSpecifier> concrete_generic_types = {};

    bool nullable = false;

    bool static_const = false;
    bool is_const = false;

    bool is_function = false;
    bool is_pointer = false;
    bool is_reference = false;
    bool is_lambda = false;
    bool resolved = false;
    unsigned int lambda_id;

    std::vector<string> ns_name;
    std::vector<TypeSpecifier> args;

    union {
        TypeSpecifier* return_type = nullptr;
        TypeSpecifier* element_type;
    };

    std::string to_string() const;
};

struct CGValue {
    CGValue() {

    }

    CGValue(llvm::Value *val, bool lvalue = true) : val(val), lvalue(lvalue) {

    }

    llvm::Value* val = nullptr;

    size_t arr_size = 0;
    bool lvalue = true;
    bool needs_alloc = true;
    bool const_arr_size = false;

    unsigned int alignment = 8;
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

    std::string s_val;
    TypeSpecifier type;
    bool _is_null = false;
    bool is_numeric = false;
    bool initialized = true;
    void check_numeric();

public:
    typedef std::shared_ptr<Variant> SharedPtr;
    typedef std::unique_ptr<Variant> UniquePtr;

    friend class cdot::var::Converter;
    friend class cdot::var::Arithmetic;

    Variant();
    Variant(TypeSpecifier);
    Variant(const Variant&);
    Variant(double);
    Variant(int);
    Variant(long);
    Variant(float);
    Variant(char);
    Variant(bool);
    Variant(std::string);

    TypeSpecifier& get_type();
    void is_any_type(bool = true);
    void set_default();
    void is_initialized(bool init) {
        initialized = init;
    }
    inline bool is_initialized() const {
        return initialized;
    }

    inline void is_nullable(bool nullable) {
        type.nullable = nullable;
    }
    inline bool is_null() const {
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

    // assignment operator
    Variant& operator=(const Variant& v);
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
