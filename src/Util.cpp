//
// Created by Jonas Zell on 16.06.17.
//

#include "Util.h"

#include "Variant/Type/Generic.h"
#include "Message/Diagnostics.h"

#include <sstream>
#include <string>
#include <vector>
#include <regex>

namespace cdot {
namespace util {

using namespace cdot::diag;
using std::ostringstream;

std::unordered_map<AccessModifier, string> am_map = {
   { AccessModifier::PUBLIC,    "public" },
   { AccessModifier::PRIVATE,   "private" },
   { AccessModifier::PROTECTED, "protected" }
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

string str_escape(const string &str)
{
   string res = "";
   for (char c : str) {
      switch (c) {
         case '\n':res += "\n";
            break;
         case '\a':res += "\a";
            break;
         case '\r':res += "\r";
            break;
         case '\v':res += "\v";
            break;
         case '\t':res += "\t";
            break;
         case '\b':res += "\b";
            break;
         default:res += c;
            break;
      }
   }

   return res;
}

bool is_reversible(const string &op)
{
   return op == "*" || op == "+" || op == "&" || op == "|" || op == "^"
          || op == "==" || op == "!=";
}

string args_to_string(std::vector<Argument> &args)
{
   string str = "(";
   for (int i = 0; i < args.size(); ++i) {
      str += args.at(i).type.toString();
      if (i < args.size() - 1) {
         str += ", ";
      }
   }

   return str + ")";
}

bool matches(const string &pattern, const string &subject)
{
   return std::regex_match(subject, std::regex(pattern));
}

std::vector<pair<string, string>> LlvmFunctionAttrs = {
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

namespace {

size_t i = 0;

} // anonymous namespace

string nextAnonymousNamespace()
{
   return "__anonymous_namespace" + std::to_string(i++);
}


string TemplateArgsToString(
   const std::vector<TemplateArg> &templateArgs, bool skipEmpty)
{
   if (skipEmpty && templateArgs.empty()) {
      return "";
   }

   ostringstream templateName;
   templateName << "<";
   auto numArgs = templateArgs.size();

   for (size_t i = 0; i < numArgs; ++i) {
      templateName << templateArgs[i].toString();
      if (i < numArgs - 1) {
         templateName << ", ";
      }
   }

   templateName << ">";
   return templateName.str();
}


void checkTemplateArgs(
   AstNode *cause,
   std::vector<TemplateConstraint> &neededArgs,
   std::vector<TemplateArg> &givenArgs)
{
   if (neededArgs.size() != givenArgs.size()) {
      diag::err(err_generic_type_count) << neededArgs.size()
                                        << givenArgs.size()
                                        << cause << diag::term;
   }
   return;
   size_t i = 0;
   for (const auto &needed : neededArgs) {
      auto &given = givenArgs[i];
   }
}

Type dummyResolver(Expression *node)
{
   llvm_unreachable("dummy resolver called");
}

} // namespace util
} // namespace cdot