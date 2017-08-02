//
// Created by Jonas Zell on 18.06.17.
//

#include "Tokenizer.h"
#include "Util.h"
#include "StdLib/Objects/Object.h"
#include "Variant/Variant.h"

Tokenizer::Tokenizer(std::string program) :
    current_token(T_BOF, { }, 0, 0, 0),
    _program(program),
    current_index(0),
    current_line(0),
    index_on_line(0),
    current_token_index(0)
{
    while (current_token.get_type() != T_EOF) {
        current_token = _get_next_token(false);
        tokens.push_back(current_token);
    }

    current_token_index = 0;
    current_token = tokens.front();
    last_token_index = 0;
    current_index = current_token.get_start();
}

std::string Tokenizer::s_val() {
    if (current_token.get_value().get_type().type != STRING_T) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected token " + util::token_names[current_token.get_type()], this);
    }

    return current_token.get_value().get<std::string>();
}

/**
 * Decides wether or not a character could be part of a number token.
 * @param _c
 * @param allowDecimalPoint Whether to allow a decimal point or not
 * @return
 */
bool Tokenizer::is_number(char _c, bool allowDecimalPoint) {
    int c = int(_c);
    // digits and dot
    return (c >= 48 && c <= 57) || (allowDecimalPoint && c == C_DOT);
}

bool Tokenizer::is_hex(char _c) {
    int c = int(_c);

    return (c >= C_CAP_A && c <= C_CAP_F) || (c >= C_A && c <= C_F) || (c >= 48 && c <= 57);
}

/**
 * Decides whether or not a character could be part of a keyword or identifier token.
 * @param _c
 * @return
 */
bool Tokenizer::is_letter(char _c) {
    int c = int(_c);
    // valid identifiers contain letters, _, $ and numbers
    return (c >= C_CAP_A && c <= C_CAP_Z) || (c >= C_A && c <= C_Z)
           || c == C_UNDERSCORE || c == C_DOLLAR_SIGN || is_number(c, false);
}

/**
 * Decides whether a value can be an identifier or not.
 * @param _c
 * @return
 */
bool Tokenizer::is_operator(std::string s) {
    for (int i = 0; i < util::binary_operators.size(); i++) {
        if (s == util::binary_operators[i]) {
            return true;
        }
    }
    for (int i = 0; i < util::unary_operators.size(); i++) {
        if (s == util::unary_operators[i]) {
            return true;
        }
    }
    for (int i = 0; i < util::tertiary_operators.size(); i++) {
        if (s == util::tertiary_operators[i]) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::is_operator_char(char c) {
    for (int i = 0; i < util::operator_chars.size(); i++) {
        if (c == util::operator_chars[i]) {
            return true;
        }
    }

    return false;
}

/**
 * Returns true if the given string is a language keyword.
 * @param s
 * @return
 */
bool Tokenizer::is_keyword(std::string s) {
    for (int i = 0; i < util::keywords.size(); i++) {
        if (s == util::keywords[i]) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::is_type_keyword(std::string s) {
    if (s.back() == C_CLOSE_SQUARE) {
        s.pop_back();
        while (s.back() > '0' && s.back() < '9') {
            s.pop_back();
        }
        s.pop_back();
    }

    for (auto type : util::types) {
        if (s == type.second) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::is_punctuator(char c) {
    for (int i = 0; i < util::punctuators.size(); i++) {
        if (c == util::punctuators[i]) {
            return true;
        }
    }

    return false;
}

bool Tokenizer::is_bool_literal(std::string s) {
    return s == "true" || s == "false";
}


/**
 * Gets the next character in the program
 * @return
 */
char Tokenizer::get_next_char() {
    if (current_index >= _program.length()) {
        current_index++;
        return '\0';
    }

    char c = _program[current_index++];
    index_on_line++;
    if (c == C_NEWLINE) {
        current_line++;
        index_on_line = 0;
    }

    return c;
}

char Tokenizer::char_lookahead() {
    if (current_index >= _program.length()) {
        current_index++;
        return '\0';
    }

    char c = _program[current_index];
    index_on_line++;
    if (c == C_NEWLINE) {
        current_line++;
        index_on_line = 0;
    }

    return c;
}

/**
 * Backtracks to the last token
 */
void Tokenizer::backtrack() {
    --current_token_index;
}

/**
 * Backtracks by the passed amount of characters
 * @param length
 */
void Tokenizer::backtrack_c(int length) {
    current_index -= length;
}

/**
 * Returns the next token
 * @return
 */
Token Tokenizer::get_next_token(bool ignore_newline) {
    if (current_token_index >= tokens.size() - 1) {
        return tokens.back();
    }

    Token t = tokens[current_token_index++];
    if (ignore_newline && t.is_punctuator('\n')) {
        return get_next_token(true);
    }

    last_token_index = t.get_start();
    current_index = t.get_end();

    return t;
}

char Tokenizer::escape_char(char c) {
    switch (c) {
        case 'n':
            return '\n';
        case 'a':
            return '\a';
        case 'r':
            return '\r';
        case 'v':
            return '\v';
        case 't':
            return '\t';
        case 'b':
            return '\b';
        case '"':
            return '\"';
        case '\'':
            return '\'';
        case '0':
            return '\0';
        default:
            return c;
    }
}

/**
 * Parses the next token
 * @return
 */
Token Tokenizer::_get_next_token(bool ignore_newline) {
    if (current_token.get_type() == T_EOF) {
        return current_token;
    }

    last_token_index = current_index;
    int start_line = current_line;
    if (current_index >= _program.length()) {
        return Token(T_EOF, { }, start_line, current_index, current_index);
    }

    std::string t = "";

    int _start_index = current_index;
    char first = get_next_char();
    while (first == ' ') {
        first = get_next_char();
        _start_index++;
    }

    if (first == '\0' && current_index >= _program.length()) {
        return Token(T_EOF, { }, start_line, current_index, current_index);
    }

    char next;

    // check for comment
    if (first == C_HASH) {
        first = get_next_char();
        ignore_comment();

        return _get_next_token(ignore_newline);
    }

    // multi-line comment
    if (first == C_SLASH) {
        next = get_next_char();
        if (next == C_TIMES) {
            ignore_ml_comment();

            return _get_next_token(ignore_newline);
        } else {
            backtrack_c(1);
        }
    }


    // character literal
    if (first == C_SINGLE_QUOTE) {
        next = get_next_char();
        if (next == C_BACKSLASH) {
            next = escape_char(get_next_char());
        }

        char _c = get_next_char();
        if (_c != '\'') {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected \"'\" after character literal", this);
        }

        return Token(T_LITERAL, { next }, start_line, _start_index, current_index);
    }
    // string literal
    else if (first == C_DOUBLE_QUOTE) {
        next = get_next_char();
        while (next != C_DOUBLE_QUOTE && current_index < _program.length()) {
            if (next == C_BACKSLASH) {
                next = get_next_char();
                t += escape_char(next);
            } else {
                t += next;
            }

            next = get_next_char();
        }

        return Token(T_LITERAL, { t }, start_line, _start_index, current_index);
    }
    // number literal (decimal, octal, hexadecimal or binary; with or without exponent or floating point)
    else if (is_number(first, false)) {
        char _pref = get_next_char();
        if (first == '0' && (_pref == 'x' || _pref == 'X')) {
            // hexadecimal literal
            std::string hex_s = "0x";
            next = get_next_char();
            while (is_hex(next)) {
                hex_s += next;
                next = get_next_char();
            }

            backtrack_c(1);
            return Token(T_LITERAL, { long(std::stoul(hex_s, nullptr, 16)) }, start_line, _start_index, current_index);
        }

        if (first == '0' && (_pref == 'b' || _pref == 'B')) {
            // binary literal
            std::string bin_s = "";
            next = get_next_char();
            while (next == '0' || next == '1') {
                bin_s += next;
                next = get_next_char();
            }

            backtrack_c(1);
            return Token(T_LITERAL, { long(std::stoul(bin_s, nullptr, 2)) }, start_line, _start_index, current_index);
        }

        backtrack_c(1); // undo _pref

        // advance integer or floating point number
        next = first;
        bool decimal_point = false;
        while ((next >= '0' && next <= '9') || next == '\'' || next == '.') {
            if (next == '.' && !decimal_point) {
                if (char_lookahead() < '0' || char_lookahead() > '9') {
                    break;
                }
                decimal_point = true;
            }

            t += next;
            next = get_next_char();
        }

        // octal literal
        if (first == '0' && t.length() != 1 && !decimal_point) {
            try {
                backtrack_c(1);
                return Token(T_LITERAL, {std::stoi(t, nullptr, 8)}, start_line, _start_index, current_index);
            } catch(...) {
                ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Invalid octal literal", this);
            }
        }

        // exponent
        if (next == 'e' || next == 'E') {
            std::string _exp = "";
            next = get_next_char();
            while (next == C_MINUS || is_number(next, false)) {
                _exp += next;
                next = get_next_char();
            }

            backtrack_c(1);
            double base = std::stod(t);
            double exp = pow(10, std::stoi(_exp));

            return Token(T_LITERAL, { base * exp }, start_line, _start_index, current_index);
        }

        backtrack_c(1);

        if (!decimal_point) {
            next = char_lookahead();
            if (next == 'l' || next == 'L') {
                get_next_char();
                return Token(T_LITERAL, { std::stol(t) }, start_line, _start_index, current_index);
            }
            else if (next == 'd' || next == 'D') {
                get_next_char();
                return Token(T_LITERAL, { std::stod(t) }, start_line, _start_index, current_index);
            }
            else {
                return Token(T_LITERAL, { std::stoi(t) }, start_line, _start_index, current_index);
            }
        }
        else {
            next = char_lookahead();
            if (next == 'f' || next == 'F') {
                get_next_char();
                return Token(T_LITERAL, { std::stof(t) }, start_line, _start_index, current_index);
            }
            else if (next == 'd' || next == 'D') {
                get_next_char();
            }

            return Token(T_LITERAL, { std::stod(t) }, start_line, _start_index, current_index);
        }
    }
    else if (is_letter(first)) {
        // advance keyword or identifier
        next = first;
        while (is_letter(next)) {
            t += next;
            next = get_next_char();
        }

        backtrack_c(1);

        if (t == "typeof") {
            return Token(T_OP, { t }, start_line, _start_index, current_index);
        }
        else if (t == "new") {
            return Token(T_OP, { t }, start_line, _start_index, current_index);
        }
        else if (t == "null") {
            return Token(T_LITERAL, { }, start_line, _start_index, current_index);
        }
        else if (is_keyword(t)) {
            return Token(T_KEYWORD, { t }, start_line, _start_index, current_index);
        }
        else if (is_type_keyword(t)) {
            return Token(T_TYPE, { t }, start_line, _start_index, current_index);
        }
        else if (is_bool_literal(t)) {
            return Token(T_LITERAL, { t == "true" }, start_line, _start_index, current_index);
        }
        else if (t == "_") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "'_' is a reserved identifier", this);
        }
        else {
            return Token(T_IDENT, { t }, start_line, _start_index, current_index);
        }
    }
    else if (is_operator_char(first)) {
        while (is_operator_char(first)) {
            t += first;
            first = get_next_char();
        }

        backtrack_c(1);

        if (t == ".") {
            return Token(T_PUNCTUATOR, { '.' }, start_line, _start_index, current_index);
        }

        while (!is_operator(t) && t.length() > 0) {
            t.pop_back();
            backtrack_c(1);
        }

        return Token(T_OP, Variant(t), start_line, _start_index, current_index);
    }
    else if (is_punctuator(first)) {
        return Token(T_PUNCTUATOR, { first }, start_line, _start_index, current_index);
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, u8"Unexpected character " + std::string(1, first), this);
    }

    return Token(T_EOF, {}, start_line, current_index, current_index);
}

/**
 * Advances to the next token, while expecting 'type'
 * @param type
 */
void Tokenizer::advance(TokenType type, bool ignore_newline) {
    current_token = get_next_token(ignore_newline);
    if (current_token.get_type() != type) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected " + util::token_names[type]
                + " but got " + util::token_names[current_token.get_type()], this);
    }
}

/**
 * Advances to the next token without restriction
 */
void Tokenizer::advance(bool ignore_newline) {
    current_token = get_next_token(ignore_newline);
}

/**
 * Looks ahead to the next token without influencing the parsing
 * @return
 */
Token Tokenizer::lookahead(bool ignore_newline) {
    if (current_token_index >= tokens.size() - 1) {
        return tokens.back();
    }

    Token ret = tokens[current_token_index];
    int  i = 1;
    while (ignore_newline && ret.is_punctuator('\n')) {
        ret = tokens[current_token_index + i++];
    }

    return ret;
}

/**
 * Returns the last token
 * @return
 */
Token Tokenizer::lookbehind() {
    if (current_token_index == 0) {
        return tokens.front();
    }

    return tokens[current_token_index - 1];
}

/**
 * Skips to the end of a comment
 */
void Tokenizer::ignore_comment() {
    char c = get_next_char();

    while (c != C_NEWLINE) {
        if (current_index >= _program.length()) {
            return;
        }

        c = get_next_char();
    }
}

/**
 * Skips to the end of a multiline comment
 */
void Tokenizer::ignore_ml_comment() {
    char c = get_next_char();

    main_loop:
    while (c != C_TIMES) {
        if (current_index >= _program.length()) {
            return;
        }

        c = get_next_char();
    }

    c = get_next_char();
    if (c != C_SLASH) {
        goto main_loop;
    }
}
