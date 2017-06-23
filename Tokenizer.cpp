//
// Created by Jonas Zell on 18.06.17.
//

#include "Tokenizer.h"
#include "Util.h"
#include "Objects/Object.h"
#include "Variant.h"

Tokenizer::Tokenizer(std::string program) :
        current_token(T_BOF, { }, 0),
        _flag(TFLAG_NONE),
        _program(program),
        current_index(0),
        current_line(0),
        index_on_line(0) {

}

void Tokenizer::set_flag(TokenizerFlag flag) {
    _flag = flag;
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

/**
 * Backtracks to the last token
 */
void Tokenizer::backtrack() {
    current_index = current_token.get_index();
    tokens.pop_back();
    current_token = tokens.back();
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
Token Tokenizer::get_next_token() {
    Token t = _get_next_token();
    tokens.push_back(t);
    if (_flag == TFLAG_IGNORE_NEWLINE && t.is_punctuator('\n')) {
        return get_next_token();
    }

    return tokens.back();
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
        case 'e':
            return '\e';
        case '"':
            return '\"';
        case '\'':
            return '\'';
        default:
            return c;
    }
}

/**
 * Parses the next token
 * @return
 */
Token Tokenizer::_get_next_token() {
    if (current_token.get_type() == T_EOF) {
        return current_token;
    }

    last_token_index = current_index;
    if (current_index >= _program.length()) {
        return Token(T_EOF, { }, current_index);
    }

    std::string t = "";

    int _start_index = current_index;
    char first = get_next_char();
    while (first == ' ') {
        first = get_next_char();
    }
    char next;

    // check for comment
    if (first == C_HASH) {
        first = get_next_char();
        ignore_comment();

        return get_next_token();
    }

    // multi-line comment
    if (first == C_SLASH) {
        next = get_next_char();
        if (next == C_TIMES) {
            ignore_ml_comment();

            return get_next_token();
        } else {
            backtrack_c(2);
        }
    }

    // parentheses
    if (is_punctuator(first)) {
        return Token(T_PUNCTUATOR, { first }, _start_index);
    }
    // character literal
    else if (first == C_SINGLE_QUOTE) {
        next = get_next_char();
        if (next == C_BACKSLASH) {
            next = escape_char(get_next_char());
        }

        char _c = get_next_char();
        if (_c != '\'') {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected \"'\" after character literal");
        }

        return Token(T_LITERAL, { next }, _start_index);
    }
    // string literal
    else if (first == C_DOUBLE_QUOTE) {
        next = get_next_char();
        while (next != C_DOUBLE_QUOTE) {
            if (next == C_BACKSLASH) {
                next = get_next_char();
                t += escape_char(next);
            } else {
                t += next;
            }

            next = get_next_char();
        }

        return Token(T_LITERAL, { t }, _start_index);
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
            return Token(T_LITERAL, { long(std::stoul(hex_s, nullptr, 16)) }, _start_index);
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
            return Token(T_LITERAL, { long(std::stoul(bin_s, nullptr, 2)) }, _start_index);
        }

        backtrack_c(1); // undo _pref

        if (first == '0' && t.length() > 1) {
            // octal literal
            std::string oct_s = "0";
            next = get_next_char();
            while (is_number(next, false)) {
                oct_s += next;
                next = get_next_char();
            }

            backtrack_c(1);
            return Token(T_LITERAL, { long(std::stoul(oct_s, nullptr, 8)) }, _start_index);
        }

        // advance integer or floating point number
        next = first;
        while (is_number(next, true) || next == C_SINGLE_QUOTE) {
            if (next != C_SINGLE_QUOTE) {
                t += next;
            }
            next = get_next_char();
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

            return Token(T_LITERAL, { base * exp }, _start_index);
        }

        backtrack_c(1);

        long long_val;
        double d_val;
        try {
            long_val = std::stol(t);
            d_val = std::stod(t);
        } catch(std::out_of_range) {
            RuntimeError::raise(ERR_VAL_TOO_LARGE, "Number " + t + " is out of range");
        }

        if (std::floor(d_val) == d_val) {
            return Token(T_LITERAL, { long_val }, _start_index);
        } else {
            return Token(T_LITERAL, { d_val }, _start_index);
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
            return Token(T_OP, { t }, _start_index);
        }
        else if (is_keyword(t)) {
            return Token(T_KEYWORD, { t }, _start_index);
        }
        else if (is_type_keyword(t)) {
            return Token(T_TYPE, { t }, _start_index);
        }
        else if (is_bool_literal(t)) {
            return Token(T_LITERAL, { t == "true" }, _start_index);
        }
        else {
            return Token(T_IDENT, { t }, _start_index);
        }
    }
    else if (is_operator_char(first)) {
        while (is_operator_char(first)) {
            t += first;
            first = get_next_char();
        }

        backtrack_c(1);

        while (!is_operator(t) && t.length() > 0) {
            t.pop_back();
            backtrack_c(1);
        }

        return Token(T_OP, Variant(t), _start_index);
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Unexpected character " + std::string(1, first));
    }

    return Token(T_EOF, {}, -1);
}

/**
 * Advances to the next token, while expecting 'type'
 * @param type
 */
void Tokenizer::advance(TokenType type) {
    current_token = get_next_token();
    if (current_token.get_type() != type) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected " + util::token_names[type]
                                                + " but got " + util::token_names[current_token.get_type()]);
    }
}

/**
 * Advances to the next token while expecting t1 or t2 as the token type
 * @param t1
 * @param t2
 * @return The actual token type
 */
TokenType Tokenizer::advance(TokenType t1, TokenType t2) {
    current_token = get_next_token();
    if (current_token.get_type() != t1 && current_token.get_type() != t2) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected " + util::token_names[t1] + " or " + util::token_names[t2]
                                                + " but got " + util::token_names[current_token.get_type()]);
    }

    return current_token.get_type();
}

/**
 * Advances to the next token without restriction
 */
void Tokenizer::advance() {
    current_token = get_next_token();
}

/**
 * Throws an error if the current token does not match the expected one
 * @param type
 */
void Tokenizer::expect(TokenType type) {
    if (current_token.get_type() != type) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected " + util::token_names[type]
                                                + " but got " + util::token_names[current_token.get_type()]);
    }
}

/**
 * Looks ahead to the next token without influencing the parsing
 * @return
 */
Token Tokenizer::lookahead() {
    int _current_index = current_index;
    int _last_token_index = last_token_index;
    Token t = get_next_token();
    tokens.pop_back();

    current_index = _current_index;
    last_token_index = _last_token_index;

    return t;
}

/**
 * Returns the last token
 * @return
 */
Token Tokenizer::lookbehind() {
    return tokens.at(tokens.size() - 1);
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

/**
 * Returns an entire block, until parentheses even out
 * @return
 */
std::string Tokenizer::get_next_block() {
    set_flag(TFLAG_IGNORE_NEWLINE);
    advance();
    if (!(current_token.is_punctuator(C_OPEN_CURLY))) {
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected opening curly bracket to start a block statement.");
    }

    int open_count = 1;
    int close_count = 0;
    int last_index = current_index;
    int start = current_index;

    while (close_count != open_count && current_token.get_type() != T_EOF) {
        advance();
        if (current_token.is_punctuator(C_OPEN_CURLY)) {
            open_count++;
        }
        else if (current_token.is_punctuator(C_CLOSE_CURLY)) {
            close_count++;
            if (close_count == open_count) {
                backtrack();
                last_index = current_index;
            }
        }
    }

    if (close_count != open_count) {
        ParseError::raise(ERR_EOF, "Expected closing curly bracket after block statement.");
    }

    auto res = _program.substr(start, last_index - start);

    advance();
    set_flag(TFLAG_NONE);

    return res;
}
