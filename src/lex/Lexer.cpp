//
// Created by Jonas Zell on 18.06.17.
//

#include "Lexer.h"

#include <cassert>
#include <regex>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

#ifndef CDOT_LEXER_STANDALONE
#include "../Message/Diagnostics.h"
#include "../Support/Casting.h"
#include "../Variant/Type/IntegerType.h"

#endif

using cdot::lex::Token;

namespace cdot {
namespace lex {

const char* LexerTraits::CommentBegin = "//";
const char* LexerTraits::CommentEnd = "\n";

const char* LexerTraits::MultiLineCommentBegin = "/*";
const char* LexerTraits::MultiLineCommentEnd = "*/";

tok::TokenType LexerTraits::getKeywordToken(const string &potentialKeyword)
{
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
   if ((Spelling) == potentialKeyword) return tok::Name;
#  define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      CDOT_KEYWORD_TOKEN(Name, Spelling)
#  include "Tokens.def"

   return tok::sentinel;
}

tok::TokenType LexerTraits::getOperatorToken(const string &potentialOperator)
{
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
   if ((Spelling) == potentialOperator) return tok::Name;
#  include "Tokens.def"

   return tok::sentinel;
}

tok::TokenType LexerTraits::getPunctuatorToken(char potentialPunctuator)
{
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling) \
   if ((Spelling) == potentialPunctuator) return tok::Name;
#  include "Tokens.def"

   return tok::sentinel;
}

template <class T>
Lexer<T>::Lexer(llvm::MemoryBuffer *buf,
                unsigned sourceId)
   : typeID(LexerID),
     srcLen(buf->getBufferSize()),
     sourceId(sourceId),
     curr(buf->getBufferStart()),
     begin(buf->getBufferStart()),
     ignoreMacro(false), ignoreDirective(false), ignoreValue(false),
     doneLexing(false), ignoreInterpolation(false)
{

}

template <class T>
Lexer<T>::Lexer(std::vector<Token> &&toks)
   : typeID(LexerID),
     srcLen(0),
     tokenIndex(1),
     ignoreMacro(false),
     ignoreDirective(false),
     ignoreValue(false),
     doneLexing(true), ignoreInterpolation(false)
{
   for (auto &tok : toks) {
      tokens.push_back(getNextToken(tok));
   }

   tokenIndex = 1;
   doneLexing = true;
}

template <class T>
MacroExpansionLexer<T>::MacroExpansionLexer(const Macro &M,
                                            Super *Outer, unsigned offset,
                                            MacroArgList &macroArgs)
   : Super(Super::TypeID::MacroExpansionLexerID), prelexedTokens(M.tokens)
{
   this->Outer = Outer;
   this->expandedFromMacro = offset;

   if (Outer) {
      this->macroExpansionDepth = Outer->getMacroExpansionDepth() + 1;
      this->sourceId = Outer->getSourceId();
      this->currentLine = Outer->currentLine;
   }

   auto numArgs = macroArgs.first.size();
   size_t i = 0;
   for (; i < numArgs; ++i) {
      this->Macros.try_emplace(M.args[i], macroArgs.first[i]);
      this->Values.try_emplace(M.args[i], move(macroArgs.second[i]));
   }
}

template<class T>
MacroExpansionLexer<T>::MacroExpansionLexer(typename Super::TokenVec &&tokens,
                                            unsigned offset,
                                            Super *Outer)
   : Super(Super::TypeID::MacroExpansionLexerID),
     prelexedTokens(std::move(tokens))
{
   this->Outer = Outer;
   this->expandedFromMacro = offset;

   if (Outer) {
      this->macroExpansionDepth = Outer->getMacroExpansionDepth() + 1;
      this->sourceId = Outer->getSourceId();
      this->currentLine = Outer->currentLine;
   }
}

template <class T>
Lexer<T>::Lexer(TypeID id)
   : typeID(id),
     curr(nullptr),
     ignoreMacro(false),
     ignoreDirective(false),
     ignoreValue(false),
     doneLexing(false),
     ignoreInterpolation(false)
{

}

template<class T>
void Lexer<T>::lex()
{
   if (doneLexing)
      return;

   for (;;) {
      auto tok = getNextToken();

      if (!tok.is(tok::sentinel)) {
         tokens.push_back(std::move(tok));
      }

      if (tokens.back().is(tok::eof)) {
         break;
      }
   }

   tokenIndex = 1;
   doneLexing = true;
}

// RAII utility classes


template<class T>
void Lexer<T>::printTokensTo(llvm::raw_ostream &out)
{
   for (const auto& tok : tokens)
      out << tok.rawRepr();

   out << "\n";
}

template<class T>
Lexer<T>::ModeScopeGuard::ModeScopeGuard(Mode m, Lexer *lex)
   : lex(lex), prevMode(lex->mode)
{
   lex->setMode(m);
}

template<class T>
Lexer<T>::ModeScopeGuard::~ModeScopeGuard()
{
   lex->setMode(prevMode);
}

template<class T>
Lexer<T>::StateSaveGuard::StateSaveGuard(Lexer *lex)
   : lex(lex), shrinkTo(lex->tokenIndex)
{
   assert(shrinkTo);
}

template<class T>
Lexer<T>::StateSaveGuard::~StateSaveGuard()
{
   if (lex->tokens.size() < shrinkTo) {
      return;
   }

   lex->tokenIndex = shrinkTo;
}

template<class T>
void Lexer<T>::reset(const char *src, size_t len)
{
   this->curr = src;
   currentIndex = 0;
   srcLen = len;
   doneLexing = false;
   tokens.clear();
}

template<class T>
string Lexer<T>::strVal()
{
   if (currentTok().getValue().strVal.empty()
       && currentTok().getKind() != tok::stringliteral) {
      CDOT_DIAGNOSE("Unexpected token " + currentTok().toString());
   }

   return currentTok()._value.strVal;
}

template<class T>
string& Lexer<T>::strRef()
{
   return tokens[tokenIndex - 1]._value.strVal;
}

/**
 * Decides wether or not a character could be part of a number token.
 * @param _c
 * @param allowDecimalPoint Whether to allow a decimal point or not
 * @return
 */
template<class T>
bool Lexer<T>::is_number(char c, bool allowDecimalPoint) {
   // digits and dot
   return (c >= '0' && c <= '9') || (allowDecimalPoint && c == '.');
}

template<class T>
bool Lexer<T>::is_hex(char c) {
   return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')
          || (c >= '0' && c <= '9');
}

/**
 * Decides whether or not a character could be part of a keyword or identifier
 * token.
 * @param _c
 * @return
 */
template<class T>
bool Lexer<T>::is_identifier_char(char c) {
   // valid identifiers contain ANYTHING (almost)
   return !is_operator_char(c) && !is_punctuator(c) && c != ' ' && c != '\0';
}

namespace {

std::regex validOp(R"_(([\+\-\*/%&\|!=<>~\^,\?:]|\.\.)+)_");

template<class T>
bool in_vector(const std::vector<T> &vec, const T &el)
{
   return std::find(vec.begin(), vec.end(), el) != vec.end();
}

bool matches(const string &pattern, const string &subject)
{
   return std::regex_match(subject, std::regex(pattern));
}

} // anonymous namespace

template<class T>
bool Lexer<T>::is_operator_char(char c) {
   if (in_vector(util::operator_chars, c)) {
      return true;
   }

   return false;
}

template<class T>
bool Lexer<T>::is_punctuator(char c)
{
   return in_vector(util::punctuators, c);
}

/**
 * Gets the next character in the program
 * @return
 */
template<class T>
char Lexer<T>::get_next_char()
{
   ++currentIndex;
   return *(curr++);
}

template<class T>
char Lexer<T>::char_lookahead()
{
   return *curr;
}

/**
 * Backtracks to the last token
 */
template<class T>
void Lexer<T>::backtrack()
{
   --tokenIndex;
   if (currentTok().is(tok::space))
      backtrack();
}

/**
 * Backtracks by the passed amount of characters
 * @param length
 */
template<class T>
void Lexer<T>::backtrack_c(int length)
{
   currentIndex -= length;
   curr -= length;
}

template<class T>
char Lexer<T>::escape_char(char c)
{
   switch (c) {
      case 'n':
         return '\n';
      case 'a':
         return '\a';
      case 'r':
         return '\r';
      case 'v':
         return '\v';
      case 't':
         return '\t';
      case 'b':
         return '\b';
      case '"':
         return '\"';
      case '\'':
         return '\'';
      case '0':
         return '\0';
      default:
         return c;
   }
}

template<class T>
string Lexer<T>::unescape_char(char c)
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
         return string(1, c);
   }
}

template<class T>
SourceLocation Lexer<T>::getSourceLoc()
{
   if (doneLexing)
      return currentTok().getSourceLoc();

   if (auto MA = support::dyn_cast<MacroExpansionLexer<T>>(this))
      return MA->prelexedTokens[MA->lexedIndex - 1].getSourceLoc();

   return SourceLocation(currentIndex, sourceId);
}

template<class Traits>
Token Lexer<Traits>::getNextToken(bool ignoreNewline)
{
   return getNextToken(lex_next_token(), ignoreNewline);
}

template<class Traits>
Token Lexer<Traits>::getNextToken(const Token &tok, bool ignoreNewline)
{
   switch (tok.getKind()) {
      case tok::ident: {
         auto &ident = tok._value.strVal;
         if (hasMacro(ident)) {
            auto toks = expand_macro(ident);
            tokens.insert(tokens.end(),
                          std::make_move_iterator(toks.begin()),
                          std::make_move_iterator(toks.end()) - 1);

            pp_finish();
            return getNextToken(ignoreNewline);
         }
         else {
            auto it = BuiltinMacros.find(ident);
            if (it != BuiltinMacros.end()) {
               auto exprTok = handlePreprocFunc(ident);
               pp_finish();

               return exprTok;
            }
         }

         return tok;
      }
      case tok::dollar_ident: {
         auto &ident = tok._value.getString();
         auto M = tryGetLispMacro(ident);

         if (!M) {
            currentIndex = tok.getOffset();
            CDOT_DIAGNOSE("undeclared macro " + ident);
         }

         auto toks = expand_lisp_macro(M, tok.getSourceLoc());
         tokens.insert(tokens.end(),
                       std::make_move_iterator(toks.begin()),
                       std::make_move_iterator(toks.end()));

         pp_finish();
         return getNextToken(ignoreNewline);
      }
      case tok::expr_begin: {
         pp_advance();

         auto loc = tok.getSourceLoc();
         auto val = valueToToken(loc, parseExpression());

         pp_finish();

         return val;
      }
      case tok::stringify_begin: {
         pp_advance();

         auto loc = currentTok().getSourceLoc();
         auto expr = valueToToken(loc, parseExpression());

         expr._value = expr._value.castTo(VariantType::STRING);
         expr._type = tok::stringliteral;

         pp_finish();

         return expr;
      }
#     define CDOT_POUND_KEYWORD_TOKEN(Name, Spelling) \
      case tok::Name:

#     include "Tokens.def"
      {
         if (tok.getKind() == tok::pound_include) {
            auto toks = handle_include();
            tokens.insert(tokens.end(),
                          std::make_move_iterator(toks.begin()),
                          std::make_move_iterator(toks.end()) - 1);
         }
         else {
            handle_directive(tok.getKind());
         }

         pp_finish();

         return getNextToken(ignoreNewline);
      }
      default:
         return tok;
   }
}

/**
 * Parses the next token
 * @return
 */
template<class Traits>
Token Lexer<Traits>::lex_next_token(bool preprocessing)
{
   if (typeID == MacroExpansionLexerID) {
      return static_cast<MacroExpansionLexer<Traits>*>(this)
         ->lex_next_token();
   }
   if (currentIndex >= srcLen) {
      return makeToken(tok::eof, {}, currentIndex);
   }

   string t;
   size_t startOffset = currentIndex;
   char first;
   char next;

   first = get_next_char();
   if (skipCommentIfPresent(first)) {
      return makeToken(tok::newline, {}, startOffset);
   }

   if (first == ' ') {
      size_t numSpaces = 1;
      while (char_lookahead() == ' ') {
         (void)get_next_char();
         ++numSpaces;
      }

      return makeToken(tok::space, Variant(IntegerType::getUnsigned(),
                                           numSpaces), startOffset);
   }

   if (first == '\\') {
      if (char_lookahead() == '\n') {
         get_next_char();
      }

      return lex_next_token();
   }

   if (currentIndex > srcLen) {
      return makeToken(tok::eof, { }, startOffset);
   }

   // preprocessor variable
   if (first == '#') {
      // preprocessor expression
      next = char_lookahead();
      if (next == '{') {
         get_next_char();

         return makeToken(tok::expr_begin, {}, startOffset);
      }
      // stringify operator
      else if (next == '#') {
         get_next_char();
         get_next_char();

         return makeToken(tok::stringify_begin, {}, startOffset);
      }
      // directive
      else {
         t += '#';
         next = get_next_char();

         while (next == ' ') {
            next = get_next_char();
         }

         while (next != ' ' && next != '\n' && next != '\0') {
            t += next;
            next = get_next_char();
         }

         backtrack_c(1);

         auto kind = Traits::getKeywordToken(t);
         return makeToken(kind, {}, startOffset);
      }
   }

   // character literal
   if (first == '\'') {
      next = get_next_char();
      if (next == '\\') {
         next = escape_char(get_next_char());
      }

      char _c = get_next_char();
      if (_c != '\'') {
         CDOT_DIAGNOSE("Expected \"'\" after character literal");
      }

      return makeToken(tok::charliteral, { next }, startOffset);
   }

   // string literal
   if (first == '\"') {
      return lexStringLiteral(startOffset, preprocessing);
   }

   // escape sequence
   if (first == '`') {
      next = get_next_char();
      while (next != '`' && currentIndex < srcLen) {
         t += next;
         next = get_next_char();

      }

      return makeToken(tok::ident, { std::move(t) }, startOffset);
   }

   // dollar identifier
   if (first == '$') {
      tok::TokenType kind = tok::dollar_ident;
      if (char_lookahead() == '$') {
         kind = tok::dollar_dollar_ident;
         get_next_char();
      }

      while (is_identifier_char(char_lookahead()) || char_lookahead() == '.')
         t += get_next_char();

      return makeToken(kind, { std::move(t) }, startOffset);
   }

   // percent identifier
   if (first == '%') {
      tok::TokenType kind = tok::percent_ident;
      if (char_lookahead() == '%') {
         kind = tok::percent_percent_ident;
         get_next_char();
      }

      while (is_identifier_char(char_lookahead()) || char_lookahead() == '.')
         t += get_next_char();

      if (!t.empty())
         return makeToken(kind, { std::move(t) }, startOffset);
   }

   // numeric literal (decimal, octal, hexadecimal or binary; with or without
   // exponent or floating point)
   if (is_number(first, false)) {
      char _pref = get_next_char();
      if (first == '0' && (_pref == 'x' || _pref == 'X')) {
         // hexadecimal literal
         string hex_s = "0x";
         next = get_next_char();
         while (is_hex(next)) {
            hex_s += next;
            next = get_next_char();
         }

         backtrack_c(1);
         return makeToken(tok::integerliteral,
                          Variant(IntegerType::get(),
                                  std::stoull(hex_s, nullptr, 16)),
                          startOffset);
      }

      if (first == '0' && (_pref == 'b' || _pref == 'B')) {
         // binary literal
         string bin_s = "";
         next = get_next_char();
         while (next == '0' || next == '1') {
            bin_s += next;
            next = get_next_char();
         }

         if (bin_s.empty()) {
            bin_s = "0";
         }

         backtrack_c(1);
         return makeToken(tok::integerliteral,
                          Variant(IntegerType::get(),
                                  std::stoull(bin_s, nullptr, 2)),
                          startOffset);
      }

      backtrack_c(1);

      // advance integer or floating point number
      next = first;
      bool decimal_point = false;
      while ((next >= '0' && next <= '9') || next == '_' || next == '.') {
         if (next == '.') {
            // tuple member access
            if (mode == MemberAccess) {
               backtrack_c(1);
               return makeToken(tok::integerliteral,
                                Variant(IntegerType::get(), std::stoull(t)),
                                startOffset);
            }

            if (char_lookahead() < '0' || char_lookahead() > '9'
                || decimal_point) {
               break;
            }
            decimal_point = true;
         }

         if (next != '_') {
            t += next;
         }
         next = get_next_char();
      }

      // octal literal
      if (matches("0[0-7]+", t)) {
         backtrack_c(1);
         return makeToken(tok::integerliteral,
                          Variant(IntegerType::get(),
                                  std::stoull(t, nullptr, 8)),
                          startOffset);
      }

      // exponent
      if (next == 'e' || next == 'E') {
         std::string _exp = "";
         next = get_next_char();
         while (next == '-' || is_number(next, false)) {
            _exp += next;
            next = get_next_char();
         }

         backtrack_c(1);
         double base = std::stod(t);
         double exp = pow(10, std::stoull(_exp));

         return makeToken(tok::fpliteral, { base * exp }, startOffset);
      }

      backtrack_c(1);

      if (!decimal_point) {
         next = char_lookahead();

         bool isUnsigned = false;
         bool isIntegral = true;
         bool isFloat = false;
         unsigned short bitwidth = sizeof(long long) * 8;
         Variant val;

         while (1) {
            bool isModifier = true;
            switch (tolower(next)) {
               case 'u':
                  isUnsigned = true;
                  break;
               case 'l':
                  break;
               case 's':
                  bitwidth = 16;
                  break;
               case 'd':
                  isIntegral = false;
                  break;
               case 'f':
                  bitwidth = 32;
                  isFloat = true;
                  isIntegral = false;
                  break;
               default:
                  isModifier = false;
                  break;
            }

            if (!isModifier)
               break;

            get_next_char();
            next = char_lookahead();
         }

         if (isIntegral && isFloat)
            CDOT_DIAGNOSE("incompatible modifiers");
         if (!isIntegral && isUnsigned)
            CDOT_DIAGNOSE("incompatible modifiers");

         if (isIntegral) {
            val = Variant(IntegerType::get(bitwidth, isUnsigned),
                          std::stoull(t));
         }
         else {
            val = Variant(std::stod(t));
         }

         tok::TokenType kind = val.isInt()
                               ? tok::integerliteral
                               : tok::fpliteral;

         return makeToken(kind, std::move(val), startOffset);
      }
      else {
         next = char_lookahead();
         if (next == 'f' || next == 'F') {
            get_next_char();
            return makeToken(tok::fpliteral, { std::stof(t) }, startOffset);
         }
         else if (next == 'd' || next == 'D') {
            get_next_char();
         }

         return makeToken(tok::fpliteral, { std::stod(t) }, startOffset);
      }
   }

   if (is_operator_char(first)) {
      bool lastWasPeriod = first == '.';

      while (is_operator_char(first)) {
         t += first;
         first = get_next_char();
         if ((lastWasPeriod && first != '.')
             || (!lastWasPeriod && first == '.')) {
            break;
         }

         lastWasPeriod = first == '.';
      }

      backtrack_c(1);

      if (t == ".") {
         return makeToken(tok::period, {}, startOffset);
      }

//      while (!is_operator(t) && t.length() > 0) {
//         t.pop_back();
//         backtrack_c(1);
//      }

      auto kind = Traits::getOperatorToken(t);
      if (kind != tok::sentinel)
         return makeToken(kind, {}, startOffset);

      return makeToken(tok::ident, { move(t) }, startOffset);
   }

   auto kind = Traits::getPunctuatorToken(first);
   if (kind != tok::sentinel)
      return makeToken(kind, {}, startOffset);

   // advance keyword or identifier
   t += first;
   next = first;
   while (is_identifier_char(char_lookahead())) {
      t += get_next_char();
   }

   if ((char_lookahead() == '?' || char_lookahead() == '!') && t == "as") {
      t += get_next_char();
   }

   kind = Traits::getOperatorToken(t);
   if (kind != tok::sentinel) {
      return makeToken(kind, {}, startOffset);
   }

   kind = Traits::getKeywordToken(t);
   if (kind != tok::sentinel) {
      return makeToken(kind, {}, startOffset);
   }

   return makeToken(tok::ident, { std::move(t) }, startOffset);
}

template<class T>
Token Lexer<T>::lexStringLiteral(unsigned startIndex, bool preprocessing)
{
   string s;
   llvm::SmallVector<Token, 8> tokens;
   char c = get_next_char();

   while (1) {
      if (c == '"')
         break;
      if (currentIndex >= srcLen)
         CDOT_DIAGNOSE("unexpected end of file, expecting '\"'");

      if (c == '\\') {
         auto next = get_next_char();
         s += escape_char(next);
      }
      else if (c == '$' && !ignoreInterpolation) {
         tokens.push_back(makeToken(tok::stringliteral, { move(s) },
                                    startIndex));
         s.clear();

         tokens.push_back(makeToken(tok::sentinel, {},
                                    startIndex));

         if (char_lookahead() == '{') {
            get_next_char();
            unsigned openParens = 1;
            unsigned closeParens = 0;
            size_t numTokens = 0;

            while (1) {
               auto tok = lex_next_token();
               switch (tok.getKind()) {
                  case tok::open_brace:
                     ++openParens; break;
                  case tok::close_brace:
                     ++closeParens; break;
                  default:
                     break;
               }

               if (openParens == closeParens)
                  break;

               ++numTokens;
               tokens.push_back(std::move(tok));
            }
         }
         else {
            tokens.push_back(lex_next_token());
         }

         tokens.push_back(makeToken(tok::sentinel, {},
                                    startIndex));
      }
      else {
         s += c;
      }

      c = get_next_char();
   }

   if (preprocessing)
      preprocessingTokens.insert(preprocessingTokens.end(),
                                 std::make_move_iterator(tokens.begin()),
                                 std::make_move_iterator(tokens.end()));
   else
      this->tokens.insert(this->tokens.end(),
                          std::make_move_iterator(tokens.begin()),
                          std::make_move_iterator(tokens.end()));

   return makeToken(tok::stringliteral, { move(s) }, startIndex);
}


template <class T>
Token MacroExpansionLexer<T>::lex_next_token()
{
   if (lexedIndex >= prelexedTokens.size()) {
      return Token(tok::eof, {}, this->tokens.empty()
                                 ? SourceLocation()
                                 : this->tokens.back().getSourceLoc());
   }

   return prelexedTokens[lexedIndex++];
}


template<class T>
void Lexer<T>::lexUntilClosingBrace()
{
   unsigned openBraces = 1;
   unsigned closeBraces = 0;

   while (1) {
      auto tok =lex_next_token();
      switch (tok.getKind()) {
         case tok::open_brace:
            ++openBraces;
            break;
         case tok::close_brace:
            ++closeBraces;
            break;
         case tok::eof:
            CDOT_DIAGNOSE("unexpected end of file, expecting '}'");
            break;
         default:
            tokens.push_back(std::move(tok));
            break;
      }

      if (openBraces == closeBraces)
         break;
   }
}

template<class T>
void Lexer<T>::lexUntilClosingParen(unsigned beginAt)
{
   unsigned openParens = beginAt;
   unsigned closeParens = 0;

   while (1) {
      auto tok = lex_next_token();
      switch (tok.getKind()) {
         case tok::open_paren:
            ++openParens;
            break;
         case tok::close_paren:
            ++closeParens;
            break;
         case tok::eof:
            CDOT_DIAGNOSE("unexpected end of file, expecting ')'");
            break;
         default:
            break;
      }

      tokens.push_back(std::move(tok));

      if (openParens & (openParens == closeParens))
         break;
   }
}

template<class T>
void Lexer<T>::lexUntil(tok::TokenType type)
{
   while (1) {
      tokens.push_back(lex_next_token());
      auto &tok = tokens.back();

      if (tok.getKind() == tok::backslash)
         lex_next_token();
      else if (tok.oneOf(type, tok::eof))
         break;
   }
}

template<class T>
Token Lexer<T>::makeToken(tok::TokenType ty,
                          Variant &&val,
                          size_t start) {
   if (ty == tok::newline)
      ++currentLine;

   return Token(ty, std::move(val),
                SourceLocation(expandedFromMacro ? expandedFromMacro + offset
                                                 : start + offset, sourceId));
}

template<class T>
void Lexer<T>::expect_impl(tok::TokenType ty)
{
   if (currentTok()._type != ty) {
      CDOT_DIAGNOSE("unexpected token " + currentTok().toString());
   }
}

template<class T>
void Lexer<T>::advance(bool ignore_newline, bool significantWhiteSpace)
{
   assert(tokenIndex < tokens.size() && "advancing past the end of the file!");

   Token &t = tokens[tokenIndex++];
   if ((t.is(tok::newline) && ignore_newline)
       || (t.is(tok::space) && !significantWhiteSpace)
       || t.is(tok::backslash)) {
      return advance(ignore_newline, significantWhiteSpace);
   }
}

template<class T>
Token Lexer<T>::lookahead(bool ignore_newline, bool sw,
                          size_t offset) {
   if(tokenIndex + offset >= tokens.size())
      return tokens.back();

   auto &tok = tokens[tokenIndex + offset];
   if ((tok.is(tok::newline) && ignore_newline)
       || (tok.is(tok::space) && !sw)
       || tok.oneOf(tok::backslash)) {
      return lookahead(ignore_newline, sw, offset + 1);
   }

   return tok;
}

template<class T>
void Lexer<T>::pp_advance(bool ignoreNewline, bool sw)
{
   if (doneLexing)
      return advance(ignoreNewline, sw);

   while (ppTokenIndex >= preprocessingTokens.size()) {
      preprocessingTokens.push_back(lex_next_token(true));
   }

   ++ppTokenIndex;
   auto &tok = ppCurrentTok();

   if ((ignoreNewline && tok.is(tok::newline))
        || (!sw && tok.is(tok::space))) {
      return pp_advance(ignoreNewline, sw);
   }
}

template<class T>
void Lexer<T>::pp_backtrack()
{
   if (doneLexing)
      return backtrack();

   --ppTokenIndex;
}

template<class T>
Token &Lexer<T>::ppCurrentTok()
{
   if (doneLexing)
      return tokens[tokenIndex - 1];

   assert(preprocessingTokens.size() > ppTokenIndex - 1);
   return preprocessingTokens[ppTokenIndex - 1];
}

template<class T>
Token Lexer<T>::pp_lookahead(bool ignoreNewline, unsigned offset)
{
   if (doneLexing)
      return lookahead(ignoreNewline);

   while (ppTokenIndex + offset >= preprocessingTokens.size()) {
      preprocessingTokens.push_back(lex_next_token());
   }

   auto tok = preprocessingTokens[ppTokenIndex + offset];
   if ((ignoreNewline && tok.is(tok::newline))
       || tok.is(tok::space)) {
      return pp_lookahead(ignoreNewline, offset + 1);
   }

   return tok;
}

template<class T>
void Lexer<T>::pp_finish()
{
   if (doneLexing)
      return;

   if (ppTokenIndex < preprocessingTokens.size()) {
      for (auto i = ppTokenIndex; i < preprocessingTokens.size(); ++i)
         if (preprocessingTokens[i].is(tok::newline))
            --currentLine;

      if (typeID == MacroExpansionLexerID) {
         auto self = static_cast<MacroExpansionLexer<T>*>(this);
         self->lexedIndex -= (preprocessingTokens.size() - ppTokenIndex);
      }
      else {
         auto &last = preprocessingTokens[ppTokenIndex];
         currentIndex = last.getSourceLoc().getOffset();
         curr = begin + currentIndex;
      }
   }

   ppTokenIndex = 0;
   preprocessingTokens.clear();
}

template<class T>
bool Lexer<T>::hasMacro(llvm::StringRef name)
{
   if (Macros.find(name) != Macros.end())
      return true;

   if (Outer)
      return Outer->hasMacro(name);

   return false;
}

template<class T>
typename Lexer<T>::Macro &Lexer<T>::getMacro(llvm::StringRef name)
{
   auto current = this;
   while (1) {
      auto it = current->Macros.find(name);
      if (it != current->Macros.end())
         return it->second;

      current = current->Outer;
      assert(current && "call hasMacro first!");
   }
}

/**
 * Skips to the end of a comment
 */
template<class Traits>
void Lexer<Traits>::ignore_comment(const char *End)
{
   while (true) {
      if (currentIndex >= srcLen)
         return;

      auto c = char_lookahead();

      if (c == End[0]) {
         size_t i = 0;
         bool isEnd = true;

         while (End[i] != '\0') {
            if (End[i] != c) {
               isEnd = false;
               break;
            }

            get_next_char();
            c = char_lookahead();
            ++i;
         }

         if (isEnd) {
            return;
         }
      }

      get_next_char();
   }
}

template<class T>
void Lexer<T>::skip_until_even(ParenKind kind)
{
   tok::TokenType open;
   tok::TokenType close;
   switch (kind) {
      case PAREN:
         open = tok::open_paren;
         close = tok::close_paren;
         break;
      case BRACE:
         open = tok::open_brace;
         close = tok::close_brace;
         break;
      case ANGLED:
         open = tok::smaller;
         close = tok::greater;
         break;
      case SQUARE:
         open = tok::open_square;
         close = tok::close_square;
         break;
   }

   if (!currentTok().is(open)) {
      return;
   }

   advance();

   unsigned openCount = 1;
   unsigned closeCount = 0;

   for (;;) {
      if (currentTok().is(open)) {
         ++openCount;
      }
      else if (currentTok().is(close)) {
         ++closeCount;
      }

      if (openCount == closeCount) {
         break;
      }

      advance();
   }
}

template<class Traits>
bool Lexer<Traits>::skipCommentIfPresent(char first)
{
   // escaped newline
   if (first == '\\' && char_lookahead() == '\n') {
      get_next_char();
      first = get_next_char();
   }

   size_t i = 0;
   char c = first;

   bool isSingleLineComment = false;
   while (Traits::CommentBegin[i] != '\0' && c == Traits::CommentBegin[i]) {
      if (currentIndex >= srcLen)
         return false;

      c = get_next_char();
      ++i;

      if (Traits::CommentBegin[i] == '\0') {
         isSingleLineComment = true;
         break;
      }
   }

   if (isSingleLineComment) {
      ignore_comment(Traits::CommentEnd);
      return true;
   }
   else {
      backtrack_c(i);
      c = first;
   }

   i = 0;
   bool isMultiLineComment = false;
   while (Traits::MultiLineCommentBegin[i] != '\0'
          && c == Traits::MultiLineCommentBegin[i]) {
      if (currentIndex >= srcLen)
         return false;

      c = get_next_char();
      ++i;

      if (Traits::MultiLineCommentBegin[i] == '\0') {
         isMultiLineComment = true;
         break;
      }
   }

   if (isMultiLineComment) {
      ignore_comment(Traits::MultiLineCommentEnd);
      return true;
   }
   else {
      backtrack_c(i);
   }

   return false;
}

template class Lexer<LexerTraits>;
template class Lexer<module::ModuleLexerTraits>;

template class MacroExpansionLexer<LexerTraits>;
template class MacroExpansionLexer<module::ModuleLexerTraits>;

} // namespace lex

namespace module {

using namespace cdot::lex;

tok::TokenType
ModuleLexerTraits::getKeywordToken(const string &potentialKeyword)
{
#  define CDOT_KEYWORD_TOKEN(Name, Spelling) \
      if (potentialKeyword == (Spelling)) return tok::Name;
#  include "Tokens.def"

   return tok::sentinel;
}

tok::TokenType
ModuleLexerTraits::getOperatorToken(const string &potentialOperator)
{
#  define CDOT_OPERATOR_TOKEN(Name, Spelling) \
      if (potentialOperator == (Spelling)) return tok::Name;
#  include "../lex/Tokens.def"

   return tok::sentinel;
}

tok::TokenType
ModuleLexerTraits::getPunctuatorToken(char potentialPunctuator)
{
#  define CDOT_PUNCTUATOR_TOKEN(Name, Spelling) \
      if (potentialPunctuator == (Spelling)) return tok::Name;
#  include "../lex/Tokens.def"

   return tok::sentinel;
}

const char *ModuleLexerTraits::CommentBegin = "//";
const char *ModuleLexerTraits::CommentEnd = "\n";

const char *ModuleLexerTraits::MultiLineCommentBegin = "";
const char *ModuleLexerTraits::MultiLineCommentEnd = "";

} // namespace module

namespace util {

llvm::StringMap<int> op_precedence {
   { "?",     -1 },
   { ":",     -1 },
   { "=",     0 },
   { "+=",    0 },
   { "-=",    0 },
   { "*=",    0 },
   { "/=",    0 },
   { "<<=",   0 },
   { ">>=",   0 },
   { "^=",    0 },
   { "&=",    0 },
   { "|=",    0 },
   { "??",    1 },
   { "||",    1 },
   { "&&",    2 },
   { "|",     3 },
   { "&",     4 },
   { "^",     4 },
   { "!=",    5 },
   { "!==",   5 },
   { "==",    5 },
   { "===",   5 },
   { "<=",    6 },
   { ">=",    6 },
   { "<",     6 },
   { ">",     6 },
   { "<<",    7 },
   { ">>",    7 },
   { ">>>",   7 },
   { "+",     8 },
   { "-",     8 },
   { "*",     9 },
   { "/",     9 },
   { "%",     9 },
   { "**",    10 },
   { "as",    11 },
   { "as!",   11 },
   { "isa",   11 },
   { "..",    12 },
   { "infix", 13 }
};

std::vector<char> operator_chars = {
   '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', '.',
   '^'
};

std::vector<char> punctuators = {
   ',',
   '(',
   ')',
   ';',
   '[',
   ']',
   '{',
   '}',
   '\n',
   '.',
   '\\',
   '@',
   '`',
   '#',
   '\'',
   '"',
   ' ',
   ':',
   '?'
};

} // namespace util
} // namespace cdot