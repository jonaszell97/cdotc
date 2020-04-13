#include "cdotc/IL/Analysis/AccessPathIterator.h"

#include "cdotc/IL/Instructions.h"

using namespace cdot::support;

namespace cdot {
namespace il {

AccessPathIterator::AccessPathIterator(il::Value* V, bool RvaluePath)
    : V(V), RvaluePath(RvaluePath)
{
   if (!V->isLvalue() && !RvaluePath)
      moveNext();
}

static il::Value* LookThroughLoad(il::Value* V)
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
   if (!RvaluePath)
      V = LookThroughLoad(V);

   if (auto Load = dyn_cast<LoadInst>(V)) {
      V = Load->getTarget();
   }
   else if (auto GEP = dyn_cast<GEPInst>(V)) {
      V = GEP->getOperand(0);
   }
   else if (auto FieldRef = dyn_cast<FieldRefInst>(V)) {
      V = FieldRef->getOperand(0);
   }
   else if (auto TupleExtract = dyn_cast<TupleExtractInst>(V)) {
      V = TupleExtract->getOperand(0);
   }
   else {
      V = nullptr;
   }

   if (!RvaluePath && V)
      V = LookThroughLoad(V);
}

} // namespace il
} // namespace cdot