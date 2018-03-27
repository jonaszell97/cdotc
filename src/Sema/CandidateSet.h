//
// Created by Jonas Zell on 15.02.18.
//

#ifndef CDOT_CANDIDATESET_H
#define CDOT_CANDIDATESET_H

#include "Basic/DeclarationName.h"
#include "Basic/Precedence.h"
#include "ConversionSequence.h"
#include "Template.h"

#include <cstdint>
#include <vector>
#include <llvm/Support/ErrorHandling.h>

namespace cdot {
namespace ast {
   class CallableDecl;
   class TemplateParamDecl;
   class Statement;
   class Expression;
   class SemaPass;
} // namespace ast

class FunctionType;

struct CandidateSet {
   enum CandStatus {
      Success,
      Ambiguous,
      NoMatch,
   };

   enum FailureReason {
      None,

      TooFewArguments,
      TooManyArguments,
      IncompatibleArgument,
      IncompatibleSelfArgument,

      CouldNotInferTemplateArg, // Must be kept in this order
      ConflictingInferredArg,
      TooManyTemplateArgs,
      IncompatibleTemplateArgKind,
      IncompatibleTemplateArgVal,

      FailedConstraint,
      MustBeStatic,

      MutatingOnConstSelf,
      MutatingOnRValueSelf,

      IsInvalid, // invalid declaration, don't emit any extra diagnostics
   };

   struct Candidate {
      Candidate() = default;

      explicit Candidate(ast::CallableDecl *Func)
         : Func(Func)
      {}

      explicit Candidate(FunctionType *FuncTy,
                         PrecedenceGroup PG = {},
                         op::OperatorKind OpKind = op::UnknownOp)
         : BuiltinCandidate{ FuncTy, PG, OpKind }
      {}

      ast::CallableDecl *Func = nullptr;
      sema::TemplateArgList TemplateArgs;

      struct {
         FunctionType *FuncTy = nullptr;
         PrecedenceGroup precedenceGroup;
         op::OperatorKind OpKind = op::UnknownOp;
      } BuiltinCandidate;

      FailureReason FR = None;
      union {
         uintptr_t ConversionPenalty = 0;
         uintptr_t Data1; // additional info dependent on the failure reason
      };

      uintptr_t Data2  = 0; // additional info dependent on the failure reason

      bool isValid() const
      {
         return FR == None;
      }

      operator bool() const
      {
         return isValid();
      }

      void setFunctionType(FunctionType *FTy)
      {
         BuiltinCandidate.FuncTy = FTy;
      }

      FunctionType *getFunctionType() const;
      SourceLocation getSourceLoc() const;
      PrecedenceGroup getPrecedenceGroup() const;

      bool isBuiltinCandidate() const
      {
         return BuiltinCandidate.OpKind != op::UnknownOp;
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

      void setHasIncompatibleArgument(uintptr_t argIndex)
      {
         FR = IncompatibleArgument;
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

      void setIsInvalid()
      {
         FR = IsInvalid;
      }
   };

   CandidateSet() = default;

   CandidateSet(const CandidateSet&) = delete;
   CandidateSet(CandidateSet&&)      = default;

   CandidateSet &operator=(const CandidateSet&) = delete;
   CandidateSet &operator=(CandidateSet&&)      = default;

   Candidate &addCandidate()
   {
      Candidates.emplace_back();
      return Candidates.back();
   }

   Candidate &addCandidate(ast::CallableDecl *CD)
   {
      Candidates.emplace_back(CD);
      return Candidates.back();
   }

   Candidate &addCandidate(FunctionType *FuncTy,
                           PrecedenceGroup PG = {},
                           op::OperatorKind OpKind = op::UnknownOp) {
      Candidates.emplace_back(FuncTy, PG, OpKind);
      return Candidates.back();
   }

   void maybeUpdateBestConversionPenalty(uintptr_t Penalty)
   {
      if (Penalty < BestConversionPenalty)
         BestConversionPenalty = Penalty;
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
                 DeclarationName funcName,
                 llvm::ArrayRef<ast::Expression *> args,
                 llvm::ArrayRef<ast::Expression *> templateArgs,
                 ast::Statement *Caller,
                 bool OperatorLookup = false,
                 SourceLocation OpLoc = {}) {
      if (Status == NoMatch)
         return diagnoseFailedCandidates(SP, funcName, args, templateArgs,
                                         Caller, OperatorLookup, OpLoc);

      assert(Status == Ambiguous && "not a failed CandSet");
      return diagnoseAmbiguousCandidates(SP, funcName, args, templateArgs,
                                         Caller, OperatorLookup, OpLoc);
   }

   void diagnoseFailedCandidates(ast::SemaPass &SP,
                                 DeclarationName funcName,
                                 llvm::ArrayRef<ast::Expression *> args,
                                 llvm::ArrayRef<ast::Expression *> templateArgs,
                                 ast::Statement *Caller,
                                 bool OperatorLookup = false,
                                 SourceLocation OpLoc = {});

   void diagnoseAnonymous(ast::SemaPass &SP,
                          DeclarationName funcName,
                          llvm::ArrayRef<ast::Expression *> args,
                          ast::Statement *Caller);

   void diagnoseAmbiguousCandidates(ast::SemaPass &SP,
                                    DeclarationName funcName,
                                    llvm::ArrayRef<ast::Expression *> args,
                                    llvm::ArrayRef<ast::Expression *> templateArgs,
                                    ast::Statement *Caller,
                                    bool OperatorLookup = false,
                                    SourceLocation OpLoc = {});

   Candidate &getBestMatch()
   {
      for (auto &C : Candidates)
         if (C && C.ConversionPenalty == BestConversionPenalty)
            return C;

      llvm_unreachable("no match found!");
   }

   bool IncludesSelfArgument = false;

   CandStatus Status = NoMatch;

   uintptr_t BestConversionPenalty = uintptr_t(-1);
   std::vector<ConversionSequence> Conversions;

   std::vector<Candidate> Candidates;
};

} // namespace cdot

#endif //CDOT_CANDIDATESET_H
