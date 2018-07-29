//
// Created by Jonas Zell on 22.12.17.
//

#ifndef CDOT_DEPENDENCYGRAPH_H
#define CDOT_DEPENDENCYGRAPH_H

#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/SmallVector.h>

#ifndef NDEBUG
#  include <llvm/Support/raw_ostream.h>
#endif

namespace cdot {

template <class T>
class DependencyGraph {
public:
   struct Vertex {
      explicit Vertex(T &&Ptr)
         : Ptr(std::move(Ptr))
      {}

      Vertex(Vertex const&) = delete;
      Vertex &operator=(Vertex const&) = delete;

      Vertex(Vertex &&vert) noexcept
         : Ptr(std::move(vert.Ptr)), Incoming(std::move(vert.Incoming)),
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

      void addIncoming(Vertex *vert)
      {
         Incoming.insert(vert);
         vert->Outgoing.insert(this);
      }

      void removeIncoming(Vertex *vert)
      {
         Incoming.erase(vert);
         vert->Outgoing.erase(this);
      }

      void reset()
      {
         for (auto *Out : Outgoing) {
            Out->Incoming.erase(this);
         }
         for (auto *In : Incoming) {
            In->Outgoing.erase(this);
         }

         Outgoing.clear();
         Incoming.clear();
      }

      const T &getPtr() { return Ptr; }

      const llvm::SmallPtrSet<Vertex*, 4> &getOutgoing() const
      {
         return Outgoing;
      }

      const llvm::SmallPtrSet<Vertex*, 4> &getIncoming() const
      {
         return Incoming;
      }

   private:
      T Ptr;
      llvm::SmallPtrSet<Vertex*, 4> Incoming;
      llvm::SmallPtrSet<Vertex*, 4> Outgoing;
   };

   DependencyGraph() = default;
   DependencyGraph(const DependencyGraph &) = delete;
   
   DependencyGraph(DependencyGraph &&that) : Vertices(std::move(that.Vertices))
   {
      that.Vertices.clear();
   }

   ~DependencyGraph()
   {
      destroyVerts();
   }

   DependencyGraph &operator=(const DependencyGraph &) = delete;
   DependencyGraph &operator=(DependencyGraph &&that) noexcept
   {
      destroyVerts();
      
      Vertices = std::move(that.Vertices);
      that.Vertices.clear();
      
      return *this;
   }

   Vertex &getOrAddVertex(T ptr)
   {
      accessed = true;

      for (auto &V : Vertices)
         if (V->getPtr() == ptr)
            return *V;

      Vertices.push_back(new Vertex(std::move(ptr)));
      return *Vertices.back();
   }

   template <class Actor>
   bool actOnGraphInOrder(Actor const& act)
   {
      llvm::SmallVector<T, 8> Order;
      if (!getEvaluationOrder(Order))
         return false;

      for (const auto &vert : Order)
         act(vert);

      return true;
   }

   std::pair<llvm::SmallVector<T, 8>, bool> constructOrderedList()
   {
      std::pair<llvm::SmallVector<T, 8>, bool> res;
      res.second = getEvaluationOrder(res.first);

      return res;
   }

   std::pair<T, T> getOffendingPair()
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

   void erase(const T &t)
   {
      auto it = Vertices.begin();
      auto end_it = Vertices.end();

      for (; it != end_it; ++it) {
         Vertex *&V = *it;
         if (V->getPtr() == t) {
            while (!V->getOutgoing().empty()) {
               Vertex *Out = *V->getOutgoing().begin();
               Out->removeIncoming(V);
            }
            while (!V->getIncoming().empty()) {
               Vertex *In = *V->getIncoming().begin();
               In->removeOutgoing(V);
            }

            Vertices.erase(it);
            delete V;
            
            break;
         }
      }
   }

   void clear()
   {
      destroyVerts();
      Vertices.clear();
   }

   bool empty() const
   {
      return Vertices.empty();
   }

   bool wasAccessed() const { return accessed; }
   void resetAccessed() { accessed = false; }

#ifndef NDEBUG
   template<class PrintFn>
   void print(const PrintFn &Fn)
   {
      int i = 0;
      for (auto &Vert : Vertices) {
         if (i++ != 0) llvm::outs() << "\n\n";
         llvm::outs() << Fn(Vert->getPtr());
         for (auto Out : Vert->getIncoming()) {
            llvm::outs() << "\n    depends on " << Fn(Out->getPtr());
         }
      }
   }
#endif

private:
   bool getEvaluationOrder(llvm::SmallVector<T, 8> &Order)
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
   
   void destroyVerts()
   {
      for (const auto &vert : Vertices)
         delete vert;
   }

   llvm::SmallVector<Vertex*, 8> Vertices;
   bool accessed = false;
};

} // namespace cdot

#endif //CDOT_DEPENDENCYGRAPH_H
