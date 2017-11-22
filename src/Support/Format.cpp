//
// Created by Jonas Zell on 18.11.17.
//

#include "Format.h"

namespace cdot {
namespace support {

const char *Base2Traits::Prefix = "0b";
const uint64_t Base2Traits::Base = 2;
const char Base2Traits::Digits[] = {
   '0', '1'
};

const char *Base8Traits::Prefix = "0";
const uint64_t Base8Traits::Base = 8;
const char Base8Traits::Digits[] = {
   '0', '1', '2', '3', '4', '5', '6', '7'
};

const char *Base16Traits::Prefix = "0x";
const uint64_t Base16Traits::Base = 16;
const char Base16Traits::Digits[] = {
   '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
   'A', 'B', 'C', 'D', 'E', 'F'
};

uint64_t intPower(uint64_t base, uint64_t exp)
{
   uint64_t result = 1;
   while (exp)
   {
      if (exp & 1)
         result *= base;
      exp >>= 1;
      base *= base;
   }

   return result;
}

char hexdigit(unsigned i)
{
   return Base16Traits::Digits[i];
}

} // namespace support
} // namespace cdot