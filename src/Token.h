//
// Created by Jonas Zell on 13.06.17.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>
#include "Variant/Variant.h"

using std::string;

enum TokenType {
    T_KEYWORD,
    T_IDENT,
    T_PUNCTUATOR,
    T_OP,
    T_LITERAL,
    T_BOF,
    T_EOF
};

using cdot::Variant;

class Token {
    public:
        Token();
        Token(TokenType, Variant, int, int, int);
        TokenType get_type();
        Variant get_value();
        inline int get_line() const {
            return line;
        }
        inline int get_start() const {
            return start;
        }
        inline int get_end() const {
            return end;
        }
        char get_punctuator();
        bool is_punctuator(char);
        bool is_keyword(std::string);
        bool is_operator(std::string);
        bool is_separator();

    protected:
        Variant _value;
        TokenType _type;
        int line;
        int start;
        int end;
};


#endif //TOKEN_H
