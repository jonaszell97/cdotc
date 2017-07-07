//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include <string>
#include <vector>

namespace util {
    int min_op_precedence = 0;

    std::unordered_map<std::string, int> op_precedence {
            {"?", 0},
            {":", 0},
            {"=", 0},
            {"+=", 0},
            {"-=", 0},
            {"*=", 0},
            {"/=", 0},
            {"??", 1},
            {"||", 1},
            {"&&", 2},
            {"|", 3},
            {"&", 4},
            {"^", 4},
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
            {"**", 10},
            {"typecast", 11},
            {"..", 12}
    };

    std::unordered_map<std::string, int> unary_op_precedence {
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
            {"new", 2}
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
            "**",
            "..",
            "??",
            "^"
    };

    std::unordered_map<std::string, ValueType> binary_op_return_types = {
            {"=", VOID_T},
            {"+=", VOID_T},
            {"-=", VOID_T},
            {"*=", VOID_T},
            {"/=", VOID_T},
            {"||", BOOL_T},
            {"&&", BOOL_T},
            {"&", INT_T},
            {"|", INT_T},
            {"!=", BOOL_T},
            {"==", BOOL_T},
            {"<=", BOOL_T},
            {">=", BOOL_T},
            {"<", BOOL_T},
            {">", BOOL_T},
            {"<<", INT_T},
            {">>", INT_T},
            {"+", INT_T},
            {"-", INT_T},
            {"*", INT_T},
            {"/", FLOAT_T},
            {"%", INT_T},
            {"**", INT_T},
            {"..", OBJECT_T},
            {"??", ANY_T},
            {"^", INT_T},
    };

    std::vector<char> operator_chars = {
        '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':', '.', '^'
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
            "typeof",
            "new"
    };

    std::unordered_map<std::string, ValueType> unary_op_return_types = {
            {"!", BOOL_T},
            {"+", INT_T},
            {"-", INT_T},
            {"~", INT_T},
            {"++", INT_T},
            {"--", INT_T},
            {"&", REF_T},
            {"*", ANY_T},
            {"typeof", STRING_T},
            {"new", OBJECT_T}
    };

    std::vector<std::string> tertiary_operators = {
            "?",
            ":"
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
            "abstract",
            "extends",
            "interface",
            "enum",
            "implements",
            "namespace",
            "typeof",
            "continue",
            "break",
            "get",
            "set",
            "goto",
            "operator",
            "module",
            "export",
            "import",
            "as",
            "from"
    };

    std::unordered_map<ValueType, std::string> types = {
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

    std::unordered_map<ValueType, std::string> classmap = {
            {INT_T, "Integer"},
            {LONG_T, "Integer"},
            {FLOAT_T, "Double"},
            {DOUBLE_T, "Double"},
            {STRING_T, "String"},
            {BOOL_T, "Boolean"},
            {CHAR_T, "Char"},
            {OBJECT_T, "Object"}
    };

    std::unordered_map<AccessModifier, std::string> am_map = {
            {AccessModifier::PUBLIC, "public"},
            {AccessModifier::PRIVATE, "private"},
            {AccessModifier::PROTECTED, "protected"}
    };

    std::unordered_map<std::string, ValueType> typemap = {
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
        '.',
        '\\'
    };

    std::set<std::string> string_modifiers = {
            "f",
            "u",
            "e"
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

    std::string str_trim(std::string target) {
        while (target[0] == ' ' || target[0] == '\n' || target[0] == '\t') {
            target = target.substr(1);
        }
        while (target[target.length() - 1] == ' ' || target[target.length() - 1] == '\n' || target[target.length() -
                1] == '\t') {
            target = target.substr(0, target.length() - 1);
        }

        return target;
    }

    std::string generate_getter_name(std::string field_name) {
        return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
            ? "get_" + field_name
            : "get" + std::string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
    }

    std::string generate_setter_name(std::string field_name) {
        return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
               ? "set_" + field_name
               : "set" + std::string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
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
                default:
                    res += c; break;
            }
        }

        return res;
    }

    bool is_reversible(std::string op) {
        return op == "*" || op == "+" || op == "&" || op == "|" || op == "^";
    }

    std::unordered_map<ValueType, std::vector<ValueType>> implicit_type_conversions = {
            {INT_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T}},
            {LONG_T, {DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T}},
            {FLOAT_T, {DOUBLE_T, STRING_T}},
            {DOUBLE_T, {STRING_T}},
            {BOOL_T, {LONG_T, DOUBLE_T, FLOAT_T, STRING_T, CHAR_T, INT_T}},
            {CHAR_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, INT_T}},
            {STRING_T, {}},
            {VOID_T, {BOOL_T}},
    };

    std::unordered_map<ValueType, std::vector<ValueType>> explicit_type_conversions = {
            {INT_T, {INT_T, LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T}},
            {LONG_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T, INT_T}},
            {FLOAT_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T, INT_T}},
            {DOUBLE_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T, INT_T}},
            {BOOL_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T, INT_T}},
            {CHAR_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T, CHAR_T, INT_T}},
            {STRING_T, {}},
    };

    std::string field_to_symbol(std::string name, TypeSpecifier type, bool is_static) {
        return "__F_" + name + "_" + type_to_symbol(type) + "_" + (is_static ? "_st" : "");
    }

    std::string method_to_symbol(std::string name, std::vector<TypeSpecifier> args, TypeSpecifier return_type, bool
        is_static) {
        std::string symbol = "$M_" + name + "_" + type_to_symbol(return_type);
        for (auto arg : args) {
            symbol += "_" + type_to_symbol(arg);
        }

        if (is_static) {
            symbol += "_st";
        }

        return symbol;
    }

    std::string fun_to_symbol(std::string name, std::vector<TypeSpecifier> args, bool is_static) {
        std::string symbol = "$F_" + std::to_string(name.length()) + name + (args.size() > 0 ? "_" : "");
        for (auto arg : args) {
            symbol += type_to_symbol(arg);
        }

        if (is_static) {
            symbol += "_st";
        }

        return symbol;
    }

    std::string method_to_symbol(std::string name, std::vector<TypeSpecifier> args, bool is_static) {
        std::string symbol = "$M_" + name;
        for (auto arg : args) {
            symbol += "_" + type_to_symbol(arg);
        }

        if (is_static) {
            symbol += "_st";
        }

        return symbol;
    }

    std::string type_to_symbol(TypeSpecifier type) {
        switch (type.type) {
            case INT_T: return "1i";
            case LONG_T: return "1l";
            case FLOAT_T: return "1f";
            case DOUBLE_T: return "1d";
            case BOOL_T: return "1b";
            case CHAR_T: return "1c";
            case VOID_T: return "1v";
            case STRING_T: return "6String";
        }

        std::string type_name = type.to_string();

        return std::to_string(type_name.length()) + type_name;
    }
}