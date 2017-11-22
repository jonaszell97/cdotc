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

#ifndef CDOT_LEXER_STANDALONE

#define CDOT_DIAGNOSE(msg) diag::err(diag::err_generic_error) << msg \
   << this << diag::term;

#else

#define CDOT_DIAGNOSE(msg) do{}while(0);

#endif

using std::unordered_map;

namespace llvm {

class MemoryBuffer;

} // namespace llvm

class Parser;

namespace cdot {

enum class BuiltinMacro : unsigned;
extern unordered_map<string, pair<BuiltinMacro, int>> BuiltinMacros;

struct TemplateArg;

namespace util {

extern std::vector<string> assignmentOperators;
string isAssignmentOperator(const string &);

extern std::unordered_map<string, int> op_precedence;

extern std::vector<string> binary_operators;
extern std::vector<string> PrefixUnaryOperators;
extern std::vector<string> PostfixUnaryOperators;

extern std::vector<char> operator_chars;
extern std::vector<char> punctuators;

extern string token_names[];
extern std::vector<string> keywords;

} // namespace util

} // namespace cdot

struct LexerLoc {
   const char *curr;
   size_t currentIndex;
   size_t currentLine;
   size_t indexOnLine;
};


class Lexer {
public:
   Lexer();
   Lexer(
      llvm::MemoryBuffer *buf,
      string const& fileName,
      unsigned sourceId
   );

   explicit Lexer(std::vector<Token> &&tokens);

   enum Mode : unsigned char {
      Normal,
      TemplateConstraints, // parse typename, value and any as keywords
      TemplateArgMode, // parse >> as two seperate tokens
      MemberAccess // parse e.g. x.0.0 as two member accesses, instead of a
                   // '.' and a floating point literal
   };

   void setMode(Mode m)
   {
      mode = m;
   }


   // RAII utility classes

   class ModeScopeGuard {
   public:
      ModeScopeGuard(Mode m, Lexer *lex);
      ~ModeScopeGuard();

   protected:
      Lexer *lex;
      Mode prevMode;
   };

   ModeScopeGuard makeModeGuard(Mode m)
   {
      return ModeScopeGuard(m, this);
   }

   class StateSaveGuard {
   public:
      StateSaveGuard(Lexer *lex);
      ~StateSaveGuard();

   protected:
      Lexer *lex;
      size_t shrinkTo;
   };

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

   string strVal();
   string &strRef();

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

   Token currentToken;
   Token getToken(size_t index)
   {
      return tokens[index];
   }

   void pushToken(Token t)
   {
      tokens.push_back(t);
   }

   void setLine(size_t line)
   {
      currentLine = line;
   }

   void setCol(size_t col)
   {
      indexOnLine = col;
   }

   void setOffset(unsigned offset)
   {
      Lexer::offset = offset;
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

   struct IgnoreScope {
      IgnoreScope(Lexer *lex,
                  bool ignoreValue = true,
                  bool ignoreMacro = true,
                  bool ignoreDirective = true)
         : lex(lex), prevIgnoreVal(lex->ignoreValue),
           prevIgnoreMacro(lex->ignoreMacro),
           prevIgnoreDirective(lex->ignoreDirective)
      {
         lex->ignoreValue = ignoreValue;
         lex->ignoreMacro = ignoreMacro;
         lex->ignoreDirective = ignoreDirective;
      }

      ~IgnoreScope() {
         lex->ignoreValue = prevIgnoreVal;
         lex->ignoreMacro = prevIgnoreMacro;
         lex->ignoreDirective = prevIgnoreDirective;
      }

      Lexer *lex = nullptr;
      bool prevIgnoreVal;
      bool prevIgnoreMacro;
      bool prevIgnoreDirective;
   };

   void addTemplateArg(const string &name, const string &value);
   void addTemplateArg(const string &name, const std::vector<Token> &tokens);
   void addTemplateArg(const string &name, const Variant &val);

   void addVariadicTemplateArg(const string &name,
                               std::vector<Variant> &vec);


   static bool is_identifier_char(char);
   static bool is_operator_char(char);
   static bool is_number(char, bool);
   static bool is_hex(char);
   static bool is_punctuator(char);
   static bool is_bool_literal(const string&);

   bool is_keyword(const string&);
   bool is_operator(const string&);

   enum ParenKind {
      PAREN = '(',
      BRACE = '{',
      ANGLED = '<',
      SQUARE = '['
   };

   void skip_until_even(ParenKind kind);

   friend class IgnoreScope;

protected:
   struct Macro {
      const char *begin;
      size_t length;
      std::vector<string> args;
      LexerLoc loc;
   };

   static unordered_map<string, Macro> GlobalMacros;
   unordered_map<string, Macro> Macros;

   typedef std::vector<std::vector<Token>> VariadicTemplateArg;

   unordered_map<string, VariadicTemplateArg> VariadicTemplateArgs;
   unordered_map<string, std::vector<Token>> TemplateArgs;

   static unordered_map<string, Variant> GlobalValues;
   unordered_map<string, Variant> Values;

   std::vector<Token> tokens;
   std::vector<Token> lookaheadList;

   const char *curr;
   const char *begin;
   string fileName;
   unsigned offset = 0;

   Mode mode = Normal;

   unsigned sourceId = 0;

   bool ignore_interpolation = false;

   size_t srcLen = 0;

   bool ignoreMacro = false;
   bool ignoreDirective = false;
   bool ignoreValue = false;

   Token makeToken(TokenType ty, Variant &&val, size_t start, size_t end,
                   bool isEscaped = false);

   Token get_next_token(bool, bool);
   Token lex_next_token(bool, bool);

   void ignore_comment();
   void ignore_ml_comment();

   LexerLoc getLexerLoc();
   void setLoc(LexerLoc &loc);

   void commit();

   std::vector<Token> templateArgToTokens(const TemplateArg &templateArg);
   std::vector<Token> typeNameToTokens(const TemplateArg &templateArg);
   std::vector<Token> valueToTokens(const TemplateArg &templateArg);

   std::vector<Token> lexString(const string &s);

   // pre processing

   struct ForScope {
      struct Loc {
         size_t line;
         size_t col;
         size_t begin;
      } loc;
      string varName;
      std::vector<Variant> vec;
   };

   bool maybeClosingTemplateBracket(Token &next);
   Token valueToToken(SourceLocation loc, Variant &&val);
   Variant parseExpression(Variant lhs = {}, int minPrecedence = 0,
                           bool isTemplateArgument = false,
                           bool allowUndeclaredIdentifiers = false);

   Variant parseArray();
   Variant currentTokenValue(bool allowUndeclaredIdentifiers = false);

   unsigned openIfs = 0;
   std::stack<ForScope> ForStack;

   void tokenError();

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

   struct TemplateArgExpansion {
      std::vector<Token> tokens;
      size_t index = 0;
      size_t tokenIndex = 0;
   };

   unsigned macroExpansionDepth = 0;
   static std::stack<MacroExpansion> ExpansionStack;
   std::stack<TemplateArgExpansion> TemplateArgStack;

   std::vector<Macro> getMacroArgs();

   void substitute_value();
   Token handlePreprocFunc(string &&macroName);

   void expand_macro(string &&macroName);
   void expand_template_arg(string &&argName);
};


#endif //LEXER_H
