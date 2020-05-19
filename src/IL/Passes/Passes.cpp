#include "cdotc/IL/Passes/Passes.h"

#include "cdotc/Driver/Compiler.h"
#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Analysis/UnsafeAnalysis.h"
#include "cdotc/IL/Context.h"
#include "cdotc/IL/Passes/BorrowCheckPass.h"
#include "cdotc/IL/Passes/DefinitiveInitializationPass.h"
#include "cdotc/IL/Passes/FinalizeFunctionPass.h"
#include "cdotc/IL/Passes/PassManager.h"
#include "cdotc/IL/Passes/ReorderBasicBlockPass.h"
#include "cdotc/IL/Passes/VerifierPass.h"
#include "cdotc/IL/Transforms/StackPromotion.h"

#include <llvm/Support/CommandLine.h>

#ifndef NDEBUG
#define DEFAULT_VERIFY true
#else
#define DEFAULT_VERIFY false
#endif

namespace cl = llvm::cl;

static cl::opt<bool> VerifyILFunctions("verify-il",
                                       cl::desc("Verify IL functions (for "
                                                "compiler debugging purposes "
                                                "only)"),
                                       cl::init(DEFAULT_VERIFY));

namespace cdot {
namespace il {

BorrowCheckPass* createBorrowCheckPass(PassManager& PM)
{
   return new BorrowCheckPass(
       PM.getModule()->getContext().getCompilation().getILGen());
}

DefinitiveInitializationPass*
createDefinitiveInitializationPass(PassManager& PM)
{
   return new DefinitiveInitializationPass(
       PM.getModule()->getContext().getCompilation().getILGen());
}

ReorderBasicBlockPass* createReorderBasicBlockPass(PassManager& PM)
{
   return new ReorderBasicBlockPass();
}

FinalizeFunctionPass* createFinalizeFunctionPass(PassManager& PM)
{
   return new FinalizeFunctionPass(
       PM.getModule()->getContext().getCompilation().getILGen());
}

VerifierPass* createVerifierPass(PassManager&) { return new VerifierPass; }

StackPromotion* createStackPromotion(PassManager& PM)
{
   return new StackPromotion(
       PM.getModule()->getContext().getCompilation().getILGen());
}

void addMandatoryPasses(PassManager& PM)
{
   if (VerifyILFunctions) {
      PM.addPass(PassKind::VerifierPassID);
   }

   PM.addPass(PassKind::DefinitiveInitializationPassID);
   PM.addPass(PassKind::BorrowCheckPassID);
   PM.addPass(PassKind::ReorderBasicBlockPassID);
   PM.addPass(PassKind::FinalizeFunctionPassID);

   PM.addAnalysis(AnalysisKind::DominanceAnalysisID);
   PM.addAnalysis(AnalysisKind::UnsafeAnalysisID);

   PM.initializeAnalyses();
   PM.isMandatoryPipeline = true;
}

void addOptimizationPasses(CompilerOptions& Opts, PassManager& PM)
{
   auto Lvl = Opts.optimizationLevelAsNumber();
   if (Lvl >= 1) {
      PM.addPass(PassKind::StackPromotionID);
   }
   if (Lvl >= 2) {
   }
   if (Lvl == 3) {
   }

   PM.initializeAnalyses();
   PM.addAnalysis(AnalysisKind::EscapeAnalysisID);
}

} // namespace il
} // namespace cdot