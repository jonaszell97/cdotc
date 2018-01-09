//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_VERIFIERPASS_H
#define CDOT_VERIFIERPASS_H

#include <llvm/Support/raw_ostream.h>
#include "InstructionVisitor.h"

namespace cdot {
namespace il {

class VerifierPass: public InstructionVisitor<VerifierPass, void>  {
public:
   explicit VerifierPass()
      : InstructionVisitor(VerifierPassID)
   {}

   void visitModule(Module const& M);
   void visitFunction(Function const& F);
   void visitBasicBlock(BasicBlock const& B);
   void visitAggregateType(AggregateType const& Ty);
   void visitGlobalVariable(GlobalVariable const &G);

#  define CDOT_INSTRUCTION(Name) \
   void visit##Name(Name const& I);
#  include "../Value/Instructions.def"

private:
   bool isValid = true;

   void emitErrorMessage(llvm::StringRef msg);
   void emitError(Instruction const& I);
   void emitError(Function const& F);
   void emitError(GlobalVariable const& G);
   void emitError(AggregateType const& Ty);
   void emitError(BasicBlock const& B);

   using BBPtrSet = llvm::SmallPtrSet<il::BasicBlock const*, 16>;

   void checkOperandAccessibility(il::Instruction const& I);
   bool checkOperandAccessibility(il::BasicBlock const& B, il::Value const*V,
                                  BBPtrSet &Visited);

   template<class T>
   void emitErrors(T const& val)
   {
      emitError(val);
      llvm::outs() << "\n\n";
   }

   template<class T, class ...Ts>
   void emitErrors(T const& val, Ts const&... rest)
   {
      emitError(val);
      llvm::outs() << "\n\n";
      emitErrors(rest...);
   }

   template<class ...Ts>
   void errorIf(bool cond, llvm::StringRef msg, Ts const&... val)
   {
      if (cond) {
         isValid = false;

         emitErrorMessage(msg);
         emitErrors(val...);
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_VERIFIERPASS_H
