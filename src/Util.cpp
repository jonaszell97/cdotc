//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"

#include <string>
#include <vector>
#include <regex>

using std::string;

namespace cdot {
namespace util {

std::vector<string> attributes = {
   "suppress",
   "carray",
   "cstring",
   "boxed"
};

std::vector<string> str_split(const string &source, char delimiter)
{
   auto res = std::vector<string>();
   string s = "";
   for (int i = 0; i < source.length(); i++) {
      if (source[i] == delimiter) {
         res.push_back(s);
         s = "";
      }
      else {
         s += source[i];
      }
   }

   if (s != "") {
      res.push_back(s);
   }

   return res;
}

string generate_getter_name(const string &field_name)
{
   return (std::find(field_name.begin(), field_name.end(), '_')
           != field_name.end())
          ? "get_" + field_name
          : "get" + string(1, toupper(field_name[0]))
            + field_name.substr(1, field_name.length() - 1);
}

string generate_setter_name(const string &field_name)
{
   return (std::find(field_name.begin(), field_name.end(), '_')
           != field_name.end())
          ? "set_" + field_name
          : "set" + string(1, toupper(field_name[0]))
            + field_name.substr(1, field_name.length() - 1);
}

bool matches(const string &pattern, const string &subject)
{
   return std::regex_match(subject, std::regex(pattern));
}

std::vector<std::pair<string, string>> LlvmFunctionAttrs = {
   { "correctly-rounded-divide-sqrt-fp-math", "false" },
   { "disable-tail-calls",                    "false" },
   { "less-precise-fpmad",                    "false" },
   { "no-frame-pointer-elim",                 "false" },
   { "no-frame-pointer-elim-non-leaf",        "" },
   { "no-infs-fp-math",                       "false" },
   { "no-jump-tables",                        "false" },
   { "no-nans-fp-math",                       "false" },
   { "no-signed-zeros-fp-math",               "false" },
   { "stack-protector-buffer-size",           "8" },
   { "unsafe-fp-math",                        "false" },
   { "use-soft-float",                        "false" }
};

} // namespace util
} // namespace cdot