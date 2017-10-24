//
// Created by Jonas Zell on 13.06.17.
//

#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <map>

#include "Variant/Variant.h"

namespace cdot {
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

   struct SourceLocation {
      SourceLocation() = default;
      SourceLocation(
         unsigned col,
         const unsigned& line,
         unsigned length,
         const unsigned& sourceId
      );

      unsigned col : 16;
      unsigned line : 16;
      unsigned length : 16;
      unsigned sourceId : 16;

      unsigned getLine() const;
      unsigned getCol() const;
      unsigned getLength() const;
      unsigned getSourceId() const;
   };
}

using std::string;
using namespace cdot;

struct Token {
   Token();
   Token(TokenType type, Variant &&val, SourceLocation loc,
      unsigned start, bool isEscaped = false);

   TokenType get_type();
   Variant get_value();

   string toString();

   unsigned getStart() const {
      return start;
   }

   unsigned getEnd() const {
      return start + loc.getLength();
   }

   unsigned getLine() const {
      return loc.getLine();
   }

   unsigned getCol() const {
      return loc.getCol();
   }

   const SourceLocation& getSourceLoc() const
   {
      return loc;
   }

   bool isEscaped() const
   {
      return isEscaped_;
   }

   void setIndent(int indent)
   {
      this->indent = indent;
   }

   unsigned getIndent() const
   {
      return indent;
   }

   bool isExpandedFromMacro() const
   {
      return expanded_from_macro;
   }

   void isExpandedFromMacro(bool exp)
   {
      expanded_from_macro = exp;
   }

   bool is_punctuator(char);
   bool is_keyword(const string &);
   bool is_operator(const string &);
   bool is_identifier(const string &);
   bool is_separator();

   bool isInterpolationStart = false;
   bool expanded_from_macro = false;

   Variant _value;
   TokenType _type;
   SourceLocation loc;

   unsigned start;
   unsigned indent = 0;

   bool isEscaped_ = false;
};


#endif //TOKEN_H
