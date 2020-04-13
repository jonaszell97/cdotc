#ifndef CDOT_ELIMINATEASSIGNPASS_H
#define CDOT_ELIMINATEASSIGNPASS_H

#include "cdotc/IL/Passes/InstructionVisitor.h"
#include "cdotc/IL/Passes/Passes.h"

namespace cdot {
namespace ast {
class ILGenPass;
} // namespace ast

namespace il {

class FinalizeFunctionPass : public FunctionPass,
                             public InstructionVisitor<FinalizeFunctionPass> {
   ast::ILGenPass& ILGen;

public:
   explicit FinalizeFunctionPass(ast::ILGenPass& ILGen)
       : FunctionPass(PassKind::FinalizeFunctionPassID), ILGen(ILGen)
   {
   }

   void run() override;

   static bool classof(ILPass const* P)
   {
      return P->getKind() == PassKind::FinalizeFunctionPassID;
   }

   friend InstructionVisitor;

private:
   void visitMoveInst(const MoveInst& I);
};

} // namespace il
} // namespace cdot

#endif // CDOT_ELIMINATEASSIGNPASS_H
