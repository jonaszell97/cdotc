//
// Created by Jonas Zell on 18.10.17.
//

#include "Lexer.h"
#include "../Files/FileManager.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSet.h>
#include <sstream>
#include <iomanip>
#include <cassert>

#ifndef CDOT_LEXER_STANDALONE
#include "../Message/Diagnostics.h"
#include "../Files/FileManager.h"
#include "Token.h"

#endif

namespace cdot {
namespace lex {

enum class BuiltinMacro: unsigned {
   TOLOWER,
   TOUPPER,
   TOSTRING,
   NUMFORMAT,
   DEFINED,
   REPEAT,
   LINE,
   FILE,
   ParseInt,
   ParseFloat,
};

llvm::StringMap<pair<BuiltinMacro, int>> BuiltinMacros = {
   { "_ToLower",   { BuiltinMacro::TOLOWER,    1 }},
   { "_ToUpper",   { BuiltinMacro::TOUPPER,    1 }},
   { "_ParseInt",  { BuiltinMacro::ParseInt,   1 }},
   { "_ParseFloat",{ BuiltinMacro::ParseFloat, 1 }},
   { "_NumFormat", { BuiltinMacro::NUMFORMAT,  2 }},
   { "_Repeat",    { BuiltinMacro::REPEAT,     2 }},
   { "__LINE__",   { BuiltinMacro::LINE,       0 }},
   { "__FILE__",   { BuiltinMacro::FILE,       0 }}
};

namespace {

//region Description
llvm::StringMap<Variant> BuiltinValues = {
   { "_WordSize", Variant((unsigned long long) (sizeof(size_t) * 8)) },
   { "__DEBUG__", Variant(true) },
   { "_CLOCKS_PER_SEC", Variant((unsigned long long) CLOCKS_PER_SEC) },
   { "EOF", Variant((unsigned long long) EOF) },
   { "SEEK_SET", Variant((unsigned long long) SEEK_SET) },
   { "SEEK_END", Variant((unsigned long long) SEEK_END) },
   { "SEEK_CUR", Variant((unsigned long long) SEEK_CUR) },
#ifdef _WIN32
   { "_WIN32", Variant(true) },
#else
   { "_WIN32", Variant(false) },
#endif
#ifdef _WIN64
   { "_WIN64", Variant(true) },
#else
   { "_WIN64", Variant(false) },
#endif
#if defined(unix) || defined(__unix) || defined(__unix__)
{ "unix", Variant(true) },
      { "__unix", Variant(true) },
      { "__unix__", Variant(true) },
#else
   { "unix", Variant(false) },
   { "__unix", Variant(false) },
   { "__unix__", Variant(false) },
#endif
#if defined(__APPLE__) || defined(__MACH__)
   { "__APPLE__", Variant(true) },
   { "__MACH__", Variant(true) },
#else
{ "__APPLE__", Variant(false) },
      { "__MACH__", Variant(false) },
#endif
#if defined(linux) || defined(__linux) || defined(__linux__)
{ "linux", Variant(true) },
      { "__linux", Variant(true) },
      { "__linux__", Variant(true) },
#else
   { "linux", Variant(false) },
   { "__linux", Variant(false) },
   { "__linux__", Variant(false) },
#endif
#ifdef __FreeBSD__
   { "__FreeBSD__", Variant(true) },
#else
   { "__FreeBSD__", Variant(false) },
#endif
   { "__builtin_eh_data_regno_0",
      Variant((unsigned long long)__builtin_eh_return_data_regno(0)) },
   { "__builtin_eh_data_regno_1",
      Variant((unsigned long long)__builtin_eh_return_data_regno(1)) }
};

llvm::SmallDenseMap<size_t, ExpansionInfo> MacroExpansionInfo;

} // anonymous namespace

template<class T>
llvm::StringMap<LispMacro> Lexer<T>::LispMacros;

ExpansionInfo *getExpansionInfo(SourceLocation loc)
{
   auto it = MacroExpansionInfo.find(loc.getSourceId());
   if (it == MacroExpansionInfo.end())
      return nullptr;

   return &it->second;
}

template<class T>
Variant Lexer<T>::currentTokenValue(bool allowUndeclaredIdentifiers)
{
   if (ppCurrentTok().is_literal()) {
      auto val = std::move(ppCurrentTok()._value);
      if (pp_lookahead().is(tok::sentinel))
         pp_advance();

      return val;
   }

   if (ppCurrentTok().is_operator()) {
      auto op = ppCurrentTok().toString();
      auto next = pp_lookahead();
      if (next.oneOf(tok::integerliteral, tok::fpliteral,
                     tok::stringliteral, tok::ident, tok::open_paren,
                     tok::open_square)) {
         pp_advance();
         auto target = currentTokenValue(allowUndeclaredIdentifiers);

         return target.applyUnaryOp(op);
      }

      return Variant(ppCurrentTok().getKind());
   }

   if (ppCurrentTok().is_identifier()) {
      auto &ident = ppCurrentTok()._value.getString();
      if (BuiltinValues.find(ident) != BuiltinValues.end()) {
         return BuiltinValues[ident];
      }
      if (Values.find(ident) != Values.end()) {
         auto &val = Values[ident];

         auto next = pp_lookahead();
         if (next.is(tok::open_square)) {
            if (!val.isArray())
               CDOT_DIAGNOSE("subscripted value is not an array");

            pp_advance();
            pp_advance();

            auto idx = parseExpression();
            if (idx.getKind() != VariantType::INT)
               CDOT_DIAGNOSE("expected integer as array index");

            if (idx.intVal >= val.vec.size())
               CDOT_DIAGNOSE("array index out of bounds (index is "
                             + std::to_string(idx.intVal) + ", array has "
                             + std::to_string(val.vec.size())
                             + " elements)");

            expect(tok::close_square);
            return val[idx.intVal];
         }

         return val;
      }
      if (allowUndeclaredIdentifiers) {
         return ppCurrentTok()._value;
      }
      if (BuiltinMacros.find(ident) != BuiltinMacros.end()) {
         auto tok = handlePreprocFunc(move(ident));
         return tok._value;
      }

      CDOT_DIAGNOSE("undeclared identifier " + ident);
   }

   if (ppCurrentTok().is_punctuator()) {
      if (ppCurrentTok().is(tok::open_paren)) {
         pp_advance();
         auto expr = parseExpression({ }, 0, allowUndeclaredIdentifiers);

         pp_advance();
         if (!ppCurrentTok().is(tok::close_paren))
            CDOT_DIAGNOSE("expected ')'");

         return expr;
      }
      if (ppCurrentTok().is(tok::open_square)) {
         return parseArray();
      }
   }

   if (ppCurrentTok().oneOf(tok::kw_true, tok::kw_false)) {
      return Variant(ppCurrentTok().getKind() == tok::kw_true);
   }

   CDOT_DIAGNOSE("Unexpected token in preprocessor expression: "
                 + ppCurrentTok().toString());
   llvm_unreachable(0);
}

template<class T>
Variant Lexer<T>::parseArray()
{
   std::vector<Variant> vec;
   while (!ppCurrentTok().is(tok::close_square)) {
      pp_advance();
      if (ppCurrentTok().is(tok::comma)) {
         continue;
      }
      if (ppCurrentTok().is(tok::close_square)) {
         break;
      }

      vec.push_back(currentTokenValue(true));
   }

   return { std::move(vec) };
}

template<class T>
bool Lexer<T>::maybeClosingTemplateBracket(const Token &next)
{
   if (next.is_literal())
      return false;
   if (next.is(tok::ident))
      return false;

   return !next.is_operator();
}

template<class T>
Variant Lexer<T>::parseExpression(Variant lhs,
                                  int minPrecedence,
                                  bool allowUndeclaredIdentifiers) {
   if (lhs.isVoid()) {
      lhs = currentTokenValue(allowUndeclaredIdentifiers);
   }

   bool maybeTemplateArg = mode == TemplateArgMode;
   auto next = pp_lookahead(false);

   // if no '>' follows on the same line, parse as closing angled brace
   while ((next.is_operator() || (next.is_identifier()
                                 && next.getValue().strVal == ".."))
          && util::op_precedence[next.getValue().strVal] >= minPrecedence) {
      pp_advance();
      if (maybeTemplateArg && maybeClosingTemplateBracket(pp_lookahead())) {
         pp_backtrack();
         break;
      }

      string op = ppCurrentTok().rawRepr();

      pp_advance();
      auto rhs = currentTokenValue(allowUndeclaredIdentifiers);

      next = pp_lookahead(false);
      while (next.is_operator()
             && util::op_precedence[next.getValue().strVal]
                > util::op_precedence[op]) {
         pp_advance(false);
         if (maybeTemplateArg && maybeClosingTemplateBracket(pp_lookahead())) {
            pp_backtrack();
            break;
         }

         rhs = parseExpression(rhs,
                           util::op_precedence[ppCurrentTok().getValue().strVal]);
      }

      lhs = lhs.applyBinaryOp(rhs, op);
   }

   if (next.is(tok::period) && lhs.kind == VariantType::STRING) {
      pp_advance();
      pp_advance();

      lhs = Variant(lhs.strVal + '.'
                    + parseExpression({ }, 0,
                                      allowUndeclaredIdentifiers).toString());
   }

   if (next.is(tok::close_brace)) {
      pp_advance();
   }

   return lhs;
}

template<class T>
Token Lexer<T>::valueToToken(SourceLocation loc, Variant &&val)
{
   Token tok;
   if (val.isStr()) {
      tok._type = tok::ident;
   }
   else {
      switch (val.kind) {
         case VariantType::INT:
            switch (val.getBitwidth()) {
               case 1:
                  if (val.intVal)
                     tok._type = tok::kw_true;
                  else
                     tok._type = tok::kw_false;
                  break;
               case 8:
                  tok._type = tok::charliteral;
                  break;
               default:
                  tok._type = tok::integerliteral;
                  break;
            }
            break;
         case VariantType::FLOAT:
            tok._type = tok::fpliteral;
            break;
         case VariantType::TokenKind:
            tok._type = val.getTokenType();
            break;
         default:
            tok._type = tok::stringliteral;
            break;
      }
   }

   tok._value = Variant(std::move(val));
   tok.loc = loc;

   return tok;
}

template<class T>
void Lexer<T>::handle_directive(tok::TokenType kind)
{
   switch (kind) {
      case tok::pound_define: return handle_define();
      case tok::pound_macro: return parse_lisp_macro();
      case tok::pound_undef: return handle_undef();
      case tok::pound_if: return handle_rawif();
      case tok::pound_ifdef: return handle_ifdef();
      case tok::pound_ifndef: return handle_ifndef();
      case tok::pound_endif:
         assert(openIfs);
         --openIfs;
         break;
      case tok::pound_else:
      case tok::pound_elseif:
         return skip_until_endif();
      case tok::pound_pragma: return handle_pragma();
      case tok::pound_for: return handle_for();
      case tok::pound_endfor: return handle_endfor();
      case tok::pound_print: return handle_print();
      case tok::pound_let: return handle_let();
      case tok::pound_using: return handle_using();
      case tok::pound_namespace: return handle_namespace();
      case tok::pound_endnamespace: return handle_endnamespace();
      default:
         llvm_unreachable("bad directive");
   }
}

template<class T>
void Lexer<T>::handle_define()
{
   IgnoreScope guard(this);

   pp_advance();

   string name = move(ppCurrentTok()._value.getString());
   std::vector<string> args;

   if (pp_lookahead(false).is(tok::newline)) {
      Macros.try_emplace(name, Macro());
      return;
   }

   pp_advance();
   if (ppCurrentTok().is(tok::open_paren)) {
      pp_advance();

      while (!ppCurrentTok().is(tok::close_paren)) {
         args.push_back(ppCurrentTok()._value.getString());
         pp_advance();
         if (ppCurrentTok().is(tok::comma)) {
            pp_advance();
         }
      }

      pp_advance();
   }

   std::vector<Token> macroTokens;
   while (1) {
      pp_advance(false, true);
      if (ppCurrentTok().is(tok::newline))
         break;

      macroTokens.emplace_back(std::move(ppCurrentTok()));
   }

   Macros.try_emplace(name, Macro { move(macroTokens), move(args) });
}

template<class T>
void Lexer<T>::handle_undef()
{
   IgnoreScope guard(this);
   pp_advance();

   auto name = ppCurrentTok()._value.getString();
   auto index = Macros.find(name);
   if (index != Macros.end()) {
      Macros.erase(index);
   }
}

template<class T>
void Lexer<T>::tokenError()
{
   CDOT_DIAGNOSE("unexpected token " + ppCurrentTok().toString());
}

template<class T>
void Lexer<T>::handle_let()
{
   pp_advance();

   auto name = ppCurrentTok()._value.getString();

   pp_advance();
   if (!ppCurrentTok().is(tok::equals))
      tokenError();

   pp_advance();

   Values[name] = parseExpression();
}

template<class T>
void Lexer<T>::handle_print()
{
   pp_advance();

   auto expr = parseExpression();
   llvm::outs() << expr.toString() << "\n";
}

template<class T>
void Lexer<T>::handle_pragma()
{

}

template<class T>
void Lexer<T>::handle_rawif()
{
   pp_advance();
   auto cond = parseExpression();

   if (cond.kind != VariantType::INT) {
      CDOT_DIAGNOSE("expected boolean expression");
   }

   handle_if(cond.intVal != 0);
}

template<class T>
void Lexer<T>::handle_ifdef()
{
   IgnoreScope guard(this);
   pp_advance();

   handle_if(hasMacro(ppCurrentTok()._value.getString()));
}

template<class T>
void Lexer<T>::handle_ifndef()
{
   IgnoreScope guard(this);
   pp_advance();

   handle_if(hasMacro(ppCurrentTok()._value.getString()));
}

template<class T>
void Lexer<T>::handle_if(bool condition)
{
   if (condition) {
      ++openIfs;
      return;
   }

   unsigned innerIfs = 0;
   for (;;) {
      pp_advance();
      while (!ppCurrentTok().is(tok::eof) && !ppCurrentTok().is_directive()) {
         pp_advance();
      }

      if (ppCurrentTok().is(tok::eof))
         CDOT_DIAGNOSE("unexpected end of file, expected #endif");

      auto dir = ppCurrentTok().getKind();
      if (dir == tok::pound_if) {
         ++innerIfs;
      }
      else if (!innerIfs && dir == tok::pound_elseif) {
         lexUntil(tok::newline);
         return handle_rawif();
      }
      else if (!innerIfs && dir == tok::pound_else) {
         ++openIfs;
         return;
      }
      else if (dir == tok::pound_endif) {
         if (innerIfs) {
            --innerIfs;
         }
         else {
            return;
         }
      }
   }
}

template<class T>
void Lexer<T>::skip_until_token(tok::TokenType kind)
{
   while (!ppCurrentTok().is(kind))
      pp_advance(false, true);
}

// only called after an if / elseif that was entered
template<class T>
void Lexer<T>::skip_until_endif()
{
   pp_advance();

   if (!openIfs) {
      tokenError();
   }

   skip_until_token(tok::pound_endif);
}

template<class T>
void Lexer<T>::handle_for()
{
   pp_advance();

   auto elName = ppCurrentTok()._value.getString();
   pp_advance();

   if (Values.find(elName) != Values.end()) {
      CDOT_DIAGNOSE("variable " + elName + " already defined");
   }

   assert(ppCurrentTok()._value.strVal == "in");
   pp_advance();

   auto arr = parseExpression();
   if (!arr.isArray()) {
      CDOT_DIAGNOSE("#for-in requires array type");
   }

   auto offset = pp_lookahead().getOffset();

   if (arr.vec.empty()) {
      skip_until_token(tok::pound_endfor);
      return;
   }

   std::vector<Variant> reversedVec;
   for (auto i = arr.vec.size() - 1;; --i) {
      reversedVec.push_back(arr[i]);
      if (i == 0) {
         break;
      }
   }

   Values[elName] = std::move(reversedVec.back());
   reversedVec.pop_back();

   ForStack.push(ForScope {
      offset, elName, std::move(reversedVec)
   });
}

template<class T>
void Lexer<T>::handle_endfor()
{
   if (ForStack.empty()) {
      tokenError();
   }

   auto &latest = ForStack.top();

   if (!latest.vec.empty()) {
      curr = begin + latest.idx;
      currentIndex = latest.idx;

      Values[latest.varName] = std::move(latest.vec.back());
      latest.vec.pop_back();

      return;
   }

   ForStack.pop();
   Values.erase(Values.find(latest.varName));
}

template<class T>
typename Lexer<T>::MacroArgList Lexer<T>::getMacroArgs()
{
   std::vector<Macro> args;
   std::vector<string> argStrings;

   if (!pp_lookahead().is(tok::open_paren))
      return std::make_pair(args, argStrings);

   unsigned openParens = 1;
   unsigned closeParens = 0;

   string str;
   std::vector<Token> currentArg;

   pp_advance();

   while (1) {
      switch (ppCurrentTok().getKind()) {
         case tok::comma:
            args.push_back(Macro { move(currentArg) });
            currentArg.clear();

            argStrings.push_back(str);
            str.clear();

            pp_advance();

            continue;
         case tok::open_paren:
            ++openParens;
            break;
         case tok::close_paren:
            ++closeParens;
            break;
         case tok::eof:
            CDOT_DIAGNOSE("unexpected end of file");
         default:
            break;
      }

      if (openParens == closeParens)
         break;

      str += ppCurrentTok().rawRepr();
      currentArg.push_back(std::move(ppCurrentTok()));

      pp_advance();
   }

   if (!currentArg.empty()) {
      args.push_back(Macro { move(currentArg) });
      argStrings.push_back(str);
   }

   return std::make_pair(args, argStrings);
}

template<class T>
typename Lexer<T>::TokenVec Lexer<T>::expand_macro(const string &macroName)
{
   if (macroExpansionDepth > 256) {
      CDOT_DIAGNOSE("max recursion depth reached");
   }

   auto offset = currentIndex - macroName.length();
   auto args = getMacroArgs();

   auto M = getMacro(macroName);

   if (args.first.size() != M.args.size()) {
      CDOT_DIAGNOSE("wrong number of arguments passed to macro "
                    + macroName);
   }

   MacroExpansionLexer<T> lex(M, this, offset, args);
   lex.lex();

   return std::move(lex.tokens);
}

template<class T>
Token Lexer<T>::handlePreprocFunc(const string &macroName)
{
   auto macro = BuiltinMacros[macroName];
   std::vector<Variant> args;

   if (pp_lookahead().is(tok::open_paren)) {
      pp_advance();
      pp_advance();

      while (true) {
         args.push_back(currentTokenValue());

         if (pp_lookahead().is(tok::comma)) {
            pp_advance();
            pp_advance();
         }
         else if (pp_lookahead().oneOf(tok::close_paren, tok::eof)) {
            pp_advance();
            break;
         }
      }
   }

   if (args.size() < macro.second) {
      CDOT_DIAGNOSE("wrong number of arguments passed to macro " + macroName);
   }

   switch (macro.first) {
      case BuiltinMacro::TOLOWER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::tolower);

         auto tok = ppCurrentTok();
         tok._type = tok::ident;
         tok._value = Variant(std::move(str));

         return tok;
      }
      case BuiltinMacro::TOUPPER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::toupper);

         auto tok = ppCurrentTok();
         tok._type = tok::ident;
         tok._value = Variant(std::move(str));

         return tok;
      }
      case BuiltinMacro::TOSTRING: {
         auto tok = ppCurrentTok();
         tok._type = tok::stringliteral;
         tok._value = Variant(args.front().toString());

         return tok;
      }
      case BuiltinMacro::NUMFORMAT: {
         if (args.front().kind != VariantType::INT
             || args[1].kind != VariantType::INT) {
            CDOT_DIAGNOSE("_NumFormat expects integers as 1st and "
                             "2nd arguments");
         }

         auto &val = args.front().intVal;
         auto &base = args[1].intVal;

         std::ostringstream out;
         out << std::setbase((int) base) << val;

         auto tok = ppCurrentTok();
         tok._type = tok::ident;
         tok._value = Variant(out.str());

         return tok;
      }
      case BuiltinMacro::ParseInt: {
         int base = 10;
         if (args.front().kind != VariantType::STRING) {
            CDOT_DIAGNOSE("_ParseInt expects string argument");
         }
         if (args.size() > 1) {
            if (args[1].kind != VariantType::INT) {
               CDOT_DIAGNOSE("_ParseInt expects int as 2nd argument");
            }

            base = (int)args[1].intVal;
         }

         size_t val = strtoull(args.front().strVal.c_str(), nullptr, base);

         auto tok = ppCurrentTok();
         tok._type = tok::integerliteral;
         tok._value = Variant(val);

         return tok;
      }
      case BuiltinMacro::ParseFloat: {
         if (args.front().kind != VariantType::STRING) {
            CDOT_DIAGNOSE("_ParseInt expects string argument");
         }

         double val = strtod(args.front().strVal.c_str(), nullptr);

         auto tok = ppCurrentTok();
         tok._type = tok::fpliteral;
         tok._value = Variant(val);

         return tok;
      }
      case BuiltinMacro::REPEAT: {
         auto &str = args.front().strVal;

         assert(args[1].kind == VariantType::INT);
         auto &arg2 = args[1].intVal;

         string res;
         for (unsigned i = 0; i < arg2; ++i) {
            res += str;
         }

         auto tok = ppCurrentTok();
         tok._type = tok::ident;
         tok._value = Variant(std::move(res));

         return tok;
      }
      case BuiltinMacro::FILE: {
         return makeToken(tok::stringliteral,
                          Variant(fs::FileManager::getFileName(sourceId).str()),
                          currentIndex);
      }
      case BuiltinMacro::LINE: {
         return makeToken(tok::integerliteral,
                          Variant(currentLine),
                          currentIndex);
      }
      default:llvm_unreachable("unknown macro");
   }
}

template <class T>
typename Lexer<T>::TokenVec Lexer<T>::handle_include()
{
   pp_advance();
   if (ppCurrentTok().isNot(tok::stringliteral))
      tokenError();

   auto &fileName = ppCurrentTok()._value.getString();
   auto Buf = fs::FileManager::openFile(fileName, true);

   Lexer lex(Buf.second.get(), Buf.first);
   lex.Outer = this;
   lex.lex();

   return std::move(lex.tokens);
}

struct LispMacro {
   struct Fragment {
      enum Kind {
         SequencePoint,
         Variable
      };

      explicit Fragment(Token &&tok)
         : kind(SequencePoint), tok(std::move(tok))
      {}

      explicit Fragment(std::string &&varName)
         : kind(Variable), varName(move(varName))
      {}

      Fragment(Fragment &&F) noexcept
         : kind(F.kind)
      {
         if (isSequencePoint())
            new (&tok) Token(std::move(F.tok));
         else
            new (&varName) std::string(F.varName);
      }

      Fragment &operator=(Fragment &&F) noexcept
      {
         destroyValue();
         new(this) Fragment(std::move(F));

         return *this;
      }

      ~Fragment() { destroyValue(); }

      bool isSequencePoint() const { return kind == SequencePoint; }
      bool isVariable()      const { return kind == Variable; }

      Token const& getTok() const
      {
         assert(isSequencePoint());
         return tok;
      }

      std::string const& getVarName() const
      {
         assert(isVariable());
         return varName;
      }

   private:
      Kind kind;
      union {
         Token tok;
         std::string varName;
      };

      void destroyValue()
      {
         if (isSequencePoint())
            tok.~Token();
         else
            varName.~string();
      }
   };

#ifdef NDEBUG
   using PatternVec = llvm::SmallVector<Fragment, 8>;
   using TokenVec   = llvm::SmallVector<Token, 8>;
#else
   using PatternVec = std::vector<Fragment>;
   using TokenVec   = std::vector<Token>;
#endif

   class StateMachine {
   public:
      explicit StateMachine(PatternVec const& pattern)
         : currentState(-1), pattern(pattern)
      {

      }

      bool moveNext(Token &tok)
      {
         assert(hasNext());
         auto &next = lookahead();

         // anything matches a variable
         if (next.isVariable()) {
            assert((currentState == -1 || !current().isVariable())
                   && "two variables after one another");

            advance();
            addVariable(tok);

            return true;
         }

         bool skipPattern = false;
         if (openParens && tok.is(tok::close_paren)) {
            skipPattern = next.getTok().is(tok::close_paren);
            --openParens;
         }
         else if (openBraces && tok.is(tok::close_brace)) {
            skipPattern = next.getTok().is(tok::close_brace);
            --openBraces;
         }
         else if (openSquare && tok.is(tok::close_square)) {
            skipPattern = next.getTok().is(tok::close_square);
            --openSquare;
         }

         if (!skipPattern && patternCompatible(tok, next.getTok())) {
            advance();
            return true;
         }

         if (currentState != -1 && current().isVariable()) {
            addVariable(tok);
            return true;
         }

         return tok.is(tok::space);
      }

      bool hasNext()
      {
         return currentState + 1 < pattern.size();
      }

      llvm::StringMap<TokenVec> &getVariableTokens()
      {
         return variableTokens;
      }

   private:
      long long currentState;
      PatternVec const& pattern;
      llvm::StringMap<TokenVec> variableTokens;

      unsigned openParens = 0;
      unsigned openBraces = 0;
      unsigned openSquare = 0;

      Fragment const& current()
      {
         return pattern[currentState];
      }

      Fragment const& lookahead()
      {
         assert(hasNext());
         return pattern[currentState + 1];
      }

      void advance()
      {
         ++currentState;
      }

      bool patternCompatible(Token const& given, Token const& needed)
      {
         if (given.getKind() != needed.getKind())
            return false;

         if (given.oneOf(tok::ident, tok::stringliteral))
            return given._value.getString() == needed._value.getString();

         if (given.is(tok::integerliteral))
            return given._value.getInt() == needed._value.getInt();

         if (given.is(tok::fpliteral))
            return given._value.getDouble() == needed._value.getDouble();

         return true;
      }

      void addVariable(Token &tok)
      {
         if (tok.is(tok::open_paren))
            ++openParens;
         else if (tok.is(tok::open_brace))
            ++openBraces;
         else if (tok.is(tok::open_square))
            ++openSquare;

         auto &varName = current().getVarName();
         variableTokens[varName].emplace_back(std::move(tok));
      }
   };

   LispMacro(string &&name, PatternVec &&pattern,
             TokenVec &&expansionTokens)
      : name(std::move(name)), pattern(std::move(pattern)),
        expansionTokens(std::move(expansionTokens))
   { }

   llvm::StringRef getName() const
   {
      return name;
   }

   const PatternVec &getPattern() const
   {
      return pattern;
   }

   const TokenVec &getExpansionTokens() const
   {
      return expansionTokens;
   }

   StateMachine makeStateMachine() const
   {
      return StateMachine(pattern);
   }

private:
   std::string name;
   PatternVec pattern;
   TokenVec expansionTokens;
};

template <class T>
void Lexer<T>::handle_using()
{
   pp_advance();
   if (!ppCurrentTok().is(tok::ident))
      CDOT_DIAGNOSE("expected identifier");

   llvm::SmallString<128> namespaceName;
   namespaceName += ppCurrentTok()._value.getString();
   getPotentiallyQualifiedName(namespaceName);

   importedNamespaces.push_back(namespaceName.str().str());
}

template <class T>
void Lexer<T>::handle_namespace()
{
   pp_advance();
   if (!ppCurrentTok().is(tok::ident))
      CDOT_DIAGNOSE("expected identifier");

   llvm::SmallString<128> namespaceName;
   namespaceName += ppCurrentTok()._value.getString();
   getPotentiallyQualifiedName(namespaceName);

   currentNamespace.push_back(namespaceName.str().str());
}

template <class T>
void Lexer<T>::handle_endnamespace()
{
   if (currentNamespace.empty())
      CDOT_DIAGNOSE("no open namespace to end");

   if (pp_lookahead(false).is(tok::ident)) {
      pp_advance();

      llvm::SmallString<128> namespaceName;
      namespaceName += ppCurrentTok()._value.getString();
      getPotentiallyQualifiedName(namespaceName);

      if (!namespaceName.equals(currentNamespace.back()))
         CDOT_DIAGNOSE("cannot end namespace " + namespaceName.str() + " before"
            " ending '" + currentNamespace.back() + "'");
   }

   currentNamespace.pop_back();
}

template <class T>
void Lexer<T>::appendNamespacePrefix(llvm::SmallString<128> &str)
{
   for (const auto &ns : currentNamespace) {
      str += ns;
      str += '.';
   }
}

template <class T>
LispMacro *Lexer<T>::tryGetLispMacro(llvm::StringRef name, bool recurse)
{
   auto it = LispMacros.find(name);
   if (it != LispMacros.end())
      return &it->second;

   llvm::SmallString<128> macroName;
   for (const auto &import : importedNamespaces) {
      macroName += import;
      macroName += '.';
      macroName += name;

      it = LispMacros.find(macroName.str());
      if (it != LispMacros.end())
         return &it->second;

      macroName.clear();
   }

   if (!recurse)
      return Outer ? Outer->tryGetLispMacro(name) : nullptr;

   llvm::SmallString<128> namespacePrefix;
   for (const auto &import : currentNamespace) {
      namespacePrefix += import;
      namespacePrefix += '.';

      macroName += namespacePrefix.str();
      macroName += name;

      if (auto M = tryGetLispMacro(macroName.str(), false))
         return M;

      macroName.clear();
   }

   return Outer ? Outer->tryGetLispMacro(name) : nullptr;
}

template <class T>
void Lexer<T>::getPotentiallyQualifiedName(llvm::SmallString<128> &initial) {
   while (pp_lookahead(false).is(tok::period)) {
      pp_advance(false);
      pp_advance();

      if (!ppCurrentTok().is(tok::ident))
         CDOT_DIAGNOSE("expected identifier");

      initial += '.';
      initial += ppCurrentTok()._value.getString();
   }
}

template <class T>
size_t Lexer<T>::getCurrentIndent()
{
   if (tokens.empty())
      return 0;

   size_t i = tokens.size() - 1;
   while (1) {
      if (tokens[i].is(tok::space)) {
         if (i == 0 || tokens[i - 1].is(tok::newline)) {
            return tokens[i]._value.getInt();
         }
      }

      if (i == 0)
         break;

      --i;
   }

   return 0;
}

template <class T>
void Lexer<T>::parse_lisp_macro()
{
   size_t baseIndent = 0;
   if (!tokens.empty() && tokens.back().is(tok::space)) {
      baseIndent = tokens.back()._value.getInt();
   }

   pp_advance(); // #macro

   llvm::SmallString<128> macroName;
   appendNamespacePrefix(macroName);

   macroName += ppCurrentTok()._value.getString();
   getPotentiallyQualifiedName(macroName);

   typename LispMacro::PatternVec pattern;
   llvm::StringSet<> variables;
   bool lastWasVariable = false;

   pp_advance(false, true);
   while (!ppCurrentTok().is(tok::newline)) {
      if (ppCurrentTok().is(tok::percent_ident)) {
         if (lastWasVariable)
            CDOT_DIAGNOSE("two macro variables cannot directly follow one "
                             "another");

         lastWasVariable = true;
         pattern.emplace_back(move(ppCurrentTok()._value.strVal));
         variables.insert(pattern.back().getVarName());
      }
      else if (ppCurrentTok().is(tok::percent_percent_ident)) {
         CDOT_DIAGNOSE("%% cannot be used in macro pattern");
      }
      else if (ppCurrentTok().is(tok::space)) {
         // ignore
      }
      else {
         lastWasVariable = false;
         pattern.emplace_back(std::move(ppCurrentTok()));
      }

      pp_advance(false, true);
   }

   if (!pattern.empty() && pattern.back().isVariable())
      CDOT_DIAGNOSE("variables cannot come last in a macro pattern");

   TokenVec expansionTokens;

   pp_advance(true, true);
   if (!ppCurrentTok().is(tok::space)) {
      LispMacros.try_emplace(macroName.str(),
                             macroName.str().str(),
                             std::move(pattern),
                             std::move(expansionTokens));

      return;
   }

   size_t indent = ppCurrentTok()._value.getInt();
   pp_advance(true, true);

   while (1) {
      if (ppCurrentTok().is(tok::eof))
         break;

      if (ppCurrentTok().is(tok::newline)) {
         expansionTokens.emplace_back(std::move(ppCurrentTok()));
         pp_advance(false, true);

         if (ppCurrentTok().is(tok::space)) {
            if (ppCurrentTok()._value.getInt() < indent) {
               pp_backtrack();
               break;
            }

            ppCurrentTok()._value.intVal -= indent - baseIndent;
         }
         else if (!ppCurrentTok().is(tok::newline)) {
            pp_backtrack();
            break;
         }
         else {
            continue;
         }
      }
      else if (ppCurrentTok().is(tok::percent_ident)) {
         auto &ident = ppCurrentTok()._value.strVal;
         auto varIt = variables.find(ident);
         if (varIt == variables.end()) {
            CDOT_DIAGNOSE("variable " + ppCurrentTok().toString()
                          + " was not declared in macro pattern");
         }
      }
      else if (ppCurrentTok().is(tok::dollar_ident)) {
         auto &ident = ppCurrentTok()._value.strVal;
         if (auto M = tryGetLispMacro(ident)) {
            ident = M->getName().str();
         }
      }
      else if (ppCurrentTok().is(tok::percent_percent_ident)) {
         auto &ident = ppCurrentTok()._value.getString();
         auto varIt = variables.find(ident);
         if (varIt == variables.end()) {
            CDOT_DIAGNOSE("variable " + ppCurrentTok().toString()
                          + " was not declared in macro pattern");
         }
      }
      else if (ppCurrentTok().is(tok::eof)) {
         break;
      }

      expansionTokens.emplace_back(std::move(ppCurrentTok()));
      pp_advance(false, true);
   }

   assert(expansionTokens.back().is(tok::newline));
   while (!expansionTokens.empty() && expansionTokens.back().is(tok::newline))
      expansionTokens.pop_back();

   auto str = macroName.str();
   LispMacros.try_emplace(str, str.str(),
                          std::move(pattern),
                          std::move(expansionTokens));
}

template <class T>
typename Lexer<T>::TokenVec Lexer<T>::expand_lisp_macro(LispMacro *Ptr,
                                                        SourceLocation beginLoc) {
   if (macroExpansionDepth > 256)
      CDOT_DIAGNOSE("maximum recursion depth exceeded");

   auto offset = currentIndex;
   auto baseIndent = getCurrentIndent();

   assert(Ptr);
   auto &Macro = *Ptr;

   TokenVec tokens;
   bool first = true;

   auto SM = Macro.makeStateMachine();
   while (SM.hasNext()) {
      pp_advance(true, !first);
      if (!SM.moveNext(ppCurrentTok()))
         CDOT_DIAGNOSE("unexpected token in macro expansion "
                       + ppCurrentTok().toString());

      first = false;
   }

   auto aliasId = fs::FileManager::createOrGetFileAlias(sourceId);

   auto &VariableTokens = SM.getVariableTokens();
   for (const auto &tok : Macro.getExpansionTokens()) {
      if (tok.is(tok::percent_ident)) {
         auto valIt = VariableTokens.find(tok._value.getString());
         assert (valIt != VariableTokens.end());

         tokens.insert(tokens.end(), valIt->second.begin(),
                       valIt->second.end());
      }
      else if (tok.is(tok::percent_percent_ident)) {
         auto valIt = VariableTokens.find(tok._value.getString());
         assert(valIt != VariableTokens.end());

         string s;
         for (const auto &strTok : valIt->second)
            s += strTok.rawRepr();

         tokens.emplace_back(tok::stringliteral, Variant(move(s)),
                             tok.getSourceLoc());
      }
      else if (tok.is(tok::space)
               && (tokens.empty() || tokens.back().is(tok::newline))) {
         tokens.emplace_back(tok::space,
                             Variant(tok._value.getInt() + baseIndent),
                             tok.getSourceLoc());
      }
      else {
         tokens.push_back(tok);
      }

      tokens.back().loc = SourceLocation(tokens.back().getOffset(), aliasId);
   }

   ExpansionInfo info;
   info.expandedMacroLoc = beginLoc;
   info.macroName = Macro.getName();

   for (const auto &var : VariableTokens) {
      string s;
      for (const auto &tok : var.second) {
         s += tok.rawRepr();

         if (s.length() > 20) {
            s += "...";
            break;
         }
      }

      info.expansionArgs.emplace_back(var.getKey().str(), std::move(s));
   }

   MacroExpansionInfo[aliasId] = std::move(info);

   MacroExpansionLexer<T> lex(std::move(tokens), offset, this);
   lex.lex();

   assert(lex.tokens.back().is(tok::eof));
   lex.tokens.pop_back();

   return lex.tokens;
}

template class Lexer<LexerTraits>;
template class Lexer<module::ModuleLexerTraits>;

template class MacroExpansionLexer<LexerTraits>;
template class MacroExpansionLexer<module::ModuleLexerTraits>;

} // namespace lex
} // namespace util