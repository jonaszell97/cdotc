#ifndef CDOT_REORDERBASICBLOCKPASS_H
#define CDOT_REORDERBASICBLOCKPASS_H

#include "cdotc/IL/Passes/Passes.h"

namespace cdot {
namespace il {

class ReorderBasicBlockPass : public FunctionPass {
public:
   ReorderBasicBlockPass();
   void run() override;

   static bool classof(ILPass const* P)
   {
      return P->getKind() == PassKind::ReorderBasicBlockPassID;
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_REORDERBASICBLOCKPASS_H
