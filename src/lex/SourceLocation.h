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
   explicit SourceLocation(uint64_t offset)
      : offset(unsigned(offset))
   {
      assert(offset != 0 && "0 should only be used as a sentinel offset");
      assert(offset < 4294967296 && "not enough space for offset");
   }

   unsigned offset;

   unsigned getOffset() const { return offset; }

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

   explicit SourceRange(SourceLocation start)
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


#endif //CDOT_SOURCELOCATION_H
