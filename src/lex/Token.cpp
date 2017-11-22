//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

#include "../Variant/Type/IntegerType.h"
#include "../Variant/Type/FPType.h"

#include <iostream>

Token::Token() : isInterpolationStart(false) {}
Token::~Token() = default;

Token::Token(TokenType type, Variant&& content, SourceLocation loc,
   unsigned start) : _type(type), _value(content), start(start), loc(loc),
                     isInterpolationStart(false)
{

}

string Token::toString() const
{
   switch (_type) {
      case T_BOF: return "T_BOF";
      case T_EOF: return "T_EOF";
      case T_IDENT: return "T_IDENT";
      case T_PREPROC_VAR: return "T_PREPROC_VAR";
      case T_LITERAL: return "T_LITERAL";
      case T_KEYWORD: return "T_KEYWORD";
      case T_OP: return "T_OP";
      case T_DIRECTIVE: return "T_DIRECTIVE";
      case T_PUNCTUATOR: return "T_PUNCTUATOR";
   }

   llvm_unreachable("Unknown token type");
}

Variant Token::get_value()
{
   return _value;
}

TokenType Token::get_type()
{
   return _type;
}

bool Token::is_punctuator(char c) const
{
   if (c == '<') {
      return _type == T_OP && _value.strVal == "<";
   }
   if (c == '>') {
      return _type == T_OP && _value.strVal == ">";
   }

   return _type == T_PUNCTUATOR && _value.intVal == c;
}

bool Token::is_punctuator() const
{
   return _type == T_PUNCTUATOR;
}

bool Token::is_keyword(const string &keyword) const
{
   return _type == T_KEYWORD && _value.strVal == keyword;
}

bool Token::is_keyword() const
{
   return _type == T_KEYWORD;
}

bool Token::is_operator(const string &op) const
{
   return _type == T_OP && _value.strVal == op;
}

bool Token::is_operator() const
{
   return _type == T_OP;
}

bool Token::is_identifier(const string &ident) const
{
   return _type == T_IDENT && _value.strVal == ident;
}

bool Token::is_identifier() const
{
   return _type == T_IDENT;
}

bool Token::is_separator() const
{
   return (_type == T_PUNCTUATOR
            && (_value.intVal == '\n' || _value.intVal == ';'))
          || _type == T_EOF;
}

string Token::TokensToString(const std::vector<Token> &tokens)
{
   string str;
   unsigned i = 0;
   auto numTokens = tokens.size();

   for (const auto &tok : tokens) {
      str += tok._value.toString();
   }

   return str;
}