//
// Created by Jonas Zell on 13.06.17.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

#include "../Variant/Variant.h"
#include "SourceLocation.h"

namespace cdot {

enum TokenType : unsigned char {
   T_KEYWORD = 0,
   T_IDENT,
   T_PUNCTUATOR,
   T_OP,
   T_LITERAL,
   T_BOF,
   T_EOF,

   T_DIRECTIVE,
   T_PREPROC_VAR,
};

} // namespace cdot

using namespace cdot;

struct Token {
   Token();
   ~Token();
   Token(TokenType type, Variant &&val, SourceLocation loc = {},
      unsigned start = 0);

   TokenType get_type();
   Variant get_value();

   std::string toString() const;

   unsigned getStart() const
   {
      return start;
   }

   unsigned getEnd() const
   {
      return start + loc.getLength();
   }

   unsigned getLine() const
   {
      return loc.getLine();
   }

   unsigned getCol() const
   {
      return loc.getCol();
   }

   const SourceLocation& getSourceLoc() const
   {
      return loc;
   }

   bool is_punctuator(char) const;
   bool is_punctuator() const;

   bool is_keyword(const std::string &) const;
   bool is_keyword() const;

   bool is_operator(const std::string &) const;
   bool is_operator() const;

   bool is_identifier(const std::string &) const;
   bool is_identifier() const;

   bool is_separator() const;

   Variant _value;
   SourceLocation loc;

   TokenType _type : 8;
   bool isInterpolationStart : 8;
   unsigned start : 32;

   static std::string TokensToString(const std::vector<Token> &tokens);
};


#endif //TOKEN_H
