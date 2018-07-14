//
// Created by Jonas Zell on 08.11.17.
//

#include "OverloadResolver.h"

#include "Sema/SemaPass.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

OverloadResolver::OverloadResolver(SemaPass &SP,
                                   llvm::ArrayRef<Expression*> givenArgs,
                                   llvm::ArrayRef<Expression*>givenTemplateArgs,
                                   Statement *Caller)
      : SP(SP), givenArgs(givenArgs), givenTemplateArgs(givenTemplateArgs),
        Caller(Caller)
{

}

static bool resolveContextDependentArgs(SemaPass &SP,
                                        Statement *Caller,
                                        llvm::ArrayRef<Expression*> args,
                                        llvm::SmallVectorImpl<QualType> &result,
                                        llvm::ArrayRef<QualType> neededArgs,
                                        CandidateSet::Candidate &Cand,
                                        MultiLevelTemplateArgList
                                                      *TemplateArgs = nullptr,
                                        bool IsVariadic = false) {
   size_t i = 0;
   for (auto &GivenArg : args) {
      if (i >= neededArgs.size()) {
         if (GivenArg->isContextDependent()) {
            auto DefaultType = SP.GetDefaultExprType(GivenArg);
            if (!DefaultType) {
               if (GivenArg->isInvalid()) {
                  Cand.setIsInvalid();
                  return false;
               }

               Cand.setCouldNotInferArgumentType(i);
               return false;
            }

            result.push_back(DefaultType);
         }
         else {
            result.push_back(GivenArg->getExprType());
         }

         if (TemplateArgs && IsVariadic)
            TemplateArgs->inferFromType(result.back(), neededArgs.back(), true);

         ++i;
         continue;
      }

      QualType NeededArg = neededArgs[i];

      if (TemplateArgs)
         NeededArg = SP.resolveDependencies(NeededArg, *TemplateArgs, Caller);

      if (GivenArg->isContextDependent()) {
         if (auto LE = dyn_cast<LambdaExpr>(GivenArg)) {
            auto LambdaTy = SP.ResolveContextualLambdaExpr(LE, NeededArg);
            if (!LambdaTy) {
               if (LE->isInvalid()) {
                  Cand.setIsInvalid();
               }
               else if (auto Def = SP.GetDefaultExprType(LE)) {
                  Cand.setHasIncompatibleArgument(i, Def);
               }
               else {
                  Cand.setCouldNotInferArgumentType(i);
               }

               return false;
            }

            if (TemplateArgs)
               TemplateArgs->inferFromType(LambdaTy, NeededArg,
                                           IsVariadic
                                              && i == neededArgs.size() - 1);

            result.push_back(LambdaTy);

            ++i;
            continue;
         }

         int CanReturn = SP.ExprCanReturn(GivenArg, NeededArg);
         if (CanReturn == -1) {
            if (GivenArg->isInvalid()) {
               Cand.setIsInvalid();
            }
            else {
               if (auto Def = SP.GetDefaultExprType(GivenArg)) {
                  Cand.setHasIncompatibleArgument(i, Def);
               }
               else {
                  Cand.setCouldNotInferArgumentType(i);
               }
            }

            return false;
         }

         if (NeededArg->isDependentType()) {
            // we can't infer a context dependent expression from a
            // dependent type
            auto DefaultType = SP.GetDefaultExprType(GivenArg);
            if (DefaultType) {
               if (TemplateArgs)
                  TemplateArgs->inferFromType(DefaultType, NeededArg,
                                              IsVariadic
                                                 && i == neededArgs.size() - 1);

               result.push_back(DefaultType);
            }
            else if (NeededArg->isDependentType()) {
               result.push_back(NeededArg);
            }
            else {
               if (GivenArg->isInvalid()) {
                  Cand.setIsInvalid();
                  return false;
               }

               Cand.setCouldNotInferArgumentType(i);
               return false;
            }
         }
         else {
            Cand.ConversionPenalty += CanReturn;
            result.push_back(NeededArg);
         }
      }
      else {
         if (TemplateArgs)
            TemplateArgs->inferFromType(GivenArg->getExprType(), NeededArg,
                                        IsVariadic
                                           && i == neededArgs.size() - 1);

         result.push_back(GivenArg->getExprType());
      }

      ++i;
   }

   return true;
}

void OverloadResolver::resolve(CandidateSet &CandSet)
{
   SmallVector<ConversionSequenceBuilder, 4> Conversions;
   bool foundMatch       = false;
   bool MatchIsDependent = false;
   bool Dependent        = false;
   bool ambiguous        = false;

   unsigned BestMatch      = 0;
   unsigned BestMatchDistance = 0;
   unsigned MaxConstraints = 0;
   unsigned NumCandidates  = (unsigned)CandSet.Candidates.size();

   for (unsigned i = 0; i < NumCandidates; ++i) {
      auto &Cand = CandSet.Candidates[i];

      if (Cand.Func) {
         if (!SP.ensureDeclared(Cand.Func) || Cand.Func->isInvalid()) {
            Cand.setIsInvalid();
            CandSet.InvalidCand = true;
            continue;
         }
      }

      assert(Cand.isBuiltinCandidate() || Cand.Func->isTemplate()
             || Cand.Func->getFunctionType()
                && "function without function type");

      if (!Cand.isBuiltinCandidate() && Cand.Func->isStatic()
            && CandSet.IncludesSelfArgument) {
         resolve(CandSet, Cand, givenArgs.drop_front(1), Conversions);
      }
      else {
         resolve(CandSet, Cand, givenArgs, Conversions);
      }

#     ifndef NDEBUG
      if (Cand.FR == CandidateSet::IsDependent)
         assert(CandSet.Dependent
                && "didn't update Dependent flag on CandidateSet!");
      if (Cand.FR == CandidateSet::IsInvalid)
         assert(CandSet.InvalidCand
                && "didn't update InvalidCand flag on CandidateSet!");
#     endif

      bool IsDependent = Cand.FR == CandidateSet::IsDependent;
      Dependent |= IsDependent;

      if (Cand || IsDependent) {
         bool IsBetterMatch =
            (!foundMatch || BestMatchDistance >= Cand.Distance)
            && CandSet.BestConversionPenalty >= Cand.ConversionPenalty
            && Cand.getNumConstraints() >= MaxConstraints;

         if (foundMatch
                && CandSet.BestConversionPenalty == Cand.ConversionPenalty
                && Cand.getNumConstraints() == MaxConstraints) {
            // dependent candidates might not actually be valid at
            // instantiation time, so don't report an error
            if (Cand.FR != CandidateSet::IsDependent && !MatchIsDependent) {
               ambiguous = true;
            }
         }

         if (IsDependent) {
            MatchIsDependent = true;
            Cand.FR = CandidateSet::None;
         }
         else if (IsBetterMatch) {
            CandSet.Conversions.resize(Conversions.size());
            std::move(Conversions.begin(), Conversions.end(),
                      CandSet.Conversions.begin());

            BestMatch = i;
            MaxConstraints = Cand.getNumConstraints();
         }

         foundMatch = true;
         BestMatchDistance = Cand.Distance;
         CandSet.maybeUpdateBestConversionPenalty(Cand.ConversionPenalty);
      }

      Conversions.clear();
   }

   if (ambiguous) {
      CandSet.Status = CandidateSet::Ambiguous;
   }
   else if (foundMatch) {
      CandSet.Status = CandidateSet::Success;
      CandSet.MatchIdx = BestMatch;
   }
   else if (Dependent) {
      CandSet.Dependent = true;
   }
}

LLVM_ATTRIBUTE_UNUSED
static bool hasDependentSignature(CallableDecl *C)
{
   for (auto &Arg : C->getArgs())
      if (Arg->isDependent())
         return true;

   if (auto E = C->getReturnType().getTypeExpr())
      return E->isDependent();

   return false;
}

void OverloadResolver::resolve(CandidateSet &CandSet,
                               CandidateSet::Candidate &Cand,
                               llvm::ArrayRef<Expression*> givenArgs,
                               ConvSeqVec &Conversions) {
   FunctionType *FuncTy = Cand.getFunctionType();
   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   bool IsTemplate = !Cand.isBuiltinCandidate()
                     && (Cand.Func->isTemplate()
                         || Cand.Func->isInitializerOfTemplate()
                         || Cand.Func->isCaseOfTemplatedEnum());

   if (IsTemplate) {
      SourceLocation listLoc = givenTemplateArgs.empty()
                               ? Caller->getSourceLoc()
                               : givenTemplateArgs.front()->getSourceLoc();

      bool NeedOuterTemplateParams
         = (isa<InitDecl>(Cand.Func) || isa<EnumCaseDecl>(Cand.Func))
            && Cand.Func->getRecord()->isTemplate();

      Cand.InnerTemplateArgs = TemplateArgList(
         SP, Cand.Func,
         // if we have an initializer or enum case, the given template
         // arguments will be passed to the record parameter list
         NeedOuterTemplateParams ? llvm::ArrayRef<Expression*>()
                                 : givenTemplateArgs,
         listLoc);

      TemplateArgList OuterTemplateArgs;
      MultiLevelTemplateArgList TemplateArgs(Cand.InnerTemplateArgs);

      // initializers and enum cases also need their containing records
      // template arguments specified (or inferred)
      if (NeedOuterTemplateParams) {
         OuterTemplateArgs = TemplateArgList(SP, Cand.Func->getRecord(),
                                             givenTemplateArgs, listLoc);

         TemplateArgs.addOuterList(OuterTemplateArgs);
      }

      // try to infer unspecified template arguments from the function signature
      if (Caller && Caller->getContextualType()) {
         TemplateArgs.inferFromType(Caller->getContextualType(),
                                    FuncTy->getReturnType());
      }

      bool IsVariadic =
         !Cand.Func->getArgs().empty()
         && Cand.Func->getArgs().back()->isVariadicArgPackExpansion();

      std::vector<QualType> resolvedNeededArgs = FuncTy->getParamTypes();
      if (!resolveContextDependentArgs(SP, Caller, givenArgs, resolvedGivenArgs,
                                       resolvedNeededArgs, Cand, &TemplateArgs,
                                       IsVariadic)) {
         return;
      }

      // if the template argument list is itself dependent, we have to delay
      // the overload resolution until instantiation time
      if (TemplateArgs.isStillDependent()) {
         CandSet.Dependent = true;
         return Cand.setIsDependent();
      }

      auto comp = TemplateArgs.checkCompatibility();
      if (!comp)
         return Cand.setTemplateArgListFailure(comp);

      // if the function signature is in any way dependent, we need to
      // instantiate the full record declaration now to be able to substitute
      // the needed types
      if (NeedOuterTemplateParams) {
         SP.maybeInstantiateRecord(Cand, OuterTemplateArgs, Caller);

         auto R = Cand.Func->getRecord();
         if (R->isInvalid()) {
            CandSet.InvalidCand = true;
            return Cand.setIsInvalid();
         }
      }

      // resolve the needed function arguments with the resolved template
      // arguments, i.e. turn
      //  `(T, i64) -> i64` into `(String, i64) -> i64`
      // with template parameter T = String
      FuncTy = SP.getInstantiator()
                 .InstantiateFunctionType(Caller, Cand.Func,
                                          TemplateArgs.innermost());

      if (!FuncTy) {
         Cand.setIsInvalid();
         CandSet.InvalidCand = true;

         return;
      }

      Cand.setFunctionType(FuncTy);

      // check the constraints here to take the resolved
      // template arguments into account
      if (!Cand.Func->getConstraints().empty()) {
         auto Res = SP.checkConstraints(Caller, Cand.Func,
                                        TemplateArgs.outermost());

         if (auto C = Res.getFailedConstraint()) {
            return Cand.setHasFailedConstraint(C);
         }
      }
   }
   else if (!givenTemplateArgs.empty()) {
      return Cand.setHasTooManyTemplateArgs(givenTemplateArgs.size(), 0);
   }
   else if (!resolveContextDependentArgs(SP, Caller, givenArgs,
                                         resolvedGivenArgs,
                                         FuncTy->getParamTypes(), Cand)) {
      return;
   }

   // find position of first argument with a default value
   size_t firstDefault = string::npos;
   if (!Cand.isBuiltinCandidate()) {
      firstDefault = 0;
      for (auto arg : Cand.Func->getArgs()) {
         if (arg->getDefaultVal())
            break;

         ++firstDefault;
      }
   }

   isCallCompatible(CandSet, Cand, resolvedGivenArgs, FuncTy, Conversions,
                    firstDefault);

   // check constraints if we haven't checked them already
   if (!IsTemplate) {
      if (Cand && !Cand.isBuiltinCandidate()) {
         auto Res = SP.checkConstraints(Caller, Cand.Func);
         if (auto C = Res.getFailedConstraint()) {
            Cand.setHasFailedConstraint(C);
         }
      }
   }
}

namespace {

size_t castPenalty(const ConversionSequenceBuilder &neededCast)
{
   size_t penalty = 0;

   // only implicit casts should occur here
   for (const auto &C : neededCast.getSteps()) {
      switch (C.getKind()) {
      case CastKind::NoOp:
      case CastKind::Move:
      case CastKind::Forward:
      case CastKind::LValueToRValue:
      case CastKind::MutRefToRef:
      case CastKind::RValueToConstRef:
      case CastKind::BitCast: // only u8* -> void* can appear as an
                              // implicit bitcast
         break;
      case CastKind::Ext:
      case CastKind::FPExt:
      case CastKind::SignFlip:
      case CastKind::EnumToInt:
      case CastKind::IntToEnum:
      case CastKind::Copy:
      case CastKind::MutPtrToPtr:
      case CastKind::IsNull:
         penalty += 1;
         break;
      case CastKind::ProtoWrap:
         penalty += 2;
         break;
      case CastKind::UpCast:
         penalty += 2;
         break;
      case CastKind::ConversionOp:
         penalty += 3;
         break;
      default:
         llvm_unreachable("bad implicit cast kind!");
      }
   }

   return penalty;
}

} // anonympus namespace

void OverloadResolver::isCallCompatible(CandidateSet &CandSet,
                                        CandidateSet::Candidate &Cand,
                                        llvm::ArrayRef<QualType> givenArgs,
                                        FunctionType *FuncTy,
                                        ConvSeqVec &Conversions,
                                        size_t firstDefaultArg) {
   auto neededArgs = FuncTy->getParamTypes();
   size_t numGivenArgs = givenArgs.size();
   size_t numNeededArgs = neededArgs.size();

   if (numGivenArgs == 0 && numNeededArgs == 0)
      return;

   auto isVararg = FuncTy->isCStyleVararg() || FuncTy->isVararg();
   if (numGivenArgs > numNeededArgs && !isVararg)
      return Cand.setHasTooManyArguments(numGivenArgs, numNeededArgs);

   auto ParamInfo = FuncTy->getParamInfo();

   size_t i = 0;
   for (auto NeededTy : neededArgs) {
      if (i < numGivenArgs) {
         auto GivenTy = givenArgs[i];
         if (NeededTy->isUnknownAnyType()) {
            assert((SP.isInDependentContext() || Cand.isBuiltinCandidate())
                   && "argument of UnknownAny type in non-dependent context!");

            ++i;

            if (!Cand.isBuiltinCandidate()) {
               Cand.setIsDependent();
               CandSet.Dependent = true;
            }

            continue;
         }

         if (GivenTy->isDependentType() || NeededTy->isDependentType()) {
            Cand.setIsDependent();
            CandSet.Dependent = true;

            ++i;
            continue;
         }

         ConversionSequenceBuilder ConvSeq;
         bool AddCopy = false;

         // allow implicit conversion from mutable reference to mutable
         // borrow for self argument
         if (i == 0
             && !Cand.isBuiltinCandidate()
             && Cand.Func->isNonStaticMethod()
             && NeededTy->isMutableBorrowType()
             && GivenTy->isMutableReferenceType()) {
            GivenTy = SP.getContext().getMutableBorrowType(
               GivenTy->stripReference());

            ConvSeq.addStep(CastKind::BitCast, GivenTy);
         }

         // check parameter passing convention
         auto &NeededParamInfo = ParamInfo[i];
         switch (NeededParamInfo.getConvention()) {
         case ArgumentConvention::Default:
            llvm_unreachable("didn't remove default convention!");
         case ArgumentConvention::Borrowed:
            if (GivenTy->isMutableBorrowType()) {
               ++Cand.ConversionPenalty;
            }

            // if we're given a mutable reference, we need to pass an
            // immutable one
            if (GivenTy->isMutableReferenceType()) {
               ConvSeq.addStep(CastKind::MutRefToRef,
                               SP.getContext().getReferenceType(
                                  GivenTy->getReferencedType()));
            }

            break;
         case ArgumentConvention::MutablyBorrowed: {
            // the left hand side of an assignment does not need to be
            // explicitly borrowed
            if (Cand.isAssignmentOperator() && i == 0) {
               if (!NeededTy->isMutableReferenceType())
                  NeededTy = SP.getContext()
                               .getMutableReferenceType(
                                  NeededTy->stripReference());
            }
            else if (!NeededTy->isMutableBorrowType()) {
               NeededTy = SP.getContext()
                            .getMutableBorrowType(NeededTy->stripReference());
            }

            break;
         }
         case ArgumentConvention::Owned: {
            // requires passing ownership; for a temporary, this is a noop
            // since we can forward, but for an lvalue it's a move

            // if the type is implicitly copyable, pass a copy instead
            if (GivenTy->isReferenceType()) {
               QualType Deref = GivenTy->getReferencedType();
               if (Deref->isRefcounted()) {
                  // can't pass a non-mutable reference to an owned parameter
//                  if (GivenTy->isNonMutableReferenceType()) {
//                     Cand.setRequiresRef(i);
//                     return;
//                  }

                  AddCopy = true;
               }
               else if (SP.IsImplicitlyCopyableType(Deref)) {
                  AddCopy = true;
               }
               else {
                  // can't pass a non-mutable reference to an owned parameter
                  if (GivenTy->isNonMutableReferenceType()) {
                     Cand.setRequiresRef(i);
                     return;
                  }

                  ConvSeq.addStep(CastKind::Forward, GivenTy);
               }
            }
            // if it's an rvalue, forward it
            else {
               ConvSeq.addStep(CastKind::Forward, GivenTy);
            }

            break;
         }
         }

         // parameter requires a reference type
         if (NeededTy->isReferenceType() && !GivenTy->isReferenceType()) {
            Cand.setRequiresRef(i);
            return;
         }

         // parameter requires a mutable reference type
         if (NeededTy->isMutableReferenceType()
             && !GivenTy->isMutableReferenceType()) {
            Cand.setRequiresRef(i);
            return;
         }

         SP.getConversionSequence(ConvSeq, GivenTy, NeededTy);
         if (!ConvSeq.isImplicit()) {
            return Cand.setHasIncompatibleArgument(i, GivenTy);
         }

         if (AddCopy)
            ConvSeq.addStep(CastKind::Copy, ConvSeq.getSteps().back()
                                                   .getResultType());

         Cand.ConversionPenalty += castPenalty(ConvSeq);
         Conversions.emplace_back(move(ConvSeq));
      }
      else if (i < firstDefaultArg) {
         return Cand.setHasTooFewArguments(numGivenArgs, numNeededArgs);
      }

      ++i;
   }

   if (isVararg)
      isVarargCallCompatible(CandSet, Cand, givenArgs, FuncTy, Conversions);
}

void
OverloadResolver::isVarargCallCompatible(CandidateSet &CandSet,
                                         CandidateSet::Candidate &Cand,
                                         llvm::ArrayRef<QualType> givenArgs,
                                         FunctionType *FuncTy,
                                         ConvSeqVec &Conversions) {
   assert(FuncTy->isCStyleVararg() || FuncTy->isVararg());

   auto neededArgs = FuncTy->getParamTypes();

   // cstyle vararg accepts all types...
   if (FuncTy->isCStyleVararg()) {
      for (size_t i = neededArgs.size(); i < givenArgs.size(); ++i) {
         auto &arg = givenArgs[i];

         Conversions.emplace_back();
         ConversionSequenceBuilder &ConvSeq = Conversions.back();

         // ...and only takes rvalues
         if (arg->isReferenceType()) {
            ConvSeq.addStep(CastKind::LValueToRValue, arg->getReferencedType());
         }
         else {
            ConvSeq.addStep(CastKind::NoOp, arg);
         }
      }

      return;
   }

   auto vaTy = neededArgs.back();

   // we only need to check the remaining arguments, others have been checked
   // by isCallCompatible
   for (size_t i = neededArgs.size(); i < givenArgs.size(); ++i) {
      auto& given = givenArgs[i];

      auto ConvSeq = SP.getConversionSequence(given, vaTy);
      if (!ConvSeq.isImplicit()) {
         return Cand.setHasIncompatibleArgument(i, given);
      }
      if (!vaTy.isNull() && given != vaTy) {
         Cand.ConversionPenalty += castPenalty(ConvSeq);
      }
   }
}

void
OverloadResolver::resolveTemplateArgs(std::vector<QualType> &resolvedArgs,
                                      llvm::ArrayRef<FuncArgDecl*> neededArgs,
                                      const MultiLevelTemplateArgList
                                                               &templateArgs) {
   size_t i = 0;
   for (auto &arg : neededArgs) {
      auto Expr = arg->getType().getTypeExpr();
      if (Expr && Expr->isVariadicArgPackExpansion()) {
         assert(resolvedArgs.back()->isDependentType()
                && "variadic expansion not reflected in arguments");

         resolvedArgs.pop_back();

         auto TA = templateArgs.getArgForParam(cast<GenericType>(arg->getType())
            ->getParam());

         assert(TA && "missing template argument");

         for (auto &VA : TA->getVariadicArgs()) {
            resolvedArgs.emplace_back(VA.getType());
         }

         break;
      }

      auto ty = arg->getType().getResolvedType();
      auto resolvedTy = QualType(SP.resolveDependencies(ty, templateArgs,
                                                        Caller));

      if (resolvedTy)
         resolvedArgs[i] = resolvedTy;
      else
         resolvedTy = ty;

      ++i;
   }
}

} // namespace ast
} // namespace cdot