//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_SOURCELOCATION_H
#define CDOT_SOURCELOCATION_H

#include <cstdint>
#include <cassert>

namespace cdot {

struct SourceLocation {
   SourceLocation() : offset(0)  {}
   explicit SourceLocation(uint32_t offset) : offset(offset)
   {

   }

private:
   uint32_t offset;

public:
   uint32_t getOffset() const { return offset; }

   bool isValid() const { return offset > 0; }
   operator bool() const { return isValid(); }

   friend bool operator==(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset == RHS.offset;
   }

   friend bool operator!=(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset != RHS.offset;
   }

   friend bool operator<(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset < RHS.offset;
   }

   friend bool operator<=(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset <= RHS.offset;
   }

   friend bool operator>(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset > RHS.offset;
   }

   friend bool operator>=(const SourceLocation &LHS, const SourceLocation &RHS)
   {
      return LHS.offset >= RHS.offset;
   }
};

struct SourceRange {
   SourceRange() = default;

   /*implicit*/ SourceRange(SourceLocation start)
      : start(start), end(SourceLocation())
   {}

   SourceRange(SourceLocation start, SourceLocation end)
      : start(start), end(end)
   {}

   SourceLocation getStart() const
   {
      return start;
   }

   SourceLocation getEnd() const
   {
      return end;
   }

   operator bool() const { return start.isValid() && end.isValid(); }

   friend bool operator==(const SourceRange &LHS, const SourceRange &RHS)
   {
      return LHS.start == RHS.start && LHS.end == RHS.end;
   }

   friend bool operator!=(const SourceRange &LHS, const SourceRange &RHS)
   {
      return !(LHS == RHS);
   }

private:
   SourceLocation start;
   SourceLocation end;
};

} // namespace cdot

namespace llvm {

template <typename T> struct PointerLikeTypeTraits;
template<class T> struct DenseMapInfo;

template<>
struct PointerLikeTypeTraits<::cdot::SourceLocation> {
public:
   static inline void *getAsVoidPointer(::cdot::SourceLocation P)
   {
      return reinterpret_cast<void*>(P.getOffset());
   }

   static inline ::cdot::SourceLocation getFromVoidPointer(void *P)
   {
      return ::cdot::SourceLocation(
         static_cast<uint32_t>(reinterpret_cast<uint64_t>(P)));
   }

   enum { NumLowBitsAvailable = 0 };
};

template<> struct DenseMapInfo<::cdot::SourceLocation> {
   static ::cdot::SourceLocation getEmptyKey()
   {
      return ::cdot::SourceLocation(static_cast<uint32_t>(-1));
   }

   static ::cdot::SourceLocation getTombstoneKey()
   {
      return ::cdot::SourceLocation(static_cast<uint32_t>(-2));
   }

   static int getHashValue(const ::cdot::SourceLocation P)
   {
      return static_cast<int>(P.getOffset());
   }

   static bool isEqual(const ::cdot::SourceLocation LHS,
                       const ::cdot::SourceLocation RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm


#endif //CDOT_SOURCELOCATION_H
