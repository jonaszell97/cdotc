//
// Created by Jonas Zell on 18.06.17.
//

#include "Lexer.h"
#include "Util.h"
#include "Variant/Variant.h"

Lexer::Lexer(string& program) :
    _program(program),
    current_index(0),
    current_line(0),
    index_on_line(0),
    current_token_index(0),
    last_token_index(0)
{

}

std::string Lexer::s_val() {
    if (current_token.get_type() != T_IDENT && current_token.get_type() != T_KEYWORD && current_token.get_type() !=
            T_OP && current_token.get_type() != T_LITERAL) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected token " + util::token_names[current_token.get_type()], this);
    }

    return current_token.get_value().getString();
}

/**
 * Decides wether or not a character could be part of a number token.
 * @param _c
 * @param allowDecimalPoint Whether to allow a decimal point or not
 * @return
 */
bool Lexer::is_number(char c, bool allowDecimalPoint) {
    // digits and dot
    return (c >= '0' && c <= '9') || (allowDecimalPoint && c == '.');
}

bool Lexer::is_hex(char c) {
    return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9');
}

/**
 * Decides whether or not a character could be part of a keyword or identifier token.
 * @param _c
 * @return
 */
bool Lexer::is_identifier_char(char c) {
    // valid identifiers contain ANYTHING (almost)
    return !is_operator_char(c) && !is_punctuator(c) && c != ' ' && c != '\0';
}

/**
 * Decides whether a value can be an identifier or not.
 * @param _c
 * @return
 */
bool Lexer::is_operator(std::string s) {
    if (util::in_vector(util::binary_operators, s)) {
        return true;
    }
    if (util::in_vector(util::assignmentOperators, s)) {
        return true;
    }
    if (util::in_vector(util::PrefixUnaryOperators, s)) {
        return true;
    }
    if (util::in_vector(util::PostfixUnaryOperators, s)) {
        return true;
    }
    if (util::in_vector(util::tertiary_operators, s)) {
        return true;
    }

    return false;
}

bool Lexer::is_operator_char(char c) {
    if (util::in_vector(util::operator_chars, c)) {
        return true;
    }

    return false;
}

/**
 * Returns true if the given string is a language keyword.
 * @param s
 * @return
 */
bool Lexer::is_keyword(std::string s) {
    if (util::in_vector(util::keywords, s)) {
        return true;
    }

    return false;
}

bool Lexer::is_punctuator(char c) {
    for (int i = 0; i < util::punctuators.size(); i++) {
        if (c == util::punctuators[i]) {
            return true;
        }
    }

    return false;
}

bool Lexer::is_bool_literal(std::string s) {
    return s == "true" || s == "false";
}


/**
 * Gets the next character in the program
 * @return
 */
char Lexer::get_next_char() {
    if (current_index >= _program.length()) {
        current_index++;
        return '\0';
    }

    char c = _program[current_index++];
    index_on_line++;

    if (c == '\n') {
        current_line++;
        index_on_line = 0;
    }

    return c;
}

char Lexer::char_lookahead() {
    if (current_index >= _program.length()) {
        current_index++;
        return '\0';
    }

    char c = _program[current_index];
    index_on_line++;

    if (c == '\n') {
        current_line++;
        index_on_line = 0;
    }

    return c;
}

/**
 * Backtracks to the last token
 */
void Lexer::backtrack() {
    --current_token_index;
    current_index = last_token_index;
}

/**
 * Backtracks by the passed amount of characters
 * @param length
 */
void Lexer::backtrack_c(int length) {
    current_index -= length;
}

char Lexer::escape_char(char c) {
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
Token Lexer::_get_next_token(bool ignore_newline, bool significantWhiteSpace) {
    if (current_token.get_type() == T_EOF) {
        return current_token;
    }

    last_token_index = current_index;
    int start_line = current_line;
    if (current_index >= _program.length()) {
        return Token(T_EOF, { }, start_line, current_index, current_index);
    }

    string t = "";

    int _start_index = current_index;
    char first = get_next_char();

    if (first == ' ' && significantWhiteSpace) {
        return Token(T_PUNCTUATOR, { first }, start_line, current_index, current_index);
    }

    while (first == ' ') {
        first = get_next_char();
        _start_index++;
    }

    if (first == '\0' && current_index >= _program.length()) {
        return Token(T_EOF, { }, start_line, current_index, current_index);
    }

    char next;

    // check for comment
    if (first == '#') {
        ignore_comment();

        return _get_next_token(ignore_newline, significantWhiteSpace);
    }

    // multi-line comment
    if (first == '/') {
        next = get_next_char();
        if (next == '*') {
            ignore_ml_comment();

            return _get_next_token(ignore_newline, significantWhiteSpace);
        } else {
            backtrack_c(1);
        }
    }

    // character literal
    if (first == '\'') {
        next = get_next_char();
        if (next == '\\') {
            next = escape_char(get_next_char());
        }

        char _c = get_next_char();
        if (_c != '\'') {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected \"'\" after character literal", this);
        }

        return Token(T_LITERAL, { next }, start_line, _start_index, current_index);
    }

    // string literal
    if (first == '\"') {
        next = get_next_char();
        while (next != '\"' && current_index < _program.length()) {
            if (next == '\\') {
                next = get_next_char();
                t += escape_char(next);
            } else {
                t += next;
            }

            next = get_next_char();
        }

        return Token(T_LITERAL, { t }, start_line, _start_index, current_index);
    }

    // escape sequence
    if (first == '`') {
        next = get_next_char();
        while (next != '`' && current_index < _program.length()) {
            t += next;
            next = get_next_char();

        }

        return Token(T_IDENT, { t }, start_line, _start_index, current_index);
    }

    // number literal (decimal, octal, hexadecimal or binary; with or without exponent or floating point)
    if (is_number(first, false)) {
        char _pref = get_next_char();
        if (first == '0' && (_pref == 'x' || _pref == 'X')) {
            // hexadecimal literal
            string hex_s = "0x";
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
            string bin_s = "";
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
        while ((next >= '0' && next <= '9') || next == '_' || next == '.') {
            if (next == '.' && !decimal_point) {
                if (char_lookahead() < '0' || char_lookahead() > '9') {
                    break;
                }
                decimal_point = true;
            }

            if (next != '_') {
                t += next;
            }
            next = get_next_char();
        }

        // octal literal
        if (util::matches("0[0-7]+", t)) {
            backtrack_c(1);
            return Token(T_LITERAL, { std::stoi(t, nullptr, 8) }, start_line, _start_index,
                current_index);
        }

        // exponent
        if (next == 'e' || next == 'E') {
            std::string _exp = "";
            next = get_next_char();
            while (next == '-' || is_number(next, false)) {
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

            bool isUnsigned = false;
            bool isIntegral = false;
            Variant val;

            if (next == 'u' || next == 'U') {
                get_next_char();
                isUnsigned = true;
                isIntegral = true;
                next = char_lookahead();
            }

            if (next == 'l' || next == 'L') {
                isIntegral = true;
                get_next_char();
                val = Variant(std::stol(t));
                next = char_lookahead();
            }

            if ((next == 'u' || next == 'U') && !isUnsigned) {
                get_next_char();
                isUnsigned = true;
                isIntegral = true;
                next = char_lookahead();
            }

            if ((next == 'd' || next == 'D') && !isIntegral) {
                get_next_char();
                val = Variant(std::stod(t));
            }
            else {
                val = Variant(std::stoi(t));
            }

            val.isUnsigned(isUnsigned, this);

            return Token(T_LITERAL, val, start_line, _start_index, current_index);
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

    if (is_identifier_char(first)) {
        // advance keyword or identifier
        next = first;
        while (is_identifier_char(next)) {
            t += next;
            next = get_next_char();
        }

        if (t == "as" && next == '!') {
            get_next_char();
            t = "as!";
        }

        backtrack_c(1);

        if (is_operator(t)) {
            return Token(T_OP, { t }, start_line, _start_index, current_index);
        }
        else if (t == "null") {
            return Token(T_LITERAL, { }, start_line, _start_index, current_index);
        }
        else if (is_keyword(t)) {
            return Token(T_KEYWORD, { t }, start_line, _start_index, current_index);
        }
        else if (is_bool_literal(t)) {
            return Token(T_LITERAL, { t == "true" }, start_line, _start_index, current_index);
        }
        else {
            return Token(T_IDENT, { t }, start_line, _start_index, current_index);
        }
    }

    if (is_operator_char(first)) {
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

    if (is_punctuator(first)) {
        return Token(T_PUNCTUATOR, { first }, start_line, _start_index, current_index);
    }

    ParseError::raise(ERR_UNEXPECTED_CHARACTER, u8"Unexpected character " + std::string(1, first), this);
    llvm_unreachable("see error above");
}

/**
 * Advances to the next token, while expecting 'type'
 * @param type
 */
void Lexer::advance(TokenType type, bool ignore_newline, bool significantWhiteSpace) {
    current_token = get_next_token(ignore_newline, significantWhiteSpace);
    if (current_token.get_type() != type) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected " + util::token_names[type]
                + " but got " + util::token_names[current_token.get_type()], this);
    }
}

/**
 * Advances to the next token without restriction
 */
void Lexer::advance(bool ignore_newline, bool significantWhiteSpace) {
    current_token = get_next_token(ignore_newline, significantWhiteSpace);
}

/**
 * Returns the next token
 * @return
 */
Token Lexer::get_next_token(bool ignore_newline, bool significantWhiteSpace) {
    if (!tokens.empty() && tokens.back().get_type() == T_EOF) {
        return tokens.back();
    }

    if (current_token_index >= tokens.size()) {
        tokens.push_back(_get_next_token(ignore_newline, significantWhiteSpace));
    }

    Token t = tokens[current_token_index++];
    if (ignore_newline && t.is_punctuator('\n')) {
        return get_next_token(true, significantWhiteSpace);
    }

    last_token_index = t.get_start();
    current_index = t.get_end();

    return t;
}

/**
 * Looks ahead to the next token without influencing the parsing
 * @return
 */
Token Lexer::lookahead(bool ignore_newline, size_t offset) {
    if (!tokens.empty() && tokens.back().get_type() == T_EOF) {
        return tokens.back();
    }

    if ((current_token_index + offset) >= tokens.size()) {
        tokens.push_back(_get_next_token(ignore_newline, false));
    }

    Token t = tokens[current_token_index + offset];
    if (ignore_newline && t.is_punctuator('\n')) {
        return lookahead(ignore_newline, offset + 1);
    }

    return t;
}

/**
 * Skips to the end of a comment
 */
void Lexer::ignore_comment() {
    char c = get_next_char();

    while (c != '\n') {
        if (current_index >= _program.length()) {
            return;
        }

        c = get_next_char();
    }
}

/**
 * Skips to the end of a multiline comment
 */
void Lexer::ignore_ml_comment() {
    char c = get_next_char();

    main_loop:
    while (c != '*') {
        if (current_index >= _program.length()) {
            return;
        }

        c = get_next_char();
    }

    c = get_next_char();
    if (c != '/') {
        goto main_loop;
    }
}
