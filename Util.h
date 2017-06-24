//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <map>
#include "Variant/Variant.h"

namespace util {
    extern int min_op_precedence;
    extern int max_op_precedence;
    extern std::map<std::string, int> op_precedence;

    std::vector<std::string> str_split(std::string, char);
    bool in_vector(std::vector<std::string>, std::string);
    std::string str_escape(std::string);

    extern std::string token_names[];
    extern std::vector<std::string> keywords;
    extern std::map<ValueType, std::string> types;
    extern std::map<std::string, ValueType> typemap;
    extern std::vector<std::string> binary_operators;
    extern std::vector<char> operator_chars;
    extern std::vector<char> punctuators;
    extern std::vector<std::string> unary_operators;
    extern std::vector<std::string> unary_only;
    extern std::vector<std::string> equality_operators;
    extern std::map<ValueType, std::vector<ValueType>> type_conversions;
};


#endif //UTIL_H
