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
   {}

   void visitModule(Module& M) override;
   void visitFunction(Function const& F);
   void visitBasicBlock(BasicBlock const& B);
   void visitGlobalVariable(GlobalVariable const &G);

#  define CDOT_INSTRUCTION(Name) \
   void visit##Name(Name const& I);
#  include "IL/Instructions.def"

   bool isValid() const { return IsValid; }

private:
   bool IsValid = true;

   void emitErrorMessage(llvm::StringRef msg);
   void emitError(Instruction const& I);
   void emitError(Function const& F);
   void emitError(GlobalVariable const& G);
   void emitError(BasicBlock const& B);

   void checkOperandAccessibility(il::Instruction const& I);

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
         IsValid = false;

         emitErrorMessage(msg);
         emitErrors(val...);
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_VERIFIERPASS_H
