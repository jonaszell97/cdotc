#ifndef CDOT_DOMINATORTREE_H
#define CDOT_DOMINATORTREE_H

#include "IL/CFG.h"
#include "IL/Analysis/Analysis.h"

#include <llvm/Support/GenericDomTree.h>

extern template class llvm::DominatorTreeBase<cdot::il::BasicBlock, false>;
extern template class llvm::DominatorTreeBase<cdot::il::BasicBlock, true>;
extern template class llvm::DomTreeNodeBase<cdot::il::BasicBlock>;

namespace llvm {
namespace DomTreeBuilder {

using ILDomTree     = llvm::DomTreeBase<cdot::il::BasicBlock>;
using ILPostDomTree = llvm::PostDomTreeBase<cdot::il::BasicBlock>;

extern template void Calculate<ILDomTree>(ILDomTree &DT);
extern template void Calculate<ILPostDomTree>(ILPostDomTree &DT);

} // namespace DomTreeBuilder
} // namespace llvm

namespace cdot {
namespace il {

using DominatorTreeBase     = llvm::DominatorTreeBase<BasicBlock, false>;
using PostDominatorTreeBase = llvm::DominatorTreeBase<BasicBlock, true>;
using DominanceInfoNode     = llvm::DomTreeNodeBase<BasicBlock>;

/// A class for computing basic dominance information.
class DominanceInfo : public DominatorTreeBase {
   using super = DominatorTreeBase;
public:
   DominanceInfo(il::Function *F);

   /// Does instruction A properly dominate instruction B?
   bool properlyDominates(il::Instruction *a, il::Instruction *b);

   /// Does instruction A dominate instruction B?
   bool dominates(il::Instruction *a, il::Instruction *b)
   {
      return a == b || properlyDominates(a, b);
   }

   /// Does value A properly dominate instruction B?
   bool properlyDominates(il::Value *a, il::Instruction *b);

   void verify() const;

   /// Return true if the other dominator tree does not match this dominator
   /// tree.
   inline bool errorOccurredOnComparison(const DominanceInfo &Other) const
   {
      const auto *R = getRootNode();
      const auto *OtherR = Other.getRootNode();

      if (!R || !OtherR || R->getBlock() != OtherR->getBlock())
         return true;

      // Returns *false* if they match.
      if (compare(Other))
         return true;

      return false;
   }

   using DominatorTreeBase::properlyDominates;
   using DominatorTreeBase::dominates;

   bool isValid(il::Function *F) const
   {
      return getNode(&*F->begin()) != nullptr;
   }

   void reset()
   {
      super::reset();
   }
};

class DominanceOrder {
   llvm::SmallVector<il::BasicBlock*, 16> buffer;
   DominanceInfo *DT;
   size_t srcIdx = 0;

public:

   /// Constructor.
   /// \p entry The root of the dominator (sub-)tree.
   /// \p DT The dominance info of the function.
   /// \p capacity Should be the number of basic blocks in the dominator tree to
   ///             reduce memory allocation.
   DominanceOrder(il::BasicBlock *root, DominanceInfo *DT, int capacity = 0)
      : DT(DT)
   {
      buffer.reserve(capacity);
      buffer.push_back(root);
   }

   /// Gets the next block from the worklist.
   ///
   il::BasicBlock *getNext()
   {
      if (srcIdx == buffer.size())
         return nullptr;
      return buffer[srcIdx++];
   }

   /// Pushes the dominator children of a block onto the worklist.
   void pushChildren(il::BasicBlock *block)
   {
      pushChildrenIf(block, [] (il::BasicBlock*) { return true; });
   }

   /// Conditionally pushes the dominator children of a block onto the worklist.
   /// \p pred Takes a block (= a dominator child) as argument and returns true
   ///         if it should be added to the worklist.
   ///
   template <typename Pred>
   void pushChildrenIf(il::BasicBlock *block,
                       Pred pred) {
      DominanceInfoNode *DINode = DT->getNode(block);
      for (auto *DIChild : *DINode) {
         il::BasicBlock *child = DIChild->getBlock();
         if (pred(child))
            buffer.push_back(DIChild->getBlock());
      }
   }
};

/// A class for computing basic post-dominance information.
class PostDominanceInfo: public PostDominatorTreeBase {
   using super = PostDominatorTreeBase;
public:
   PostDominanceInfo(il::Function *F);

   bool properlyDominates(il::Instruction *A, il::Instruction *B);

   void verify() const;

   /// Return true if the other dominator tree does not match this dominator
   /// tree.
   inline bool errorOccurredOnComparison(const PostDominanceInfo &Other) const
   {
      const auto *R = getRootNode();
      const auto *OtherR = Other.getRootNode();

      if (!R || !OtherR || R->getBlock() != OtherR->getBlock())
         return true;

      if (!R->getBlock()) {
         // The post dom-tree has multiple roots. The compare() function can not
         // cope with multiple roots if at least one of the roots is caused by
         // an infinite loop in the CFG (it crashes because no nodes are allocated
         // for the blocks in the infinite loop).
         // So we return a conservative false in this case.
         // TODO: eventually fix the DominatorTreeBase::compare() function.
         return false;
      }

      // Returns *false* if they match.
      if (compare(Other))
         return true;

      return false;
   }

   bool isValid(il::Function *F) const
   {
      return getNode(&*F->begin()) != nullptr;
   }

   using super::properlyDominates;
};

class DominanceAnalysis : public FunctionAnalysisBase<DominanceInfo> {
protected:
   virtual void verify(DominanceInfo *DI) const override {
      if (DI->getRoots().empty())
         return;
      DI->verify();
   }

public:
   DominanceAnalysis()
      : FunctionAnalysisBase<DominanceInfo>(AnalysisKind::DominanceAnalysisID)
   {}

   DominanceAnalysis(const DominanceAnalysis &) = delete;
   DominanceAnalysis &operator=(const DominanceAnalysis &) = delete;

   static bool classof(const Analysis *S)
   {
      return S->getKind() == AnalysisKind::DominanceAnalysisID;
   }

   DominanceInfo *newFunctionAnalysis(Function *F) override
   {
      return new DominanceInfo(F);
   }

   bool shouldInvalidate(Analysis::InvalidationKind K) override
   {
      return K & InvalidationKind::Branches;
   }
};

class PostDominanceAnalysis : public FunctionAnalysisBase<PostDominanceInfo> {
protected:
   virtual void verify(PostDominanceInfo *PDI) const override {
      if (PDI->getRoots().empty())
         return;
      PDI->verify();
   }

public:
   PostDominanceAnalysis()
      : FunctionAnalysisBase<PostDominanceInfo>(AnalysisKind::PostDominanceAnalysisID)
   {}

   PostDominanceAnalysis(const PostDominanceAnalysis &) = delete;
   PostDominanceAnalysis &operator=(const PostDominanceAnalysis &) = delete;

   PostDominanceInfo *newFunctionAnalysis(Function *F) override
   {
      return new PostDominanceInfo(F);
   }

   bool shouldInvalidate(Analysis::InvalidationKind K) override
   {
      return K & InvalidationKind::Branches;
   }

   static bool classof(Analysis const *A)
   { return A->getKind() == AnalysisKind::PostDominanceAnalysisID; }
};

} // namespace il
} // namespace cdot

namespace llvm {

/// DominatorTree GraphTraits specialization so the DominatorTree can be
/// iterable by generic graph iterators.
template <> struct GraphTraits<cdot::il::DominanceInfoNode *> {
   using ChildIteratorType = cdot::il::DominanceInfoNode::iterator;
   typedef cdot::il::DominanceInfoNode *NodeRef;

   static NodeRef getEntryNode(NodeRef N) { return N; }
   static inline ChildIteratorType child_begin(NodeRef N) { return N->begin(); }
   static inline ChildIteratorType child_end(NodeRef N) { return N->end(); }
};

template <> struct GraphTraits<const cdot::il::DominanceInfoNode *> {
   using ChildIteratorType = cdot::il::DominanceInfoNode::const_iterator;
   typedef const cdot::il::DominanceInfoNode *NodeRef;

   static NodeRef getEntryNode(NodeRef N) { return N; }
   static inline ChildIteratorType child_begin(NodeRef N) { return N->begin(); }
   static inline ChildIteratorType child_end(NodeRef N) { return N->end(); }
};

} // end namespace llvm

#endif //CDOT_DOMINATORTREE_H
