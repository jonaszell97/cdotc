#ifndef CDOT_VERIFIERPASS_H
#define CDOT_VERIFIERPASS_H

#include "cdotc/IL/Passes/InstructionVisitor.h"
#include "cdotc/IL/Passes/Passes.h"

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace il {

class VerifierPass : public FunctionPass,
                     public InstructionVisitor<VerifierPass, void> {
public:
   explicit VerifierPass() : FunctionPass(PassKind::VerifierPassID) {}

   void run() override;
   bool isValid() const { return IsValid; }

   void visitGlobals(Module& M);

   friend InstructionVisitor;

private:
   void visitBasicBlock(BasicBlock const& B);
   void visitGlobalVariable(GlobalVariable const& G);

#define CDOT_INSTRUCTION(Name) void visit##Name(Name const& I);
#include "cdotc/IL/Instructions.def"

   bool IsValid = true;

   void emitErrorMessage(llvm::StringRef msg);
   void emitError(Instruction const& I);
   void emitError(Function const& F);
   void emitError(GlobalVariable const& G);
   void emitError(BasicBlock const& B);

   void checkOperandAccessibility(il::Instruction const& I);

   template<class T> void emitErrors(T const& val)
   {
      emitError(val);
      llvm::outs() << "\n\n";
   }

   template<class T, class... Ts>
   void emitErrors(T const& val, Ts const&... rest)
   {
      emitError(val);
      llvm::outs() << "\n\n";
      emitErrors(rest...);
   }

   template<class... Ts>
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

#endif // CDOT_VERIFIERPASS_H
