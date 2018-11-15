//
// Created by Jonas Zell on 23.05.18.
//

#include "Passes.h"

#include "BorrowCheckPass.h"
#include "Driver/Compiler.h"
#include "DefinitiveInitializationPass.h"
#include "FinalizeFunctionPass.h"
#include "IL/Context.h"
#include "IL/Analysis/Dominance.h"
#include "IL/Analysis/EscapeAnalysis.h"
#include "IL/Analysis/UnsafeAnalysis.h"
#include "IL/Transforms/StackPromotion.h"
#include "PassManager.h"
#include "ReorderBasicBlockPass.h"
#include "VerifierPass.h"

#include <llvm/Support/CommandLine.h>

namespace cl = llvm::cl;

static cl::opt<bool> VerifyILFunctions("verify-il",
                                       cl::desc("Verify IL functions (for "
                                                "compiler debugging purposes "
                                                "only)"),
                                       cl::init(false));

namespace cdot {
namespace il {

BorrowCheckPass *createBorrowCheckPass(PassManager &PM)
{
   return new BorrowCheckPass(PM.getModule()->getContext().getCompilation()
                              .getILGen());
}

DefinitiveInitializationPass*
createDefinitiveInitializationPass(PassManager &PM)
{
   return new DefinitiveInitializationPass(
      PM.getModule()->getContext().getCompilation().getILGen());
}

ReorderBasicBlockPass *createReorderBasicBlockPass(PassManager &PM)
{
   return new ReorderBasicBlockPass();
}

FinalizeFunctionPass *createFinalizeFunctionPass(PassManager &PM)
{
   return new FinalizeFunctionPass(
      PM.getModule()->getContext().getCompilation().getILGen());
}

VerifierPass *createVerifierPass(PassManager&)
{
   return new VerifierPass;
}

StackPromotion *createStackPromotion(PassManager &PM)
{
   return new StackPromotion(
      PM.getModule()->getContext().getCompilation().getILGen());
}

void addMandatoryPasses(PassManager &PM)
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

void addOptimizationPasses(CompilerOptions &Opts, PassManager &PM)
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