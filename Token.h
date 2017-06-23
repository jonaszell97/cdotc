//
// Created by Jonas Zell on 13.06.17.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Variant.h"
#include <map>

/**
 * Tokens with K_ represent keywords
 * Tokens with T_ represent other Tokens
 */
enum TokenType {
    T_KEYWORD,
    T_TYPE,
    T_IDENT,
    T_PUNCTUATOR,
    T_OP,
    T_LITERAL,
    T_BOF,
    T_EOF
};

enum OperatorType {
    UNARY_OP,
    BINARY_OP,
    TERTIARY_OP
};

enum LiteralType {
    STRING_LITERAL,
    NUMBER_LITERAL,
    CHAR_LITERAL,
    BOOL_LITERAL,
    OBJECT_LITERAL
};

extern std::map<LiteralType, std::string> _literal_names;

enum CharCode {
    C_A = 97,
    C_Z = 122,
    C_CAP_A = 65,
    C_CAP_Z = 90,
    C_F = 102,
    C_CAP_F = 70,
    C_PLUS = 43,
    C_MINUS = 45,
    C_TIMES = 42,
    C_SLASH = 47,
    C_PERCENT = 37,
    C_OPEN_PAREN = 40,
    C_CLOSE_PAREN = 41,
    C_OPEN_CURLY = 123,
    C_CLOSE_CURLY = 125,
    C_OPEN_SQUARE = 91,
    C_CLOSE_SQUARE = 93,
    C_LOWER = 60,
    C_GREATER = 62,
    C_ANGLED_OPEN = 60,
    C_ANGLED_CLOSE = 62,
    C_PIPE = 124,
    C_DOLLAR_SIGN = 36,
    C_SINGLE_QUOTE = 39,
    C_DOUBLE_QUOTE = 34,
    C_BACKSLASH = 92,
    C_COMMA = 44,
    C_UNDERSCORE = 95,
    C_DOT = 46,
    C_EQUALS = 61,
    C_NEWLINE = 10,
    C_SEMICOLON = 59,
    C_HASH = 35,
    C_CARET = 94,
    C_COLON = 58,
    C_TILDE = 126
};

class Token {
    public:
        Token (TokenType, Variant, int);
        TokenType get_type();
        Variant get_value();
        int get_index();
        char get_punctuator();
        bool is_punctuator(char);
        bool is_literal(LiteralType);
        bool is_operator(OperatorType);
        Token set_literal_type(LiteralType);
        Token set_operator_type(OperatorType);
    protected:
        Variant _value;
        TokenType _type;
        int _index;
        OperatorType _op_type;
        LiteralType _literal_type;
};


#endif //TOKEN_H
