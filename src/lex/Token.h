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
namespace lex {

namespace tok {

enum TokenType: unsigned short {
   __initial = 0,

#  define CDOT_TOKEN(Name, Spelling) \
      Name,
#  include "Tokens.def"
};

string tokenTypeToString(TokenType ty);

} // namespace tok

struct Token {
   Token();
   ~Token();
   Token(tok::TokenType type, Variant &&val,
         const SourceLocation &loc);

   tok::TokenType getKind() const { return _type; }

   Variant &getValue() { return _value; }
   Variant const& getValue() const { return _value; }

   bool isContextualKeyword(const string &kw) const
   {
      return _type == tok::ident && _value.strVal == kw;
   }

   std::string toString() const;
   std::string rawRepr() const;

   unsigned getOffset() const
   {
      return loc.getOffset();
   }

   const SourceLocation& getSourceLoc() const
   {
      return loc;
   }

   bool is(tok::TokenType ty) const { return _type == ty; }
   bool isNot(tok::TokenType ty) const { return !is(ty); }

   template<class ...Rest>
   bool oneOf(tok::TokenType ty, Rest... rest) const
   {
      if (_type == ty) return true;
      return oneOf(rest...);
   }

   bool oneOf(tok::TokenType ty) const { return is(ty); }

   bool is_punctuator() const;
   bool is_keyword() const;
   bool is_operator() const;
   bool is_identifier() const;
   bool is_literal() const;
   bool is_separator() const;

   bool is_directive() const;

   Variant _value;
   SourceLocation loc;

   tok::TokenType _type;
   static std::string TokensToString(const std::vector<Token> &tokens);
};

} // namespace lex
} // namespace cdot

#endif //TOKEN_H
