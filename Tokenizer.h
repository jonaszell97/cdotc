//
// Created by Jonas Zell on 18.06.17.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include "Token.h"
#include "Interpreter.h"
#include <vector>

class Interpreter;

enum TokenizerFlag {
    TFLAG_NONE,
    TFLAG_IGNORE_NEWLINE
};

class Tokenizer {
public:
    Tokenizer(std::string);

    friend class Interpreter;
    typedef std::unique_ptr<Tokenizer> UniquePtr;
protected:
    std::vector<Token> tokens;
    Interpreter *_interpreter;
    std::string _program;
    int current_index;
    int current_line;
    int index_on_line;
    int last_token_index;
    Token current_token;
    TokenizerFlag _flag;

    inline std::string s_val() {
        return current_token.get_value().get<std::string>();
    }
    inline double d_val() {
        return current_token.get_value().get<double>();
    }
    inline bool b_val() {
        return current_token.get_value().get<bool>();
    }
    inline char c_val() {
        return current_token.get_value().get<char>();
    }
    void set_flag(TokenizerFlag);
    Token get_next_token();
    Token _get_next_token();
    char get_next_char();
    void backtrack_c(int);
    void backtrack();
    void ignore_comment();
    void ignore_ml_comment();
    TokenType advance(TokenType, TokenType);
    void advance(TokenType);
    void advance();
    void expect(TokenType);
    Token lookahead();
    Token lookbehind();
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

    std::string get_next_block();
};


#endif //TOKENIZER_H
