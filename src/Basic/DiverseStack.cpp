#include "DiverseStack.h"

namespace cdot {

void DiverseStackBase::pushNewStorageSlow(std::size_t needed) {
   bool wasInline = isAllocatedInline();

   std::size_t capacity = End - Allocated;
   std::size_t requiredCapacity = capacity + needed;
   do {
      capacity = 2 * capacity + 16;
   } while (capacity < requiredCapacity);

   assert(capacity % 16 == 0 && "not allocating multiple of alignment");

   char *oldAllocation = Allocated;
   char *oldBegin = Begin;
   std::size_t oldSize = (std::size_t) (End - oldBegin);

   Allocated = new char[capacity];
   End = Allocated + capacity;
   Begin = End - oldSize;
   std::memcpy(Begin, oldBegin, oldSize);

   Begin -= needed;

   if (!wasInline) delete[] oldAllocation;
}

} // namespace cdot