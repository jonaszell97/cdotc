#ifndef CDOT_DATAFLOWPROBLEM_H
#define CDOT_DATAFLOWPROBLEM_H

#include "IL/BasicBlock.h"
#include "IL/Function.h"
#include "IL/Instructions.h"
#include "IL/Utils/BlockIterator.h"

#include <llvm/ADT/BitVector.h>

namespace cdot {
namespace il {
namespace detail {

using BlockBitVectorMap = llvm::DenseMap<BasicBlock*, llvm::BitVector>;

inline void andNot(llvm::BitVector &LHS, llvm::BitVector &RHS, unsigned NumBits)
{
   assert(LHS.size() == NumBits && RHS.size() == NumBits);
   for (unsigned i = 0; i < NumBits; ++i) {
      LHS[i] = LHS[i] & ~RHS[i];
   }
}

} // namespace detail

template<llvm::BitVector&(llvm::BitVector::*meet)(const llvm::BitVector&),
         class GenFnTy>
void solveForwardProblem(Function &F, unsigned NumFacts,
                         detail::BlockBitVectorMap &GenMap,
                         detail::BlockBitVectorMap &KillMap,
                         detail::BlockBitVectorMap &InMap,
                         detail::BlockBitVectorMap &OutMap,
                         const GenFnTy &GenFn,
                         bool InitialValue = true) {
   // Calculate Gen and Kill sets for each BB
   // Kill always empty since a variable can't be "deinitialized"
   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      auto &Gen = GenMap[&B];
      Gen.resize(NumFacts);

      auto &Kill = KillMap[&B];
      Kill.resize(NumFacts);

      for (auto &I : B) {
         GenFn(I, Gen, Kill);
      }
   }

   // Calculate In / Out until no more changes happen
   llvm::SmallPtrSet<BasicBlock*, 16> WorkList;

   for (auto &B : F) {
      auto &In = InMap[&B];
      auto &Out = OutMap[&B];

      In.resize(NumFacts);
      Out.resize(NumFacts);

      if (&B != F.getEntryBlock()) {
         if (InitialValue)
            Out.flip();

         WorkList.insert(&B);
      }
      else {
         Out = GenMap[&B];
         Out |= InMap[&B];
      }
   }

   while (!WorkList.empty()) {
      auto BB = *WorkList.begin();
      WorkList.erase(BB);

      if (BB == F.getEntryBlock() || BB->hasNoPredecessors())
         continue;

      auto &In = InMap[BB];
      auto &Out = OutMap[BB];

      bool first = true;

      // recalculate In as intersection of all predecessors (a value is
      // initialized in B if it is intialized in all predecessors of B)
      for (auto pred : getPredecessors(BB)) {
         if (first) {
            first = false;
            In = OutMap[pred];
         }
         else {
            (In.*meet)(OutMap[pred]);
         }
      }

      // Out(B) = (Gen(B) | In(B)) & ~Kill(B)
      auto NewOut = GenMap[BB];
      NewOut |= In;
      detail::andNot(NewOut, KillMap[BB], NumFacts);


      if (NewOut != Out) {
         for (auto Succ : getSuccessors(BB))
            WorkList.insert(Succ);

         Out = NewOut;
      }
   }
}

template<class GenFnTy>
void solveForwardMayProblem(Function &F, unsigned NumFacts,
                            detail::BlockBitVectorMap &GenMap,
                            detail::BlockBitVectorMap &KillMap,
                            detail::BlockBitVectorMap &InMap,
                            detail::BlockBitVectorMap &OutMap,
                            const GenFnTy &GenFn,
                            bool InitialValue = true) {
   solveForwardProblem<&llvm::BitVector::operator|=>(F, NumFacts, GenMap,
                                                     KillMap,
                                                     InMap, OutMap, GenFn,
                                                     InitialValue);
}

template<class GenFnTy>
void solveForwardMustProblem(Function &F, unsigned NumFacts,
                             detail::BlockBitVectorMap &GenMap,
                             detail::BlockBitVectorMap &KillMap,
                             detail::BlockBitVectorMap &InMap,
                             detail::BlockBitVectorMap &OutMap,
                             const GenFnTy &GenFn,
                             bool InitialValue = true) {
   solveForwardProblem<&llvm::BitVector::operator&=>(F, NumFacts, GenMap,
                                                     KillMap,
                                                     InMap, OutMap, GenFn,
                                                     InitialValue);
}

} // namespace il
} // namespace cdot

#endif //CDOT_DATAFLOWPROBLEM_H
