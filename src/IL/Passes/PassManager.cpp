//
// Created by Jonas Zell on 22.11.17.
//

#include "PassManager.h"

#include "InstructionVisitor.h"
#include "ReturnVerifierPass.h"
#include "PrimitivePromoter.h"
#include "VerifierPass.h"
#include "IRGen/IRGen.h"

namespace cdot {
namespace il {

void PassManager::runPasses()
{
   for (const auto &P : Passes) {
      P->visitModule(*M);
   }
}

} // namespace il
} // namespace cdot