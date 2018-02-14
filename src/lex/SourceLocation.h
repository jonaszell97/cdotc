//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_SOURCELOCATION_H
#define CDOT_SOURCELOCATION_H

#include <cstdint>
#include <cassert>

namespace cdot {

struct SourceLocation {
   SourceLocation() : offset(0), sourceId(0) {}
   SourceLocation(uint64_t offset,
                  uint64_t sourceId)
      : offset(unsigned(offset)), sourceId(unsigned(sourceId))
   {
      assert(offset   < 4294967296 && "not enough space for offset");
      assert(sourceId < 4294967296 && "not enough space for sourceId");
   }

   unsigned offset : 32;
   unsigned sourceId : 32;

   unsigned getOffset() const { return offset; }
   unsigned getSourceId() const { return sourceId; }

   bool isValid() const { return sourceId != 0; }
   operator bool() const { return isValid(); }
};

} // namespace cdot


#endif //CDOT_SOURCELOCATION_H
