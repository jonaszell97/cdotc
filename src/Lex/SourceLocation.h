#ifndef CDOT_SOURCELOCATION_H
#define CDOT_SOURCELOCATION_H

#include "Support/Config.h"

#include <llvm/ADT/StringRef.h>

#include <cassert>
#include <cstdint>

namespace cdot {

struct SourceLocation {
   CDOT_LLDB_STEP_OVER
   SourceLocation() : offset(0)  {}

   CDOT_LLDB_STEP_OVER
   explicit SourceLocation(uint32_t offset) : offset(offset)
   {

   }

private:
   uint32_t offset;

public:
   uint32_t getOffset() const { return offset; }

   SourceLocation offsetBy(uint32_t offset) const
   {
      return SourceLocation(this->offset + offset);
   }

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
   CDOT_LLDB_STEP_OVER
   SourceRange() = default;

   CDOT_LLDB_STEP_OVER
   /*implicit*/ SourceRange(SourceLocation start)
      : start(start), end(SourceLocation())
   {}

   CDOT_LLDB_STEP_OVER
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

struct FullSourceLoc {
private:
   /// \brief The source file of this location.
   llvm::StringRef fileName;

   /// \brief The source line of this location.
   unsigned line;

   /// \brief The source column of this location.
   unsigned column;

public:
   /// \brief Memberwise C'tor.
   FullSourceLoc(llvm::StringRef fileName, unsigned line, unsigned column)
      : fileName(fileName), line(line), column(column)
   {}

   /// \return The source file of this location.
   llvm::StringRef getSourceFileName() const { return fileName; }

   /// \return The source line of this location.
   unsigned getLine() const { return line; }

   /// \return The source column of this location.
   unsigned getColumn() const { return column; }
};

} // namespace cdot

namespace llvm {

template <typename T> struct PointerLikeTypeTraits;
template<class T> struct DenseMapInfo;
class raw_ostream;

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

namespace cdot {

::llvm::raw_ostream &operator<<(::llvm::raw_ostream &OS,
                                const ::cdot::FullSourceLoc &loc);

} // namespace cdot

#endif //CDOT_SOURCELOCATION_H
