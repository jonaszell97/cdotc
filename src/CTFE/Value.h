
#ifndef CDOT_CTFEVALUE_H
#define CDOT_CTFEVALUE_H

#include <string>
#include <cstring>
#include <cstdint>

#include <llvm/ADT/StringMap.h>
#include <llvm/Support/Allocator.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallPtrSet.h>

#include "Support/Casting.h"
#include "AST/Type.h"

namespace llvm {
   class APFloat;
   class APSInt;
} // namespace llvm

namespace cdot {

struct Variant;

namespace il {
   class Function;
} // namespace il

namespace ctfe {

using namespace cdot::support;

class Value {
public:
   using Allocator = llvm::BumpPtrAllocator;

   Value() : buffer(nullptr) {}

#  define INT_INIT(Ty)                                   \
   static Value getInt(Ty val) { return Value(val); }

   INT_INIT(size_t)
   INT_INIT(uint64_t)
   INT_INIT(uint32_t)
   INT_INIT(uint16_t)
   INT_INIT(uint8_t)
   INT_INIT(bool)
   INT_INIT(char)
   INT_INIT(int64_t)
   INT_INIT(int32_t)
   INT_INIT(int16_t)
   INT_INIT(int8_t)

#  undef INT_INIT

   static Value getBool(bool val)
   {
      return Value(uint64_t(val));
   }

   static Value getDouble(double d)
   {
      return Value(d);
   }

   static Value getFloat(float f)
   {
      return Value(f);
   }

   static Value getPtr(void *ptr, Allocator &Alloc)
   {
      return Value(ptr);
   }

   static Value getConstPtr(void *ptr)
   {
      return Value(ptr);
   }

   static Value getStr(llvm::StringRef str, Allocator &Alloc)
   {
      auto buffer = Alloc.Allocate(str.size() + 1, alignof(char));
      ::memcpy(buffer, str.data(), str.size());
      reinterpret_cast<char*>(buffer)[str.size()] = '\0';

      return Value(buffer);
   }

   static Value getFunc(il::Function const* F)
   {
      return Value((void*)const_cast<il::Function*>(F));
   }

   static Value getUntyped(size_t bufferSize, Allocator &Alloc)
   {
      return Value(Alloc.Allocate(bufferSize, 1));
   }

   static Value getPreallocated(void *buffer)
   {
      return Value(buffer);
   }

   char *getBuffer() const { return buffer; }
   char *& operator *() { return buffer; }

   /*implicit*/ Value(void *buffer) : buffer((char*)buffer)
   {
      (void)APF;
      (void)APS;
   }

   /*implicit*/ operator char*() const { return buffer; }
   /*implicit*/ operator void*() const { return (void*) buffer; }

private:
   union {
      char *buffer;
      uint64_t u64;
      uint32_t u32;
      uint16_t u16;
      uint8_t u8;
      int64_t i64;
      int32_t i32;
      int16_t i16;
      int8_t i8;

      int i;
      long l;
      long long ll;

      unsigned u;
      unsigned long lu;
      unsigned long long llu;

      bool b;
      char c;
      double d;
      float f;

      Value* V;
      il::Function *F;

      llvm::APSInt *APS;
      llvm::APFloat *APF;
   };

public:
#  define CDOT_VALUE_INIT(Field, Ty)        \
   Value(Ty val) { u64 = 0; Field = val; }

   CDOT_VALUE_INIT(u64, size_t)
   CDOT_VALUE_INIT(u64, uint64_t)
   CDOT_VALUE_INIT(u32, uint32_t)
   CDOT_VALUE_INIT(u16, uint16_t)
   CDOT_VALUE_INIT(u8, uint8_t)
   CDOT_VALUE_INIT(b, bool)
   CDOT_VALUE_INIT(c, char)
   CDOT_VALUE_INIT(i64, int64_t)
   CDOT_VALUE_INIT(i32, int32_t)
   CDOT_VALUE_INIT(i16, int16_t)
   CDOT_VALUE_INIT(i8, int8_t)

   CDOT_VALUE_INIT(d, double)
   CDOT_VALUE_INIT(f, float)

   CDOT_VALUE_INIT(V, Value*)
   CDOT_VALUE_INIT(F, il::Function*)

#  undef CDOT_VALUE_INIT

#  define CDOT_VALUE_GETTER(Name, Field, Type)      \
   Type get##Name() const { return Field; }

   CDOT_VALUE_GETTER(U64, u64, uint64_t)
   CDOT_VALUE_GETTER(U32, u32, uint32_t)
   CDOT_VALUE_GETTER(U16, u16, uint16_t)
   CDOT_VALUE_GETTER(U8, u8, uint8_t)
   CDOT_VALUE_GETTER(Bool, b, bool)
   CDOT_VALUE_GETTER(Char, c, char)
   CDOT_VALUE_GETTER(I64, i64, int64_t)
   CDOT_VALUE_GETTER(I32, i32, int32_t)
   CDOT_VALUE_GETTER(I16, i16, int16_t)
   CDOT_VALUE_GETTER(I8, i8, int8_t)

   CDOT_VALUE_GETTER(Int, i, int)
   CDOT_VALUE_GETTER(Long, l, long)
   CDOT_VALUE_GETTER(LongLong, ll, long long)
   CDOT_VALUE_GETTER(Unsigned, u, unsigned)
   CDOT_VALUE_GETTER(ULong, lu, unsigned long)
   CDOT_VALUE_GETTER(ULongLong, llu, unsigned long long)

   CDOT_VALUE_GETTER(Double, d, double)
   CDOT_VALUE_GETTER(Float, f, float)

   CDOT_VALUE_GETTER(ValuePtr, V, Value const*)
   CDOT_VALUE_GETTER(FuncPtr, F, il::Function*)

#  undef CDOT_VALUE_GETTER

   char *getUntypedPtr()       { return buffer; }
   char *getUntypedPtr() const { return buffer; }

   Value load(QualType Ty) const
   {
      return Ty->needsStructReturn() ? *this
                                     : Value(getValuePtr()->getBuffer());
   }

   Value *getValuePtr()  { return reinterpret_cast<Value*>(buffer); }
   std::string getString() const;
};

} // namespace ctfe
} // namespace cdot

#undef CDOT_CTFE_VALUE

#endif //CDOT_CTFEVALUE_H
