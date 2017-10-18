//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <set>
#include <llvm/IR/Type.h>
#include <regex>
#include "Variant/Type/Generic.h"


namespace cdot {
   class BuiltinType;
   class ObjectType;
   class GenericType;

   enum class BuiltinFn : unsigned int;

   struct Argument;

   enum class CompatibilityType {
      COMPATIBLE,
      FUNC_NOT_FOUND,
      NO_MATCHING_CALL,

      CONSTRAINT_FAILED
   };
}

class TypeCheckPass;
class Expression;
using std::pair;
using std::string;
using std::unordered_map;
using namespace cdot;

enum class AccessModifier : unsigned int {
   DEFAULT,
   PUBLIC,
   PRIVATE,
   PROTECTED
};

class Function;

namespace cdot {
   namespace cl {
      struct Method;
      struct ExtensionConstraint;
   }
}

struct CallCandidate {
   union {
      cl::Method* method;
      Function* func;
   };

   size_t incompatibleArg;
   const cl::ExtensionConstraint* failedConstraint = nullptr;
};

struct CallCompatability {
   CompatibilityType compatibility = CompatibilityType::FUNC_NOT_FOUND;
   bool isCompatible() {
      return compatibility == CompatibilityType::COMPATIBLE;
   }

   int castPenalty = 0;
   bool perfectMatch = false;

   std::vector<size_t> neededCasts;
   std::vector<pair<size_t, bool>> argOrder;

   std::vector<Argument> resolvedArgs;
   std::vector<GenericType*> generics;

   Function* func = nullptr;
   cl::Method* method = nullptr;

   size_t incompatibleArg;
   const cl::ExtensionConstraint* failedConstraint = nullptr;

   std::vector<CallCandidate> failedCandidates;
};

namespace util {
   extern std::unordered_map<string, int> op_precedence;

   std::vector<string> str_split(string, char);
   string str_trim(string);

   string args_to_string(std::vector<Argument>&);

   extern std::vector<string> stdLibImports;

   extern std::vector<string> assignmentOperators;
   string isAssignmentOperator(string&);

   extern std::vector<pair<string, string>> LlvmFunctionAttrs;

   template <class T>
   bool in_vector(std::vector<T>& vec, T el) {
      return std::find(vec.begin(), vec.end(), el) != vec.end();
   }

   string str_escape(string);

   template<class T, class R>
   bool in_pair_vector(std::vector<std::pair<T, R>> vec, T el) {
      return std::find_if(vec.begin(), vec.end(), [el](const std::pair<T, R> pair) {
         return pair.first == el;
      }) != vec.end();
   };

   template<class T, class R>
   R get_second(std::vector<std::pair<T, R>> vec, T el) {
      auto pos = std::find_if(vec.begin(), vec.end(), [el](const std::pair<T, R> pair) {
         return pair.first == el;
      });

      return pos->second;
   };

   template <class T>
   std::unordered_map<string, T> merge_maps(std::unordered_map<string, T> m1,
       std::unordered_map<string, T> m2)
   {
      m1.insert(m2.begin(), m2.end());
      return m1;
   };

   bool matches(string pattern, string& subject);
   std::smatch get_match(string pattern, string& subject);

   string generate_getter_name(string);
   string generate_setter_name(string);

   extern std::vector<string> builtinFunctions;
   extern unordered_map<string, pair<cdot::BuiltinFn, std::vector<BuiltinType*>>> builtinTypes;

   extern string token_names[];
   extern std::vector<string> keywords;

   extern std::vector<string> types;
   extern std::unordered_map<AccessModifier, string> am_map;
   bool is_reversible(string);

   extern std::vector<string> binary_operators;
   extern std::vector<string> tertiary_operators;
   extern std::vector<string> PrefixUnaryOperators;
   extern std::vector<string> PostfixUnaryOperators;

   extern std::vector<char> operator_chars;
   extern std::vector<char> punctuators;

   extern std::vector<string> attributes;

   extern TypeCheckPass* TCPass;

   bool resolveGeneric(
      BuiltinType* given,
      BuiltinType* needed,
      std::vector<GenericType*>& givenGenerics,
      std::vector<GenericConstraint>& neededGenerics
   );

   int resolveGenerics(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,
      std::vector<GenericType*>& givenGenerics,
      std::vector<GenericConstraint>& neededGenerics
   );

   CallCompatability funcCallCompatible(
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &neededArgs
   );

   CallCompatability varargCallCompatible(
      std::vector<Argument> &givenArgs,
      std::vector<Argument> &neededArgs
   );

   CallCompatability findMatchingCall(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,

      std::vector<GenericType*> givenGenerics,
      std::vector<GenericConstraint> neededGenerics
   );

   CallCompatability findMatchingCall(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs,

      std::vector<GenericType*> givenGenerics,
      std::vector<GenericConstraint> neededGenerics,

      unordered_map<string, BuiltinType*> classGenerics
   );

   std::vector<pair<size_t, bool>> orderArgs(
      std::vector<Argument>& givenArgs,
      std::vector<Argument>& neededArgs
   );

   string nextAnonymousNamespace();
};


#endif //UTIL_H
