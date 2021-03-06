#ifndef CDOT_LITERALPARSER_H
#define CDOT_LITERALPARSER_H

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/StringRef.h>

namespace cdot {

class LiteralParser {
public:
   LiteralParser(llvm::StringRef Str) : Str(Str)
   {
      assert(!Str.empty() && "empty literal");
   }

   struct IntResult {
      llvm::APSInt APS;
      bool wasTruncated;
   };

   IntResult parseInteger(unsigned bitwidth = 64, bool isSigned = true);

   struct FPResult {
      llvm::APFloat APF;
      llvm::APFloat::opStatus status;
   };

   FPResult parseFloating();

   struct CharResult {
      uint32_t Char;
      bool Malformed;
   };

   CharResult parseCharacter();

   struct StringResult {
      std::string Str;
      bool Malformed;
   };

   StringResult parseString();

private:
   llvm::StringRef Str;

   uint8_t getIntegerRadix();
};

} // namespace cdot

#endif // CDOT_LITERALPARSER_H
