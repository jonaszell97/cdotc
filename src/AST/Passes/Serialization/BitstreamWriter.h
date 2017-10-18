//
// Created by Jonas Zell on 17.10.17.
//

#ifndef CDOT_BITSTREAMWRITER_H
#define CDOT_BITSTREAMWRITER_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/Endian.h>
#include <llvm/Support/raw_ostream.h>

using llvm::SmallVector;

namespace cdot {
namespace serial {

class BitstreamWriter {
public:
   BitstreamWriter() = default;
   ~BitstreamWriter() {
      assert(!currentWord && "didn't call finalize!");
   }

   void Finalize(llvm::raw_ostream &outStream)
   {
      if (currentWord) {
         WriteWord(currentWord);
      }

      outStream << out;
   }

   void WriteByte(unsigned char byte)
   {
      out.push_back(byte);
   }

   void WriteWord(unsigned word)
   {
      word = llvm::support::endian::byte_swap<uint32_t, llvm::support::little>(word);
      out.append(reinterpret_cast<unsigned char*>(&word),
                 reinterpret_cast<unsigned char*>(&word + 1));
   }

   void WriteULEB128(size_t value)
   {
      unsigned char byte;
      do {
         byte = (unsigned char)(value & 127);
         value >>= 7;
         if (value != 0) {
            byte |= 128;
         }

         WriteByte(byte);
      } while (value != 0);
   }

   void WriteSLEB128(long long value)
   {
      bool more = true;
      bool negative = (value < 0);
      unsigned char byte;

      while (more) {
         byte = (unsigned char)(value & 127);
         value >>= 7;

         /* sign bit of byte is second high order bit (0x40) */
         if ((value == 0 && !(value & 128)) || (value == -1 && (value & 128))) {
            more = false;
         }
         else {
            byte |= 128;
         }

         WriteByte(byte);
      }
   }

   void Write(unsigned val, unsigned numBits)
   {
      assert(!(val & (~0u << numBits)) && "high bits set");
      assert(numBits > 0 && numBits <= 32);

      currentWord |= val << numBits;
      if (currentBit < 32 - numBits) {
         currentBit += numBits;
         return;
      }

      WriteWord(currentWord);

      if (currentBit) {
         currentWord = val >> (32 - currentBit);
      }
      else {
         currentWord = 0;
      }

      currentBit = (currentBit + numBits) & 31;
   }

   struct Block {
      unsigned blockID;
      unsigned blockSize;
      unsigned char *blockBegin;
   };

   void BeginBlock()
   {

   }

protected:
   SmallVector<char, 256> out;

   unsigned currentBit = 0;
   unsigned currentWord = 0;
};

}
}

#endif //CDOT_BITSTREAMWRITER_H
