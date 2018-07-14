//
// Created by Jonas Zell on 29.04.18.
//

#include "Dominance.h"

#include <llvm/Support/GenericDomTreeConstruction.h>

using namespace cdot::support;

template class llvm::DominatorTreeBase<cdot::il::BasicBlock, false>;
template class llvm::DominatorTreeBase<cdot::il::BasicBlock, true>;
template class llvm::DomTreeNodeBase<cdot::il::BasicBlock>;

namespace llvm {
namespace DomTreeBuilder {

using ILDomTree = llvm::DomTreeBase<cdot::il::BasicBlock>;
using ILPostDomTree = llvm::PostDomTreeBase<cdot::il::BasicBlock>;

template void Calculate<ILDomTree>(ILDomTree &DT);
template void Calculate<ILPostDomTree>(ILPostDomTree &DT);

} // namespace DomTreeBuilder
} // namespace llvm

namespace cdot {
namespace il {

/// Compute the immediate-dominators map.
DominanceInfo::DominanceInfo(il::Function *F)
   : DominatorTreeBase()
{
   assert(!F->isDeclared() &&
          "Make sure the function is a definition and not a declaration.");
   recalculate(*F);
}

bool DominanceInfo::properlyDominates(il::Instruction *a, il::Instruction *b)
{
   auto aBlock = a->getParent(), bBlock = b->getParent();

   // If the blocks are different, it's as easy as whether A's block
   // dominates B's block.
   if (aBlock != bBlock)
      return properlyDominates(a->getParent(), b->getParent());

   // Otherwise, they're in the same block, and we just need to check
   // whether B comes after A.  This is a non-strict computation.
   auto aIter = a->getIterator();
   auto bIter = b->getIterator();
   auto fIter = aBlock->begin();
   while (bIter != fIter) {
      --bIter;
      if (aIter == bIter)
         return true;
   }

   return false;
}

/// Does value A properly dominate instruction B?
bool DominanceInfo::properlyDominates(il::Value *a, il::Instruction *b)
{
   if (auto *Inst = dyn_cast<Instruction>(a)) {
      return properlyDominates(Inst, b);
   }
   if (auto *Arg = dyn_cast<il::Argument>(a)) {
      return dominates(Arg->getParent(), b->getParent());
   }

   return false;
}

void DominanceInfo::verify() const
{
   // Recompute.
   auto *F = getRoot()->getParent();
   DominanceInfo OtherDT(F);

   // And compare.
   if (errorOccurredOnComparison(OtherDT)) {
      llvm::errs() << "DominatorTree is not up to date!\nComputed:\n";
      print(llvm::errs());
      llvm::errs() << "\nActual:\n";
      OtherDT.print(llvm::errs());
      abort();
   }
}

/// Compute the immediate-post-dominators map.
PostDominanceInfo::PostDominanceInfo(il::Function *F)
   : PostDominatorTreeBase()
{
   assert(!F->isDeclared() &&
          "Cannot construct a post dominator tree for a declaration");
   recalculate(*F);
}

bool
PostDominanceInfo::properlyDominates(il::Instruction *I1, il::Instruction *I2)
{
   il::BasicBlock *BB1 = I1->getParent(), *BB2 = I2->getParent();

   // If the blocks are different, it's as easy as whether BB1 post dominates
   // BB2.
   if (BB1 != BB2)
      return properlyDominates(BB1, BB2);

   // Otherwise, they're in the same block, and we just need to check
   // whether A comes after B.
   for (auto II = I1->getIterator(), IE = BB1->end(); II != IE; ++II) {
      if (&*II == I2) {
         return false;
      }
   }

   return true;
}

void PostDominanceInfo::verify() const
{
   // Recompute.
   //
   // Even though at the IL level we have "one" return function, we can have
   // multiple exits provided by no-return functions.
   auto *F = getRoots()[0]->getParent();
   PostDominanceInfo OtherDT(F);

   // And compare.
   if (errorOccurredOnComparison(OtherDT)) {
      llvm::errs() << "PostDominatorTree is not up to date!\nComputed:\n";
      print(llvm::errs());
      llvm::errs() << "\nActual:\n";
      OtherDT.print(llvm::errs());
      abort();
   }
}

} // namespace il
} // namespace cdot