#include "cdotc/IL/Passes/PassManager.h"

#include "cdotc/IL/Analysis/AnalysisKinds.h"
#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Analysis/EscapeAnalysis.h"
#include "cdotc/IL/Analysis/UnsafeAnalysis.h"
#include "cdotc/IL/Passes/BorrowCheckPass.h"
#include "cdotc/IL/Passes/DefinitiveInitializationPass.h"
#include "cdotc/IL/Passes/FinalizeFunctionPass.h"
#include "cdotc/IL/Passes/PassKinds.h"
#include "cdotc/IL/Passes/ReorderBasicBlockPass.h"
#include "cdotc/IL/Passes/VerifierPass.h"
#include "cdotc/IL/Transforms/StackPromotion.h"

using namespace cdot::support;

namespace cdot {
namespace il {

PassManager::PassManager(il::Module* M, bool isMandatoryPipeline)
    : M(M), isMandatoryPipeline(isMandatoryPipeline)
{
}

bool PassManager::continueTransforming()
{
   if (isMandatoryPipeline)
      return true;

   return true;
}

bool PassManager::analysesUnlocked()
{
   for (auto* A : Analyses)
      if (A->isLocked())
         return false;

   return true;
}

void PassManager::runPassOnFunction(unsigned TransIdx, il::Function* F)
{
   assert(analysesUnlocked() && "Expected all analyses to be unlocked!");

   auto* SFT = cast<FunctionPass>(Passes[TransIdx]);
   SFT->setPassManager(this);
   SFT->setFunction(F);

   SFT->run();

   ++NumPassesRun;
}

void PassManager::runFunctionPasses(unsigned FromTransIdx, unsigned ToTransIdx)
{
   if (ToTransIdx <= FromTransIdx)
      return;

   for (auto& F : M->getFuncList()) {
      // Only include functions that are definitions, and which have not
      // been intentionally excluded from optimization.
      if (!F.isDeclared() && !F.isInvalid())
         FunctionWorklist.emplace_back(&F);
   }

   // The maximum number of times the pass pipeline can be restarted for a
   // function. This is used to ensure we are not going into an infinite loop in
   // cases where (for example) we have recursive type-based specialization
   // happening.
   const unsigned MaxNumRestarts = 20;

   // Run all transforms for all functions, starting at the tail of the
   // allDecls.
   while (!FunctionWorklist.empty() && continueTransforming()) {
      unsigned TailIdx = (unsigned)FunctionWorklist.size() - 1;
      unsigned PipelineIdx = FunctionWorklist[TailIdx].PipelineIdx;
      Function* F = FunctionWorklist[TailIdx].F;

      if (PipelineIdx >= (ToTransIdx - FromTransIdx)) {
         // All passes did already run for the function. Pop it off the
         // allDecls.
         FunctionWorklist.pop_back();
         continue;
      }

      assert(!shouldRestartPipeline()
             && "Did not expect function pipeline set up to restart from "
                "beginning!");

      runPassOnFunction(FromTransIdx + PipelineIdx, F);

      // Note: Don't get entry reference prior to runPassOnFunction().
      // A pass can push a new function to the allDecls which may cause a
      // reallocation of the buffer and that would invalidate the reference.
      WorklistEntry& Entry = FunctionWorklist[TailIdx];
      if (shouldRestartPipeline() && Entry.NumRestarts < MaxNumRestarts) {
         ++Entry.NumRestarts;
         Entry.PipelineIdx = 0;
      }
      else {
         ++Entry.PipelineIdx;
      }

      clearRestartPipeline();
   }
}

void PassManager::runModulePass(unsigned TransIdx)
{
   auto* SMT = cast<ModulePass>(Passes[TransIdx]);
   SMT->setPassManager(this);
   SMT->setModule(M);

   CurrentPassHasInvalidated = false;
   assert(analysesUnlocked() && "Expected all analyses to be unlocked!");

   SMT->run();

   assert(analysesUnlocked() && "Expected all analyses to be unlocked!");
}

void PassManager::execute()
{
   // Run the transforms by alternating between function transforms and
   // module transforms. We'll queue up all the function transforms
   // that we see in a row and then run the entire group of transforms
   // on each function in turn. Then we move on to running the next set
   // of consecutive module transforms.
   unsigned Idx = 0, NumTransforms = (unsigned)Passes.size();

   while (Idx < NumTransforms && continueTransforming()) {
      ILPass* Tr = Passes[Idx];
      assert((isa<FunctionPass>(Tr) || isa<ModulePass>(Tr))
             && "Unexpected pass kind!");

      unsigned FirstFuncTrans = Idx;
      while (Idx < NumTransforms && isa<FunctionPass>(Passes[Idx]))
         ++Idx;

      runFunctionPasses(FirstFuncTrans, Idx);

      while (Idx < NumTransforms && isa<ModulePass>(Passes[Idx])
             && continueTransforming()) {
         runModulePass(Idx);

         ++Idx;
         ++NumPassesRun;
      }
   }
}

void PassManager::runPassesOnFunction(il::Function& F)
{
   assert(isMandatoryPipeline && "only call this with empty allDecls!");

   for (auto P : Passes) {
      if (auto FP = dyn_cast<FunctionPass>(P)) {
         FP->setPassManager(this);
         FP->setFunction(&F);
         FP->run();
      }
   }
}

PassManager::~PassManager()
{
   for (auto P : Passes)
      delete P;

   for (auto A : Analyses)
      delete A;
}

void PassManager::addFunctionToWorklist(Function* F)
{
   assert(F && !F->isDeclared() && isMandatoryPipeline
          && "Expected optimizable function definition!");

   FunctionWorklist.emplace_back(F);
}

void PassManager::restartWithCurrentFunction(ILPass* T)
{
   assert(isa<FunctionPass>(T)
          && "Can only restart the pipeline from function passes");
   RestartPipeline = true;
}

void PassManager::resetAndRemovePasses()
{
   for (auto* T : Passes)
      delete T;

   Passes.clear();
}

void PassManager::addPass(il::PassKind Kind)
{
   switch (Kind) {
#define CDOT_PASS(CLASS, NAME)                                                 \
   case PassKind::CLASS##ID:                                                   \
      Passes.push_back(create##CLASS(*this));                                  \
      break;
#include "cdotc/IL/Passes/Passes.def"

   default:
      llvm_unreachable("bad pass kind");
   }
}

void PassManager::addAnalysis(cdot::il::AnalysisKind Kind)
{
   switch (Kind) {
#define CDOT_ANALYSIS(CLASS, NAME)                                             \
   case AnalysisKind::CLASS##ID:                                               \
      Analyses.push_back(create##CLASS(*this));                                \
      break;
#include "cdotc/IL/Analysis/Analysis.def"
   }
}
void PassManager::initializeAnalyses()
{
   for (auto A : Analyses)
      A->initialize(this);
}

} // namespace il
} // namespace cdot