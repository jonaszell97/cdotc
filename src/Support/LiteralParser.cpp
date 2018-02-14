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

LiteralParser::IntResult LiteralParser::parseInteger(unsigned int bitwidth,
                                                     bool isSigned) {
   uint8_t radix = getIntegerRadix();
   llvm::APInt API(bitwidth, Str, radix);
   llvm::APSInt APS(std::move(API), !isSigned);

   return IntResult{ std::move(APS), false };
}

} // namespace cdot