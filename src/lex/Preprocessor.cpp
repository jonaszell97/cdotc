//
// Created by Jonas Zell on 18.10.17.
//

#include "Lexer.h"

#include <unordered_map>
#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include <iomanip>

#ifndef CDOT_LEXER_STANDALONE
#include "../Message/Diagnostics.h"
#endif

unordered_map<string, Lexer::Macro> Lexer::GlobalMacros;
unordered_map<string, Variant> Lexer::GlobalValues;

std::stack<Lexer::MacroExpansion> Lexer::ExpansionStack;

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
   { "_WordSize", Variant((unsigned long long)(sizeof(size_t) * 8)) },
   { "__DEBUG__", Variant(true) },
   { "_CLOCKS_PER_SEC", Variant((unsigned long long)CLOCKS_PER_SEC) },
   { "EOF", Variant((unsigned long long)EOF) },
   { "SEEK_SET", Variant((unsigned long long)SEEK_SET) },
   { "SEEK_END", Variant((unsigned long long)SEEK_END) },
   { "SEEK_CUR", Variant((unsigned long long)SEEK_CUR) },
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

} // anonymous namespace

Variant Lexer::currentTokenValue(bool allowUndeclaredIdentifiers)
{
   switch (currentToken.get_type()) {
      case T_LITERAL:
         return currentToken._value;
      case T_OP: {
         auto &op = strRef();
         advance();
         auto target = currentTokenValue(allowUndeclaredIdentifiers);

         return target.applyUnaryOp(op);
      }
      case T_IDENT: {
         auto &ident = strRef();
         if (BuiltinValues.find(ident) != BuiltinValues.end()) {
            return BuiltinValues[ident];
         }
         if (Values.find(ident) != Values.end()) {
            return Values[ident];
         }
         if (allowUndeclaredIdentifiers) {
            return currentToken._value;
         }

         CDOT_DIAGNOSE("undeclared identifier" + ident);
      }
      case T_PUNCTUATOR: {
         if (currentToken.is_punctuator('(')) {
            advance();
            auto expr = parseExpression({}, 0, false,
                                        allowUndeclaredIdentifiers);

            advance();
            if (!currentToken.is_punctuator(')')) {
               CDOT_DIAGNOSE("Expected ')'");
            }

            return expr;
         }
         if (currentToken.is_punctuator('[')) {
            return parseArray();
         }
      }
      default:
         CDOT_DIAGNOSE("Unexpected token in preprocessor expression");
         llvm_unreachable(0);
   }
}

Variant Lexer::parseArray()
{
   std::vector<Variant> vec;
   while (!currentToken.is_punctuator(']')) {
      advance();
      if (currentToken.is_punctuator(',')) {
         continue;
      }
      if (currentToken.is_punctuator(']')) {
         break;
      }

      vec.push_back(currentTokenValue(true));
   }

   return { std::move(vec) };
}

bool Lexer::maybeClosingTemplateBracket(Token &next)
{
   if (next.is_operator(">")) {
      auto buf = curr;
      auto idx = currentIndex;

      while (idx < srcLen) {
         if (*buf == '>') {
            return true;
         }
         if (*buf == '\n') {
            return true;
         }

         ++idx;
         ++buf;
      }
   }

   return false;
}

Variant Lexer::parseExpression(Variant lhs,
                               int minPrecedence,
                               bool isTemplateArg,
                               bool allowUndeclaredIdentifiers) {
   bool prev = ignoreDirective;
   ignoreDirective = true;

   if (lhs.isVoid()) {
      lhs = currentTokenValue(allowUndeclaredIdentifiers);
   }

   auto next = lookahead(false);

   // if no '>' follows on the same line, parse as closing angled brace
   while (next.get_type() == T_OP
          && util::op_precedence[next.get_value().strVal] >= minPrecedence
          && (!isTemplateArg || !maybeClosingTemplateBracket(next))) {
      advance();

      string op = strVal();
      advance();

      auto rhs = currentTokenValue(allowUndeclaredIdentifiers);

      next = lookahead();
      while (next.get_type() == T_OP
             && util::op_precedence[next.get_value().strVal]
                > util::op_precedence[op]
             && (!isTemplateArg || !maybeClosingTemplateBracket(next))) {
         rhs = parseExpression(rhs,
                               util::op_precedence[next.get_value().strVal]);
         next = lookahead();
      }

      lhs = lhs.applyBinaryOp(rhs, op);
   }

   if (next.is_punctuator('.') && lhs.type == VariantType::STRING) {
      advance();
      advance();

      lhs = Variant(lhs.strVal + '.'
                    + parseExpression({}, 0, isTemplateArg,
                                      allowUndeclaredIdentifiers).toString());
   }

   ignoreDirective = prev;

   return lhs;
}

Token Lexer::valueToToken(SourceLocation loc, Variant &&val)
{
   Token tok;
   if (val.isStr()) {
      tok._type = T_IDENT;
   }
   else {
      tok._type = T_LITERAL;
   }

   tok._value = Variant(std::move(val));
   tok.loc = loc;

   return std::move(tok);
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

   string name = strVal();
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
         args.push_back(strVal());
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

   auto name = strVal();
   auto index = Macros.find(name);
   if (index != Macros.end()) {
      Macros.erase(index);
   }
}

void Lexer::tokenError()
{
   CDOT_DIAGNOSE("Unexpected token " +
                 util::token_names[currentToken.get_type()])
}

void Lexer::handle_let()
{
   advance();

   if (currentToken.get_type() != T_IDENT) {
      tokenError();
   }

   auto name = strVal();
   advance();

   if (!currentToken.is_operator("=")) {
      tokenError();
   }

   advance();

   Values[name] = parseExpression();
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

   auto name = strVal();
   handle_if(Macros.find(name) != Macros.end());
}

void Lexer::handle_ifndef()
{
   advance();
   auto name = strVal();

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
         CDOT_DIAGNOSE("expected #" + dir);
      }

      next = get_next_char();
   }
}

// only called after an if / elseif that was entered
void Lexer::skip_until_endif()
{
   if (!openIfs) {
      tokenError();
   }

   skip_until_directive("endif");
}

void Lexer::handle_for()
{
   advance();

   auto elName = strVal();
   advance();

   if (Values.find(elName) != Values.end()) {
      CDOT_DIAGNOSE("variable " + elName + " already defined");
   }

   assert(currentToken._value.strVal == "in");

   size_t line = currentLine;
   size_t col = indexOnLine;
   size_t begin = currentIndex;
   auto buff = curr;

   while (*buff != '\n' && begin < srcLen) {
      ++begin;
      ++col;
      ++buff;
   }

   advance();
   auto arr = parseExpression();
   if (!arr.isArray()) {
      CDOT_DIAGNOSE("#for-in requires array type");
   }

   if (arr.vec->empty()) {
      skip_until_directive("endfor");
      return;
   }

   std::vector<Variant> reversedVec;
   for (auto i = arr.vec->size() - 1;; --i) {
      reversedVec.push_back(arr[i]);
      if (i == 0) {
         break;
      }
   }

   Values[elName] = std::move(reversedVec.back());
   reversedVec.pop_back();

   ForStack.push(ForScope {
      { line, col, begin }, elName,
      std::move(reversedVec)
   });
}

void Lexer::handle_endfor()
{
   if (ForStack.empty()) {
      tokenError();
   }

   auto &latest = ForStack.top();

   if (!latest.vec.empty()) {
      currentLine = latest.loc.line;
      indexOnLine = latest.loc.col;
      curr = begin + latest.loc.begin;
      currentIndex = latest.loc.begin;
      lookaheadList.clear();

      Values[latest.varName] = std::move(latest.vec.back());
      latest.vec.pop_back();

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
      CDOT_DIAGNOSE("wrong number of arguments passed to macro "
                        + macroName);
   }

   auto numArgs = args.size();
   unsigned i = 0;
   for (; i < numArgs; ++i) {
      Macros.emplace(it->second.args[i], args[i]);
   }

   auto _buff = llvm::MemoryBuffer::getMemBufferCopy(
      llvm::StringRef(it->second.begin, it->second.length));
   auto buff = _buff.release();

   auto lex = std::make_unique<Lexer>(buff, fileName, sourceId);
   lex->currentLine = it->second.loc.currentLine;
   lex->indexOnLine = it->second.loc.indexOnLine;
   lex->macroExpansionDepth = macroExpansionDepth + 1;

   if (macroExpansionDepth > 256) {
      CDOT_DIAGNOSE("max recursion depth reached");
   }

   ExpansionStack.emplace(std::move(lex), buff);
}

void Lexer::expand_template_arg(string &&argName)
{
   auto &arg = TemplateArgs[argName];
   lookaheadList.insert(lookaheadList.end(), arg.rbegin(), arg.rend());
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
         CDOT_DIAGNOSE("expected '('");
      }

      currentIndex += curr - buf + 1;
      indexOnLine += curr - buf + 1;
      curr = buf + 1;

      ignoreValue = true;
      advance();

      if (currentToken.get_type() != T_PREPROC_VAR) {
         CDOT_DIAGNOSE("_ToString expects a preprocessor variable");
      }

      string str;
      auto &ident = currentToken._value.strVal;
      if (Values.find(ident) != Values.end()) {
         str = Values[ident].toString();
      }
      else {
         str = ident;
      }

      args.emplace_back(std::move(str));
      advance();

      if (!currentToken.is_punctuator(')')) {
         CDOT_DIAGNOSE("expected ')'");
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
      CDOT_DIAGNOSE("wrong number of arguments passed to macro " + macroName);
   }

   switch (macro.first) {
      case BuiltinMacro::TOLOWER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::tolower);

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(std::move(str));

         return tok;
      }
      case BuiltinMacro::TOUPPER: {
         auto &str = args.front().strVal;
         std::transform(str.begin(), str.end(), str.begin(), ::toupper);

         auto tok = currentToken;
         tok._type = T_IDENT;
         tok._value = Variant(std::move(str));

         return tok;
      }
      case BuiltinMacro::TOSTRING: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant(args.front().toString());

         return tok;
      }
      case BuiltinMacro::NUMFORMAT: {
         if (args.front().type != VariantType::INT
             || args[1].type != VariantType::INT) {
            CDOT_DIAGNOSE("_NumFormat expects integers as 1st and "
                                 "2nd arguments");
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
         tok._value = Variant(std::move(res));

         return tok;
      }
      case BuiltinMacro::FILE: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant(string(fileName));

         return tok;
      }
      case BuiltinMacro::LINE: {
         auto tok = currentToken;
         tok._type = T_LITERAL;
         tok._value = Variant(currentLine);

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
         CDOT_DIAGNOSE("unknown value " + name);
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