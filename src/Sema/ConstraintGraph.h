#ifndef CDOT_CONSTRAINTGRAPH_H
#define CDOT_CONSTRAINTGRAPH_H

#include "AST/Type.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wnested-anon-types"

namespace cdot {
namespace sema {

class Constraint;
class ConstraintGraph;
class ConstraintGraphNode;
class ConstraintSystem;

struct ConstraintGraphEdge {
   /// C'tor.
   ConstraintGraphEdge(ConstraintGraphNode *End, Constraint *C);

   /// \return the end node of this edge.
   ConstraintGraphNode *getEndNode() const { return End; }

   /// \return the constraint forming this edge.
   Constraint *getConstraint() const { return C; }

private:
   /// The node at the end of this edge.
   ConstraintGraphNode *End;

   /// The constraint that forms this edge.
   Constraint *C;
};

class ConstraintGraphNode {
   /// The type variable this node represents.
   TypeVariableType *TypeVar;

   /// The current assignment of this type variable.
   QualType Assignment;

   /// The current overload choice.
   unsigned OverloadIndex = -1;

   /// Type variables that are adjacent to this node.
   SmallVector<TypeVariableType*, 4> Adjacencies;

   /// All constraints mentioning this type variable.
   SmallVector<Constraint*, 4> Constraints;

   /// Map from constraints to edges in the constraint vector.
   llvm::DenseMap<Constraint*, unsigned> IndexMap;

   /// Map from constraints to edges in the adjacency vector.
   llvm::DenseMap<TypeVariableType*, unsigned> AdjacencyIndexMap;

   /// Type variables that are equivalent to this one.
   llvm::DenseSet<ConstraintGraphNode*> EquivalenceClass;

   /// Private C'tor for use only by the constraint graph.
   explicit ConstraintGraphNode(TypeVariableType *TypeVar);

   /// Add a new adjacency to this node
   void addAdjacency(TypeVariableType *T);

   /// Remove an adjacency of this node.
   void removeAdjacency(TypeVariableType *T);

   /// Add a new constraint mentioning this node.
   void addConstraint(Constraint *C);

   /// Remove a constraint mentioning this node.
   void removeConstraint(Constraint *C);

public:
   /// For use of the private C'tor.
   friend class ConstraintGraph;

   /// \return the type variable this node represents.
   TypeVariableType *getTypeVariable() const { return TypeVar; }

   /// Get the constraints mentioning this node.
   ArrayRef<Constraint*> getConstraints() const { return Constraints; }

   /// Get the adjacencies of this type variable.
   ArrayRef<TypeVariableType*> getAdjacencies() const { return Adjacencies; }
};

class ConstraintGraph {
   /// Reference to the constraint system for allocation.
   ConstraintSystem &Sys;

   /// Map from type variables to nodes.
   llvm::DenseMap<TypeVariableType*, ConstraintGraphNode*> NodeMap;

   /// Vector of active constraints.
   SmallVector<Constraint*, 4> ActiveConstraints;

   /// Map from constraints to their mentioned type variables.
   llvm::DenseMap<Constraint*, std::vector<TypeVariableType*>>
      MentionedTypeVarMap;

   /// True iff we're currently undoing changes.
   bool UndoingChanges = false;

   /// Bind a type variable to a concrete type without binding the entire
   /// equivalence class.
   void bindTypeVariableNoEquivalence(ConstraintGraphNode *Node,
                                      QualType ConcreteTy,
                                      unsigned OverloadChoice);

public:
   /// C'tor.
   explicit ConstraintGraph(ConstraintSystem &Sys);

   /// Get the node representing a type variable.
   ConstraintGraphNode *getOrAddNode(TypeVariableType *T);

   /// Remove a node from the constraint graph.
   void removeNode(TypeVariableType *T);

   /// Add a new adjacency to a node.
   void addAdjacency(ConstraintGraphNode *Src,
                     TypeVariableType *T);

   /// Remove an adjacency of a node.
   void removeAdjacency(ConstraintGraphNode *Src,
                        TypeVariableType *T);

   /// Get the type variables mentioned by a constraint.
   ArrayRef<TypeVariableType*> getMentionedTypeVars(Constraint *C);

   /// Add a new constraint.
   void addConstraint(Constraint *C);

   /// Remove a constraint.
   void removeConstraint(Constraint *C);

   /// Bind a type variable to a concrete type.
   void bindTypeVariable(TypeVariableType *TypeVar, QualType ConcreteTy,
                         unsigned OverloadChoice);

   /// Bind a variadic type variable to a concrete type.
   void bindVariadicTypeVariable(TypeVariableType *TypeVar,
                                 QualType ConcreteTy,
                                 unsigned Index);

   /// Make two type variables equivalent.
   void makeEquivalent(TypeVariableType *T1, TypeVariableType *T2);

   /// \return The current binding of a type variable, if any.
   QualType getBinding(TypeVariableType *TypeVar) const;

   /// \return The current overload choice for a type variable.
   unsigned getOverloadChoice(TypeVariableType *TypeVar) const;

   /// \return All currently active constraints.
   ArrayRef<Constraint*> getActiveConstraints() const{return ActiveConstraints;}

   /// Compute the connected components in this graph.
   unsigned computeConnectedComponents(ArrayRef<TypeVariableType*> TypeVars,
                                       SmallVectorImpl<unsigned> &Components);

   /// Represents a change that was made to the constraint graph.
   struct Change {
      enum ChangeKind {
         /// \brief A node was added.
         AddedNode,

         /// \brief A node was removed.
         RemovedNode,

         /// \brief An adjacency was added.
         AddedAdjacency,

         /// \brief An adjacency was removed.
         RemovedAdjacency,

         /// \brief A constraint was added.
         AddedConstraint,

         /// \brief A constraint was removed.
         RemovedConstraint,

         /// \brief An equivalence was added.
         AddedEquivalence,

         /// \brief An equivalence was removed.
         RemovedEquivalence,

         /// \brief We bound a type variable to a concrete type.
         BoundTypeVariable,
      };

      /// Create a node addition change.
      static Change addedNode(ConstraintGraphNode *Node);

      /// Create a node removal change.
      static Change removedNode(ConstraintGraphNode *Node);

      /// Create an adjacency addition change.
      static Change addedAdjacency(ConstraintGraphNode *Node,
                                   TypeVariableType *T);

      /// Create an adjacency removal change.
      static Change removedAdjacency(ConstraintGraphNode *Node,
                                     TypeVariableType *T);

      /// Create a constraint addition change.
      static Change addedConstraint(Constraint *C);

      /// Create a constraint removal change.
      static Change removedConstraint(Constraint *C);

      /// Create an equivalence addition change.
      static Change addedEquivalence(ConstraintGraphNode *N1,
                                     ConstraintGraphNode *N2);

      /// Create an equivalence removal change.
      static Change removedEquivalence(ConstraintGraphNode *N1,
                                       ConstraintGraphNode *N2);

      /// Create a binding change.
      static Change boundTypeVariable(ConstraintGraphNode *Node,
                                      unsigned VariadicIdx = -1);

      /// Undo this change.
      void undo(ConstraintGraph &CG);

   private:
      /// The kind of change.
      ChangeKind Kind;

      /// Additional data dependent on the kind of change.
      union {
         struct {
            ConstraintGraphNode *Node;
            TypeVariableType *T;
            unsigned VariadicIdx;
         } NodeData;

         struct {
            Constraint *C;
         } ConstraintData;

         struct {
            ConstraintGraphNode *N1;
            ConstraintGraphNode *N2;
         } EquivalenceData;
      };
   };

private:
   /// Keep track of applied changes.
   SmallVector<Change, 4> Changes;

public:
   /// \return The total number of changes made.
   unsigned getNumMadeChanges() const { return Changes.size(); }

   /// Undo the changes made until a certain point.
   void undoChanges(unsigned Until);
};

} // namespace sema
} // namespace cdot

#pragma clang diagnostic pop

#endif //CDOT_CONSTRAINTGRAPH_H
