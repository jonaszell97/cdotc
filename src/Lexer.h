//
// Created by Jonas Zell on 18.06.17.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include "Token.h"
#include "Parser.h"
#include <vector>

class Parser;

class Tokenizer {
public:
    Tokenizer(std::string);

    void advance(TokenType, bool = true);
    void advance(bool = true);
    Token lookahead(bool = true);
    Token lookbehind();
    char get_next_char();
    char char_lookahead();
    void backtrack_c(int);
    void backtrack();

    std::string s_val();
    inline double d_val() {
        return current_token.get_value().get<double>();
    }
    inline bool b_val() {
        return current_token.get_value().get<bool>();
    }
    inline char c_val() {
        return current_token.get_value().get<char>();
    }

    int current_index;
    int current_line;
    int index_on_line;
    int last_token_index;
    int current_token_index;
    Token current_token;

    friend class Parser;
    friend class ParseError;
    typedef std::unique_ptr<Tokenizer> UniquePtr;

protected:
    std::vector<Token> tokens;
    std::string _program;

    Token get_next_token(bool);
    Token _get_next_token(bool);
    void ignore_comment();
    void ignore_ml_comment();
    bool is_letter(char);
    bool is_operator_char(char);
    bool is_operator(std::string);
    bool is_number(char, bool);
    bool is_hex(char);
    bool is_keyword(std::string);
    bool is_type_keyword(std::string);
    bool is_punctuator(char);
    bool is_bool_literal(std::string);
    char escape_char(char);
};


#endif //TOKENIZER_H
