#ifndef CDOT_UNSAFEANALYSIS_H
#define CDOT_UNSAFEANALYSIS_H

#include "cdotc/IL/Analysis/Analysis.h"

namespace cdot {
namespace il {

class Instruction;

class UnsafeAnalysisInfo {
   llvm::DenseMap<const il::Instruction*, bool> Results;

   void computeFn(Function& F, DominanceAnalysis* DA);

public:
   explicit UnsafeAnalysisInfo(Function& F, DominanceAnalysis* DA);

   bool isUnsafe(const Instruction& I);
   bool isUnsafe(const Instruction* I) { return isUnsafe(*I); }

   void reset() { Results.clear(); }
};

class UnsafeAnalysis : public FunctionAnalysisBase<UnsafeAnalysisInfo> {
   DominanceAnalysis* DA = nullptr;

public:
   UnsafeAnalysis() : FunctionAnalysisBase(AnalysisKind::UnsafeAnalysisID) {}

   UnsafeAnalysis(const UnsafeAnalysis&) = delete;
   UnsafeAnalysis& operator=(const UnsafeAnalysis&) = delete;

   static bool classof(const Analysis* S)
   {
      return S->getKind() == AnalysisKind::UnsafeAnalysisID;
   }

   void initialize(PassManager* PM) override;

   UnsafeAnalysisInfo* newFunctionAnalysis(Function* F) override
   {
      return new UnsafeAnalysisInfo(*F, DA);
   }

   bool shouldInvalidate(InvalidationKind K) override
   {
      return (K & InvalidationKind::Instructions) != 0;
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_UNSAFEANALYSIS_H
