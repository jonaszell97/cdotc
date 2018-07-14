//
// Created by Jonas Zell on 22.11.17.
//

#include "Use.h"
#include <list>

namespace cdot {
namespace il {

#ifndef NDEBUG

void Use::verify()
{
   Use *U = this;
   while (U->Prev) {
      U = U->Prev;
   }

   // verify linked list invariants
   while (U->Next) {
      Use *Next = U->Next;
      assert(Next->Prev == U);

      U = Next;
   }
}

#endif

} // namespace il
} // namespace cdot