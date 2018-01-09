//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

#include "../Variant/Type/IntegerType.h"
#include "../Variant/Type/FPType.h"

#include <iostream>

namespace cdot {
namespace lex {

namespace tok {

string tokenTypeToString(TokenType ty)
{
   switch (ty) {
#  define CDOT_TOKEN(Name, Spelling) \
      case Name:                     \
         return #Name;

#  include "Tokens.def"
      default:
         llvm_unreachable("bad token kind");
   }
}

} // namespace tok

using tok::TokenType;

Token::Token() {}
Token::~Token() = default;

Token::Token(TokenType type, Variant&& content, const SourceLocation &loc)
   : _value(std::move(content)), loc(loc), _type(type)
{

}

namespace {

string unescape_char(char c)
{
   switch (c) {
      case '\n':
         return "\\n";
      case '\a':
         return "\\a";
      case '\r':
         return "\\r";
      case '\v':
         return "\\v";
      case '\t':
         return "\\t";
      case '\b':
         return "\\b";
      case '\0':
         return "\\0";
      default:
         return string(1,c);
   }
}

} // anonymous namespace

string Token::toString() const
{
   switch (_type) {
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_CONTEXTUAL_KW_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling) \
      case tok::Name: return string("'") + unescape_char((Spelling)) + "'";
#  define CDOT_LITERAL_TOKEN(Name, Spelling) \
      case tok::Name: return _value.toString();
      case tok::sentinel: return "sentinel";
      case tok::eof: return "eof";
      case tok::ident: return _value.strVal;
      case tok::dollar_ident: return "$" + _value.strVal;
      case tok::dollar_dollar_ident: return "$$" + _value.strVal;
      case tok::percent_ident: return "%" + _value.strVal;
      case tok::percent_percent_ident: return "%%" + _value.strVal;
      default:
         llvm_unreachable("unhandled token kind");

#  include "Tokens.def"
   }
}

string Token::rawRepr() const
{
   if (_type == tok::space) {
      return string(_value.getInt(), ' ');
   }

   switch (_type) {
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_CONTEXTUAL_KW_TOKEN(Name, Spelling) \
      case tok::Name: return (Spelling);
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling) \
      case tok::Name: return string(1, (Spelling));
      case tok::integerliteral:
      case tok::fpliteral:
         return _value.toString();
      case tok::stringliteral:
         return '"' + _value.toString() + '"';
      case tok::charliteral:
         return '\'' + _value.toString() + '\'';
      case tok::ident: return _value.strVal;
      case tok::dollar_ident: return "$" + _value.strVal;
      case tok::dollar_dollar_ident: return "$$" + _value.strVal;
      case tok::percent_ident: return "%" + _value.strVal;
      case tok::percent_percent_ident: return "%%" + _value.strVal;
      case tok::expr_begin: return "<expr_begin>";
      case tok::stringify_begin: return "<stringify_begin>";
      case tok::eof: return "<eof>";
      case tok::sentinel: return "<sentinel>";
      default:
         llvm_unreachable("unhandled token kind");

#  include "Tokens.def"
   }
}

bool Token::is_punctuator() const
{
   switch (_type) {
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

bool Token::is_keyword() const
{
   switch (_type) {
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name:
#  define CDOT_MODULE_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

bool Token::is_operator() const
{
   switch (_type) {
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

bool Token::is_directive() const
{
   switch (_type) {
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

bool Token::is_identifier() const
{
   return _type == tok::ident;
}

bool Token::is_separator() const
{
   return oneOf(tok::newline, tok::semicolon, tok::eof);
}

bool Token::is_literal() const
{
   switch (_type) {
#  define CDOT_LITERAL_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

string Token::TokensToString(const std::vector<Token> &tokens)
{
   string str;
   for (const auto &tok : tokens) {
      str += tok._value.toString();
   }

   return str;
}

} // namespace lex
} // namespace cdot