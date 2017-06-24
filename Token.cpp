//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"
#include "Variant/Variant.h"

std::map<LiteralType, std::string> _literal_names = {
        {NUMBER_LITERAL, "NumberLiteral"},
        {CHAR_LITERAL, "CharacterLiteral"},
        {BOOL_LITERAL, "BooleanLiteral"},
        {STRING_LITERAL, "StringLiteral"},
        {OBJECT_LITERAL, "ObjectLiteral"}
};

Token::Token(TokenType type, Variant content, int index) {
    _type = type;
    _value = content;
    _index = index;
}

int Token::get_index() {
    return _index;
}

Variant Token::get_value() {
    return _value;
}

TokenType Token::get_type() {
    return _type;
}

bool Token::is_punctuator(char c) {
    return _type == T_PUNCTUATOR && _value.get<char>() == c;
}

char Token::get_punctuator() {
    return _value.get<char>();
}

bool Token::is_literal(LiteralType type) {
    return _type == T_LITERAL && _literal_type == type;
}

bool Token::is_operator(OperatorType type) {
    return _type == T_OP && _op_type == type;
}

Token Token::set_literal_type(LiteralType type) {
    _literal_type = type;

    return *this;
}

Token Token::set_operator_type(OperatorType type) {
    _op_type = type;

    return *this;
}