//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include "AST/Visitor/StaticAnalysis/Class.h"
#include <string>
#include <vector>

namespace util {

    std::unordered_map<string, int> op_precedence {
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
            {"..", 12},
            {"infix", 13}
    };

    string token_names[] = {
            "T_KEYWORD",
            "T_TYPE",
            "T_IDENT",
            "T_PUNCTUATOR",
            "T_OP",
            "T_LITERAL",
            "T_BOF",
            "T_EOF"
    };

    std::vector<string> binary_operators = {
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
            "^",
            "->",
            "=>"
    };

    std::vector<char> operator_chars = {
        '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':', '.', '^'
    };

    std::vector<string> unary_operators = {
            "!",
            "+",
            "-",
            "~",
            "++",
            "--",
            "&",
            "*",
            "typeof",
            "new",
            "..."
    };

    std::vector<string> tertiary_operators = {
            "?",
            ":"
    };

    std::vector<string> keywords = {
        "def",
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
        "interface",
        "enum",
        "with",
        "namespace",
        "typeof",
        "continue",
        "break",
        "get",
        "set",
        "goto",
        "operator",
        "export",
        "using",
        "from",
        "let",
        "const",
        "extend",
        "typedef",
        "init",
        "delete"
    };

    std::unordered_map<ValueType, string> types = {
            {INT_T, "Int"},
            {FLOAT_T, "Float"},
            {DOUBLE_T, "Double"},
            {STRING_T, "String"},
            {BOOL_T, "Bool"},
            {CHAR_T, "Char"},
            {OBJECT_T, "Object"},
            {VOID_T, "Void"}
    };

    std::unordered_map<ValueType, string> classmap = {
            {INT_T, "Integer"},
            {LONG_T, "Integer"},
            {FLOAT_T, "Double"},
            {DOUBLE_T, "Double"},
            {STRING_T, "String"},
            {BOOL_T, "Boolean"},
            {CHAR_T, "Char"},
            {OBJECT_T, "Object"}
    };

    std::unordered_map<AccessModifier, string> am_map = {
            {AccessModifier::PUBLIC, "public"},
            {AccessModifier::PRIVATE, "private"},
            {AccessModifier::PROTECTED, "protected"}
    };

    std::unordered_map<string, ValueType> typemap = {
            {"Int", INT_T},
            {"UInt", INT_T},
            {"Int8", INT_T},
            {"UInt8", INT_T},
            {"Int16", INT_T},
            {"UInt16", INT_T},
            {"Int32", INT_T},
            {"UInt32", INT_T},
            {"Int64", INT_T},
            {"UInt64", INT_T},

            {"Double", DOUBLE_T},
            {"Float", FLOAT_T},
            {"Bool", BOOL_T},
            {"Char", CHAR_T},
            {"Object", OBJECT_T},
            {"Void", VOID_T}
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
        '\\',
        '@'
    };

    std::set<string> string_modifiers = {
            "f",
            "u",
            "e"
    };

    std::vector<string> attributes = {
        "UnsafePtr",
        "NoStdLib",
        "Suppress",
        "RawArray",
        "CString",
        "Boxed"
    };

    template <class T>
    bool in_vector(std::vector<T> vec, T el) {
        return std::find(vec.begin(), vec.end(), el) != vec.end();
    }
    
    template bool in_vector<string>(std::vector<string>, string);

    template<class T, class R>
    bool in_pair_vector(std::vector<std::pair<T, R>> vec, T el) {
        return std::find_if(vec.begin(), vec.end(), [el](const std::pair<T, R> pair) {
            return pair.first == el;
        }) != vec.end();
    };

    template<class T, class R>
    R get_second(std::vector<std::pair<T, R>> vec, T el) {
        auto pos = std::find_if(vec.begin(), vec.end(), [el](const std::pair<T, R> pair) {
            return pair.first == el;
        });

        return pos->second;
    };

    template bool in_pair_vector<string, cdot::cl::Method*>(std::vector<pair<string, cdot::cl::Method*>>, string);
    template cdot::cl::Method* get_second<string, cdot::cl::Method*>(std::vector<pair<string, cdot::cl::Method*>>, string);

    template bool in_pair_vector<string, cdot::cl::Field*>(std::vector<pair<string, cdot::cl::Field*>>, string);
    template cdot::cl::Field* get_second<string, cdot::cl::Field*>(std::vector<pair<string, cdot::cl::Field*>>,
        string);

    template bool in_pair_vector<string, string>(std::vector<pair<string, string>>, string);
    template string get_second<string, string>(std::vector<pair<string, string>>, string);

    template <>
    bool in_vector<ValueType>(std::vector<ValueType> vec, ValueType el) {
        return std::find(vec.begin(), vec.end(), el) != vec.end();
    }

    std::vector<string> str_split(string source, char delimiter) {
        auto res = std::vector<string>();
        string s = "";
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

    string str_trim(string target) {
        while (target[0] == ' ' || target[0] == '\n' || target[0] == '\t') {
            target = target.substr(1);
        }
        while (target[target.length() - 1] == ' ' || target[target.length() - 1] == '\n' || target[target.length() -
                1] == '\t') {
            target = target.substr(0, target.length() - 1);
        }

        return target;
    }

    string generate_getter_name(string field_name) {
        return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
            ? "get_" + field_name
            : "Get" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
    }

    string generate_setter_name(string field_name) {
        return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
               ? "set_" + field_name
               : "Set" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
    }

    string str_escape(string str) {
        string res = "";
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

    bool is_reversible(string op) {
        return op == "*" || op == "+" || op == "&" || op == "|" || op == "^";
    }

    std::unordered_map<ValueType, std::vector<ValueType>> implicit_type_conversions = {
            {INT_T, {LONG_T, DOUBLE_T, FLOAT_T, BOOL_T, STRING_T}},
            {LONG_T, {DOUBLE_T, FLOAT_T, BOOL_T, STRING_T}},
            {FLOAT_T, {DOUBLE_T, STRING_T}},
            {DOUBLE_T, {STRING_T}},
            {BOOL_T, {LONG_T, DOUBLE_T, FLOAT_T, STRING_T, INT_T}},
            {CHAR_T, {}},
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

    string type_to_symbol(TypeSpecifier type) {
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

        string type_name = type.to_string();

        return std::to_string(type_name.length()) + type_name;
    }

    string args_to_string(std::vector<TypeSpecifier>&args) {
        string str = "(";
        for (int i = 0; i < args.size(); ++i) {
            str += args.at(i).to_string();
            if (i < args.size() - 1) {
                str += ", ";
            }
        }

        return str + ")";
    }

    std::unordered_map<string, TypeSpecifier> builtin_functions = {
        {"printf", TypeSpecifier(INT_T)},
        {"puts", TypeSpecifier(INT_T)},
        {"scanf", TypeSpecifier(INT_T)},
        {"sprintf", TypeSpecifier(INT_T)},
        {"snprintf", TypeSpecifier(INT_T)},
        {"time", TypeSpecifier(INT_T)},
        {"srand", TypeSpecifier(VOID_T)},
        {"rand", TypeSpecifier(INT_T)},
        {"free", TypeSpecifier(VOID_T)}
    };

    std::vector<string> builtin_namespaces = {
        "Primitive"
    };

    bool resolve_generic(TypeSpecifier& given, TypeSpecifier& needed, std::vector<TypeSpecifier>& given_generics,
        std::vector<pair<string, TypeSpecifier>>& needed_generics)
    {
        if (needed.element_type != nullptr && given.element_type != nullptr && needed.element_type->is_generic) {
            std::vector<TypeSpecifier> new_given;
            auto res = resolve_generic(*given.element_type, *needed.element_type, new_given, needed_generics);
            if (new_given.size() != given_generics.size()) {
                given_generics.insert(given_generics.begin(), new_given.begin(), new_given.end());
            }

            return res;
        }

        if (!needed.is_generic) {
            return true;
        }

        if (given_generics.size() < needed_generics.size()) {
            given_generics.push_back(given);
        }

        auto covariance = get_second(needed_generics, needed.generic_class_name);
        if (!val::is_compatible(given, covariance)) {
            return false;
        }

        if (util::typemap.find(covariance.class_name) != util::typemap.end()) {
            covariance.type = util::typemap[covariance.class_name];
        }

        return true;
    }

    /**
     * Returns:
     *  -1 if compatible
     *  -2 if incompatible argument count
     *  index of first incompatible arg otherwise
     * @param given
     * @param needed
     * @param given_generics
     * @param needed_generics
     * @return
     */
    CallCompatability func_call_compatible(std::vector<TypeSpecifier>& given_args, std::vector<TypeSpecifier>& needed_args,
        std::vector<TypeSpecifier>& given_generics, std::vector<pair<string, TypeSpecifier>>& needed_generics)
    {
        CallCompatability comp;
        size_t given_size = given_args.size();
        size_t needed_size = needed_args.size();
        size_t i = 0;
        bool perfect_match = true;

        if (given_size == 0 && needed_size == 0 && given_generics.empty() && needed_generics.empty()) {
            comp.is_compatible = true;
            comp.compat_score = 0;
            return comp;
        }

        auto var_arg = needed_size > 0 && needed_args.back().is_vararg;
        if (given_size > needed_size && !var_arg) {
            return comp;
        }

        if (var_arg) {
            goto var_arg;
        }

        for (auto& needed : needed_args) {
            if (i < given_size) {
                auto& given = given_args.at(i);

                if (!resolve_generic(given, needed, given_generics, needed_generics)) {
                    comp.incomp_arg = i;
                    return comp;
                }

                if (!val::is_compatible(given, needed)) {
                    comp.incomp_arg = i;
                    return comp;
                }
                if (given != needed) {
                    perfect_match = false;
                }
            }
            else if (!needed.nullable) {
                comp.incomp_arg = i;
                return comp;
            }

            ++i;
        }

        end:
        comp.is_compatible = true;
        comp.compat_score = func_score(needed_args);
        comp.perfect_match = perfect_match;

        return comp;

        var_arg:
        auto& va_type = needed_args.back();
        for (auto& given : given_args) {
            auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;

            if (!resolve_generic(given, needed, given_generics, needed_generics)) {
                comp.incomp_arg = i;
                return comp;
            }

            if (!val::is_compatible(given, needed)) {
                comp.incomp_arg = i;
                return comp;
            }
            if (given != needed) {
                perfect_match = false;
            }

            ++i;
        }

        goto end;
    }

    /**
     * Returns:
     *  -1 if compatible
     *  -2 if incompatible argument count
     *  index of first incompatible arg otherwise
     * @param given
     * @param needed
     * @param given_generics
     * @param needed_generics
     * @return
     */
    CallCompatability func_call_compatible(std::vector<Expression::SharedPtr>& given_args, std::vector<TypeSpecifier>&
        needed_args, TypeCheckVisitor& Visitor, std::vector<TypeSpecifier>& given_generics,
        std::vector<pair<string, TypeSpecifier>>& needed_generics)
    {
        size_t given_size = given_args.size();
        size_t needed_size = needed_args.size();
        size_t i = 0;
        std::vector<TypeSpecifier> given;

        if (given_size > needed_size && !(needed_size > 0 && needed_args.back().is_vararg)) {
            return CallCompatability();
        }

        if (needed_size > 0 && needed_args.back().is_vararg) {
            goto var_arg;
        }

        for (auto& needed : needed_args) {
            if (i < given_size) {
                auto& given_expr = given_args.at(i);

                given_expr->checkIfReturnable(needed);
                given.push_back(given_expr->accept(Visitor));
                given_expr->doneCheck();
            }

            ++i;
        }

        end:
        return func_call_compatible(given, needed_args, given_generics, needed_generics);

        var_arg:
        auto& va_type = needed_args.back();
        for (auto& given_expr : given_args) {
            auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;

            given_expr->checkIfReturnable(needed);
            given.push_back(given_expr->accept(Visitor));
            given_expr->doneCheck();
            ++i;
        }

        goto end;
    }

    CallCompatability func_call_compatible(std::vector<TypeSpecifier>& given_args, std::vector<TypeSpecifier>&needed_args) {
        CallCompatability comp;
        size_t given_size = given_args.size();
        size_t needed_size = needed_args.size();
        size_t i = 0;
        bool perfect_match = true;

        if (given_size == 0 && needed_size == 0) {
            comp.is_compatible = true;
            comp.compat_score = 0;
            return comp;
        }

        auto var_arg = needed_size > 0 && needed_args.back().is_vararg;
        if (given_size > needed_size && !var_arg) {
            return comp;
        }

        if (var_arg) {
            goto var_arg;
        }

        for (auto& needed : needed_args) {
            if (i < given_size) {
                auto& given = given_args.at(i);
                if (!val::is_compatible(given, needed)) {
                    comp.incomp_arg = i;
                    return comp;
                }
                if (given != needed) {
                    perfect_match = false;
                }
            }
            else if (!needed.nullable) {
                comp.incomp_arg = i;
                return comp;
            }

            ++i;
        }

        end:
        comp.is_compatible = true;
        comp.compat_score = func_score(needed_args);
        comp.perfect_match = perfect_match;

        return comp;

        var_arg:
        auto& va_type = needed_args.back();
        for (auto& given : given_args) {
            auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;
            if (!val::is_compatible(given, needed)) {
                comp.incomp_arg = i;
                return comp;
            }
            if (given != needed) {
                perfect_match = false;
            }

            ++i;
        }

        goto end;
    }

    CallCompatability func_call_compatible(std::vector<std::shared_ptr<Expression>>& given_args, std::vector<TypeSpecifier>&
        needed_args, TypeCheckVisitor& Visitor)
    {
        size_t given_size = given_args.size();
        size_t needed_size = needed_args.size();
        size_t i = 0;
        std::vector<TypeSpecifier> given;

        if (needed_size > 0 && needed_args.back().is_vararg) {
            goto var_arg;
        }

        for (auto& needed : needed_args) {
            if (i < given_size) {
                auto& given_expr = given_args.at(i);

                given_expr->checkIfReturnable(needed);
                given.push_back(given_expr->accept(Visitor));
                given_expr->doneCheck();
            }

            ++i;
        }

        end:
        return func_call_compatible(given, needed_args);

        var_arg:
        auto& va_type = needed_args.back();
        for (auto& given_expr : given_args) {
            auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;

            given_expr->checkIfReturnable(needed);
            given.push_back(given_expr->accept(Visitor));
            given_expr->doneCheck();
            ++i;
        }

        goto end;
    }

    int func_score(std::vector<TypeSpecifier>& args) {
        int score = 0;
        for (const auto& arg : args) {
            if (arg.type == OBJECT_T && !arg.raw_array) {
                score += Namespace::global()->get_class(arg.class_name)->getDepth();
            }
            else if (!arg.raw_array) {
                score += Namespace::global()->get_class(types[arg.type])->getDepth();
            }
            else {
                score += Namespace::global()->get_class(arg.element_type->class_name)->getDepth();
            }
        }

        return score;
    }
}