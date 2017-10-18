//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

#include <iostream>

namespace cdot {
   void printBits(unsigned long long l, bool space = false) {
      string str;
      unsigned long long curr = 63;
      curr <<= 63;

      for (int i = 0; i < 64; ++i) {
         if (space && (i == 24 || i == 44 || i == 54)) {
            str += ' ';
         }

         str += (l & curr) ? "1" : "0";
         curr >>= 1;
      }

      std::cout << "0b" + str << std::endl;
   }

   SourceLocation::SourceLocation(
      unsigned col,
      const unsigned &line,
      unsigned length,
      const unsigned &sourceId) : col(col), line(line), length(length), sourceId(sourceId)
   {
//      assert(line < 1048576 && "line too high");
//      assert(sourceId < 16384 && "sourceid too high");
//
//      if (col >= 32768) {
//         col = 32767;
//      }
//
//      if (length >= 32768) {
//         length = 32768;
//      }
//
//      loc = 0;
//
//      // first 15 bits - column
//      loc |= col;
//      loc <<= 49;
//
//      // next 20 bits - line
//      loc |= (line << 29);
//
//      // next 15 bits - length
//      loc |= (length << 14);
//
//      // last 14 bits - source id
//      loc |= sourceId;
//
//      if (getLine() != line || getCol() != col || getLength() != length || getSourceId() != sourceId) {
//         printBits(col);
//         printBits(line);
//         printBits(length);
//         printBits(sourceId);
//         printBits(loc, true);
//         exit(0);
//      }
   }

   unsigned SourceLocation::getCol() const
   {
      return col; //(unsigned)(loc >> 49);
   }

   unsigned SourceLocation::getLine() const
   {
      return line; // (unsigned)((loc >> 29) & 1048575);
   }

   unsigned SourceLocation::getLength() const
   {
      return length; // (unsigned)((loc >> 14) & 32767);
   }

   unsigned SourceLocation::getSourceId() const
   {
      return sourceId; // (unsigned)(loc & 16383);
   }
}

Token::Token() = default;

Token::Token(TokenType type, Variant&& content, SourceLocation loc,
   unsigned start, bool escaped) :
   isEscaped_(escaped), _type(type), _value(content), start(start),
   loc(loc)
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