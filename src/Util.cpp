//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include "AST/Visitor/StaticAnalysis/Class.h"
#include "Variant/Type/PointerType.h"
#include "Variant/Type/ObjectType.h"
#include "Variant/Type/IntegerType.h"
#include "Variant/Type/FPType.h"
#include "Variant/Type/AutoType.h"
#include <string>
#include <vector>
#include <regex>

namespace util {

   std::unordered_map<string, int> op_precedence {
      {"?", -1},
      {":", -1},
      {"=", 0},
      {"+=", 0},
      {"-=", 0},
      {"*=", 0},
      {"/=", 0},
      {"<<=", 0},
      {">>=", 0},
      {"^=", 0},
      {"&=", 0},
      {"|=", 0},
      {"??", 1},
      {"||", 1},
      {"&&", 2},
      {"|", 3},
      {"&", 4},
      {"^", 4},
      {"!=", 5},
      {"!==", 5},
      {"==", 5},
      {"===", 5},
      {"<=", 6},
      {">=", 6},
      {"<", 6},
      {">", 6},
      {"<<", 7},
      {">>", 7},
      {">>>", 7},
      {"+", 8},
      {"-", 8},
      {"*", 9},
      {"/", 9},
      {"%", 9},
      {"**", 10},
      {"as", 11},
      {"as!", 11},
      {"isa", 11},
      {"..", 12},
      {"infix", 13}
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

   string isAssignmentOperator(string& op) {
      if (op == "=") {
         return op;
      }

      if (in_vector(assignmentOperators, op)) {
         return op.substr(0, op.length() - 1);
      }

      return "";
   }

   std::vector<char> operator_chars = {
      '+', '-', '=', '<', '>', '&', '|', '%', '!', '*', '/', '~', '?', ':', '.', '^'
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
      "do",
      "match",
      "for",
      "in",
      "case",
      "default",
      "struct",
      "throw",
      "class",
      "public",
      "private",
      "protected",
      "static",
      "abstract",
      "protocol",
      "enum",
      "with",
      "namespace",
      "typeof",
      "continue",
      "break",
      "get",
      "set",
      "goto",
      "infix",
      "prefix",
      "postfix",
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

   std::unordered_map<AccessModifier, string> am_map = {
         {AccessModifier::PUBLIC, "public"},
         {AccessModifier::PRIVATE, "private"},
         {AccessModifier::PROTECTED, "protected"}
   };

   std::vector<string> types = {
         "UInt",
         "Int8",
         "UInt8",
         "Int16",
         "UInt16",
         "Int32",
         "UInt32",
         "Int64",
         "UInt64",

         "Float",
         "Object",
         "Void"
   };

   std::vector<string> stdLibImports = {
      "Extern",
      "Any",
      "Lambda",
      "Option",
      "Protocol/Equatable",
      "Protocol/Comparable",
      "Protocol/Ordered",
      "Protocol/Hashable",
      "Protocol/Number",
      "Protocol/IntegerProtocol",
      "Protocol/Iterable",
      "Protocol/Iterator",
      "Protocol/StringRepresentable",
      "Int",
      "Math",
//      "Float",
      "Double",
      "Bool",
      "Char",
      "Array",
      "ArrayIterator",
      "String",
      "Range",
      "Print"
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
      '`'
   };

   std::set<string> string_modifiers = {
         "f",
         "u",
         "e"
   };

   std::vector<string> attributes = {
      "suppress",
      "carray",
      "cstring",
      "boxed"
   };

   std::vector<string> str_split(string source, char delimiter) {
      auto res = std::vector<string>();
      string s = "";
      for (int i = 0; i < source.length(); i++) {
         if (source[i] == delimiter) {
            res.push_back(s);
            s = "";
         } else {
            s += source[i];
         }
      }

      if (s != "") {
         res.push_back(s);
      }

      return res;
   }

   string str_trim(string target) {
      while (target[0] == ' ' || target[0] == '\n' || target[0] == '\t') {
         target = target.substr(1);
      }
      while (target[target.length() - 1] == ' ' || target[target.length() - 1] == '\n' || target[target.length() -
            1] == '\t') {
         target = target.substr(0, target.length() - 1);
      }

      return target;
   }

   string generate_getter_name(string field_name) {
      return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
         ? "get_" + field_name
         : "get" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
   }

   string generate_setter_name(string field_name) {
      return (std::find(field_name.begin(), field_name.end(), '_') != field_name.end())
            ? "set_" + field_name
            : "set" + string(1, toupper(field_name[0])) + field_name.substr(1, field_name.length() - 1);
   }

   string str_escape(string str) {
      string res = "";
      for (char c : str) {
         switch (c) {
            case '\n':
               res += "\\n"; break;
            case '\a':
               res += "\\a"; break;
            case '\r':
               res += "\\r"; break;
            case '\v':
               res += "\\v"; break;
            case '\t':
               res += "\\t"; break;
            case '\b':
               res += "\\b"; break;
            default:
               res += c; break;
         }
      }

      return res;
   }

   bool is_reversible(string op) {
      return op == "*" || op == "+" || op == "&" || op == "|" || op == "^" || op == "==" || op == "!=";
   }

   string args_to_string(std::vector<Type*>&args) {
      string str = "(";
      for (int i = 0; i < args.size(); ++i) {
         str += args.at(i)->toString();
         if (i < args.size() - 1) {
            str += ", ";
         }
      }

      return str + ")";
   }

   bool resolve_generic(
      Type* given,
      Type* needed,
      std::vector<Type*>& given_generics,
      std::vector<ObjectType*>& needed_generics,
      unordered_map<size_t, pair<Type*, Type*>>& neededCasts,
      size_t argNum
   ) {
      if (needed_generics.empty()) {
         return given_generics.empty();
      }

      if (needed->isPointerTy()) {
         if (!given->isPointerTy()) {
            return false;
         }

         if (!resolve_generic(given->getPointeeType(), needed->getPointeeType(), given_generics,
            needed_generics, neededCasts, argNum))
         {
            return false;
         }
      }

      if (!needed->isObject() || !needed->isGeneric()) {
         return true;
      }

      if (given_generics.size() < needed_generics.size()) {
         given_generics.push_back(given->deepCopy());
      }
      else {
         if (!Type::GenericTypesCompatible(given, needed)) {
            return false;
         }

         needed = given_generics.back();
      }

      if (!Type::GenericTypesCompatible(given, needed)) {
         return false;
      }
      else if (*given != needed) {
         neededCasts.emplace(argNum, pair<Type*, Type*>{ given->deepCopy(), needed->deepCopy() });
      }

      return true;
   }

   /**
    * Returns:
    *  -1 if compatible
    *  -2 if incompatible argument count
    *  index of first incompatible arg otherwise
    * @param given
    * @param needed
    * @param given_generics
    * @param needed_generics
    * @return
    */
   CallCompatability func_call_compatible(
      std::vector<Type*>& given_args,
      std::vector<Type*>& needed_args,
      std::vector<Type*>& given_generics,
      std::vector<ObjectType*>& needed_generics)
   {
      CallCompatability comp;
      size_t given_size = given_args.size();
      size_t needed_size = needed_args.size();
      size_t i = 0;
      bool perfect_match = true;

      if (given_size == 0 && needed_size == 0 && given_generics.empty() && needed_generics.empty()) {
         comp.is_compatible = true;
         comp.compat_score = 0;
         comp.perfect_match = true;
         return comp;
      }

      auto var_arg = needed_size > 0 && needed_args.back()->isVararg();
      if (given_size > needed_size && !var_arg) {
         return comp;
      }

      if (var_arg) {
         goto var_arg;
      }

      for (auto& needed : needed_args) {
         if (i < given_size && given_args[i] != nullptr) {
            auto& given = given_args.at(i);

            if (!needed_generics.empty()) {
               if (!resolve_generic(given, needed, given_generics, needed_generics, comp.needed_casts, i)) {
                  comp.foundType = given->toString();
                  comp.expectedType = needed->toString();
                  comp.incomp_arg = i;
                  return comp;
               }
               if (*given != needed) {
                  comp.needed_casts.emplace(i, pair<Type*, Type*>{ given->deepCopy(), needed->deepCopy() });
               }
            }
            else if (!given->implicitlyCastableTo(needed)) {
               comp.incomp_arg = i;
               comp.foundType = given->toString();
               comp.expectedType = needed->toString();
               return comp;
            }
            else if (*given != needed) {
               perfect_match = false;
               comp.needed_casts.emplace(i, pair<Type*, Type*>{ given->deepCopy(), needed->deepCopy() });
            }
         }
         else if (!needed->hasDefaultArgVal()) {
            comp.incomp_arg = i;
            return comp;
         }

         ++i;
      }

      end:
      if (given_generics.size() != needed_generics.size()) {
         return comp;
      }

      comp.is_compatible = true;
      comp.compat_score = func_score(needed_args);
      comp.perfect_match = perfect_match;

      return comp;

      var_arg:
      auto& va_type = needed_args.back();
      for (auto& given : given_args) {
         auto& needed = i < needed_size - 1 ? needed_args.at(i) : va_type;
         // accepts all types
         if (needed->isCStyleVararg()) {
            goto end;
         }

         if (needed->isGeneric()) {
            if (!resolve_generic(given, needed, given_generics, needed_generics, comp.needed_casts, i)) {
               comp.incomp_arg = i;
               return comp;
            }
         }
         else if (!given->implicitlyCastableTo(needed)) {
            comp.incomp_arg = i;
            return comp;
         }
         else if (*given != needed) {
            perfect_match = false;
            comp.needed_casts.emplace(i, pair<Type*, Type*>{ given->deepCopy(), needed->deepCopy() });
         }

         ++i;
      }

      goto end;
   }

   CallCompatability func_call_compatible(
      std::vector<Type*>& given_args,
      std::vector<Type*>& needed_args)
   {
      std::vector<Type*> givenGenerics;
      std::vector<ObjectType*> neededGenerics;

      return func_call_compatible(given_args, needed_args, givenGenerics, neededGenerics);
   }

   std::vector<pair<string, std::shared_ptr<Expression>>> orderArgs(
      std::vector<string>& givenLabels,
      std::vector<Type*>& givenArgs,
      std::vector<string> &declaredArgs,
      std::vector<pair<string, std::shared_ptr<Expression>>>& argValues,
      std::vector<std::shared_ptr<Expression>>& defaultValues)
   {
      assert(givenLabels.size() == givenArgs.size() && "Should be checked before calling");
      std::vector<Type*> orderedTypes;
      std::vector<pair<string, std::shared_ptr<Expression>>> orderedArgs;
      size_t i = 0;

      for (const auto& arg : declaredArgs) {
         auto index = std::find(givenLabels.begin(), givenLabels.end(), arg);
         if (!arg.empty() && index != givenLabels.end()) {
            auto ind = (size_t)(index - givenLabels.begin());
            orderedTypes.push_back(givenArgs[ind]);
            orderedArgs.emplace_back(arg, util::get_second(argValues, arg));
         }
         else if (givenArgs.size() > i) {
            orderedTypes.push_back(givenArgs[i]);
            orderedArgs.emplace_back(arg, argValues[i].second);
         }
         else if (defaultValues[i] != nullptr) {
            orderedTypes.push_back(nullptr);
            orderedArgs.emplace_back(arg, defaultValues[i]);
         }
         else {
            orderedTypes.push_back(nullptr);
         }

         ++i;
      }

      for (; i < givenArgs.size(); ++i) {
         orderedTypes.push_back(givenArgs[i]);
         orderedArgs.emplace_back(givenLabels[i], argValues[i].second);
      }

      givenArgs = orderedTypes;
      return orderedArgs;
   }

   int func_score(std::vector<Type*>& args) {
      int score = 0;
      for (const auto& argument : args) {
         if (isa<ObjectType>(argument)) {
            auto asObj = cast<ObjectType>(argument);
            score += SymbolTable::getClass(asObj->getClassName())->getDepth();
         }
         else if (isa<IntegerType>(argument)) {
            auto asInt = cast<IntegerType>(argument);
            score += 8 - int(asInt->getBitwidth() / 8);
         }
         else if (isa<FPType>(argument)) {
            auto asFloat = cast<FPType>(argument);
            score += 3 - int(asFloat->getPrecision() / 32);
         }
         else {
            score += 1;
         }
      }

      return score;
   }

   bool matches(string pattern, string& subject) {
      return std::regex_match(subject, std::regex(pattern));
   }

   std::vector<pair<string, string>> LlvmFunctionAttrs = {
      { "correctly-rounded-divide-sqrt-fp-math", "false" },
      { "disable-tail-calls", "false" },
      { "less-precise-fpmad" , "false" },
      { "no-frame-pointer-elim", "false" },
      { "no-frame-pointer-elim-non-leaf", "" },
      { "no-infs-fp-math", "false" },
      { "no-jump-tables", "false" },
      { "no-nans-fp-math", "false" },
      { "no-signed-zeros-fp-math", "false" },
      { "stack-protector-buffer-size", "8" },
      { "unsafe-fp-math", "false" },
      { "use-soft-float", "false" }
   };
}