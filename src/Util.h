//
// Created by Jonas Zell on 16.06.17.
//

#ifndef UTIL_H
#define UTIL_H

#include <unordered_map>
#include <regex>

namespace cdot {

class Type;
class ObjectType;
class GenericType;

enum class BuiltinFn : unsigned char;

class QualType;



}

using namespace cdot;

namespace cdot {
namespace ast {

class AstNode;
class SemaPass;
class Expression;
class StaticExpr;

} // namespace ast

} // namespace cdot

namespace cdot {
namespace util {

std::vector<std::string> str_split(const std::string &, char);

extern std::vector<std::string> stdLibImports;
extern std::vector<std::pair<std::string, std::string>> LlvmFunctionAttrs;

template<class T>
bool in_vector(const std::vector<T> &vec, const T &el)
{
   return std::find(vec.begin(), vec.end(), el) != vec.end();
}

std::string str_escape(const std::string &);

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
std::string vectorToString(const std::vector<T> &vec, bool withLength = false)
{
   std::string s;
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
std::string vectorToString(const std::vector<T* > &vec,
                           bool withLength = false) {
   std::string s;
   if (Begin != '\0') {
      s += Begin;
   }

   size_t numItems = vec.size();
   size_t i = 0;

   for (const auto &item : vec) {
      auto str = item->tostd::string();
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

bool matches(const std::string &pattern, const std::string &subject);

std::string generate_getter_name(const std::string &);
std::string generate_setter_name(const std::string &);

extern std::vector<std::string> attributes;

} // namespace util
} // namespace cdot


#endif //UTIL_H
