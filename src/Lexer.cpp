//
// Created by Jonas Zell on 18.06.17.
//

#include "Lexer.h"

#include <regex>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

#ifndef CDOT_LEXER_STANDALONE
#include "Message/Diagnostics.h"
#endif

Lexer::Lexer(llvm::MemoryBuffer *buf,
             string const& fileName,
             unsigned sourceId) : curr(buf->getBufferStart()),
                                  begin(buf->getBufferStart()),
                                  srcLen(buf->getBufferSize()),
                                  sourceId(sourceId), fileName(fileName) {
   currentToken = Token(T_BOF, {}, SourceLocation(), 0);
}

Lexer::Lexer(std::vector<Token> &&tokens)
   : lookaheadList(tokens), srcLen(0)
{

}

Lexer::Lexer()
   : curr(nullptr)
{

}

// RAII utility classes

Lexer::ModeScopeGuard::ModeScopeGuard(Mode m, Lexer *lex)
   : lex(lex), prevMode(lex->mode)
{
   lex->setMode(m);
}

Lexer::ModeScopeGuard::~ModeScopeGuard()
{
   lex->setMode(prevMode);
}

Lexer::StateSaveGuard::StateSaveGuard(Lexer *lex)
   : lex(lex), shrinkTo(lex->tokens.size())
{

}

Lexer::StateSaveGuard::~StateSaveGuard()
{
   assert(lex->tokens.size() >= shrinkTo);
   while (lex->tokens.size() > shrinkTo) {
      lex->lookaheadList.push_back(std::move(lex->tokens.back()));
      lex->tokens.pop_back();
   }

   lex->currentToken = lex->tokens.back();
}

std::vector<Token> Lexer::lexString(const string &s)
{
   auto buf = llvm::MemoryBuffer::getMemBuffer(llvm::StringRef(s.data(),
                                                               s.length()));

   Lexer lex(buf.get(), fileName, sourceId);
   lex.advance();

   std::vector<Token> res;
   while (lex.currentToken.get_type() != T_EOF) {
      res.push_back(lex.currentToken);
      lex.advance();
   }

   return std::move(res);
}

std::vector<Token> Lexer::templateArgToTokens(const TemplateArg &templateArg)
{
   llvm_unreachable("unsupported template arg kind");
}

std::vector<Token> Lexer::valueToTokens(const TemplateArg &templateArg)
{
//   auto &val = templateArg.getValue();
//   return {Token(T_LITERAL, val};
   llvm_unreachable("henlo");
}

void Lexer::addTemplateArg(const string &name, const string &value)
{
   Values[name] = Variant(string(value));
   TemplateArgs[name] = lexString(value);
}

void Lexer::addTemplateArg(
   const string &name, const std::vector<Token> &tokens)
{
   TemplateArgs.emplace(name, tokens);
   Values[name] = Variant(Token::TokensToString(tokens));
}

void Lexer::addTemplateArg(const string &name, const Variant &val)
{
   string str = val.toString();
   if (val.type == VariantType::STRING) {
      str = "\"" + str;
      str += "\"";
   }

   addTemplateArg(name, str);
   Values[name] = val;
}

void Lexer::addVariadicTemplateArg(const string &name,
                                   std::vector<Variant> &vec) {
   Values[name] = Variant(std::move(vec));
}

void Lexer::reset(const char *src, size_t len)
{
   this->curr = src;
   currentToken = Token(T_BOF, {}, SourceLocation(), 0);
   continueInterpolation = false;
   indexOnLine = 0;
   currentLine = 1;
   currentIndex = 0;
   srcLen = len;
   tokens.clear();
}

string Lexer::strVal()
{
   if (currentToken.get_value().strVal.empty()
       && currentToken.get_type() != T_LITERAL) {
      CDOT_DIAGNOSE("Unexpected token " + currentToken.toString());
   }

   return currentToken._value.strVal;
}

string& Lexer::strRef()
{
   return currentToken._value.strVal;
}

/**
 * Decides wether or not a character could be part of a number token.
 * @param _c
 * @param allowDecimalPoint Whether to allow a decimal point or not
 * @return
 */
bool Lexer::is_number(char c, bool allowDecimalPoint) {
   // digits and dot
   return (c >= '0' && c <= '9') || (allowDecimalPoint && c == '.');
}

bool Lexer::is_hex(char c) {
   return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f')
          || (c >= '0' && c <= '9');
}

/**
 * Decides whether or not a character could be part of a keyword or identifier
 * token.
 * @param _c
 * @return
 */
bool Lexer::is_identifier_char(char c) {
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

/**
 * Decides whether a value can be an identifier or not.
 * @param _c
 * @return
 */
bool Lexer::is_operator(const string &s) {
   if ((mode == TemplateArgMode || mode == TemplateConstraints)
       && (matches("^[<>].+", s))) {
      return false;
   }
   if (std::regex_match(s, validOp)) {
      return true;
   }
   if (in_vector(util::binary_operators, s)) {
      return true;
   }
   if (in_vector(util::assignmentOperators, s)) {
      return true;
   }
   if (in_vector(util::PrefixUnaryOperators, s)) {
      return true;
   }
   if (in_vector(util::PostfixUnaryOperators, s)) {
      return true;
   }

   return false;
}

bool Lexer::is_operator_char(char c) {
   if (in_vector(util::operator_chars, c)) {
      return true;
   }

   return false;
}

/**
 * Returns true if the given string is a language keyword.
 * @param s
 * @return
 */
bool Lexer::is_keyword(const string &s)
{
   if (in_vector(util::keywords, s)) {
      return true;
   }

   if (mode == TemplateConstraints) {
      return s == "typename" || s == "value" || s == "any";
   }

   return false;
}

bool Lexer::is_punctuator(char c)
{
   return in_vector(util::punctuators, c);
}

bool Lexer::is_bool_literal(const string &s)
{
   return s == "true" || s == "false";
}


/**
 * Gets the next character in the program
 * @return
 */
char Lexer::get_next_char()
{
   if (currentIndex >= srcLen) {
      ++currentIndex;
      return '\0';
   }

   ++indexOnLine;
   ++currentIndex;

   return *(curr++);
}

char Lexer::char_lookahead()
{
   if (currentIndex >= srcLen) {
      currentIndex++;
      return '\0';
   }

   return *curr;
}

/**
 * Backtracks to the last token
 */
void Lexer::backtrack(bool discard)
{
   if (!discard) {
      lookaheadList.push_back(std::move(tokens.back()));
   }
   else {
      currentIndex = tokens.back().start;
      currentLine = tokens.back().getLine();
      indexOnLine = tokens.back().getCol();
      curr = begin + currentIndex;
   }

   tokens.pop_back();
   currentToken = tokens.back();
}

/**
 * Backtracks by the passed amount of characters
 * @param length
 */
void Lexer::backtrack_c(int length)
{
   currentIndex -= length;
   indexOnLine -= length;
   curr -= length;
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
         return string(1,c);
   }
}

/**
 * Parses the next token
 * @return
 */
Token Lexer::lex_next_token(bool ignore_newline, bool significantWhiteSpace)
{
   if (currentToken.get_type() == T_EOF) {
      CDOT_DIAGNOSE("Unexpected end of file");
   }

   if (currentIndex >= srcLen) {
      return makeToken(T_EOF, { }, currentIndex, 0);
   }

   string t;
   size_t _start_index = currentIndex;
   size_t indent = 0;
   size_t col;
   char first;
   char next;

   if (continueInterpolation) {
      goto string_literal;
   }

   first = get_next_char();

   if (first == ' ' && significantWhiteSpace) {
      return makeToken(T_PUNCTUATOR, { first }, currentIndex, indexOnLine);
   }


   col = indexOnLine;
   while (first == ' ') {
      first = get_next_char();
      ++_start_index;
      ++col;
   }

   if (first == '\0' && currentIndex >= srcLen) {
      return makeToken(T_EOF, { }, currentIndex, col);
   }

   // comment
   if (first == '/') {
      next = get_next_char();
      if (next == '*') {
         ignore_ml_comment();

         return lex_next_token(ignore_newline, significantWhiteSpace);
      }
      else if (next == '/') {
         ignore_comment();
         return lex_next_token(ignore_newline, significantWhiteSpace);
      }
      else {
         backtrack_c(1);
      }
   }

   // preprocessor variable
   if (first == '#' && char_lookahead() == '{') {
      if (ignoreValue) {
         advance();
         skip_until_even(BRACE);

         return makeToken(T_IDENT, {string("__pl")}, _start_index, col);
      }

      get_next_char();
      advance();

      SourceLocation loc(indexOnLine, currentLine, 0, sourceId);
      auto tok = valueToToken(loc, parseExpression());
      advance();

      if (!currentToken.is_punctuator('}')) {
         CDOT_DIAGNOSE("expected '}'");
      }

      return tok;
   }

   // preprocessor directive
   if (first == '#') {
      next = get_next_char();
      while (next != ' ' && next != '\n' && next != '\0') {
         t += next;
         next = get_next_char();
      }

      backtrack_c(1);

      if (!ignoreDirective) {
         handle_directive(std::move(t));
         advance(ignore_newline, significantWhiteSpace);

         return currentToken;
      }

      return makeToken(T_DIRECTIVE, { std::move(t) }, _start_index, col);
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

      return makeToken(T_LITERAL, { next }, _start_index, col);
   }

   // string literal
   if (first == '\"') {
      string_literal:
      while (currentIndex < srcLen) {
         next = get_next_char();
         switch (next) {
            case '\"':
               goto done;
            case '\\': {
               auto esc = escape_char(get_next_char());
               if (esc != next) {
                  t += esc;
               } else {
                  t += '\\';
                  t += esc;
               }
               break;
            }
            case '$': {
               if (!ignore_interpolation) {
                  auto tok = makeToken(T_LITERAL, {std::move(t)}, _start_index,
                                       col);
                  tok.isInterpolationStart = true;

                  return tok;
               }
               [[clang::fallthrough]];
            }
            default:
               t += next;
         }
      }

      done:
      continueInterpolation = false;
      return makeToken(T_LITERAL, { std::move(t) }, _start_index, col);
   }

   // escape sequence
   if (first == '`') {
      next = get_next_char();
      while (next != '`' && currentIndex < srcLen) {
         t += next;
         next = get_next_char();

      }

      return makeToken(T_IDENT, { std::move(t) }, _start_index, col, true);
   }

   if (first == '-') {
      next = char_lookahead();
      if (next >= 48 && next <= 57) {
         t += "-";
         first = next;
         get_next_char();
      }
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
         return makeToken(T_LITERAL, { std::stoull(hex_s, nullptr, 16) },
                          _start_index, col);
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
         return makeToken(T_LITERAL, { std::stoull(bin_s, nullptr, 2) },
                          _start_index, col);
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
               return makeToken(T_LITERAL, { std::stoull(t) },
                                _start_index, col);
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
         return makeToken(T_LITERAL, { std::stoull(t, nullptr, 8) },
                          _start_index, col);
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

         return makeToken(T_LITERAL, { base * exp }, _start_index, col);
      }

      backtrack_c(1);

      if (!decimal_point) {
         next = char_lookahead();

         bool isUnsigned = false;
         bool isIntegral = false;
         Variant val;

         if (next == 'u' || next == 'U') {
            get_next_char();
            isUnsigned = true;
            isIntegral = true;
            next = char_lookahead();
         }

         if (next == 'l' || next == 'L') {
            isIntegral = true;
            get_next_char();
            val = Variant(std::stoull(t));
            next = char_lookahead();
         }

         if ((next == 'u' || next == 'U') && !isUnsigned) {
            get_next_char();
            isUnsigned = true;
            isIntegral = true;
            next = char_lookahead();
         }

         if ((next == 'd' || next == 'D') && !isIntegral) {
            get_next_char();
            val = Variant(std::stod(t));
         }
         else {
            val = Variant(std::stoull(t));
         }

         val.is_unsigned = isUnsigned;

         return makeToken(T_LITERAL, std::move(val), _start_index, col);
      }
      else {
         next = char_lookahead();
         if (next == 'f' || next == 'F') {
            get_next_char();
            return makeToken(T_LITERAL, { std::stof(t) }, _start_index, col);
         }
         else if (next == 'd' || next == 'D') {
            get_next_char();
         }

         return makeToken(T_LITERAL, { std::stod(t) }, _start_index, col);
      }
   }

   if (is_identifier_char(first)) {
      // advance keyword or identifier
      next = first;
      while (is_identifier_char(next)) {
         t += next;
         next = get_next_char();
      }

      if (t == "as" && next == '!') {
         get_next_char();
         t = "as!";
      }

      backtrack_c(1);

      if (is_operator(t)) {
         return makeToken(T_OP, { std::move(t) }, _start_index, col);
      }
      else if (t == "none") {
         return makeToken(T_LITERAL, { }, _start_index, col);
      }
      else if (is_keyword(t)) {
         return makeToken(T_KEYWORD, { std::move(t) }, _start_index, col);
      }
      else if (is_bool_literal(t)) {
         return makeToken(T_LITERAL, { t == "true" }, _start_index, col);
      }
      else if (!ignoreMacro && Macros.find(t) != Macros.end()) {
         expand_macro(std::move(t));
         return get_next_token(ignore_newline, significantWhiteSpace);
      }
      else if (!ignoreMacro && BuiltinMacros.find(t) != BuiltinMacros.end()) {
         return handlePreprocFunc(std::move(t));
      }
      else if (!ignoreValue && TemplateArgs.find(t) != TemplateArgs.end()) {
         expand_template_arg(std::move(t));
         return get_next_token(ignore_newline, significantWhiteSpace);
      }
      else {
         return makeToken(T_IDENT, { std::move(t) }, _start_index, col);
      }
   }

   if (is_operator_char(first)) {
      bool lastWasPeriod = first == '.';
      while (is_operator_char(first)) {
         t += first;
         first = get_next_char();
         if (lastWasPeriod && first != '.') {
            break;
         }

         lastWasPeriod = first == '.';
      }

      backtrack_c(1);

      if (t == ".") {
         return makeToken(T_PUNCTUATOR, { '.' }, _start_index, col);
      }

      while (!is_operator(t) && t.length() > 0) {
         t.pop_back();
         backtrack_c(1);
      }

      return makeToken(T_OP, { std::move(t) }, _start_index, col);
   }

   if (is_punctuator(first)) {
      if (first == '\n') {
         ++currentLine;
         indexOnLine = 0;
      }

      return makeToken(T_PUNCTUATOR, { first }, _start_index, col);
   }

   CDOT_DIAGNOSE(u8"Unexpected character " + std::string(1, first));
   llvm_unreachable(0);
}

/**
 * Advances to the next token, while expecting 'type'
 * @param type
 */
void Lexer::advance(TokenType type, bool ignore_newline,
                    bool significantWhiteSpace)
{
   advance(ignore_newline, significantWhiteSpace);
   if (currentToken.get_type() != type) {
      CDOT_DIAGNOSE("Expected " + util::token_names[type]
         + " but got " + util::token_names[currentToken.get_type()]);
   }
}

/**
 * Advances to the next token without restriction
 */
Token Lexer::get_next_token(bool ignore_newline, bool significantWhiteSpace)
{
   if (!lookaheadList.empty()) {
      auto tok = std::move(lookaheadList.back());
      lookaheadList.pop_back();

      return tok;
   }
   if (ExpansionStack.size() > macroExpansionDepth) {
      auto &latest = ExpansionStack.top();
      auto &lex = *latest.lex;

      auto tok = lex.get_next_token(ignore_newline, significantWhiteSpace);
      if (tok.get_type() == T_EOF) {
         ExpansionStack.pop();

         return get_next_token(ignore_newline, significantWhiteSpace);
      }

      return tok;
   }
//   else if (!TemplateArgStack.empty()) {
//      auto &top = TemplateArgStack.top();
//      if (top.tokenIndex >= top.tokens.size()) {
//         TemplateArgStack.pop();
//         return get_next_token(ignore_newline, significantWhiteSpace);
//      }
//      else {
//         auto tok = std::move(top.tokens[top.tokenIndex]);
//         top.tokenIndex++;
//         top.index += tok.getSourceLoc().getLength();
//
//         return tok;
//      }
//   }

   return lex_next_token(ignore_newline, significantWhiteSpace);
}

Token Lexer::makeToken(
   TokenType ty,
   Variant &&val,
   size_t start,
   size_t col,
   bool isEscaped)
{
   return Token(ty, std::move(val),
      SourceLocation(col, currentLine, currentIndex - start, sourceId),
      start);
}

LexerLoc Lexer::getLexerLoc()
{
   return LexerLoc { curr, currentIndex, currentLine, indexOnLine };
}

void Lexer::setLoc(LexerLoc &loc)
{
   curr = loc.curr;
   currentIndex = loc.currentIndex;
   currentLine = loc.currentLine;
   indexOnLine = loc.indexOnLine;
}

/**
 * Returns the next token
 * @return
 */
void Lexer::advance(bool ignore_newline, bool significantWhiteSpace)
{
   if (!tokens.empty() && tokens.back().get_type() == T_EOF) {
      currentToken = tokens.back();
      return;
   }

   pushToken(get_next_token(ignore_newline, significantWhiteSpace));

   Token &t = tokens.back();
   if ((t.is_punctuator('\n') && ignore_newline)
       || (t.is_punctuator(' ') && !significantWhiteSpace)
       || t.is_punctuator('\\')) {
      return advance(ignore_newline, significantWhiteSpace);
   }
   else if (t.get_type() == T_DIRECTIVE && !ignoreDirective) {
      handle_directive(std::move(t._value.strVal));
      return advance(ignore_newline, significantWhiteSpace);
   }
   else if (t.get_type() == T_PREPROC_VAR && !ignoreValue) {
      currentToken = t;
      substitute_value();
      t = get_next_token(ignore_newline, significantWhiteSpace);
   }
   else if (!ignoreValue && t.get_type() == T_IDENT) {
      auto &str = t._value.strVal;
      if (TemplateArgs.find(str) != TemplateArgs.end()) {
         expand_template_arg(std::move(str));
         t = get_next_token(ignore_newline, significantWhiteSpace);
      }
   }

   currentToken = t;
}

Token Lexer::lookahead(bool ignore_newline, size_t offset)
{
   bool wasNewLine = currentToken.is_punctuator('\n');
   for (size_t i = 0; i < offset + 1; ++i) {
      advance(ignore_newline);
   }

   auto tok = currentToken;
   for (size_t i = 0; i < offset + 1;) {
      backtrack();

      // backtrack doesn't skip newlines
      if (!ignore_newline || !currentToken.is_punctuator('\n')) {
         ++i;
      }
   }

   while (!wasNewLine && currentToken.is_punctuator('\n')) {
      backtrack();
   }

   if (ignore_newline && tok.is_punctuator('\n')) {
      return lookahead(ignore_newline, offset + 1);
   }

   return tok;
}

void Lexer::commit()
{
   tokens.clear();
}

/**
 * Skips to the end of a comment
 */
void Lexer::ignore_comment()
{
   char c = get_next_char();

   while (c != '\n') {
      if (currentIndex >= srcLen) {
         return;
      }

      c = get_next_char();
   }

   ++currentLine;
   indexOnLine = 0;
}

/**
 * Skips to the end of a multiline comment
 */
void Lexer::ignore_ml_comment()
{
   char c = get_next_char();

   main_loop:
   while (c != '*') {
      if (currentIndex >= srcLen) {
         return;
      }
      if (c == '\n') {
         ++currentLine;
         indexOnLine = 0;
      }

      c = get_next_char();
   }

   c = get_next_char();
   if (c != '/') {
      goto main_loop;
   }
}

void Lexer::skip_until_even(ParenKind kind)
{
   char open;
   char close;
   switch (kind) {
      case PAREN:
         open = '(';
         close = ')';
         break;
      case BRACE:
         open = '{';
         close = '}';
         break;
      case ANGLED:
         open = '<';
         close = '>';
         break;
      case SQUARE:
         open = '[';
         close = ']';
         break;
   }

   if (!currentToken.is_punctuator(open)) {
      return;
   }

   advance();

   unsigned openCount = 1;
   unsigned closeCount = 0;

   for (;;) {
      if (currentToken.is_punctuator(open)) {
         ++openCount;
      }
      else if (currentToken.is_punctuator(close)) {
         ++closeCount;
      }

      if (openCount == closeCount) {
         break;
      }

      advance();
   }
}

namespace cdot {
namespace util {

std::unordered_map<string, int> op_precedence {
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

string token_names[] = {
   "T_KEYWORD",
   "T_IDENT",
   "T_PUNCTUATOR",
   "T_OP",
   "T_LITERAL",
   "T_BOF",
   "T_EOF"
};

std::vector<string> binary_operators = {
   "=",
   "+=",
   "-=",
   "*=",
   "/=",
   "||",
   "&&",
   "|",
   "&",
   "!=",
   "==",
   "<=",
   ">=",
   "<",
   ">",
   "<<",
   ">>",
   ">>>",
   "+",
   "-",
   "*",
   "/",
   "%",
   "**",
   "..",
   "??",
   "^",
   "->",
   "=>",
   "as",
   "as!",
   "isa",
   "===",
   "!=="
};

std::vector<string> assignmentOperators = {
   "=", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", "&=", "|=", "^="
};

string isAssignmentOperator(const string &op)
{
   if (op == "=") {
      return op;
   }

   if (in_vector(assignmentOperators, op)) {
      return op.substr(0, op.length() - 1);
   }

   return "";
}

std::vector<char> operator_chars = {
   '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':', '.',
   '^'
};

std::vector<string> PrefixUnaryOperators = {
   "!",
   "+",
   "-",
   "~",
   "++",
   "--",
   "&",
   "*",
   "..."
};

std::vector<string> PostfixUnaryOperators = {
   "++",
   "--"
};

std::vector<string> tertiary_operators = {
   "?",
   ":"
};

std::vector<string> keywords = {
   "def",
   "return",
   "if",
   "else",
   "while",
   "loop",
   "match",
   "for",
   "case",
   "default",
   "struct",
   "throw",
   "try",
   "catch",
   "finally",
   "class",
   "union",
   "public",
   "private",
   "protected",
   "static",
   "abstract",
   "protocol",
   "prop",
   "enum",
   "with",
   "where",
   "namespace",
   "typeof",
   "continue",
   "break",
   "goto",
   "infix",
   "prefix",
   "postfix",
   "mutating",
   "declare",
   "using",
   "native",
   "let",
   "var",
   "extend",
   "typedef",
   "init",
   "deinit",
   "ref",
   "unsafe",
   "memberwise",
   "__debug",
   "__unreachable"
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
   '#'
};

} // namespace util
} // namespace cdot