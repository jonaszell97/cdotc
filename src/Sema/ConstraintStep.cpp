//
// Created by Jonas Zell on 2018-12-03.
//

#include "ConstraintStep.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Query/QueryContext.h"
#include "Sema/SemaPass.h"

#include <llvm/Support/raw_ostream.h>

#ifndef NDEBUG
#  define LOG_ERR(...) log(indent_t{Indent * 3, false}, __VA_ARGS__);
#  define LOG(...) log(indent_t{Indent * 3, true}, __VA_ARGS__);
#else
#  define LOG_ERR(...)
#  define LOG(...)
#endif

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

SolverStep::SolverStep(ConstraintSystem &Sys,
                       SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                       SmallVectorImpl<SolverStep*> &NextSteps,
                       unsigned Indent)
   : Sys(Sys), Solutions(Solutions), NextSteps(NextSteps),
#ifndef NDEBUG
     Indent(Indent),
#endif
     State(Idle)
{

}

void SolverStep::done(bool IsFailure)
{
   transition(IsFailure ? Error : Done);
}

void SolverStep::suspend(SolverStep *NextStep)
{
   NextSteps.push_back(this);
   if (NextStep) {
      NextSteps.push_back(NextStep);
   }

   transition(Suspended);
}

void SolverStep::transition(SolverStepState ToState)
{
   switch (ToState) {
   case Idle:
      llvm_unreachable("cannot transition to Idle!");
   case Active:
      assert((State == Idle || State == Suspended) && "bad state transition!");
      break;
   case Suspended:
      assert((State == Active || State == Suspended)&& "bad state transition!");
      break;
   case Done:
      assert(State == Active && "bad state transition!");
      break;
   case Error:
      assert(State == Active && "bad state transition!");
      break;
   }

   State = ToState;
}

#ifndef NDEBUG

void SolverStep::applyIndent(unsigned Indent, bool List)
{
   while (Indent--) {
      (*Sys.getLoggingStream()) << ' ';
   }

   if (List) {
      (*Sys.getLoggingStream()) << " - ";
   }
   else {
      (*Sys.getLoggingStream()) << "   ";
   }
}

#endif

SplitterStep::SplitterStep(ConstraintSystem &Sys,
                           SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                           SmallVectorImpl<SolverStep*> &NextSteps,
                           unsigned Indent)
   : SolverStep(Sys, Solutions, NextSteps, Indent)
{

}

void SplitterStep::execute(bool PrevFailed)
{
   unsigned NumTypeVars = Sys.getNumTypeVariables();

   // Find the connected components of the graph.
   auto AllTypeVars = Sys.getTypeVariables();

   SmallVector<unsigned, 4> Components;
   unsigned NumComponents = Sys.getConstraintGraph()
      .computeConnectedComponents(AllTypeVars, Components);

   PartialSolutions.reserve(NumComponents);

#ifndef NDEBUG
   if (auto *OSPtr = Sys.getLoggingStream()) {
      (*OSPtr) << "components: ";
   }
#endif

   SmallVector<TypeVariableType*, 8> TypeVars;
   for (unsigned i = 0; i < NumComponents; ++i) {
      for (unsigned j = 0; j < NumTypeVars; ++j) {
         if (Components[j] == i) {
            TypeVars.push_back(AllTypeVars[j]);
         }
      }

#ifndef NDEBUG
      if (auto *OSPtr = Sys.getLoggingStream()) {
         if (i != 0) (*OSPtr) << ", ";
         (*OSPtr) << "(";
         unsigned k = 0;
         for (auto *TV : TypeVars) {
            if (k++ != 0) (*OSPtr) << ", ";
            (*OSPtr) << QualType(TV);
         }
         (*OSPtr) << ")";
      }
#endif

      auto Comp = [&](TypeVariableType *LHS, TypeVariableType *RHS) {
         if (LHS == RHS) {
            return LHS->getVariableID() < RHS->getVariableID();
         }

         bool LHSHasBinding = Sys.hasConcreteBinding(LHS);
         bool RHSHasBinding = Sys.hasConcreteBinding(RHS);

         if (LHSHasBinding != RHSHasBinding) {
            if (LHSHasBinding) {
               return true;
            }

            return false;
         }

         return LHS->getVariableID() < RHS->getVariableID();
      };

      // Sort the type variables based on some heuristic criteria.
      std::sort(TypeVars.begin(), TypeVars.end(), Comp);

      // Resume this step after each component.
      TypeVariableType **Mem = Sys.Allocate<TypeVariableType*>(TypeVars.size());
      std::copy(TypeVars.begin(), TypeVars.end(), Mem);

      suspend(new(Sys) FindSolutionStep(Sys, SolutionCache, NextSteps,
                                        { Mem, TypeVars.size() }, Indent));

      TypeVars.clear();
   }

#ifndef NDEBUG
   if (auto *OSPtr = Sys.getLoggingStream()) {
      (*OSPtr) << "\n";
   }
#endif

   transition(Suspended);
}

void SplitterStep::resume(bool PrevFailed)
{
   if (PrevFailed) {
      return done(/*IsFailure=*/true);
   }
   if (SolutionCache.empty()) {
      return done(/*IsFailure=*/true);
   }

   // Find the best solution for the current component.
   SmallVector<ConstraintSystem::Solution, 2> ValidSolutions;

   unsigned BestScore = -1;
   for (auto &S : SolutionCache) {
      if (S.Score < BestScore) {
         BestScore = S.Score;
      }
   }

   for (auto &S : SolutionCache) {
      if (S.Score != BestScore) {
         continue;
      }

      if (ValidSolutions.empty()) {
         ValidSolutions.emplace_back(move(S));
      }
      else {
         auto &Other = ValidSolutions.front();
         auto CompResult = Sys.compareSolutions(Other, S);
         switch (CompResult) {
         case ConstraintSystem::EquivalentSolution:
         case ConstraintSystem::WorseSolution:
            break;
         case ConstraintSystem::BetterSolution:
            ValidSolutions.clear();
            ValidSolutions.emplace_back(move(S));
            break;
         case ConstraintSystem::EqualSolution:
            ValidSolutions.emplace_back(move(S));
            break;
         }
      }
   }

   PartialSolutions.emplace_back(move(ValidSolutions), Sys.getBestScore());
   SolutionCache.clear();

   if (NextSteps.empty()) {
      return mergePartialSolutions();
   }

   transition(Suspended);
}

static void addPartialSolution(ConstraintSystem::Solution &S,
                               ConstraintSystem::Solution &PartialSolution) {
   S.Score += PartialSolution.Score;

   for (auto &Assignment : PartialSolution.AssignmentMap) {
      S.AssignmentMap.insert(Assignment);
   }
   for (auto &Choice : PartialSolution.OverloadChoices) {
      S.OverloadChoices.insert(Choice);
   }
}

static void split(SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                  unsigned NumPartialSolutions,
                  unsigned StartIndex) {
   unsigned NumSolutions = Solutions.size() - StartIndex;

   unsigned i = StartIndex;
   while (NumSolutions < NumPartialSolutions) {
      Solutions.emplace_back(Solutions[i++]);
      ++NumSolutions;
   }
}

void SplitterStep::mergePartialSolutions()
{
   unsigned Index = Solutions.size();
   Solutions.emplace_back();
   Solutions.back().Score = 0;

   for (auto &SolutionsAndScore : PartialSolutions) {
      unsigned NumPartialSolutions = SolutionsAndScore.Solutions.size();
      split(Solutions, NumPartialSolutions, Index);

      for (unsigned i = 0; i < NumPartialSolutions; ++i) {
         addPartialSolution(Solutions[i + Index],
                            SolutionsAndScore.Solutions[i]);
      }
   }
}

FindSolutionStep::FindSolutionStep(ConstraintSystem &Sys,
                                   SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                                   SmallVectorImpl<SolverStep*> &NextSteps,
                                   ArrayRef<TypeVariableType*> TypeVariables,
                                   unsigned Indent)
   : SolverStep(Sys, Solutions, NextSteps, Indent),
     TypeVariables(TypeVariables)
{

}

void FindSolutionStep::execute(bool PrevFailed)
{
   // Create a guessing constraint for each type variable that has possible
   // bindings.
   bool FoundUnassigned = false;
   bool FoundPotentialBindings = false;

   for (auto *TypeVar : TypeVariables) {
      if (Sys.isAssigned(TypeVar)) {
         continue;
      }

      FoundUnassigned = true;

      if (Sys.isOverloadChoice(TypeVar)) {
         DisjunctionConstraint *Disjunction = nullptr;
         for (auto *C : Sys.getConstraintGraph().getOrAddNode(TypeVar)
                           ->getConstraints()) {
            Disjunction = dyn_cast<DisjunctionConstraint>(C);
            if (Disjunction) {
               break;
            }
         }

         assert(Disjunction && "not a disjunction type variable!");

         FoundPotentialBindings = true;
         NextSteps.push_back(new(Sys) DisjunctionChoiceStep(Sys, Solutions,
                                                            NextSteps,
                                                            Disjunction,
                                                            TypeVariables,
                                                            Indent));
      }
      else {
         TypeVariableBindingProducer Producer(TypeVar, Sys);
         if (!Producer.hasAnyBindings()) {
            continue;
         }

         FoundPotentialBindings = true;
         NextSteps.push_back(new(Sys) BindTypeVariableStep(Sys, Solutions,
                                                           NextSteps, TypeVar,
                                                           std::move(Producer),
                                                           TypeVariables,
                                                           Indent));
      }
   }

   if (!FoundPotentialBindings) {
      if (FoundUnassigned) {
         LOG_ERR("incomplete solution\n");
         return done(/*IsFailure=*/Solutions.empty());
      }

      // Found a new solution.
      Sys.appendCurrentSolution(Solutions, TypeVariables);
      LOG_ERR("solution found (", Sys.getCurrentScore(), ")\n");
   }

   return done();
}

void FindSolutionStep::resume(bool)
{
   llvm_unreachable("step does not suspend!");
}

TypeVariableBindingProducer::TypeVariableBindingProducer(TypeVariableType *T,
                                                        ConstraintSystem &Sys) {
   computeInitialBindings(T, Sys);
}

static void addBinding(ConstraintSystem &Sys,
                       TypeVariableType *TypeVar,
                       CanType T,
                       SmallVectorImpl<PotentialBinding> &Bindings,
                       unsigned Score,
                       unsigned OverloadIndex) {
   if (Sys.representsTemplateParam(TypeVar) && T->isProtocol()) {
      Score += 1;
   }

   Bindings.emplace_back(T, Score, OverloadIndex);
}

static void findBindings(ConstraintSystem &Sys,
                         Constraint *C,
                         TypeVariableType *TypeVar,
                         SmallVectorImpl<PotentialBinding> &Bindings,
                         SmallPtrSetImpl<CanType> &FoundBindings,
                         unsigned Score,
                         unsigned OverloadIndex = -1) {
   switch (C->getKind()) {
   default:
      break;
   case Constraint::ImplicitConversionID:
   case Constraint::TypeEqualityID:
   case Constraint::TypeBindingID:
   case Constraint::DefaultableID: {
      CanType T;
      if (C->getConstrainedType() == TypeVar) {
         T = C->getRHSType();
      }
      else if (C->getRHSType() == TypeVar) {
         T = C->getConstrainedType();
      }
      else {
         break;
      }

      T = Sys.getConcreteType(T);
      if (T->containsTypeVariable()) {
         break;
      }

      if (FoundBindings.insert(T).second) {
         addBinding(Sys, TypeVar, T, Bindings, Score, OverloadIndex);
      }

      break;
   }
   case Constraint::LiteralID: {
      auto *Lit = cast<LiteralConstraint>(C);

      QualType T = Lit->getDefaultLiteralType(Sys.QC);
      if (FoundBindings.insert(T).second) {
         addBinding(Sys, TypeVar, T, Bindings, Score, OverloadIndex);
      }

      break;
   }
   case Constraint::MemberID: {
      auto *MC = cast<MemberConstraint>(C);

      // If we know the type of the member, we can guess the parent type
      // based on that.
      CanType MemberTy = Sys.getConcreteType(MC->getMemberType());
      if (!MemberTy->containsTypeVariable()) {
         if (FoundBindings.insert(MemberTy).second) {
            addBinding(Sys, TypeVar, MemberTy, Bindings, Score, OverloadIndex);
         }
      }

      break;

      // We can also guess based on the parent type if we know it and the
      // member is unambiguous.
//      QualType ParentTy = getConcreteType(MC->getConstrainedType());
//      if ((Cnt & GuessedBasedOnParentType) == 0
//          && MemberTy->isTypeVariableType()
//          && !ParentTy->containsTypeVariable()) {
//         Cnt |= GuessedBasedOnParentType;
//
//         const MultiLevelLookupResult *LookupRes;
//         if (auto Err = QC.MultiLevelTypeLookup(LookupRes, ParentTy,
//                                                MC->getMemberName())) {
//            updateStatus(Err.K);
//            return true;
//         }
//
//         if (LookupRes->empty() || !LookupRes->unique()) {
//            return false;
//         }
//
//         auto *ND = LookupRes->front().front();
//         QualType T = getTypeFor(ND, QC);
//
//         if (!T) {
//            return false;
//         }
//
//         pushAssignment(MemberTy->asTypeVariableType(), T);
//         return true;
//      }
//
//      return false;
   }
   case Constraint::ConditionalID: {
      auto *Cond = cast<ConditionalConstraint>(C);
      auto *DJ = Cond->getDisjunctionVar();

      if (!Sys.isAssigned(DJ)) {
         break;
      }

      auto Choice = Sys.getConstraintGraph().getOverloadChoice(DJ);
      assert(Choice != -1 && "no overload selected!");

      return findBindings(Sys, Cond->getConstraintAt(Choice), TypeVar,
                          Bindings, FoundBindings, Score,
                          OverloadIndex);
   }
   }
}

void TypeVariableBindingProducer::computeInitialBindings(TypeVariableType *T,
                                                         ConstraintSystem &Sys){
   assert(!Sys.isAssigned(T) && "don't call this on assigned type variables!");

   // If this variable has a direct binding, use only that one.
   if (Sys.hasConcreteBinding(T)) {
      auto *C = Sys.getFirstConstraint<TypeBindingConstraint>(T);
      findBindings(Sys, C, T, Bindings, FoundBindings, 0);

      return;
   }

   auto &CG = Sys.getConstraintGraph();

   unsigned Score = 0;
   if (QualType Pref = Sys.getPreferredBinding(T)) {
      FoundBindings.insert(Pref);
      Bindings.emplace_back(Pref, 0);

      Score += 1;
   }

   // Get all constraints associated with the type variable.
   ArrayRef<Constraint*> Constraints = CG.getOrAddNode(T)->getConstraints();

   // Walk over all constraints and see which types can be inferred from them.
   for (auto *C : Constraints) {
      findBindings(Sys, C, T, Bindings, FoundBindings, Score);
   }
}

PotentialBinding* TypeVariableBindingProducer::operator()()
{
   if (Index < Bindings.size()) {
      return &Bindings[Index++];
   }

   return nullptr;
}

void TypeVariableBindingProducer::nextStage()
{
   switch (Stage) {
   case Initial: Stage = BaseClasses; break;
   case BaseClasses: Stage = Protocols; break;
   case Protocols: Stage = Any; break;
   case Any: llvm_unreachable("should not move on from Any!");
   }
}

bool TypeVariableBindingProducer::computeFollowupBindings(ConstraintSystem &Sys)
{
   if (Stage == Any) {
      return false;
   }

   nextStage();

   switch (Stage) {
   case Initial: {
      llvm_unreachable("should never appear here!");
   }
   case BaseClasses: {
      bool FoundNew = false;
      auto &&Bindings = this->Bindings;
      this->Bindings.clear();

      for (auto &Binding : Bindings) {
         if (!Binding.Type->isClass()) {
            continue;
         }

         unsigned Score = Binding.Score;
         auto *Base = cast<ClassDecl>(Binding.Type->getRecord())
            ->getParentClass();

         while (Base) {
            Score += 1;

            CanType BaseBinding = Sys.QC.Context.getRecordType(Base);
            if (FoundBindings.insert(BaseBinding).second) {
               this->Bindings.emplace_back(BaseBinding, Score);
               FoundNew = true;
            }

            Base = Base->getParentClass();
         }
      }

      if (!FoundNew) {
         return computeFollowupBindings(Sys);
      }

      Index = 0;
      break;
   }
   case Protocols: {
      return computeFollowupBindings(Sys);
   }
   case Any: {
      this->Bindings.clear();

      // Finally, try the 'Any' type as a binding.
      ProtocolDecl *AnyDecl;
      Sys.QC.GetBuiltinProtocol(AnyDecl, GetBuiltinProtocolQuery::Any);

      QualType Any = Sys.QC.Context.getRecordType(AnyDecl);
      if (!FoundBindings.insert(Any).second) {
         return false;
      }

      Index = 0;
      Bindings.emplace_back(Any, 3);

      break;
   }
   }

   return !Bindings.empty();
}

BindTypeVariableStep::BindTypeVariableStep(
                        ConstraintSystem &Sys,
                        SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                        SmallVectorImpl<SolverStep*> &NextSteps,
                        TypeVariableType *TypeVar,
                        TypeVariableBindingProducer &&Producer,
                        ArrayRef<TypeVariableType*> TypeVariables,
                        unsigned Indent)
   : SolverStep(Sys, Solutions, NextSteps, Indent),
     TypeVar(TypeVar), Producer(std::move(Producer)),
     TypeVariables(TypeVariables)
{

}

void BindTypeVariableStep::execute(bool PrevFailed)
{
   return resume(PrevFailed);
}

void BindTypeVariableStep::resume(bool PrevFailed)
{
   PotentialBinding *NextBinding = Producer();
   if (!NextBinding) {
      // Don't produce follow-up bindings if we already found a solution
      // because their score will always be higher.
      if (!AnyValid && Producer.computeFollowupBindings(Sys)) {
         return resume(PrevFailed);
      }

      Scope = nullptr;
      return done();
   }

   // Begin a new solver scope.
   Scope = nullptr;
   Scope = std::make_unique<ConstraintSystem::SolverScope>(Sys);

   // Perform the next binding.
   auto &Binding = *NextBinding;
   Sys.bindTypeVariable(TypeVar, Binding.Type);
   Sys.increaseScore(Binding.Score);

   LOG("assigned ", QualType(TypeVar), " = ", Binding.Type, " (+",
       Binding.Score, ")\n");

   // If this solution's score is already worse than the best one, stop.
   if (Sys.getCurrentScore() > Sys.getBestScore()) {
      LOG_ERR("current score is worse than best score, backtracking...\n");

      Scope = nullptr;
      return suspend();
   }

   // If this solution is already not solvable, bail out and try the next
   // binding.
   if (auto *FailedConstraint = Sys.simplifyConstraints(TypeVar)) {
      if (Sys.stopAfterFirstFailure()) {
         LOG_ERR("failed constraint: ", *FailedConstraint, ", stopping...\n");
         return done(/*IsFailure=*/true);
      }

      LOG_ERR("failed constraint: ", *FailedConstraint, ", backtracking...\n");

      Scope = nullptr;
      return suspend();
   }

   // If all variables are bound, this must be a valid solution.
   if (Sys.getNumBoundTypeVariables() == TypeVariables.size()) {
      AnyValid = true;
   }

   // Create a new solver step within the scope of this binding.
   return suspend(new(Sys) FindSolutionStep(Sys, Solutions, NextSteps,
                                            TypeVariables, Indent + 1));
}

DisjunctionBindingProducer::DisjunctionBindingProducer(ConstraintSystem &Sys,
                                           DisjunctionConstraint *Disjunction) {
   computeInitialBindings(Sys, Disjunction);
}

void DisjunctionBindingProducer::computeInitialBindings(ConstraintSystem &Sys,
                                           DisjunctionConstraint *Disjunction) {
   SmallPtrSet<CanType, 4> FoundBindings;
   unsigned Score = 0;

   unsigned Index = 0;
   for (auto *C : Disjunction->getConstraints()) {
      findBindings(Sys, C, C->getConstrainedType(), Bindings,
                   FoundBindings, Score, Index++);
   }
}

PotentialBinding *DisjunctionBindingProducer::operator()()
{
   if (Index < Bindings.size()) {
      return &Bindings[Index++];
   }

   return nullptr;
}

DisjunctionChoiceStep::DisjunctionChoiceStep(ConstraintSystem &Sys,
                                             SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                                             SmallVectorImpl<SolverStep*> &NextSteps,
                                             DisjunctionConstraint *Disjunction,
                                             ArrayRef<TypeVariableType*> TypeVariables,
                                             unsigned Indent)
   : SolverStep(Sys, Solutions, NextSteps, Indent),
     TypeVar(Disjunction->getConstrainedType()),
     Producer(Sys, Disjunction),
     TypeVariables(TypeVariables)
{

}

void DisjunctionChoiceStep::execute(bool PrevFailed)
{
   return resume(PrevFailed);
}

void DisjunctionChoiceStep::resume(bool PrevFailed)
{
   PotentialBinding *NextBinding = Producer();
   if (!NextBinding) {
      Scope = nullptr;
      return done();
   }

   // Begin a new solver scope.
   Scope = nullptr;
   Scope = std::make_unique<ConstraintSystem::SolverScope>(Sys);

   // Perform the next binding.
   auto &Binding = *NextBinding;
   Sys.bindTypeVariable(TypeVar, Binding.Type, Binding.OverloadIndex);
   Sys.increaseScore(Binding.Score);

   LOG("assigned ", QualType(TypeVar), " = ", Binding.Type, " (+",
       Binding.Score, ") (overload #", Binding.OverloadIndex, ")\n");

   // If this solution's score is already worse than the best one, stop.
   if (Sys.getCurrentScore() > Sys.getBestScore()) {
      LOG_ERR("current score is worse than best score, backtracking...\n");

      Scope = nullptr;
      return suspend();
   }

   // If this solution is already not solvable, bail out and try the next
   // binding.
   if (auto *FailedConstraint = Sys.simplifyConstraints(TypeVar)) {
      if (Sys.stopAfterFirstFailure()) {
         LOG_ERR("failed constraint: ", *FailedConstraint, ", stopping...\n");
         return done(/*IsFailure=*/true);
      }

      LOG_ERR("failed constraint: ", *FailedConstraint, ", backtracking...\n");

      Scope = nullptr;
      return suspend();
   }

   // Create a new solver step within the scope of this binding.
   return suspend(new(Sys) FindSolutionStep(Sys, Solutions, NextSteps,
                                            TypeVariables, Indent + 1));
}