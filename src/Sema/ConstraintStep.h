//
// Created by Jonas Zell on 2018-12-03.
//

#ifndef CDOT_CONSTRAINTSTEP_H
#define CDOT_CONSTRAINTSTEP_H

#include "ConstraintSystem.h"

namespace cdot {
namespace sema {

class SolverStep {
protected:
   /// Virtual D'tor.
   virtual ~SolverStep() = default;

   /// Base C'tor.
   explicit SolverStep(ConstraintSystem &Sys,
                       SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                       SmallVectorImpl<SolverStep*> &NextSteps,
                       unsigned Indent = 0);

   /// Finalize a step.
   void done(bool IsFailure = false);

   /// Suspend by delegating to a different step.
   void suspend(SolverStep *NextStep = nullptr);

#ifndef NDEBUG
   void applyIndent(unsigned Indent, bool List);

   struct indent_t { unsigned Value; bool List; };

   /// Log a message if logging is enabled.
   template<class MsgType, class ...Args>
   void log(indent_t Indent, MsgType &&Msg, Args&&... args)
   {
      if (!Sys.loggingEnabled())
         return;

      applyIndent(Indent.Value, Indent.List);

      (*Sys.getLoggingStream()) << Msg;
      log(std::forward<Args&&>(args)...);
   }

private:
   template<class MsgType, class ...Args>
   void log(MsgType &&Msg, Args&&... args)
   {
      (*Sys.getLoggingStream()) << Msg;
      log(std::forward<Args&&>(args)...);
   }

   /// Endpoint to avoid infinite recursion.
   void log() {}
#endif

public:
   /// The state of a solver step.
   enum SolverStepState {
      /// \brief The step has not started running.
      Idle,

      /// \brief The step is currently executing.
      Active,

      /// \brief Execution of the step is paused.
      Suspended,

      /// \brief The step has finished executing.
      Done,

      /// \brief The step has finished with an error.
      Error,
   };

   /// Attempt to execute this step.
   virtual void execute(bool PrevFailed) = 0;

   /// Resume the execution of this step after suspension.
   virtual void resume(bool PrevFailed) = 0;

   /// \return The current state of this step.
   SolverStepState state() const { return State; }

   /// Transition to another state.
   void transition(SolverStepState ToState);

protected:
   /// Reference to the constraint system.
   ConstraintSystem &Sys;
#
   /// Reference to a vector of solutions.
   SmallVectorImpl<ConstraintSystem::Solution> &Solutions;

   /// Reference to the worklist of steps to take.
   SmallVectorImpl<SolverStep*> &NextSteps;

#ifndef NDEBUG
   /// Indent to use for logging.
   unsigned Indent = 0;
#endif

   /// The current status of this step.
   SolverStepState State;
};

/// Splits the constraint graph into independent components that are solved
/// seperately.
class SplitterStep: public SolverStep {
   /// Vector of solutions for other steps to use.
   SmallVector<ConstraintSystem::Solution, 4> SolutionCache;

   struct ComponentSolution {
      ComponentSolution(
         SmallVector<ConstraintSystem::Solution, 4> &&Solutions,
         unsigned int BestScore)
         : Solutions(std::move(Solutions)), BestScore(BestScore)
      { }

      SmallVector<ConstraintSystem::Solution, 4> Solutions;
      unsigned BestScore;
   };

   /// The best solutions we have found per component.
   SmallVector<ComponentSolution, 4> PartialSolutions;

   /// Merge all partial solutions to arrive at a complete solution.
   void mergePartialSolutions();

public:
   SplitterStep(ConstraintSystem &Sys,
                SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                SmallVectorImpl<SolverStep*> &NextSteps,
                unsigned Indent = 0);

   /// \inherit
   void execute(bool PrevFailed) override;

   /// \inherit
   void resume(bool PrevFailed) override;
};

/// Attempts to solve the constraint system.
class FindSolutionStep: public SolverStep {
   /// The type variables in this component.
   ArrayRef<TypeVariableType*> TypeVariables;

public:
   FindSolutionStep(ConstraintSystem &Sys,
                    SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                    SmallVectorImpl<SolverStep*> &NextSteps,
                    ArrayRef<TypeVariableType*> TypeVariables,
                    unsigned Indent = 0);

   /// \inherit
   void execute(bool PrevFailed) override;

   /// \inherit
   void resume(bool PrevFailed) override;
};

/// Produces potential bindings for type variables.
class TypeVariableBindingProducer {
   enum ProducerStage {
      Initial, BaseClasses, Protocols, Any,
   };

   /// The initial bindings derived from constraints.
   SmallVector<PotentialBinding, 4> Bindings;

   /// The bindings we have already found.
   SmallPtrSet<CanType, 2> FoundBindings;

   /// The 'stage' of the binding process.
   ProducerStage Stage = Initial;

   /// Current index into the binding vector.
   unsigned Index = 0;

   /// Whether or not the type variable has a concrete binding.
   bool hasConcreteBinding = false;

   /// Computes the initial bindings.
   void computeInitialBindings(TypeVariableType *T, ConstraintSystem &Sys);

   /// Advance to the next stage.
   void nextStage();

public:
   explicit TypeVariableBindingProducer(TypeVariableType *T,
                                        ConstraintSystem &Sys);

   /// \return The next potential binding, if any.
   PotentialBinding *operator()();

   /// Recompute the next level of bindings.
   /// \return true iff there were new bindings produced.
   bool computeFollowupBindings(ConstraintSystem &Sys);

   /// \return true iff there are bindings at the current stage.
   bool hasAnyBindings() const { return !Bindings.empty(); }
};

/// Attempts to bind a type variable by making an educated guess.
class BindTypeVariableStep: public SolverStep {
   /// The type variable we are guessing.
   TypeVariableType *TypeVar;

   /// The potential bindings for this variable.
   TypeVariableBindingProducer Producer;

   /// The type variables in the current component.
   ArrayRef<TypeVariableType*> TypeVariables;

   /// Whether or not any binding we tried lead to a valid solution.
   bool AnyValid = false;

   /// The solver scope introduced by this binding.
   std::unique_ptr<ConstraintSystem::SolverScope> Scope;

public:
   BindTypeVariableStep(ConstraintSystem &Sys,
                        SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                        SmallVectorImpl<SolverStep*> &NextSteps,
                        TypeVariableType *TypeVar,
                        TypeVariableBindingProducer &&Producer,
                        ArrayRef<TypeVariableType*> TypeVariables,
                        unsigned Indent = 0);

   /// \inherit
   void execute(bool PrevFailed) override;

   /// \inherit
   void resume(bool PrevFailed) override;
};

/// Produces bindings for disjunctions.
class DisjunctionBindingProducer {
   /// The bindings obtained by the disjunction.
   SmallVector<PotentialBinding, 4> Bindings;

   /// Current index into the binding vector.
   unsigned Index = 0;

   /// Computes the initial bindings.
   void computeInitialBindings(ConstraintSystem &Sys,
                               DisjunctionConstraint *Disjunction);

public:
   DisjunctionBindingProducer(ConstraintSystem &Sys,
                              DisjunctionConstraint *Disjunction);

   /// \return The next potential binding, if any.
   PotentialBinding *operator()();

   /// \return true iff there are bindings at the current stage.
   bool hasAnyBindings() const { return !Bindings.empty(); }
};

/// Attempts to bind a disjunction to one of the available options.
class DisjunctionChoiceStep: public SolverStep {
   /// The type variable we are guessing.
   TypeVariableType *TypeVar;

   /// The potential bindings for this variable.
   DisjunctionBindingProducer Producer;

   /// The type variables in the current component.
   ArrayRef<TypeVariableType*> TypeVariables;

   /// The solver scope introduced by this binding.
   std::unique_ptr<ConstraintSystem::SolverScope> Scope;

public:
   DisjunctionChoiceStep(ConstraintSystem &Sys,
                         SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                         SmallVectorImpl<SolverStep*> &NextSteps,
                         DisjunctionConstraint *Disjunction,
                         ArrayRef<TypeVariableType*> TypeVariables,
                         unsigned Indent = 0);

   /// \inherit
   void execute(bool PrevFailed) override;

   /// \inherit
   void resume(bool PrevFailed) override;
};

} // namespace sema
} // namespace cdot

#endif //CDOT_CONSTRAINTSTEP_H
