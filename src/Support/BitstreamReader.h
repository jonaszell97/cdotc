//
// Created by Jonas Zell on 17.10.17.
//

#ifndef CDOT_BITSTREAMREADER_H
#define CDOT_BITSTREAMREADER_H

#include <llvm/Support/MemoryBuffer.h>

namespace cdot {
namespace serial {

class BitstreamReader {
public:
   explicit BitstreamReader(llvm::MemoryBuffer *buff)
      : buff(buff),
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

   char ReadByte()
   {
      return *(current++);
   }

   template<typename IntTy>
   IntTy ReadInt()
   {
      IntTy value = 0;
      unsigned shift = 0;

      for (unsigned i = 0; i < sizeof(IntTy); ++i) {
         value |= ReadByte() << shift;
         shift += 8;
      }

      return value;
   }

   size_t ReadULEB128()
   {
      size_t result = 0;
      unsigned shift = 0;
      char byte;

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
      char byte;
      static constexpr unsigned size = sizeof(long long) * 8;

      do {
         byte = ReadByte();
         result |= (static_cast<long long>(byte & 127) << shift);
         shift += 7;
      } while ((byte & 0x80) != 0);

      if (shift < size && (byte & 0x40) != 0) {
         result |= -(static_cast<long long>(1) << shift);
      }

      return result;
   }

   ptrdiff_t getOffset() const
   {
      return current - buff->getBufferStart();
   }

   const char* data() const { return current; }
   const char* skip(size_t numBytes)
   {
      auto ptr = current;
      current += numBytes;

      return ptr;
   }

   void reposition(size_t offset)
   {
      current = buff->getBufferStart() + offset;
      currentBit = 0;
   }

protected:
   llvm::MemoryBuffer* buff;
   const char *current;

   unsigned short currentBit = 0;
};

}
}
#endif //CDOT_BITSTREAMREADER_H
