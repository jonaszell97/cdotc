//
// Created by Jonas Zell on 15.02.18.
//

#ifndef CDOT_CANDIDATESET_H
#define CDOT_CANDIDATESET_H

#include "AST/StmtOrDecl.h"
#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"
#include "ConversionSequence.h"
#include "Template.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Support/ErrorHandling.h>

#include <cstdint>
#include <vector>

namespace cdot {
namespace ast {
   class CallableDecl;
   class TemplateParamDecl;
   class Statement;
   class Expression;
   class SemaPass;
   class PrecedenceGroupDecl;
} // namespace ast

namespace sema {
   class ConstraintBuilder;
} // namespace sema

class FunctionType;

struct CandidateSet {
   enum CandStatus: unsigned char {
      Success,
      Ambiguous,
      NoMatch,
   };

   enum FailureReason: unsigned char {
      None,

      TooFewArguments,
      TooManyArguments,
      IncompatibleArgument,
      IncompatibleLabel,
      IncompatibleSelfArgument,
      IncompatibleReturnType,
      CouldNotInferArgumentType,
      ArgumentRequiresRef,

      CouldNotInferTemplateArg, // begin of fixed order, don't change
      ConflictingInferredArg,
      TooManyTemplateArgs,
      IncompatibleTemplateArgKind,
      IncompatibleTemplateArgVal,
      TemplateArgCovarianceError, // end of fixed order

      FailedConstraint,
      MustBeStatic,

      RecursivelyDependent,

      MutatingOnConstSelf,
      MutatingOnRValueSelf,

      IsDependent,
      IsInvalid, // invalid declaration, don't emit any extra diagnostics
   };

   struct Candidate {
      Candidate();

      explicit Candidate(ast::CallableDecl *Func, unsigned Distance = 0);
      explicit Candidate(ast::AliasDecl *Alias, unsigned Distance = 0);
      explicit Candidate(ast::Expression *FnExpr);
      explicit Candidate(ast::NamedDecl *FnDecl);

      ~Candidate();

      Candidate(const Candidate&) = delete;
      Candidate(Candidate&&) noexcept;

      Candidate &operator=(const Candidate&) = delete;
      Candidate &operator=(Candidate&&) noexcept;

      StmtOrDecl CandDecl;

      sema::TemplateArgList InnerTemplateArgs;
      sema::FinalTemplateArgumentList *OuterTemplateArgs = nullptr;

      FailureReason FR : 7;
      bool IsAnonymousCand : 1;
      uint8_t Distance;

      // Mapping from arguments to their assigned type after typechecking.
      std::unique_ptr<sema::ConstraintBuilder> Builder;

      union {
         uintptr_t ConversionPenalty = 0;
         uintptr_t Data1; // additional info dependent on the failure reason
      };

      uintptr_t Data2  = 0; // additional info dependent on the failure reason
      uintptr_t Data3  = 0; // additional info dependent on the failure reason

      bool isValid() const { return FR == None; }
      operator bool() const { return isValid(); }

      bool isAssignmentOperator();

      ast::CallableDecl *getFunc() const;
      ast::Expression *getFuncExpr() const;
      ast::AliasDecl *getAlias() const;
      ast::NamedDecl *getFuncDecl() const;

      void setCandDecl(StmtOrDecl D) { CandDecl = D; }

      FunctionType *getFunctionType() const;
      SourceLocation getSourceLoc() const;
      ast::PrecedenceGroupDecl *getPrecedenceGroup() const;

      unsigned getNumConstraints() const;

      bool isAnonymousCandidate() const
      {
         return IsAnonymousCand;
      }

      void setHasTooFewArguments(uintptr_t givenCount,
                                 uintptr_t expectedAtLeast) {
         FR = TooFewArguments;
         Data1 = givenCount;
         Data2 = expectedAtLeast;
      }

      void setHasTooManyArguments(uintptr_t givenCount,
                                  uintptr_t expectedAtMost) {
         FR = TooManyArguments;
         Data1 = givenCount;
         Data2 = expectedAtMost;
      }

      void setHasIncompatibleArgument(uintptr_t argIndex, QualType GivenTy,
                                      QualType NeededTy) {
         FR = IncompatibleArgument;
         Data1 = argIndex;
         Data2 = reinterpret_cast<uintptr_t>(GivenTy.getAsOpaquePtr());
         Data3 = reinterpret_cast<uintptr_t>(NeededTy.getAsOpaquePtr());
      }

      void setHasIncompatibleLabel(uintptr_t argIndex, IdentifierInfo *Label)
      {
         FR = IncompatibleLabel;
         Data1 = argIndex;
         Data2 = reinterpret_cast<uintptr_t>(Label);
      }

      void setHasIncompatibleReturnType(QualType Expected, QualType Found)
      {
         FR = IncompatibleReturnType;
         Data1 = reinterpret_cast<uintptr_t>(Expected.getAsOpaquePtr());
         Data2 = reinterpret_cast<uintptr_t>(Found.getAsOpaquePtr());
      }

      void setCouldNotInferArgumentType(uintptr_t argIndex)
      {
         FR = CouldNotInferArgumentType;
         Data1 = argIndex;
      }

      void setRequiresRef(uintptr_t argIndex)
      {
         FR = ArgumentRequiresRef;
         Data1 = argIndex;
      }

      void setHasIncompatibleSelfArgument(QualType expected, QualType given)
      {
         FR = IncompatibleSelfArgument;
         Data1 = reinterpret_cast<uintptr_t>(expected.getAsOpaquePtr());
         Data2 = reinterpret_cast<uintptr_t>(given.getAsOpaquePtr());
      }

      void setCouldNotInferTemplateArg(ast::TemplateParamDecl const* Decl)
      {
         FR = CouldNotInferTemplateArg;
         Data1 = reinterpret_cast<uintptr_t>(Decl);
      }

      void setHasConflictingInference(Type *conflicting, size_t idx)
      {
         FR = ConflictingInferredArg;
         Data1 = reinterpret_cast<uintptr_t>(conflicting);
         Data2 = idx;
      }

      void setHasTooManyTemplateArgs(uintptr_t givenCount,
                                     uintptr_t expectedAtMost) {
         FR = TooManyTemplateArgs;
         Data1 = givenCount;
         Data2 = expectedAtMost;
      }

      void setHasIncompatibleTemplateArgKind(unsigned diagSelect,
                                             size_t idx) {
         FR = IncompatibleTemplateArgKind;
         Data1 = diagSelect;
         Data2 = idx;
      }

      void setHasIncompatibleTemplateArgType(Type *given, size_t idx)
      {
         FR = IncompatibleTemplateArgVal;
         Data1 = reinterpret_cast<uintptr_t>(given);
         Data2 = idx;
      }

      void setTemplateArgListFailure(sema::TemplateArgListResult &res)
      {
         FR = (FailureReason)(res.ResultKind + CouldNotInferTemplateArg);
         Data1 = res.Data1;
         Data2 = res.Data2;
      }

      void setHasFailedConstraint(ast::Expression *Expr)
      {
         FR = FailedConstraint;
         Data1 = reinterpret_cast<uintptr_t>(Expr);
      }

      void setMutatingOnConstSelf()
      {
         FR = MutatingOnConstSelf;
      }

      void setMutatingOnRValueSelf()
      {
         FR = MutatingOnRValueSelf;
      }

      void setMustBeStatic()
      {
         FR = MustBeStatic;
      }

      void setRecursivelyDependent(SourceLocation Loc)
      {
         Data1 = Loc.getOffset();
         FR = RecursivelyDependent;
      }

      void setIsInvalid()
      {
         FR = IsInvalid;
      }

      void setIsDependent()
      {
         FR = IsDependent;
      }
   };

   CandidateSet()
      : Status(NoMatch), IncludesSelfArgument(false), InvalidCand(false),
        Dependent(false), MatchIdx((unsigned short)-1)
   {}

   CandidateSet(const CandidateSet&) = delete;
   CandidateSet(CandidateSet&&) noexcept;

   CandidateSet &operator=(const CandidateSet&) = delete;
   CandidateSet &operator=(CandidateSet&&) noexcept;

   Candidate *addCandidate(ast::CallableDecl *CD, unsigned Distance = 0);
   Candidate *addCandidate(ast::AliasDecl *Alias, unsigned Distance = 0);
   Candidate &addCandidate(ast::Expression *FnExpr);
   Candidate &addCandidate(ast::NamedDecl *FnDecl);

   void maybeUpdateBestConversionPenalty(uintptr_t Penalty)
   {
      if (Penalty < BestConversionPenalty)
         BestConversionPenalty = Penalty;
   }

   void foundMatch(unsigned MatchIdx)
   {
      Status = Success;
      this->MatchIdx = MatchIdx;
   }

   bool isAmbiguous() const
   {
      return Status == Ambiguous;
   }

   bool noMatchFound() const
   {
      return Status == NoMatch;
   }

   operator bool() const
   {
      return !isAmbiguous() && !noMatchFound();
   }

   void diagnose(ast::SemaPass &SP,
                 ast::Expression *SelfVal,
                 ArrayRef<ast::Expression *> args,
                 ArrayRef<ast::Expression *> templateArgs,
                 ast::Statement *Caller,
                 SourceLocation OpLoc = {}) {
      if (Status == NoMatch)
         return diagnoseFailedCandidates(SP, SelfVal, args, templateArgs,
                                         Caller, OpLoc);

      assert(Status == Ambiguous && "not a failed CandSet");
      return diagnoseAmbiguousCandidates(SP, Caller);
   }

   void diagnoseAlias(ast::SemaPass &SP, DeclarationName AliasName,
                      llvm::ArrayRef<ast::Expression *> templateArgs,
                      ast::Statement *Caller);

   void diagnoseFailedCandidates(ast::SemaPass &SP,
                                 ast::Expression *SelfVal,
                                 ArrayRef<ast::Expression *> args,
                                 ArrayRef<ast::Expression *> templateArgs,
                                 ast::Statement *Caller,
                                 SourceLocation OpLoc = {});

   void diagnoseAmbiguousCandidates(ast::SemaPass &SP, ast::Statement *Caller);

   Candidate &getBestMatch() { return Candidates[MatchIdx]; }

   bool isDependent() const
   {
      return Dependent;
   }

   CandStatus Status         : 8;
   bool IncludesSelfArgument : 1;
   bool InvalidCand          : 1;
   bool Dependent            : 1;
   unsigned MatchIdx         : 16;

   uintptr_t BestConversionPenalty = uintptr_t(-1);
   std::vector<ast::Expression*> ResolvedArgs;

   std::vector<Candidate> Candidates;
   SmallPtrSet<ast::NamedDecl*, 8> CandidateFns;
};

} // namespace cdot

#endif //CDOT_CANDIDATESET_H
