#ifndef CDOT_ANALYSIS_H
#define CDOT_ANALYSIS_H

#include "AnalysisKinds.h"

#include <llvm/ADT/DenseMap.h>

namespace cdot {
namespace il {

class Function;
class PassManager;

class Analysis {
public:
   enum InvalidationKind : unsigned {
      /// The pass did not change anything.
      Nothing = 0x0,

      /// The pass created, deleted or rearranged some instructions in a
      /// function.
      Instructions = 0x1,

      /// The pass modified some calls (apply instructions).
      ///
      /// The intention of this invalidation kind is to allow analysis that
      /// rely on a specific call graph structure to recompute themselves.
      Calls = 0x2,

      /// A pass has invalidated some branches in the program.
      ///
      /// The intention of this invalidation kind is to tell analyses like the
      /// Dominance Analysis and the PostOrder Analysis that the underlying CFG
      /// has been modified.
      Branches = 0x4,

      /// Convenience states:
      FunctionBody = Calls | Branches | Instructions,
      CallsAndInstructions = Calls | Instructions,
      BranchesAndInstructions = Branches | Instructions,
      Everything = Calls | Branches | Instructions,
   };

private:
   AnalysisKind Kind;
   bool invalidationLock;

public:
   Analysis(AnalysisKind Kind) : Kind(Kind), invalidationLock(false) {}
   virtual ~Analysis() = default;

   AnalysisKind getKind() const { return Kind; }

   /// Can be used to retrieve other analysis passes from \p PM, which this
   /// analysis depends on.
   virtual void initialize(PassManager*) { }

   /// Lock the analysis. This means that invalidation messages are ignored.
   void lockInvalidation() { invalidationLock = true; }

   /// Unlock the analysis. This means that invalidation messages are handled.
   void unlockInvalidation() { invalidationLock = false; }

   /// Return True if this analysis is locked and should not be invalidated.
   bool isLocked() { return invalidationLock; }

   /// Invalidate all information in this analysis.
   virtual void invalidate() = 0;

   /// Invalidate all of the information for a specific function.
   virtual void invalidate(Function *F, InvalidationKind K) = 0;

   /// Notify the analysis about a newly created function.
   virtual void notifyAddFunction(Function *F) = 0;

   /// Notify the analysis about a function which will be deleted from the
   /// module.
   virtual void notifyDeleteFunction(Function *F) = 0;

   /// Verify the state of this analysis.
   virtual void verify() const {}
   virtual void verify(Function*) const { verify(); }
};

// RAII helper for locking analyses.
class AnalysisPreserver {
   Analysis *A;

public:
   explicit AnalysisPreserver(Analysis *A) : A(A)
   {
      A->lockInvalidation();
   }

   ~AnalysisPreserver()
   {
      A->unlockInvalidation();
   }
};

/// An abstract base class that implements the boiler plate of caching and
/// invalidating analysis for specific functions.
template<typename AnalysisTy>
class FunctionAnalysisBase: public Analysis {
protected:
   using StorageTy = llvm::DenseMap<Function*, AnalysisTy*>;

   /// Maps functions to their analysis provider.
   StorageTy Storage;

   /// Construct a new empty analysis for a specific function \p F.
   virtual AnalysisTy *newFunctionAnalysis(Function *F) = 0;

   /// Return True if the analysis should be invalidated given trait \K is
   /// preserved.
   virtual bool shouldInvalidate(Analysis::InvalidationKind K) = 0;

   /// A stub function that verifies the specific AnalysisTy \p A. This is
   /// meant to be overridden by subclasses.
   virtual void verify(AnalysisTy*) const {}

   void deleteAllAnalysisProviders()
   {
      for (auto D : Storage)
         delete D.second;

      Storage.clear();
   }

public:
   /// Returns true if we have an analysis for a specific function \p F without
   /// actually constructing it.
   bool hasAnalysis(Function *F) const { return Storage.count(F) != 0; }

   AnalysisTy *maybeGet(Function *F)
   {
      auto Iter = Storage.find(F);
      if (Iter == Storage.end())
         return nullptr;

      return Iter->second;
   }

   /// Returns an analysis provider for a specific function \p F.
   AnalysisTy *get(Function *F)
   {
      auto &it = Storage.FindAndConstruct(F);
      if (!it.second)
         it.second = newFunctionAnalysis(F);

      return it.second;
   }

   /// Invalidate all information in this analysis.
   void invalidate() override
   {
      deleteAllAnalysisProviders();
   }

   /// Helper function to remove the analysis data for a function.
   void invalidateFunction(Function *F)
   {
      auto &it = Storage.FindAndConstruct(F);
      if (it.second) {
         delete it.second;
         it.second = nullptr;
      }
   }

   /// Invalidate all of the information for a specific function.
   void invalidate(Function *F, Analysis::InvalidationKind K) override
   {
      if (shouldInvalidate(K))
         invalidateFunction(F);
   }

   /// Notify the analysis about a newly created function.
   void notifyAddFunction(Function*) override {}

   /// Notify the analysis about a function which will be deleted from the
   /// module.
   void notifyDeleteFunction(Function *F) override
   {
      invalidateFunction(F);
   }

   explicit FunctionAnalysisBase(AnalysisKind Kind)
      : Analysis(Kind), Storage()
   {}

   ~FunctionAnalysisBase() override
   {
      deleteAllAnalysisProviders();
   }

   FunctionAnalysisBase(const FunctionAnalysisBase &) = delete;
   FunctionAnalysisBase &operator=(const FunctionAnalysisBase &) = delete;

   /// Verify all of the AnalysisTy for all functions.
   ///
   /// This is not meant to be overridden by subclasses. See "void
   /// verify(AnalysisTy *A)".
   void verify() const final
   {
      for (auto Iter : Storage) {
         if (!Iter.second)
            continue;

         verify(Iter.second);
      }
   }

   /// Verify the AnalysisTy that we have stored for the specific function \p
   /// F.
   ///
   /// This is not meant to be overridden by subclasses. See "void
   /// verify(AnalysisTy *A)".
   void verify(Function *F) const final
   {
      auto Iter = Storage.find(F);
      if (Iter == Storage.end())
         return;
      if (!Iter->second)
         return;

      verify(Iter->second);
   }
};

/// Given a specific type of analysis and its function info. Store the
/// analysis and upon request instantiate the function info, caching the
/// function info for subsequent requests.
template <class AnalysisTy, class FunctionInfoTy>
class LazyFunctionInfo {
   Function *F;
   AnalysisTy *A;
   FunctionInfoTy *FTy;

public:
   LazyFunctionInfo(Function *F, AnalysisTy *A) : F(F), A(A), FTy(nullptr) {}

   /*implicit*/ operator FunctionInfoTy *()
   {
      if (!FTy)
         FTy = A->get(F);

      return FTy;
   }

   FunctionInfoTy *operator->() { return *this; }
};

} // namespace il
} // namespace cdot

#endif //CDOT_ANALYSIS_H
