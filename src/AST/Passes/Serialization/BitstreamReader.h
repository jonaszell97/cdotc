//
// Created by Jonas Zell on 17.10.17.
//

#ifndef CDOT_BITSTREAMREADER_H
#define CDOT_BITSTREAMREADER_H

#include <memory>
#include <llvm/Support/MemoryBuffer.h>

namespace cdot {
namespace serial {

   class BitstreamReader {
   public:
      BitstreamReader(std::unique_ptr<llvm::MemoryBuffer> &&buff) : buff(std::move(buff)),
                                                                    current(this->buff->getBufferStart())
      {

      }

      bool ReadBit()
      {
         auto res = (*current & (1 << currentBit)) != 0;
         if (currentBit == 7) {
            currentBit = 0;
            ++current;
         }
         else {
            ++currentBit;
         }

         return res;
      }

      void SkipCurrentByte()
      {
         ++current;
         currentBit = 0;
      }

      unsigned char ReadByte()
      {
         return *(current++);
      }

      unsigned ReadWord()
      {
         unsigned value = 0;
         value |= ReadByte();
         value |= ReadByte() << 8;
         value |= ReadByte() << 16;
         value |= ReadByte() << 24;

         return value;
      }

      size_t ReadULEB128()
      {
         size_t result = 0;
         unsigned shift = 0;
         unsigned char byte;

         do {
            byte = ReadByte();
            result |= (byte & 0x7F) << shift;

            shift += 7;
         } while ((byte & 0x80) != 0);

         return result;
      }

      long long ReadSLEB128()
      {
         long long result = 0;
         unsigned shift = 0;
         unsigned char byte;

         do {
            byte = ReadByte();
            result |= (byte & 0x7F) << shift;
            shift += 7;
         } while ((byte & 0x80) != 0);

         if (shift < 8 * sizeof(long long) && (byte & 0x40) != 0) {
            result |= ~(0ull) << shift;
         }

         return result;
      }

   protected:
      std::unique_ptr<llvm::MemoryBuffer> buff;
      const char *current;

      unsigned short currentBit = 0;
   };

}
}
#endif //CDOT_BITSTREAMREADER_H