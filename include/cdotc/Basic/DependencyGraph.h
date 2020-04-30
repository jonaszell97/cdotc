#ifndef CDOT_DEPENDENCYGRAPH_H
#define CDOT_DEPENDENCYGRAPH_H

#include <llvm/ADT/SetVector.h>
#include <llvm/ADT/SmallVector.h>

#ifndef NDEBUG
#include <llvm/Support/raw_ostream.h>
#endif

namespace cdot {

template<class T> class DependencyGraph {
public:
   struct Vertex {
      explicit Vertex(T Ptr) : Val(Ptr) {}

      Vertex(Vertex const&) = delete;
      Vertex& operator=(Vertex const&) = delete;

      Vertex(Vertex&& vert) = delete;
      Vertex& operator=(Vertex &&) = delete;

      void addOutgoing(Vertex* vert)
      {
         outgoing().insert(vert);
         vert->incoming().insert(this);
      }

      void removeOutgoing(Vertex* vert)
      {
         Outgoing->remove(vert);
         vert->Incoming->remove(this);
      }

      void addIncoming(Vertex* vert)
      {
         incoming().insert(vert);
         vert->outgoing().insert(this);
      }

      void removeIncoming(Vertex* vert)
      {
         Incoming->remove(vert);
         vert->Outgoing->remove(this);
      }

      void reset()
      {
         for (auto* Out : getOutgoing()) {
            Out->Incoming->remove(this);
         }
         for (auto* In : getIncoming()) {
            In->Outgoing->remove(this);
         }

         if (Outgoing) {
            Outgoing->clear();
         }
         if (Incoming) {
            Incoming->clear();
         }
      }

      T getVal() const { return Val; }

      llvm::ArrayRef<Vertex*> getOutgoing() const
      {
         return Outgoing ? Outgoing->getArrayRef() : llvm::ArrayRef<Vertex*>();
      }

      llvm::ArrayRef<Vertex*> getIncoming() const
      {
         return Incoming ? Incoming->getArrayRef() : llvm::ArrayRef<Vertex*>();
      }

   private:
      llvm::SetVector<Vertex*> &incoming()
      {
         if (Incoming == nullptr) {
            Incoming = std::make_unique<llvm::SetVector<Vertex*>>();
         }

         return *Incoming;
      }

      llvm::SetVector<Vertex*> &outgoing()
      {
         if (Outgoing == nullptr) {
            Outgoing = std::make_unique<llvm::SetVector<Vertex*>>();
         }

         return *Outgoing;
      }

      T Val;
      std::unique_ptr<llvm::SetVector<Vertex*>> Incoming;
      std::unique_ptr<llvm::SetVector<Vertex*>> Outgoing;
   };

   DependencyGraph() = default;
   DependencyGraph(const DependencyGraph&) = delete;

   DependencyGraph(DependencyGraph&& that) : Vertices(std::move(that.Vertices))
   {
      that.Vertices.clear();
   }

   ~DependencyGraph() { destroyVerts(); }

   DependencyGraph& operator=(const DependencyGraph&) = delete;
   DependencyGraph& operator=(DependencyGraph&& that) noexcept
   {
      destroyVerts();

      Vertices = std::move(that.Vertices);
      that.Vertices.clear();

      return *this;
   }

   Vertex& getOrAddVertex(T ptr)
   {
      auto it = VertexMap.find(ptr);
      if (it != VertexMap.end()) {
         return *it->getSecond();
      }

      auto *result = new Vertex(std::move(ptr));
      Vertices.push_back(result);
      VertexMap[ptr] = result;

      return *result;
   }

   template<class Actor> bool actOnGraphInOrder(Actor const& act)
   {
      llvm::SmallVector<T, 8> Order;
      if (!getEvaluationOrder(Order))
         return false;

      for (const auto& vert : Order)
         act(vert);

      return true;
   }

   template<class Decide>
   void remove_if(const Decide &Fn)
   {
      auto begin = std::partition(Vertices.begin(), Vertices.end(), [Fn](Vertex *V) {
         return !Fn(V->getVal());
      });

      auto end = Vertices.end();
      for (auto it = begin; it != end; ++it) {
         Vertex *V = *it;
         assert(Fn(V->getVal()) && "what?");

         V->reset();
         VertexMap.erase(V->getVal());
         delete V;
      }

      Vertices.erase(begin, end);
   }

   std::pair<llvm::SmallVector<T, 8>, bool>
   constructOrderedList(bool nonDestructive = false)
   {
      std::pair<llvm::SmallVector<T, 8>, bool> res;
      res.second = nonDestructive
          ? getEvaluationOrderNonDestructive(res.first)
          : getEvaluationOrder(res.first);

      return res;
   }

   bool constructOrderedList(llvm::SmallVectorImpl<T> &Order,
                             bool nonDestructive = false)
   {
      return nonDestructive
         ? getEvaluationOrderNonDestructive(Order)
         : getEvaluationOrder(Order);
   }

   std::pair<T, T> getOffendingPair()
   {
      auto valid = constructOrderedList(false);
      assert(!valid.second && "order is valid");

      for (auto& vert : Vertices) {
         if (!vert->getOutgoing().empty()) {
            return {vert->getVal(), (*vert->getOutgoing().begin())->getVal()};
         }
      }

      llvm_unreachable("order is valid!");
   }

   llvm::ArrayRef<Vertex*> getVertices() const { return Vertices; }

   void clear()
   {
      destroyVerts();
      Vertices.clear();
      VertexMap.clear();
   }

   [[nodiscard]] bool empty() const { return Vertices.empty(); }
   [[nodiscard]] size_t size() const { return Vertices.size(); }

#ifndef NDEBUG
   template<class PrintFn> void print(const PrintFn& Fn,
                                      llvm::raw_ostream &OS = llvm::outs())
   {
      int i = 0;
      for (auto& Vert : Vertices) {
         if (i++ != 0)
            OS << "\n\n";

         OS << Fn(Vert->getVal());
         for (auto Out : Vert->getIncoming()) {
            OS << "\n    depends on " << Fn(Out->getVal());
         }
      }
   }
#endif

private:
   bool getEvaluationOrder(llvm::SmallVectorImpl<T> &Order)
   {
      llvm::SmallSetVector<Vertex*, 4> VerticesWithoutIncomingEdges;
      for (auto& vert : Vertices)
         if (vert->getIncoming().empty())
            VerticesWithoutIncomingEdges.insert(vert);

      size_t cnt = 0;
      while (!VerticesWithoutIncomingEdges.empty()) {
         auto vert = *VerticesWithoutIncomingEdges.begin();
         VerticesWithoutIncomingEdges.remove(vert);

         Order.push_back(vert->getVal());

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

   bool getEvaluationOrderNonDestructive(llvm::SmallVectorImpl<T> &Order)
   {
      llvm::DenseMap<Vertex*, llvm::SetVector<Vertex*>> Outgoing;
      llvm::DenseMap<Vertex*, llvm::SetVector<Vertex*>> Incoming;

      llvm::SmallSetVector<Vertex*, 4> VerticesWithoutIncomingEdges;
      for (auto& vert : Vertices) {
         auto in = vert->getIncoming();
         if (in.empty()) {
            VerticesWithoutIncomingEdges.insert(vert);
         }
         else {
            Incoming[vert] = llvm::SetVector<Vertex*>(in.begin(), in.end());
         }

         auto out = vert->getOutgoing();
         if (!out.empty()) {
            Outgoing[vert] = llvm::SetVector<Vertex*>(out.begin(), out.end());
         }
      }

      size_t cnt = 0;
      while (!VerticesWithoutIncomingEdges.empty()) {
         auto vert = *VerticesWithoutIncomingEdges.begin();
         VerticesWithoutIncomingEdges.remove(vert);

         Order.push_back(vert->getVal());

         while (!Outgoing[vert].empty()) {
            auto out = *Outgoing[vert].begin();
            Outgoing[vert].remove(out);
            Incoming[out].remove(vert);

            if (Incoming[out].empty()) {
               VerticesWithoutIncomingEdges.insert(out);
            }
         }

         ++cnt;
      }

      return cnt == Vertices.size();
   }

   void destroyVerts()
   {
      for (const auto& vert : Vertices) {
         delete vert;
      }
   }

   llvm::SmallVector<Vertex*, 8> Vertices;
   llvm::DenseMap<T, Vertex*> VertexMap;
};

} // namespace cdot

#endif // CDOT_DEPENDENCYGRAPH_H
