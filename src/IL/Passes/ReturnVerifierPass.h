//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_CFANALYSISPASS_H
#define CDOT_CFANALYSISPASS_H


#include "InstructionVisitor.h"

namespace cdot {
namespace il {

class ReturnVerifierPass: public InstructionVisitor<ReturnVerifierPass, void> {
public:
   ReturnVerifierPass() : InstructionVisitor(ReturnVerifierPassID)
   {

   }

   void visitModule(Module &M);
   void visitFunction(Function &F);
   void visitBasicBlock(BasicBlock &B);

   void diagnoseError(Function const &F);

protected:
   llvm::SmallPtrSet<BasicBlock*, 8> VisitedBasicBlocks;
};

} // namespace il
} // namespace cdot


#endif //CDOT_CFANALYSISPASS_H
