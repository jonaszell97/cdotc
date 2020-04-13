#include "FinalizeFunctionPass.h"

#include "IL/Analysis/AccessPathDescriptor.h"
#include "IL/Instructions.h"
#include "ILGen/ILGenPass.h"
#include "Sema/SemaPass.h"

using namespace cdot::support;

namespace cdot {
namespace il {

void FinalizeFunctionPass::run()
{
   llvm::SmallPtrSet<AssignInst*, 8> Assigns;
   for (auto &B : *F) {
      for (auto &I : B) {
         if (auto Assign = dyn_cast<AssignInst>(&I)) {
            Assigns.insert(Assign);
         }

         visit(I);
      }
   }

   auto &Builder = ILGen.Builder;
   for (auto Assign : Assigns) {;
      Builder.SetInsertPoint(Assign->getIterator());

      auto Ld = Builder.CreateLoad(Assign->getDst());
      Ld->setSynthesized(true);

      DefaultCleanup(Assign->getSrc()).Emit(ILGen);

      if (!Assign->getDst()->getType()->isMutableReferenceType()) {
         ILGen.getSema().diagnose(diag::err_reassign_constant,
                                  Assign->getSourceLoc());
      }

      auto Store = Builder.CreateStore(Assign->getSrc(), Assign->getDst());
      Store->setTagged(Assign->isTagged());
      Store->setMemoryOrder(Assign->getMemoryOrder());

      Assign->replaceAllUsesWith(Store);
      Assign->detachAndErase();
   }
}

void FinalizeFunctionPass::visitMoveInst(const il::MoveInst &I)
{
   if (!I.getType()->isMutableReferenceType()) {
      ILGen.getSema().diagnose(diag::err_cannot_move_immutable,
                               getAccessPathDescriptor(ILGen, I),
                               I.getSourceLoc());
   }
}

} // namespace il
} // namespace cdot