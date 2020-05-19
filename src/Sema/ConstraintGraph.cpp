#include "cdotc/Sema/ConstraintGraph.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/ConstraintSystem.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::sema;

ConstraintGraphEdge::ConstraintGraphEdge(ConstraintGraphNode* End,
                                         Constraint* C)
    : End(End), C(C)
{
}

ConstraintGraphNode::ConstraintGraphNode(TypeVariableType* TypeVar)
    : TypeVar(TypeVar)
{
}

void ConstraintGraphNode::addAdjacency(TypeVariableType* T)
{
   AdjacencyIndexMap[T] = Adjacencies.size();
   Adjacencies.push_back(T);
}

void ConstraintGraphNode::removeAdjacency(TypeVariableType* T)
{
   auto Idx = AdjacencyIndexMap[T];
   if (Idx != Adjacencies.size() - 1) {
      AdjacencyIndexMap[Adjacencies.back()] = Idx;
      std::swap(Adjacencies.back(), Adjacencies[Idx]);
   }

   Adjacencies.pop_back();
   AdjacencyIndexMap.erase(T);
}

void ConstraintGraphNode::addConstraint(Constraint* C)
{
   IndexMap[C] = Constraints.size();
   Constraints.push_back(C);
}

void ConstraintGraphNode::removeConstraint(Constraint* C)
{
   auto Idx = IndexMap[C];
   if (Idx != Constraints.size() - 1) {
      IndexMap[Constraints.back()] = Idx;
      std::swap(Constraints.back(), Constraints[Idx]);
   }

   Constraints.pop_back();
   IndexMap.erase(C);
}

ConstraintGraph::ConstraintGraph(ConstraintSystem& Sys) : Sys(Sys) {}

namespace {

class TypeVariableVisitor : public RecursiveTypeVisitor<TypeVariableVisitor> {
   /// The set of type variables.
   SmallPtrSetImpl<TypeVariableType*>& TypeVars;

public:
   TypeVariableVisitor(SmallPtrSetImpl<TypeVariableType*>& TypeVars)
       : TypeVars(TypeVars)
   {
   }

   bool visitTypeVariableType(TypeVariableType* T)
   {
      TypeVars.insert(T);
      return false;
   }
};

} // anonymous namespace

ArrayRef<TypeVariableType*> ConstraintGraph::getMentionedTypeVars(Constraint* C)
{
   auto It = MentionedTypeVarMap.find(C);
   if (It != MentionedTypeVarMap.end()) {
      return It->getSecond();
   }

   auto& Vec = MentionedTypeVarMap[C];
   if (QualType RHSType = C->getRHSType()) {
      SmallPtrSet<TypeVariableType*, 4> MentionedTypeVars;
      MentionedTypeVars.insert(C->getConstrainedType());

      TypeVariableVisitor(MentionedTypeVars).visit(RHSType);
      Vec.insert(Vec.end(), MentionedTypeVars.begin(), MentionedTypeVars.end());
   }
   else if (auto* Cond = support::dyn_cast<ConditionalConstraint>(C)) {
      Vec.push_back(C->getConstrainedType());
      Vec.push_back(Cond->getDisjunctionVar());
   }
   else if (auto* Lit = support::dyn_cast<LiteralConstraint>(C)) {
      auto associatedTypes = Lit->getAssociatedTypes();
      if (auto *TV = support::dyn_cast_or_null<TypeVariableType>(associatedTypes.first)) {
         Vec.push_back(TV);
      }
      if (auto *TV = support::dyn_cast_or_null<TypeVariableType>(associatedTypes.second)) {
         Vec.push_back(TV);
      }

      Vec.push_back(C->getConstrainedType());
   }
   else {
      Vec.push_back(C->getConstrainedType());
   }

   return Vec;
}

ConstraintGraphNode* ConstraintGraph::getOrAddNode(TypeVariableType* T)
{
   auto It = NodeMap.find(T);
   if (It != NodeMap.end()) {
      return It->getSecond();
   }

   auto* Node = new (Sys) ConstraintGraphNode(T);
   NodeMap[T] = Node;

   if (!UndoingChanges) {
      Changes.push_back(Change::addedNode(Node));
   }

   return Node;
}

void ConstraintGraph::removeNode(TypeVariableType* T)
{
   auto It = NodeMap.find(T);
   if (It == NodeMap.end()) {
      return;
   }

   auto* Node = It->getSecond();

   if (!UndoingChanges) {
      Changes.push_back(Change::removedNode(Node));
   }

   Node->~ConstraintGraphNode();
   NodeMap.erase(It);
}

void ConstraintGraph::addAdjacency(ConstraintGraphNode* Src,
                                   TypeVariableType* T)
{
   Src->addAdjacency(T);
   if (!UndoingChanges) {
      Changes.push_back(Change::addedAdjacency(Src, T));
   }
}

void ConstraintGraph::removeAdjacency(ConstraintGraphNode* Src,
                                      TypeVariableType* T)
{
   Src->removeAdjacency(T);
   if (!UndoingChanges) {
      Changes.push_back(Change::removedAdjacency(Src, T));
   }
}

void ConstraintGraph::addConstraint(Constraint* C)
{
   ArrayRef<TypeVariableType*> MentionedTypeVars = getMentionedTypeVars(C);
   for (auto* TypeVar : MentionedTypeVars) {
      auto* Node = getOrAddNode(TypeVar);
      Node->addConstraint(C);

      for (auto* Other : MentionedTypeVars) {
         if (Other == TypeVar) {
            continue;
         }

         Node->addAdjacency(Other);
      }
   }

   if (!UndoingChanges) {
      Changes.push_back(Change::addedConstraint(C));
   }

   ActiveConstraints.push_back(C);
}

void ConstraintGraph::removeConstraint(Constraint* C)
{
   ArrayRef<TypeVariableType*> MentionedTypeVars = getMentionedTypeVars(C);
   for (auto* TypeVar : MentionedTypeVars) {
      auto* Node = getOrAddNode(TypeVar);
      Node->removeConstraint(C);

      for (auto* Other : MentionedTypeVars) {
         if (Other == TypeVar) {
            continue;
         }

         Node->removeAdjacency(Other);
      }
   }

   if (!UndoingChanges) {
      Changes.push_back(Change::removedConstraint(C));
   }

   ActiveConstraints.erase(
       std::find(ActiveConstraints.begin(), ActiveConstraints.end(), C));
}

void ConstraintGraph::bindTypeVariable(TypeVariableType* TypeVar,
                                       QualType ConcreteTy,
                                       unsigned OverloadChoice)
{
   auto* Node = getOrAddNode(TypeVar);
   bindTypeVariableNoEquivalence(Node, ConcreteTy, OverloadChoice);

   for (auto* EqNode : Node->EquivalenceClass) {
      bindTypeVariableNoEquivalence(EqNode, ConcreteTy, -1);
   }
}

void ConstraintGraph::bindVariadicTypeVariable(TypeVariableType* TypeVar,
                                               QualType ConcreteTy,
                                               unsigned Index)
{
   auto* Node = getOrAddNode(TypeVar);
   auto* Tup = Node->Assignment->asTupleType();
   assert(Tup && "bad variadic argument type");

   auto ContainedTypes = Tup->getContainedTypes();
   SmallVector<QualType, 4> NewTypes(ContainedTypes.begin(),
                                     ContainedTypes.end());

   if (Index != -1) {
      while (Index >= NewTypes.size()) {
         NewTypes.push_back(Sys.QC.Sema->ErrorTy);
      }

      NewTypes[Index] = ConcreteTy;
   }
   else {
      NewTypes.push_back(ConcreteTy);
   }

   Node->Assignment = Sys.QC.Context.getTupleType(NewTypes);

   if (!UndoingChanges) {
      Changes.push_back(Change::boundTypeVariable(Node, Index));
   }
}

void ConstraintGraph::bindTypeVariableNoEquivalence(ConstraintGraphNode* Node,
                                                    QualType ConcreteTy,
                                                    unsigned OverloadChoice)
{
   Node->Assignment = ConcreteTy;
   Node->OverloadIndex = OverloadChoice;

   if (!UndoingChanges) {
      Changes.push_back(Change::boundTypeVariable(Node));
   }
}

void ConstraintGraph::makeEquivalent(TypeVariableType* T1, TypeVariableType* T2)
{
   auto* Node1 = getOrAddNode(T1);
   auto* Node2 = getOrAddNode(T2);

   Node1->EquivalenceClass.insert(Node2);
   Node2->EquivalenceClass.insert(Node1);

   Changes.push_back(Change::addedEquivalence(Node1, Node2));
}

QualType ConstraintGraph::getBinding(TypeVariableType* TypeVar) const
{
   auto It = NodeMap.find(TypeVar);
   if (It == NodeMap.end()) {
      return QualType();
   }

   return It->getSecond()->Assignment;
}

unsigned ConstraintGraph::getOverloadChoice(TypeVariableType* TypeVar) const
{
   auto It = NodeMap.find(TypeVar);
   if (It == NodeMap.end()) {
      return -1;
   }

   return It->getSecond()->OverloadIndex;
}

void ConstraintGraph::undoChanges(unsigned Until)
{
   UndoingChanges = true;

   while (Changes.size() > Until) {
      auto& C = Changes.back();
      C.undo(*this);

      Changes.pop_back();
   }

   UndoingChanges = false;
}

ConstraintGraph::Change
ConstraintGraph::Change::addedNode(ConstraintGraphNode* Node)
{
   Change C;
   C.Kind = AddedNode;
   C.NodeData.Node = Node;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::removedNode(ConstraintGraphNode* Node)
{
   Change C;
   C.Kind = RemovedNode;
   C.NodeData.Node = Node;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::addedConstraint(Constraint* Cons)
{
   Change C;
   C.Kind = AddedConstraint;
   C.ConstraintData.C = Cons;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::addedAdjacency(ConstraintGraphNode* Node,
                                        TypeVariableType* T)
{
   Change C;
   C.Kind = AddedAdjacency;
   C.NodeData.Node = Node;
   C.NodeData.T = T;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::removedAdjacency(ConstraintGraphNode* Node,
                                          TypeVariableType* T)
{
   Change C;
   C.Kind = RemovedAdjacency;
   C.NodeData.Node = Node;
   C.NodeData.T = T;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::removedConstraint(Constraint* Cons)
{
   Change C;
   C.Kind = RemovedConstraint;
   C.ConstraintData.C = Cons;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::addedEquivalence(ConstraintGraphNode* N1,
                                          ConstraintGraphNode* N2)
{
   Change C;
   C.Kind = AddedEquivalence;
   C.EquivalenceData.N1 = N1;
   C.EquivalenceData.N2 = N2;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::removedEquivalence(ConstraintGraphNode* N1,
                                            ConstraintGraphNode* N2)
{
   Change C;
   C.Kind = AddedEquivalence;
   C.EquivalenceData.N1 = N1;
   C.EquivalenceData.N2 = N2;

   return C;
}

ConstraintGraph::Change
ConstraintGraph::Change::boundTypeVariable(ConstraintGraphNode* Node,
                                           unsigned VariadicIdx)
{
   Change C;
   C.Kind = BoundTypeVariable;
   C.NodeData.Node = Node;
   C.NodeData.VariadicIdx = VariadicIdx;

   return C;
}

void ConstraintGraph::Change::undo(ConstraintGraph& CG)
{
   switch (Kind) {
   case AddedNode: {
      CG.removeNode(NodeData.Node->getTypeVariable());
      break;
   }
   case RemovedNode: {
      CG.getOrAddNode(NodeData.Node->getTypeVariable());
      break;
   }
   case AddedAdjacency: {
      CG.removeAdjacency(NodeData.Node, NodeData.T);
      break;
   }
   case RemovedAdjacency: {
      CG.addAdjacency(NodeData.Node, NodeData.T);
      break;
   }
   case AddedConstraint: {
      CG.removeConstraint(ConstraintData.C);
      break;
   }
   case RemovedConstraint: {
      CG.addConstraint(ConstraintData.C);
      break;
   }
   case AddedEquivalence: {
      EquivalenceData.N1->EquivalenceClass.erase(EquivalenceData.N2);
      EquivalenceData.N2->EquivalenceClass.erase(EquivalenceData.N1);
      break;
   }
   case RemovedEquivalence: {
      EquivalenceData.N1->EquivalenceClass.insert(EquivalenceData.N2);
      EquivalenceData.N2->EquivalenceClass.insert(EquivalenceData.N1);
      break;
   }
   case BoundTypeVariable: {
      if (CG.Sys.representsVariadicTemplateParam(NodeData.Node->TypeVar)) {
         auto Index = NodeData.VariadicIdx;
         auto* Tup = NodeData.Node->Assignment->asTupleType();
         assert(Tup && "bad variadic argument type");

         auto ContainedTypes = Tup->getContainedTypes();
         SmallVector<QualType, 4> NewTypes(ContainedTypes.begin(),
                                           ContainedTypes.end());

         if (Index == -1 || Index == NewTypes.size() - 1) {
            NewTypes.pop_back();
         }
         else {
            NewTypes[Index] = CG.Sys.QC.Sema->ErrorTy;
         }

         NodeData.Node->Assignment = CG.Sys.QC.Context.getTupleType(NewTypes);
      }
      else {
         NodeData.Node->Assignment = QualType();
         NodeData.Node->OverloadIndex = -1;
      }

      break;
   }
   }
}

static void connectedComponentsImpl(ConstraintGraph& CG,
                                    ConstraintGraphNode* Node,
                                    unsigned NumTypeVars,
                                    unsigned CurrentComponent,
                                    unsigned& LastComponent,
                                    SmallVectorImpl<unsigned>& Components)
{
   auto* T = Node->getTypeVariable();
   if (Components[T->getVariableID()] != NumTypeVars) {
      return;
   }

   if (CurrentComponent == NumTypeVars) {
      CurrentComponent = LastComponent++;
   }

   Components[T->getVariableID()] = CurrentComponent;

   for (auto* Other : Node->getAdjacencies()) {
      connectedComponentsImpl(CG, CG.getOrAddNode(Other), NumTypeVars,
                              CurrentComponent, LastComponent, Components);
   }
}

unsigned ConstraintGraph::computeConnectedComponents(
    ArrayRef<TypeVariableType*> TypeVars, SmallVectorImpl<unsigned>& Components)
{
   unsigned NumTypeVars = TypeVars.size();
   Components.assign(NumTypeVars, NumTypeVars);

   unsigned LastComponent = 0;
   for (auto* TV : TypeVars) {
      connectedComponentsImpl(*this, getOrAddNode(TV), NumTypeVars, NumTypeVars,
                              LastComponent, Components);
   }

   return LastComponent;
}