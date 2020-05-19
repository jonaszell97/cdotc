#include "cdotc/Support/LiteralParser.h"

#include "cdotc/Support/Format.h"

#include <llvm/Support/Error.h>

using namespace cdot;

LiteralParser::FPResult LiteralParser::parseFloating()
{
   llvm::APFloat APF(0.0);
   auto statusOrErr = APF.convertFromString(Str, llvm::APFloat::rmNearestTiesToEven);

   return FPResult{std::move(APF),
       !statusOrErr ? llvm::APFloat::opInexact : statusOrErr.get()};
}

uint8_t LiteralParser::getIntegerRadix()
{
   if (Str[0] != '0')
      return 10;

   if (Str.size() == 1)
      return 8;

   switch (Str[1]) {
   case 'x':
   case 'X':
      Str = Str.drop_front(2);
      return 16;
   case 'b':
   case 'B':
      Str = Str.drop_front(2);
      return 2;
   default:
      break;
   }

   Str = Str.drop_front(1);
   return 8;
}

static uint8_t getDigitValue(char c, uint8_t radix)
{
   switch (radix) {
   case 2:
      return (uint8_t)(c == '1');
   case 8:
   case 10:
      return (uint8_t)(c - '0');
   case 16:
      if (c <= '9' && c >= '0') {
         return (uint8_t)(c - '0');
      }
      else {
         return (uint8_t)(::toupper(c) - 'A') + uint8_t(10);
      }
   default:
      llvm_unreachable("bad radix!");
   }
}

LiteralParser::IntResult LiteralParser::parseInteger(unsigned int bitwidth,
                                                     bool isSigned)
{
   llvm::APInt API(bitwidth, 0);

   uint8_t radix = getIntegerRadix();
   size_t len = Str.size();

   assert(len && "empty literal string!");
   if (len == 1) {
      API += static_cast<uint64_t>(getDigitValue(Str.front(), radix));
      return IntResult{llvm::APSInt(std::move(API), !isSigned), false};
   }

   unsigned shift;
   switch (radix) {
   case 2:
      shift = 1;
      break;
   case 8:
      shift = 3;
      break;
   case 10:
      shift = 0;
      break;
   case 16:
      shift = 4;
      break;
   default:
      llvm_unreachable("bad radix!");
   }

   const char* Ptr = Str.begin();
   const char* End = Str.end();

   for (; Ptr != End; ++Ptr) {
      if (*Ptr == '_') {
         continue;
      }

      uint8_t Val = getDigitValue(*Ptr, radix);
      if (shift) {
         API <<= shift;
      }
      else {
         API *= radix;
      }

      API += Val;
   }

   return IntResult{llvm::APSInt(std::move(API), !isSigned), false};
}

LiteralParser::CharResult LiteralParser::parseCharacter()
{
   if (Str.size() == 1) {
      return {static_cast<uint32_t>(Str.front()), false};
   }

   assert(Str.front() == '\\' && "malformed character literal");

   uint32_t Val = 0;

   // Hex literal.
   if (Str[1] == 'x') {
      assert(Str.size() == 4 && "malformed hex character literal!");
      Val += getDigitValue(Str[2], 16) * 16;
      Val += getDigitValue(Str[3], 16);
   }
   else if (Str[1] == 'u') {
      llvm_unreachable("TODO!");
   }
   else {
      assert(Str.size() == 2 && "malformed hex character literal!");
      Val = static_cast<uint32_t>(support::escape_char(Str[1]));
   }

   return {Val, false};
}

LiteralParser::StringResult LiteralParser::parseString()
{
   std::string str;
   str.reserve(Str.size());

   bool escaped = false;

   const char* ptr = Str.data();
   unsigned Len = (unsigned)Str.size();

   for (unsigned i = 0; i < Len; ++ptr, ++i) {
      char c = *ptr;

      if (escaped) {
         str += support::escape_char(c);
         escaped = false;
      }
      else if (c == '\\') {
         escaped = true;
      }
      else if (c == '$' && ptr[1] == '$') {
         // ignore this '$', append the next one
      }
      else {
         str += c;
      }
   }

   return {move(str), false};
}