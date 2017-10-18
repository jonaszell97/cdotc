//
// Created by Jonas Zell on 18.06.17.
//

#include "Lexer.h"

#include "Util.h"
#include "Variant/Variant.h"
#include "Message/Exceptions.h"

#include "llvm/Support/MemoryBuffer.h"

namespace {
   string empty;
}

Lexer::Lexer(llvm::MemoryBuffer *buf, string const& fileName, unsigned sourceId) :
   src(buf->getBufferStart()),
   srcLen(buf->getBufferSize()),
   sourceId(sourceId),
   fileName(fileName)
{
   current_token = Token(T_BOF, {}, SourceLocation(), 0, 0);
}

Lexer::Lexer() : src(nullptr), fileName(empty)
{

}

void Lexer::reset(const char *src, size_t len)
{
   this->src = src;
   current_token = Token(T_BOF, {}, SourceLocation(), 0, 0);
   continueInterpolation = false;
   indexOnLine = 0;
   currentLine = 1;
   current_index = 0;
   last_token_index = 0;
   current_token_index = 0;
   srcLen = len;
   tokens.clear();
}

size_t Lexer::lastTokenEnd()
{
   if (tokens.size() < 2) {
      return 0;
   }

   return tokens[current_token_index - 1].getEnd();
}

std::string Lexer::s_val()
{
   if (current_token.get_value().strVal.empty() && current_token.get_type() != T_LITERAL) {
      ParseError::raise("Unexpected token " + current_token.toString(), this);
   }

   return current_token.get_value().strVal;
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
   return (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') || (c >= '0' && c <= '9');
}

/**
 * Decides whether or not a character could be part of a keyword or identifier token.
 * @param _c
 * @return
 */
bool Lexer::is_identifier_char(char c) {
   // valid identifiers contain ANYTHING (almost)
   return !is_operator_char(c) && !is_punctuator(c) && c != ' ' && c != '\0';
}

std::regex validOp(R"_(([\+\-\*/%&\|!=<>~\^,\?:]|\.\.)+)_");

/**
 * Decides whether a value can be an identifier or not.
 * @param _c
 * @return
 */
bool Lexer::is_operator(std::string s) {
   if (std::regex_match(s, validOp)) {
      return true;
   }
   if (util::in_vector(util::binary_operators, s)) {
      return true;
   }
   if (util::in_vector(util::assignmentOperators, s)) {
      return true;
   }
   if (util::in_vector(util::PrefixUnaryOperators, s)) {
      return true;
   }
   if (util::in_vector(util::PostfixUnaryOperators, s)) {
      return true;
   }

   return false;
}

bool Lexer::is_operator_char(char c) {
   if (util::in_vector(util::operator_chars, c)) {
      return true;
   }

   return false;
}

/**
 * Returns true if the given string is a language keyword.
 * @param s
 * @return
 */
bool Lexer::is_keyword(std::string s) {
   if (util::in_vector(util::keywords, s)) {
      return true;
   }

   return false;
}

bool Lexer::is_punctuator(char c) {
   return util::in_vector(util::punctuators, c);
}

bool Lexer::is_bool_literal(std::string s) {
   return s == "true" || s == "false";
}


/**
 * Gets the next character in the program
 * @return
 */
char Lexer::get_next_char() {
   if (current_index >= srcLen) {
      current_index++;
      return '\0';
   }

   ++indexOnLine;
   return src[current_index++];
}

char Lexer::char_lookahead() {
   if (current_index >= srcLen) {
      current_index++;
      return '\0';
   }

   return src[current_index];
}

/**
 * Backtracks to the last token
 */
void Lexer::backtrack() {
   --current_token_index;
   if (current_token_index <= 0) {
      current_token_index = 0;
      return;
   }

   current_token = tokens[current_token_index - 1];
}

void Lexer::hardBacktrack()
{
   if (!tokens.empty()) {
      tokens.pop_back();
   }

   backtrack();
}

/**
 * Backtracks by the passed amount of characters
 * @param length
 */
void Lexer::backtrack_c(int length) {
   current_index -= length;
   indexOnLine -= length;
}

char Lexer::escape_char(char c) {
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

string Lexer::unescape_char(char c) {
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
Token Lexer::_get_next_token(bool ignore_newline, bool significantWhiteSpace) {
   if (current_token.get_type() == T_EOF) {
      ParseError::raise("Unexpected end of file", this);
   }

   last_token_index = current_index;

   if (current_index >= srcLen) {
      return makeToken(T_EOF, { }, current_index, 0);
   }

   string t;
   bool beginningOfLine = true;
   auto index = current_index == 0 ? 0 : current_index - 1;
   while (index > 0) {
      char c = src[index--];
      if (c == '\n') {
         break;
      }
      if (c != ' ') {
         beginningOfLine = false;
         break;
      }
   }

   size_t _start_index = current_index;
   size_t indent = 0;
   size_t col;
   char first;
   char next;

   if (continueInterpolation) {
      goto string_literal;
   }

   first = get_next_char();
   if (first == ' ') {
      index = current_index;
      while (src[index] == ' ' && index < srcLen) {
         ++index;
      }

      if (src[index] == '#' && src[index + 1] != '{') {
         indent = index >= current_index ? current_index - index : 0;
         current_index = index + 1;
         first = '#';
      }
   }

   if (first == ' ' && significantWhiteSpace) {
      return makeToken(T_PUNCTUATOR, { first }, current_index, indexOnLine);
   }


   col = indexOnLine;
   while (first == ' ') {
      first = get_next_char();
      ++_start_index;
      ++col;
   }

   if (first == '\0' && current_index >= srcLen) {
      return makeToken(T_EOF, { }, current_index, col);
   }

   // comment
   if (first == '/') {
      next = get_next_char();
      if (next == '*') {
         ignore_ml_comment();

         return _get_next_token(ignore_newline, significantWhiteSpace);
      }
      else if (next == '/') {
         ignore_comment();
         return _get_next_token(ignore_newline, significantWhiteSpace);
      }
      else {
         backtrack_c(1);
      }
   }

   // preprocessor variable
   if (first == '#' && char_lookahead() == '{') {
      get_next_char();
      next = get_next_char();

      while (next != '}' && current_index < srcLen) {
         t += next;
         next = get_next_char();
      }

      return makeToken(T_PREPROC_VAR, { t }, _start_index, col);
   }

   // preprocessor directive
   if (beginningOfLine && first == '#') {
      next = get_next_char();
      while (next != ' ' && next != '\n' && next != '\0') {
         t += next;
         next = get_next_char();
      }

      auto tok = makeToken(T_DIRECTIVE, { t }, _start_index, col);
      tok.setIndent(indent);

      return tok;
   }

   // character literal
   if (first == '\'') {
      next = get_next_char();
      if (next == '\\') {
         next = escape_char(get_next_char());
      }

      char _c = get_next_char();
      if (_c != '\'') {
         ParseError::raise("Expected \"'\" after character literal", this);
      }

      return makeToken(T_LITERAL, { next }, _start_index, col);
   }

   // string literal
   if (first == '\"') {
      string_literal:
      while (current_index < srcLen) {
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
                  auto tok = makeToken(T_LITERAL, {t}, _start_index, col);
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
      return makeToken(T_LITERAL, { t }, _start_index, col);
   }

   // escape sequence
   if (first == '`') {
      next = get_next_char();
      while (next != '`' && current_index < srcLen) {
         t += next;
         next = get_next_char();

      }

      return makeToken(T_IDENT, { t }, _start_index, col, true);
   }

   if (first == '-') {
      next = char_lookahead();
      if (next >= 48 && next <= 57) {
         t += "-";
         first = next;
         get_next_char();
      }
   }

   // number literal (decimal, octal, hexadecimal or binary; with or without exponent or floating point)
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
         return makeToken(T_LITERAL, { long(std::stoul(hex_s, nullptr, 16)) }, _start_index, col);
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
         return makeToken(T_LITERAL, { long(std::stoul(bin_s, nullptr, 2)) }, _start_index, col);
      }

      backtrack_c(1); // undo _pref

      // advance integer or floating point number
      next = first;
      bool decimal_point = false;
      while ((next >= '0' && next <= '9') || next == '_' || next == '.') {
         if (next == '.') {
            if (char_lookahead() < '0' || char_lookahead() > '9' || decimal_point) {
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
      if (util::matches("0[0-7]+", t)) {
         backtrack_c(1);
         return makeToken(T_LITERAL, { std::stoi(t, nullptr, 8) }, _start_index, col);
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
         double exp = pow(10, std::stoi(_exp));

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
            val = Variant(std::stol(t));
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
            val = Variant((long)std::stoul(t));
         }

         val.isUnsigned = isUnsigned;

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
         return makeToken(T_OP, { t }, _start_index, col);
      }
      else if (t == "none") {
         return makeToken(T_LITERAL, { }, _start_index, col);
      }
      else if (is_keyword(t)) {
         return makeToken(T_KEYWORD, { t }, _start_index, col);
      }
      else if (is_bool_literal(t)) {
         return makeToken(T_LITERAL, { t == "true" }, _start_index, col);
      }
      else {
         return makeToken(T_IDENT, { t }, _start_index, col);
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

      return makeToken(T_OP, { t }, _start_index, col);
   }

   if (is_punctuator(first)) {
      if (first == '\n') {
         ++currentLine;
         indexOnLine = 0;
      }

      return makeToken(T_PUNCTUATOR, { first }, _start_index, col);
   }

   ParseError::raise(u8"Unexpected character " + std::string(1, first), this);
   llvm_unreachable(0);
}

/**
 * Advances to the next token, while expecting 'type'
 * @param type
 */
void Lexer::advance(TokenType type, bool ignore_newline, bool significantWhiteSpace) {
   current_token = get_next_token(ignore_newline, significantWhiteSpace);
   if (current_token.get_type() != type) {
      ParseError::raise("Expected " + util::token_names[type]
         + " but got " + util::token_names[current_token.get_type()], this);
   }
}

/**
 * Advances to the next token without restriction
 */
void Lexer::advance(bool ignore_newline, bool significantWhiteSpace) {
   current_token = get_next_token(ignore_newline, significantWhiteSpace);
}

Token Lexer::makeToken(
   TokenType ty,
   Variant &&val,
   size_t start,
   size_t col,
   bool isEscaped)
{
   return Token(ty, std::move(val),
      SourceLocation(col, currentLine, current_index - start, sourceId),
      start, isEscaped);
}

/**
 * Returns the next token
 * @return
 */
Token Lexer::get_next_token(bool ignore_newline, bool significantWhiteSpace)
{
   if (!tokens.empty() && tokens.back().get_type() == T_EOF) {
      return tokens.back();
   }

   if (current_token_index >= tokens.size()) {
      tokens.push_back(_get_next_token(ignore_newline, significantWhiteSpace));
   }

   Token t = tokens[current_token_index++];
   if (t.is_punctuator('\n')) {
      if (ignore_newline) {
         return get_next_token(true, significantWhiteSpace);
      }
   }

   last_token_index = t.getStart();

   return t;
}

/**
 * Looks ahead to the next token without influencing the parsing
 * @return
 */
Token Lexer::lookahead(bool ignore_newline, size_t offset) {
   if (!tokens.empty() && tokens.back().get_type() == T_EOF) {
      return tokens.back();
   }

   while ((current_token_index + offset) >= tokens.size()) {
      tokens.push_back(_get_next_token(ignore_newline, false));
   }

   Token t = tokens[current_token_index + offset];
   if (t.is_punctuator('\n')) {
      if (ignore_newline) {
         t = lookahead(ignore_newline, offset + 1);
      }
   }

   return t;
}

/**
 * Skips to the end of a comment
 */
void Lexer::ignore_comment() {
   char c = get_next_char();

   while (c != '\n') {
      if (current_index >= srcLen) {
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
void Lexer::ignore_ml_comment() {
   char c = get_next_char();

   main_loop:
   while (c != '*') {
      if (current_index >= srcLen) {
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
