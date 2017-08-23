//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include "AST/Visitor/StaticAnalysis/Class.h"
#include "Variant/Type/PointerType.h"
#include "Variant/Type/ObjectType.h"
#include "Variant/Type/IntegerType.h"
#include "Variant/Type/FPType.h"
#include "Variant/Type/GenericType.h"
#include <string>
#include <vector>
#include <regex>

namespace util {

    std::unordered_map<string, int> op_precedence {
        {"?", -1},
        {":", -1},
        {"=", 0},
        {"+=", 0},
        {"-=", 0},
        {"*=", 0},
        {"/=", 0},
        {"<<=", 0},
        {">>=", 0},
        {"^=", 0},
        {"&=", 0},
        {"|=", 0},
        {"??", 1},
        {"||", 1},
        {"&&", 2},
        {"|", 3},
        {"&", 4},
        {"^", 4},
        {"!=", 5},
        {"!==", 5},
        {"==", 5},
        {"===", 5},
        {"<=", 6},
        {">=", 6},
        {"<", 6},
        {">", 6},
        {"<<", 7},
        {">>", 7},
        {">>>", 7},
        {"+", 8},
        {"-", 8},
        {"*", 9},
        {"/", 9},
        {"%", 9},
        {"**", 10},
        {"as", 11},
        {"as!", 11},
        {"isa", 11},
        {"..", 12},
        {"infix", 13}
    };

    string token_names[] = {
        "T_KEYWORD",
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
        ">>>",
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
        "=>",
        "as",
        "as!",
        "isa",
        "===",
        "!=="
    };

    std::vector<string> assignmentOperators = {
        "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^="
    };

    string isAssignmentOperator(string& op) {
        if (op == "=") {
            return op;
        }

        if (in_vector(assignmentOperators, op)) {
            return op.substr(0, op.length() - 1);
        }

        return "";
    }

    std::vector<char> operator_chars = {
        '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':', '.', '^'
    };

    std::vector<string> PrefixUnaryOperators = {
        "!",
        "+",
        "-",
        "~",
        "++",
        "--",
        "&",
        "*",
        "..."
    };

    std::vector<string> PostfixUnaryOperators = {
        "++",
        "--"
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
        "protocol",
        "enum",
        "with",
        "namespace",
        "typeof",
        "continue",
        "break",
        "get",
        "set",
        "goto",
        "infix",
        "prefix",
        "postfix",
        "declare",
        "using",
        "native",
        "let",
        "const",
        "extend",
        "typedef",
        "init",
        "delete",
        "ref",
        "unsafe",
        "memberwise",
        "__debug"
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
//            {"Int", INT_T},
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
//            {"Char", CHAR_T},
            {"Object", OBJECT_T},
            {"Void", VOID_T}
    };

    std::vector<string> stdLibImports = {
        "Extern",
        "Any",
        "Interface/Equatable",
        "Interface/Comparable",
        "Interface/Hashable",
        "Interface/Number",
        "Interface/IntegerProtocol",
        "Interface/Iterable",
        "Interface/Iterator",
//        "Interface/Printable",
//        "Int",
//        "Float",
//        "Double",
//        "Bool",
//        "Char",
//        "Array",
//        "ArrayIterator",
//        "String",
//        "Print"
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
        '@',
        '`'
    };

    std::set<string> string_modifiers = {
            "f",
            "u",
            "e"
    };

    std::vector<string> attributes = {
        "suppress",
        "carray",
        "cstring",
        "boxed"
    };

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
            : "get" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
    }

    string generate_setter_name(string field_name) {
        return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
               ? "set_" + field_name
               : "set" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
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

    std::unordered_map<ValueType, unsigned int> type_hierarchy = {
        {BOOL_T, 0},
        {CHAR_T, 1},
        {INT_T, 2},
        {LONG_T, 3},
        {FLOAT_T, 4},
        {DOUBLE_T, 5}
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
            default: break;
        }

        string type_name = type.to_string();

        return std::to_string(type_name.length()) + type_name;
    }

    string args_to_string(std::vector<Type*>&args) {
        string str = "(";
        for (int i = 0; i < args.size(); ++i) {
            str += args.at(i)->toString();
            if (i < args.size() - 1) {
                str += ", ";
            }
        }

        return str + ")";
    }

    bool resolve_generic(
        std::vector<Type*>& givenTypes,
        std::vector<Type*>& neededTypes,
        std::vector<Type*>& given_generics,
        std::vector<GenericType*>& needed_generics,
        unordered_map<size_t, Type*>& neededCasts,
        size_t argNum
    ) {
        size_t i = 0;
        for (const auto& givenTy : givenTypes) {
            auto givenContained = givenTy->getContainedTypes();
            auto neededContained = neededTypes.at(i)->getContainedTypes();

            if (givenContained.size() != neededContained.size()) {
                return false;
            }

            if (!givenContained.empty()) {
                if (!resolve_generic(givenContained, neededContained, given_generics, needed_generics, neededCasts,
                    argNum)
                ) {
                    return false;
                }
            }

            ++i;
        }

        if (!isa<GenericType>(neededTypes.front())) {
            return true;
        }

        auto& given = givenTypes.front();
        auto needed = cast<GenericType>(neededTypes.front());

        if (given_generics.size() < needed_generics.size()) {
            given_generics.push_back(given);
        }

        for (const auto& gen : needed_generics) {
            if (gen->getGenericClassName() == needed->getGenericClassName()) {
                if (!GenericType::GenericTypesCompatible(gen, needed)) {
                    return false;
                }
            }
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
    CallCompatability func_call_compatible(
        std::vector<Type*>& given_args,
        std::vector<Type*>& needed_args,
        std::vector<Type*>& given_generics,
        std::vector<GenericType*>& needed_generics)
    {
        CallCompatability comp;
        size_t given_size = given_args.size();
        size_t needed_size = needed_args.size();
        size_t i = 0;
        bool perfect_match = true;

        if (given_size == 0 && needed_size == 0 && given_generics.empty() && needed_generics.empty()) {
            comp.is_compatible = true;
            comp.compat_score = 0;
            comp.perfect_match = true;
            return comp;
        }

        auto var_arg = needed_size > 0 && needed_args.back()->isVararg();
        if (given_size > needed_size && !var_arg) {
            return comp;
        }

        if (var_arg) {
            goto var_arg;
        }

        for (auto& needed : needed_args) {
            if (i < given_size) {
                auto& given = given_args.at(i);

                if (needed->isGeneric()) {
                    auto givenCont = given->getContainedTypes(true);
                    auto neededCont = needed->getContainedTypes(true);

                    if (givenCont.size() != neededCont.size() || !resolve_generic(givenCont, neededCont, given_generics,
                        needed_generics, comp.needed_casts, i)
                    ) {
                        comp.incomp_arg = i;
                        return comp;
                    }
                    if (*given != needed) {
                        comp.needed_casts.emplace(i, needed);
                    }
                }
                else if (!given->implicitlyCastableTo(needed)) {
                    comp.incomp_arg = i;
                    return comp;
                }
                else if (*given != needed) {
                    perfect_match = false;
                    comp.needed_casts.emplace(i, needed);
                }
            }
            else if (!needed->hasDefaultArgVal()) {
                comp.incomp_arg = i;
                return comp;
            }

            ++i;
        }

        end:
        if (given_generics.size() != needed_generics.size()) {
            return comp;
        }

        comp.is_compatible = true;
        comp.compat_score = func_score(needed_args);
        comp.perfect_match = perfect_match;

        return comp;

        var_arg:
        auto& va_type = needed_args.back();
        for (auto& given : given_args) {
            auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;
            // accepts all types
            if (needed->isCStyleVararg()) {
                goto end;
            }

            if (needed->isGeneric()) {
                auto givenCont = given->getContainedTypes(true);
                auto neededCont = needed->getContainedTypes(true);

                if (givenCont.size() != neededCont.size() || !resolve_generic(givenCont, neededCont, given_generics,
                    needed_generics,comp.needed_casts, i)
                ) {
                    comp.incomp_arg = i;
                    return comp;
                }
            }
            else if (!given->implicitlyCastableTo(needed)) {
                comp.incomp_arg = i;
                return comp;
            }
            else if (*given != needed) {
                perfect_match = false;
                comp.needed_casts.emplace(i, needed);
            }

            ++i;
        }

        goto end;
    }

    CallCompatability func_call_compatible(
        std::vector<Type*>& given_args,
        std::vector<Type*>& needed_args)
    {
        std::vector<Type*> givenGenerics;
        std::vector<GenericType*> neededGenerics;

        return func_call_compatible(given_args, needed_args, givenGenerics, neededGenerics);
    }

    int func_score(std::vector<Type*>& args) {
        int score = 0;
        for (const auto& argument : args) {
            if (isa<ObjectType>(argument)) {
                auto asObj = cast<ObjectType>(argument);
                score += SymbolTable::getClass(asObj->getClassName())->getDepth();
            }
            else if (isa<IntegerType>(argument)) {
                auto asInt = cast<IntegerType>(argument);
                score += 8 - int(asInt->getBitwidth() / 8);
            }
            else if (isa<FPType>(argument)) {
                auto asFloat = cast<FPType>(argument);
                score += 3 - int(asFloat->getPrecision() / 32);
            }
            else {
                score += 1;
            }
        }

        return score;
    }

    bool matches(string pattern, string& subject) {
        return std::regex_match(subject, std::regex(pattern));
    }

    int pointerDepth(llvm::Type *type) {
        unsigned int depth = 0;
        auto current = type;
        while (current->isPointerTy()) {
            ++depth;
            current = current->getPointerElementType();
        }

        return depth;
    }
}