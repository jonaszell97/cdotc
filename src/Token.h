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
   T_EOF,

   T_DIRECTIVE,
   T_PREPROC_VAR,
};

using cdot::Variant;

struct Token {
   Token();
   Token(TokenType type, Variant &&val, size_t start, size_t end, size_t line,
      size_t indexOnLine, bool isEscaped = false);

   TokenType get_type();
   Variant get_value();

   string toString();

   size_t getStart() {
      return start;
   }

   size_t getEnd() {
      return end;
   }

   size_t getLine() {
      return line;
   }

   size_t getIndexOnLine() {
      return indexOnLine;
   }

   bool isEscaped() {
      return isEscaped_;
   }

   void setIndent(int indent) {
      this->indent = indent;
   }

   size_t getIndent() {
      return indent;
   }

   bool is_punctuator(char);
   bool is_keyword(std::string);
   bool is_operator(std::string);
   bool is_separator();

   bool isInterpolationStart = false;

   Variant _value;
   TokenType _type;

   size_t start;
   size_t end;
   size_t line;
   size_t indexOnLine;

   size_t indent = 0;

   bool isEscaped_ = false;
};


#endif //TOKEN_H
