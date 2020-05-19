#include "cdotc/IL/Use.h"

#include <list>
#include <llvm/ADT/SmallPtrSet.h>

namespace cdot {
namespace il {

#ifndef NDEBUG

void Use::verify()
{
   Use* U = this;
   while (U->Prev) {
      U = U->Prev;
   }

   // verify linked list invariants
   llvm::SmallPtrSet<Use*, 4> Uses;
   while (U->Next) {
      assert(Uses.insert(U).second && "duplicate use!");

      Use* Next = U->Next;
      assert(Next->Prev == U);

      U = Next;
   }
}

#endif

} // namespace il
} // namespace cdot