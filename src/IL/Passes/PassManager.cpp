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
      switch (P->getTypeID()) {
#     define CDOT_ALL(Name) \
         case InstVisitorBase::Name##ID: \
            static_cast<Name*>(P)->visitModule(*M); \
            delete static_cast<Name*>(P); break;
#     include "Passes.def"
         default:
            llvm_unreachable("bad pass kind");
      }
   }
}

} // namespace il
} // namespace cdot