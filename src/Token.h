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

class Token {
public:
   Token();
   Token(TokenType, Variant&&, int, int, bool = false);
   TokenType get_type();
   Variant get_value();

   string toString();

   int getStart() {
      return start;
   }

   int getEnd() {
      return end;
   }

   bool isEscaped() {
      return isEscaped_;
   }

   void setIndent(int indent) {
      this->indent = indent;
   }

   int getIndent() {
      return indent;
   }

   bool is_punctuator(char);
   bool is_keyword(std::string);
   bool is_operator(std::string);
   bool is_separator();

protected:
   Variant _value;
   TokenType _type;

   int start;
   int end;

   int indent = 0;

   bool isEscaped_ = false;
};


#endif //TOKEN_H
