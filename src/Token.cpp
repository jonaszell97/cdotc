//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

Token::Token() {}

Token::Token(TokenType type, Variant&& content, int start, int end, bool escaped) :
   isEscaped_(escaped), _type(type), _value(content), start(start), end(end)
{

}

string Token::toString()
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

Variant Token::get_value() {
   return _value;
}

TokenType Token::get_type() {
   return _type;
}

bool Token::is_punctuator(char c) {
   return _type == T_PUNCTUATOR && _value.charVal == c;
}

bool Token::is_keyword(std::string keyword) {
   return _type == T_KEYWORD && _value.strVal == keyword;
}

bool Token::is_operator(std::string op) {
   return _type == T_OP && _value.strVal == op;
}

bool Token::is_separator() {
   return _type == T_PUNCTUATOR && (_value.intVal == '\n' || _value.intVal == ';');
}