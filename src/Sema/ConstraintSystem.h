//
// Created by Jonas Zell on 2018-12-01.
//

#ifndef CDOT_CONSTRAINTSYSTEM_H
#define CDOT_CONSTRAINTSYSTEM_H

#include "Basic/DeclarationName.h"
#include "CandidateSet.h"
#include "ConstraintGraph.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class QueryContext;

namespace ast {

class OverloadedDeclRefExpr;

} // namespace ast

namespace sema {

class ConstraintSystem;

using TypeVariable = TypeVariableType*;

class ConstraintLocator: public llvm::FoldingSetNode {
public:
   /// Describes the kind of locator.
   enum LocatorKind : uint8_t {
      /// \brief The contextual type of the expression.
      ContextualType,

      /// \brief The type of a function parameter.
      ParameterType,

      /// \brief A member reference.
      MemberReference,

      /// \brief Name of an overloaded declaration.
      OverloadedDeclName,

      /// \brief Type of an overloaded declaration.
      OverloadedDeclLoc,

      /// \brief A template parameter location.
      TemplateParam,
   };

   /// An element in the locator path.
   struct PathElement {
      /// \return The path element kind.
      LocatorKind getKind() const { return Kind; }

      /// \return The source range stored in this path element.
      SourceRange getSourceRange() const
      {
         assert(storesLoc(Kind) && "path element does not store a type!");
         return SourceRange(SourceLocation((unsigned)Data),
                            SourceLocation((unsigned)(Data >> 32)));
      }

      /// \return The declaration name stored in this path element.
      DeclarationName getDeclarationName() const
      {
         assert(storesDeclName(Kind) && "path element does not store a name!");
         return DeclarationName::getFromOpaquePtr((void*)Data);
      }

      /// \return The function parameter declaration.
      ast::FuncArgDecl *getParamDecl() const
      {
         assert(storesFuncParam(Kind)
            && "path element does not store a param!");
         return reinterpret_cast<ast::FuncArgDecl*>(Data);
      }

      /// \return The template parameter declaration.
      ast::TemplateParamDecl *getTemplateParamDecl() const
      {
         assert(storesTemplateParam(Kind)
                && "path element does not store a param!");
         return reinterpret_cast<ast::TemplateParamDecl*>(Data);
      }

      /// FoldingSetNode Impl
      void Profile(llvm::FoldingSetNodeID &ID) const;
      static void Profile(llvm::FoldingSetNodeID &ID,
                          LocatorKind Kind,
                          uint64_t Data);

      /// Create a contextual type path element.
      static PathElement contextualType(SourceRange SR)
      {
         uint64_t Data = SR.getStart().getOffset()
            | ((uint64_t)(SR.getEnd().getOffset()) << 32);

         return PathElement(ContextualType, Data);
      }

      /// Create a parameter type element.
      static PathElement parameterType(ast::FuncArgDecl *Decl)
      {
         return PathElement(ParameterType, reinterpret_cast<uint64_t>(Decl));
      }

      /// Create a template parameter type element.
      static PathElement templateParam(ast::TemplateParamDecl *Decl)
      {
         return PathElement(TemplateParam, reinterpret_cast<uint64_t>(Decl));
      }

      /// Create a member reference path element.
      static PathElement memberReference(DeclarationName Name)
      {
         return PathElement(MemberReference, (uint64_t)Name.getAsOpaquePtr());
      }

      /// Create an overload path element.
      static PathElement overloadedDecl(DeclarationName Name)
      {
         return PathElement(OverloadedDeclName,(uint64_t)Name.getAsOpaquePtr());
      }

      /// Create an overloaded decl location path element.
      static PathElement overloadedDeclLoc(SourceRange SR)
      {
         uint64_t Data = SR.getStart().getOffset()
                         | ((uint64_t)(SR.getEnd().getOffset()) << 32);

         return PathElement(OverloadedDeclLoc, Data);
      }

   private:
      PathElement(LocatorKind Kind, uint64_t Data) : Kind(Kind), Data(Data)
      {
         assert(needsData(Kind) && "bad path element kind!");
      }

      explicit PathElement(LocatorKind Kind) : Kind(Kind), Data(0)
      {
         assert(!needsData(Kind) && "bad path element kind!");
      }

      /// The kind of path element.
      LocatorKind Kind;

      /// The stored data of this element.
      uint64_t Data;

      // Validation functions.
      static bool needsData(LocatorKind Kind)
      {
         switch (Kind) {
         case ContextualType:
         case ParameterType:
         case MemberReference:
         case OverloadedDeclName:
         case OverloadedDeclLoc:
         case TemplateParam:
            return true;
         default:
            return false;
         }
      }

      static bool storesLoc(LocatorKind Kind)
      {
         switch (Kind) {
         case ContextualType:
         case OverloadedDeclLoc:
            return true;
         default:
            return false;
         }
      }

      static bool storesDeclName(LocatorKind Kind)
      {
         switch (Kind) {
         case MemberReference:
         case OverloadedDeclName:
            return true;
         default:
            return false;
         }
      }

      static bool storesFuncParam(LocatorKind Kind)
      {
         switch (Kind) {
         case ParameterType:
            return true;
         default:
            return false;
         }
      }

      static bool storesTemplateParam(LocatorKind Kind)
      {
         switch (Kind) {
         case TemplateParam:
            return true;
         default:
            return false;
         }
      }
   };

   /// \return The anchor expression.
   ast::Expression *getAnchor() const { return Anchor; }

   /// \return The path elements of this locator.
   ArrayRef<PathElement> getPathElements() const
   {
      return { reinterpret_cast<const PathElement*>(this + 1), NumElements };
   }

   /// FoldingSetNode Impl
   void Profile(llvm::FoldingSetNodeID &ID) const;
   static void Profile(llvm::FoldingSetNodeID &ID,
                       ast::Expression *Anchor,
                       ArrayRef<PathElement> Elements);

   static ConstraintLocator *Create(llvm::BumpPtrAllocator &Allocator,
                                    ast::Expression *Anchor,
                                    ArrayRef<PathElement> Elements);

private:
   ConstraintLocator(ast::Expression *Anchor,
                     ArrayRef<PathElement> Elements);

   /// The expression anchor.
   ast::Expression *Anchor;

   /// The number of contained path elements.
   unsigned NumElements;
};

using Locator = ConstraintLocator*;

class Constraint {
public:
   /// Describes the type of constraint this represents.
   enum ConstraintKind {
#     define CDOT_CONSTRAINT(NAME) NAME##ID,
#     include "Constraints.def"
   };

   /// \return the kind of constraint this is.
   ConstraintKind getKind() const { return Kind; }

   /// \return the constrained type.
   TypeVariable getConstrainedType() const { return ConstrainedType; }

   /// \return the right-hand side type of this constraint, if it has one.
   QualType getRHSType() const;

   /// \return the locator for this constraint.
   Locator getLocator() const { return Loc; }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// Dump the constraint to stderr.
   void dump() const;

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C) { return true; }

protected:
   /// Subclass C'tor.
   Constraint(ConstraintKind K, TypeVariable Var, Locator Loc);

   /// \brief The kind of constraint this is.
   ConstraintKind Kind;

   /// \brief The constrained type variable.
   TypeVariable ConstrainedType;

   /// \brief Information about the location where this constraint was
   /// generated.
   Locator Loc;
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const Constraint &C);

/// Superclass for all relational constraints.
class RelationalConstraint: public Constraint {
protected:
   RelationalConstraint(ConstraintKind K, TypeVariable Var, QualType RHS,
                        Locator Loc);

   /// \brief The right hand side of this relational constraint.
   QualType Type;

public:
   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      switch (C->getKind()) {
#     define CDOT_RELATIONAL_CONSTRAINT(NAME) case NAME##ID:
#     include "Constraints.def"
         return true;
      default:
         return false;
      }
   }

   /// \return the type data of this constraint.
   QualType getType() const { return Type; }
};

/// Superclass for all type property constraints.
class TypePropertyConstraint: public Constraint {
protected:
   TypePropertyConstraint(ConstraintKind K, TypeVariable Var,
                          Locator Loc);

public:
   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      switch (C->getKind()) {
#     define CDOT_TYPE_PROPERTY_CONSTRAINT(NAME) case NAME##ID:
#     include "Constraints.def"
         return true;
      default:
         return false;
      }
   }
};

/// The constrained type variable must exactly equal a concrete type
/// or a type variable.
class TypeBindingConstraint: public RelationalConstraint {
   TypeBindingConstraint(TypeVariable Var, QualType Type, Locator Loc);

public:
   static TypeBindingConstraint *Create(ConstraintSystem &Sys,
                                        TypeVariable Var,
                                        QualType Type,
                                        Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == TypeBindingID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable must equal a concrete type
/// or a type variable, dropping references.
class TypeEqualityConstraint: public RelationalConstraint {
   TypeEqualityConstraint(TypeVariable Var, QualType Type, Locator Loc);

public:
   static TypeEqualityConstraint *Create(ConstraintSystem &Sys,
                                         TypeVariable Var,
                                         QualType Type,
                                         Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == TypeEqualityID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable must not equal a concrete type
/// or a type variable.
class TypeInequalityConstraint: public RelationalConstraint {
   TypeInequalityConstraint(TypeVariable Var, QualType Type, Locator Loc);

public:
   static TypeInequalityConstraint *Create(ConstraintSystem &Sys,
                                           TypeVariable Var,
                                           QualType Type,
                                           Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == TypeInequalityID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable is the return type of the right hand side
/// type, which must be a function type.
class FunctionReturnTypeConstraint: public RelationalConstraint {
   FunctionReturnTypeConstraint(TypeVariable Var, QualType Type, Locator Loc);

public:
   static FunctionReturnTypeConstraint *Create(ConstraintSystem &Sys,
                                               TypeVariable Var,
                                               QualType Type,
                                               Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == FunctionReturnTypeID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable is a parameter type of the right hand side
/// type, which must be a function type.
class FunctionParamTypeConstraint: public RelationalConstraint {
   FunctionParamTypeConstraint(TypeVariable Var, QualType Type,
                               unsigned ParamIdx,
                               Locator Loc);

   /// The index of the function parameter.
   unsigned ParamIdx;

public:
   static FunctionParamTypeConstraint *Create(ConstraintSystem &Sys,
                                              TypeVariable Var,
                                              QualType Type,
                                              unsigned ParamIdx,
                                              Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == FunctionParamTypeID;
   }

   /// \return The index of the function parameter.
   unsigned getParamIndex() const { return ParamIdx; }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type represents a template parameter, which must satisfy
/// covariance constraint.
class CovarianceConstraint: public RelationalConstraint {
   CovarianceConstraint(TypeVariable Var,
                        QualType Covariance,
                        bool Variadic,
                        Locator Loc);

   /// Whether or not the template parameter is variadic.
   bool Variadic;

public:
   static CovarianceConstraint *Create(ConstraintSystem &Sys,
                                       TypeVariable Var,
                                       QualType Covariance,
                                       bool Variadic,
                                       Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == CovarianceID;
   }

   /// \return Whether or not the template parameter is variadic.
   bool isVariadic() const { return Variadic; }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable, which represents a template parameter, can
/// be inferred based on the right hand side type.
class InferenceConstraint: public RelationalConstraint {
   InferenceConstraint(TypeVariable Var,
                       QualType RHS,
                       Locator Loc);

public:
   static InferenceConstraint *Create(ConstraintSystem &Sys,
                                      TypeVariable Var,
                                      QualType RHS,
                                      Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == InferenceID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type should be attempted to be set to a given type.
/// In this regard it serves more as a hint to the solver rather than an
/// actual constraint.
class DefaultableConstraint: public Constraint {
   DefaultableConstraint(TypeVariable Var, QualType Type, Locator Loc);

   /// \brief The right hand side of this type constraint.
   QualType Type;

public:
   static DefaultableConstraint *Create(ConstraintSystem &Sys,
                                        TypeVariable Var,
                                        QualType Type,
                                        Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == DefaultableID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return the type data of this constraint.
   QualType getType() const { return Type; }
};

/// The constrained type variable must conform to a protocol.
class ConformanceConstraint: public TypePropertyConstraint {
   ConformanceConstraint(TypeVariable Var, ast::ProtocolDecl *ProtoDecl,
                         Locator Loc);

   /// \brief The protocol this constraint references.
   ast::ProtocolDecl *ProtoDecl;

public:
   static ConformanceConstraint *Create(ConstraintSystem &Sys,
                                        TypeVariable Var,
                                        ast::ProtocolDecl *ProtoDecl,
                                        Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == ConformanceID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return the protocol of this constraint.
   ast::ProtocolDecl *getProtoDecl() const { return ProtoDecl; }
};

/// The constrained type variable must be a class.
class ClassConstraint: public TypePropertyConstraint {
   ClassConstraint(TypeVariable Var, Locator Loc);

public:
   static ClassConstraint *Create(ConstraintSystem &Sys,
                                  TypeVariable Var,
                                  Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == ClassID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable must have a default value.
class HasDefaultValueConstraint: public TypePropertyConstraint {
   HasDefaultValueConstraint(TypeVariable Var, Locator Loc);

public:
   static HasDefaultValueConstraint *Create(ConstraintSystem &Sys,
                                            TypeVariable Var,
                                            Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == HasDefaultValueID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type variable must have a member with a
/// given name that is of a given type.
class MemberConstraint: public Constraint {
   MemberConstraint(TypeVariable Var, DeclarationName MemberName,
                    QualType MemberType, Locator Loc);

   /// The member name.
   DeclarationName MemberName;

   /// The member type.
   QualType MemberType;

public:
   static MemberConstraint *Create(ConstraintSystem &Sys,
                                   TypeVariable Var,
                                   DeclarationName MemberName,
                                   QualType MemberType,
                                   Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == MemberID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return the member name of this constraint.
   DeclarationName getMemberName() const { return MemberName; }

   /// \return the member type of this constraint.
   QualType getMemberType() const { return MemberType; }
};

/// The constrained type variable must be implicitly convertible
/// to another type.
class ImplicitConversionConstraint: public RelationalConstraint {
   ImplicitConversionConstraint(TypeVariable Var, QualType Type, Locator Loc);

public:
   static ImplicitConversionConstraint *Create(ConstraintSystem &Sys,
                                               TypeVariable Var,
                                               QualType Type,
                                               Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == ImplicitConversionID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;
};

/// The constrained type is a literal of some kind.
class LiteralConstraint: public Constraint {
public:
   enum LiteralKind {
      /// An integer literal.
      IntegerLiteral,

      /// A floating point literal.
      FPLiteral,

      /// A boolean literal.
      BoolLiteral,

      /// A grapheme cluster literal.
      GraphemeClasterLiteral,

      /// A code point literal.
      CodePointLiteral,

      /// An ASCII literal.
      ASCIILiteral,

      /// A String literal.
      StringLiteral,

      /// An array literal.
      ArrayLiteral,

      /// A dictionary literal.
      DictionaryLiteral,

      /// A none literal.
      NoneLiteral,
   };

private:
   LiteralConstraint(TypeVariable Var, LiteralKind LK, Locator Loc);

   /// The literal kind.
   LiteralKind LK;

public:
   static LiteralConstraint *Create(ConstraintSystem &Sys,
                                    TypeVariable Var,
                                    LiteralKind LK,
                                    Locator Loc);

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == LiteralID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return The default literal type for this constraint.
   QualType getDefaultLiteralType(QueryContext &QC);

   /// \return the literal kind.
   LiteralKind getLiteralKind() const { return LK; }
};

/// A disjunction of two or more constraints that is satisfied iff one of the
/// constraints is satisfied.
class DisjunctionConstraint final: public Constraint,
                             TrailingObjects<DisjunctionConstraint, Constraint*>
{
   DisjunctionConstraint(ArrayRef<Constraint*> Constraints, Locator Loc);

   /// The number of contained constraints.
   unsigned NumConstraints;

public:
   static DisjunctionConstraint *Create(ConstraintSystem &Sys,
                                        ArrayRef<Constraint*> Constraints,
                                        Locator Loc);

   friend TrailingObjects;

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == DisjunctionID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return the contained constraints.
   ArrayRef<Constraint*> getConstraints() const;
};

/// A constraint that only holds if a particular overload is selected.
class ConditionalConstraint final: public Constraint,
                             TrailingObjects<ConditionalConstraint, Constraint*>
{
   ConditionalConstraint(ArrayRef<Constraint*> Constraints,
                         TypeVariableType *Disjunction,
                         Locator Loc);

   /// The disjunction type variable.
   TypeVariableType *Disjunction;

   /// The number of contained constraints.
   unsigned NumConstraints;

public:
   static ConditionalConstraint *Create(ConstraintSystem &Sys,
                                        ArrayRef<Constraint*> Constraints,
                                        TypeVariableType *Disjunction,
                                        Locator Loc);

   friend TrailingObjects;

   /// isa<>, cast<> dyn_cast<>
   static bool classof(const Constraint *C)
   {
      return C->getKind() == ConditionalID;
   }

   /// Print the constraint to a stream.
   void print(llvm::raw_ostream &OS) const;

   /// \return The disjunction type variable.
   TypeVariableType *getDisjunctionVar() const { return Disjunction; }

   /// \return the contained constraints.
   ArrayRef<Constraint*> getConstraints() const;

   /// \return A specific contained constraint.
   Constraint *getConstraintAt(unsigned Index) const;
};

/// A potential binding of a type variable.
struct PotentialBinding {
   PotentialBinding(CanType Type, unsigned Score, unsigned OverloadIndex = -1)
      : Type(Type), Score(Score), OverloadIndex(OverloadIndex)
   {}

   /// The concrete type of this binding.
   CanType Type;

   /// The score of this binding.
   unsigned Score;

   /// The overload index, if this is an overload binding.
   unsigned OverloadIndex;
};

/// Potential bindings for a type variable at a specific stage of the solving
/// process.
struct PotentialBindings {
   explicit PotentialBindings(SmallVector<PotentialBinding, 2> &&Bindings)
      : Bindings(std::move(Bindings))
   { }

   PotentialBindings() = default;

   SmallVector<PotentialBinding, 2> Bindings;
};

/// Represents the choice of an overload in the constraint system.
struct OverloadChoice {
   OverloadChoice(unsigned ChosenIndex)
      : ChosenIndex(ChosenIndex)
   { }

   /// The index in the overload set that was chosen.
   unsigned ChosenIndex;
};

class ConstraintSystem {
public:
   /// Result of constraint solving.
   enum ResultKind {
      /// The assignment is dependent.
      Dependent,

      /// We encountered unrelated errors during solving.
      Error,

      /// The assignment was successful.
      Success,

      /// The assignment was unsuccessful.
      Failure,
   };

   /// Map for type variable assignments.
   using AssignmentMapType = llvm::DenseMap<TypeVariable, QualType>;

   /// Map for overload choices.
   using OverloadChoiceMapType = llvm::DenseMap<TypeVariable, OverloadChoice>;

   /// Map from type variables to the expressions the belong to.
   using ExprMapTy = llvm::DenseMap<ast::Expression*, QualType>;

   /// Map from type variables to the template parameters they belong to.
   using ParamMapTy = llvm::DenseMap<ast::TemplateParamDecl*, TypeVariable>;

   /// Collection of bindings for the solution.
   struct SolutionBindings {
      ExprMapTy ExprBindings;
      ParamMapTy ParamBindings;
      llvm::DenseMap<ast::OverloadedDeclRefExpr*, OverloadChoice> OverloadChoices;
   };

   /// A viable solution to the constraint system.
   struct Solution {
      /// Construct a viable solution.
      Solution(const AssignmentMapType &AssignmentMap, unsigned Score);

      /// Construct a solution that is not viable.
      Solution() : Score(-1) {}

      /// Move construction and assignment.
      Solution(Solution&&) = default;
      Solution &operator=(Solution&&) = default;

      /// Disallow copy assignment and construction.
      Solution(const Solution&) = default;
      Solution &operator=(const Solution&) = default;

      /// \return true iff this solution is viable.
      /*implicit*/ operator bool() { return Score != -1; }

      /// The final variable assignments.
      AssignmentMapType AssignmentMap;

      /// The chosen overloads.
      OverloadChoiceMapType OverloadChoices;

      /// The score of this solution.
      unsigned Score;
   };

   /// Reference to the query context.
   QueryContext &QC;

   /// The location this constraint system belongs to.
   SourceRange Loc;

   /// The current scope of the solver.
   struct SolverScope {
      /// Enter a new solver scope.
      explicit SolverScope(ConstraintSystem &Sys);

      /// Leave a solver scope.
      ~SolverScope();

      // Disallow copying scopes.
      SolverScope(const SolverScope &) = delete;
      SolverScope &operator=(const SolverScope &) = delete;

      /// Reference to the constraint system.
      ConstraintSystem &Sys;

      /// The previous solver scope.
      SolverScope *PreviousScope;

      /// The number of changes in the constraint graph before this scope.
      unsigned PreviousNumChanges;

      /// The previous number of bound type variables.
      unsigned PreviousBoundTypeVars;

      /// The score before entering this scope.
      unsigned PreviousScore;

      /// The best score before entering this scope.
      unsigned PreviousBestScore;
   };

   /// Additional flags for a type variable.
   enum TypeVariableFlags : uint8_t {
      /// \brief No flags.
      None = 0x0,

      /// \brief This type variable represents a template parameter.
      IsTemplateParameter = 0x1,

      /// \brief This type variable has a literal constraint.
      HasLiteralConstraint = 0x2,

      /// \brief This type variable can bind to a reference.
      CanBindToReference = 0x4,

      /// \brief This type variable represents an overload choice.
      IsOverloadChoice = 0x8,

      /// \brief The type variable has a concrete binding.
      HasConcreteBinding = 0x10,

      /// \brief This type variable represents a variadic template parameter.
      IsVariadicTemplateParameter = 0x20,

      /// \brief This type variable can be used to infer a template parameter.
      CanInferTemplateParam = 0x40,
   };

   /// Allocator for constraints in this system.
   mutable llvm::BumpPtrAllocator Allocator;

   /// The last failed constraint.
   Constraint *FailedConstraint = nullptr;

   /// Locators created for this constraint system.
   llvm::FoldingSet<ConstraintLocator> Locators;

private:
   /// The next type variable to assign.
   unsigned NextTypeVariable = 0;

   /// Vector of type variables, indexed by their ID.
   SmallVector<TypeVariableType*, 4> TypeVariables;

   /// The current constraint graph of the system.
   ConstraintGraph CG;

   /// The current solver scope.
   SolverScope *CurrentSolverScope = nullptr;

   /// Preferred bindings for type variables.
   llvm::DenseMap<TypeVariableType*, QualType> PreferredBindings;

   /// Flags for type variables.
   llvm::DenseMap<TypeVariableType*, uint8_t> TypeVariableFlagMap;

   /// Map from type variables to exact bindings.
   llvm::DenseMap<TypeVariableType*, QualType> DirectBindingMap;

   /// Map from type variables to their variadic parameter index.
   llvm::DenseMap<TypeVariableType*, unsigned> VariadicParamIdx;

   /// The score of the current solution.
   unsigned CurrentScore = 0;

   /// The number of bound type variables.
   unsigned BoundTypeVariables = 0;

   /// If true, stop after the first constraint failure.
   bool StopAfterFirstFailure = false;

   /// True iff we encountered an error.
   bool EncounteredError = false;

   /// True iff we encountered a type dependent expression.
   bool TypeDependent = false;

   /// The score of the best solution we found so far.
   unsigned BestScore = -1;

   /// The most recent best score.
   unsigned RecentBestScore = -1;

   /// If given, log the solving process.
   llvm::raw_ostream *LogStream = nullptr;

private:
   /// Update the status based on a query result.
   void updateStatus(unsigned Status);

   /// Function to check if a constraint is satisfied.
   bool isSatisfied(Constraint *C);

   /// Try to simplify a single constraint.
   /// \return true if this constraint is unsolvable, false otherwise.
   bool simplify(Constraint *C, SmallVectorImpl<Constraint*> &Worklist);

   /// Functions to check if a specific constraint is satisfied.
#  define CDOT_CONSTRAINT(NAME)                    \
   bool isSatisfied(NAME##Constraint *C);
#  include "Constraints.def"

public:
   /// Construct a new constraint system.
   explicit ConstraintSystem(QueryContext &QC,
                             SourceRange Loc = SourceRange(),
                             llvm::raw_ostream *LogStream = nullptr);

   /// \return The constraint graph associated with this system.
   ConstraintGraph &getConstraintGraph() { return CG; }

   /// Check if the given type variable is assigned.
   bool isAssigned(QualType T);

   /// Get the concrete type from a type data value.
   QualType getConcreteType(QualType T,
                            TypeVariableType *ConstrainedType = nullptr);

   /// Bind a type variable to a concrete type.
   void bindTypeVariable(TypeVariableType *TypeVar, QualType ConcreteTy,
                         unsigned OverloadChoice = -1,
                         bool AllowRebinding = false);

   /// Get the binding for a type variable introduced by a binding constraint.
   QualType getConstrainedBinding(TypeVariableType *TypeVar);

   /// \return The variadic parameter index of the given type variable.
   unsigned getVariadicParamIdx(TypeVariableType *TypeVar);

   /// Set the variadic parameter index of the given type variable.
   void setVariadicParamIdx(TypeVariableType *TypeVar, unsigned Idx);

   /// \return true iff there are unassigned type variables.
   bool hasUnassignedTypeVariables() const;

   /// \return The number of bound type variables.
   unsigned getNumBoundTypeVariables() const { return BoundTypeVariables; }

   /// Increase the score of the current solution.
   void increaseScore(unsigned By) { CurrentScore += By; }

   /// \return The score of the best solution found so far.
   unsigned getBestScore() const { return BestScore; }

   /// \return The score of the most recent solution.
   unsigned getRecentBestScore() const { return RecentBestScore; }

   /// \return The current score of this solution.
   unsigned getCurrentScore() const { return CurrentScore; }

   /// Set a preferred type binding.
   void setPreferredBinding(TypeVariableType *TypeVar, QualType T);

   /// Get a preferred type binding.
   QualType getPreferredBinding(TypeVariableType *TypeVar) const;

   /// Utility function to create a new constraint.
   template<class T, class ...Args>
   T *newConstraint(Args&&... args)
   {
      auto *Cons = T::Create(*this, std::forward<Args&&>(args)...);
      registerConstraint(Cons);

      return Cons;
   }

   /// Register a new node in the constraint graph.
   void registerConstraint(Constraint *C);

   /// Remove a constraint.
   void removeConstraint(Constraint *C);

   /// Find a specific kind of constraint for a type variable.
   template<class T>
   T *getFirstConstraint(TypeVariableType *TypeVar)
   {
      for (auto *C : CG.getOrAddNode(TypeVar)->getConstraints()) {
         if (T *TC = support::dyn_cast<T>(C)) {
            return TC;
         }
      }

      return nullptr;
   }

   /// Utility function to create a fresh type variable.
   TypeVariable newTypeVariable(uint8_t Flags = None);

   /// \return The number of total type variables.
   unsigned getNumTypeVariables() const { return NextTypeVariable; }

   /// \return All type variables in the system.
   ArrayRef<TypeVariableType*> getTypeVariables() const { return TypeVariables;}

   /// \return The raw flags of the type variable.
   uint8_t getFlags(TypeVariableType *TypeVar);

   /// Add a flag to a type variable.
   void addFlag(TypeVariableType *TypeVar, uint8_t Flag);

   /// Check if the type variable represents a template parameter.
   bool representsTemplateParam(TypeVariableType *TypeVar);

   /// Check if the type variable represents a variadic template parameter.
   bool representsVariadicTemplateParam(TypeVariableType *TypeVar);

   /// Check if the type variable can be used to infer a template parameter.
   bool canInferTemplateParam(TypeVariableType *TypeVar);

   /// Check if the type variable has a literal constraint.
   bool hasLiteralConstraint(TypeVariableType *TypeVar);

   /// Check if the type variable can bind to a reference.
   bool canBindToReference(TypeVariableType *TypeVar);

   /// Check if the type variable represents an overload choice.
   bool isOverloadChoice(TypeVariableType *TypeVar);

   /// Check if the type variable has a concrete binding.
   bool hasConcreteBinding(TypeVariableType *TypeVar);

   /// \return true if logging is enabled.
   bool loggingEnabled() const { return LogStream != nullptr; }

   /// \return The logging stream.
   llvm::raw_ostream *getLoggingStream() const { return LogStream; }

   /// \return Whether or not we should stop after the first failure.
   bool stopAfterFirstFailure() const { return StopAfterFirstFailure; }

   /// Try to solve the constraint system.
   ResultKind solve(SmallVectorImpl<Solution> &Solutions,
                    bool StopAfterFirstFailure = false);

   /// Create a solution from the current solver state.
   bool appendCurrentSolution(SmallVectorImpl<Solution> &Solutions,
                              ArrayRef<TypeVariableType*> TypeVars);

   /// Diagnose the constraint system failure.
   bool diagnoseFailure();

   /// Diagnose the constraint system failure for a function overload.
   bool diagnoseCandidateFailure(CandidateSet::Candidate &Cand);

   /// Diagnose an ambiguous assignment.
   bool diagnoseAmbiguity(const Solution &S1, const Solution &S2);

   /// The result of a solution comparison.
   enum SolutionComparisonResult {
      /// The two solutions are equivalent.
      EquivalentSolution,

      /// The two solutions are equally valid.
      EqualSolution,

      /// The solution is worse than the other one.
      WorseSolution,

      /// The solution is better than the other one.
      BetterSolution,
   };

   /// Compare two solutions.
   SolutionComparisonResult compareSolutions(const Solution &S1,
                                             const Solution &S2);

   /// Simplify the current constraints.
   /// \return the failed constraint, if there is one.
   Constraint *simplifyConstraints(TypeVariableType *Modified = nullptr);

   /// Print the solution to stderr.
   void dumpSolution(const Solution &S);

   /// Print the solution to a stream.
   void printSolution(const Solution &S, llvm::raw_ostream &OS);

   /// Print the constraint system to stderr.
   void dumpConstraints();

   /// Print the constraint system to a stream.
   void printConstraints(llvm::raw_ostream &OS);

   /// Allocator Implementation.
   void *Allocate(size_t size, size_t alignment = 8) const
   {
      return Allocator.Allocate(size, alignment);
   }

   template <typename T>
   T *Allocate(size_t Num = 1) const
   {
      return static_cast<T*>(Allocate(Num * sizeof(T), alignof(T)));
   }

   void Deallocate(void*) const {}
};

} // namespace sema
} // namespace cdot

inline void *operator new(size_t size, const cdot::sema::ConstraintSystem &Sys,
                          size_t alignment = 8) {
   return Sys.Allocate(size, alignment);
}

inline void operator delete(void *ptr, const cdot::sema::ConstraintSystem &Sys,
                            size_t) {
   return Sys.Deallocate(ptr);
}

inline void *operator new[](size_t size,
                            const cdot::sema::ConstraintSystem &Sys,
                            size_t alignment = 8) {
   return Sys.Allocate(size, alignment);
}

inline void operator delete[](void *ptr,
                              const cdot::sema::ConstraintSystem &Sys,
                              size_t) {
   return Sys.Deallocate(ptr);
}

#endif //CDOT_CONSTRAINTSYSTEM_H
