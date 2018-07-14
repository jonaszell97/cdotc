//
// Created by Jonas Zell on 13.05.18.
//

#include "StackPromotion.h"

#include "AST/Decl.h"
#include "IL/Analysis/EscapeAnalysis.h"
#include "IL/Passes/Passes.h"
#include "IL/Passes/DataflowProblem.h"
#include "IL/Passes/InstructionVisitor.h"
#include "ILGen/ILGenPass.h"

#include <llvm/ADT/BitVector.h>

using namespace cdot::support;
using llvm::BitVector;

namespace cdot {
namespace il {

StackPromotion::StackPromotion(cdot::ast::ILGenPass &ILGen)
   : FunctionPass(PassKind::StackPromotionID),
     ILGen(ILGen), Builder(ILGen.Builder)
{

}

void StackPromotion::removeRelease(Value *V, StructInitInst *Alloc)
{
   if (auto Val = dyn_cast<ReleaseInst>(V)) {
      Builder.SetInsertPoint(Val->getIterator());
      Builder.CreateCall(ILGen.getFunc(Alloc->getInitializedType()
                                            ->getDeinitializer()),
                         { Alloc });

      InstsToDestroy.insert(Val);
      return;
   }

   if (auto Store = dyn_cast<InitInst>(V)) {
      assert(isa<AllocaInst>(Store->getDst()));
      for (auto use : Store->getDst()->getUses()) {
         if (use->getUser() != Store)
            removeRelease(use->getUser(), Alloc);
      }
   }

   if (auto Ld = dyn_cast<LoadInst>(V)) {
      if (auto Use = Ld->getSingleUser()) {
         removeRelease(Use, Alloc);
      }
   }
}

#ifndef NDEBUG

bool verifyNonEscapingUse(Value *Use)
{
   assert(!isa<RetainInst>(Use) && "retaining non-escaping value?");
   return true;
}

#endif

void StackPromotion::run()
{
   for (auto &B : *F) {
      for (auto &I : B) {
         if (auto Init = dyn_cast<StructInitInst>(&I)) {
            if (Init->isHeapAllocated()) {
               HeapAllocs.try_emplace(Init, HeapAllocs.size());
            }
         }
      }
   }

   EscapeAnalysis *EA = PM->getAnalysis<EscapeAnalysis>();
   auto IP = Builder.saveIP();

   for (auto &Alloc : HeapAllocs) {
      if (EA->doesEscape(Alloc.getFirst()))
         continue;

      Alloc.getFirst()->setHeapAllocated(false);

      // replace releases with destructor call
      for (auto use : Alloc.getFirst()->getUses()) {
         assert(verifyNonEscapingUse(use->getUser()));
         removeRelease(use->getUser(), Alloc.getFirst());
      }
   }

   removeReleases();
   Builder.restoreIP(IP);

   HeapAllocs.clear();
   InstsToDestroy.clear();
}

} // namespace il
} // namespace cdot