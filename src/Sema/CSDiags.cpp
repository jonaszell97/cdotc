#include "ConstraintSystem.h"

#include "AST/Decl.h"
#include "AST/TypeVisitor.h"
#include "Query/QueryContext.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using OverloadCandidate = CandidateSet::Candidate;

static DisjunctionConstraint *getDisjunctionConstraint(ConstraintSystem &Sys,
                                                       Constraint *Other) {
   if (!Sys.isOverloadChoice(Other->getConstrainedType())) {
      return nullptr;
   }

   for (auto *C : Sys.getConstraintGraph()
                 .getOrAddNode(Other->getConstrainedType())->getConstraints()) {
      if (auto *DJ = dyn_cast<DisjunctionConstraint>(C)) {
         return DJ;
      }
   }

   llvm_unreachable("not an overload choice!");
}

static bool diagnoseDisjunctionFailure(ConstraintSystem &Sys,
                                       QualType RequiredType,
                                       SourceRange RequiredTypeLoc,
                                       DisjunctionConstraint *DJ) {
   auto *Loc = DJ->getLocator();
   if (!Loc) {
      return false;
   }

   auto Name = Loc->getPathElements().back().getDeclarationName();
   Sys.QC.Sema->diagnose(err_generic_error,
      "no visible overload of '" + Name.toString() + "' has type "
      + RequiredType.toDiagString(),
      Loc->getAnchor()->getSourceRange(),
      RequiredTypeLoc);

   for (auto *C : DJ->getConstraints()) {
      auto *DeclLoc = C->getLocator();
      if (!DeclLoc) {
         continue;
      }

      Sys.QC.Sema->diagnose(note_candidate_here,
         DeclLoc->getPathElements().back().getSourceRange());
   }

   return true;
}

static bool diagnoseConversionFailure(ConstraintSystem &Sys,
                                      ImplicitConversionConstraint *C,
                                      OverloadCandidate *Cand) {
   QualType BoundTy = Sys.getConcreteType(C->getConstrainedType());
   QualType RHSTy = Sys.getConcreteType(C->getRHSType(), C->getConstrainedType());

   SourceRange ExprLoc;
   SourceRange TypeLoc;

   if (auto *L = C->getLocator()) {
      ExprLoc = L->getAnchor()->getSourceRange();

      auto Elements = L->getPathElements();
      if (!Elements.empty()
      && Elements.back().getKind() == ConstraintLocator::ContextualType) {
         TypeLoc = Elements.front().getSourceRange();
      }
   }

   if (auto *DJ = getDisjunctionConstraint(Sys, C)) {
      if (diagnoseDisjunctionFailure(Sys, RHSTy, TypeLoc, DJ)) {
         return true;
      }
   }

   if (Cand) {
      auto *Loc = C->getLocator();
      auto Elements = Loc->getPathElements();

      if (Elements.empty()
      || Elements.back().getKind() != ConstraintLocator::ParameterType) {
         return false;
      }

      auto *ParmDecl = Elements.back().getParamDecl();
      if (ParmDecl->isSelf()) {
         if (BoundTy->isMetaType() && !RHSTy->isMetaType()
         && BoundTy->removeMetaType() == RHSTy) {
            Cand->setMustBeStatic();
         }
         else {
            Cand->setHasIncompatibleSelfArgument(RHSTy, BoundTy);
         }
      }
      else {
         unsigned Index = 0;
         for (auto *Decl : cast<CallableDecl>(ParmDecl->getDeclContext())
                                                      ->getArgs()) {
            if (Decl == ParmDecl) {
               break;
            }

            ++Index;
         }

         Cand->setHasIncompatibleArgument(Index, BoundTy, RHSTy);
      }
   }
   else {
      Sys.QC.Sema->diagnose(err_no_implicit_conv, ExprLoc, TypeLoc,
                            BoundTy, RHSTy);
   }

   return true;
}

static bool diagnoseCovarianceFailure(ConstraintSystem &Sys,
                                      CovarianceConstraint *C,
                                      ConstraintSystem::SolutionBindings &Bindings,
                                      OverloadCandidate *Cand) {
   QualType BoundTy = Sys.getConcreteType(C->getConstrainedType());
   QualType RHSTy = Sys.getConcreteType(C->getRHSType(), C->getConstrainedType());

   SourceRange ExprLoc;
   SourceRange TypeLoc;

   if (auto *L = C->getLocator()) {
      auto Elements = L->getPathElements();
      if (!Elements.empty()
          && Elements.back().getKind() == ConstraintLocator::ContextualType) {
         TypeLoc = Elements.front().getSourceRange();
      }
   }

   auto *Loc = C->getLocator();
   auto Elements = Loc->getPathElements();

   if (Elements.empty()
   || Elements.back().getKind() != ConstraintLocator::TemplateParam) {
      return false;
   }

   auto *param = Elements.back().getTemplateParamDecl();
   (void) param;

   if (Cand) {
      FuncArgDecl *argDecl = nullptr;
      for (auto *cs : Sys.getConstraintGraph().getActiveConstraints()) {
         if (cs->getConstrainedType() != C->getConstrainedType()) {
            continue;
         }

         auto *otherLoc = cs->getLocator();
         auto otherElements = otherLoc->getPathElements();

         if (otherElements.empty()
         || otherElements.back().getKind() != ConstraintLocator::ParameterType) {
            continue;
         }

         argDecl = otherElements.back().getParamDecl();
         break;
      }

      if (!argDecl) {
         return false;
      }

      unsigned Index = 0;
      for (auto *Decl : cast<CallableDecl>(argDecl->getDeclContext())->getArgs()) {
         if (Decl == argDecl) {
            break;
         }

         ++Index;
      }

      Cand->setHasIncompatibleArgument(Index, BoundTy, RHSTy);
   }
   else {
      Sys.QC.Sema->diagnose(err_no_implicit_conv, ExprLoc, TypeLoc,
                            BoundTy, RHSTy);
   }

   return true;
}

static bool diagnoseMemberFailure(ConstraintSystem &Sys,
                                  MemberConstraint *C,
                                  OverloadCandidate *Cand) {
   auto *L = C->getLocator();
   if (!L) {
      return false;
   }

   auto Elements = L->getPathElements();
   if (Elements.back().getKind() != ConstraintLocator::MemberReference) {
      return false;
   }

   QualType BoundTy = Sys.getConcreteType(C->getConstrainedType());
   QualType MemberTy = Sys.getConcreteType(C->getMemberType());

   LookupOpts Opts = DefaultLookupOpts;
   if (MemberTy->isMetaType()) {
      Opts |= LookupOpts::TypeLookup;
   }

   const MultiLevelLookupResult *LookupRes;
   if (Sys.QC.MultiLevelTypeLookup(LookupRes, BoundTy,
                                   C->getMemberName(), Opts)) {
      return false;
   }

   if (LookupRes->empty()) {
      Sys.QC.Sema->diagnose(err_generic_error,
                            "type '" + BoundTy.toDiagString()
                            + "' does not have a member named '"
                            + L->getPathElements().back()
                               .getDeclarationName().toString() + "'",
                            L->getAnchor()->getSourceRange());
   }
   else {
      Sys.QC.Sema->diagnose(err_generic_error,
                            "type '" + BoundTy.toDiagString()
                            + "' does not have a member named '"
                            + L->getPathElements().back()
                               .getDeclarationName().toString()
                            + "' of type '" + MemberTy.toDiagString() + "'",
                            L->getAnchor()->getSourceRange());

      for (auto *ND : LookupRes->allDecls()) {
         // FIXME 'candidate of type XXX here'
         Sys.QC.Sema->diagnose(note_candidate_here, ND->getSourceLoc());
      }
   }

   return true;
}

static bool diagnoseLiteralFailre(ConstraintSystem &Sys, LiteralConstraint *LC,
                                  OverloadCandidate *Cand) {
   if (!LC->getLocator()) {
      return false;
   }

   StringRef Desc;
   switch (LC->getLiteralKind()) {
   case LiteralConstraint::IntegerLiteral:
      Desc = "integer literal";
      break;
   case LiteralConstraint::FPLiteral:
      Desc = "floating point literal";
      break;
   case LiteralConstraint::BoolLiteral:
      Desc = "boolean literal";
      break;
   case LiteralConstraint::GraphemeClasterLiteral:
      Desc = "grapheme cluster literal";
      break;
   case LiteralConstraint::CodePointLiteral:
      Desc = "code point literal";
      break;
   case LiteralConstraint::ASCIILiteral:
      Desc = "ascii literal";
      break;
   case LiteralConstraint::StringLiteral:
      Desc = "string literal";
      break;
   case LiteralConstraint::ArrayLiteral:
      Desc = "array literal";
      break;
   case LiteralConstraint::DictionaryLiteral:
      Desc = "dictionary literal";
      break;
   case LiteralConstraint::NoneLiteral:
      Desc = "none literal";
      break;
   }

   SourceRange ExprLoc;
   SourceRange TypeLoc;

   if (auto *L = LC->getLocator()) {
      ExprLoc = L->getAnchor()->getSourceRange();

      auto Elements = L->getPathElements();
      if (!Elements.empty()
      && Elements.back().getKind() == ConstraintLocator::ContextualType) {
         TypeLoc = Elements.front().getSourceRange();
      }
   }

   QualType BoundTy = Sys.getConcreteType(LC->getConstrainedType());

   if (Cand) {
      // There must be a binding / conversion constraint elsewhere that is
      // not satisfied.
      auto *Conv = Sys.getFirstConstraint<ImplicitConversionConstraint>(
         LC->getConstrainedType());

      Sys.bindTypeVariable(LC->getConstrainedType(),
                           LC->getDefaultLiteralType(Sys.QC),
                           -1, true);

      return diagnoseConversionFailure(Sys, Conv, Cand);
   }
   else {
      Sys.QC.Sema->diagnose(err_generic_error,
         Desc.str() + " cannot produce value of type '"
         + BoundTy.toDiagString() + "'",
         ExprLoc, TypeLoc);
   }

   return true;
}

namespace {

class TypeEquivalenceChecker: public TypeComparer<TypeEquivalenceChecker> {
   /// Reference to the constraint system.
   ConstraintSystem &Sys;

   /// The equality constraint.
   Constraint *C;

   SourceRange getSourceRange()
   {
      if (auto *L = C->getLocator()) {
         return L->getAnchor()->getSourceRange();
      }

      return SourceRange();
   }

public:
   TypeEquivalenceChecker(ConstraintSystem &Sys,
                          Constraint *C)
      : Sys(Sys), C(C)
   { }

   /// Set to true once we diagnosed an issue.
   bool diagnosedIssue = false;

   bool visitMutablePointerType(MutablePointerType *LHS, QualType RHS)
   {
      if (!RHS->isMutablePointerType()) {
         Sys.QC.Sema->diagnose(err_generic_error,
            "pointer must be mutable", getSourceRange());

         diagnosedIssue = true;
         return false;
      }

      return true;
   }

   bool visitReferenceType(ReferenceType *LHS, QualType RHS)
   {
      if (RHS->getTypeID() != Type::ReferenceTypeID) {
         Sys.QC.Sema->diagnose(
            err_generic_error,
            "reference type expected", getSourceRange());

         diagnosedIssue = true;
         return false;
      }

      return false;
   }

   bool visitMutableReferenceType(MutableReferenceType *LHS, QualType RHS)
   {
      if (RHS->getTypeID() != Type::MutableReferenceTypeID) {
         Sys.QC.Sema->diagnose(
            err_generic_error,
            "mutable reference type expected", getSourceRange());

         diagnosedIssue = true;
         return false;
      }

      return false;
   }
};

} // anonymous namespace

static bool diagnoseEqualityFailure(ConstraintSystem &Sys,
                                    TypeEqualityConstraint *EC,
                                    OverloadCandidate *Cand) {
   TypeEquivalenceChecker checker(Sys, EC);

   QualType LHS = Sys.getConcreteType(EC->getConstrainedType());
   QualType RHS = Sys.getConcreteType(EC->getRHSType(), EC->getConstrainedType());
   checker.visit(RHS, LHS);

   return checker.diagnosedIssue;
}

static bool checkUninferrableTemplateParam(ConstraintSystem &Sys,
                                           OverloadCandidate *Cand) {
   if (!Cand) {
      return false;
   }

   TypeVariableType *ParamVar = nullptr;
   for (auto *TV : Sys.getTypeVariables()) {
      if (Sys.representsTemplateParam(TV) && !Sys.isAssigned(TV)) {
         ParamVar = TV;
         break;
      }
   }

   if (!ParamVar) {
      return false;
   }

   // Get the covariance conversion constraint.
   auto *Conv = Sys.getFirstConstraint<CovarianceConstraint>(ParamVar);
   if (!Conv) {
      return false;
   }

   auto *Loc = Conv->getLocator();
   if (!Loc || Loc->getPathElements().empty()
         || Loc->getPathElements().front().getKind()
            != ConstraintLocator::TemplateParam) {
      return false;
   }

   auto *Param = Loc->getPathElements().back().getTemplateParamDecl();
   Cand->setCouldNotInferTemplateArg(Param);

   return true;
}

static bool checkIncompleteInformation(ConstraintSystem &Sys,
                                       OverloadCandidate *Cand) {
   MemberConstraint *MC = nullptr;
   for (auto *C : Sys.getConstraintGraph().getActiveConstraints()) {
      if (auto *Mem = dyn_cast<MemberConstraint>(C)) {
         if (!Sys.isAssigned(Mem->getConstrainedType())) {
            MC = Mem;
            break;
         }
      }
   }

   if (!MC || !MC->getLocator()) {
      return false;
   }

   if (Cand) {
      auto *Loc = MC->getLocator();
      auto Elements = Loc->getPathElements();

      if (Elements.empty()
      || Elements.back().getKind() != ConstraintLocator::ParameterType) {
         return false;
      }

      llvm_unreachable("TODO");
   }
   else {
      Sys.QC.Sema->diagnose(err_generic_error,
                            "reference to member '"
                            + MC->getMemberName().toString() +
                            "' cannot be resolved without a contextual type",
                            MC->getLocator()->getAnchor()->getSourceRange());
   }

   return true;
}

static bool diagnoseFailureImpl(ConstraintSystem &Sys,
                                Constraint *FailedConstraint,
                                ConstraintSystem::SolutionBindings &Bindings,
                                OverloadCandidate *Cand = nullptr) {
   switch (FailedConstraint->getKind()) {
   case Constraint::CovarianceID: {
      auto *Cov = cast<CovarianceConstraint>(FailedConstraint);
      if (diagnoseCovarianceFailure(Sys, Cov, Bindings, Cand)) {
         return true;
      }

      break;
   }
   case Constraint::ImplicitConversionID: {
      auto *Conv = cast<ImplicitConversionConstraint>(FailedConstraint);
      if (diagnoseConversionFailure(Sys, Conv, Cand)) {
         return true;
      }

      break;
   }
   case Constraint::MemberID: {
      auto *MC = cast<MemberConstraint>(FailedConstraint);
      if (diagnoseMemberFailure(Sys, MC, Cand)) {
         return true;
      }

      break;
   }
   case Constraint::LiteralID: {
      auto *LC = cast<LiteralConstraint>(FailedConstraint);
      if (diagnoseLiteralFailre(Sys, LC, Cand)) {
         return true;
      }

      break;
   }
   case Constraint::TypeEqualityID: {
      auto *EC = cast<TypeEqualityConstraint>(FailedConstraint);
      if (diagnoseEqualityFailure(Sys, EC, Cand)) {
         return true;
      }

      break;
   }
   case Constraint::TypeBindingID: {
      // If a type binding fails, there has to be some other constraint that
      // violates that binding.
      auto constraints = Sys.getConstraintGraph().getOrAddNode(
         FailedConstraint->getConstrainedType())->getConstraints();

      for (auto *C : constraints) {
         switch (C->getKind()) {
         case Constraint::ImplicitConversionID:
         case Constraint::MemberID:
         case Constraint::LiteralID:
         case Constraint::TypeEqualityID:
            return diagnoseFailureImpl(Sys, C, Bindings, Cand);
         default:
            break;
         }
      }

      break;
   }
   default:
      break;
   }

   return false;
}

static bool diagnoseFailureImpl(ConstraintSystem &Sys,
                                ConstraintSystem::SolutionBindings &Bindings,
                                OverloadCandidate *Cand = nullptr) {
   // Check if there was incomplete contextual information for a constraint.
   if (checkIncompleteInformation(Sys, Cand)) {
      return true;
   }
   if (checkUninferrableTemplateParam(Sys, Cand)) {
      return true;
   }
   
   auto *FailedConstraint = Sys.FailedConstraint;
   if (!FailedConstraint) {
      return false;
   }

   return diagnoseFailureImpl(Sys, FailedConstraint, Bindings, Cand);
}

bool ConstraintSystem::diagnoseFailure(SolutionBindings &Bindings)
{
   return diagnoseFailureImpl(*this, Bindings);
}

bool ConstraintSystem::diagnoseCandidateFailure(CandidateSet::Candidate &Cand,
                                                SolutionBindings &Bindings)
{
   return diagnoseFailureImpl(*this, Bindings, &Cand);
}

bool diagnoseAmbiguousOverloadChoice(ConstraintSystem &Sys,
                                     const ConstraintSystem::Solution &S1,
                                     const ConstraintSystem::Solution &S2) {
   for (auto *TypeVar : Sys.getTypeVariables()) {
      if (!Sys.isOverloadChoice(TypeVar)) {
         continue;
      }

      auto It1 = S1.AssignmentMap.find(TypeVar);
      assert(It1 != S1.AssignmentMap.end() && "type variable not assigned!");

      auto It2 = S2.AssignmentMap.find(TypeVar);
      assert(It2 != S2.AssignmentMap.end() && "type variable not assigned!");

      CanType Choice1 = It1->getSecond();
      CanType Choice2 = It2->getSecond();

      if (Choice1 == Choice2) {
         continue;
      }

      auto *DJ = Sys.getFirstConstraint<DisjunctionConstraint>(TypeVar);
      assert(DJ && "not a disjunction!");

      auto *Loc = DJ->getLocator();
      if (!Loc) {
         return false;
      }

      auto Name = Loc->getPathElements().back().getDeclarationName();
      Sys.QC.Sema->diagnose(
         err_generic_error, "ambiguous reference to '"
         + Name.toString() + "'", Loc->getAnchor()->getSourceRange());

      for (auto *C : DJ->getConstraints()) {
         auto *DeclLoc = C->getLocator();
         if (!DeclLoc) {
            continue;
         }

         Sys.QC.Sema->diagnose(
            note_candidate_here,
            DeclLoc->getPathElements().back().getSourceRange().getStart());
      }

      return true;
   }

   return false;
}

bool ConstraintSystem::diagnoseAmbiguity(const Solution &S1, const Solution &S2)
{
   // Try to find an ambiguous overload choice.
   if (diagnoseAmbiguousOverloadChoice(*this, S1, S2)) {
      return true;
   }

#ifndef NDEBUG
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      printConstraints(OS);
   }

   QC.Sema->diagnose(err_generic_error, "ambiguous solution for system:\n" + s, Loc);

   std::string s1;
   std::string s2;

   {
      llvm::raw_string_ostream OS(s1);
      printSolution(S1, OS);
   }
   {
      llvm::raw_string_ostream OS(s2);
      printSolution(S2, OS);
   }

   QC.Sema->diagnose(note_generic_note, "possible assignment:\n" + s1);
   QC.Sema->diagnose(note_generic_note, "possible assignment:\n" + s2);

   return true;
#endif

   return false;
}