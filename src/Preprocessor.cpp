//
// Created by Jonas Zell on 18.10.17.
//

#include "Lexer.h"
#include "Message/Exceptions.h"
#include "Util.h"
#include "Compiler.h"
#include "Message/Diagnostics.h"

#include <unordered_map>
#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include <iomanip>

unordered_map<string, Lexer::Macro> Lexer::GlobalMacros;
unordered_map<string, Lexer::Macro> Lexer::Macros;

unordered_map<string, Variant> Lexer::GlobalValues;
unordered_map<string, Variant> Lexer::Values;

unordered_map<string, std::vector<Variant>> Lexer::GlobalArrays;
unordered_map<string, std::vector<Variant>> Lexer::Arrays;

std::stack<Lexer::MacroExpansion> Lexer::ExpansionStack;

using namespace cdot::diag;

namespace cdot {

enum class BuiltinMacro : unsigned {
   TOLOWER,
   TOUPPER,
   TOSTRING,
   NUMFORMAT,
   DEFINED,
   REPEAT,
   LINE,
   FILE
};

unordered_map<string, pair<BuiltinMacro, int>> BuiltinMacros = {
   { "_ToLower", {BuiltinMacro::TOLOWER, 1} },
   { "_ToUpper", {BuiltinMacro::TOUPPER, 1} },
   { "_ToString", {BuiltinMacro::TOSTRING, 1} },
   { "_NumFormat", {BuiltinMacro::NUMFORMAT, 2} },
   { "_Repeat", {BuiltinMacro::REPEAT, 2} },
   { "__LINE__", {BuiltinMacro::LINE, 0} },
   { "__FILE__", {BuiltinMacro::FILE, 0} }
};

}

namespace {

//region Description
unordered_map<string, Variant> BuiltinValues = {
   { "_WordSize", Variant((long)(sizeof(size_t) * 8)) },
   { "__DEBUG__", Variant(true) },
   { "_CLOCKS_PER_SEC", Variant((long)CLOCKS_PER_SEC) },
   { "EOF", Variant((long)EOF) },
   { "SEEK_SET", Variant((long)SEEK_SET) },
   { "SEEK_END", Variant((long)SEEK_END) },
   { "SEEK_CUR", Variant((long)SEEK_CUR) },
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
   { "__builtin_eh_data_regno_0", Variant(__builtin_eh_return_data_regno(0)) },
   { "__builtin_eh_data_regno_1", Variant(__builtin_eh_return_data_regno(1)) }
};

}

Variant Lexer::currentTokenValue()
{
   switch (currentToken.get_type()) {
      case T_LITERAL:
      case T_IDENT:
         return currentToken.get_value();
      case T_OP: {
         auto op = s_val();
         advance();
         auto target = currentTokenValue();

         return target.applyUnaryOp(op);
      }
      case T_PREPROC_VAR: {
         auto ident = s_val();
         if (BuiltinValues.find(ident) != BuiltinValues.end()) {
            return BuiltinValues[ident];
         }
         if (Values.find(ident) != Values.end()) {
            return Values[ident];
         }

         return Variant(ident);
      }
      case T_PUNCTUATOR: {
         if (currentToken.is_punctuator('(')) {
            advance();
            auto expr = parseExpression();

            advance();
            if (!currentToken.is_punctuator(')')) {
               ParseError::raise("Expected ')'", this);
            }

            return expr;
         }
      }
      default:
         ParseError::raise("Unexpected token in preprocessor expression", this);
         llvm_unreachable(0);
   }
}

Variant Lexer::parseExpression(Variant lhs, int minPrecedence)
{
   bool prev = ignoreDirective;
   ignoreDirective = true;

   if (lhs.isVoid()) {
      lhs = currentTokenValue();
   }

   auto next = lookahead(false);
   while (next.get_type() == T_OP && util::op_precedence[next.get_value().strVal] >= minPrecedence) {
      advance();

      string op = s_val();
      advance();

      auto rhs = currentTokenValue();

      next = lookahead();
      while (next.get_type() == T_OP && util::op_precedence[next.get_value().strVal] > util::op_precedence[op]) {
         rhs = parseExpression(rhs, util::op_precedence[next.get_value().strVal]);
         next = lookahead();
      }

      lhs = lhs.applyBinaryOp(rhs, op);
   }

   if (next.is_punctuator('.') && lhs.type == VariantType::STRING) {
      advance();
      advance();

      lhs = Variant(lhs.strVal + '.' + parseExpression().toString());
   }

   ignoreDirective = prev;

   return lhs;
}

void Lexer::handle_directive(string &&dir)
{
   if (dir == "define") {
      handle_define();
   }
   else if (dir == "undef") {
      handle_undef();
   }
   else if (dir == "if") {
      handle_rawif();
   }
   else if (dir == "ifdef") {
      handle_ifdef();
   }
   else if (dir == "ifndef") {
      handle_ifndef();
   }
   else if (dir == "endif") {
      assert(openIfs);
      --openIfs;
   }
   else if (dir == "else" || dir == "elseif") {
      skip_until_endif();
   }
   else if (dir == "pragma") {
      handle_pragma();
   }
   else if (dir == "print") {
      handle_print();
   }
   else if (dir == "let") {
      handle_let();
   }
   else if (dir == "for") {
      handle_for();
   }
   else if (dir == "endfor") {
      handle_endfor();
   }
}

void Lexer::handle_define()
{
   advance();

   string name = s_val();
   std::vector<string> args;

   if (char_lookahead() == '\n') {
      Macros.emplace(name, Macro());
      return;
   }

   IgnoreScope guard(this);

   advance();

   if (currentToken.is_punctuator('(')) {
      advance();
      while (!currentToken.is_punctuator(')')) {
         args.push_back(s_val());
         advance();
         if (currentToken.is_punctuator(',')) {
            advance();
         }
      }
   }
   else {
      backtrack(true);
   }

   const char *begin = curr;
   size_t len = 0;
   auto loc = getLexerLoc();

   char next = get_next_char();
   for (;;) {
      if (next == '\\') {
         ++len;
         next = get_next_char();

         if (next == '\n') {
            ++currentLine;
            indexOnLine = 0;
         }
      }
      else if (next == '\n') {
         ++currentLine;
         indexOnLine = 0;
         break;
      }
      else if (currentIndex > srcLen) {
         break;
      }

      ++len;
      next = get_next_char();
   }

   Macros.emplace(name, Macro { begin, len, args, loc });
}

void Lexer::handle_undef()
{
   advance();

   auto name = s_val();
   auto index = Macros.find(name);
   if (index != Macros.end()) {
      Macros.erase(index);
   }
}

namespace {
void tokenError(Lexer* lexer) {
   ParseError::raise("Unexpected token " +
                     util::token_names[lexer->currentToken.get_type()], lexer);
}
}

void Lexer::handle_let()
{
   advance();

   if (currentToken.get_type() != T_IDENT) {
      tokenError(this);
   }

   auto name = s_val();
   advance();

   if (!currentToken.is_operator("=")) {
      tokenError(this);
   }

   advance();

   if (currentToken.is_punctuator('[')) {
      std::vector<Variant> elements;
      while (!currentToken.is_punctuator(']')) {
         advance();
         if (currentToken.is_punctuator(',')) {
            continue;
         }
         if (currentToken.is_punctuator(']')) {
            break;
         }

         elements.emplace_back(currentTokenValue());
      }

      Arrays[name] = elements;
   }
   else {
      Values[name] = parseExpression();
   }
}

void Lexer::handle_print()
{
   advance();

   auto expr = parseExpression();
   llvm::outs() << expr.toString() << "\n";
}

void Lexer::handle_pragma()
{

}

void Lexer::handle_rawif()
{
   advance();
   auto cond = parseExpression();

   if (cond.type != VariantType::INT) {
      llvm_unreachable(0); //FIXME
   }

   handle_if(cond.intVal != 0);
}

void Lexer::handle_ifdef()
{
   ignoreMacro = true;
   advance();
   ignoreMacro = false;

   auto name = s_val();
   handle_if(Macros.find(name) != Macros.end());
}

void Lexer::handle_ifndef()
{
   advance();
   auto name = s_val();

   handle_if(Macros.find(name) == Macros.end());
}

void Lexer::handle_if(bool condition)
{
   ++openIfs;
   if (condition) {
      return;
   }

   IgnoreScope guard(this);
   unsigned innerIfs = 0;
   for (;;) {
      while (currentToken.get_type() != T_DIRECTIVE) {
         advance();
      }

      auto &dir = currentToken._value.strVal;
      if (dir == "if") {
         ++innerIfs;
      }
      else if (!innerIfs && dir == "elseif") {
         return handle_rawif();
      }
      else if (!innerIfs && dir == "else") {
         return;
      }
      else if (dir == "endif") {
         if (innerIfs) {
            --innerIfs;
         }
         else {
            return;
         }
      }

      advance();
   }
}

void Lexer::skip_until_directive(const string &dir)
{
   auto next = get_next_char();
   for (;;) {
      if (next == '#') {
         string found;

         next = get_next_char();
         while (next == ' ') {
            next = get_next_char();
         }

         while (next >= 'a' && next <= 'z') {
            found += next;
            next = get_next_char();
         }

         if (found == dir) {
            return;
         }
      }

      if (currentIndex >= srcLen) {
         ParseError::raise("expected #" + dir, this);
      }

      next = get_next_char();
   }
}

// only called after an if / elseif that was entered
void Lexer::skip_until_endif()
{
   if (!openIfs) {
      tokenError(this);
   }

   skip_until_directive("endif");
}

void Lexer::handle_for()
{
   advance();

   auto elName = s_val();
   advance();

   if (Values.find(elName) != Values.end()) {
      ParseError::raise("variable " + elName + " already defined", this);
   }

   assert(s_val() == "in");
   advance();

   auto arrName = s_val();
   auto it = Arrays.find(arrName);

   if (it == Arrays.end()) {
      ParseError::raise("array " + arrName + " not declared", this);
   }

   ForStack.push(ForScope {
      getLexerLoc(), elName,
      it->second.begin(), it->second.end()
   });

   if (it->second.empty()) {
      skip_until_directive("endfor");
      return;
   }

   Values.emplace(elName, *it->second.begin());
}

void Lexer::handle_endfor()
{
   if (ForStack.empty()) {
      tokenError(this);
   }

   auto &latest = ForStack.top();
   ++latest.it;

   if (latest.it != latest.end) {
      setLoc(latest.loc);
      Values[latest.varName] = *latest.it;
      return;
   }

   ForStack.pop();
   Values.erase(Values.find(latest.varName));
}

std::vector<Lexer::Macro> Lexer::getMacroArgs()
{
   std::vector<Macro> args;
   auto buf = curr;
   while (*buf == ' ') {
      ++buf;
   }

   if (*buf == '(') {
      auto openParens = 1;
      auto closeParens = 0;

      currentIndex += curr - buf + 1;
      indexOnLine += curr - buf + 1;
      curr = buf + 1;

      const char *begin = curr;
      char next = get_next_char();
      size_t len = 0;

      while (openParens != closeParens) {
         if (next == ',') {
            args.push_back(Macro { begin, len, {}, getLexerLoc() });
            len = 0;
            begin = curr;
            if (char_lookahead() == ' ') {
               get_next_char();
            }

            next = get_next_char();
            continue;
         }

         if (next == '(') {
            ++openParens;
         }
         else if (next == ')') {
            ++closeParens;
            if (openParens == closeParens) {
               break;
            }
         }

         ++len;
         next = get_next_char();
      }

      if (len > 0) {
         args.push_back(Macro { begin, len, {}, getLexerLoc() });
      }
   }

   return args;
}

void Lexer::expand_macro(string &&macroName)
{
   auto args = getMacroArgs();

   auto it = Macros.find(macroName);
   assert(it != Macros.end());

   if (args.size() != it->second.args.size()) {
      ParseError::raise("wrong number of arguments passed to macro " + macroName, this);
   }

   auto numArgs = args.size();
   unsigned i = 0;
   for (; i < numArgs; ++i) {
      Macros.emplace(it->second.args[i], args[i]);
   }

   auto _buff = llvm::MemoryBuffer::getMemBufferCopy(llvm::StringRef(it->second.begin, it->second.length));
   auto buff = _buff.release();

   auto lex = std::make_unique<Lexer>(buff, fileName, sourceId);
   lex->currentLine = it->second.loc.currentLine;
   lex->indexOnLine = it->second.loc.indexOnLine;
   lex->macroExpansionDepth = macroExpansionDepth + 1;

   if (macroExpansionDepth > Compiler::getOptions().maxMacroRecursionDepth) {
      diag::err(err_max_macro_recursion_depth)
         << (size_t)Compiler::getOptions().maxMacroRecursionDepth
         << macroName << this << diag::term;
   }

   ExpansionStack.emplace(std::move(lex), buff);
}

Token Lexer::handlePreprocFunc(
   string &&macroName)
{
   auto macro = BuiltinMacros[macroName];
   std::vector<Variant> args;

   auto buf = curr;
   while (*buf == ' ') {
      ++buf;
   }

   bool isToString = macroName == "_ToString";
   if (macroName == "_ToString") {
      if (*buf != '(') {
         ParseError::raise("expected '('", this);
      }

      currentIndex += curr - buf + 1;
      indexOnLine += curr - buf + 1;
      curr = buf + 1;

      ignoreValue = true;
      advance();

      if (currentToken.get_type() != T_PREPROC_VAR) {
         ParseError::raise("_ToString expects a preprocessor variable", this);
      }

      string str;
      auto &ident = currentToken._value.strVal;
      if (Values.find(ident) != Values.end()) {
         str = Values[ident].toString();
      }
      else {
         str = ident;
      }

      args.emplace_back(str);
      advance();

      if (!currentToken.is_punctuator(')')) {
         ParseError::raise("expected ')'", this);
      }
   }
   else if (*buf == '(') {
      currentIndex += curr - buf + 1;
      indexOnLine += curr - buf + 1;
      curr = buf + 1;
      advance();

      while (!currentToken.is_punctuator(')')) {
         args.push_back(parseExpression());
         advance();

         if (currentToken.is_punctuator(',')) {
            advance();
         }
      }
   }

   if (args.size() != macro.second) {
      ParseError::raise("wrong number of arguments passed to macro " + macroName, this);
   }

   switch (macro.first) {
      case BuiltinMacro::TOLOWER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::tolower);

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(str);

         return tok;
      }
      case BuiltinMacro::TOUPPER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::toupper);

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(str);

         return tok;
      }
      case BuiltinMacro::TOSTRING: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant(args.front().toString());

         return tok;
      }
      case BuiltinMacro::NUMFORMAT: {
         if (args.front().type != VariantType::INT || args[1].type != VariantType::INT) {
            ParseError::raise("_NumFormat expects integers as 1st and 2nd arguments", this);
         }

         auto &val = args.front().intVal;
         auto &base = args[1].intVal;

         std::ostringstream out;
         out << std::setbase((int) base) << val;

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(out.str());

         return tok;
      }
      case BuiltinMacro::REPEAT: {
         auto &str = args.front().strVal;

         assert(args[1].type == VariantType::INT);
         auto &arg2 = args[1].intVal;

         string res;
         for (unsigned i = 0; i < arg2; ++i) {
            res += str;
         }

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(res);

         return tok;
      }
      case BuiltinMacro::FILE: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant(fileName);

         return tok;
      }
      case BuiltinMacro::LINE: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant((long)currentLine);

         return tok;
      }
      default:
         llvm_unreachable("unknown macro");
   }
}

void Lexer::substitute_value()
{
   assert(currentToken.get_type() == T_PREPROC_VAR);
   auto &name = currentToken._value.strVal;
   auto &tok = currentToken;

   Variant *val;
   auto it = Values.find(name);
   if (it != Values.end()) {
      val = &it->second;
   }
   else {
      auto it = BuiltinValues.find(name);
      if (it == BuiltinValues.end()) {
         ParseError::raise("unknown value " + name, this);
      }

      val = &it->second;
   }

   auto _buff = llvm::MemoryBuffer::getMemBufferCopy(val->toString());
   auto buff = _buff.release();

   auto lex = std::make_unique<Lexer>(buff, fileName, sourceId);
   lex->currentLine = currentLine;
   lex->indexOnLine = indexOnLine;
   lex->macroExpansionDepth = macroExpansionDepth + 1;

   ExpansionStack.emplace(std::move(lex), buff);
}