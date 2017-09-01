//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <set>
#include <llvm/IR/Type.h>
#include <regex>

namespace cdot {
   class Type;
   class ObjectType;

   enum class CompatibilityType {
      COMPATIBLE,
      FUNC_NOT_FOUND,
      NO_MATCHING_CALL
   };
}

class TypeCheckVisitor;
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

struct CallCompatability {
   bool is_compatible = false;
   int compat_score;
   bool perfect_match = false;

   unordered_map<size_t, pair<Type*, Type*>> needed_casts;

   size_t incomp_arg = 0;
   string expectedType;
   string foundType;
};

namespace util {
   extern std::unordered_map<string, int> op_precedence;

   std::vector<string> str_split(string, char);
   string str_trim(string);

   string args_to_string(std::vector<Type*>&);

   extern std::vector<string> stdLibImports;

   extern std::vector<string> assignmentOperators;
   string isAssignmentOperator(string&);

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

   extern string token_names[];
   extern std::vector<string> keywords;

   extern std::vector<string> types;
   extern std::unordered_map<AccessModifier, string> am_map;
   bool is_reversible(string);

   extern std::vector<string> binary_operators;
   extern std::vector<string> tertiary_operators;
   extern std::vector<string> PrefixUnaryOperators;
   extern std::vector<string> PostfixUnaryOperators;

   extern std::set<string> string_modifiers;

   extern std::vector<char> operator_chars;
   extern std::vector<char> punctuators;

   extern std::vector<string> attributes;

   bool resolve_generic(Type* given, Type* needed,
      std::vector<Type*>& given_generics, std::vector<ObjectType*>& needed_generics, unordered_map<size_t,
      pair<Type*, Type*>>& needed_casts, size_t argNum);

   CallCompatability func_call_compatible(std::vector<Type*>& given_args, std::vector<Type*>& needed_args,
      std::vector<Type*>& given_generics, std::vector<ObjectType*>& needed_generics);

   CallCompatability func_call_compatible(std::vector<Type*>& given_args, std::vector<Type*>& needed_args);

   int func_score(std::vector<Type*>&);

   std::vector<pair<string, std::shared_ptr<Expression>>> orderArgs(
      std::vector<string>& givenLabels,
      std::vector<Type*>&givenArgs,
      std::vector<string>&  declaredArgs,
      std::vector<pair<string, std::shared_ptr<Expression>>>& argValues,
      std::vector<std::shared_ptr<Expression>>& defaultValues
   );
};


#endif //UTIL_H
