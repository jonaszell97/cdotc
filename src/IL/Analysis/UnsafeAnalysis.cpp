#include "cdotc/IL/Analysis/UnsafeAnalysis.h"

#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/IL/Passes/PassManager.h"
#include "cdotc/IL/Utils/BlockIterator.h"

namespace cdot {
namespace il {

void UnsafeAnalysis::initialize(cdot::il::PassManager* PM)
{
   DA = PM->getAnalysis<DominanceAnalysis>();
}

UnsafeAnalysisInfo::UnsafeAnalysisInfo(il::Function& F, DominanceAnalysis* DA)
{
   computeFn(F, DA);
}

void UnsafeAnalysisInfo::computeFn(il::Function& F, DominanceAnalysis* DA)
{
   for (auto& B : F) {
      for (auto& I : B) {
         if (I.isEndUnsafe()) {
            auto* End = support::cast<IntrinsicCallInst>(&I);
            auto* Begin
                = support::cast<IntrinsicCallInst>(End->getArgs().front());

            (void)DA;

#ifndef NDEBUG
            auto* DI = DA->get(I.getParent()->getParent());
            assert(DI->dominates(Begin, &I)
                   && "bad placement of begin/end unsafe "
                      "instructions");
#endif

            SmallPtrSet<BasicBlock*, 8> Visited;
            SmallPtrSet<BasicBlock*, 8> Worklist{I.getParent()};
            Instruction* Inst = &I;

            while (!Worklist.empty()) {
               auto* BB = *Worklist.begin();
               Worklist.erase(BB);

               if (!Visited.insert(BB).second)
                  continue;

               if (!Inst)
                  Inst = &BB->getInstructions().back();

               bool done = false;
               while (Inst) {
                  if (Inst == Begin) {
                     done = true;
                     Inst = nullptr;

                     break;
                  }

                  Results[Inst] = true;
                  Inst = Inst->getPrevNode();
               }

               if (done)
                  continue;

               for (auto Pred : getPredecessors(BB)) {
                  Worklist.insert(Pred);
               }
            }
         }
      }
   }
}

bool UnsafeAnalysisInfo::isUnsafe(const il::Instruction& I)
{
   return Results[&I];
}

} // namespace il
} // namespace cdot