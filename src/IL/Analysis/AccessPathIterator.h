#ifndef CDOT_ACCESSPATHITERATOR_H
#define CDOT_ACCESSPATHITERATOR_H

#include <llvm/ADT/iterator_range.h>

namespace cdot {
namespace il {

class Value;

struct AccessPathIterator {
private:
   il::Value *V;
   bool RvaluePath = false;

   void moveNext();

public:
   AccessPathIterator() : V(nullptr) {}
   explicit AccessPathIterator(il::Value *V, bool RvaluePath = false);

   AccessPathIterator &operator++() { moveNext(); return *this; }
   AccessPathIterator operator++(int)
   {
      auto cpy = *this;
      ++*this;
      return cpy;
   }

   il::Value *operator*()  const { return V; }
   il::Value *operator->() const { return V; }

   friend bool operator==(const AccessPathIterator &LHS,
                          const AccessPathIterator &RHS) {
      return LHS.V == RHS.V;
   }

   friend bool operator!=(const AccessPathIterator &LHS,
                          const AccessPathIterator &RHS) {
      return !(LHS == RHS);
   }
};

using access_path_range = llvm::iterator_range<AccessPathIterator>;

inline access_path_range AccessPath(il::Value *V, bool RvaluePath = false)
{
   return access_path_range(AccessPathIterator(V, RvaluePath),
                            AccessPathIterator());
}

inline Value *AccessedMem(il::Value *V)
{
   auto It = AccessPathIterator(V);
   il::Value *Mem = nullptr;
   while (*It) {
      Mem = *It++;
   }

   return Mem;
}

} // namespace il
} // namespace cdot

#endif //CDOT_ACCESSPATHITERATOR_H
