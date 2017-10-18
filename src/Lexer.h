//
// Created by Jonas Zell on 18.06.17.
//

#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

#include "Token.h"

namespace llvm {
   class MemoryBuffer;
}

class Parser;

class Lexer {
public:
   Lexer();
   Lexer(
      llvm::MemoryBuffer *buf,
      string const& fileName,
      unsigned sourceId
   );

   void advance(TokenType, bool = true, bool = false);
   void advance(bool = true, bool = false);
   Token lookahead(bool = true, size_t i = 0);
   char get_next_char();
   char char_lookahead();
   void backtrack_c(int);
   void backtrack();

   void hardBacktrack();

   void reset(const char *src, size_t len);
   void ignoreInterpolation(bool ignore)
   {
      ignore_interpolation = ignore;
   }

   size_t lastTokenEnd();

   static char escape_char(char);
   static string unescape_char(char);

   string s_val();

   const char* getSrc()
   {
      return src;
   }

   bool continueInterpolation = false;
   size_t indexOnLine = 0;
   size_t currentLine = 1;

   size_t current_index = 0;
   size_t last_token_index = 0;
   size_t current_token_index = 0;
   Token current_token;

   friend class Parser;
   friend class ParseError;

   typedef std::unique_ptr<Lexer> UniquePtr;

protected:
   std::vector<Token> tokens;
   const char *src;
   string fileName;

   unsigned sourceId = 0;

   bool ignore_interpolation = false;

   size_t srcLen = 0;

   Token makeToken(TokenType ty, Variant &&val, size_t start, size_t end, bool isEscaped = false);

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
