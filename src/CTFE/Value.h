
#ifndef CDOT_CTFEVALUE_H
#define CDOT_CTFEVALUE_H

#include <llvm/ADT/StringMap.h>
#include <string>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/SmallPtrSet.h>

#include "../Support/Casting.h"

#include "../Variant/Type/Type.h"
#include "../Variant/Type/IntegerType.h"
#include "../Variant/Type/PointerType.h"
#include "../Variant/Type/FPType.h"

namespace cdot {

struct Variant;

namespace il {
   class Function;
} // namespace il

namespace ctfe {

using namespace cdot::support;

class Value {
public:
   Value() : buffer(nullptr) {}

   static Value getInt(Type *ty, uint64_t val)
   {
      return Value(ty, val);
   }

   static Value getDouble(double d)
   {
      return Value(FPType::getDoubleTy(), d);
   }

   static Value getFloat(float f)
   {
      return Value(FPType::getFloatTy(), f);
   }

   static Value getPtr(Type *ty, void *ptr)
   {
      auto buf = (void**)__managed_malloc(sizeof(void*));
      *buf = ptr;

      return Value(ty, buf);
   }

   static Value getConstPtr(Type *ty, void *ptr)
   {
      return Value(ty, ptr);
   }

   static Value getStr(Type *ty, llvm::StringRef str)
   {
      auto buffer = __managed_malloc(str.size() + 1);
      memcpy(buffer, str.data(), str.size());
      reinterpret_cast<char*>(buffer)[str.size()] = '\0';

      return Value(ty, buffer);
   }

   static Value getFunc(il::Function const* F);
   static Value getStruct(Type *ty, llvm::ArrayRef<Value> fieldValues);
   static Value getArray(Type *ty, llvm::ArrayRef<Value> fieldValues);
   static Value getTuple(Type *ty, llvm::ArrayRef<Value> fieldValues);
   static Value getUnion(Type *ty, Type *initTy, Value Initializer);
   static Value getEnum(Type *ty, llvm::StringRef caseName,
                        llvm::ArrayRef<Value> fieldValues);

   static Value getLambda(il::Function const* F,
                          llvm::ArrayRef<std::pair<Type*, Value>> captures);

   static Value getUntyped(size_t bufferSize)
   {
      return Value(IntegerType::getCharTy()->getPointerTo(),
                   __managed_malloc(bufferSize));
   }

   static Value getPreallocated(Type *ty, void *buffer)
   {
      return Value(ty, buffer);
   }

   static Value getNullValue(Type *ty);

   std::string toString(Type *type);
   Variant toVariant(Type *type);

   char *getBuffer() const { return buffer; }

   char *& operator *() { return buffer; }

   static size_t getNumCreatedValues()
   {
      return allocations.size();
   }

   static void resize(size_t toSize)
   {
      size_t i = allocations.size() - 1;
      while (i > toSize) {
         free(allocations[i]);
         --i;
      }

      allocations.resize(toSize);
   }

   struct ScopeGuard {
   public:
      ScopeGuard()
         : size(getNumCreatedValues())
      {

      }

      ~ScopeGuard()
      {
         resize(size);
      }

   private:
      size_t size;
   };

private:
   static llvm::SmallVector<void*, 128> allocations;

   explicit Value(Type *type, void *buffer)
      : buffer((char*)buffer)
   {

   }

   static void *__managed_malloc(size_t size)
   {
      auto ptr = malloc(size);
      allocations.push_back(ptr);

      return ptr;
   }

#ifndef NDEBUG
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
      il::Function const* F;
   };
#else
   char *buffer;
#endif

   void destroyValue(Type *type);

public:
#  define CDOT_VALUE_INIT(Field, Ty)        \
   Value(Type *ty, Ty val) : Field(val) {}

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
   CDOT_VALUE_INIT(F, il::Function const*)

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
   CDOT_VALUE_GETTER(FuncPtr, F, il::Function const*)

#  undef CDOT_VALUE_GETTER

   char *getUntypedPtr()       { return buffer; }
   char *getUntypedPtr() const { return buffer; }

   Value *getValuePtr()  { return reinterpret_cast<Value*>(buffer); }

   Value getStructElement(Type *ty, llvm::StringRef fieldName);
   Value getStructElement(Type *ty, size_t idx);

   Value getArrayElement(Type *ty, size_t idx);
   Value getTupleElement(Type *ty, size_t idx);

   Value getElementPtr(Type *ty, size_t idx);

   Value getLambdaEnvironment();
   il::Function *getLambdaFuncPtr();

   Value getEnumRawValue(Type *type);
   Value getEnumCaseValue(Type *ty, llvm::StringRef caseName, size_t idx);

   std::string getString() const;

   void doStore(Type *ty, Value dst) const;
};

} // namespace ctfe
} // namespace cdot

#undef CDOT_CTFE_VALUE

#endif //CDOT_CTFEVALUE_H
