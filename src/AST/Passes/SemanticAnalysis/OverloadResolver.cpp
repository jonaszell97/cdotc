//
// Created by Jonas Zell on 08.11.17.
//

#include "OverloadResolver.h"

#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"

#include "AST/Expression/StaticExpr.h"
#include "AST/Expression/TypeRef.h"

#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/StaticExpr/StaticExprEvaluator.h"

#include "Variant/Type/Type.h"

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSet.h>

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

OverloadResolver::OverloadResolver(
   SemaPass &SP,
   llvm::ArrayRef<Expression*> givenArgs,
   const std::vector<TemplateArgExpr*> &givenTemplateArgs,
   Statement *Caller)
      : SP(SP), givenArgs(givenArgs), givenTemplateArgs(givenTemplateArgs),
        Caller(Caller)
{

}

static bool resolveContextDependentArgs(SemaPass &SP,
                                        llvm::ArrayRef<Expression*> args,
                                        llvm::SmallVectorImpl<QualType> &result,
                                        FunctionType *FuncTy,
                                        CallCompatability &comp,
                                        Statement *Caller) {
   size_t i = 0;
   std::vector<QualType> resolvedGivenArgs;
   auto neededArgs = FuncTy->getArgTypes();

   for (auto &arg : args) {
      QualType needed;
      if (i < neededArgs.size())
         needed = neededArgs[i];

      if (arg->isContextDependent()) {
         SemaPass::DiagnosticScopeRAII diagnosticScopeRAII(SP);
         arg->setContextualType(needed);

         auto res = SP.visitExpr(Caller, arg);
         if (res.hadError()) {
            comp.incompatibleArg = i;
            comp.failureReason = FailureReason::IncompatibleArgument;

            return false;
         }

         result.push_back(res.getType());
      }
      else {
         result.push_back(arg->getExprType());
      }

      ++i;
   }

   return true;
}

CallCompatability OverloadResolver::checkIfViable(CallableDecl *callable)
{
   CallCompatability comp;
   auto &neededArgs = callable->getArgs();
   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                    callable->getFunctionType(), comp,
                                    Caller)) {
      return comp;
   }

   FunctionType *FuncTy = callable->getFunctionType();

   if (callable->isTemplate()) {
      TemplateArgList list(SP, callable, givenTemplateArgs);
      list.inferFromArgList(resolvedGivenArgs, neededArgs);

      comp.templateArgList = std::move(list);

      if (!comp.templateArgList.checkCompatibility()) {
         comp.failureReason = FailureReason::CouldNotInferTemplateArg;
         return comp;
      }

      auto resolvedNeededArgs = resolveTemplateArgs(neededArgs,
                                                    comp.templateArgList);

      FuncTy = SP.getContext().getFunctionType(callable->getFunctionType()
                                                       ->getReturnType(),
                                               resolvedNeededArgs,
                                               callable->getFunctionType()
                                                       ->getRawFlags());
   }

   comp.FuncTy = FuncTy;

   if (!callable->getConstraints().empty()) {
      SemaPass::ScopeGuard guard(SP, callable);
      SemaPass::TemplateArgRAII taGuard(SP, comp.templateArgList);

      for (const auto &Constraint : callable->getConstraints()) {
         auto res = SP.evaluateAsBool(Constraint);
         if (!res)
            continue;

         if (!res.getValue().getZExtValue()) {
            comp.failureReason = FailureReason::FailedConstraint;
            comp.failedConstraint = Constraint;

            return comp;
         }
      }
   }

   isCallCompatible(comp, resolvedGivenArgs, FuncTy);

   return comp;
}

CallCompatability OverloadResolver::checkIfViable(FunctionType *funcTy)
{
   CallCompatability comp;
   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                    funcTy, comp, Caller)) {
      return comp;
   }

   comp.FuncTy = funcTy;
   isCallCompatible(comp, resolvedGivenArgs, funcTy);

   return comp;
}

CallCompatability OverloadResolver::checkIfViable(EnumCaseDecl *Case)
{
   CallCompatability comp;
   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   if (!resolveContextDependentArgs(SP, givenArgs, resolvedGivenArgs,
                                    Case->getFunctionType(), comp,
                                    Caller)) {
      return comp;
   }

   comp.FuncTy = Case->getFunctionType();
   isCallCompatible(comp, resolvedGivenArgs, Case->getFunctionType());

   return comp;
}

namespace {

size_t castPenalty(SemaPass &SP, const QualType &from, const QualType &to)
{
   size_t penalty = 0;
   auto neededCast = getCastKind(SP, *from, *to);

   // only implicit casts should occur here
   for (const auto &C : neededCast.getNeededCasts()) {
      switch (C.first) {
         case CastKind::NoOp:
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
         case CastKind::BitCast:
            penalty += 2;
            break;
         case CastKind::ConversionOp:
            penalty += 3;
            break;
         default:
            penalty += 69;
            llvm_unreachable("bad cast kind!");
      }
   }

   return penalty;
}

} // anonympus namespace

void
OverloadResolver::isCallCompatible(CallCompatability &comp,
                                   llvm::ArrayRef<QualType> givenArgs,
                                   FunctionType *FuncTy,
                                   size_t firstDefaultArg) {
   comp.incompatibleArg = 0;
   comp.perfectMatch = true;

   auto neededArgs = FuncTy->getArgTypes();
   size_t numGivenArgs = givenArgs.size();
   size_t numNeededArgs = neededArgs.size();
   size_t i = 0;

   if (numGivenArgs == 0 && numNeededArgs == 0) {
      comp.perfectMatch = true;
      return;
   }

   auto isVararg = FuncTy->isCStyleVararg() || FuncTy->isVararg();
   if (numGivenArgs > numNeededArgs && !isVararg) {
      comp.failureReason = FailureReason::IncompatibleArgCount;
      return;
   }

   for (auto &neededArg : neededArgs) {
      auto& needed = neededArg;
      if (i < numGivenArgs && !givenArgs[i].isNull()) {
         auto givenTy = givenArgs[i];

         if (!SP.implicitlyCastableTo(givenTy, needed)) {
            comp.incompatibleArg = i;
            comp.failureReason = FailureReason::IncompatibleArgument;
            return;
         }
         else if (givenTy != needed) {
            comp.perfectMatch = false;
            comp.castPenalty += castPenalty(SP, givenTy, needed);
         }
      }
      else if (i < firstDefaultArg) {
         comp.incompatibleArg = i;
         comp.failureReason = FailureReason::IncompatibleArgCount;
         return;
      }

      ++i;
   }

   if (isVararg)
      isVarargCallCompatible(comp, givenArgs, FuncTy);
}

void
OverloadResolver::isVarargCallCompatible(CallCompatability &comp,
                                         llvm::ArrayRef<QualType> givenArgs,
                                         FunctionType *FuncTy) {
   assert(FuncTy->isCStyleVararg() || FuncTy->isVararg());

   // cstyle vararg accepts all types
   if (FuncTy->isCStyleVararg()) {
      return;
   }

   auto neededArgs = FuncTy->getArgTypes();
   auto vaTy = neededArgs.back();

   // we only need to check the remaining arguments, others have been checked
   // by isCallCompatible
   for (size_t i = neededArgs.size() - 1; i < givenArgs.size(); ++i) {
      auto& given = givenArgs[i];

      if (!SP.implicitlyCastableTo(given, vaTy)) {
         comp.incompatibleArg = i;
         comp.failureReason = FailureReason::IncompatibleArgument;
         return;
      }
      else if (!vaTy.isNull() && given != vaTy) {
         comp.castPenalty += castPenalty(SP, given, vaTy);
         comp.perfectMatch = false;
      }
   }
}

std::vector<QualType> OverloadResolver::resolveTemplateArgs(
                                      llvm::ArrayRef<FuncArgDecl*> neededArgs,
                                      TemplateArgList const& templateArgs) {
   std::vector<QualType> resolvedArgs;
   for (auto &arg : neededArgs) {
      if (arg->getArgType()->isVariadicArgPackExpansion()) {
         auto typeName = arg->getArgType()->getType()->asGenericType()
            ->getGenericTypeName();

         auto TA = templateArgs.getNamedArg(typeName);
         assert(TA && "missing template argument");

         auto ty = arg->getArgType()->getType();
         for (auto &VA : TA->getVariadicArgs()) {
            resolvedArgs.emplace_back(VA.getType(), ty.isLvalue(),
                                      ty.isConst());
         }

         break;
      }

      auto ty = arg->getArgType()->getType();
      resolvedArgs.emplace_back(SP.resolveDependencies(*ty, templateArgs),
                                ty.isLvalue(),
                                ty.isConst());
   }

   return resolvedArgs;
}

} // namespace ast
} // namespace cdot