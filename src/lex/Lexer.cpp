//
// Created by Jonas Zell on 18.06.17.
//

#include "Lexer.h"
#include "Preprocessor.h"

#include "Basic/IdentifierInfo.h"
#include "Message/Diagnostics.h"

#include <cassert>
#include <regex>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>


using std::string;
using cdot::lex::Token;
using namespace cdot::diag;

namespace cdot {
namespace lex {

Lexer::Lexer(IdentifierTable &Idents,
             llvm::MemoryBuffer *buf,
             unsigned sourceId,
             const char InterpolationBegin)
   : Idents(Idents),
     sourceId(sourceId),
     CurPtr(buf->getBufferStart()),
     BufStart(buf->getBufferStart()),
     BufEnd(buf->getBufferEnd()),
     InterpolationBegin(InterpolationBegin),
     doneLexing(false),
     isModuleLexer(false)
{

}

llvm::StringRef Lexer::getCurrentIdentifier() const
{
   return currentTok().getIdentifierInfo()->getIdentifier();
}

void Lexer::lex()
{
   if (doneLexing)
      return;

   do {
      tokens.push_back(lex_next_token());
   }
   while (!tokens.back().is(tok::eof));

   TokenVec finalTokenVec;
   Preprocessor PP(finalTokenVec, Idents, tokens, sourceId);
   PP.doPreprocessing();

   tokens     = std::move(finalTokenVec);
   tokenIndex = 1;
   doneLexing = true;
}

// RAII utility classes

void Lexer::printTokensTo(llvm::raw_ostream &out)
{
   for (const auto& tok : tokens)
      out << tok.rawRepr();

   out << "\n";
}

Lexer::StateSaveGuard::StateSaveGuard(Lexer *lex)
   : lex(lex), shrinkTo(lex->tokenIndex)
{
   assert(shrinkTo);
}

Lexer::StateSaveGuard::~StateSaveGuard()
{
   if (lex->tokens.size() < shrinkTo) {
      return;
   }

   lex->tokenIndex = shrinkTo;
}

void Lexer::backtrack()
{
   --tokenIndex;
   if (currentTok().is(tok::space))
      backtrack();
}

char Lexer::escape_char(char c)
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

string Lexer::unescape_char(char c)
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

Token Lexer::lex_next_token()
{
   TokBegin = CurPtr++;
   tok::TokenType kind;

   switch (*TokBegin) {
      case '\0': {
         if (CurPtr >= BufEnd)
            return makeToken(tok::eof);

         // ignore embedded nul characters
         return lex_next_token();
      }
      // whitespace
      case ' ': {
         while (*CurPtr == ' ')
            ++CurPtr;

         return makeToken(TokBegin, CurPtr - TokBegin, tok::space);
      }
      // carriage return / newline
      case '\r':
         if (*CurPtr == '\n')
            ++CurPtr;
         LLVM_FALLTHROUGH;
      case '\n':
         kind = tok::newline;
         break;
      // character literal
      case '\'':
         return lexCharLiteral();
      // string literal
      case '"':
         return lexStringLiteral();
      // numeric literals
      case '0': case '1': case '2': case '3': case '4':
      case '5': case '6': case '7': case '8': case '9':
         return lexNumericLiteral();
      // preprocessing related tokens
      case '#': {
         if (*CurPtr == '{') {
            tokens.push_back(makeToken(tok::expr_begin));
            lexPreprocessorExpr();

            return makeToken(tok::close_brace);
         }
         // stringify operator
         else if (*CurPtr == '#') {
            ++CurPtr;

            if (*CurPtr == '{') {
               tokens.push_back(makeToken(tok::stringify_begin));
               lexPreprocessorExpr();

               return makeToken(tok::close_brace);
            }
            else {
               diag::err(err_generic_error)
                  << "unexpected character after '##', expecting '{'"
                  << SourceLocation(TokBegin - BufStart, sourceId)
                  << diag::cont;
            }
         }
         // directive
         else {
            while (*CurPtr >= 'a' && *CurPtr <= 'z')
               ++CurPtr;

            auto &II = Idents.get(llvm::StringRef(TokBegin,
                                                  CurPtr - TokBegin));

            if (II.getKeywordTokenKind() == tok::sentinel)
               err(err_generic_error)
                  << "unknown directive " + II.getIdentifier()
                  << SourceLocation(TokBegin - BufStart, sourceId)
                  << diag::term;

            return Token(&II, SourceLocation(TokBegin - BufStart, sourceId),
                         II.getKeywordTokenKind());
         }

         break;
      }
      // dollar identifier
      case '$': {
         kind = tok::dollar_ident;

         if (*CurPtr == '$') {
            ++CurPtr;
            kind = tok::dollar_dollar_ident;
         }
         else if (!isIdentifierContinuationChar(*CurPtr))
            return makeToken(tok::dollar);

         return lexIdentifier(kind);
      }
      // percent identifier
      case '%': {
         kind = tok::percent_ident;

         if (*CurPtr == '%') {
            ++CurPtr;
            kind = tok::percent_percent_ident;
         }
         else if (!isIdentifierContinuationChar(*CurPtr))
            return makeToken(tok::percent);

         return lexIdentifier(kind);
      }
      // punctuators
      case ',': kind = tok::comma; break;
      case '(': kind = tok::open_paren; break;
      case ')': kind = tok::close_paren; break;
      case ';': kind = tok::semicolon; break;
      case '[': kind = tok::open_square; break;
      case ']': kind = tok::close_square; break;
      case '{': kind = tok::open_brace; break;
      case '}': kind = tok::close_brace; break;
      case '\\': kind = tok::backslash; break;
      case '@': kind = tok::at; break;
      case ':': kind = tok::colon; break;
      case '?': kind = tok::question; break;
      // possible comment
      case '/':
         if (*CurPtr == '/')
            return skipSingleLineComment();

         if (*CurPtr == '*')
            return skipMultiLineComment();

         LLVM_FALLTHROUGH;
      // operators
      case '.':
         // periods have to appear in sequences of two or more
         if (*CurPtr != '.')
            return makeToken(tok::period);

         if (*(CurPtr + 1) == '.') {
            CurPtr += 2;
            return makeToken(tok::triple_period);
         }

         LLVM_FALLTHROUGH;
      case '+':
      case '-':
      case '=':
      case '&':
      case '|':
      case '!':
      case '*':
      case '~':
      case '^':
      case '<':
      case '>':{
         lexOperator();

         auto op = llvm::StringRef(TokBegin, CurPtr - TokBegin);
         kind = getBuiltinOperator(op);

         if (kind == tok::sentinel) {
            auto &II = Idents.get(op);
            return Token(&II, SourceLocation(TokBegin - BufStart, sourceId),
                         tok::op_ident);
         }

         break;
      }
      case '`': {
         ++TokBegin;
         while (*CurPtr != '`' && *CurPtr != '\0')
            ++CurPtr;

         if (CurPtr == BufEnd) {
            err(err_generic_error)
               << "unexpected end of file, expecting '`'"
               << SourceLocation(currentIndex(), sourceId)
               << diag::cont;

            note(note_generic_note)
               << "to match this"
               << SourceLocation(TokBegin - BufStart, sourceId)
               << diag::term;
         }

         ++CurPtr;

         auto &II = Idents.get({ TokBegin, size_t(CurPtr - TokBegin - 1) });
         return Token(&II, SourceLocation(TokBegin - BufStart, sourceId),
                      tok::ident);
      }
      // as, as?, as!
      case 'a': {
         auto ptr = CurPtr;
         if (*ptr++ == 's') {
            if (*ptr == '!') {
               CurPtr += 2;
               kind = tok::as_exclaim;
               break;
            }
            else if (*ptr == '?') {
               CurPtr += 2;
               kind = tok::as_question;
               break;
            }
            //FIXME find better way to identify 'as'
            else if (*ptr == ' ') {
               ++CurPtr;
               kind = tok::as;
               break;
            }
         }

         LLVM_FALLTHROUGH;
      }
      default:
         return lexIdentifier();
   }

   return makeToken(kind);
}

tok::TokenType Lexer::getBuiltinOperator(llvm::StringRef str)
{
   assert(!str.empty());
   
   const char *ptr = str.data();
   tok::TokenType kind = tok::sentinel;
   
   switch (str.size()) {
      case 1:
         switch (str.front()) {
            case '+': kind = tok::plus; break;
            case '-': kind = tok::minus; break;
            case '*': kind = tok::times; break;
            case '/': kind = tok::div; break;
            case '=': kind = tok::equals; break;
            case '<': kind = tok::smaller; break;
            case '>': kind = tok::greater; break;
            case '&': kind = tok::op_and; break;
            case '|': kind = tok::op_or; break;
            case '!': kind = tok::exclaim; break;
            case '~': kind = tok::tilde; break;
            case '^': kind = tok::caret; break;
            case '.': kind = tok::period; break;
            default: break;
         }
         
         break;
      case 2:
         switch (*ptr++) {
            case '+':
               switch (*ptr) {
                  case '+': kind = tok::plus_plus; break;
                  case '=': kind = tok::plus_equals; break;
                  default: break;
               }

               break;
            case '-':
               switch (*ptr) {
                  case '-': kind = tok::minus_minus; break;
                  case '>': kind = tok::arrow_single; break;
                  case '=': kind = tok::minus_equals; break;
                  default: break;
               }

               break;
            case '*':
               switch (*ptr) {
                  case '*': kind = tok::times_times; break;
                  case '=': kind = tok::times_equals; break;
                  default: break;
               }

               break;
            case '/':
               switch (*ptr) {
                  case '=': kind = tok::div_equals; break;
                  default: break;
               }

               break;
            case '%':
               switch (*ptr) {
                  case '=': kind = tok::mod_equals; break;
                  default: break;
               }

               break;
            case '=':
               switch (*ptr) {
                  case '=': kind = tok::double_equals; break;
                  case '>': kind = tok::arrow_double; break;
                  default: break;
               }

               break;
            case '!':
               if (*ptr == '=')
                  kind = tok::exclaim_equals;

               break;
            case '<':
               switch (*ptr) {
                  case '=': kind = tok::smaller_equals; break;
                  case '<': kind = tok::shl; break;
                  default: break;
               }

               break;
            case '>':
               switch (*ptr) {
                  case '=': kind = tok::greater_equals; break;
                  case '>': kind = tok::ashr; break;
                  default: break;
               }

               break;
            case '^':
               switch (*ptr) {
                  case '=': kind = tok::xor_equals; break;
                  default: break;
               }

               break;
            case '&':
               switch (*ptr) {
                  case '&': kind = tok::logical_and; break;
                  case '=': kind = tok::and_equals; break;
                  default: break;
               }

               break;
            case '|':
               switch (*ptr) {
                  case '|': kind = tok::logical_or; break;
                  case '=': kind = tok::or_equals; break;
                  default: break;
               }

               break;
            default:
               break;
         }

         break;
      case 3:
         switch (*ptr++) {
            case '=':
               if (*ptr++ == '=')
                  if (*ptr == '=')
                     kind = tok::triple_equals;

               break;
            case '!':
               if (*ptr++ == '=')
                  if (*ptr == '=')
                     kind = tok::exclaim_double_equals;

               break;
            case '.':
               if (*ptr++ == '.')
                  if (*ptr == '.')
                     kind = tok::triple_period;

               break;
            case '>':
               if (*ptr++ == '>') {
                  if (*ptr == '>')
                     kind = tok::lshr;
                  else if (*ptr == '=')
                     kind = tok::ashr_equals;
               }

               break;
            case '<':
               if (*ptr++ == '<')
                  if (*ptr == '=')
                     kind = tok::shl_equals;

               break;
            default:
               break;
         }
      case 4:
         if (str == ">>>=")
            kind = tok::lshr_equals;

         break;
      default:
         break;
   }

   return kind;
}

Token Lexer::lexStringLiteral()
{
   assert(*TokBegin == '"');
   ++TokBegin;

   while (1) {
      if (*CurPtr == '"')
         break;
      else if (*CurPtr == '\0') {
         if (CurPtr >= BufEnd) {
            err(err_generic_error)
               << "unexpected end of file, expecting '\"'"
               << SourceLocation(currentIndex(), sourceId)
               << diag::term;

            break;
         }
      }
      else if (*CurPtr == '\\') {
         if (!isModuleLexer) {
            // normal escape, e.g. "\n"
            ++CurPtr;
         }
         else {
            // hex escape, e.g. "\0A"
            CurPtr += 2;
         }
      }
      else if (*CurPtr == InterpolationBegin) {
         tokens.emplace_back(TokBegin, CurPtr - TokBegin,
                             tok::stringliteral,
                             SourceLocation(CurPtr - BufStart, sourceId));

         tokens.emplace_back(tok::sentinel);

         ++CurPtr;
         if (*CurPtr == '{') {
            ++CurPtr;

            unsigned openParens = 1;
            unsigned closeParens = 0;

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

               tokens.push_back(tok);
            }
         }
         else {
            tokens.push_back(lex_next_token());
         }

         tokens.emplace_back(tok::sentinel);
         TokBegin = CurPtr;

         continue;
      }

      ++CurPtr;
   }

   assert(*CurPtr == '"');
   ++CurPtr;

   return makeToken(TokBegin, CurPtr - TokBegin - 1, tok::stringliteral);
}

Token Lexer::lexCharLiteral()
{
   assert(*TokBegin == '\'');

   switch (*CurPtr) {
      case '\\':
         ++CurPtr;
         LLVM_FALLTHROUGH;
      default:
         ++CurPtr;
         break;
   }

   if (*CurPtr != '\'')
      err(err_generic_error)
         << "expected \"'\" after character literal"
         << SourceLocation(currentIndex(), sourceId)
         << diag::end;

   return makeToken(TokBegin + 1, CurPtr - TokBegin - 2, tok::charliteral);
}

Token Lexer::lexNumericLiteral()
{
   assert(::isdigit(*TokBegin));

   char first = *TokBegin;
   char next  = *CurPtr;

   // hexadecimal literal
   if (first == '0' && (next == 'x' || next == 'X')) {
      ++CurPtr;
      while (::ishexnumber(*CurPtr))
         ++CurPtr;

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   // binary literal
   if (first == '0' && (next == 'b' || next == 'B')) {
      ++CurPtr;
      while (*CurPtr == '0' || *CurPtr == '1')
         ++CurPtr;

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   // octal literal
   if (first == '0') {
      while (*CurPtr >= '0' && *CurPtr <= '7')
         ++CurPtr;

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   bool foundDecimalPoint = false;
   while (1) {
      if (*CurPtr == '.' && !foundDecimalPoint)
         foundDecimalPoint = true;
      else if (!::isdigit(*CurPtr))
         break;

      ++CurPtr;
   }

   return makeToken(TokBegin, CurPtr - TokBegin,
                    foundDecimalPoint ? tok::fpliteral : tok::integerliteral);
}

bool Lexer::isIdentifierContinuationChar(char c)
{
   switch (c) {
      case '+':
      case '-':
      case '*':
      case '/':
      case '.':
      case '=':
      case '<':
      case '>':
      case '&':
      case '|':
      case '!':
      case '~':
      case '^':
      case ',':
      case '(':
      case ')':
      case ';':
      case '[':
      case ']':
      case '{':
      case '}':
      case '\\':
      case '@':
      case '`':
      case ':':
      case '?':
      case '\'':
      case '"':
      case '\n':
      case '\r':
      case ' ':
      case '\0':
         return false;
      default:
         return true;
   }
}

Token Lexer::lexIdentifier(cdot::lex::tok::TokenType identifierKind)
{
   while (isIdentifierContinuationChar(*CurPtr)) {
      ++CurPtr;
   }

   auto &II = Idents.get({ TokBegin, size_t(CurPtr - TokBegin) });
   if (II.getKeywordTokenKind() != tok::sentinel)
      identifierKind = II.getKeywordTokenKind();

   return Token(&II, SourceLocation(TokBegin - BufStart, sourceId),
                identifierKind);
}

void Lexer::lexOperator()
{
   --CurPtr;

   while (1) {
      switch (*CurPtr) {
         case '+':
         case '-':
         case '=':
         case '<':
         case '&':
         case '|':
         case '%':
         case '!':
         case '*':
         case '/':
         case '~':
         case '?':
         case '^':
         case '>':
            break;
         case '.':
            // periods must occur in sequences of two or more
            if (*(CurPtr + 1) != '.')
               return;

            ++CurPtr;
            break;
         default:
            return;
      }

      ++CurPtr;
   }
}

void Lexer::lexPreprocessorExpr()
{
   assert(*CurPtr == '{');
   ++CurPtr;

   unsigned openedBraces = 1;
   unsigned closedBraces = 0;

   while (openedBraces != closedBraces) {
      auto tok = lex_next_token();
      switch (tok.getKind()) {
         case tok::open_brace:
            ++openedBraces;
            break;
         case tok::close_brace:
            ++closedBraces;
            if (openedBraces == closedBraces)
               continue;

            break;
         case tok::eof:
            diag::err(err_generic_error)
               << "unexpected end of file, expecting '{'"
               << tok.getSourceLoc()
               << diag::term;
         default:
            break;
      }

      tokens.push_back(tok);
   }
}

Token Lexer::skipSingleLineComment()
{
   while (*CurPtr != '\n' && *CurPtr != '\0')
      ++CurPtr;

   return lex_next_token();
}

Token Lexer::skipMultiLineComment()
{
   assert(*CurPtr == '*');

   ++CurPtr;
   while (1) {
      switch (*CurPtr++) {
         case '\0':
            return lex_next_token();
         case '*':
            if (*CurPtr == '/') {
               ++CurPtr;
               return lex_next_token();
            }

            LLVM_FALLTHROUGH;
         default:
            break;
      }
   }

   llvm_unreachable("file is not zero terminated!");
}

void Lexer::expect_impl(tok::TokenType ty)
{
   if (!currentTok().is(ty)) {
      err(err_generic_error)
         << "unexpected token " + currentTok().toString()
         << currentTok().getSourceLoc()
         << diag::term;
   }
}

void Lexer::advance(bool ignore_newline, bool significantWhiteSpace)
{
   assert(tokenIndex < tokens.size() && "advancing past the end of the file!");

   Token &t = tokens[tokenIndex++];
   if ((t.is(tok::newline) && ignore_newline)
       || (t.is(tok::space) && !significantWhiteSpace)
       || t.is(tok::backslash)) {
      return advance(ignore_newline, significantWhiteSpace);
   }
}

Token Lexer::lookahead(bool ignore_newline, bool sw,
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

void Lexer::skip_until_even(ParenKind kind)
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

} // namespace lex
} // namespace cdot