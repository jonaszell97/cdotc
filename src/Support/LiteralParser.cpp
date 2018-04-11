//
// Created by Jonas Zell on 13.02.18.
//

#include "LiteralParser.h"

namespace cdot {

LiteralParser::FPResult LiteralParser::parseFloating()
{
   llvm::APFloat APF(0.0);
   auto status = APF.convertFromString(Str, llvm::APFloat::rmNearestTiesToEven);

   return FPResult{ std::move(APF), status };
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

//static const char *BinaryDigits  = "01";
//static const char *OctalDigits   = "01234567";
//static const char *DecimalDigits = "0123456789";
//static const char *HexDigits     = "0123456789ABCDEF";

uint8_t getDigitValue(char c, uint8_t radix)
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
         return (uint8_t)(::toupper(c) - 'A');
      }
   default:
      llvm_unreachable("bad radix!");
   }
}

LiteralParser::IntResult LiteralParser::parseInteger(unsigned int bitwidth,
                                                     bool isSigned) {
   llvm::APInt API(bitwidth, 0);

   uint8_t radix = getIntegerRadix();
   size_t len = Str.size();

   assert(len && "empty literal string!");
   if (len == 1) {
      API += static_cast<uint64_t>(getDigitValue(Str.front(), radix));
      return IntResult{ llvm::APSInt(std::move(API), !isSigned), false };
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

   const char *Ptr = Str.begin();
   const char *End = Str.end();

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

   return IntResult{ llvm::APSInt(std::move(API), !isSigned), false };
}

} // namespace cdot