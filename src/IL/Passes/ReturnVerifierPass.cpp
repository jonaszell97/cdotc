//
// Created by Jonas Zell on 22.11.17.
//

#include "../../Variant/Variant.h"

#include "ReturnVerifierPass.h"
#include "../../Message/Diagnostics.h"

#include "../Value/Function/BasicBlock.h"
#include "../Value/Instruction/CallInst.h"
#include "../Value/Instruction/Terminator/TerminatorInst.h"
#include "../Value/Instruction/ControlFlow/ControlFlowInst.h"

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace il {

void ReturnVerifierPass::visitModule(Module &M)
{
   for (auto it = M.begin(); it != M.end(); ++it)
      visitFunction(*it);
}

void ReturnVerifierPass::visitFunction(Function &F)
{
   if (F.getReturnType()->isVoidType() || F.isDeclared()
       || F.getName() == "main" || isa<Initializer>(F)) {
      return;
   }

   visitBasicBlock(*F.begin());
   VisitedBasicBlocks.clear();
}

void ReturnVerifierPass::visitBasicBlock(BasicBlock &B)
{
   if (B.hasNoPredecessors()) {
      return;
   }
   if (VisitedBasicBlocks.find(&B) != VisitedBasicBlocks.end()) {
      return;
   }

   VisitedBasicBlocks.insert(&B);

   if (!B.getTerminator() || B.getInstructions().empty()) {
      diagnoseError(*B.getParent());
   }

   auto Term = B.getTerminator();
   if (isa<RetInst>(Term) || isa<UnreachableInst>(Term)
       || isa<ThrowInst>(Term)) {
      return;
   }

   if (auto Br = dyn_cast<BrInst>(Term)) {
      visitBasicBlock(*Br->getTargetBranch());
      if (auto Else = Br->getElseBranch()) {
         visitBasicBlock(*Else);
      }

      return;
   }

   if (auto Invoke = dyn_cast<InvokeInst>(Term)) {
      visitBasicBlock(*Invoke->getNormalContinuation());
      visitBasicBlock(*Invoke->getLandingPad());
   }

   if (auto Switch = dyn_cast<SwitchInst>(Term)) {
      for (const auto &C : Switch->getCases()) {
         visitBasicBlock(*C.second);
      }

      return;
   }

   llvm_unreachable("bad terminator kind");
}

void ReturnVerifierPass::diagnoseError(Function const &F)
{
   diag::err(err_not_all_code_paths_return)
      << F.getLocation()->getLocation() << diag::term;
}

} // namespace il
} // namespace cdot