//
// Created by Jonas Zell on 18.11.17.
//

#ifndef CDOT_FORMAT_H
#define CDOT_FORMAT_H

#include <string>
#include <cmath>
#include <cassert>
#include <llvm/ADT/SmallString.h>
#include <ctime>

namespace cdot {
namespace support {

uint64_t intPower(uint64_t base, uint64_t exp);

struct Base2Traits {
   static const char Digits[];
   static const char* Prefix;
   static const uint64_t Base;
};

struct Base8Traits {
   static const char Digits[];
   static const char* Prefix;
   static const uint64_t Base;
};

struct Base16Traits {
   static const char Digits[];
   static const char* Prefix;
   static const uint64_t Base;
};

template<class FormatTraits = Base16Traits>
std::string formatInteger(uint64_t val)
{
   llvm::SmallString<128> res;
   res += FormatTraits::Prefix;

   if (val == 0) {
      res += std::to_string(0);
      return res.str().str();
   }

   auto neededDigits = uint64_t(std::ceil(std::log(val)
                                        / std::log(FormatTraits::Base)));

   if (neededDigits == 0) {
      res += FormatTraits::Digits[val];
      return res.str().str();
   }

   uint64_t power = neededDigits - 1;

   for (;;) {
      uint64_t pow = intPower(FormatTraits::Base, power);
      uint64_t fits = val / pow;
      assert(fits < FormatTraits::Base);

      res += FormatTraits::Digits[fits];

      if (!power) {
         break;
      }

      --power;
      val -= fits * pow;
   }

   return res.str().str();
}

template<class T>
std::string formatAsHexInteger(T val)
{
   union {
      T t;
      uint64_t i;
   } Union;

   Union.i = 0; // zero out upper bits if the given type is smaller
   Union.t = val;

   return formatInteger<Base16Traits>(Union.i);
}

inline std::string formatTime(time_t time = -1,
                              const char *fmt = "%Y/%m/%d %H:%M:%S") {
   if (time == -1)
      time = std::time(nullptr);

   char buffer[40];
   struct tm* time_s = localtime(&time);

   auto len = strftime(buffer, sizeof(buffer), fmt, time_s);
   return std::string(buffer, len);
}

char hexdigit(unsigned i);

inline char unescape_char(char c)
{
   switch (c) {
      case '\n':
         return 'n';
      case '\a':
         return 'a';
      case '\r':
         return 'r';
      case '\v':
         return 'v';
      case '\t':
         return 't';
      case '\b':
         return 'b';
      case '\0':
         return '0';
      default:
         return c;
   }
}

template <class Stream>
inline Stream& unescape_char(char c, Stream &out)
{
   switch (c) {
      case '\n':
         return out << "\\n";
      case '\a':
         return out << "\\a";
      case '\r':
         return out << "\\r";
      case '\v':
         return out << "\\v";
      case '\t':
         return out << "\\t";
      case '\b':
         return out << "\\b";
      case '\0':
         return out << "\\0";
      default:
         if (!::isprint(c)) {
            return out << '\\'
                       << hexdigit((c & 0b11110000) >> 4)
                       << hexdigit(c & 0b1111);
         }
         else
            return out << c;
   }
}

template<unsigned N>
inline llvm::SmallString<N> &unescape_char(char c, llvm::SmallString<N> &out)
{
   switch (c) {
      case '\n':
         return out += "\\n";
      case '\a':
         return out += "\\a";
      case '\r':
         return out += "\\r";
      case '\v':
         return out += "\\v";
      case '\t':
         return out += "\\t";
      case '\b':
         return out += "\\b";
      case '\0':
         return out += "\\0";
      default:
         if (!::isprint(c)) {
            out += '\\';
            out += hexdigit((c & 0b11110000) >> 4);
            out += hexdigit(c & 0b1111);

            return out;
         }
         else
            return out += c;
   }
}

inline char escape_char(char c)
{
   switch (c) {
      case 'n':
         return '\n';
      case 'a':
         return '\a';
      case 'r':
         return '\r';
      case 'v':
         return '\v';
      case 't':
         return '\t';
      case 'b':
         return '\b';
      case '"':
         return '\"';
      case '\'':
         return '\'';
      case '0':
         return '\0';
      default:
         return c;
   }
}

} // namespace support
} // namespace cdot

#endif //CDOT_FORMAT_H
