//
// Created by Jonas Zell on 08.11.17.
//

#include "OverloadResolver.h"

#include "AST/NamedDecl.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "Variant/Type/Type.h"
#include "CandidateSet.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSet.h>
#include <AST/Passes/ASTIncludes.h>

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
                                        std::vector<QualType> &neededArgs,
                                        CandidateSet::Candidate &Cand,
                                        Statement *Caller) {
   size_t i = 0;
   std::vector<QualType> resolvedGivenArgs;
   auto &declaredArgs = Cand.Func->getArgs();

   bool foundVariadic = false;
   size_t variadicIdx = 0;

   for (auto &arg : args) {
      QualType needed;
      if (i < neededArgs.size()) {
         needed = neededArgs[i];

         auto &argDecl = declaredArgs[i];
         if (argDecl->getArgType().getTypeExpr()->isVariadicArgPackExpansion()){
            foundVariadic = true;
         }
      }

      if (needed && needed->isDependentType()) {
         if (foundVariadic) {
            needed = SP.resolveDependencies(needed, Cand.TemplateArgs, Caller,
                                            variadicIdx);
         }
         else {
            needed = SP.resolveDependencies(needed, Cand.TemplateArgs, Caller);
         }
      }

      if (arg->isContextDependent()) {
         arg->setSemanticallyChecked(false);

         SemaPass::DiagnosticScopeRAII diagnosticScopeRAII(SP);
         arg->setContextualType(needed);

         auto res = SP.visitExpr(arg);
         if (!res) {
            Cand.setHasIncompatibleArgument(i);
            return false;
         }

         result.push_back(res.get()->getExprType());
      }
      else {
         result.push_back(arg->getExprType());
      }

      if (!foundVariadic)
         ++i;
      else
         ++variadicIdx;
   }

   return true;
}

static bool resolveContextDependentArgs(SemaPass &SP,
                                        llvm::ArrayRef<Expression*> args,
                                        llvm::SmallVectorImpl<QualType> &result,
                                        FunctionType *FuncTy,
                                        CandidateSet::Candidate &Cand,
                                        Statement *Caller) {
   size_t i = 0;
   std::vector<QualType> resolvedGivenArgs;
   auto neededArgs = FuncTy->getArgTypes();

   for (auto &arg : args) {
      QualType needed;
      if (i < neededArgs.size()) {
         needed = neededArgs[i];
      }

      if (arg->isContextDependent()) {
         arg->setSemanticallyChecked(false);

         SemaPass::DiagnosticScopeRAII diagnosticScopeRAII(SP);

         auto res = SP.visitExpr(Caller, arg, needed);
         if (!res) {
            Cand.setHasIncompatibleArgument(i);
            return false;
         }

         result.push_back(res.get()->getExprType());
      }
      else {
         result.push_back(arg->getExprType());
      }

      ++i;
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

         if (!givenSelf->isObjectType()
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
      if (Cand.Func) {
         if (!SP.ensureDeclared(Cand.Func)) {
            Cand.setIsInvalid();
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
         resolve(Cand, givenArgs.drop_front(1), Conversions);
      }
      else {
         resolve(Cand, givenArgs, Conversions);
      }

      if (Cand) {
         if (foundMatch) {
            if (CandSet.BestConversionPenalty == Cand.ConversionPenalty)
               ambiguous = true;
         }

         foundMatch = true;
         CandSet.maybeUpdateBestConversionPenalty(Cand.ConversionPenalty);
         CandSet.Conversions.insert(CandSet.Conversions.end(),
                                    std::make_move_iterator(Conversions.begin()),
                                    std::make_move_iterator(Conversions.end()));
      }

      Conversions.clear();
   }

   if (ambiguous)
      CandSet.Status = CandidateSet::Ambiguous;
   else if (foundMatch)
      CandSet.Status = CandidateSet::Success;
}

void OverloadResolver::resolve(CandidateSet::Candidate &Cand,
                               llvm::ArrayRef<Expression*> givenArgs,
                               llvm::SmallVectorImpl<ConversionSequence>
                                                                 &Conversions) {
   FunctionType *FuncTy = Cand.getFunctionType();

   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   bool IsTemplate = Cand.Func && Cand.Func->isTemplate();
   if (IsTemplate) {
      SourceLocation listLoc = givenTemplateArgs.empty()
                               ? (Caller ? Caller->getSourceLoc()
                                         : SourceLocation())
                               : givenTemplateArgs.front()->getSourceLoc();

      Cand.TemplateArgs = TemplateArgList(SP, Cand.Func, givenTemplateArgs,
                                          listLoc);

      std::vector<QualType> resolvedNeededArgs = FuncTy->getArgTypes();
      if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                       resolvedNeededArgs, Cand, Caller)) {
         return;
      }

      if (Caller && Caller->getContextualType()) {
         Cand.TemplateArgs.inferFromReturnType(Caller->getContextualType(),
                                               FuncTy->getReturnType());
      }

      Cand.TemplateArgs.inferFromArgList(resolvedGivenArgs,
                                         Cand.Func->getArgs());

      auto comp = Cand.TemplateArgs.checkCompatibility();
      if (!comp)
         return Cand.setTemplateArgListFailure(comp);

      resolveTemplateArgs(resolvedNeededArgs, Cand.Func->getArgs(),
                          Cand.TemplateArgs);

      FuncTy = SP.getContext().getFunctionType(FuncTy->getReturnType(),
                                               resolvedNeededArgs,
                                               FuncTy->getRawFlags());

      Cand.setFunctionType(FuncTy);

      if (!Cand.Func->getConstraints().empty()) {
         auto idx = SP.checkConstraints(Caller, Cand.Func,
                                        Cand.TemplateArgs);

         if (idx != string::npos) {
            return Cand.setHasFailedConstraint(
               Cand.Func->getConstraints()[idx]);
         }
      }
   }
   else if (!givenTemplateArgs.empty()) {
      return Cand.setHasTooManyTemplateArgs(givenTemplateArgs.size(), 0);
   }
   else if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                         FuncTy, Cand, Caller)) {
      return;
   }

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

   if (!IsTemplate) {
      if (Cand && Cand.Func) {
         auto idx = SP.checkConstraints(Caller, Cand.Func);
         if (idx != string::npos) {
            Cand.setHasFailedConstraint(Cand.Func->getConstraints()[idx]);
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
            break;
         case CastKind::IBox:
         case CastKind::IUnbox:
         case CastKind::FPBox:
         case CastKind::FPUnbox:
            penalty += 2;
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
                                        llvm::SmallVectorImpl<ConversionSequence>
                                                                   &Conversions,
                                        size_t firstDefaultArg) {
   auto neededArgs = FuncTy->getArgTypes();
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
         else if (givenTy != needed) {
            Cand.ConversionPenalty += castPenalty(ConvSeq);
         }
      }
      else if (i < firstDefaultArg) {
         return Cand.setHasTooFewArguments(numGivenArgs, numNeededArgs);
      }

      ++i;
   }

   if (isVararg)
      isVarargCallCompatible(Cand, givenArgs, FuncTy);
}

void
OverloadResolver::isVarargCallCompatible(CandidateSet::Candidate &Cand,
                                         llvm::ArrayRef<QualType> givenArgs,
                                         FunctionType *FuncTy) {
   assert(FuncTy->isCStyleVararg() || FuncTy->isVararg());

   // cstyle vararg accepts all types
   if (FuncTy->isCStyleVararg())
      return;

   auto neededArgs = FuncTy->getArgTypes();
   auto vaTy = neededArgs.back();

   // we only need to check the remaining arguments, others have been checked
   // by isCallCompatible
   for (size_t i = neededArgs.size() - 1; i < givenArgs.size(); ++i) {
      auto& given = givenArgs[i];

      auto ConvSeq = SP.getConversionSequence(given, vaTy);
      if (!ConvSeq.isImplicit()) {
         return Cand.setHasIncompatibleArgument(i);
      }
      else if (!vaTy.isNull() && given != vaTy) {
         Cand.ConversionPenalty += castPenalty(ConvSeq);
      }
   }
}

void
OverloadResolver::resolveTemplateArgs(std::vector<QualType> &resolvedArgs,
                                      llvm::ArrayRef<FuncArgDecl*> neededArgs,
                                      TemplateArgList const& templateArgs) {
   size_t i = 0;
   for (auto &arg : neededArgs) {
      if (arg->getArgType().getTypeExpr()->isVariadicArgPackExpansion()) {
         auto typeName = arg->getArgType()->uncheckedAsGenericType()
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

      auto ty = arg->getArgType().getResolvedType();
      resolvedArgs[i] = QualType(SP.resolveDependencies(ty, templateArgs,
                                                        Caller));

      ++i;
   }
}

} // namespace ast
} // namespace cdot