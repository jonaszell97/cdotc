//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <set>
#include "Variant/Variant.h"

struct TypeSpecifier;
class TypeCheckVisitor;
class Expression;

enum ValueType : unsigned int;

enum class AccessModifier {
    PUBLIC,
    PRIVATE,
    PROTECTED
};

struct CallCompatability {
    bool is_compatible = false;
    size_t incomp_arg;
    int compat_score;
    bool perfect_match = false;
};

using std::pair;
using std::string;

namespace util {
    extern std::unordered_map<std::string, int> op_precedence;

    std::vector<std::string> str_split(std::string, char);
    std::string str_trim(std::string);

    std::string args_to_string(std::vector<TypeSpecifier>&);

    extern std::unordered_map<std::string, TypeSpecifier> builtin_functions;
    extern std::vector<std::string> builtin_namespaces;

    inline bool is_builtin_import(std::string name) {
        return std::find(builtin_namespaces.begin(), builtin_namespaces.end(), name) != builtin_namespaces.end();
    }

    template <class T>
    bool in_vector(std::vector<T>, T);
    std::string str_escape(std::string);

    template<class T, class R>
    bool in_pair_vector(std::vector<std::pair<T, R>>, T);

    template<class T, class R>
    R get_second(std::vector<std::pair<T, R>>, T);

    template <class T>
    std::unordered_map<std::string, T> merge_maps(std::unordered_map<std::string, T> m1,
         std::unordered_map<std::string, T> m2)
    {
        m1.insert(m2.begin(), m2.end());
        return m1;
    };

    std::string generate_getter_name(std::string);
    std::string generate_setter_name(std::string);

    std::string type_to_symbol(TypeSpecifier type);

    extern std::string token_names[];
    extern std::vector<std::string> keywords;

    extern std::unordered_map<ValueType, std::string> types;
    extern std::unordered_map<std::string, ValueType> typemap;
    extern std::unordered_map<ValueType, std::string> classmap;
    extern std::unordered_map<AccessModifier, std::string> am_map;
    bool is_reversible(std::string);

    extern std::vector<std::string> binary_operators;
    extern std::vector<std::string> tertiary_operators;
    extern std::vector<std::string> unary_operators;

    extern std::set<std::string> string_modifiers;

    extern std::vector<char> operator_chars;
    extern std::vector<char> punctuators;

    extern std::vector<std::string> attributes;

    extern std::unordered_map<ValueType, std::vector<ValueType>> implicit_type_conversions;
    extern std::unordered_map<ValueType, std::vector<ValueType>> explicit_type_conversions;

    bool resolve_generic(TypeSpecifier& given, TypeSpecifier& needed,
        std::vector<TypeSpecifier>& given_generics, std::vector<pair<string, TypeSpecifier>>& needed_generics);

    CallCompatability func_call_compatible(std::vector<TypeSpecifier>& given_args, std::vector<TypeSpecifier>& needed_args,
        std::vector<TypeSpecifier>& given_generics, std::vector<pair<string, TypeSpecifier>>& needed_generics);

    CallCompatability func_call_compatible(std::vector<TypeSpecifier>& given_args, std::vector<TypeSpecifier>& needed_args);
    CallCompatability func_call_compatible(std::vector<std::shared_ptr<Expression>>& given_args, std::vector<TypeSpecifier>&
        needed_args, TypeCheckVisitor& Visitor);

    CallCompatability func_call_compatible(std::vector<std::shared_ptr<Expression>>& given_args,
        std::vector<TypeSpecifier>& needed_args, TypeCheckVisitor& Visitor, std::vector<TypeSpecifier>& given_generics,
        std::vector<pair<string, TypeSpecifier>>&needed_generics);

    int func_score(std::vector<TypeSpecifier>&);
};


#endif //UTIL_H
