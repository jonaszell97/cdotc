//
// Created by Jonas Zell on 18.06.17.
//

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <llvm/Support/MemoryBuffer.h>
#include <queue>

#include "Token.h"

using std::unordered_map;

namespace llvm {
   class MemoryBuffer;
}

namespace cdot {

enum class BuiltinMacro : unsigned;
extern unordered_map<string, pair<BuiltinMacro, int>> BuiltinMacros;

enum PPDirective {
   PD_DEFINE,
   PD_RAWDEF,
   PD_ENDDEF,
   PD_UNDEF,
   PD_IFDEF,
   PD_IFNDEF,
   PD_IF,
   PD_ENDIF,
   PD_ELSE,
   PD_ELSEIF,
   PD_PRAGMA,

   PD_PRINT,
   PD_LET,
   PD_FOR,
   PD_ENDFOR,

   PD_NONE,
   PD_INVALID
};

}

class Parser;

class Lexer {
public:
   Lexer();
   Lexer(
      llvm::MemoryBuffer *buf,
      string const& fileName,
      unsigned sourceId
   );

   void advance(TokenType, bool = true, bool = false);
   void advance(bool ignoreNewLine = true, bool significantWhitespace = false);
   Token lookahead(bool = true, size_t i = 0);
   char get_next_char();
   char char_lookahead();
   void backtrack_c(int);
   void backtrack(bool discard = false);

   void reset(const char *src, size_t len);
   void ignoreInterpolation(bool ignore)
   {
      ignore_interpolation = ignore;
   }

   static char escape_char(char);
   static string unescape_char(char);

   string s_val();

   const char* getSrc()
   {
      return begin;
   }

   const char* getBuffer()
   {
      return curr;
   }

   bool continueInterpolation = false;
   size_t indexOnLine = 0;
   size_t currentLine = 1;

   size_t currentIndex = 0;
   size_t lastTokenIndex = 0;

   size_t currentTokenIndex = 0;
   Token currentToken;

   size_t &getCurrentTokenIndex()
   {
      return currentTokenIndex;
   }

   Token getToken(size_t index)
   {
      return tokens[index];
   }

   void pushToken(Token t)
   {
      tokens.push_back(t);
   }

   SourceLocation getSourceLoc() const
   {
      return SourceLocation(
         indexOnLine,
         currentLine,
         currentToken.getSourceLoc().getLength(),
         sourceId
      );
   }

   friend class Parser;
   friend class ParseError;

   typedef std::unique_ptr<Lexer> UniquePtr;

   struct LexerLoc {
      const char *curr;
      size_t currentIndex;
      size_t currentLine;
      size_t indexOnLine;
   };

   struct IgnoreScope {
      IgnoreScope(Lexer *lex) : lex(lex) {
         lex->ignoreValue = true;
         lex->ignoreMacro = true;
         lex->ignoreDirective = true;
      }

      ~IgnoreScope() {
         lex->ignoreValue = false;
         lex->ignoreMacro = false;
         lex->ignoreDirective = false;
      }

      Lexer *lex = nullptr;
   };


   friend class IgnoreScope;

protected:
   struct Macro {
      const char *begin;
      size_t length;
      std::vector<string> args;
      LexerLoc loc;
   };

   static unordered_map<string, Macro> GlobalMacros;
   static unordered_map<string, Macro> Macros;

   static unordered_map<string, Variant> GlobalValues;
   static unordered_map<string, Variant> Values;

   static unordered_map<string, std::vector<Variant>> GlobalArrays;
   static unordered_map<string, std::vector<Variant>> Arrays;

   std::vector<Token> tokens;
   std::vector<Token> lookaheadList;

   const char *curr;
   const char *begin;
   string fileName;

   unsigned sourceId = 0;

   bool ignore_interpolation = false;

   size_t srcLen = 0;

   bool ignoreMacro = false;
   bool ignoreDirective = false;
   bool ignoreValue = false;

   Token makeToken(TokenType ty, Variant &&val, size_t start, size_t end, bool isEscaped = false);

   Token get_next_token(bool, bool);
   Token lex_next_token(bool, bool);

   void ignore_comment();
   void ignore_ml_comment();
   bool is_identifier_char(char);
   bool is_operator_char(char);
   bool is_operator(std::string);
   bool is_number(char, bool);
   bool is_hex(char);
   bool is_keyword(std::string);

   bool is_punctuator(char);
   bool is_bool_literal(std::string);

   Lexer::LexerLoc getLexerLoc();
   void setLoc(Lexer::LexerLoc &loc);

   void commit();

   // pre processing

   struct ForScope {
      LexerLoc loc;
      string varName;
      std::vector<Variant>::iterator it;
      std::vector<Variant>::iterator end;
   };

   Variant parseExpression(Variant lhs = {}, int minPrecedence = 0);
   Variant currentTokenValue();

   unsigned openIfs = 0;
   std::stack<ForScope> ForStack;

   void handle_directive(string &&str);

   void handle_if(bool condition);

   void handle_rawif();
   void handle_ifdef();
   void handle_ifndef();

   void skip_until_directive(const string &dir);
   void skip_until_endif();

   void handle_define();
   void handle_undef();

   void handle_pragma();

   void handle_print();
   void handle_let();

   void handle_for();
   void handle_endfor();

   struct MacroExpansion {
      MacroExpansion(
         std::unique_ptr<Lexer> &&lex,
         llvm::MemoryBuffer *buff
      ) : lex(std::move(lex)), buff(buff)
      {

      }

      ~MacroExpansion() {
         delete buff;
      }

      std::unique_ptr<Lexer> lex;
      llvm::MemoryBuffer *buff;
   };

   unsigned macroExpansionDepth = 0;
   static std::stack<MacroExpansion> ExpansionStack;

   std::vector<Macro> getMacroArgs();

   void substitute_value();
   Token handlePreprocFunc(string &&macroName);

   void expand_macro(string &&macroName);
};


#endif //LEXER_H
