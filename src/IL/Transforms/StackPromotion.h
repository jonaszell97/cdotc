//
// Created by Jonas Zell on 13.05.18.
//

#ifndef CDOT_STACKPROMOTION_H
#define CDOT_STACKPROMOTION_H

#include "IL/Passes/InstructionVisitor.h"
#include "IL/Passes/Passes.h"

#include <llvm/ADT/BitVector.h>

namespace cdot {
namespace ast {
   class ILGenPass;
}

namespace il {

class StackPromotion: public FunctionPass,
                      public InstructionVisitor<StackPromotion, void> {
   ast::ILGenPass &ILGen;
   il::ILBuilder &Builder;

   /// Map of heap allocations in the function
   llvm::DenseMap<il::StructInitInst*, unsigned> HeapAllocs;

   /// Instructions to destroy
   llvm::SmallPtrSet<Instruction*, 8> InstsToDestroy;

   void removeRelease(Value *V, StructInitInst *Alloc);
   void removeReleases()
   {
      for (auto Inst : InstsToDestroy)
         Inst->detachAndErase();
   }

public:
   explicit StackPromotion(ast::ILGenPass &ILGen);

   void run() override;

   static bool classof(ILPass const *P)
   { return P->getKind() == PassKind::StackPromotionID; }

   friend InstructionVisitor;
};

} // namespace il
} // namespace cdot

#endif //CDOT_STACKPROMOTION_H
