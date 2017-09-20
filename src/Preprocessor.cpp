//
// Created by Jonas Zell on 16.09.17.
//

#include <iostream>
#include "Preprocessor.h"
#include "Message/Exceptions.h"
#include "Lexer.h"
#include "Util.h"

namespace cdot {

   namespace {
      enum class BuiltinMacro {
         TOLOWER,
         TOUPPER,
         REPEAT
      };

      unordered_map<string, pair<BuiltinMacro, int>> BuiltinMacros = {
         { "_ToLower", {BuiltinMacro::TOLOWER, 1} },
         { "_ToUpper", {BuiltinMacro::TOUPPER, 1} },
         { "_Repeat", {BuiltinMacro::REPEAT, 2} }
      };

      unordered_map<string, Variant> BuiltinValues = {
         { "_WordSize", Variant((long)(sizeof(size_t) * 8)) },
      };
   }

   unordered_map<string, pair<string, std::vector<MacroArg>>> Preprocessor::Macros;
   unordered_map<string, std::vector<string>> Preprocessor::Arrays;
   unordered_map<string, Variant> Preprocessor::Values;

   Preprocessor::Preprocessor(string &src, string &fileName)
      : src(src), srcLength(src.length()), lexer(new Lexer(src, fileName)), fileName(fileName)
   {
      lexer->advance();
   }

   Preprocessor::~Preprocessor()
   {
      for (const auto& def : ownDefs) {
         auto index = Values.find(def);
         if (index != Values.end()) {
            Values.erase(index);
         }
      }
   }

   void Preprocessor::run(bool dump)
   {
      for (;;) {
         while (lexer->current_token.is_punctuator('\n')) {
            advance();
         }

         auto dir = getNextDirective();
         runDirective(dir);
         if (dir == PPDirective::PD_NONE) {
            if (dump) {
               std::cout << src << "\n\n----------\n\n";
               std::cout << out << std::endl;
            }
            src = out;
            break;
         }
      }
   }

   void Preprocessor::runDirective(PPDirective directive)
   {
      switch (directive) {
         case PPDirective::PD_NONE:
            return;
         case PPDirective::PD_IFDEF:
            parseIfDef(); break;
         case PPDirective::PD_IFNDEF:
            parseIfNDef(); break;
         case PPDirective::PD_DEFINE:
            parseDefine(); break;
         case PPDirective::PD_RAWDEF:
            parseDefine(true); break;
         case PPDirective::PD_UNDEF:
            parseUndef(); break;
         case PPDirective::PD_IF:
            parseIf(); break;
         case PPDirective::PD_PRAGMA:
            parsePragma(); break;
         case PPDirective::PD_LET:
            parseLet(); break;
         case PPDirective::PD_FOR:
            parseFor(); break;
         case PPDirective::PD_PRINT:
            parsePrint(); break;
         default:
            ParseError::raise("Unknown pre-processor token", lexer);
      }
   }

   void Preprocessor::advance()
   {
      lexer->advance(false, true);
   }

   PPDirective Preprocessor::getNextDirective()
   {
      int start = lexer->last_token_index;
      int end = lexer->current_index;

      for (;;) {
         Token next = lexer->current_token;

         if (next.get_type() == T_DIRECTIVE) {
            auto directive = lexer->s_val();
            indent = lexer->current_token.getIndent();

            lexer->advance();

            if (directive == "define") {
               return PD_DEFINE;
            }
            if (directive == "rawdef") {
               return PD_RAWDEF;
            }
            if (directive == "enddef") {
               return PD_ENDDEF;
            }
            if (directive == "ifdef") {
               return PD_IFDEF;
            }
            if (directive == "ifndef") {
               return PD_IFNDEF;
            }
            if (directive == "undef") {
               return PD_UNDEF;
            }
            if (directive == "if") {
               return PD_IF;
            }
            if (directive == "endif") {
               return PD_ENDIF;
            }
            if (directive == "pragma") {
               return PD_PRAGMA;
            }
            if (directive == "for") {
               return PD_FOR;
            }
            if (directive == "let") {
               return PD_LET;
            }
            if (directive == "print") {
               return PD_PRINT;
            }

            return PD_INVALID;
         }
         else if (next.is_punctuator('\n')) {
            out += '\n';

            int i = 0;
            while (i < indent && lexer->lookahead().is_punctuator(' ')) {
               advance();
               ++i;
            }
         }
         else if (next.get_type() == T_PREPROC_VAR) {
            substituteValue();
         }
         else if (next.get_type() == T_EOF) {
            return PD_NONE;
         }
         else if (next.get_type() == T_IDENT && (Macros.find(lexer->s_val()) != Macros.end() ||
            BuiltinMacros.find(lexer->s_val()) != BuiltinMacros.end()))
         {
            substituteMacro();
         }
         else if (start != end) {
            out += src.substr(start, end - start);
         }

         start = lexer->current_token.getEnd();
         advance();
         end = lexer->current_token.getEnd();
      }

      return PD_NONE;
   }

   Variant Preprocessor::currentTokenValue()
   {
      switch (lexer->current_token.get_type()) {
         case T_LITERAL:
            return lexer->current_token.get_value();
         case T_OP: {
            auto op = lexer->s_val();
            lexer->advance();
            auto target = currentTokenValue();

            return target.applyUnaryOp(op);
         }
         case T_IDENT:
         case T_PREPROC_VAR: {
            auto ident = lexer->s_val();
            if (Macros.find(ident) != Macros.end()) {
               substituteMacro();
               return currentTokenValue();
            }
            if (BuiltinValues.find(ident) != BuiltinValues.end()) {
               return BuiltinValues[ident];
            }
            if (Values.find(ident) == Values.end()) {
               return Variant(ident);
            }

            return Values[ident];
         }
         case T_PUNCTUATOR: {
            if (lexer->current_token.is_punctuator('(')) {
               lexer->advance();
               auto expr = parseExpression();

               lexer->advance();
               if (!lexer->current_token.is_punctuator(')')) {
                  ParseError::raise("Expected ')'", lexer);
               }

               return expr;
            }
         }
         default:
            ParseError::raise("Unexpected token in preprocessor expression", lexer);
            llvm_unreachable(0);
      }
   }

   Variant Preprocessor::parseExpression(Variant lhs, int minPrecedence)
   {
      if (lhs.isVoid()) {
         lhs = currentTokenValue();
      }

      auto next = lexer->lookahead();
      while (next.get_type() == T_OP && util::op_precedence[next.get_value().strVal] >= minPrecedence) {
         lexer->advance();

         string op = lexer->s_val();
         lexer->advance();

         auto rhs = currentTokenValue();

         next = lexer->lookahead();
         while (next.get_type() == T_OP && util::op_precedence[next.get_value().strVal] > util::op_precedence[op]) {
            rhs = parseExpression(rhs, util::op_precedence[next.get_value().strVal]);
            next = lexer->lookahead();
         }

         lhs = lhs.applyBinaryOp(rhs, op);
      }

      return lhs;
   }

   void Preprocessor::substituteValue()
   {
      auto valName = lexer->s_val();
      if (Values.find(valName) == Values.end()) {
         ParseError::raise("Reference to undeclared value " + valName, lexer);
      }

      out += Values[valName].toString();
   }

   void Preprocessor::substituteBuiltinMacro()
   {
      auto name = lexer->s_val();
      auto& macro = BuiltinMacros[name];
      std::vector<Variant> args;

      if (lexer->lookahead().is_punctuator('(') && macro.second != 0) {
         lexer->advance();
         lexer->advance();

         while (!lexer->current_token.is_punctuator(')')) {
            args.push_back(parseExpression());
            lexer->advance();

            if (lexer->current_token.is_punctuator(',')) {
               lexer->advance();
            }
         }
      }

      if (args.size() != macro.second) {
         ParseError::raise("Unexpected argument count for macro " + name, lexer);
      }

      string res;

      switch (macro.first) {
         case BuiltinMacro::TOLOWER: {
            auto target = args[0].toString();
            std::transform(target.begin(), target.end(), target.begin(), ::tolower);
            res += target;
            break;
         }
         case BuiltinMacro::TOUPPER: {
            auto target = args[0].toString();
            std::transform(target.begin(), target.end(), target.begin(), ::toupper);
            res += target;
            break;
         }
         case BuiltinMacro::REPEAT: {
            if (args[0].type != VariantType::STRING || args[1].type != VariantType::INT) {
               ParseError::raise("Unexpected argument types for builtin macro _Repeat", lexer);
            }

            string s = args[0].strVal;
            long length = args[1].intVal;

            string rep;
            for (int i = 0; i < length; ++i) {
               rep += s;
            }

            res += rep;
            break;
         }
      }

      Preprocessor(res, fileName).run();
      out += res;
   }

   void Preprocessor::substituteMacro()
   {
      size_t indent = 0;
      auto index = out.length() - 1;

      while (out[index] == ' ') {
         ++indent;
         --index;
      }

      auto macroName = lexer->s_val();
      std::vector<string> args;

      if (BuiltinMacros.find(macroName) != BuiltinMacros.end()) {
         return substituteBuiltinMacro();
      }

      assert(Macros.find(macroName) != Macros.end() && "Function shouldn't be called in this case");

      auto& macro = Macros[macroName];
      if (lexer->lookahead().is_punctuator('(') && !macro.second.empty()) {
         lexer->advance();
         lexer->advance();

         while (!lexer->current_token.is_punctuator(')')) {
            args.push_back(parseExpression().toString());
            lexer->advance();

            if (lexer->current_token.is_punctuator(',')) {
               lexer->advance();
            }
         }
      }

      if (args.size() != macro.second.size()) {
         ParseError::raise("Unexpected argument count for macro " + macroName, lexer);
      }

      if (args.empty()) {
         out += macro.first;
         return;
      }

      size_t i = 0;
      int offset = 0;
      string replacement = macro.first;

      for (const auto & arg : macro.second) {
         for (const auto& occur : arg.occurrences) {
            replacement = replacement.substr(0, occur.first + offset)
               + args[i] + replacement.substr(occur.second + offset);
            offset += args[i].length() - arg.argName.length();
         }

         ++i;
      }

      if (indent > 0) {
         string indentStr = "\n";
         indentStr.reserve(indent);
         for (int i = 0; i < indent; ++i) {
            indentStr += ' ';
         }

         size_t start = 0;
         auto pos = replacement.find('\n');
         while (pos != string::npos) {
            replacement.replace(pos, 1, indentStr);
            start = pos + 1;
            pos = replacement.find('\n', start);
         }
      }

      Preprocessor(replacement, fileName).run();
      out += replacement;
   }

   void Preprocessor::parseDefine(bool rawDef)
   {
      string name = lexer->s_val();
      std::vector<MacroArg> args;

      if (lexer->lookahead(false, true).is_punctuator('\n')) {
         advance();
         Macros.emplace(name, pair<string, std::vector<MacroArg>>{ "", args });
         return;
      }

      advance();

      if (lexer->current_token.is_punctuator('(')) {
         lexer->advance();

         while (!lexer->current_token.is_punctuator(')')) {
            args.push_back(MacroArg{ lexer->s_val() });
            lexer->advance();
            if (lexer->current_token.is_punctuator(',')) {
               lexer->advance();
            }
         }

         lexer->advance();
      }

      int start = lexer->last_token_index;
      bool enddefFound = false;

      while (rawDef && lexer->current_token.is_punctuator('\n')) {
         advance();
      }

      size_t innerDefines = 0;

      while (!(lexer->current_token.is_punctuator('\n') && !lexer->current_token.isEscaped()) &&
         lexer->current_token.get_type() != T_EOF)
      {
         if (lexer->current_token.get_type() == T_IDENT) {
            string name = lexer->s_val();
            auto index = std::find_if(args.begin(), args.end(), [name](const MacroArg &arg) {
               return arg.argName == name;
            });

            if (index != args.end()) {
               index->occurrences.emplace_back(lexer->current_token.getStart() - start,
                  lexer->current_token.getEnd() - start);
            }
         }
         if (rawDef && lexer->current_token.get_type() == T_DIRECTIVE) {
            if (lexer->s_val() == "rawdef") {
               ++innerDefines;
            }
            if (lexer->s_val() == "enddef") {
               if (innerDefines-- == 0) {
                  enddefFound = true;
               }
               break;
            }
         }

         advance();

         while (rawDef && lexer->current_token.is_punctuator('\n')) {
            advance();
         }
      }

      string macro = src.substr(start, lexer->last_token_index - start);
      if (rawDef && macro.back() == '\n') {
         macro.pop_back();
      }

      Macros.emplace(name, pair<string, std::vector<MacroArg>>{ macro, args});

      if (rawDef) {
         if (!enddefFound) {
            ParseError::raise("Expected #enddef", lexer);
         }
         advance();
      }
   }

   void Preprocessor::parseIfDef(bool reverseCondition)
   {
      string name = lexer->s_val();
      advance();

      bool hasElse = false;
      int ifStartIndex = lexer->current_index;
      int ifEndIndex = -1;

      int elseStartIndex;
      int elseEndIndex;

      for (;;) {
         auto next = lexer->current_token;
         if (next.get_type() == T_DIRECTIVE) {
            auto dir = lexer->s_val();
            if (dir == "else") {
               hasElse = true;
               elseStartIndex = lexer->current_index;
               ifEndIndex = lexer->last_token_index;

               advance();
               continue;
            }
            else if (dir == "endif") {
               if (ifEndIndex == -1) {
                  ifEndIndex = lexer->last_token_index;
               }
               else {
                  elseEndIndex = lexer->last_token_index;
               }

               advance();
               break;
            }
         }
         else if (next.get_type() == T_EOF) {
            ParseError::raise("Expected #else or #endif", lexer);
         }

         advance();
      }

      auto isDef = Macros.find(name) != Macros.end();
      isDef = reverseCondition ? !isDef : isDef;

      if (isDef) {
         auto sub = src.substr(ifStartIndex, ifEndIndex - ifStartIndex);
         Preprocessor(sub, fileName).run();
         out += sub;
      }
      else if (hasElse) {
         auto sub = src.substr(elseStartIndex, elseEndIndex - elseStartIndex);
         Preprocessor(sub, fileName).run();
         out += sub;
      }
   }

   void Preprocessor::parseIfNDef()
   {
      parseIfDef(true);
   }

   void Preprocessor::parseIf()
   {
      auto cond = parseExpression();
      auto condIsTrue = false;
      switch (cond.type) {
         case VariantType::INT:
            condIsTrue = cond.intVal != 0; break;
         case VariantType::FLOAT:
            condIsTrue = cond.floatVal != 0.0; break;
         case VariantType::STRING:
            condIsTrue = cond.strVal != ""; break;
         default:
            break;
      }

      bool hasElse = false;
      bool hasElseIf = false;
      int ifStartIndex = lexer->last_token_index;
      int ifEndIndex = -1;

      int elseStartIndex;
      int elseEndIndex;

      int innerIfs = 0;

      for (;;) {
         auto next = lexer->current_token;
         if (next.get_type() == T_DIRECTIVE) {
            auto dir = lexer->s_val();

            if (dir == "if") {
               ++innerIfs;
            }
            else if (dir == "elseif") {
               if (innerIfs == 0) {
                  hasElseIf = true;
                  if (!condIsTrue) {
                     advance();
                     parseIf();

                     break;
                  }
                  else {
                     ifEndIndex = lexer->last_token_index;
                     for (;;) {
                        if (lexer->current_token.get_type() == T_DIRECTIVE && lexer->s_val() == "endif"
                           && innerIfs-- == 0)
                        {
                           lexer->advance();
                           break;
                        }

                        lexer->advance();
                     }

                     break;
                  }
               }
            }
            else if (dir == "else") {
               if (innerIfs == 0) {
                  hasElse = true;

                  elseStartIndex = lexer->current_index;
                  ifEndIndex = lexer->last_token_index;
               }
            }
            else if (dir == "endif") {
               if (innerIfs-- == 0) {
                  if (ifEndIndex == -1) {
                     ifEndIndex = lexer->last_token_index;
                  }
                  else {
                     elseEndIndex = lexer->last_token_index;
                  }

                  advance();
                  break;
               }
            }
         }
         else if (next.get_type() == T_EOF) {
            ParseError::raise("Expected #else or #endif", lexer);
         }

         advance();
      }

      if (hasElseIf && !condIsTrue) {
         return;
      }
      if (condIsTrue) {
         auto sub = src.substr(ifStartIndex, ifEndIndex - ifStartIndex);
         Preprocessor(sub, fileName).run();
         out += sub;
      }
      else if (hasElse) {
         auto sub = src.substr(elseStartIndex, elseEndIndex - elseStartIndex);
         Preprocessor(sub, fileName).run();
         out += sub;
      }
   }

   void Preprocessor::parsePragma()
   {

   }

   void Preprocessor::parseUndef()
   {
      string name = lexer->s_val();
      auto index = Macros.find(name);
      if (index != Macros.end()) {
         Macros.erase(index);
      }

      advance();
   }

   namespace {
      void tokenError(Lexer* lexer) {
         ParseError::raise("Unexpected token " +
                     util::token_names[lexer->current_token.get_type()], lexer);
      }
   }

   void Preprocessor::parsePrint()
   {
      auto var = currentTokenValue().toString();
      advance();

      std::cout << "\033[33m" << var << "\033[0m\n";
   }

   void Preprocessor::parseLet()
   {
      if (lexer->current_token.get_type() != T_IDENT) {
         tokenError(lexer);
      }

      auto name = lexer->s_val();
      lexer->advance();

      if (Values.find(name) == Values.end()) {
         ownDefs.push_back(name);
      }

      if (!lexer->current_token.is_operator("=")) {
         ParseError::raise("Expected '='", lexer);
      }

      lexer->advance();

      std::vector<string> elements;
      if (lexer->current_token.is_punctuator('[')) {
         while (!lexer->current_token.is_punctuator(']')) {
            lexer->advance();

            int start = lexer->current_token.getStart();
            int end;

            for (;;) {
               if (lexer->current_token.is_punctuator(',') || lexer->current_token.is_punctuator(']')) {
                  break;
               }

               lexer->advance();
            }

            end = lexer->last_token_index;
            elements.push_back(src.substr(start, end - start));
         }

         advance();

         if (Arrays.find(name) != Arrays.end()) {
            Arrays[name] = elements;
         }
         else {
            Arrays.emplace(name, elements);
         }
      }
      else {
         if (Values.find(name) != Values.end()) {
            Values[name] = parseExpression();
         }
         else {
            Values.emplace(name, parseExpression());
         }

         lexer->advance();
      }
   }

   void Preprocessor::parseFor()
   {
      if (lexer->current_token.get_type() != T_IDENT) {
         tokenError(lexer);
      }

      auto elName = lexer->s_val();
      lexer->advance();
      if (!lexer->current_token.is_keyword("in")) {
         tokenError(lexer);
      }

      lexer->advance();
      if (lexer->current_token.get_type() != T_IDENT) {
         tokenError(lexer);
      }

      auto arrName = lexer->s_val();
      if (Arrays.find(arrName) == Arrays.end()) {
         ParseError::raise(arrName + " was not defined", lexer);
      }

      string block;
      int start = lexer->current_index;
      bool done = false;
      bool endForFound = false;
      size_t nestedFors = 0;

      while (!done) {
         switch (lexer->current_token.get_type()) {
            case T_DIRECTIVE: {
               if (lexer->s_val() == "endfor") {
                  if (nestedFors-- == 0) {
                     done = true;
                     endForFound = true;
                     block = src.substr(start, lexer->last_token_index - start);
                  }
               }
               if (lexer->s_val() == "for") {
                  ++nestedFors;
               }

               break;
            }
            case T_EOF: {
               done = true;
               break;
            }
            default:
               break;
         }

         lexer->advance();
      }

      if (!endForFound) {
         ParseError::raise("Expected #endfor", lexer);
      }

      for (const auto& el : Arrays[arrName]) {
         string iteration(block);
         size_t startpos = 0;
         string search = "#{" + elName + "}";
         size_t pos = iteration.find(search);

         while (pos != string::npos) {
            iteration = iteration.replace(pos, search.length(), el);
            startpos = pos;
            pos = iteration.find(search, startpos);
         }

         Preprocessor(iteration, fileName).run();
         out += iteration;
      }
   }
}