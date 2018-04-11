//
// Created by Jonas Zell on 18.06.17.
//

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stack>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/ArrayRef.h>

#include "Token.h"
#include "../Message/Diagnostics.h"

namespace llvm {
   class MemoryBuffer;
} // namespace llvm

namespace cdot {
   struct Variant;
   class IdentifierTable;
   class PreprocessorImpl;
} // namespace cdot

namespace cdot {

namespace parse {
   class Parser;
} // namespace Parse

namespace diag {
   class DiagnosticBuilder;
} // namespace Parse

namespace module {
   struct ModuleLexerTraits;
} // namespace module

namespace lex {

class Lexer {
public:
#ifdef NDEBUG
   using TokenVec   = llvm::SmallVector<Token, 256>;
   using PPTokenVec = llvm::SmallVector<Token, 64>;
#else
   using TokenVec   = std::vector<Token>;
   using PPTokenVec = std::vector<Token>;
#endif

   Lexer(IdentifierTable &Idents,
         DiagnosticsEngine &Diags,
         llvm::MemoryBuffer *buf,
         unsigned sourceId,
         unsigned offset = 1,
         const char InterpolationBegin = '$');

#ifndef NDEBUG
   virtual
#endif
   ~Lexer() = default;

   void lex();
   void lexDiagnostic();

   llvm::StringRef getCurrentIdentifier() const;

   // RAII utility classes

   class StateSaveGuard {
   public:
      StateSaveGuard(Lexer *lex);
      ~StateSaveGuard();

   protected:
      Lexer *lex;
      size_t shrinkTo;
   };

   template<class ...Rest>
   void expect(tok::TokenType ty, Rest... rest)
   {
      advance();
      expect_impl(ty, rest...);
   }

   template<class ...Rest>
   void expect_impl(tok::TokenType ty, Rest... rest)
   {
      if (currentTok().is(ty)) return;
      expect_impl(rest...);
   }

   void expect_impl(tok::TokenType ty);

   template <class ...Rest>
   bool advanceIf(tok::TokenType ty, Rest... rest)
   {
      if (currentTok().is(ty)) {
         advance();
         return true;
      }
      else {
         return advanceIf(rest...);
      }
   }

   bool advanceIf(tok::TokenType ty)
   {
      if (currentTok().is(ty)) {
         advance();
         return true;
      }

      return false;
   }

   void advance(bool ignoreNewLine = true, bool significantWhitespace = false);
   Token lookahead(bool ignoreNewline = true, bool sw = false, size_t i = 0);
   void backtrack();

   bool isOperatorContinuationChar(char c);

   static char escape_char(char);
   static std::string unescape_char(char);

   const char* getSrc()
   {
      return BufStart;
   }

   const char* getBuffer()
   {
      return CurPtr;
   }

   unsigned currentIndex() const
   {
      return unsigned(CurPtr - BufStart);
   }

   Token const& currentTok() const
   {
      return tokens[tokenIndex - 1];
   }

   SourceLocation getSourceLoc() const
   {
      if (doneLexing) return currentTok().getSourceLoc();
      return SourceLocation(currentIndex() + offset);
   }

   IdentifierTable &getIdents() const
   {
      return Idents;
   }

   struct SavePoint {
      SavePoint(Lexer &lex, size_t idx)
         : lex(lex), idx(idx)
      {}

      void reset()
      {
         lex.tokenIndex = idx;
      }

   private:
      Lexer &lex;
      size_t idx;
   };

   SavePoint makeSavePoint()
   {
      return SavePoint(*this, tokenIndex);
   }

   void printTokensTo(llvm::raw_ostream &out);

   enum ParenKind {
      PAREN = '(',
      BRACE = '{',
      ANGLED = '<',
      SQUARE = '['
   };

   void skip_until_even(ParenKind kind);

   std::vector<Token> &getTokens()
   {
      return tokens;
   }

   unsigned int getSourceId() const { return sourceId; }

   friend diag::DiagnosticBuilder& operator<<(diag::DiagnosticBuilder &builder,
                                              Lexer* const& lex) {
      builder.setLoc(lex->getSourceLoc());
      return builder;
   }

   friend class parse::Parser;

protected:
   Token lexStringLiteral();
   Token lexNumericLiteral();
   Token lexCharLiteral();

   bool isIdentifierContinuationChar(char c);
   Token lexIdentifier(tok::TokenType = tok::ident);

   void lexOperator();
   void lexPreprocessorExpr();
   Token skipSingleLineComment();
   Token skipMultiLineComment();

   tok::TokenType getBuiltinOperator(llvm::StringRef str);

   template<class ...Args>
   Token makeToken(Args&&... args)
   {
      SourceLocation loc(TokBegin - BufStart + offset);
      return Token(args..., loc);
   }

   Token makeEOF();

   IdentifierTable &Idents;
   DiagnosticsEngine &Diags;

   TokenVec tokens;
   unsigned sourceId = 0;
   size_t tokenIndex = 0;

   const char *CurPtr;
   const char *TokBegin;

   const char *BufStart;
   const char *BufEnd;

   const char InterpolationBegin;

   bool doneLexing : 1;
   bool isModuleLexer : 1;

   unsigned offset = 0;

   Token lex_next_token();
};

} // namespace lex
} // namespace cdot

#endif //LEXER_H
