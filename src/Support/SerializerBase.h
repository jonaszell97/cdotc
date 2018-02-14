//
// Created by Jonas Zell on 12.01.18.
//

#ifndef CDOT_SERIALIZERBASE_H
#define CDOT_SERIALIZERBASE_H

#include <llvm/ADT/StringRef.h>
#include <vector>

#include "BitstreamWriter.h"
#include "BitstreamReader.h"

namespace cdot {
namespace support {

template<class SubClass>
class SerializerBase {
public:
   void finalize(llvm::raw_ostream &out)
   {
      Writer.Finalize(out);
   }

   void clear()        { Writer.clear(); }
   size_t size() const { return Writer.size(); }

protected:
   serial::BitstreamWriter Writer;

   template<class T, class WriterT, class ...Args>
   void WriteList(const T &arr, WriterT write, Args&&... args)
   {
      Writer.WriteULEB128(arr.size());
      for (const auto &el : arr) {
         (static_cast<SubClass*>(this)->*write)(el,
                                                std::forward<Args&&>(args)...);
      }
   }

   void WriteString(llvm::StringRef str)
   {
      Writer.WriteULEB128(str.size());
      Writer.insert(str);
   }

   void WriteStringWithoutSize(llvm::StringRef str)
   {
      Writer.insert(str);
   }

   void WriteBool(bool b)
   {
      Writer.WriteByte(b);
   }

   template <class T>
   void WriteSize(T const& t)
   {
      Writer.WriteULEB128(t.size());
   }

   template <class T>
   void WriteByte(T t)
   {
      Writer.WriteByte((char)t);
   }

   template<class ...Args>
   void WriteBools(bool b, Args... rest)
   {
      static_assert(sizeof...(Args) <= 7, "can't fit more than 8 bits "
         "into a byte");

      _WriteBoolsImpl(0, 0, b, rest...);
   }

#ifndef NDEBUG
   void WriteSyncPoint()
   {
      WriteString("123$$_Pattern_$321");
   }
#endif

private:
   template<class ...Args>
   void _WriteBoolsImpl(unsigned char c, unsigned shift, bool b, Args... rest)
   {
      c |= (b << shift);
      _WriteBoolsImpl(c, ++shift, rest...);
   }

   void _WriteBoolsImpl(unsigned char c, unsigned shift)
   {
      Writer.WriteByte(c);
   }
};

template <class SubClass>
class DeserializerBase {
protected:
   explicit DeserializerBase(llvm::MemoryBuffer *Buf)
      : Reader(Buf)
   {}

   serial::BitstreamReader Reader;

   template<class T, class ReaderT, class ...Args>
   std::vector<T> ReadList(ReaderT read, Args&&... args)
   {
      std::vector<T> vec;
      auto size = Reader.ReadULEB128();

      for (size_t i = 0; i < size; ++i)
         vec.emplace_back((static_cast<SubClass*>(this)->*read)(
            std::forward<Args&&>(args)...));

      return vec;
   }

   std::string ReadString()
   {
      auto size = Reader.ReadULEB128();
      std::string s;
      s.reserve(size);

      for (size_t i = 0; i < size; ++i)
         s += Reader.ReadByte();

      return s;
   }

   size_t ReadSize()
   {
      return Reader.ReadULEB128();
   }

   bool ReadBool()
   {
      return Reader.ReadByte() != 0;
   }

   template<class ...Args>
   void ReadBools(bool &b, Args&... args)
   {
      _ReadBoolsImpl(Reader.ReadByte(), 0, b, std::forward<Args&>(args)...);
   }

   template<class T>
   T ReadEnum()
   {
      if constexpr (sizeof(typename std::underlying_type<T>::type) == 1)
         return (T)(Reader.ReadByte());
      else
         return (T)(Reader.ReadULEB128());
   }

#ifndef NDEBUG
   void ReadSyncPoint()
   {
      assert(ReadString() == "123$$_Pattern_$321" && "out of sync!");
   }
#endif

private:
   template<class ...Args>
   void _ReadBoolsImpl(char c, unsigned shift, bool &b, Args&... args)
   {
      b = (c & (1 << shift)) != 0;
      _ReadBoolsImpl(c, ++shift, std::forward<Args&>(args)...);
   }

   void _ReadBoolsImpl(char c, unsigned shift) {}
};

} // namespace support
} // namespace cdot

#endif //CDOT_SERIALIZERBASE_H
