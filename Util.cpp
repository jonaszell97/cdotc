//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include <string>
#include <vector>

namespace util {
    int min_op_precedence = 0;

    std::map<std::string, int> op_precedence {
            {"?", 0},
            {":", 0},
            {"=", 0},
            {"+=", 0},
            {"-=", 0},
            {"*=", 0},
            {"/=", 0},
            {"||", 1},
            {"&&", 2},
            {"|", 3},
            {"&", 4},
            {"!=", 5},
            {"==", 5},
            {"<=", 6},
            {">=", 6},
            {"<", 6},
            {">", 6},
            {"<<", 7},
            {">>", 7},
            {"+", 8},
            {"-", 8},
            {"*", 9},
            {"/", 9},
            {"%", 9},
            {"**", 10}
    };

    std::map<std::string, int> unary_op_precedence {
            {"!", 0},
            {"&", 0},
            {"*", 0},
            {"~", 0},
            {"-", 0},
            {"+", 0},
            {"typeof", 0},
            {"--pre", 1},
            {"++pre", 1},
            {"--post", 1},
            {"++post", 1},
    };

    std::string token_names[] = {
            "T_KEYWORD",
            "T_TYPE",
            "T_IDENT",
            "T_PUNCTUATOR",
            "T_OP",
            "T_LITERAL",
            "T_BOF",
            "T_EOF"
    };

    std::vector<std::string> binary_operators = {
            "=",
            "+=",
            "-=",
            "*=",
            "/=",
            "||",
            "&&",
            "|",
            "&",
            "!=",
            "==",
            "<=",
            ">=",
            "<",
            ">",
            "<<",
            ">>",
            "+",
            "-",
            "*",
            "/",
            "%",
            "**"
    };

    std::vector<char> operator_chars = {
        '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':'
    };

    std::vector<std::string> equality_operators = {
            "=",
            "*=",
            "/=",
            "+=",
            "-="
    };

    std::vector<std::string> unary_operators = {
            "!",
            "+",
            "-",
            "~",
            "++",
            "--",
            "&",
            "*",
            "typeof"
    };

    std::vector<std::string> tertiary_operators = {
            "?",
            ":"
    };

    std::vector<std::string> unary_only = {
            "!",
            "++",
            "--",
            "typeof"
    };

    std::vector<std::string> keywords = {
            "def",
            "in",
            "out",
            "outln",
            "return",
            "if",
            "else",
            "while",
            "do",
            "switch",
            "for",
            "case",
            "default",
            "struct",
            "throw",
            "class",
            "public",
            "private",
            "protected",
            "static",
            "new",
            "namespace",
            "typeof",
            "continue",
            "break"
    };

    std::map<ValueType, std::string> types = {
            {INT_T, "int"},
            {LONG_T, "long"},
            {FLOAT_T, "float"},
            {DOUBLE_T, "double"},
            {STRING_T, "string"},
            {BOOL_T, "bool"},
            {CHAR_T, "char"},
            {OBJECT_T, "object"},
            {ANY_T, "any"},
            {VOID_T, "void"},
            {AUTO_T, "let"},
            {REF_T, "reference"}
    };

    std::map<ValueType, std::string> classmap = {
            {INT_T, "Integer"},
            {LONG_T, "Integer"},
            {FLOAT_T, "Double"},
            {DOUBLE_T, "Double"},
            {STRING_T, "String"},
            {BOOL_T, "Boolean"},
            {CHAR_T, "Char"},
            {OBJECT_T, "Object"}
    };

    std::map<AccessModifier, std::string> am_map = {
            {AccessModifier::PUBLIC, "public"},
            {AccessModifier::PRIVATE, "private"},
            {AccessModifier::PROTECTED, "protected"}
    };

    std::map<std::string, ValueType> typemap = {
            {"int", INT_T},
            {"long", LONG_T},
            {"double", DOUBLE_T},
            {"float", FLOAT_T},
            {"string", STRING_T},
            {"bool", BOOL_T},
            {"char", CHAR_T},
            {"object", OBJECT_T},
            {"void", VOID_T},
            {"any", ANY_T},
            {"let", AUTO_T},
            {"reference", REF_T}
    };

    std::vector<char> punctuators = {
        ',',
        '(',
        ')',
        ';',
        '[',
        ']',
        '{',
        '}',
        '\n',
        '.'
    };

    template <>
    bool in_vector<std::string>(std::vector<std::string> vec, std::string el) {
        return std::find(vec.begin(), vec.end(), el) != vec.end();
    }

    template <>
    bool in_vector<ValueType>(std::vector<ValueType> vec, ValueType el) {
        return std::find(vec.begin(), vec.end(), el) != vec.end();
    }

    std::vector<std::string> str_split(std::string source, char delimiter) {
        auto res = std::vector<std::string>();
        std::string s = "";
        for (int i = 0; i < source.length(); i++) {
            if (source[i] == delimiter) {
                res.push_back(s);
                s = "";
            } else {
                s += source[i];
            }
        }

        if (s != "") {
            res.push_back(s);
        }

        return res;
    }

    std::string str_escape(std::string str) {
        std::string res = "";
        for (char c : str) {
            switch (c) {
                case '\n':
                    res += "\\n"; break;
                case '\a':
                    res += "\\a"; break;
                case '\r':
                    res += "\\r"; break;
                case '\v':
                    res += "\\v"; break;
                case '\t':
                    res += "\\t"; break;
                case '\b':
                    res += "\\b"; break;
                case '\e':
                    res += "\\e"; break;
                default:
                    res += c; break;
            }
        }

        return res;
    }

    std::map<ValueType, std::vector<ValueType>> type_conversions = {
            {INT_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T}},
            {LONG_T, {DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T}},
            {FLOAT_T, {DOUBLE_T, STRING_T}},
            {DOUBLE_T, {STRING_T}},
            {BOOL_T, {LONG_T, DOUBLE_T, FLOAT_T, STRING_T, CHAR_T, INT_T}},
            {CHAR_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, INT_T}},
            {STRING_T, {}},
    };
}