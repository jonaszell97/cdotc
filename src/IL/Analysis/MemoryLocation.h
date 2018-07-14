//
// Created by Jonas Zell on 30.04.18.
//

#ifndef CDOT_MEMORYLOCATION_H
#define CDOT_MEMORYLOCATION_H

#include "Basic/DeclarationName.h"

namespace cdot {
namespace il {

class Value;
class Instruction;

struct MemoryLocation {
private:
   uintptr_t OpaqueValue;

   MemoryLocation() : OpaqueValue(0) {}
   explicit MemoryLocation(uintptr_t Val) : OpaqueValue(Val) {}

   static MemoryLocation getImpl(il::Value *V, bool Exact, bool LookThroughMove);
   static MemoryLocation getImpl(il::Instruction *I, bool Exact,
                                 bool LookThroughMove);

public:
   static MemoryLocation get();
   static MemoryLocation get(il::Value *V, bool LookThroughMove = false)
   { return getImpl(V, false, LookThroughMove); }
   static MemoryLocation get(il::Instruction *I, bool LookThroughMove = false)
   { return getImpl(I, false, LookThroughMove); }

   static MemoryLocation getTupleField(MemoryLocation Tup, unsigned Idx);
   static MemoryLocation getStructField(MemoryLocation S,
                                        DeclarationName FieldName);

   static MemoryLocation getExact(il::Value *V, bool LookThroughMove = true)
   { return getImpl(V, true, LookThroughMove); }

   static MemoryLocation getExact(il::Instruction *I,
                                  bool LookThroughMove = true) {
      return getImpl(I, true, LookThroughMove);
   }

   MemoryLocation(MemoryLocation&&)      = default;
   MemoryLocation(const MemoryLocation&) = default;

   MemoryLocation &operator=(MemoryLocation&&)      = default;
   MemoryLocation &operator=(const MemoryLocation&) = default;

   bool isValid() const { return OpaqueValue != 0; }
   explicit operator bool() const { return isValid(); }

   friend bool operator==(const MemoryLocation &LHS, const MemoryLocation &RHS)
   {
      return LHS.OpaqueValue == RHS.OpaqueValue;
   }

   friend bool operator!=(const MemoryLocation &LHS, const MemoryLocation &RHS)
   {
      return LHS.OpaqueValue != RHS.OpaqueValue;
   }

   void *getAsOpaquePtr() const { return reinterpret_cast<void*>(OpaqueValue); }
   static MemoryLocation getFromOpaquePtr(void *Ptr)
   {
      return MemoryLocation(reinterpret_cast<uintptr_t>(Ptr));
   }
};

} // namespace il
} // namespace cdot

namespace llvm {

template <typename T> struct PointerLikeTypeTraits;
template<class T> struct DenseMapInfo;

template<>
struct PointerLikeTypeTraits<::cdot::il::MemoryLocation> {
public:
   static inline void *getAsVoidPointer(::cdot::il::MemoryLocation P)
   {
      return P.getAsOpaquePtr();
   }

   static inline ::cdot::il::MemoryLocation getFromVoidPointer(void *P)
   {
      return ::cdot::il::MemoryLocation::getFromOpaquePtr(P);
   }

   enum { NumLowBitsAvailable = 0 };
};

template<> struct DenseMapInfo<::cdot::il::MemoryLocation> {
   static ::cdot::il::MemoryLocation getEmptyKey()
   {
      return ::cdot::il::MemoryLocation::getFromOpaquePtr(
         reinterpret_cast<void*>(-1));
   }

   static ::cdot::il::MemoryLocation getTombstoneKey()
   {
      return ::cdot::il::MemoryLocation::getFromOpaquePtr(
         reinterpret_cast<void*>(-2));
   }

   static int getHashValue(const ::cdot::il::MemoryLocation P)
   {
      return static_cast<int>(
         reinterpret_cast<uintptr_t>(P.getAsOpaquePtr()));
   }

   static bool isEqual(const ::cdot::il::MemoryLocation LHS,
                       const ::cdot::il::MemoryLocation RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm

#endif //CDOT_MEMORYLOCATION_H
