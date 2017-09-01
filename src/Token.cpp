//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

Token::Token() {}

Token::Token(TokenType type, Variant content, int line, int start, int end) : line(line), start(start), end(end) {
    _type = type;
    _value = content;
}

Variant Token::get_value() {
    return _value;
}

TokenType Token::get_type() {
    return _type;
}

bool Token::is_punctuator(char c) {
    return _type == T_PUNCTUATOR && _value.charVal == c;
}

bool Token::is_keyword(std::string keyword) {
    return _type == T_KEYWORD && _value.strVal == keyword;
}

bool Token::is_operator(std::string op) {
    return _type == T_OP && _value.strVal == op;
}

bool Token::is_separator() {
    return _type == T_PUNCTUATOR && (_value.intVal == '\n' || _value.intVal == ';');
}