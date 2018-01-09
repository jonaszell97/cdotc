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
class GenericType;

enum class BuiltinFn : unsigned int;

struct Argument;
class QualType;

class TemplateArg;
struct TemplateParameter;

enum class AccessModifier : unsigned int {
   DEFAULT = 0,
   PUBLIC,
   PRIVATE,
   PROTECTED
};

}
using std::pair;
using std::string;
using std::unordered_map;
using namespace cdot;

namespace cdot {
namespace cl {

struct Method;
struct ExtensionConstraint;

} // namespace cl

namespace ast {

class AstNode;
class SemaPass;
class Expression;
class Function;
class StaticExpr;

} // namespace ast

} // namespace cdot

namespace cdot {
namespace util {

std::vector<string> str_split(const string &, char);

string args_to_string(std::vector<Argument> &);

extern std::vector<string> stdLibImports;

extern std::vector<pair<string, string>> LlvmFunctionAttrs;

template<class T>
bool in_vector(const std::vector<T> &vec, const T &el)
{
   return std::find(vec.begin(), vec.end(), el) != vec.end();
}

string str_escape(const string &);

template<class T, class R>
bool in_pair_vector(const std::vector<std::pair<T, R>> &vec, const T &el)
{
   return
      std::find_if(vec.begin(), vec.end(), [el](const std::pair<T, R> pair) {
         return pair.first == el;
      }) != vec.end();
};

template<class T, class R>
R get_second(const std::vector<std::pair<T, R>> &vec, T el)
{
   auto pos = std::find_if(vec.begin(), vec.end(),
                           [el](const std::pair<T, R> pair) {
                              return pair.first == el;
                           });

   return pos->second;
};

template<typename T, char Begin = '(', char Sep = ',', char End = ')',
   bool withSpace = true>
string vectorToString(const std::vector<T> &vec, bool withLength = false)
{
   string s;
   if (Begin != '\0') {
      s += Begin;
   }

   size_t numItems = vec.size();
   size_t i = 0;

   for (const auto &item : vec) {
      auto str = item.toString();
      if (withLength) {
         s += std::to_string(str.length());
      }
      s += str;

      if (i < numItems - 1 && Sep != '\0') {
         s += Sep;
         if (withSpace)
            s += ' ';
      }

      ++i;
   }

   if (End != '\0') {
      s += End;
   }

   return s;
};

template<typename T, char Begin = '(', char Sep = ',', char End = ')'>
string vectorToString(const std::vector<std::shared_ptr<T>> &vec,
                      bool withLength = false) {
   string s;
   if (Begin != '\0') {
      s += Begin;
   }

   size_t numItems = vec.size();
   size_t i = 0;

   for (const auto &item : vec) {
      auto str = item->toString();
      if (withLength) {
         s += std::to_string(str.length());
      }
      s += str;

      if (i < numItems - 1 && Sep != '\0') {
         s += Sep;
      }

      ++i;
   }

   if (End != '\0') {
      s += End;
   }

   return s;
};

bool matches(const string &pattern, const string &subject);

std::smatch get_match(string pattern, string &subject);

string generate_getter_name(const string &);

string generate_setter_name(const string &);

extern std::vector<string> types;
extern std::unordered_map<AccessModifier, string> am_map;

bool is_reversible(const string &);

extern std::vector<string> attributes;

typedef std::function<QualType(ast::Expression *)> ResolverFn;

QualType dummyResolver(ast::Expression *node);


size_t castPenalty(const QualType &from, const QualType &to);

string nextAnonymousNamespace();

} // namespace util
} // namespace cdot


#endif //UTIL_H
