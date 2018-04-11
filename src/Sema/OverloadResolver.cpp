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
                                        llvm::ArrayRef<Expression*> args,
                                        llvm::SmallVectorImpl<QualType> &result,
                                        llvm::ArrayRef<QualType> neededArgs,
                                        CandidateSet::Candidate &Cand) {
   size_t i = 0;
   for (auto &GivenArg : args) {
      if (i >= neededArgs.size()) {
         auto Res = SP.visitExpr(GivenArg);
         if (!Res) {
            Cand.setIsInvalid();
         }

         result.push_back(Res.get()->getExprType());

         ++i;
         continue;
      }

      QualType NeededArg = neededArgs[i++];
      if (GivenArg->isContextDependent()) {
         if (!SP.ExprCanReturn(GivenArg, NeededArg)) {
            if (GivenArg->isInvalid()) {
               Cand.setIsInvalid();
            }
            else {
               if (!SP.visitExpr(GivenArg))
                  Cand.setIsInvalid();
               else
                  Cand.setHasIncompatibleArgument(i);
            }

            return false;
         }

         result.push_back(NeededArg);
      }
      else {
         result.push_back(GivenArg->getExprType());
      }
   }

   return true;
}

static bool nonStaticMethodCalledStatically(CandidateSet &CandSet,
                                            CandidateSet::Candidate &Cand) {
   return Cand.Func && Cand.Func->isNonStaticMethod()
          && !CandSet.IncludesSelfArgument;
}

static bool needDropFirstArgument(CandidateSet &CandSet,
                                  CandidateSet::Candidate &Cand) {
   return CandSet.IncludesSelfArgument
          && dyn_cast_or_null<MethodDecl>(Cand.Func);
}

static bool isMutatingMethod(CandidateSet::Candidate &Cand)
{
   auto M = dyn_cast_or_null<MethodDecl>(Cand.Func);
   return M && M->hasMutableSelf();
}

static bool incompatibleSelf(SemaPass &SP,
                             CandidateSet &CandSet,
                             CandidateSet::Candidate &Cand,
                             llvm::ArrayRef<Expression*> givenArgs) {
   if (auto M = dyn_cast_or_null<MethodDecl>(Cand.Func)) {
      if (CandSet.IncludesSelfArgument) {
         QualType givenSelf = givenArgs.front()->getExprType();
         if (givenSelf->isReferenceType())
            givenSelf = givenSelf->getReferencedType();

         if (!givenSelf->isRecordType()
               || givenSelf->getRecord() != M->getRecord()) {
            Cand.setHasIncompatibleSelfArgument(
               SP.getContext().getRecordType(M->getRecord()), givenSelf);
            return true;
         }
      }
   }

   return false;
}

void OverloadResolver::resolve(CandidateSet &CandSet)
{
   llvm::SmallVector<ConversionSequence, 4> Conversions;
   bool foundMatch = false;
   bool ambiguous  = false;

   for (auto &Cand : CandSet.Candidates) {
      assert(Cand.isBuiltinCandidate() || Cand.Func->isTemplate()
             || Cand.Func->getFunctionType()
                && "function without function type");

      if (Cand.Func) {
         if (!SP.ensureDeclared(Cand.Func) || Cand.Func->isInvalid()) {
            Cand.setIsInvalid();
            CandSet.InvalidCand = true;
            continue;
         }
      }

      if (nonStaticMethodCalledStatically(CandSet, Cand)) {
         Cand.setMustBeStatic();
         continue;
      }

      if (isMutatingMethod(Cand)) {
         auto Self = givenArgs.front();

         if (Self->isConst()) {
            Cand.setMutatingOnConstSelf();
            continue;
         }

         if (!Self->isLValue()) {
            Cand.setMutatingOnRValueSelf();
            continue;
         }
      }

      if (incompatibleSelf(SP, CandSet, Cand, givenArgs)) {
         continue;
      }

      if (needDropFirstArgument(CandSet, Cand)) {
         resolve(CandSet, Cand, givenArgs.drop_front(1), Conversions);
      }
      else {
         resolve(CandSet, Cand, givenArgs, Conversions);
      }

      if (Cand) {
         if (foundMatch) {
            if (CandSet.BestConversionPenalty == Cand.ConversionPenalty)
               ambiguous = true;
         }

         foundMatch = true;
         CandSet.maybeUpdateBestConversionPenalty(Cand.ConversionPenalty);

         CandSet.Conversions.resize(Conversions.size());
         std::move(Conversions.begin(), Conversions.end(),
                   CandSet.Conversions.begin());
      }

      Conversions.clear();
   }

   if (ambiguous)
      CandSet.Status = CandidateSet::Ambiguous;
   else if (foundMatch)
      CandSet.Status = CandidateSet::Success;
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
                               llvm::SmallVectorImpl<ConversionSequence>
                                                                 &Conversions) {
   FunctionType *FuncTy = Cand.getFunctionType();

   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   bool IsTemplate = Cand.Func
                     && (Cand.Func->isTemplate()
                         || Cand.Func->isInitializerOfTemplate()
                         || Cand.Func->isCaseOfTemplatedEnum());

   if (IsTemplate) {
      SourceLocation listLoc = givenTemplateArgs.empty()
                               ? Caller->getSourceLoc()
                               : givenTemplateArgs.front()->getSourceLoc();

      bool NeedOuterTemplateParams
         = isa<InitDecl>(Cand.Func) || isa<EnumCaseDecl>(Cand.Func);

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
         OuterTemplateArgs = TemplateArgList(
            SP, cast<RecordDecl>(Cand.Func->getDeclContext()),
            givenTemplateArgs, listLoc);

         TemplateArgs.addOuterList(OuterTemplateArgs);
      }

      std::vector<QualType> resolvedNeededArgs = FuncTy->getParamTypes();
      if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                       resolvedNeededArgs, Cand)) {
         return;
      }

      // try to infer unspecified template arguments from the function signature
      if (Caller && Caller->getContextualType()) {
         TemplateArgs.inferFromReturnType(Caller->getContextualType(),
                                          FuncTy->getReturnType());
      }

      TemplateArgs.inferFromArgList(resolvedGivenArgs, Cand.Func->getArgs());

      // if the template argument list is itself dependent, we have to delay
      // the overload resolution until instantiation time
      if (TemplateArgs.isStillDependent())
         return Cand.setIsDependent();

      auto comp = TemplateArgs.checkCompatibility();
      if (!comp)
         return Cand.setTemplateArgListFailure(comp);

      // if the function signature is in any way dependent, we need to
      // instantiate the full record declaration now to be able to substitute
      // the needed types
      if (NeedOuterTemplateParams) {
         bool DidInstantiate = SP.maybeInstantiateRecord(Cand,
                                                         move(OuterTemplateArgs),
                                                         Caller);

         auto R = Cand.Func->getRecord();
         if (R->isInvalid()) {
            CandSet.InvalidCand = true;
            return Cand.setIsInvalid();
         }

         // this 'steals' our outer template argument list, so we need to
         // update it
         if (DidInstantiate) {
            (void) TemplateArgs.pop_back_val();
            TemplateArgs.addOuterList(R->getTemplateArgs());
         }
      }

      // resolve the needed function arguments with the resolved template
      // arguments, i.e. turn
      //  `(T, i64) -> i64` into `(String, i64) -> i64`
      // with template parameter T = String
      {
         SemaPass::DeclScopeRAII declScopeRAII(SP, Cand.Func->getDeclContext());
         resolveTemplateArgs(resolvedNeededArgs, Cand.Func->getArgs(),
                             TemplateArgs);
      }

      FuncTy = SP.getContext().getFunctionType(FuncTy->getReturnType(),
                                               resolvedNeededArgs,
                                               FuncTy->getRawFlags());

      Cand.setFunctionType(FuncTy);

      // check the constraints here to take the resolved
      // template arguments into account
      if (!Cand.Func->getConstraints().empty()) {
         auto Res = SP.checkConstraints(Caller->getSourceLoc(), Cand.Func,
                                        TemplateArgs);

         if (auto C = Res.getFailedConstraint()) {
            return Cand.setHasFailedConstraint(C);
         }
      }
   }
   else if (!givenTemplateArgs.empty()) {
      return Cand.setHasTooManyTemplateArgs(givenTemplateArgs.size(), 0);
   }
   else if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                         FuncTy->getParamTypes(), Cand)) {
      return;
   }

   // find position of first argument with a default value
   size_t firstDefault = string::npos;
   if (Cand.Func) {
      firstDefault = 0;
      for (auto arg : Cand.Func->getArgs()) {
         if (arg->getDefaultVal())
            break;

         ++firstDefault;
      }
   }

   isCallCompatible(Cand, resolvedGivenArgs, FuncTy, Conversions, firstDefault);

   // check constraints if we haven't checked them already
   if (!IsTemplate) {
      if (Cand && Cand.Func) {
         auto Res = SP.checkConstraints(Caller->getSourceLoc(), Cand.Func);
         if (auto C = Res.getFailedConstraint()) {
            Cand.setHasFailedConstraint(C);
         }
      }
   }
}

namespace {

size_t castPenalty(const ConversionSequence &neededCast)
{
   size_t penalty = 0;

   // only implicit casts should occur here
   for (const auto &C : neededCast.getSteps()) {
      switch (C.getKind()) {
         case CastKind::NoOp:
         case CastKind::LValueToRValue:
         case CastKind::BitCast: // only u8* -> void* can appear as an
                                 // implicit bitcast
            break;
         case CastKind::Ext:
         case CastKind::FPExt:
         case CastKind::SignFlip:
         case CastKind::EnumToInt:
         case CastKind::IntToEnum:
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

void OverloadResolver::isCallCompatible(CandidateSet::Candidate &Cand,
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

   size_t i = 0;
   for (auto &neededArg : neededArgs) {
      auto& needed = neededArg;
      if (i < numGivenArgs) {
         auto givenTy = givenArgs[i];

         auto ConvSeq = SP.getConversionSequence(givenTy, needed);
         if (!ConvSeq.isImplicit()) {
            return Cand.setHasIncompatibleArgument(i);
         }

         Cand.ConversionPenalty += castPenalty(ConvSeq);
         Conversions.emplace_back(move(ConvSeq));
      }
      else if (i < firstDefaultArg) {
         return Cand.setHasTooFewArguments(numGivenArgs, numNeededArgs);
      }

      ++i;
   }

   if (isVararg)
      isVarargCallCompatible(Cand, givenArgs, FuncTy, Conversions);
}

void
OverloadResolver::isVarargCallCompatible(CandidateSet::Candidate &Cand,
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
         ConversionSequence &ConvSeq = Conversions.back();

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
         return Cand.setHasIncompatibleArgument(i);
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
         auto typeName = cast<GenericType>(arg->getType())
            ->getGenericTypeName();

         assert(resolvedArgs.back()->isDependentType()
                && "variadic expansion not reflected in arguments");

         resolvedArgs.pop_back();

         auto TA = templateArgs.getNamedArg(typeName);
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