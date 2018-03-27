//
// Created by Jonas Zell on 13.06.17.
//

#include "Token.h"

#include "Basic/Variant.h"
#include "Basic/IdentifierInfo.h"
#include "Support/Format.h"

#include <iostream>
#include <llvm/ADT/SmallString.h>

using std::string;

namespace cdot {
namespace lex {

namespace tok {

string tokenTypeToString(TokenType kind)
{
   switch (kind) {
   case tok::ident:
      return "identifier";
   case tok::op_ident:
      return "operator";
   case tok::dollar_ident:
      return "$-identifier";
   case tok::dollar_dollar_ident:
      return "$$-identifier";
   case tok::percent_ident:
      return "%-identifier";
   case tok::percent_percent_ident:
      return "%%-identifier";
   case tok::charliteral:
      return "character literal";
   case tok::stringliteral:
      return "string literal";
   case tok::fpliteral:
      return "floating point literal";
   case tok::integerliteral:
      return "integer literal";
   default:
      return Token(kind).toString();
   }
}

} // namespace tok

using tok::TokenType;

template<unsigned N>
void Token::rawRepr(llvm::SmallString<N> &s) const
{
   if (kind == tok::space) {
      s += getText();
      return;
   }

   switch (kind) {
      case tok::charliteral:
      case tok::stringliteral:
         s += llvm::StringRef(reinterpret_cast<const char*>(Ptr) - 1,
                              Data + 2);
         break;
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling)                               \
      case tok::Name: s += (Spelling); break;
#  include "Tokens.def"
      default:
         toString(s);
         break;
   }
}

template<unsigned N>
void Token::toString(llvm::SmallString<N> &s) const
{
   switch (kind) {
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
      case tok::Name: s += (Spelling); return;
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: s += (Spelling); return;
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name: s += (Spelling); return;
#  define CDOT_CONTEXTUAL_KW_TOKEN(Name, Spelling) \
      case tok::Name: s += (Spelling); return;
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling)                               \
      case tok::Name: s += '\''; support::unescape_char((Spelling), s);       \
         s += '\''; return;
      case tok::sentinel: s += "<sentinel>"; return;
      case tok::eof: s += "<eof>"; return;
      case tok::expr_begin: s += "#{"; return;
      case tok::stringify_begin: s += "##{"; return;
      case tok::ident:
      case tok::op_ident:
         s += getIdentifier(); return;
      case tok::dollar_ident: {
         s += "$";
         s += getIdentifier();

         return;
      }
      case tok::dollar_dollar_ident: {
         s += "$$";
         s += getIdentifier();

         return;
      }
      case tok::percent_ident: {
         s += "%";
         s += getIdentifier();

         return;
      }
      case tok::percent_percent_ident: {
         s += "%%";
         s += getIdentifier();

         return;
      }
      case tok::charliteral:
         s += *reinterpret_cast<const char*>(Ptr);
         return;
      case tok::stringliteral:
      case tok::fpliteral:
      case tok::integerliteral:
         s += getText();
         return;
      case tok::preprocessor_value:
         s += getPreprocessorValue().toString();
         break;
      default:
         llvm_unreachable("unhandled token kind");

#  include "Tokens.def"
   }
}

template void Token::toString(llvm::SmallString<64>&) const;
template void Token::toString(llvm::SmallString<128>&) const;
template void Token::toString(llvm::SmallString<256>&) const;
template void Token::toString(llvm::SmallString<512>&) const;

template void Token::rawRepr(llvm::SmallString<64>&) const;
template void Token::rawRepr(llvm::SmallString<128>&) const;
template void Token::rawRepr(llvm::SmallString<256>&) const;
template void Token::rawRepr(llvm::SmallString<512>&) const;

bool Token::is(cdot::IdentifierInfo *II) const
{
   if (!is_identifier())
      return false;

   return getIdentifierInfo() == II;
}

bool Token::isIdentifier(llvm::StringRef str) const
{
   if (!is_identifier())
      return false;

   return getIdentifierInfo()->getIdentifier().equals(str);
}

bool Token::isIdentifierStartingWith(llvm::StringRef str) const
{
   if (!is_identifier())
      return false;

   return getIdentifierInfo()->getIdentifier().startswith(str);
}

bool Token::isIdentifierEndingWith(llvm::StringRef str) const
{
   if (!is_identifier())
      return false;

   return getIdentifierInfo()->getIdentifier().endswith(str);
}

llvm::StringRef Token::getIdentifier() const
{
   return getIdentifierInfo()->getIdentifier();
}

llvm::APInt Token::getIntegerValue() const
{
   assert(kind == tok::integerliteral);

   auto txt = getText();
   uint8_t base = 10;
   unsigned offset = 0;

   if (txt[0] == '0') {
      if (txt.size() > 1) {
         if (txt[1] == 'x' || txt[1] == 'X') {
            offset = 2;
            base = 16;
         }
         else if (txt[1] == 'b' || txt[1] == 'B') {
            offset = 2;
            base = 2;
         }
         else {
            offset = 1;
            base = 8;
         }
      }
   }

   llvm::StringRef str(txt.data() + offset, txt.size() - offset);
   return llvm::APInt(64, str, base);
}

string Token::toString() const
{
   llvm::SmallString<128> str;
   toString(str);

   return str.str();
}

string Token::rawRepr() const
{
   llvm::SmallString<128> str;
   rawRepr(str);

   return str.str();
}

bool Token::is_punctuator() const
{
   switch (kind) {
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
   switch (kind) {
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
   switch (kind) {
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
   switch (kind) {
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

bool Token::is_literal() const
{
   switch (kind) {
#  define CDOT_LITERAL_TOKEN(Name, Spelling) \
      case tok::Name:
#  include "Tokens.def"
         return true;
      default:
         return false;
   }
}

} // namespace lex
} // namespace cdot