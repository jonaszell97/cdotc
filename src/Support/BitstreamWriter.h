//
// Created by Jonas Zell on 17.10.17.
//

#ifndef CDOT_BITSTREAMWRITER_H
#define CDOT_BITSTREAMWRITER_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>
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

   char *data() { return out.data(); }
   llvm::ArrayRef<char> getData() { return out; };

   void Finalize(llvm::raw_ostream &outStream)
   {
      if (currentWord)
         WriteWord(currentWord);

      if (out.empty())
         return;

      outStream.write((const char*)&out.front(), out.size());
   }

   void clear()
   {
      out.clear();
   }

   size_t size() const
   {
      return out.size();
   }

   void WriteByte(char byte)
   {
      out.push_back(byte);
   }

   void WriteWord(unsigned word)
   {
      word = llvm::support::endian::byte_swap<uint32_t,
         llvm::support::little>(word);
      out.append(reinterpret_cast<char*>(&word),
                 reinterpret_cast<char*>(&word + 1));
   }

   template<typename IntTy>
   void WriteInt(IntTy value)
   {
      for (unsigned i = 0; i < sizeof(IntTy); ++i) {
         WriteByte((char)(value & 0xFF));
         value >>= 8;
      }
   }

   void WriteULEB128(size_t value)
   {
      char byte;
      do {
         byte = (char)(value & 0x7F);
         value >>= 7;
         if (value != 0) {
            byte |= 0x80;
         }

         WriteByte(byte);
      } while (value != 0);
   }

   void WriteSLEB128(long long value)
   {
      bool more = true;
      bool negative = value < 0;
      char byte;
      static constexpr long long size = sizeof(long long) * 8;

      while (more) {
         byte = (char)(value & 0x7F);
         value >>= 7;

         if (negative)
            value |= -(static_cast<long long>(1) << (size - 7));

         /* sign bit of byte is col high order bit (0x40) */
         if ((value == 0 && !(value & 0x40)) || (value == -1 && (value & 0x40))){
            more = false;
         }
         else {
            byte |= 0x80;
         }

         WriteByte(byte);
      }
   }

   template<class T>
   void WriteEnum(T t)
   {
      WriteULEB128((size_t)t);
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

   template <class Data>
   void insert(Data const& data)
   {
      out.insert(out.end(), data.begin(), data.end());
   }

   size_t reserve(size_t size)
   {
      auto pos = out.size();
      out.resize(out.size() + size);

      return pos;
   }

protected:
   SmallVector<char, 256> out;

   unsigned currentBit = 0;
   unsigned currentWord = 0;
};

}
}

#endif //CDOT_BITSTREAMWRITER_H
