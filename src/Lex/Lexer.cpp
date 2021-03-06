
#include "cdotc/Lex/Lexer.h"

#include "cdotc/Basic/IdentifierInfo.h"
#include "cdotc/Basic/FileManager.h"
#include "cdotc/Diagnostics/DiagnosticsEngine.h"
#include "cdotc/Support/Format.h"
#include "cdotc/Support/SaveAndRestore.h"

#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

#include <cassert>
#include <cctype>
#include <regex>

using std::string;
using cdot::lex::Token;
using namespace cdot::diag;

namespace cdot {
namespace lex {

Lexer::Lexer(IdentifierTable& Idents, DiagnosticsEngine& Diags,
             llvm::MemoryBuffer* buf, unsigned sourceId, unsigned offset,
             const char InterpolationBegin, bool primeLexer)
    : Idents(Idents), Diags(Diags), sourceId(sourceId),
      CurPtr(buf->getBufferStart()), BufStart(buf->getBufferStart()),
      BufEnd(buf->getBufferEnd()), InterpolationBegin(InterpolationBegin),
      offset(offset)
{
   Idents.addKeywords();

   if (primeLexer)
      advance(false, true);
}

Lexer::Lexer(IdentifierTable& Idents, DiagnosticsEngine& Diags,
             llvm::ArrayRef<Token> Tokens, unsigned sourceId,
             unsigned int offset)
    : Idents(Idents), Diags(Diags), LookaheadVec(Tokens.begin(), Tokens.end()),
      sourceId(sourceId), CurPtr(nullptr), BufStart(nullptr), BufEnd(nullptr),
      InterpolationBegin('$'), offset(offset), IsTokenLexer(true)
{
   Idents.addKeywords();

   if (LookaheadVec.empty() || !LookaheadVec.back().is(tok::eof))
      LookaheadVec.emplace_back(makeEOF());

   CurTok = LookaheadVec.front();
   LookaheadIdx = 1;
}

void Lexer::reset(llvm::ArrayRef<Token> Tokens)
{
   assert(IsTokenLexer && "can't reset non-token lexer");

   LookaheadVec.clear();
   LookaheadVec.append(Tokens.begin(), Tokens.end());

   if (LookaheadVec.empty() || !LookaheadVec.back().is(tok::eof))
      LookaheadVec.emplace_back(makeEOF());

   CurTok = LookaheadVec.front();
   LookaheadIdx = 1;
}

void Lexer::lexCompleteFile()
{
   auto SAR1 = support::saveAndRestore(LastTok);
   auto SAR2 = support::saveAndRestore(CurTok);

   while (!CurTok.is(tok::eof)) {
      advance(false, true);
      LookaheadVec.push_back(CurTok);
      ++LookaheadIdx;
   }

   LookaheadIdx = 0;
}

namespace {

class EmitCommentConsumer: public CommentConsumer {
   /// The stream to emit comments to.
   llvm::raw_ostream &OS;

   /// Number of emitted tokens.
   int &i;

   /// Base offset of the source file.
   unsigned BaseOffset;

public:
   explicit EmitCommentConsumer(llvm::raw_ostream &OS, int &i,
                                unsigned BaseOffset)
                                 : OS(OS), i(i), BaseOffset(BaseOffset)
   {}

   void HandleLineComment(llvm::StringRef Txt, SourceRange SR) override
   {
      if (i++ != 0)
         OS << "\n";

      auto LocStart = SR.getStart().getOffset() - BaseOffset;
      OS << "(" << LocStart << ", line_comment, " << Txt << ")";
   }

   void HandleBlockComment(llvm::StringRef Txt, SourceRange SR) override
   {
      if (i++ != 0)
         OS << "\n";

      auto LocStart = SR.getStart().getOffset() - BaseOffset;
      OS << "(" << LocStart << ", block_comment, ";

      for (auto &C : Txt) {
         support::unescape_char(C, OS);
      }

      OS << ")";
   }
};

} // anonymous namespace

void Lexer::lexAndEmitTokens(llvm::raw_ostream &OS)
{
   int i = 0;
   EmitCommentConsumer CC(OS, i, offset);

   auto SAR = support::saveAndRestore(this->commentConsumer,
                                      (CommentConsumer*)&CC);

   while (!currentTok().is(tok::eof)) {
      if (i++ != 0)
         OS << "\n";

      auto &Tok = currentTok();
      auto LocStart = Tok.getSourceLoc().getOffset() - offset;
      OS << "(" << LocStart << ", ";
      ::operator<<(OS, Tok.getKind());
      OS << ", " << Tok << ")";

      advance(false, true);
   }
}

void Lexer::findComments()
{
   assert(commentConsumer != nullptr);

   auto SAR1 = support::saveAndRestore(CurPtr);
   auto SAR2 = support::saveAndRestore(TokBegin);

   while (*CurPtr != '\0') {
      if (*CurPtr == '/') {
         ++CurPtr;
         if (*CurPtr == '/')
            skipSingleLineComment();

         if (*CurPtr == '*')
            skipMultiLineComment();
      }

      ++CurPtr;
   }
}

Token Lexer::makeEOF()
{
   auto Offset = std::max<unsigned long>(1, BufEnd - BufStart + offset - 1);
   return Token(tok::eof, SourceLocation(Offset));
}

llvm::StringRef Lexer::getCurrentIdentifier() const
{
   return currentTok().getIdentifierInfo()->getIdentifier();
}

// RAII utility classes

Lexer::LookaheadRAII::LookaheadRAII(Lexer& L)
    : L(L), LastTok(L.LastTok), CurTok(L.CurTok)
{
}

Lexer::LookaheadRAII::~LookaheadRAII()
{
   L.LastTok = LastTok;
   L.CurTok = CurTok;

   // Keep the lookahead tokens that we didn't see yet.
   Tokens.append(L.LookaheadVec.begin() + L.LookaheadIdx, L.LookaheadVec.end());
   L.LookaheadVec = std::move(Tokens);

   L.LookaheadIdx = 0;
}

void Lexer::LookaheadRAII::advance(bool ignoreNewline,
                                   bool significantWhitespace)
{
   // Keep all tokens.
   L.advance(false, true);

   while ((ignoreNewline && L.currentTok().is(tok::newline))
          || (!significantWhitespace && L.currentTok().is(tok::space))) {
      Tokens.push_back(L.currentTok());
      L.advance(false, true);
   }

   Tokens.push_back(L.currentTok());
}

void Lexer::printTokensTo(llvm::raw_ostream& out)
{
   while (!eof()) {
      advance();
      CurTok.print(out);
   }
}

void Lexer::dump() { printTokensTo(llvm::outs()); }

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

Token Lexer::lexNextToken()
{
   TokBegin = CurPtr++;
   tok::TokenType kind;

   switch (*TokBegin) {
   case '\0': {
      if (CurPtr >= BufEnd)
         return makeEOF();

      // ignore embedded nul characters
      return lexNextToken();
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
   case '0':
   case '1':
   case '2':
   case '3':
   case '4':
   case '5':
   case '6':
   case '7':
   case '8':
   case '9':
      return lexNumericLiteral();
   // preprocessing related tokens
   case '#': {
      //      if (*CurPtr == '{') {
      //         tokens.push_back(makeToken(tok::expr_begin));
      //         lexPreprocessorExpr();
      //
      //         return makeToken(tok::close_brace);
      //      }
      //      // stringify operator
      //      else if (*CurPtr == '#') {
      //         ++CurPtr;
      //
      //         if (*CurPtr == '{') {
      //            tokens.push_back(makeToken(tok::stringify_begin));
      //            lexPreprocessorExpr();
      //
      //            return makeToken(tok::close_brace);
      //         }
      //         else {
      //            Diags.Diag(err_generic_error)
      //               << "unexpected character after '##', expecting '{'"
      //               << SourceLocation(TokBegin - BufStart + offset);
      //         }
      //      }
      //      // directive
      //      else {
      //         while (*CurPtr >= 'a' && *CurPtr <= 'z')
      //            ++CurPtr;
      //
      //         auto &II = Idents.get(llvm::StringRef(TokBegin,
      //                                               CurPtr - TokBegin));
      //
      //         if (II.getKeywordTokenKind() == tok::sentinel) {
      //            Diags.Diag(err_generic_error)
      //               << "unknown directive " + II.getIdentifier()
      //               << SourceLocation(TokBegin - BufStart + offset);
      //         }
      //
      //         return Token(&II, SourceLocation(TokBegin - BufStart + offset),
      //                      II.getKeywordTokenKind());
      //      }

      llvm_unreachable("what preprocessor?");
   }
   // dollar identifier
   case '$': {
      if (::isdigit(*CurPtr))
         return lexClosureArgumentName();

      if (isIdentifierContinuationChar(*CurPtr)) {
         ++TokBegin;
         return lexIdentifier(tok::dollar_ident);
      }

      kind = tok::dollar;
      break;
   }
   // punctuators
   case ',':
      kind = tok::comma;
      break;
   case '(':
      kind = tok::open_paren;
      break;
   case ')':
      kind = tok::close_paren;
      break;
   case ';':
      kind = tok::semicolon;
      break;
   case '[':
      kind = tok::open_square;
      break;
   case ']':
      kind = tok::close_square;
      break;
   case '{':
      kind = tok::open_brace;
      break;
   case '}':
      kind = tok::close_brace;
      break;
   case '\\':
      kind = tok::backslash;
      break;
   case '@':
      kind = tok::at;
      break;
   // possible comment
   case '/':
      if (*CurPtr == '/') {
         skipSingleLineComment();
         return lexNextToken();
      }

      if (*CurPtr == '*') {
         skipMultiLineComment();
         return lexNextToken();
      }

      goto case_operator;
   // operators
   case '.':
      // periods have to appear in sequences of two or more
      if (*CurPtr != '.')
         return makeToken(tok::period);

      if (*(CurPtr + 1) == '.') {
         CurPtr += 2;
         return makeToken(tok::triple_period);
      }

      goto case_operator;
   case_operator:
   case '+':
   case '-':
   case '=':
   case '&':
   case '|':
   case '!':
   case '?':
   case ':':
   case '*':
   case '~':
   case '^':
   case '<':
   case '>':
   case '%': {
      lexOperator();

      auto op = llvm::StringRef(TokBegin, CurPtr - TokBegin);
      kind = getBuiltinOperator(op);

      if (kind == tok::sentinel) {
         auto& II = Idents.get(op);
         return Token(&II, SourceLocation(TokBegin - BufStart + offset),
                      tok::op_ident);
      }

      break;
   }
   case '_':
      if (isIdentifierContinuationChar(*CurPtr))
         return lexIdentifier();

      return makeToken(tok::underscore);
   case '`': {
      ++TokBegin;

      bool done = false;
      while (!done) {
         switch (*CurPtr++) {
         case '`':
            done = true;
            break;
         case '\0':
            if (CurPtr >= BufEnd) {
               auto Tok = makeEOF();
               Diags.Diag(err_unexpected_eof)
                   << true << "'`'"
                   << SourceLocation(Tok.getOffset() - 1);

               Diags.Diag(note_to_match_this)
                   << SourceLocation(TokBegin - 1 - BufStart + offset);

               return Tok;
            }

            break;
         case '\n': {
            auto Loc = SourceLocation(currentIndex() + offset - 1);
            Diags.Diag(err_unexpected_newline_expecting)
                << "'`'" << Loc;

            Diags.Diag(note_to_match_this)
                << SourceLocation(TokBegin - 1 - BufStart + offset);

            done = true;
            break;
         }
         default:
            break;
         }
      }

      auto& II = Idents.get({TokBegin, size_t(CurPtr - TokBegin - 1)});
      return Token(&II, SourceLocation(TokBegin - BufStart + offset),
                   tok::ident, true);
   }
   default:
      return lexIdentifier();
   }

   return makeToken(kind);
}

tok::TokenType Lexer::getBuiltinOperator(llvm::StringRef str)
{
   assert(!str.empty());

   const char* ptr = str.data();
   tok::TokenType kind = tok::sentinel;

   switch (str.size()) {
   case 1:
      switch (str.front()) {
      case '+':
         kind = tok::plus;
         break;
      case '-':
         kind = tok::minus;
         break;
      case '*':
         kind = tok::times;
         break;
      case '/':
         kind = tok::div;
         break;
      case '=':
         kind = tok::equals;
         break;
      case '<':
         kind = tok::smaller;
         break;
      case '>':
         kind = tok::greater;
         break;
      case '&':
         kind = tok::op_and;
         break;
      case '|':
         kind = tok::op_or;
         break;
      case '!':
         kind = tok::exclaim;
         break;
      case '~':
         kind = tok::tilde;
         break;
      case '^':
         kind = tok::caret;
         break;
      case '.':
         kind = tok::period;
         break;
      case ':':
         kind = tok::colon;
         break;
      case '?':
         kind = tok::question;
         break;
      default:
         break;
      }

      break;
   case 2:
      switch (*ptr++) {
      case '+':
         switch (*ptr) {
         case '+':
            kind = tok::plus_plus;
            break;
         case '=':
            kind = tok::plus_equals;
            break;
         default:
            break;
         }

         break;
      case '-':
         switch (*ptr) {
         case '-':
            kind = tok::minus_minus;
            break;
         case '>':
            kind = tok::arrow_single;
            break;
         case '=':
            kind = tok::minus_equals;
            break;
         default:
            break;
         }

         break;
      case '*':
         switch (*ptr) {
         case '*':
            kind = tok::times_times;
            break;
         case '=':
            kind = tok::times_equals;
            break;
         default:
            break;
         }

         break;
      case '/':
         switch (*ptr) {
         case '=':
            kind = tok::div_equals;
            break;
         default:
            break;
         }

         break;
      case '%':
         switch (*ptr) {
         case '=':
            kind = tok::mod_equals;
            break;
         default:
            break;
         }

         break;
      case '=':
         switch (*ptr) {
         case '=':
            kind = tok::double_equals;
            break;
         case '>':
            kind = tok::arrow_double;
            break;
         default:
            break;
         }

         break;
      case '!':
         if (*ptr == '=')
            kind = tok::exclaim_equals;

         break;
      case '<':
         switch (*ptr) {
         case '=':
            kind = tok::smaller_equals;
            break;
         case '<':
            kind = tok::shl;
            break;
         default:
            break;
         }

         break;
      case '>':
         switch (*ptr) {
         case '=':
            kind = tok::greater_equals;
            break;
         case '>':
            kind = tok::ashr;
            break;
         default:
            break;
         }

         break;
      case '^':
         switch (*ptr) {
         case '=':
            kind = tok::xor_equals;
            break;
         default:
            break;
         }

         break;
      case '&':
         switch (*ptr) {
         case '&':
            kind = tok::logical_and;
            break;
         case '=':
            kind = tok::and_equals;
            break;
         default:
            break;
         }

         break;
      case '|':
         switch (*ptr) {
         case '|':
            kind = tok::logical_or;
            break;
         case '=':
            kind = tok::or_equals;
            break;
         default:
            break;
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
         if (ptr[0] == '=')
            if (ptr[1] == '=')
               kind = tok::exclaim_double_equals;

         if (ptr[0] == 'i')
            if (ptr[1] == 's')
               kind = tok::exclaim_is;

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

      if (*CurPtr == '\0') {
         if (CurPtr >= BufEnd) {
            Diags.Diag(err_unexpected_eof)
                << true << "'\"'"
                << SourceLocation(currentIndex() + offset - 1);

            --CurPtr;
            return makeToken(TokBegin, CurPtr - TokBegin - 1,
                             tok::stringliteral);
         }
      }
      else if (*CurPtr == '\\') {
         //         if (!isModuleLexer) {
         // normal escape, e.g. "\n"
         ++CurPtr;
         //         }
         //         else {
         //            // hex escape, e.g. "\0A"
         //            CurPtr += 2;
         //         }
      }
      else if (*CurPtr == InterpolationBegin && InterpolationBegin != '\0') {
         if (!isIdentifierContinuationChar(CurPtr[1]) && CurPtr[1] != '{') {
            ++CurPtr;
            continue;
         }

         TokBegin -= 1;
         CurPtr = TokBegin + 1;
         InInterpolation = true;

         return makeToken(nullptr, 0, tok::interpolation_begin);
      }

      ++CurPtr;
   }

   assert(*CurPtr == '"');
   ++CurPtr;

   return makeToken(TokBegin, CurPtr - TokBegin - 1, tok::stringliteral);
}

void Lexer::lexStringInterpolation()
{
   assert(*TokBegin == '"');
   ++TokBegin;

   assert(InInterpolation && "not a string interpolation");
   InInterpolation = false;

   auto& Toks = LookaheadVec;
   while (1) {
      if (*CurPtr == '"')
         break;

      if (*CurPtr == '\0') {
         if (CurPtr >= BufEnd) {
            Diags.Diag(err_unexpected_eof)
                << true << "'\"'"
                << SourceLocation(currentIndex() + offset);

            Toks.push_back(makeToken(CurPtr - 1, 0, tok::eof));
         }
      }
      else if (*CurPtr == '\\') {
         ++CurPtr;
      }
      else if (*CurPtr == InterpolationBegin && InterpolationBegin != '\0') {
         if (!isIdentifierContinuationChar(CurPtr[1]) && CurPtr[1] != '{') {
            ++CurPtr;
            continue;
         }

         Toks.emplace_back(
             makeToken(TokBegin, CurPtr - TokBegin, tok::stringliteral));

         Toks.emplace_back(tok::expr_begin, getSourceLoc());

         ++CurPtr;
         if (*CurPtr == '{') {
            ++CurPtr;

            unsigned openParens = 1;
            unsigned closeParens = 0;

            while (1) {
               auto tok = lexNextToken();
               switch (tok.getKind()) {
               case tok::open_brace:
                  ++openParens;
                  break;
               case tok::close_brace:
                  ++closeParens;
                  break;
               default:
                  break;
               }

               if (openParens == closeParens)
                  break;

               Toks.push_back(tok);
            }
         }
         else {
            TokBegin = CurPtr;
            Toks.push_back(lexIdentifier(tok::ident, false));
         }

         Toks.emplace_back(tok::interpolation_end, getSourceLoc());
         TokBegin = CurPtr;

         continue;
      }

      ++CurPtr;
   }

   assert(*CurPtr == '"');
   ++CurPtr;

   Toks.push_back(
       makeToken(TokBegin, CurPtr - TokBegin - 1, tok::stringliteral));
}

void Lexer::lexDiagnostic()
{
   TokBegin = CurPtr++;
   if (*TokBegin == '"') {
      ++TokBegin;
   }

   auto& Tokens = LookaheadVec;
   while (1) {
      if (*CurPtr == '"')
         break;

      if (*CurPtr == '\0') {
         if (CurPtr >= BufEnd)
            break;
      }
      else if (*CurPtr == '\\') {
         //         if (!isModuleLexer) {
         // normal escape, e.g. "\n"
         ++CurPtr;
         //         }
         //         else {
         //            // hex escape, e.g. "\0A"
         //            CurPtr += 2;
         //         }
      }
      else if (*CurPtr == InterpolationBegin) {
         if (CurPtr[1] == InterpolationBegin) {
            ++CurPtr;
            Tokens.emplace_back(TokBegin, CurPtr - TokBegin, tok::stringliteral,
                                SourceLocation(CurPtr - BufStart + offset));

            TokBegin = ++CurPtr;
            continue;
         }

         Tokens.emplace_back(TokBegin, CurPtr - TokBegin, tok::stringliteral,
                             SourceLocation(CurPtr - BufStart + offset));

         Tokens.emplace_back(tok::sentinel);

         ++CurPtr;
         if (*CurPtr == '{') {
            ++CurPtr;

            // diagnostic functions are of the form
            //  ${ <integer_literal> | <fn_name>(<args>,...) }
            // where <args> should be parsed as comma seperated string literals

            unsigned openBraces = 1;
            unsigned closeBraces = 0;

            while (1) {
               auto tok = lexNextToken();
               switch (tok.getKind()) {
               case tok::open_brace:
                  ++openBraces;
                  break;
               case tok::close_brace:
                  ++closeBraces;
                  break;
               // begin of the argument list
               case tok::open_paren: {
                  Tokens.push_back(tok);

                  unsigned openParens = 1;
                  unsigned closedParens = 0;

                  auto StrBegin = CurPtr;
                  while (openParens != closedParens) {
                     switch (*CurPtr++) {
                     // ignore quoted strings
                     case '\'':
                     case '"': {
                        auto endChar = CurPtr[-1];
                        while (*CurPtr++ != endChar) {
                           assert(CurPtr != BufEnd
                                  && "unclosed string in "
                                     "diagnostic message");
                        }

                        break;
                     }
                     case '(':
                        ++openParens;
                        break;
                     case ')':
                        if (++closedParens != openParens) {
                           break;
                        }

                        LLVM_FALLTHROUGH;
                     case ',': {
                        // allow one comma to begin the argument
                        if (CurPtr - 1 == StrBegin)
                           break;

                        // allow commas in nested parentheses
                        if (closedParens < openParens - 1)
                           break;

                        if (CurPtr - StrBegin - 1 != 0) {
                           auto Str = makeToken(StrBegin, CurPtr - StrBegin - 1,
                                                tok::stringliteral);

                           Tokens.push_back(Str);
                        }

                        // skip at most one whitespace after the comma
                        if (*CurPtr == ' ') {
                           ++CurPtr;
                        }

                        StrBegin = CurPtr;
                        break;
                     }
                     case '\\': {
                        CurPtr++;
                        break;
                     }
                     case '\0':
                        llvm_unreachable("unexpected end of file or nul "
                                         "character!");
                     default:
                        break;
                     }
                  }

                  tok = makeToken(tok::close_paren);
                  break;
               }
               default:
                  break;
               }

               if (openBraces == closeBraces)
                  break;

               Tokens.push_back(tok);
            }
         }
         else {
            Tokens.push_back(lexNextToken());
         }

         Tokens.emplace_back(tok::sentinel);
         TokBegin = CurPtr;

         continue;
      }

      ++CurPtr;
   }

   auto diff = CurPtr - TokBegin;
   if (*TokBegin == '"') {
      ++TokBegin;
   }

   Tokens.push_back(makeToken(TokBegin, diff, tok::stringliteral));
   Tokens.push_back(makeEOF());

   CurTok = Tokens.front();
   LookaheadIdx = 1;
   IsTokenLexer = true;
}

Token Lexer::lexCharLiteral()
{
   assert(*TokBegin == '\'');

   if (*CurPtr++ == '\\') {
      if (*CurPtr == 'x') {
         ++CurPtr;

         for (int i = 0; i < 2; ++i) {
            if (!isxdigit(*CurPtr++)) {
               Diags.Diag(err_invalid_hex_digit)
                   << *(CurPtr - 1)
                   << SourceLocation(currentIndex() + offset);
            }
         }
      }
      else if (*CurPtr == 'u') {
         ++CurPtr;

         for (int i = 0; i < 4; ++i) {
            if (!isxdigit(*CurPtr++)) {
               Diags.Diag(err_invalid_hex_digit)
                   << *(CurPtr - 1)
                   << SourceLocation(currentIndex() + offset);
            }
         }
      }
      else {
         ++CurPtr;
      }
   }

   if (*CurPtr != '\'')
      Diags.Diag(err_expected_after_char_literal)
         << SourceLocation(currentIndex() + offset);

   ++CurPtr;

   return makeToken(TokBegin + 1, CurPtr - TokBegin - 2, tok::charliteral);
}

Token Lexer::lexNumericLiteral()
{
   assert(::isdigit(*TokBegin));

   char first = *TokBegin;
   char next = *CurPtr;

   // hexadecimal literal
   if (first == '0' && (next == 'x' || next == 'X')) {
      ++CurPtr;

      bool IsDecimal = false;
      bool FoundExponent = false;

      bool done = false;
      while (!done) {
         switch (*CurPtr) {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '8':
         case '9':
         case 'a':
         case 'b':
         case 'c':
         case 'd':
         case 'e':
         case 'f':
         case 'A':
         case 'B':
         case 'C':
         case 'D':
         case 'E':
         case 'F':
         case '_':
            ++CurPtr;
            break;
         case '.':
            if (IsDecimal || FoundExponent) {
               break;
            }

            IsDecimal = true;
            ++CurPtr;

            break;
         case 'p':
         case 'P':
            if (FoundExponent) {
               break;
            }
            if (*(++CurPtr) == '-') {
               ++CurPtr;
            }

            FoundExponent = true;
            break;
         case ' ':
         case '\n':
         case '\t':
         case '\r':
         case '\0':
         case 'u':
         case 'i':
            done = true;
            break;
         default:
            if (isIdentifierContinuationChar(*CurPtr)) {
               Diags.Diag(err_invalid_hex_digit)
                   << *CurPtr << SourceLocation(currentIndex() + offset);

               ++CurPtr;
            }
            else {
               done = true;
               break;
            }

            break;
         }
      }

      if (IsDecimal) {
         return makeToken(TokBegin, CurPtr - TokBegin, tok::fpliteral);
      }

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   // binary literal
   if (first == '0' && (next == 'b' || next == 'B')) {
      ++CurPtr;

      bool done = false;
      while (!done) {
         switch (*CurPtr) {
         case '0':
         case '1':
         case '_':
            ++CurPtr;
            break;
         case ' ':
         case '\n':
         case '\t':
         case '\r':
         case '\0':
         case 'u':
         case 'i':
            done = true;
            break;
         default:
            if (isIdentifierContinuationChar(*CurPtr) || ::isalnum(*CurPtr)) {
               Diags.Diag(err_invalid_binary_digit)
                   << *CurPtr << SourceLocation(currentIndex() + offset);

               ++CurPtr;
            }
            else {
               done = true;
               break;
            }

            break;
         }
      }

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   // octal literal
   if (first == '0' && ::isalnum(next)) {
      bool done = false;
      while (!done) {
         switch (*CurPtr) {
         case '0':
         case '1':
         case '2':
         case '3':
         case '4':
         case '5':
         case '6':
         case '7':
         case '_':
            ++CurPtr;
            break;
         case ' ':
         case '\n':
         case '\t':
         case '\r':
         case '\0':
         case 'u':
         case 'i':
            done = true;
            break;
         default:
            if (isIdentifierContinuationChar(*CurPtr) || ::isalnum(*CurPtr)) {
               Diags.Diag(err_invalid_octal_digit)
                   << *CurPtr << SourceLocation(currentIndex() + offset);

               ++CurPtr;
            }
            else {
               done = true;
               break;
            }

            break;
         }
      }

      return makeToken(TokBegin, CurPtr - TokBegin, tok::integerliteral);
   }

   bool foundExponent = false;
   bool foundDecimalPoint = false;
   bool lastWasDecimal = false;

   while (1) {
      if (*CurPtr == '.' && !foundDecimalPoint) {
         foundDecimalPoint = true;
         lastWasDecimal = true;
      }
      else if ((*CurPtr == 'e' || *CurPtr == 'E') && !foundExponent) {
         foundExponent = true;
         foundDecimalPoint = true; // no decimal point allowed after exponent
         ++CurPtr;

         if (CurPtr[0] == '+' || CurPtr[0] == '-')
            ++CurPtr;
      }
      else if (!::isdigit(*CurPtr) && *CurPtr != '_') {
         break;
      }
      else {
         lastWasDecimal = false;
      }

      ++CurPtr;
   }

   if (lastWasDecimal) {
      --CurPtr;
      foundDecimalPoint = false;
   }

   return makeToken(TokBegin, CurPtr - TokBegin,
                    foundDecimalPoint ? tok::fpliteral : tok::integerliteral);
}

Token Lexer::lexClosureArgumentName()
{
   if (!::isdigit(*CurPtr)) {
      Diags.Diag(err_expected_digit_after_dollar)
         << SourceLocation(currentIndex() + offset);
   }

   while (::isdigit(*CurPtr))
      ++CurPtr;

   return makeToken(TokBegin + 1, CurPtr - TokBegin - 1, tok::closure_arg);
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
   case '$':
      return false;
   default:
      return true;
   }
}

static bool isMacroInvocation(const char* CurPtr)
{
   if (*CurPtr != '!')
      return false;

   // Allow an identifier to appear before the delimiter.
   auto Ptr = CurPtr + 1;
   while (true) {
      switch (*Ptr++) {
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
      case ')':
      case ';':
      case ']':
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
      case '\0':
      case '$':
      case '#':
         return false;
      case '(':
      case '[':
      case '{':
         return true;
      default:
         break;
      }
   }
}

Token Lexer::lexIdentifier(tok::TokenType identifierKind, bool AllowMacro)
{
   while (isIdentifierContinuationChar(*CurPtr)) {
      ++CurPtr;
   }

   bool IsKeyword = false;
   auto& II = Idents.get({TokBegin, size_t(CurPtr - TokBegin)});

   if (II.getKeywordTokenKind() != tok::sentinel) {
      identifierKind = II.getKeywordTokenKind();
      IsKeyword = true;
   }

   if (identifierKind == tok::ident && !IsKeyword && AllowMacro
       && isMacroInvocation(CurPtr)) {
      auto& II = Idents.get({TokBegin, size_t(CurPtr - TokBegin)});
      ++CurPtr;

      return Token(&II, SourceLocation(TokBegin - BufStart + offset),
                   tok::macro_name);
   }

   return Token(&II, SourceLocation(TokBegin - BufStart + offset),
                identifierKind);
}

static bool isSeperatorChar(char c)
{
   switch (c) {
   case '\n':
   case ' ':
   case '\r':
      return true;
   default:
      return false;
   }
}

void Lexer::lexOperator()
{
   --CurPtr;

   bool First = true;
   while (1) {
      switch (*CurPtr) {
      case '<':
         if (CurMode == Mode::ParsingTemplateParams) {
            ++CurPtr;
            return;
         }

         LLVM_FALLTHROUGH;
      case '>':
         if (CurMode == Mode::ParsingTemplateParams
             || CurMode == Mode::ParsingTemplateArgs) {
            if (First)
               ++CurPtr;

            return;
         }

         break;
      case '!':
         if (CurPtr[1] == 'i' && CurPtr[2] == 's'
             && isSeperatorChar(CurPtr[3])) {
            CurPtr += 3;
            return;
         }

         break;
      case '+':
      case '-':
      case '=':
      case '&':
      case '|':
      case '%':
      case '*':
      case '/':
      case '~':
      case '^':
      case '?':
      case ':':
         break;
      case '.':
         // periods must occur in sequences of two or more
         if (CurPtr[1] != '.')
            return;

         ++CurPtr;
         break;
      default:
         return;
      }

      ++CurPtr;
      First = false;
   }
}

void Lexer::skipSingleLineComment()
{
   auto Begin = CurPtr - 1;
   SourceLocation BeginLoc(Begin - BufStart + offset);

   while (*CurPtr != '\n' && *CurPtr != '\0')
      ++CurPtr;

   if (commentConsumer) {
      SourceLocation EndLoc(CurPtr - BufStart + offset);
      llvm::StringRef Comment(Begin, CurPtr - Begin);
      commentConsumer->HandleLineComment(Comment, SourceRange(BeginLoc, EndLoc));
   }
}

void Lexer::skipMultiLineComment()
{
   auto Begin = CurPtr - 1;
   SourceLocation BeginLoc(Begin - BufStart + offset);

   assert(*CurPtr == '*');

   ++CurPtr;

   bool done = false;
   int open = 1;

   while (!done) {
      assert(CurPtr < BufEnd && "file is not zero terminated!");

      switch (*CurPtr++) {
      case '\0': {
         done = true;
         break;
      }
      case '/': {
         if (*CurPtr == '*') {
            ++CurPtr;
            ++open;
         }

         break;
      }
      case '*':
         if (*CurPtr == '/') {
            ++CurPtr;
            if (--open == 0) {
               done = true;
            }
         }

         LLVM_FALLTHROUGH;
      default:
         break;
      }
   }

   if (commentConsumer) {
      SourceLocation EndLoc(CurPtr - BufStart + offset);
      llvm::StringRef Comment(Begin, CurPtr - Begin);
      commentConsumer->HandleBlockComment(Comment, SourceRange(BeginLoc, EndLoc));
   }
}

void Lexer::expect_impl(tok::TokenType ty)
{
   if (!currentTok().is(ty)) {
      Diags.Diag(err_unexpected_token)
          << currentTok().toString()
          << currentTok().getSourceLoc();
   }
}

void Lexer::advance(bool ignoreNewline, bool significantWhiteSpace,
                    bool rememberTok)
{
   assert(!CurTok.is(tok::eof) && "advancing past the end of the file!");

   /// Remember this token for backtracking.
   if (rememberTok) {
      LastTok = CurTok;
   }

   if (LookaheadIdx < LookaheadVec.size()) {
      /// Get the next token from the lookahead vector.
      CurTok = LookaheadVec[LookaheadIdx++];
      if (LookaheadIdx == LookaheadVec.size()) {
         LookaheadVec.clear();
         LookaheadIdx = 0;
         AtEOF |= IsTokenLexer;
      }
   }
   else {
      /// Lex the next token.
      CurTok = lexNextToken();
   }

   switch (CurTok.getKind()) {
   case tok::newline:
      if (ignoreNewline) {
         return advance(ignoreNewline, significantWhiteSpace, false);
      }

      break;
   case tok::space:
      if (!significantWhiteSpace) {
         return advance(ignoreNewline, significantWhiteSpace, false);
      }

      break;
   case tok::interpolation_begin:
      if (InInterpolation) {
         lexStringInterpolation();
      }

      break;
   default:
      break;
   }
}

Token Lexer::lookahead(bool ignoreNewline, bool sw, size_t offset)
{
   assert(!CurTok.is(tok::eof) && "advancing past the end of the file!");

   /// Lex as many tokens as necessary for the required lookahead.
   while (LookaheadVec.size() <= LookaheadIdx + offset) {
      LookaheadVec.push_back(lexNextToken());
   }

   // Check if the lookahead token is what we want.
   Token LookaheadTok = LookaheadVec[LookaheadIdx + offset];
   switch (LookaheadTok.getKind()) {
   case tok::newline:
      if (ignoreNewline) {
         return lookahead(ignoreNewline, sw, offset + 1);
      }

      break;
   case tok::space:
      if (!sw) {
         return lookahead(ignoreNewline, sw, offset + 1);
      }

      break;
   case tok::interpolation_begin:
      if (InInterpolation) {
         lexStringInterpolation();
      }

      break;
   default:
      break;
   }

   return LookaheadTok;
}

void Lexer::backtrack()
{
   assert(LastTok && "can't backtrack by more than one token!");

   // Put our current token in the lookahead vector.
   LookaheadVec.insert(LookaheadVec.begin(), CurTok);

   // Backtrack by one token.
   CurTok = LastTok;

   // Reset the last token to check that we only backtrack by one.
   LastTok = Token();
}

} // namespace lex
} // namespace cdot