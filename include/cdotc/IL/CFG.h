#ifndef CDOT_CFG_H
#define CDOT_CFG_H

#include "cdotc/IL/Function.h"
#include "cdotc/IL/Utils/BlockIterator.h"

#include <llvm/ADT/GraphTraits.h>

namespace llvm {

//===----------------------------------------------------------------------===//
// GraphTraits for SILBasicBlock
//===----------------------------------------------------------------------===//

template<> struct GraphTraits<cdot::il::BasicBlock*> {
   using ChildIteratorType = cdot::il::succ_iterator;
   using Node = cdot::il::BasicBlock;
   using NodeRef = Node*;

   static NodeRef getEntryNode(NodeRef BB) { return BB; }

   static ChildIteratorType child_begin(NodeRef N)
   {
      return ::cdot::il::succ_begin(N);
   }

   static ChildIteratorType child_end(NodeRef N)
   {
      return ::cdot::il::succ_end(N);
   }
};

template<> struct GraphTraits<const cdot::il::BasicBlock*> {
   using ChildIteratorType = cdot::il::const_succ_iterator;
   using Node = const cdot::il::BasicBlock;
   using NodeRef = Node*;

   static NodeRef getEntryNode(NodeRef BB) { return BB; }

   static ChildIteratorType child_begin(NodeRef N)
   {
      return ::cdot::il::succ_begin(N);
   }

   static ChildIteratorType child_end(NodeRef N)
   {
      return ::cdot::il::succ_end(N);
   }
};

template<> struct GraphTraits<Inverse<cdot::il::BasicBlock*> > {
   using ChildIteratorType = cdot::il::pred_iterator;
   using Node = cdot::il::BasicBlock;
   using NodeRef = Node*;

   static NodeRef getEntryNode(Inverse<cdot::il::BasicBlock*> G)
   {
      return G.Graph;
   }

   static inline ChildIteratorType child_begin(NodeRef N)
   {
      return ::cdot::il::pred_begin(N);
   }

   static inline ChildIteratorType child_end(NodeRef N)
   {
      return ::cdot::il::pred_end(N);
   }
};

template<> struct GraphTraits<Inverse<const cdot::il::BasicBlock*> > {
   using ChildIteratorType = cdot::il::const_pred_iterator;
   using Node = const cdot::il::BasicBlock;
   using NodeRef = Node*;

   static NodeRef getEntryNode(Inverse<const cdot::il::BasicBlock*> G)
   {
      return G.Graph;
   }

   static inline ChildIteratorType child_begin(NodeRef N)
   {
      return ::cdot::il::pred_begin(N);
   }

   static inline ChildIteratorType child_end(NodeRef N)
   {
      return ::cdot::il::pred_end(N);
   }
};

template<>
struct GraphTraits<cdot::il::Function*>
    : public GraphTraits<cdot::il::BasicBlock*> {
   using GraphType = cdot::il::Function*;
   using NodeRef = cdot::il::BasicBlock*;

   static NodeRef getEntryNode(GraphType F) { return &*F->begin(); }

   typedef pointer_iterator<cdot::il::Function::iterator> nodes_iterator;
   static nodes_iterator nodes_begin(GraphType F)
   {
      return nodes_iterator(F->begin());
   }
   static nodes_iterator nodes_end(GraphType F)
   {
      return nodes_iterator(F->end());
   }
   static unsigned size(GraphType F) { return F->getBasicBlocks().size(); }
};

template<>
struct GraphTraits<Inverse<cdot::il::Function*> >
    : public GraphTraits<Inverse<cdot::il::BasicBlock*> > {
   typedef Inverse<cdot::il::Function*> GraphType;
   typedef NodeRef NodeRef;

   static NodeRef getEntryNode(GraphType F) { return &*F.Graph->begin(); }

   typedef pointer_iterator<cdot::il::Function::iterator> nodes_iterator;
   static nodes_iterator nodes_begin(GraphType F)
   {
      return nodes_iterator(F.Graph->begin());
   }
   static nodes_iterator nodes_end(GraphType F)
   {
      return nodes_iterator(F.Graph->end());
   }
   static unsigned size(GraphType F)
   {
      return F.Graph->getBasicBlocks().size();
   }
};

} // namespace llvm

#endif // CDOT_CFG_H
