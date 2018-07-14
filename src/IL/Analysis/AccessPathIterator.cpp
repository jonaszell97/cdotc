//
// Created by Jonas Zell on 02.05.18.
//

#include "AccessPathIterator.h"
#include "IL/Instructions.h"

using namespace cdot::support;

namespace cdot {
namespace il {

AccessPathIterator::AccessPathIterator(il::Value *V) : V(V)
{
   if (!V->isLvalue())
      moveNext();
}

static il::Value *LookThroughLoad(il::Value *V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getOperand(0);

   // also look through MutRefToRef casts
   if (auto BC = dyn_cast<BitCastInst>(V))
      if (BC->getType()->isReferenceType()
          && BC->getOperand(0)->getType()->isReferenceType()) {
         return BC->getOperand(0);
      }

   return V;
}

void AccessPathIterator::moveNext()
{
   V = LookThroughLoad(V);
   if (auto Load = dyn_cast<LoadInst>(V)) {
      V = Load->getTarget();
   }
   else if (auto GEP = dyn_cast<GEPInst>(V)) {
      V = LookThroughLoad(GEP->getOperand(0));
   }
   else if (auto FieldRef = dyn_cast<FieldRefInst>(V)) {
      V = LookThroughLoad(FieldRef->getOperand(0));
   }
   else if (auto TupleExtract = dyn_cast<TupleExtractInst>(V)) {
      V = LookThroughLoad(TupleExtract->getOperand(0));
   }
   else {
      V = nullptr;
   }
}

} // namespace il
} // namespace cdot