//
// Created by Jonas Zell on 30.01.18.
//

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/StringSet.h>

#include "Preprocessor.h"
#include "Token.h"
#include "Lexer.h"

#include "Files/FileManager.h"
#include "Files/FileUtils.h"

#include "Variant/Variant.h"
#include "Basic/IdentifierInfo.h"
#include "Basic/Precedence.h"

#include "Message/Diagnostics.h"

using std::string;
using namespace cdot::lex;
using namespace cdot::diag;

namespace cdot {
namespace {

enum class BuiltinMacro {
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

} // anonymous namespace

class PreprocessorImpl: public DiagnosticIssuer {
public:
#ifdef NDEBUG
   using TokenVec   = llvm::SmallVector<Token, 256>;
#else
   using TokenVec   = std::vector<Token>;
#endif

   PreprocessorImpl(TokenVec &dst, IdentifierTable &Idents,
                    llvm::MutableArrayRef<Token> spelledTokens,
                    size_t sourceId)
      : dst(dst), Idents(Idents), tokens(spelledTokens),
        tokenIndex(0), sourceId(sourceId)
   {}

   void doPreprocessing()
   {
      processUntil(tok::eof);
      assert(currentTok().is(tok::eof));

      dst.push_back(currentTok());
   }

private:
   TokenVec &dst;
   IdentifierTable &Idents;

   llvm::MutableArrayRef<Token> tokens;
   size_t tokenIndex;

   size_t sourceId;

   struct LispMacro {
      struct Fragment {
         enum Kind {
            SequencePoint,
            Variable
         };

         explicit Fragment(Token tok)
            : kind(SequencePoint), tok(tok)
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

         bool moveNext(Token const& tok)
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

         std::string stringifyNextExpectedToken()
         {
            assert(hasNext());
            return lookahead().getTok().toString();
         }

         SourceLocation getNextExpectedTokenSourceLoc()
         {
            assert(hasNext());
            return lookahead().getTok().getSourceLoc();
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

            if (given.is(tok::ident)) {
               auto ident1 = given.getIdentifierInfo()->getIdentifier();
               auto ident2 = needed.getIdentifierInfo()->getIdentifier();

               return ident1 == ident2;
            }

            if (given.oneOf(tok::stringliteral, tok::integerliteral,
                            tok::fpliteral, tok::charliteral)) {
               return given.getText() == needed.getText();
            }

            return true;
         }

         void addVariable(Token const& tok)
         {
            if (tok.is(tok::open_paren))
               ++openParens;
            else if (tok.is(tok::open_brace))
               ++openBraces;
            else if (tok.is(tok::open_square))
               ++openSquare;

            auto &varName = current().getVarName();
            variableTokens[varName].emplace_back(tok);
         }
      };

      LispMacro(PatternVec &&pattern,
                TokenVec &&expansionTokens)
         : pattern(std::move(pattern)),
           expansionTokens(std::move(expansionTokens))
      { }

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
      PatternVec pattern;
      TokenVec expansionTokens;
   };

   llvm::SmallPtrSet<IdentifierInfo*, 16>   Macros;
   llvm::StringMap<LispMacro>               LispMacros;
   llvm::StringMap<Variant>                 Values;

   template<class ...Args>
   void emplace(Args&&... args)
   {
      dst.emplace_back(std::forward<Args&&>(args)...);
   }

   Token const& currentTok() const
   {
      return tokens[tokenIndex];
   }

   void advance(bool ignoreNewline = true, bool ignoreWhitespace = true)
   {
      assert(tokenIndex + 1 < tokens.size() && "advancing past eof");
      ++tokenIndex;

      if (ignoreNewline && currentTok().is(tok::newline))
         advance(ignoreNewline, ignoreWhitespace);

      if (ignoreWhitespace && currentTok().is(tok::space))
         advance(ignoreNewline, ignoreWhitespace);
   }
   
   bool expect(tok::TokenType kind,
               bool ignoreNewline = true,
               bool ignoreWhitespace = true) {
      advance(ignoreNewline, ignoreWhitespace);
      if (!currentTok().is(kind)) {
         err(err_generic_error)
            << "unexpected token " + currentTok().toString()
            << currentTok().getSourceLoc()
            << diag::term;
         
         return false;
      }
      
      return true;
   }

   Token const& lookahead(bool ignoreNewline = true,
                          bool ignoreWhitespace = true,
                          unsigned offset = 1) {
      assert(tokenIndex + offset < tokens.size() && "looking ahead past eof");

      auto &tok = tokens[tokenIndex + offset];
      if (ignoreNewline && tok.is(tok::newline))
         return lookahead(ignoreNewline, ignoreWhitespace, offset + 1);

      if (ignoreWhitespace && tok.is(tok::space))
         return lookahead(ignoreNewline, ignoreWhitespace, offset + 1);

      return tok;
   }

   Token const& lookbehind(bool ignoreNewline = true,
                           bool ignoreWhitespace = true,
                           unsigned offset = 1) {
      assert(tokenIndex - offset < tokens.size() && "looking behind past eof");

      auto &tok = tokens[tokenIndex - offset];
      if (ignoreNewline && tok.is(tok::newline))
         return lookbehind(ignoreNewline, ignoreWhitespace, offset + 1);

      if (ignoreWhitespace && tok.is(tok::space))
         return lookbehind(ignoreNewline, ignoreWhitespace, offset + 1);

      return tok;
   }

   void backtrack()
   {
      assert(tokenIndex && "backtracking past beginning of file");
      --tokenIndex;
   }

   void copyAndAdvance(bool ignoreNewline = true, bool ignoreWhitespace = true)
   {
      auto tok = std::move(tokens[tokenIndex]);
      advance(ignoreNewline, ignoreWhitespace);
      emplace(std::move(tok));
   }

   template<class ...Args>
   void processUntil(Args ...kinds)
   {
      while (!currentTok().oneOf(tok::eof, kinds...)) {
         if (currentTok().is_directive()) {
            handle_directive();
         }
         else if (currentTok().is(tok::expr_begin)) {
            auto loc = currentTok().getSourceLoc();
            advance();

            auto val = parseExpression();
            dst.push_back(VariantToToken(std::move(val), loc));

            advance();
            assert(currentTok().is(tok::close_brace));
         }
         else if (currentTok().is(tok::stringify_begin)) {
            auto loc = currentTok().getSourceLoc();
            advance();

            auto val = parseExpression();
            dst.push_back(VariantToStringLiteral(std::move(val), loc));

            advance();
            assert(currentTok().is(tok::close_brace));
         }
         else if (currentTok().is(tok::dollar_ident)) {
            auto macroName = currentTok().getIdentifier().drop_front(1);
            auto it = LispMacros.find(macroName);

            if (it != LispMacros.end()) {
               expand_lisp_macro(it->getValue(),
                                 macroName,
                                 currentTok().getSourceLoc());
            }
            else {
               err(err_generic_error)
                  << "macro " + currentTok().getIdentifierInfo()
                                            ->getIdentifier().str()
                     + " was not defined"
                  << currentTok().getSourceLoc()
                  << diag::cont;
            }
         }
         else {
            dst.emplace_back(tokens[tokenIndex]);
         }

         advance(false, false);
      }
   }

   Variant parseExpression(Variant lhs = {}, int minPrecedence = 0)
   {
      if (!lhs)
         lhs = parseUnaryExpr();

      auto prec = getOperatorPrecedence(lookahead().getKind());
      while (prec != prec::Unknown && prec >= minPrecedence) {
         advance();

         auto op = currentTok().toString();
         advance();

         auto rhs = parseUnaryExpr();
         auto savedPrec = prec;
         auto nextPrec = getOperatorPrecedence(lookahead().getKind());

         while (nextPrec != prec::Unknown && nextPrec > savedPrec) {
            rhs = parseExpression(rhs, nextPrec);

            savedPrec = nextPrec;
            nextPrec = getOperatorPrecedence(lookahead().getKind());
         }

         lhs = lhs.applyBinaryOp(rhs, op);
         prec = getOperatorPrecedence(lookahead().getKind());
      }

      return lhs;
   }

   Variant parseUnaryExpr()
   {
      string unaryOp;
      switch (currentTok().getKind()) {
         case tok::exclaim:
         case tok::tilde:
         case tok::plus_plus:
         case tok::minus_minus:
            unaryOp = currentTok().toString();
            advance();
            break;
         default:
            break;
      }

      auto val = currentTokenValue();
      if (!unaryOp.empty())
         val = val.applyUnaryOp(unaryOp);

      unaryOp.clear();

      switch (lookahead().getKind()) {
         case tok::plus_plus:
         case tok::minus_minus:
            unaryOp = lookahead().toString();
            advance();
            break;
         default:
            break;
      }

      if (!unaryOp.empty())
         val = val.applyUnaryOp(unaryOp);

      return val;
   }

   Variant currentTokenValue()
   {
      switch (currentTok().getKind()) {
         case tok::charliteral:
            return Variant(currentTok().getText().front());
         case tok::stringliteral:
            return Variant(currentTok().getText());
         case tok::integerliteral:
            return currentTok().getIntegerValue();
         case tok::fpliteral: {
            llvm::APFloat APFloat(0.0);
            APFloat.convertFromString(currentTok().getText(),
                                      llvm::APFloat::rmNearestTiesToEven);

            return Variant(std::move(APFloat));
         }
         case tok::ident: {
            auto ident = currentTok().getIdentifierInfo()->getIdentifier();

            auto it = Values.find(ident);
            if (it != Values.end())
               return it->getValue();

            auto funcIt = BuiltinMacros.find(ident);
            if (funcIt != BuiltinMacros.end()
                                          && lookahead().is(tok::open_paren)) {
               advance();
               return handleBuiltinFn(funcIt->getValue().first);
            }

            err(err_generic_error)
               << "reference to undeclared identifier " + ident
               << currentTok().getSourceLoc()
               << diag::term;

            return {};
         }
         case tok::open_square:
            return parseArray();
         default:
            err(err_generic_error)
               << "unexpected token in preprocessor expression: "
                  + currentTok().toString()
               << currentTok().getSourceLoc()
               << diag::term;

            return {};
      }
   }

   Variant parseArray()
   {
      assert(currentTok().is(tok::open_square));
      std::vector<Variant> vec;

      advance();
      while (!currentTok().is(tok::close_square)) {
         vec.push_back(parseExpression());

         advance();
         if (currentTok().is(tok::comma))
            advance();
      }

      return Variant(VariantType::Array, move(vec));
   }

   Token VariantToToken(Variant &&V, SourceLocation loc)
   {
      auto *Mem = Idents.getAllocator().Allocate<Variant>();
      return Token(new (Mem) Variant(std::move(V)), loc);
   }

   Token VariantToStringLiteral(Variant &&V, SourceLocation loc)
   {
      auto *Mem = Idents.getAllocator().Allocate<Variant>();
      if (V.isStr()) {
         return Token(new (Mem) Variant(std::move(V)), loc);
      }

      return Token(new (Mem) Variant(V.toString()), loc);
   }

   void handle_directive()
   {
      switch (currentTok().getKind()) {
         case tok::pound_define: return handle_define();
         case tok::pound_macro: return parse_lisp_macro();
         case tok::pound_undef: return handle_undef();
         case tok::pound_if: return handle_plain_if();
         case tok::pound_ifdef: return handle_ifdef();
         case tok::pound_ifndef: return handle_ifndef();
         case tok::pound_endif:
         case tok::pound_else:
         case tok::pound_elseif:
            err(err_generic_error)
               << currentTok().toString() + " without preceding #if"
               << currentTok().getSourceLoc();

            break;
         case tok::pound_pragma: return handle_pragma();
         case tok::pound_for: return handle_for();
         case tok::pound_endfor:
            err(err_generic_error)
               << "#endfor without preceding #for"
               << currentTok().getSourceLoc();

            break;
         case tok::pound_print: return handle_print();
         case tok::pound_let: return handle_let();
         case tok::pound_using: return handle_using();
         case tok::pound_namespace: return handle_namespace();
         case tok::pound_endnamespace: return handle_endnamespace();
         case tok::pound_include: return handle_include();
         default:
            llvm_unreachable("bad directive");
      }
   }

   void handle_if_common(bool condition);
   void handle_plain_if();
   void handle_ifdef();
   void handle_ifndef();

   template<class ...Args>
   void discard_until(Args ...kinds)
   {
      while (!currentTok().oneOf(tok::eof, kinds...))
         advance();
   }

   void skipToEndOfDirective()
   {
      discard_until(tok::newline);
   }

   void handle_define();
   void handle_undef();

   void handle_pragma() {}

   void handle_print();
   void handle_let();

   void handle_for();

   void handle_using()        {}
   void handle_namespace()    {}
   void handle_endnamespace() {}

   void handle_include();

   void parse_lisp_macro();
   void expand_lisp_macro(LispMacro &Macro,
                          llvm::StringRef macroName,
                          SourceLocation beginLoc);

   Variant handleBuiltinFn(BuiltinMacro Fn);
};

void PreprocessorImpl::handle_let()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   auto ident = currentTok().getIdentifierInfo()->getIdentifier();
   if (!expect(tok::equals))
      return skipToEndOfDirective();

   advance();
   auto V = parseExpression();

   Values[ident] = V;
}

void PreprocessorImpl::handle_print()
{
   advance();
   auto V = parseExpression();

   llvm::outs() << V.toString() << "\n";
}

void PreprocessorImpl::handle_define()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   Macros.insert(currentTok().getIdentifierInfo());
}

void PreprocessorImpl::handle_undef()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   Macros.erase(currentTok().getIdentifierInfo());
}

void PreprocessorImpl::handle_plain_if()
{
   advance();
   auto V = parseExpression();

   bool cond = false;
   if (!V.isInt()) {
      err(err_generic_error)
         << "condition must be integral"
         << currentTok().getSourceLoc();
   }
   else {
      cond = V.getAPSInt().getBoolValue();
   }

   handle_if_common(cond);
}

void PreprocessorImpl::handle_ifdef()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   handle_if_common(Macros.count(currentTok().getIdentifierInfo()) != 0);
}

void PreprocessorImpl::handle_ifndef()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   handle_if_common(Macros.count(currentTok().getIdentifierInfo()) == 0);
}

void PreprocessorImpl::handle_if_common(bool condition)
{
   if (condition) {
      processUntil(tok::pound_endif, tok::pound_else, tok::pound_elseif);
      switch (currentTok().getKind()) {
         case tok::eof:
            err(err_generic_error)
               << "unexpected end of file, expecting #endif"
               << currentTok().getSourceLoc()
               << diag::cont;

            return;
         case tok::pound_else:
         case tok::pound_elseif:
            return discard_until(tok::pound_endif);
         default:
            llvm_unreachable("bad token kind");
      }
   }
   else {
      discard_until(tok::pound_endif, tok::pound_else, tok::pound_elseif);
      switch (currentTok().getKind()) {
         case tok::eof:
            err(err_generic_error)
               << "unexpected end of file, expecting #endif"
               << currentTok().getSourceLoc()
               << diag::cont;

            return;
         case tok::pound_else:
            return;
         case tok::pound_elseif:
            return handle_plain_if();
         default:
            llvm_unreachable("bad token kind");
      }
   }
}

void PreprocessorImpl::handle_for()
{
   if (!expect(tok::ident))
      return skipToEndOfDirective();

   auto ident = currentTok().getIdentifierInfo()->getIdentifier();
   if (!expect(tok::equals))
      return skipToEndOfDirective();

   advance();

   auto arr = parseExpression();
   if (!arr.isArray()) {
      err(err_generic_error)
         << "value is not an array"
         << currentTok().getSourceLoc()
         << diag::term;

      return;
   }

   advance();

   Variant prevValue;
   auto it = Values.find(ident);
   if (it != Values.end())
      prevValue = std::move(it->getValue());

   auto idx = tokenIndex;
   for (auto &V : arr) {
      Values[ident] = V;

      processUntil(tok::pound_endfor);
      if (currentTok().is(tok::eof))
         err(err_generic_error)
            << "unexpected end of file, expecting #endfor"
            << currentTok().getSourceLoc();

      tokenIndex = idx;
   }

   if (prevValue)
      Values[ident] = prevValue;
}

void PreprocessorImpl::handle_include()
{
   if (!expect(tok::stringliteral))
      return skipToEndOfDirective();

   llvm::SmallVector<std::string, 4> includeDirs;
   includeDirs.push_back(fs::getPath(
      fs::FileManager::getFileName(sourceId).str()));
   includeDirs.push_back("");

   auto fileName = currentTok().getText();

   auto file = fs::findFileInDirectories(fileName, includeDirs);
   if (file.empty()) {
      err(err_generic_error)
         << "file " + fileName + " not found"
         << currentTok().getSourceLoc() << diag::cont;

      return;
   }

   auto BufAndId = fs::FileManager::openFile(file, true);

   Lexer lexer(Idents, BufAndId.second.get(), BufAndId.first);
   lexer.lex();

   dst.insert(dst.end(),
              std::make_move_iterator(lexer.getTokens().begin()),
              std::make_move_iterator(lexer.getTokens().end() - 1));
}

void PreprocessorImpl::parse_lisp_macro()
{
   assert(currentTok().is(tok::pound_macro));

   unsigned baseIndent = 0;
   auto &prev = lookbehind(false, false);
   if (prev.is(tok::space)) {
      baseIndent = prev.getNumSpaces();
   }

   if (!expect(tok::ident))
      return skipToEndOfDirective();

   auto macroName = currentTok().getIdentifierInfo()->getIdentifier();

   LispMacro::PatternVec pattern;
   llvm::StringSet<> variables;
   bool lastWasVariable = false;

   advance(false, false);
   while (!currentTok().is(tok::newline)) {
      if (currentTok().is(tok::percent_ident)) {
         if (lastWasVariable)
            err(err_generic_error)
               << "two macro variables may not directly follow each other"
               << currentTok().getSourceLoc() << diag::cont;

         lastWasVariable = true;
         pattern.emplace_back(currentTok().getIdentifierInfo()
                                          ->getIdentifier());

         variables.insert(pattern.back().getVarName());
      }
      else if (currentTok().is(tok::percent_percent_ident)) {
         err(err_generic_error)
            << "%% may not appear in macro pattern"
            << currentTok().getSourceLoc() << diag::cont;
      }
      else if (currentTok().is(tok::space)) {
         // ignore
      }
      else {
         lastWasVariable = false;
         pattern.emplace_back(currentTok());
      }

      advance(false, false);
   }

   if (!pattern.empty() && pattern.back().isVariable())
      err(err_generic_error)
         << "variables may not appear last in a macro pattern"
         << currentTok().getSourceLoc() << diag::cont;

   TokenVec expansionTokens;

   advance(true, false);
   if (!currentTok().is(tok::space)) {
      LispMacros.try_emplace(macroName,
                             std::move(pattern),
                             std::move(expansionTokens));

      return;
   }

   unsigned indent = currentTok().getNumSpaces();
   advance(false, false);

   while (1) {
      if (currentTok().is(tok::eof))
         break;

      if (currentTok().is(tok::newline)) {
         expansionTokens.emplace_back(currentTok());
         advance(false, false);

         if (lookahead().is(tok::space)) {
            if (lookahead().getNumSpaces() < indent) {
               break;
            }

            advance();

            dst.emplace_back(currentTok().getText().data(),
                             currentTok().getNumSpaces() - indent - baseIndent,
                             tok::space, currentTok().getSourceLoc());

            advance(false, false);
            continue;
         }
         else if (!currentTok().is(tok::newline)) {
            backtrack();
            break;
         }
         else {
            continue;
         }
      }
      else if (currentTok().oneOf(tok::percent_ident,
                                  tok::percent_percent_ident)) {
         auto ident = currentTok().getIdentifierInfo()->getIdentifier();
         auto varIt = variables.find(ident);
         if (varIt == variables.end()) {
            err(err_generic_error)
               << "variable " + ident + " was not declared in macro pattern"
               << currentTok().getSourceLoc() << diag::cont;
         }
      }

      expansionTokens.emplace_back(currentTok());
      advance(false, false);
   }

   assert(expansionTokens.back().is(tok::newline));
   while (!expansionTokens.empty() && expansionTokens.back().is(tok::newline))
      expansionTokens.pop_back();

   LispMacros.try_emplace(macroName,
                          std::move(pattern),
                          std::move(expansionTokens));
}

void PreprocessorImpl::expand_lisp_macro(LispMacro &Macro,
                                         llvm::StringRef macroName,
                                         SourceLocation beginLoc) {
   unsigned baseIndent = 0;
   auto &prev = lookbehind(false, false);
   if (prev.is(tok::space)) {
      baseIndent = prev.getNumSpaces();
   }

   TokenVec tokens;
   bool first = true;

   auto SM = Macro.makeStateMachine();
   while (SM.hasNext()) {
      advance(true, first);

      if (currentTok().is(tok::eof)) {
         err(err_generic_error)
            << "unexpected end of file in macro expansion: expecting "
               + SM.stringifyNextExpectedToken()
            << SM.getNextExpectedTokenSourceLoc()
            << diag::cont;

         note(note_generic_note)
            << "expanding macro " + macroName + " here"
            << beginLoc << diag::term;
      }

      if (!SM.moveNext(currentTok())) {
         diag::err(err_generic_error)
            << "unexpected token in macro expansion"
            << currentTok().getSourceLoc()
            << diag::cont;

         return;
      }

      first = false;
   }

   auto &VariableTokens = SM.getVariableTokens();
   for (const auto &tok : Macro.getExpansionTokens()) {
      if (tok.is(tok::percent_ident)) {
         auto ident = tok.getIdentifierInfo()->getIdentifier();
         auto valIt = VariableTokens.find(ident);
         assert (valIt != VariableTokens.end());

         tokens.insert(tokens.end(),
                       valIt->second.begin(),
                       valIt->second.end());
      }
      else if (tok.is(tok::percent_percent_ident)) {
         auto ident = tok.getIdentifierInfo()->getIdentifier();
         auto valIt = VariableTokens.find(ident);
         assert(valIt != VariableTokens.end());

         string s;
         for (const auto &strTok : valIt->second)
            s += strTok.rawRepr();

         tokens.emplace_back(VariantToStringLiteral(Variant(move(s)),
                                                    tok.getSourceLoc()));
      }
      else if (tok.is(tok::space)
               && (tokens.empty() || tokens.back().is(tok::newline))) {
         tokens.emplace_back(Token::Space,
                             tok.getNumSpaces() + baseIndent,
                             tok.getSourceLoc());
      }
      else {
         tokens.push_back(tok);
      }
   }

   // preprocessor expects last token to be EOF
   tokens.emplace_back(tok::eof);

   PreprocessorImpl PP(dst, Idents, tokens, sourceId);
   PP.doPreprocessing();

   assert(dst.back().is(tok::eof));
   dst.pop_back();
}

#define EXPECT_NUM_ARGUMENTS(ArgNo)                                          \
   if (args.size() != ArgNo) {                                               \
      err(err_generic_error) << "expected " #ArgNo " arguments, but found "  \
         + std::to_string(args.size()) << loc << diag::cont;                 \
      return {};                                                             \
   }

#define EXPECT_ARG_TYPE(ArgNo, ArgTy)                                        \
   if (args[ArgNo].getKind() != VariantType::ArgTy) {                        \
      err(err_generic_error) << "expected " #ArgTy " for argument "  #ArgNo  \
         << loc << diag::cont;                                               \
      return {};                                                             \
   }

Variant PreprocessorImpl::handleBuiltinFn(BuiltinMacro Fn)
{
   auto loc = currentTok().getSourceLoc();
   assert(currentTok().is(tok::open_paren));

   llvm::SmallVector<Variant, 2> args;
   advance();
   while (!currentTok().is(tok::close_paren)) {
      args.push_back(parseExpression());

      advance();
      if (currentTok().is(tok::comma))
         advance();
   }

   switch (Fn) {
      case BuiltinMacro::TOLOWER:
      case BuiltinMacro::TOUPPER: {
         EXPECT_NUM_ARGUMENTS(1)
         EXPECT_ARG_TYPE(0, String)

         string str(args.front().getString());

         if (Fn == BuiltinMacro::TOLOWER)
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);
         else
            std::transform(str.begin(), str.end(), str.begin(), ::toupper);

         return move(str);
      }
      case BuiltinMacro::TOSTRING:
         EXPECT_NUM_ARGUMENTS(1)
         return args.front().toString();
      case BuiltinMacro::NUMFORMAT: {
         EXPECT_NUM_ARGUMENTS(2)
         EXPECT_ARG_TYPE(0, Int)
         EXPECT_ARG_TYPE(1, Int)

         llvm::SmallString<128> str;
         args[0].getAPSInt().toString(str, args[1].getSExtValue());

         return str.str().str();
      }
      case BuiltinMacro::ParseInt: {
         EXPECT_NUM_ARGUMENTS(1)
         EXPECT_ARG_TYPE(0, String)

         unsigned base = 10;
         if (args.size() == 2) {
            EXPECT_ARG_TYPE(1, Int)
            base = unsigned(args[1].getZExtValue());
         }

         return std::stoull(args[0].getString(), nullptr, base);
      }
      case BuiltinMacro::ParseFloat: {
         EXPECT_NUM_ARGUMENTS(1)
         EXPECT_ARG_TYPE(0, String)

         unsigned base = 10;
         if (args.size() == 2) {
            EXPECT_ARG_TYPE(1, Int)
            base = unsigned(args[1].getZExtValue());
         }

         return std::stod(args[0].getString(), nullptr);
      }
      case BuiltinMacro::REPEAT: {
         EXPECT_NUM_ARGUMENTS(2)
         EXPECT_ARG_TYPE(0, String)
         EXPECT_ARG_TYPE(1, Int)

         llvm::SmallString<128> str;
         for (size_t i = 0; i < args[1].getAPSInt(); ++i)
            str += args[0].getString();

         return str.str().str();
      }
      case BuiltinMacro::DEFINED: {
         EXPECT_NUM_ARGUMENTS(1)
         EXPECT_ARG_TYPE(0, String)

         auto it = Macros.find(&Idents.get(args[0].getString()));
         return it != Macros.end();
      }
      case BuiltinMacro::FILE: {
         return fs::FileManager::getFileName(currentTok().getSourceLoc()
                                                         .getSourceId()).str();
      }
      case BuiltinMacro::LINE: {
         auto l = fs::FileManager::getLineAndCol(currentTok().getSourceLoc());
         return uint64_t(l.first);
      }
   }

   llvm_unreachable("bad builtin fn");
}

#undef EXPECT_NUM_ARGUMENTS
#undef EXPECT_ARG_TYPE

Preprocessor::Preprocessor(cdot::Preprocessor::TokenVec &dst,
                           cdot::IdentifierTable &Idents,
                           llvm::MutableArrayRef<Token> spelledTokens,
                           size_t sourceId)
   : pImpl(new PreprocessorImpl(dst, Idents, spelledTokens, sourceId))
{

}

Preprocessor::~Preprocessor()
{
   delete pImpl;
}

void Preprocessor::doPreprocessing()
{
   pImpl->doPreprocessing();
}

namespace {

//region Description
//llvm::StringMap<Variant> BuiltinValues = {
//   { "_WordSize", Variant((unsigned long long) (sizeof(size_t) * 8)) },
//   { "__DEBUG__", Variant(true) },
//   { "_CLOCKS_PER_SEC", Variant((unsigned long long) CLOCKS_PER_SEC) },
//   { "EOF", Variant((unsigned long long) EOF) },
//   { "SEEK_SET", Variant((unsigned long long) SEEK_SET) },
//   { "SEEK_END", Variant((unsigned long long) SEEK_END) },
//   { "SEEK_CUR", Variant((unsigned long long) SEEK_CUR) },
//#ifdef _WIN32
//   { "_WIN32", Variant(true) },
//#else
//   { "_WIN32", Variant(false) },
//#endif
//#ifdef _WIN64
//   { "_WIN64", Variant(true) },
//#else
//   { "_WIN64", Variant(false) },
//#endif
//#if defined(unix) || defined(__unix) || defined(__unix__)
//{ "unix", Variant(true) },
//      { "__unix", Variant(true) },
//      { "__unix__", Variant(true) },
//#else
//   { "unix", Variant(false) },
//   { "__unix", Variant(false) },
//   { "__unix__", Variant(false) },
//#endif
//#if defined(__APPLE__) || defined(__MACH__)
//   { "__APPLE__", Variant(true) },
//   { "__MACH__", Variant(true) },
//#else
//{ "__APPLE__", Variant(false) },
//      { "__MACH__", Variant(false) },
//#endif
//#if defined(linux) || defined(__linux) || defined(__linux__)
//{ "linux", Variant(true) },
//      { "__linux", Variant(true) },
//      { "__linux__", Variant(true) },
//#else
//   { "linux", Variant(false) },
//   { "__linux", Variant(false) },
//   { "__linux__", Variant(false) },
//#endif
//#ifdef __FreeBSD__
//   { "__FreeBSD__", Variant(true) },
//#else
//   { "__FreeBSD__", Variant(false) },
//#endif
//   { "__builtin_eh_data_regno_0",
//      Variant((unsigned long long)__builtin_eh_return_data_regno(0)) },
//   { "__builtin_eh_data_regno_1",
//      Variant((unsigned long long)__builtin_eh_return_data_regno(1)) }
//};

} // anonymous namespace

} // namespace cdot