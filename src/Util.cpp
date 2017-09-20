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
#include "Variant/Type/VoidType.h"
#include <string>
#include <vector>
#include <regex>

namespace util {

   std::unordered_map<string, int> op_precedence{
      {"?",     -1},
      {":",     -1},
      {"=",     0},
      {"+=",    0},
      {"-=",    0},
      {"*=",    0},
      {"/=",    0},
      {"<<=",   0},
      {">>=",   0},
      {"^=",    0},
      {"&=",    0},
      {"|=",    0},
      {"??",    1},
      {"||",    1},
      {"&&",    2},
      {"|",     3},
      {"&",     4},
      {"^",     4},
      {"!=",    5},
      {"!==",   5},
      {"==",    5},
      {"===",   5},
      {"<=",    6},
      {">=",    6},
      {"<",     6},
      {">",     6},
      {"<<",    7},
      {">>",    7},
      {">>>",   7},
      {"+",     8},
      {"-",     8},
      {"*",     9},
      {"/",     9},
      {"%",     9},
      {"**",    10},
      {"as",    11},
      {"as!",   11},
      {"isa",   11},
      {"..",    12},
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

   string isAssignmentOperator(string &op) {
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
      "loop",
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

   std::unordered_map<AccessModifier, string> am_map = {
      {AccessModifier::PUBLIC,    "public"},
      {AccessModifier::PRIVATE,   "private"},
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
      "Protocol/Randomizable",
      "Integers",
      "Math",
      "Console",
      "Double",
      "Random",
      "Bool",
      "Char",
      "Array",
      "Dictionary",
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
      '`',
      '#'
   };

   std::vector<string> attributes = {
      "suppress",
      "carray",
      "cstring",
      "boxed"
   };

   std::vector<string> builtinFunctions = {
      "sizeof",
      "alignof",
      "__builtin_memcpy",
      "__builtin_memset",
      "__builtin_bitcast",
      "__nullptr"
   };

   unordered_map<string, pair<cdot::BuiltinFn, std::vector<Type *>>> builtinTypes = {
      {"sizeof",            {BuiltinFn::SIZEOF,  {ObjectType::get("Int")}}},
      {"alignof",           {BuiltinFn::ALIGNOF, {ObjectType::get("Int")}}},
      {
       "__builtin_memcpy",  {
                             BuiltinFn::MEMCPY,  {
                                                  new VoidType, IntegerType::get(8)->getPointerTo(),
                                                    IntegerType::get(8)->getPointerTo(), IntegerType::get()}}},
      {
       "__builtin_memset",  {
                             BuiltinFn::MEMSET,  {
                                                  new VoidType, IntegerType::get(8)->getPointerTo(),
                                                    IntegerType::get(8),                 IntegerType::get()}}},
      {"__builtin_bitcast", {BuiltinFn::BITCAST, {}}},
      {"__nullptr",         {BuiltinFn::NULLPTR, {}}}
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
               res += "\n";
               break;
            case '\a':
               res += "\a";
               break;
            case '\r':
               res += "\r";
               break;
            case '\v':
               res += "\v";
               break;
            case '\t':
               res += "\t";
               break;
            case '\b':
               res += "\b";
               break;
            default:
               res += c;
               break;
         }
      }

      return res;
   }

   bool is_reversible(string op) {
      return op == "*" || op == "+" || op == "&" || op == "|" || op == "^" || op == "==" || op == "!=";
   }

   string args_to_string(std::vector<Argument> &args) {
      string str = "(";
      for (int i = 0; i < args.size(); ++i) {
         str += args.at(i).type->toString();
         if (i < args.size() - 1) {
            str += ", ";
         }
      }

      return str + ")";
   }

   bool resolveGeneric(
      Type* given,
      Type* needed,
      std::vector<Type*>& givenGenerics,
      std::vector<ObjectType*>& neededGenerics)
   {
      if (neededGenerics.empty()) {
         return givenGenerics.empty();
      }

      if (needed->isPointerTy()) {
         if (!given->isPointerTy()) {
            return false;
         }

         if (!resolveGeneric(given->getPointeeType(), needed->getPointeeType(), givenGenerics,
            neededGenerics))
         {
            return false;
         }
      }

      if (!needed->isObject() || !needed->isGeneric()) {
         return true;
      }

      if (givenGenerics.size() < neededGenerics.size()) {
         givenGenerics.push_back(given->deepCopy());
      }
      else {
         if (!Type::GenericTypesCompatible(given, needed)) {
            return false;
         }

         needed = givenGenerics.back();
      }

      if (!Type::GenericTypesCompatible(given, needed)) {
         return false;
      }

      return true;
   }

   int resolveGenerics(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,
      std::vector<Type*>& givenGenerics,
      std::vector<ObjectType*>& neededGenerics)
   {
      int i = 0;
      for (auto &needed : neededArgs) {
         if (i >= givenArgs.size()) {
            return -1;
         }

         if (!resolveGeneric(givenArgs[i].type, needed.type, givenGenerics, neededGenerics)) {
            return i;
         }

         ++i;
      }

      return -1;
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
   CallCompatability funcCallCompatible(
      std::vector<Argument> &given_args,
      std::vector<Argument> &needed_args)
   {
      CallCompatability comp;
      size_t given_size = given_args.size();
      size_t needed_size = needed_args.size();
      size_t i = 0;
      bool perfect_match = true;

      if (given_size == 0 && needed_size == 0) {
         comp.is_compatible = true;
         comp.perfect_match = true;

         return comp;
      }

      auto var_arg = needed_size > 0 && needed_args.back().isVararg;
      if (given_size > needed_size && !var_arg) {
         return comp;
      }

      if (var_arg) {
         return varargCallCompatible(given_args, needed_args);
      }

      for (auto &neededArg : needed_args) {
         auto& needed = neededArg.type;
         if (i < given_size && given_args[i].type != nullptr) {
            auto& givenArg = given_args.at(i);
            auto& given = givenArg.type;

            if (!given->implicitlyCastableTo(needed)) {
               comp.incomp_arg = i;
               comp.foundType = given->toString();
               comp.expectedType = needed->toString();
               return comp;
            }
            else if (*given != needed) {
               perfect_match = false;
               comp.needed_casts.push_back(i);
            }
         }
         else if (!needed->hasDefaultArgVal()) {
            comp.incomp_arg = i;
            return comp;
         }

         ++i;
      }

      comp.is_compatible = true;
      comp.perfect_match = perfect_match;

      return comp;
   }

   CallCompatability varargCallCompatible(
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &neededArgs)
   {
      CallCompatability comp;
      if (givenArgs.size() < neededArgs.size() - 1) {
         return comp;
      }

      // check arguments up until vararg
      std::vector<Argument> noVarargGiven;
      std::vector<Argument> noVarargNeeded(neededArgs);
      noVarargNeeded.pop_back();

      size_t varargIndex = noVarargNeeded.size();
      for (int i = 0; i < varargIndex; ++i) {
         noVarargGiven.push_back(givenArgs[i]);
      }

      auto novararg = funcCallCompatible(noVarargGiven, noVarargNeeded);
      if (!novararg.is_compatible) {
         return novararg;
      }

      bool cstyle = neededArgs.back().cstyleVararg;
      if (cstyle) {
         comp.is_compatible = true;
         return comp;
      }

      bool perfectMatch = novararg.perfect_match;
      auto vaTy = neededArgs.back().type;
      for (size_t i = 0; i < givenArgs.size(); ++i) {
         auto& given = givenArgs[i];

         if (!given.type->implicitlyCastableTo(vaTy)) {
            comp.incomp_arg = i;
            comp.foundType = given.type->toString();
            comp.expectedType = vaTy->toString();

            return comp;
         }
         else if (*given.type != vaTy) {
            comp.needed_casts.push_back(i);
            perfectMatch = false;
         }
      }

      comp.is_compatible = true;
      comp.perfect_match = perfectMatch;

      return comp;
   }

   std::vector<pair<size_t, bool>> orderArgs(
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &neededArgs)
   {
      size_t i = 0;
      std::vector<pair<size_t, bool>> order;
      std::vector<pair<string, std::shared_ptr<Expression>>> orderedArgs;
      orderedArgs.reserve(neededArgs.size());

      // if no labels are supplied, just return the arguments in order
      bool hasLabels = false;
      auto firstLabelIndex = std::find_if(givenArgs.begin(), givenArgs.end(), [](const Argument &given) {
         return !given.label.empty();
      });

      if (firstLabelIndex == givenArgs.end()) {
         hasLabels = false;
      }

      for (const auto& arg : neededArgs) {
         auto labelIndex = std::find_if(givenArgs.begin(), givenArgs.end(), [arg](const Argument &given) {
            return given.label == arg.label;
         });

         // argument with this label was supplied
         if (hasLabels && labelIndex != givenArgs.end()) {
            auto distance = std::distance(givenArgs.begin(),labelIndex);
            orderedArgs.emplace_back(arg.label, givenArgs[distance].defaultVal);

            order.emplace_back(distance, false);
         }
         // no arg with label found
         else if (hasLabels) {
            return order;
         }
         // argument exists at position
         else if (givenArgs.size() > i) {
            orderedArgs.emplace_back(arg.label, givenArgs[i].defaultVal);
            order.emplace_back(i, false);
         }
         // default arg exists
         else if (arg.defaultVal != nullptr) {
            orderedArgs.emplace_back(arg.label, arg.defaultVal);
            order.emplace_back(i, true);
         }
         // incompatible
         else {
            return order;
         }

         ++i;
      }

      while (order.size() < givenArgs.size()) {
         order.emplace_back(i++, false);
      }

      return order;
   }

   CallCompatability findMatchingCall(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,

      std::vector<Type*> givenGenerics,
      std::vector<ObjectType*> neededGenerics)
   {
      CallCompatability comp;
      std::vector<Argument> resolvedGivenArgs;
      std::vector<Argument> resolvedNeededArgs;

      size_t i = 0;
      bool vararg = !neededArgs.empty() && neededArgs.back().isVararg;
      bool cstyleVararg = vararg && neededArgs.back().cstyleVararg;
      for (const auto& arg : givenArgs) {
         if (neededArgs.size() < i) {
            resolvedGivenArgs.emplace_back(arg.label, arg.type->deepCopy(), arg.defaultVal);
            continue;
         }

         Type* needed = neededArgs.size() > i ? neededArgs[i].type : nullptr;
         auto& argVal = arg.defaultVal;

         if (argVal != nullptr && argVal->needsContextualInformation()) {
            if (needed == nullptr || !argVal->canReturn(needed)) {
               if (cstyleVararg) {
                  resolvedGivenArgs.emplace_back(arg.label, new AutoType, argVal);
                  continue;
               }
               else if (vararg) {
                  auto& arg = neededArgs.back();
                  resolvedGivenArgs.emplace_back(arg.label, arg.type->deepCopy(), arg.defaultVal);
                  continue;
               }

               comp.incomp_arg = i;
               return comp;
            }

            resolvedGivenArgs.emplace_back(arg.label, needed->deepCopy(), argVal);
         }
         else if (needed != nullptr && arg.type->isLvalue() && !needed->isLvalue()) {
            resolvedGivenArgs.emplace_back(arg.label, arg.type->deepCopy()->toRvalue(), argVal);
         }
         else {
            resolvedGivenArgs.emplace_back(arg.label, arg.type->deepCopy(), argVal);
         }

         ++i;
      }

      auto order = orderArgs(resolvedGivenArgs, neededArgs);
      auto genComp = resolveGenerics(resolvedGivenArgs, neededArgs, givenGenerics, neededGenerics);
      if (genComp != -1) {
         comp.incomp_arg = genComp;
         return comp;
      }

      for (const auto& arg : neededArgs) {
         auto newTy = arg.type->deepCopy();
         if (newTy->isObject()) {
            auto asObj = cast<ObjectType>(newTy);
            std::vector<Type*> unqual;
            for (const auto& ty : givenGenerics) {
               unqual.push_back(ty->deepCopy());
            }

            asObj->setUnqualGenerics(unqual);

            Type::resolveUnqualified(newTy);
            Type::resolveGeneric(&newTy, newTy->getConcreteGenericTypes());
         }

         resolvedNeededArgs.emplace_back(arg.label, newTy, arg.defaultVal);
      }

      auto res = funcCallCompatible(
         resolvedGivenArgs,
         resolvedNeededArgs
      );

      res.argOrder = order;
      res.generics = givenGenerics;

      for (const auto& arg : resolvedGivenArgs) {
         delete arg.type;
      }

      for (const auto& arg : resolvedNeededArgs) {
         delete arg.type;
      }

      return res;
   }

   CallCompatability findMatchingCall(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,

      std::vector<Type*> givenGenerics,
      std::vector<ObjectType*> neededGenerics,

      unordered_map<string, Type*> classGenerics)
   {
      std::vector<Argument> classResolved;
      for (const auto& arg : neededArgs) {
         auto ty = arg.type->deepCopy();

         Type::resolveGeneric(&ty, classGenerics);
         classResolved.emplace_back(arg.label, ty, arg.defaultVal);
      }

      auto res = findMatchingCall(givenArgs, neededArgs, givenGenerics, neededGenerics);

      for (const auto& arg : classResolved) {
         delete arg.type;
      }

      return res;
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

   const std::regex interpolationRegex(
      R"_((?:[^\\]|^)\$([^ \[\]\+\-\*/%&\|!=<>\.~\^,\(\)\{\}\$]+)|(?:[^\\]|^)\$\{([^\}]+)\})_"
   );

   std::vector<pair<size_t, string>> findInterpolations(string& str) {
      std::smatch match;
      std::vector<pair<size_t, string>> interpolations;

      size_t beginPos = 0;
      auto begin = str.cbegin();
      while (std::regex_search(begin, str.cend(), match, interpolationRegex)) {
         auto pos = beginPos + match.position();
         if (match[0].str().front() != '$') {
            ++pos;
         }

         interpolations.emplace_back(beginPos, str.substr(beginPos, pos - beginPos));

         if (!match[1].str().empty()) {
            interpolations.emplace_back(pos, match[1].str());
         }
         else {
            interpolations.emplace_back(pos, match[2].str());
         }

         beginPos += match.position() + match.length();
         begin += match.position() + match.length();
      }

      if (!interpolations.empty() && beginPos != str.length()) {
         interpolations.emplace_back(beginPos, str.substr(beginPos));
      }

      return interpolations;
   }

   size_t i = 0;
   string nextAnonymousNamespace() {
      return "__anonymous_namespace" + std::to_string(i++);
   }
}