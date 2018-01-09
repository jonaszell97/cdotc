//
// Created by Jonas Zell on 08.11.17.
//

#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringSet.h>

#include "OverloadResolver.h"
#include "TemplateInstantiator.h"

#include "Function.h"
#include "Record/Class.h"
#include "Record/Enum.h"

#include "../../Statement/Declaration/CallableDecl.h"
#include "../../Expression/StaticExpr.h"
#include "../StaticExpr/StaticExprEvaluator.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/TypeGroup.h"
#include "../../../Variant/Type/ArrayType.h"
#include "../../../Variant/Type/IntegerType.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {

OverloadResolver::OverloadResolver(
   SemaPass &SP,
   const std::vector<Argument> &givenArgs,
   const std::vector<TemplateArg> &givenTemplateArgs,
   SourceLocation callerLoc)
      : SP(SP), givenArgs(givenArgs), givenTemplateArgs(givenTemplateArgs),
        callerLoc(callerLoc)
{

}

CallCompatability OverloadResolver::checkIfViable(Callable *callable)
{
   CallCompatability comp;

   auto &neededArgs = callable->getArguments();
   auto resolvedGivenArgs = resolveContextual(givenArgs,
                                              neededArgs);

   // if the function is templated and no generics are given, try to infer
   // the needed template parameters, then resolve the needed arguments with
   // the given template type parameters
   // if the type substitution would fail, no error is thrown but this
   // overload will not be considered viable (like C++ SFINAE)
   auto resolvedNeededArgs = neededArgs;
   if (callable->isTemplate() || !givenTemplateArgs.empty()) {
      TemplateArgList list(SP, callable, givenTemplateArgs);
      list.inferFromArgList(resolvedGivenArgs, resolvedNeededArgs);

      comp.templateArgList = std::move(list);

      if (!comp.templateArgList.checkCompatibility()) {
         comp.failureReason = FailureReason::CouldNotInferTemplateArg;
         return comp;
      }

      resolveTemplateArgs(resolvedNeededArgs, comp.templateArgList);
   }

   if (auto Decl = callable->getDeclaration()) {
      StaticExprEvaluator Eval(SP, SP.currentClass(), callable,
                               SP.importedNamespaces(), &comp.templateArgList);

      SemaPass::ScopeGuard guard(SP, callable);
      SemaPass::TemplateArgRAII taGuard(SP, comp.templateArgList);

      for (const auto &Constraint : Decl->getConstraints()) {
         auto Inst =
            TemplateInstantiator::InstantiateStaticExpr(SP, {}, Constraint,
                                                        comp.templateArgList);

         Inst->setContextualType(IntegerType::getBoolTy());

         auto res = Eval.evaluate(Inst.get(), callerLoc);
         if (res.hadError) {
            SP.issueDiagnostics(res);
            continue;
         }

         auto ty = res.val.typeOf();
         if (ty->isObjectTy() && ty->getClassName() == "Bool") {
            res.val = res.val.getField(0);
         }

         if (!res.val.typeOf()->isIntegerTy()) {
            diag::err(diag::err_generic_error)
               << "constraint must be boolean"
               << Inst.get() << diag::term;
         }

         if (res.val.intVal == 0) {
            comp.diagnostics = std::move(res.diagnostics);
            comp.failureReason = FailureReason::FailedConstraint;
            comp.failedConstraint = Inst.get();

            return comp;
         }
      }
   }

   isCallCompatible(comp, resolvedGivenArgs, resolvedNeededArgs);

   comp.resolvedArgs = std::move(resolvedGivenArgs);
   comp.resolvedNeededArgs = std::move(resolvedNeededArgs);

   return comp;
}

CallCompatability OverloadResolver::checkIfViable(FunctionType *funcTy)
{
   CallCompatability comp;

   auto &neededArgs = funcTy->getArgTypes();
   auto resolvedGivenArgs = resolveContextual(givenArgs,
                                              neededArgs);

   isCallCompatible(comp, resolvedGivenArgs, neededArgs);

   comp.resolvedArgs = std::move(resolvedGivenArgs);
   comp.resolvedNeededArgs = std::vector<Argument>(neededArgs);

   return comp;

}

CallCompatability OverloadResolver::checkIfViable(cl::EnumCase const &Case)
{
   CallCompatability comp;
   auto &neededArgs = Case.associatedValues;

   auto resolvedGivenArgs = resolveContextual(givenArgs,
                                              neededArgs);

   isCallCompatible(comp, resolvedGivenArgs, neededArgs);

   comp.resolvedArgs = std::move(resolvedGivenArgs);
   comp.resolvedNeededArgs = std::vector<Argument>(neededArgs);

   return comp;
}

std::vector<QualType> OverloadResolver::resolveContextual(
                                    const std::vector<Argument> &givenArgs,
                                    const std::vector<Argument> &neededArgs) {
   std::vector<QualType> res;
   for (const auto &arg : givenArgs)
      res.push_back(arg.type);

   return res;
}

void
OverloadResolver::isCallCompatible(CallCompatability &comp,
                                   const std::vector<Argument> &givenArgs,
                                   const std::vector<Argument> &neededArgs) {
   return isCallCompatible(comp, resolveContextual(givenArgs, neededArgs),
                           neededArgs);
}

namespace {

size_t castPenalty(const QualType &from, const QualType &to)
{
   size_t penalty = 0;
   auto neededCast = getCastKind(*from, *to);

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
                                   const std::vector<QualType> &givenArgs,
                                   const std::vector<Argument> &neededArgs,
                                   size_t checkUntil) {
   comp.incompatibleArg = 0;

   size_t numGivenArgs = givenArgs.size();
   size_t numNeededArgs = neededArgs.size();
   size_t i = 0;
   bool perfect_match = true;

   if (numGivenArgs == 0 && numNeededArgs == 0) {
      comp.perfectMatch = true;
      return;
   }

   if (!checkUntil) {
      auto isVararg = numNeededArgs > 0 && neededArgs.back().isVararg;
      if (numGivenArgs > numNeededArgs && !isVararg) {
         comp.failureReason = FailureReason::IncompatibleArgCount;
         return;
      }

      if (isVararg) {
         return isVarargCallCompatible(comp, givenArgs, neededArgs);
      }
   }

   for (auto &neededArg : neededArgs) {
      if (checkUntil && i >= checkUntil) {
         break;
      }

      auto& needed = neededArg;
      if (i < numGivenArgs && !givenArgs[i]->isAutoTy()) {
         auto& givenTy = givenArgs.at(i);

         if (!givenTy.implicitlyCastableTo(needed.type)) {
            comp.incompatibleArg = i;
            comp.failureReason = FailureReason::IncompatibleArgument;
            return;
         }
         else if (givenTy != needed.type) {
            perfect_match = false;
            comp.castPenalty += castPenalty(givenTy, needed.type);
         }
      }
      else if (needed.defaultVal == nullptr) {
         comp.incompatibleArg = i;
         comp.failureReason = FailureReason::IncompatibleArgCount;
         return;
      }

      ++i;
   }

   comp.perfectMatch = perfect_match;
}

void
OverloadResolver::isVarargCallCompatible(
                                    CallCompatability &comp,
                                    const std::vector<QualType> &givenArgs,
                                    const std::vector<Argument> &neededArgs) {
   comp.incompatibleArg = 0;

   if (givenArgs.size() < neededArgs.size() - 1) {
      return;
   }

   // check arguments up until vararg
   size_t varargIndex = neededArgs.size() - 1;
   CallCompatability withoutVararg;
   isCallCompatible(withoutVararg, givenArgs, neededArgs, varargIndex);

   if (!withoutVararg.isCompatible()) {
      new (&comp) CallCompatability(std::move(withoutVararg));
      return;
   }

   bool cstyle = neededArgs.back().cstyleVararg;
   if (cstyle) {
      return;
   }

   bool perfectMatch = withoutVararg.perfectMatch;
   auto vaTy = neededArgs.back().type;
   for (size_t i = 0; i < givenArgs.size(); ++i) {
      auto& given = givenArgs[i];

      if (!given.implicitlyCastableTo(vaTy)) {
         comp.incompatibleArg = i;
         comp.failureReason = FailureReason::IncompatibleArgument;
         return;
      }
      else if (!vaTy->isAutoTy() && given != vaTy) {
         comp.castPenalty += castPenalty(given, vaTy);
         perfectMatch = false;
      }
   }

   comp.perfectMatch = perfectMatch;
}

void OverloadResolver::resolveTemplateArgs(
                                      std::vector<Argument> &resolvedNeededArgs,
                                      TemplateArgList const& templateArgs) {
   for (auto &arg : resolvedNeededArgs) {
      if (arg.isVariadic()) {
         auto typeName = cast<GenericType>(*arg.getType())
            ->getGenericTypeName();

         auto TA = templateArgs.getNamedArg(typeName);
         assert(TA && "missing template argument");

         auto back = std::move(resolvedNeededArgs.back());
         resolvedNeededArgs.pop_back();

         for (auto &VA : TA->getVariadicArgs()) {
            resolvedNeededArgs.emplace_back(arg.getLabel(),
                                            QualType(VA.getType(),
                                                     back.getType().isLvalue(),
                                                     back.getType().isConst()));
         }

         break;
      }

      *arg.type = SP.resolveDependencies(*arg.getType(), templateArgs);
   }
}

OverloadResolver::ArgOrder
OverloadResolver::reorderArgs(const std::vector<Argument> &givenArgs,
                              const std::vector<Argument> &neededArgs) {
   size_t i = 0;
   ArgOrder order;
   std::vector<pair<string, std::shared_ptr<Expression>>> orderedArgs;
   orderedArgs.reserve(neededArgs.size());

   // if no labels are supplied, just return the arguments in order
   bool hasLabels = false;
   auto firstLabelIndex = std::find_if(givenArgs.begin(), givenArgs.end(),
                                       [](const Argument &given) {
      return !given.label.empty();
   });

   if (firstLabelIndex == givenArgs.end()) {
      hasLabels = false;
   }

   for (const auto& arg : neededArgs) {
      auto labelIndex = std::find_if(givenArgs.begin(), givenArgs.end(),
                                     [arg](const Argument &given) {
         return given.label == arg.label;
      });

      // argument with this label was supplied
      if (hasLabels && labelIndex != givenArgs.end()) {
         auto distance = std::distance(givenArgs.begin(),labelIndex);
         orderedArgs.emplace_back(arg.label, givenArgs[distance].defaultVal);

         order.emplace_back(distance, false);
      }
         // no arg with label found
      else if (hasLabels) {
         return order;
      }
         // argument exists at position
      else if (givenArgs.size() > i) {
         orderedArgs.emplace_back(arg.label, givenArgs[i].defaultVal);
         order.emplace_back(i, false);
      }
         // default arg exists
      else if (arg.defaultVal != nullptr) {
         orderedArgs.emplace_back(arg.label, arg.defaultVal);
         order.emplace_back(i, true);
      }
         // incompatible
      else {
         return order;
      }

      ++i;
   }

   while (order.size() < givenArgs.size()) {
      order.emplace_back(i++, false);
   }

   return order;
}

} // namespace cdot