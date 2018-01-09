//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_PRIMITIVEFOLDERPASS_H
#define CDOT_PRIMITIVEFOLDERPASS_H

#include <queue>

#include "InstructionVisitor.h"
#include "../ILBuilder.h"

namespace cdot {
namespace il {

class PrimitivePromoter: public InstructionVisitor<PrimitivePromoter, Value*> {
public:
   explicit PrimitivePromoter(Module *M)
      : InstructionVisitor(PrimitivePromoterID), Builder(M)
   {}

   void visitModule(Module &M);
   void visitFunction(Function &F);
   void visitBasicBlock(BasicBlock &B);

   Value *visit(Instruction &I);

   Instruction *getUnaryInstForMethod(Method *M, Value *lhs);
   Instruction *getBinaryInstForMethod(Method *M, Value *lhs, Value *rhs);

   Value* HandleBinaryInstruction(CallInst &I);
   Value* HandleUnaryInstruction(CallInst &I);

   Value *visitCallInst(CallInst &I);
   Value *visitInitInst(InitInst &I);

   Value *visitLoadInst(LoadInst &I);
   Value *visitIntegerCastInst(IntegerCastInst &I);
   Value *visitFPCastInst(FPCastInst &I);

protected:
   ILBuilder Builder;
   std::queue<Instruction*> WorkList;
};


} // namespace il
} // namespace cdot

#endif //CDOT_PRIMITIVEFOLDERPASS_H
