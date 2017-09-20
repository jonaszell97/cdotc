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

class Lexer {
public:
   Lexer();
   explicit Lexer(string &src, string &fileName);
   
   void reset(string &src, string &fileName);

   void advance(TokenType, bool = true, bool = false);
   void advance(bool = true, bool = false);
   Token lookahead(bool = true, size_t i = 0);
   char get_next_char();
   char char_lookahead();
   void backtrack_c(int);
   void backtrack();

   static char escape_char(char);
   static string unescape_char(char);

   string s_val();

   int current_index;
   int index_on_line;
   int last_token_index;
   int current_token_index;
   Token current_token;

   friend class Parser;
   friend class ParseError;
   typedef std::unique_ptr<Lexer> UniquePtr;

protected:
   std::vector<Token> tokens;
   string& src;
   string fileName;

   Token get_next_token(bool, bool);
   Token _get_next_token(bool, bool);
   void ignore_comment();
   void ignore_ml_comment();
   bool is_identifier_char(char);
   bool is_operator_char(char);
   bool is_operator(std::string);
   bool is_number(char, bool);
   bool is_hex(char);
   bool is_keyword(std::string);

   bool is_punctuator(char);
   bool is_bool_literal(std::string);
};


#endif //TOKENIZER_H
