#ifndef CDOT_ESCAPEANALYSIS_H
#define CDOT_ESCAPEANALYSIS_H

#include "cdotc/IL/Analysis/Analysis.h"
#include "cdotc/IL/Analysis/AnalysisKinds.h"
#include "cdotc/IL/Passes/InstructionVisitor.h"

namespace cdot {
namespace il {

class EscapeAnalysis : public Analysis {
   /// Analysis result cache
   llvm::DenseMap<il::Value*, bool> GlobalCache;
   llvm::DenseMap<il::Function*, llvm::DenseMap<il::Value*, bool>> PerFnCache;

   bool visit(Value& V, Value* ValToCheck);

public:
   EscapeAnalysis() : Analysis(AnalysisKind::EscapeAnalysisID) {}

   bool doesEscape(Value* V);
   bool doesEscape(Instruction* I);

   void invalidate(Function* F, InvalidationKind K) override;
   void invalidate() override;

   void notifyAddFunction(Function*) override {}
   void notifyDeleteFunction(Function*) override {}

   static bool classof(Analysis const* A)
   {
      return A->getKind() == AnalysisKind::EscapeAnalysisID;
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_ESCAPEANALYSIS_H
