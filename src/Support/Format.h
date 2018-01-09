//
// Created by Jonas Zell on 18.11.17.
//

#ifndef CDOT_FORMAT_H
#define CDOT_FORMAT_H

#include <string>
#include <cmath>
#include <cassert>

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
   std::string res(FormatTraits::Prefix);
   if (val == 0) {
      res += std::to_string(0);
      return res;
   }

   auto neededDigits = uint64_t(std::ceil(std::log(val)
                                        / std::log(FormatTraits::Base)));
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

   return res;
}

template<class T>
std::string formatAsHexInteger(T val)
{
   union {
      T t;
      uint64_t i;
   } Union { val };

   return formatInteger<Base16Traits>(Union.i);
}

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

char hexdigit(unsigned i);

} // namespace support
} // namespace cdot

#endif //CDOT_FORMAT_H
