//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <set>
#include "Variant/Variant.h"
#include "StdLib/Class/Class.h"

namespace util {
    extern int min_op_precedence;
    extern int max_op_precedence;
    extern std::unordered_map<std::string, int> op_precedence;
    extern std::unordered_map<std::string, int> unary_op_precedence;

    std::vector<std::string> str_split(std::string, char);
    std::string str_trim(std::string);

    std::string field_to_symbol(std::string, TypeSpecifier, bool = false);
    std::string method_to_symbol(std::string, std::vector<TypeSpecifier>, TypeSpecifier, bool = false);
    std::string method_to_symbol(std::string, std::vector<TypeSpecifier>, bool = false);
    std::string fun_to_symbol(std::string, std::vector<TypeSpecifier>, bool = false);
    std::string type_to_symbol(TypeSpecifier);

    template <class T>
    bool in_vector(std::vector<T>, T);
    std::string str_escape(std::string);

    template <class T>
    std::unordered_map<std::string, T> merge_maps(std::unordered_map<std::string, T> m1,
         std::unordered_map<std::string, T> m2)
    {
        m1.insert(m2.begin(), m2.end());
        return m1;
    };

    std::string generate_getter_name(std::string);
    std::string generate_setter_name(std::string);

    extern std::string token_names[];
    extern std::vector<std::string> keywords;

    extern std::unordered_map<ValueType, std::string> types;
    extern std::unordered_map<std::string, ValueType> typemap;
    extern std::unordered_map<ValueType, std::string> classmap;
    extern std::unordered_map<AccessModifier, std::string> am_map;
    extern std::unordered_map<std::string, ValueType> unary_op_return_types;
    extern std::unordered_map<std::string, ValueType> binary_op_return_types;

    bool is_reversible(std::string);

    extern std::vector<std::string> binary_operators;
    extern std::vector<std::string> tertiary_operators;
    extern std::vector<std::string> unary_operators;
    extern std::vector<std::string> equality_operators;

    extern std::set<std::string> string_modifiers;

    extern std::vector<char> operator_chars;
    extern std::vector<char> punctuators;

    extern std::unordered_map<ValueType, std::vector<ValueType>> implicit_type_conversions;
    extern std::unordered_map<ValueType, std::vector<ValueType>> explicit_type_conversions;
};


#endif //UTIL_H
