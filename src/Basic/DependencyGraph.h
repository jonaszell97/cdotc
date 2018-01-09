//
// Created by Jonas Zell on 22.12.17.
//

#ifndef CDOT_DEPENDENCYGRAPH_H
#define CDOT_DEPENDENCYGRAPH_H

#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallVector.h>

namespace cdot {

template <class T>
class DependencyGraph {
public:
   struct Vertex {
      explicit Vertex(T* Ptr)
         : Ptr(Ptr) {}

      Vertex(Vertex const&) = delete;
      Vertex &operator=(Vertex const&) = delete;

      Vertex(Vertex &&vert) noexcept
         : Ptr(vert.Ptr), Incoming(std::move(vert.Incoming)),
           Outgoing(std::move(vert.Outgoing))
      {

      }

      void addOutgoing(Vertex *vert)
      {
         Outgoing.insert(vert);
         vert->Incoming.insert(this);
      }

      void removeOutgoing(Vertex *vert)
      {
         Outgoing.erase(vert);
         vert->Incoming.erase(this);
      }

      T *getPtr() const
      {
         return Ptr;
      }

      const llvm::SmallPtrSet<Vertex*, 4> &getOutgoing() const
      {
         return Outgoing;
      }

      const llvm::SmallPtrSet<Vertex*, 4> &getIncoming() const
      {
         return Incoming;
      }

   private:
      T* Ptr;
      llvm::SmallPtrSet<Vertex*, 4> Incoming;
      llvm::SmallPtrSet<Vertex*, 4> Outgoing;
   };

   DependencyGraph() = default;
   DependencyGraph(const DependencyGraph &) = delete;
   DependencyGraph(DependencyGraph &&) = delete;

   ~DependencyGraph()
   {
      for (const auto &vert : Vertices)
         delete vert;
   }

   DependencyGraph &operator=(const DependencyGraph &) = delete;
   DependencyGraph &operator=(DependencyGraph &&) = delete;

   Vertex &getOrAddVertex(T *ptr)
   {
      for (auto &V : Vertices)
         if (V->getPtr() == ptr)
            return *V;

      Vertices.push_back(new Vertex(ptr));
      return *Vertices.back();
   }

   template <class Actor>
   bool actOnGraphInOrder(Actor const& act)
   {
      llvm::SmallVector<T*, 8> Order;
      if (!getEvaluationOrder(Order))
         return false;

      for (const auto &vert : Order)
         act(vert);

      return true;
   }

   std::pair<llvm::SmallVector<T*, 8>, bool> constructOrderedList()
   {
      std::pair<llvm::SmallVector<T*, 8>, bool> res;
      res.second = getEvaluationOrder(res.first);

      return res;
   }

   std::pair<T*, T*> getOffendingPair()
   {
      for (auto &vert : Vertices)
         if (!vert->getOutgoing().empty())
            return { vert->getPtr(),
               (*vert->getOutgoing().begin())->getPtr() };

      llvm_unreachable("order is valid!");
   }

   const llvm::SmallVector<Vertex*, 8> &getVertices() const
   {
      return Vertices;
   }

private:
   bool getEvaluationOrder(llvm::SmallVector<T*, 8> &Order)
   {
      llvm::SmallPtrSet<Vertex*, 4> VerticesWithoutIncomingEdges;
      for (auto &vert : Vertices)
         if (vert->getIncoming().empty())
            VerticesWithoutIncomingEdges.insert(vert);

      size_t cnt = 0;
      while (!VerticesWithoutIncomingEdges.empty()) {
         auto vert = *VerticesWithoutIncomingEdges.begin();
         VerticesWithoutIncomingEdges.erase(vert);

         Order.push_back(vert->getPtr());

         while (!vert->getOutgoing().empty()) {
            auto out = *vert->getOutgoing().begin();
            vert->removeOutgoing(out);

            if (out->getIncoming().empty())
               VerticesWithoutIncomingEdges.insert(out);
         }

         ++cnt;
      }

      return cnt == Vertices.size();
   }

   llvm::SmallVector<Vertex*, 8> Vertices;
};

} // namespace cdot

#endif //CDOT_DEPENDENCYGRAPH_H
