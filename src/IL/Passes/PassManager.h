#ifndef CDOT_ILPASSMANAGER_H
#define CDOT_ILPASSMANAGER_H

#include "IL/Function.h"
#include "IL/Analysis/Analysis.h"
#include "Support/Casting.h"
#include "PassKinds.h"

#include <llvm/ADT/SmallVector.h>

#include <bitset>

namespace cdot {

struct CompilerOptions;

namespace il {

class FunctionPass;
class ILPass;
class Module;

class PassManager {
   /// The module that this pass manager belongs to
   Module *M;

   /// List of passes added to this pass manager
   llvm::SmallVector<ILPass*, 16> Passes;

   /// List of analyses added to this pass manager
   llvm::SmallVector<Analysis*, 16> Analyses;

   struct WorklistEntry {
      WorklistEntry(Function *F)
         : F(F)
      {}

      Function *F;
      unsigned PipelineIdx = 0;
      unsigned NumRestarts = 0;
   };

   /// The worklist of functions to be processed by function passes.
   std::vector<WorklistEntry> FunctionWorklist;

   /// A mask which has one bit for each pass. A one for a pass-bit means that
   /// the pass doesn't need to run, because nothing has changed since the
   /// previous run of that pass.
   using CompletedPasses = std::bitset<(size_t)1>;

   /// A completed-passes mask for each function.
   llvm::DenseMap<Function *, CompletedPasses> CompletedPassesMap;

   /// The number of passes run so far.
   unsigned NumPassesRun = 0;

   /// Set to true when a pass invalidates an analysis.
   bool CurrentPassHasInvalidated = false;
   
   /// True if we need to stop running passes and restart again on the
   /// same function.
   bool RestartPipeline = false;

   /// True if this is a mandatory pass pipeline
   bool isMandatoryPipeline;

public:
   explicit PassManager(Module *M = nullptr, bool isMandatoryPipeline = false);

   /// \brief Searches for an analysis of type T in the list of registered
   /// analysis. If the analysis is not found, the program terminates.
   template<typename T>
   T *getAnalysis()
   {
      for (Analysis *A : Analyses)
         if (auto *R = support::dyn_cast<T>(A))
            return R;

      llvm_unreachable("Unable to find analysis for requested type.");
   }

   /// \returns the module that the pass manager owns.
   Module *getModule() { return M; }
   void setModule(Module *M) { this->M = M; }

   unsigned getNumRegisteredPasses() const { return (unsigned)Passes.size(); }
   unsigned getNumRegisteredAnalyses() const
   { return (unsigned)Analyses.size(); }

   /// \brief Restart the function pass pipeline on the same function
   /// that is currently being processed.
   void restartWithCurrentFunction(ILPass *T);
   void clearRestartPipeline() { RestartPipeline = false; }
   bool shouldRestartPipeline() { return RestartPipeline; }

   /// \brief Iterate over all analysis and invalidate them.
   void invalidateAllAnalyses()
   {
      // Invalidate the analysis (unless they are locked)
      for (auto AP : Analyses)
         if (!AP->isLocked())
            AP->invalidate();

      CurrentPassHasInvalidated = true;

      // Assume that all functions have changed. Clear all masks of all functions.
      CompletedPassesMap.clear();
   }

   /// \brief Add the function \p F to the function pass worklist.
   /// If not null, the function \p DerivedFrom is the function from which \p F
   /// is derived. This is used to avoid an infinite amount of functions pushed
   /// on the worklist (e.g. caused by a bug in a specializing optimization).
   void addFunctionToWorklist(Function *F);

   /// \brief Iterate over all analysis and notify them of the function.
   /// This function does not necessarily have to be newly created function. It
   /// is the job of the analysis to make sure no extra work is done if the
   /// particular analysis has been done on the function.
   void notifyAnalysisOfFunction(Function *F) {
      for (auto AP : Analyses)
         AP->notifyAddFunction(F);
   }

   /// \brief Broadcast the invalidation of the function to all analysis.
   void invalidateAnalysis(Function *F,
                           Analysis::InvalidationKind K) {
      // Invalidate the analysis (unless they are locked)
      for (auto AP : Analyses)
         if (!AP->isLocked())
            AP->invalidate(F, K);

      CurrentPassHasInvalidated = true;
      // Any change let all passes run again.
      CompletedPassesMap[F].reset();
   }

   /// \brief Iterate over all analysis and notify them of a deleted function.
   void notifyDeleteFunction(Function *F)
   {
      // Invalidate the analysis (unless they are locked)
      for (auto AP : Analyses)
         if (!AP->isLocked())
            AP->notifyDeleteFunction(F);

      CurrentPassHasInvalidated = true;
      // Any change let all passes run again.
      CompletedPassesMap[F].reset();
   }

   /// \brief Reset the state of the pass manager and remove all transformations
   /// owned by the pass manager. Analysis passes will be kept.
   void resetAndRemovePasses();


   /// D'tor.
   ~PassManager();

   /// Verify all analyses.
   void verifyAnalyses() const
   {
      for (auto *A : Analyses) {
         A->verify();
      }
   }

   /// Verify all analyses, limiting the verification to just this one function
   /// if possible.
   ///
   /// Discussion: We leave it up to the analyses to decide how to implement
   /// this. If no override is provided the SILAnalysis should just call the
   /// normal verify method.
   void verifyAnalyses(Function *F) const
   {
      for (auto *A : Analyses) {
         A->verify(F);
      }
   }

   void runPassesOnFunction(Function &F);
   void execute();

   friend void addMandatoryPasses(PassManager &PM);
   friend void addOptimizationPasses(CompilerOptions &Opts, PassManager &PM);

private:
   /// Add a pass of a specific kind.
   void addPass(PassKind Kind);

   /// Add an analysis of a specific kind.
   void addAnalysis(AnalysisKind Kind);
   void initializeAnalyses();

   /// Run the \p TransIdx'th SIL module transform over all the functions in
   /// the module.
   void runModulePass(unsigned TransIdx);

   /// Run the \p TransIdx'th pass on the function \p F.
   void runPassOnFunction(unsigned TransIdx, Function *F);

   /// Run the passes in Transform from \p FromTransIdx to \p ToTransIdx.
   void runFunctionPasses(unsigned FromTransIdx, unsigned ToTransIdx);

   /// A helper function that returns (based on SIL stage and debug
   /// options) whether we should continue running passes.
   bool continueTransforming();

   /// Return true if all analyses are unlocked.
   bool analysesUnlocked();
};

} // namespace il
} // namespace cdot


#endif //CDOT_ILPASSMANAGER_H
