//
// Created by Jonas Zell on 18.06.17.
//

#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include <stack>
#include <llvm/Support/MemoryBuffer.h>

#include "Token.h"

#ifndef CDOT_LEXER_STANDALONE

#define CDOT_DIAGNOSE(msg) diag::err(diag::err_generic_error) << msg \
   << this << diag::term;

#else

#define CDOT_DIAGNOSE(msg) do{}while(0);

#endif

namespace llvm {

class MemoryBuffer;

} // namespace llvm

namespace cdot {

class TemplateArg;

namespace util {

extern std::vector<string> assignmentOperators;
string isAssignmentOperator(const string &);

extern llvm::StringMap<int> op_precedence;

extern std::vector<string> binary_operators;
extern std::vector<string> PrefixUnaryOperators;
extern std::vector<string> PostfixUnaryOperators;

extern std::vector<char> operator_chars;
extern std::vector<char> punctuators;

extern string token_names[];
extern std::vector<string> keywords;

} // namespace util

} // namespace cdot

namespace cdot {

namespace parse {
class Parser;
} // namespace parse

namespace diag {
class DiagnosticBuilder;
} // namespace parse

namespace module {
struct ModuleLexerTraits;
} // namespace module

namespace lex {

enum class BuiltinMacro : unsigned;
extern llvm::StringMap<pair<BuiltinMacro, int>> BuiltinMacros;

struct LispMacro;
struct LexerTraits;

struct ExpansionInfo {
   SourceLocation expandedMacroLoc;
   llvm::StringRef macroName;
   std::vector<std::pair<std::string, std::string>> expansionArgs;
};

ExpansionInfo *getExpansionInfo(SourceLocation loc);

template<class Traits = LexerTraits>
class Lexer {
public:
   enum TypeID {
      LexerID,
      MacroExpansionLexerID
   };

#ifdef NDEBUG
   using TokenVec   = llvm::SmallVector<Token, 256>;
   using PPTokenVec = llvm::SmallVector<Token, 64>;
#else
   using TokenVec   = std::vector<Token>;
   using PPTokenVec = std::vector<Token>;
#endif

   explicit Lexer(TypeID id = LexerID);
   Lexer(llvm::MemoryBuffer *buf,
         unsigned sourceId);

   explicit Lexer(std::vector<Token> &&tokens);

#ifndef NDEBUG
   virtual
#endif
   ~Lexer() = default;

   enum Mode : unsigned char {
      Normal,
      ForInStmt, // enable contextual 'in' keyword
      TemplateConstraints, // parse typename, value and any as keywords
      TemplateArgMode, // parse >> as two seperate tokens
      MemberAccess // parse e.g. x.0.0 as two member accesses, instead of a
                   // '.' and a floating point literal
   };

   void lex();

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

   template<class ...Rest>
   void expect(tok::TokenType ty, Rest... rest)
   {
      advance();
      expect_impl(ty, rest...);
   }

   template<class ...Rest>
   void expect_impl(tok::TokenType ty, Rest... rest)
   {
      if (currentTok()._type == ty) return;
      expect_impl(rest...);
   }

   void expect_impl(tok::TokenType ty);

   template <class ...Rest>
   bool advanceIf(tok::TokenType ty, Rest... rest)
   {
      if (currentTok()._type == ty) {
         advance();
         return true;
      }
      else {
         return advanceIf(rest...);
      }
   }

   bool advanceIf(tok::TokenType ty)
   {
      if (currentTok()._type == ty) {
         advance();
         return true;
      }

      return false;
   }

   void advance(bool ignoreNewLine = true, bool significantWhitespace = false);
   Token lookahead(bool ignoreNewline = true, bool sw = false, size_t i = 0);
   char get_next_char();
   char char_lookahead();
   void backtrack_c(int);
   void backtrack();

   void reset(const char *src, size_t len);

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

   size_t currentIndex = 0;
   size_t currentLine = 1;

   Token const& currentTok() const
   {
      return tokens[tokenIndex - 1];
   }

   SourceLocation getSourceLoc() const
   {
      return SourceLocation(currentIndex, sourceId);
   }

   void setIgnoreInterpolation(bool ignoreInterpolation)
   {
      Lexer::ignoreInterpolation = ignoreInterpolation;
   }

   void printTokensTo(llvm::raw_ostream &out);

   friend class parse::Parser;
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


   static bool is_identifier_char(char);
   static bool is_operator_char(char);
   static bool is_number(char, bool);
   static bool is_hex(char);
   static bool is_punctuator(char);

   enum ParenKind {
      PAREN = '(',
      BRACE = '{',
      ANGLED = '<',
      SQUARE = '['
   };

   void skip_until_even(ParenKind kind);

   llvm::ArrayRef<Token> getTokens() const
   {
      return tokens;
   }

   SourceLocation getSourceLoc();

   unsigned int getMacroExpansionDepth() const { return macroExpansionDepth; }

   unsigned int getSourceId() const { return sourceId; }

   TypeID getTypeID() const { return typeID; }

   static bool classof(Lexer const *T) { return true; }

   friend class cdot::diag::DiagnosticBuilder;

protected:
   struct Macro {
      std::vector<Token> tokens;
      std::vector<string> args;
   };

   bool skipCommentIfPresent(char c);
   Token lexStringLiteral(unsigned startIndex, bool preprocessing);

   TypeID typeID;

   llvm::StringMap<Macro> Macros;
   llvm::StringMap<Variant> Values;

   TokenVec tokens;
   size_t srcLen = 0;
   unsigned sourceId = 0;
   size_t tokenIndex = 0;

   const char *curr;
   const char *begin;

   Mode mode = Normal;

   bool ignoreMacro : 1;
   bool ignoreDirective : 1;
   bool ignoreValue : 1;
   bool doneLexing : 1;
   bool ignoreInterpolation : 1;

   Lexer *Outer = nullptr;
   unsigned expandedFromMacro = 0;
   unsigned offset = 0;

   Token makeToken(tok::TokenType ty, Variant &&val, size_t offset);

   Token getNextToken(bool ignoreNewline = false);
   Token getNextToken(const Token &tok, bool ignoreNewline = false);
   Token lex_next_token(bool preprocessing = false);

   void ignore_comment(const char *End);

   bool hasMacro(llvm::StringRef name);
   Macro &getMacro(llvm::StringRef name);

   // pre processing

   struct ForScope {
      size_t idx;
      string varName;
      std::vector<Variant> vec;
   };

   bool maybeClosingTemplateBracket(const Token &next);
   Token valueToToken(SourceLocation loc, Variant &&val);
   Variant parseExpression(Variant lhs = {}, int minPrecedence = 0,
                           bool allowUndeclaredIdentifiers = false);

   Variant parseArray();
   Variant currentTokenValue(bool allowUndeclaredIdentifiers = false);

   unsigned openIfs = 0;
   std::stack<ForScope> ForStack;

   void tokenError();

   void lexUntilClosingParen(unsigned beginAt = 1);
   void lexUntilClosingBrace();
   void lexUntil(tok::TokenType type);

   Token& ppCurrentTok();

   PPTokenVec preprocessingTokens;
   size_t ppTokenIndex = 0;

   Token pp_lookahead(bool ignoreNewline = true, unsigned offset = 0);
   void pp_advance(bool ignoreNewline = true, bool sw = false);
   void pp_backtrack();
   void pp_finish();

   void handle_directive(tok::TokenType kind);

   void handle_if(bool condition);

   void handle_rawif();
   void handle_ifdef();
   void handle_ifndef();

   void skip_until_token(tok::TokenType kind);
   void skip_until_endif();

   void handle_define();
   void handle_undef();

   void handle_pragma();

   void handle_print();
   void handle_let();

   void handle_for();
   void handle_endfor();

   void handle_using();
   void handle_namespace();
   void handle_endnamespace();

   std::vector<std::string> currentNamespace;
   std::vector<std::string> importedNamespaces;

   void appendNamespacePrefix(llvm::SmallString<128> &str);

   TokenVec handle_include();

   unsigned macroExpansionDepth = 0;

   typedef std::pair<std::vector<Macro>, std::vector<string>> MacroArgList;

   MacroArgList getMacroArgs();

   Token handlePreprocFunc(const string &macroName);

   TokenVec expand_macro(const string &macroName);

   static llvm::StringMap<LispMacro> LispMacros;

   void getPotentiallyQualifiedName(llvm::SmallString<128> &initial);
   size_t getCurrentIndent();

   LispMacro *tryGetLispMacro(llvm::StringRef name, bool recurse = true);

   void parse_lisp_macro();
   TokenVec expand_lisp_macro(LispMacro *Ptr,
                              SourceLocation beginLoc);
};

template<class Traits = LexerTraits>
class MacroExpansionLexer: public Lexer<Traits> {
   using Super        = Lexer<Traits>;
   using Macro        = typename Super::Macro;
   using MacroArgList = typename Super::MacroArgList;

public:
   MacroExpansionLexer(const Macro &M, Super *Outer,
                       unsigned offset, MacroArgList &macroArgs);

   MacroExpansionLexer(typename Super::TokenVec &&tokens, unsigned offset,
                       Super *Outer);

   Token lex_next_token();

   static bool classof(Super const *T)
   {
      return T->getTypeID() == Super::TypeID::MacroExpansionLexerID;
   }

   friend class Lexer<Traits>;

private:
   const std::vector<Token> &prelexedTokens;
   size_t lexedIndex = 0;
};

struct LexerTraits {
   static tok::TokenType getKeywordToken(const string &potentialKeyword);
   static tok::TokenType getOperatorToken(const string &potentialOperator);
   static tok::TokenType getPunctuatorToken(char potentialPunctuator);

   static const char* CommentBegin;
   static const char* CommentEnd;

   static const char* MultiLineCommentBegin;
   static const char* MultiLineCommentEnd;
};

extern template class Lexer<module::ModuleLexerTraits>;
extern template class Lexer<LexerTraits>;

extern template class MacroExpansionLexer<module::ModuleLexerTraits>;
extern template class MacroExpansionLexer<LexerTraits>;

} // namespace lex

namespace module {

struct ModuleLexerTraits {
   static lex::tok::TokenType getKeywordToken(const string &potentialKeyword);
   static lex::tok::TokenType getOperatorToken(const string &potentialOperator);
   static lex::tok::TokenType getPunctuatorToken(char potentialPunctuator);

   static const char* CommentBegin;
   static const char* CommentEnd;

   static const char* MultiLineCommentBegin;
   static const char* MultiLineCommentEnd;
};

} // namespace module

} // namespace cdot

#endif //LEXER_H
