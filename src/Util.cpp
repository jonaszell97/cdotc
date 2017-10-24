//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"
#include "AST/Passes/SemanticAnalysis/Record/Class.h"
#include "Variant/Type/PointerType.h"
#include "Variant/Type/ObjectType.h"
#include "Variant/Type/IntegerType.h"
#include "Variant/Type/FPType.h"
#include "Variant/Type/AutoType.h"
#include "Variant/Type/VoidType.h"
#include "Variant/Type/Generic.h"
#include "Variant/Type/GenericType.h"

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
      "Macro",

      "Extern",
      "Any",
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
      "Print",
      "Time",
      "Thread"
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
      "typeof",
      "stackalloc",
      "__builtin_memcpy",
      "__builtin_memset",
      "__builtin_bitcast",
      "__nullptr",
      "__builtin_isnull",
      "__builtin_unwrap_protocol"
   };

   unordered_map<string, pair<cdot::BuiltinFn, std::vector<BuiltinType *>>> builtinTypes = {
      {"sizeof",            {BuiltinFn::SIZEOF,  {ObjectType::get("Int")}}},
      {"alignof",           {BuiltinFn::ALIGNOF, {ObjectType::get("Int")}}},
      {"stackalloc",        {BuiltinFn::STACK_ALLOC, {IntegerType::get(8)->getPointerTo(), IntegerType::get()}}},
      {"__builtin_memcpy",  {BuiltinFn::MEMCPY,  {VoidType::get(), IntegerType::get(8)->getPointerTo(),
                                                    IntegerType::get(8)->getPointerTo(), IntegerType::get()}} },
      {"__builtin_memset",  {BuiltinFn::MEMSET,  { VoidType::get(), IntegerType::get(8)->getPointerTo(),
                                                    IntegerType::get(8), IntegerType::get()}} },
      {"__builtin_bitcast", {BuiltinFn::BITCAST, {}}},
      {"__nullptr",         {BuiltinFn::NULLPTR, {}}},
      {"__builtin_isnull",  {BuiltinFn::ISNULL, { ObjectType::get("Bool") }}},
      {"__builtin_unwrap_protocol",  {BuiltinFn::UNWRAP_PROTO, { IntegerType::get(8, true)->getPointerTo() }}},
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
         str += args.at(i).type.toString();
         if (i < args.size() - 1) {
            str += ", ";
         }
      }

      return str + ")";
   }

   size_t castPenalty(Type &from, Type &to)
   {
      size_t penalty = 0;
      switch (from->getTypeID()) {
         case TypeID::IntegerTypeID: {
            if (to->isIntegerTy()) {
               auto fromBW = from->getBitwidth();
               auto toBW = to->getBitwidth();
               auto fromUnsigned = from->isUnsigned();
               auto toUnsigned = to->isUnsigned();

               if (fromBW > toBW) {
                  ++penalty;
               }
               else if (fromUnsigned != toUnsigned) {
                  ++penalty;
               }
            }
            else if (to->isFPType()) {
               penalty += 2;
            }
            else if (to->isBoxedPrimitive() && to->unbox()->isIntegerTy()) {
               if (to->unbox() != *from) {
                  ++penalty;
               }
            }
            else {
               penalty += 3;
            }
            break;
         }
         case TypeID::FPTypeID: {
            if (to->isFPType()) {
               if (to->asFloatingTy()->getPrecision() < from->asFloatingTy()->getPrecision()) {
                  ++penalty;
               }
            }
            else if (to->isIntegerTy()) {
               penalty += 2;
            }
            else if (to->isBoxedPrimitive() && to->unbox()->isFloatTy()) {
               if (to->unbox() != *from) {
                  ++penalty;
               }
            }
            else {
               penalty += 3;
            }
            break;
         }
         case TypeID::PointerTypeID: {
            if (to->isPointerTy() || to->isRawFunctionTy()) {
               ++penalty;
            }
            else if (to->isIntegerTy()) {
               penalty += 2;
            }
            else {
               penalty += 3;
            }
            break;
         }
         case TypeID::ObjectTypeID: {
            auto fromObj = from->asObjTy();

            if (to->isObject()) {
               auto fromRec = from->getRecord()->getAs<Class>();
               auto toRec = to->getRecord()->getAs<Class>();

               if (fromRec->getName() == toRec->getName()) {
                  ++penalty;
               }
               // upcast
               else if (toRec->isBaseClassOf(fromRec->getName())) {
                  ++penalty;
               }
               // downcast
               else if (fromRec->isBaseClassOf(toRec->getName())) {
                  penalty += 3;
               }
               else if (fromRec->isProtocol() == toRec->isProtocol()) {
                  penalty += 2;
               }
               else {
                  penalty += 3;
               }
            }
            else if (from->isBoxedPrimitive() && to->isNumeric()) {
               if (from->unbox() == *to) {
                  ++penalty;
               }
               else {
                  penalty += 2;
               }
            }
            else {
               penalty += 3;
            }

            break;
         }
         case TypeID::TupleTypeID: {
            if (to->isTupleTy()) {
               penalty += 2;
            }
            else {
               penalty += 3;
            }

            break;
         }
         default:
            penalty += 3;
            break;
      }

      return penalty;
   }

   SemaPass* TCPass = nullptr;

   bool resolveGeneric(
      BuiltinType* given,
      BuiltinType* needed,
      std::vector<GenericType*>& givenGenerics,
      std::vector<GenericConstraint>& neededGenerics)
   {
      if (neededGenerics.empty()) {
         return givenGenerics.empty();
      }

      if (needed->isPointerTy()) {
         if (!given->isPointerTy()) {
            return false;
         }

         auto givenPointee = *given->asPointerTy()->getPointeeType();
         auto neededPointee = *needed->asPointerTy()->getPointeeType();

         if (!resolveGeneric(givenPointee, neededPointee, givenGenerics, neededGenerics)) {
            return false;
         }
      }

      if (needed->isObject()) {
         if (!given->isObject()) {
            return false;
         }

         auto& givenConcrete = given->getConcreteGenericTypes();
         auto& neededConcrete = needed->getConcreteGenericTypes();
         if (givenConcrete.size() != neededConcrete.size()) {
            return false;
         }

         size_t i = 0;
         for (const auto& gen : givenConcrete) {
            if (!resolveGeneric(gen, neededConcrete[i], givenGenerics, neededGenerics)) {
               return false;
            }

            ++i;
         }
      }

      if (!needed->isGeneric()) {
         return true;
      }

      size_t i = 0;
      for (const auto& gen : neededGenerics) {
         if (gen.genericTypeName == needed->asGenericTy()->getGenericClassName()) {
            break;
         }

         ++i;
      }

      assert(i < neededGenerics.size() && "Generic name mismatch");

      auto& neededGen = neededGenerics[i];
      if (givenGenerics.size() < neededGenerics.size()) {
         givenGenerics.push_back(
            GenericType::get(neededGen.genericTypeName, given)
         );
      }

      if (i >= givenGenerics.size()) {
         return false;
      }

      if (!cdot::GenericTypesCompatible(givenGenerics[i], neededGen)) {
         return false;
      }

      return true;
   }

   int resolveGenerics(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,
      std::vector<GenericType*>& givenGenerics,
      std::vector<GenericConstraint>& neededGenerics)
   {
      int i = 0;
      for (auto &needed : neededArgs) {
         if (i >= givenArgs.size()) {
            return -1;
         }

         if (!resolveGeneric(*givenArgs[i].type, *needed.type, givenGenerics, neededGenerics)) {
            return i;
         }

         ++i;
      }

      return -1;
   }

   CallCompatability funcCallCompatible(
      std::vector<Argument> &given_args,
      std::vector<Argument> &needed_args)
   {
      CallCompatability comp;
      comp.incompatibleArg = 0;

      size_t given_size = given_args.size();
      size_t needed_size = needed_args.size();
      size_t i = 0;
      bool perfect_match = true;

      if (given_size == 0 && needed_size == 0) {
         comp.compatibility = CompatibilityType::COMPATIBLE;
         comp.perfectMatch = true;

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
         auto& needed = neededArg;
         if (i < given_size && !given_args[i].type->isAutoTy()) {
            auto& givenArg = given_args.at(i);
            auto& given = givenArg.type;

            if (!given.implicitlyCastableTo(needed.type)) {
               comp.incompatibleArg = i;

               return comp;
            }
            else if (given != needed.type) {
               perfect_match = false;
               comp.neededCasts.push_back(i);
               comp.castPenalty += castPenalty(given, needed.type);
            }
         }
         else if (needed.defaultVal == nullptr) {
            comp.incompatibleArg = i;
            return comp;
         }

         ++i;
      }

      comp.compatibility = CompatibilityType::COMPATIBLE;
      comp.perfectMatch = perfect_match;

      return comp;
   }

   CallCompatability varargCallCompatible(
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &neededArgs)
   {
      CallCompatability comp;
      comp.incompatibleArg = 0;

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
      if (!novararg.isCompatible()) {
         return novararg;
      }

      bool cstyle = neededArgs.back().cstyleVararg;
      if (cstyle) {
         comp.compatibility = CompatibilityType::COMPATIBLE;
         return comp;
      }

      bool perfectMatch = novararg.perfectMatch;
      auto vaTy = neededArgs.back().type;
      for (size_t i = 0; i < givenArgs.size(); ++i) {
         auto& given = givenArgs[i];

         if (!given.type.implicitlyCastableTo(vaTy)) {
            comp.incompatibleArg = i;

            return comp;
         }
         else if (!vaTy->isAutoTy() && given.type != vaTy) {
            comp.neededCasts.push_back(i);
            comp.castPenalty += castPenalty(given.type, vaTy);
            perfectMatch = false;
         }
      }

      comp.compatibility = CompatibilityType::COMPATIBLE;
      comp.perfectMatch = perfectMatch;

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

      std::vector<GenericType*> givenGenerics,
      std::vector<GenericConstraint> neededGenerics)
   {
      assert(TCPass != nullptr && "No TypeCheck Visitor!");
      CallCompatability comp;
      comp.compatibility = CompatibilityType::NO_MATCHING_CALL;
      comp.incompatibleArg = 0;

      std::vector<Argument> resolvedGivenArgs;
      std::vector<Argument> resolvedNeededArgs;

      size_t i = 0;
      bool vararg = !neededArgs.empty() && neededArgs.back().isVararg;
      bool cstyleVararg = vararg && neededArgs.back().cstyleVararg;
      for (auto& arg : givenArgs) {
         if (neededArgs.size() < i) {
            resolvedGivenArgs.emplace_back(arg.label, arg.type, arg.defaultVal);
            continue;
         }

         auto& argVal = arg.defaultVal;
         Type needed;
         if (neededArgs.size() > i) {
            needed = neededArgs[i].type;
         }
         else if (!neededArgs.empty()) {
            needed = neededArgs.back().type;
         }

         if (needed->isAutoTy()) {
            if (cstyleVararg) {
               resolvedGivenArgs.emplace_back(arg.label, arg.type, argVal);
               ++i;

               continue;
            }
            if (neededArgs.empty()) {
               comp.incompatibleArg = i;
               return comp;
            }

            llvm_unreachable("No needed type?");
         }

         if (!needed->isAutoTy() && argVal != nullptr && argVal->needsContextualInformation()) {
            argVal->setContextualType(needed);
            resolvedGivenArgs.emplace_back(arg.label, argVal->accept(*TCPass), argVal);

            ++i;
            continue;
         }

         assert(!arg.type->isAutoTy() && "No argument type or value");
         if (needed->isAutoTy() && arg.type.isLvalue() && !needed.isLvalue()) {
            auto ty = arg.type;
            ty.isLvalue(false);

            resolvedGivenArgs.emplace_back(arg.label, ty, argVal);
         }
         else {
            resolvedGivenArgs.emplace_back(arg.label, arg.type, argVal);
         }

         ++i;
      }

      auto order = orderArgs(resolvedGivenArgs, neededArgs);
      auto genComp = resolveGenerics(resolvedGivenArgs, neededArgs, givenGenerics, neededGenerics);
      if (genComp != -1) {
         comp.incompatibleArg = genComp;
         return comp;
      }

      for (const auto& arg : neededArgs) {
         auto newArg = arg;
         if (newArg.type->isGeneric()) {
            resolveGenerics(newArg.type, givenGenerics);
         }

         resolvedNeededArgs.push_back(newArg);
      }

      auto res = funcCallCompatible(
         resolvedGivenArgs,
         resolvedNeededArgs
      );

      res.argOrder = order;
      res.generics = givenGenerics;

      if (res.isCompatible()) {
         res.resolvedArgs = resolvedGivenArgs;
      }

      return res;
   }

   CallCompatability findMatchingCall(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,

      std::vector<GenericType*> givenGenerics,
      std::vector<GenericConstraint> neededGenerics,

      unordered_map<string, BuiltinType*> classGenerics)
   {
      std::vector<Argument> classResolved;
      for (const auto& arg : neededArgs) {
         auto ty = arg.type;
         classResolved.emplace_back(arg.label, ty, arg.defaultVal);
      }

      return findMatchingCall(givenArgs, neededArgs, givenGenerics, neededGenerics);
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

   size_t i = 0;
   string nextAnonymousNamespace() {
      return "__anonymous_namespace" + std::to_string(i++);
   }
}